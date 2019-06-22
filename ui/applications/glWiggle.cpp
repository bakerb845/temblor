#include <cstdio>
#include <cstdlib>
#include <string>
#include <array>
#include <cmath>
//#include <giomm/resource.h>
#include <epoxy/gl.h> //GL/glew.h>
//#include <GL/glew.h>
#if __has_include(<pstl/algorithm>)
#include <pstl/execution>
#include <pstl/algorithm>
#define USE_PSTL
#else
#include <algorithm>
#endif
#include "temblor/library/private/filesystem.hpp"
#include "glWiggle.hpp"
#include "glslShader.hpp"


namespace
{

void checkGlError(const char* op);
void checkGlError(const char* op)
{
    for (GLint error = glGetError(); error; error=glGetError())
    {
        fprintf(stderr, "After %s() glError (0x%d)%d\n",
                op, error, GL_INVALID_OPERATION);
    }
}

struct point
{
    GLfloat x = 0;
    GLfloat y = 0;
};

}

class GLWiggle::GLWiggleImpl
{
public:
    class GLSLShader mShader;
    //GLuint mVBO[2] = {0, 0};
    GLuint mVAOHandle = 0;
    GLuint mCoord2DVBO = 0;
    std::string mVertexShaderFile;
    std::string mFragmentShaderFile;
    std::vector<GLfloat> mAxis;
    std::vector<GLfloat> mTimeSeries;
    GLfloat mMaxAbs;
};

GLWiggle::GLWiggle() :
    pImpl(std::make_unique<GLWiggleImpl> ())
{
    // Load the shader programs
    setVertexShaderFileName("shaders/array.vs");
    setFragmentShaderFileName("shaders/array.fs");
    // Hook up the signals for GLArea
    signal_realize().connect(sigc::mem_fun(*this,
                             &GLWiggle::initializeRenderer));
    // Important that the unrealize signal calls our handler to clean up
    // GL resources _before_ the default unrealize handler is called (the "false")
    signal_unrealize().connect(sigc::mem_fun(*this,
                               &GLWiggle::destroyRenderer), false);
    // This does the actual drawing
    signal_render().connect(sigc::mem_fun(*this,
                            &GLWiggle::render));
}

GLWiggle::~GLWiggle() = default;

/// Sets the seismogram for plotting
void GLWiggle::setSeismogram(const int npts, const double x[])
{
    pImpl->mTimeSeries.resize(npts);
    pImpl->mAxis.resize(npts);
    GLfloat *dataPtr = pImpl->mTimeSeries.data();
    //#pragma omp simd
    pImpl->mMaxAbs = 0;
    for (auto i=0; i<npts; ++i)
    {
        dataPtr[i] = static_cast<GLfloat> (x[i]);
        pImpl->mMaxAbs = std::max(pImpl->mMaxAbs, std::abs(dataPtr[i]));
        pImpl->mAxis[i] = i/static_cast<float> (npts);
    }
    // Normalize and flip
    for (auto i=0; i<npts; ++i)
    {
        dataPtr[i] =-dataPtr[i]/pImpl->mMaxAbs;
    }
/*
#ifdef USE_PSTL
    auto minMax = std::minmax(std::execution::unseq,
                              pImpl->mTimeSeries.begin(),
                              pImpl->mTimeSeries.end());
#else
    auto minMax = std::minmax(pImpl->mTimeSeries.begin(),
                              pImpl->mTimeSeries.end());
#endif
    pImpl->mMaxAbs = std::max(std::abs(*minMax.first),
                              std::abs(*minMax.second));
*/
}

//GLWiggle::setShaders(const std::string &

void GLWiggle::setVertexShaderFileName(const std::string &fileName)
{
    pImpl->mVertexShaderFile.clear();
#ifdef TEMBLOR_USE_FILESYSTEM
    if (!fs::exists(fileName))
    {
        throw std::invalid_argument("Vertex shader = " + fileName
                                  + " does not exist");
    }
#endif
    pImpl->mVertexShaderFile = fileName;
}

void GLWiggle::setFragmentShaderFileName(const std::string &fileName)
{
    pImpl->mFragmentShaderFile.clear();
#ifdef TEMBLOR_USE_FILESYSTEM
    if (!fs::exists(fileName))
    {
        throw std::invalid_argument("Fragment shader = " + fileName
                                  + " does not exist");
    }
#endif
    pImpl->mFragmentShaderFile = fileName;
}

