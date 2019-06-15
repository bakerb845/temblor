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

class GLWiggle::GLWiggleImpl
{
public:
    class GLSLShader mShader;
    std::string mVertexShaderFile;
    std::string mFragmentShaderFile;
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
    GLfloat *dataPtr = pImpl->mTimeSeries.data();
    #pragma omp simd
    for (auto i=0; i<npts; ++i)
    {
        dataPtr[i] = static_cast<GLfloat> (x[i]);
    }
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
    auto glOffsetHandle = pImpl->mShader("offset");
    auto glScaleHandle = pImpl->mShader("scale");
    const void *data = pImpl->mTimeSeries.data();
    unsigned int size = pImpl->mTimeSeries.size(); 
    GLfloat offset = 0;
    GLfloat scale = pImpl->mMaxAbs;

    glVertexAttribPointer(gyPositionHandle, 1, GL_FLOAT, GL_FALSE, 0, data);
    glEnableVertexAttribArray(gyPositionHandle);
    glUniform1f(glOffsetHandle, offset);
    glUniform1f(glScaleHandle, scale);
    glDrawArrays(GL_LINE_STRIP, 0, size);  
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
    }
    catch (const std::exception &e) 
    {
        fprintf(stderr, "%s: Failed to initialize renderer\n", __func__);
    }
    // Add the attributes and uniforms
    try
    {
        pImpl->mShader.useProgram();
        pImpl->mShader.addAttribute("xPosition\0");
        pImpl->mShader.addAttribute("yPosition\0");
        pImpl->mShader.addUniform("offset\0");
        pImpl->mShader.addUniform("scale\0");
        pImpl->mShader.addUniform("color\0");
        pImpl->mShader.releaseProgram();
    }   
    catch (const std::exception &e) 
    {
        fprintf(stderr, "%s: Failed to add attributes\n", __func__);
    }
}

/// signal_render: Does the rendering
bool GLWiggle::render(const Glib::RefPtr<Gdk::GLContext> &context)
{
    //auto allocation = get_allocation();
    //auto height = allocation.get_height();
    //auto width  = allocation.get_width();
    //float ratio = static_cast<float> (width)/static_cast<float> (height);
    //glViewport(0, 0, width, height); // Shifts triangle
    try
    {
         throw_if_error();
         glClearColor(0.5, 0.5, 0.5, 1.0);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         //draw(); //_triangle();
         pImpl->mShader.useProgram();

         glLineWidth(2.0f);
         drawLinePlot();
         //glBindVertexArray(mVAOHandle);
         //glDrawArrays(GL_LINE_STRIP, 0, 3 );
         pImpl->mShader.releaseProgram();

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
