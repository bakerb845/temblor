#include <cstdio>
#include <cstdlib>
#include <string>
#include <cmath>
//#include <giomm/resource.h>
#include <epoxy/gl.h> //GL/glew.h>
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

       static void checkGlError(const char* op) {
         for (GLint error = glGetError(); error; error
         = glGetError()) {
           fprintf(stderr, "After %s() glError (0x%d)%d\n", op, error, GL_INVALID_OPERATION);
         }
}

class GLWiggle::GLWiggleImpl
{
public:
    class GLSLShader mShader;
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
    setVertexShaderFileName("shaders/wiggle.vs");
    setFragmentShaderFileName("shaders/wiggle.fs"); 
    // Hook up the signals for GLArea
    signal_realize().connect(sigc::mem_fun(*this,
                             &GLWiggle::initializeRenderer));
    // Important that the unrealize signal calls our handler to clean up
    // GL resources _before_ the default unrealize handler is called (the "false")
    signal_unrealize().connect(sigc::mem_fun(*this,
                               &GLWiggle::destroyRenderer), false);
    // This does the actual drawing
    signal_render().connect(sigc::mem_fun(*this,
                            &GLWiggle::render), false);
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
        pImpl->mAxis[i] = i;
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

void GLWiggle::drawLinePlot()
//GLfloat *data, unsigned int size,
//                            GLfloat scale, GLfloat offset)
{
    auto gyPositionHandle = pImpl->mShader["yPosition"];
    auto gxPositionHandle = pImpl->mShader["xPosition"];
    auto glOffsetHandle = pImpl->mShader("offset");
    auto glScaleHandle = pImpl->mShader("scale");
    auto gColorHandle = pImpl->mShader("color");
    const GLvoid *axis = pImpl->mAxis.data();
    const GLvoid *data = pImpl->mTimeSeries.data();
    unsigned int size = pImpl->mTimeSeries.size(); 
    GLfloat offset = 0.5;
    GLfloat scale = pImpl->mMaxAbs;
printf("%f %f %d\n", offset, scale, size);
printf("%d %d %d\n", gyPositionHandle, gxPositionHandle, glOffsetHandle);
    glUniform4f(gColorHandle, 1.0f, 0.0f, 0.0f, 1.0f);
    checkGlError("glColorHandle");

printf("bgra: %d %d\n", size, GL_BGRA);
if (axis){printf("okay\n");}
    glVertexAttribPointer(gxPositionHandle, 1, GL_FLOAT, GL_FALSE, 0, axis);
    checkGlError("glVertexAttribPointer_x");
   
    glEnableVertexAttribArray(gxPositionHandle);
    checkGlError("gxPosotionHandle");

    glVertexAttribPointer(gyPositionHandle, 1, GL_FLOAT, GL_FALSE, 0, data);
    checkGlError("glVertexAttribPointer_y");

    glEnableVertexAttribArray(gyPositionHandle);
    checkGlError("gyPositionHandle");

    glUniform1f(glOffsetHandle, offset);
    checkGlError("offset");
    glUniform1f(glScaleHandle, scale);
    checkGlError("scale");

    glDrawArrays(GL_LINE_STRIP, 0, size);
    checkGlError("draw");
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
        //initializeBuffers();
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
        pImpl->mShader.releaseProgram();
    }   
    catch (const std::exception &e) 
    {
        fprintf(stderr, "%s: Failed to add attributes\n", __func__);
        fprintf(stderr, "%s\n", e.what());
    }
}

/// signal_render: Does the rendering
bool GLWiggle::render(const Glib::RefPtr<Gdk::GLContext> &context)
{
    auto allocation = get_allocation();
    auto height = allocation.get_height();
    auto width  = allocation.get_width();
    //float ratio = static_cast<float> (width)/static_cast<float> (height);
    glViewport(0, 0, width, height); // Shifts triangle
checkGlError("viewport");
    try
    {
         throw_if_error();
         glClearColor(0.0, 0.0, 0.0, 1.0);
         checkGlError("clear color");
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         checkGlError("clear");

         //draw(); //_triangle();

         glLineWidth(0.2f);
         checkGlError("lineWidth");

         pImpl->mShader.useProgram();
         checkGlError("glUseProgram");

         drawLinePlot();
         //glBindVertexArray(mVAOHandle);
         //glDrawArrays(GL_LINE_STRIP, 0, 3 );
         pImpl->mShader.releaseProgram();
         checkGlError("glUnuseProgram");

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

/// signal_unrealize: Deletes the renderer and frees resources
void GLWiggle::destroyRenderer()
{
    make_current();
    try
    {
        throw_if_error();
        pImpl->mShader.deleteShaderProgram();
        //glDeleteBuffers(1, &pImpl->mVBOVerticesID);
        //glDeleteBuffers(1, &pImpl->mVBOIndicesID);
        //glDeleteBuffers(1, &pImpl->mVAO);
    }
    catch (const Gdk::GLError &gle)
    {
        fprintf(stderr, "%s: An error occurred unrealizing context\n",
                __func__);
        fprintf(stderr, "%s: %u-%d-%s\n", __func__, gle.domain(),
                gle.code(), gle.what().c_str());
    } 
}