void GLWiggle::initializeBuffers()
{
    // Create and populate teh buffer objects
    glGenBuffers(1, &pImpl->mCoord2DVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pImpl->mCoord2DVBO);

    int len = pImpl->mTimeSeries.size();
    std::vector<point> graph(len); //std::array<point, 2000> graph;
    for (auto i=0; i<len; ++i)
    {
        float x = 2.0f*static_cast<float> (i)/static_cast<float> (len-1) - 1.0f; //100.0;
        graph[i].x = x;
        graph[i].y = 0.95*pImpl->mTimeSeries[i];
    }
    
    //glBufferData(GL_ARRAY_BUFFER, sizeof graph, graph, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, len*sizeof(point), graph.data(), GL_STATIC_DRAW);
    checkGlError("glBufferData");
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    checkGlError("glBufferClose");
    // Create and setup the vertex array object
    glGenVertexArrays(1, &pImpl->mVAOHandle);
    glBindVertexArray(pImpl->mVAOHandle);
    // Enable the vertex attribute arrays
    //glEnableVertexAttribArray(0);
    
}

/// signal_realize: nitialize the renderer
void GLWiggle::initializeRenderer()
{
    make_current();
    if (pImpl->mVertexShaderFile.length() == 0)
    {
        fprintf(stderr, "%s: Vertex shader must be set\n", __func__);
        return;
    }  
    if (pImpl->mFragmentShaderFile.length() == 0)
    {
        fprintf(stderr, "%s: Fragment shader must be set\n", __func__);
    }
    try 
    {
        throw_if_error();
        pImpl->mShader.createVertexShaderFromFile(pImpl->mVertexShaderFile);
        pImpl->mShader.createFragmentShaderFromFile(pImpl->mFragmentShaderFile);
        pImpl->mShader.makeShaderProgram();
        initializeBuffers();
/*
        glGenBuffers(2, pImpl->mVBO); //mVBOHandles);
        glGenVertexArrays(1, &pImpl->mVAO);
        glBindVertexArray(pImpl->mVAO);

        glGenBuffers(1, &pImpl->mCoord2DVBO);
        glBindBuffer(GL_ARRAY_BUFFER, pImpl->mCoord2DVBO);
*/
    }
    catch (const Gdk::GLError &gle)
    {
        fprintf(stderr, "%s: An error occured making the context\n", __func__);
        fprintf(stderr, "%s: %u-%d-%s\n", __func__, gle.domain(),
                gle.code(), gle.what().c_str());
        return;
    }
    catch (const std::exception &e) 
    {
        fprintf(stderr, "%s: Failed to initialize renderer %s\n", __func__, e.what());
    }
    // Add the attributes and uniforms
    try
    {
        pImpl->mShader.useProgram();
 
        pImpl->mShader.addAttribute("coord2d");
        checkGlError("coord2d");
        pImpl->mShader.addUniform("offset_x");
        checkGlError("offset_x");
        pImpl->mShader.addUniform("scale_x");
        checkGlError("scale_x");
        pImpl->mShader.addUniform("color");
        checkGlError("color");
        /*
        pImpl->mShader.addAttribute("xPosition\0");
        checkGlError("xpos");
        pImpl->mShader.addAttribute("yPosition\0");
        checkGlError("yPos");
        pImpl->mShader.addUniform("offset\0");
        checkGlError("offset");
        pImpl->mShader.addUniform("scale\0");
        checkGlError("scale");
        pImpl->mShader.addUniform("color\0");
        checkGlError("color");
        */
        pImpl->mShader.releaseProgram();
    }
    catch (const std::exception &e) 
    {
        fprintf(stderr, "%s: Failed to add attributes\n", __func__);
        fprintf(stderr, "%s\n", e.what());
    }
}

void GLWiggle::drawLinePlot(const float xOffset,
                            const float xScale,
                            const float color[4])
{
    // Use the program
    pImpl->mShader.useProgram();
    checkGlError("glUseProgram");

    auto offsetHandle = pImpl->mShader("offset_x");
    auto scaleXHandle = pImpl->mShader("scale_x");
    auto colorHandle = pImpl->mShader("color");

    // Bind the uniform parameters of the shader
    glUniform1f(offsetHandle, xOffset); //0.0f);
    checkGlError("offset_x");
    glUniform1f(scaleXHandle,  xScale); //1.0f);//1.0f);
    checkGlError("scale_x");
    glUniform4f(colorHandle, color[0], color[1], color[2], color[3]); //0.0f, 1.0f, 0.0f, 1.0f);
    checkGlError("color");
    // Bind the appropriate buffer object
    glBindBuffer(GL_ARRAY_BUFFER, pImpl->mCoord2DVBO);
    checkGlError("bindVBO");
    // Draw the coord2d data on this VBO
    glEnableVertexAttribArray(pImpl->mShader["coord2d"]);
    checkGlError("enableCoord2d");
    glVertexAttribPointer(pImpl->mShader["coord2d"], 2,
                          GL_FLOAT, GL_FALSE, 0, nullptr);
    checkGlError("attribPointer");
    // Do the drawing 
    int len = pImpl->mTimeSeries.size();
    glDrawArrays(GL_LINE_STRIP, 0, len);
    checkGlError("drawArrays");

    glDisableVertexAttribArray(0);
    checkGlError("disable");
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    checkGlError("close array");

    // Unuse the program
    pImpl->mShader.releaseProgram();
    checkGlError("glUnuseProgram"); 
}

/*
void GLWiggle::on_resize(const int width, const int height)
{
    make_current();
    glViewport(0, 0, width, height);
    checkGlError("glViewportResize");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    checkGlError("glClearColor");
}
*/

/// signal_render: Does the rendering
bool GLWiggle::render(const Glib::RefPtr<Gdk::GLContext> &context)
{
    make_current();
    auto allocation = get_allocation();
    auto height = allocation.get_height();
    auto width  = allocation.get_width();
    float ratio = static_cast<float> (height)/static_cast<float> (width);
    ratio = static_cast<float> (width)/static_cast<float> (height);
    if (width > height)
    {
        ratio = static_cast<float> (height)/static_cast<float> (width);
    }
    //glViewport(0, 0, width, height);
    //checkGlError("glViewport");
    //glClear(GL_COLOR_BUFFER_BIT);
    //checkGlError("glClear");
    try
    {
        throw_if_error();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        checkGlError("clear color");
        glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT);
        checkGlError("clear");

        // Prevent wrap-around
/*
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
*/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        checkGlError("pameteri");


        float xScale = 1; //ratio;
        float xOffset = 0;
        float color[4] = {0, 1, 0, 1}; //ratio, ratio, ratio, 1.0};
        drawLinePlot(xOffset, xScale, color);
/*
        // Bind the uniform parameters of the shader
        glUniform1f(pImpl->mShader("offset_x"), 0.0f);
        checkGlError("offset_x");
        glUniform1f(pImpl->mShader("scale_x"),  1.0f);//1.0f);
        checkGlError("scale_x");
        glUniform4f(pImpl->mShader("color"), 0.0f, 1.0f, 0.0f, 1.0f);
        checkGlError("color");
        // Bind the appropriate buffer object
        glBindBuffer(GL_ARRAY_BUFFER, pImpl->mCoord2DVBO);
        checkGlError("bindVBO");
        // Draw the coord2d data on this VBO
        glEnableVertexAttribArray(pImpl->mShader["coord2d"]);
        checkGlError("enableCoord2d");
        glVertexAttribPointer(pImpl->mShader["coord2d"], 2,
                              GL_FLOAT, GL_FALSE, 0, nullptr);
        checkGlError("attribPointer");
        // Do the drawing 
        int len = pImpl->mTimeSeries.size();
        glDrawArrays(GL_LINE_STRIP, 0, len);
        checkGlError("drawArrays");

        glDisableVertexAttribArray(0);
        checkGlError("disable");
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        checkGlError("close array");
*/
/*
        glBindVertexArray(pImpl->mVAOHandle); 
        checkGlError("bindVAO");
        glBindBuffer(GL_ARRAY_BUFFER, pImpl->mCoord2DVBO);
        checkGlError("bindVBO");
*/
        
        //glDrawArrays(GL_POINTS, 0, 2000);
        
        //checkGlError("drawArrays");
        //glBindVertexArray(mVAOHandle);
        //glDrawArrays(GL_LINE_STRIP, 0, 3 );
//        pImpl->mShader.releaseProgram();
        //checkGlError("glUnuseProgram");

        glFlush();
     }
     catch(const Gdk::GLError& gle)
     {
        fprintf(stderr, "%s: An error occurred in the render callback\n",
                __func__);
        fprintf(stderr, "%s: %u-%d-%s\n",
                __func__, gle.domain(), gle.code(), gle.what().c_str());
        return false;
     }   
     return true;
}

void GLWiggle::freeBuffers()
{
    glDeleteBuffers(1, &pImpl->mCoord2DVBO);
    checkGlError("Delete mCoord2dVBO");
    glDeleteBuffers(1, &pImpl->mVAOHandle); 
    checkGlError("Delete mVAOHandle");
}
/// signal_unrealize: Deletes the renderer and frees resources
void GLWiggle::destroyRenderer()
{
    make_current();
    try
    {
        throw_if_error();
        pImpl->mShader.deleteShaderProgram();
        freeBuffers();
    }
    catch (const Gdk::GLError &gle)
    {
        fprintf(stderr, "%s: An error occurred unrealizing context\n",
                __func__);
        fprintf(stderr, "%s: %u-%d-%s\n", __func__, gle.domain(),
                gle.code(), gle.what().c_str());
    } 
}
