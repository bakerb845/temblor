#include <cstdio>
#include <cstdlib>
#include <string>
#include <cmath>
#include <GL/glew.h>
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
    if (!fs::exists(fileName))
    {
        throw std::invalid_argument("Vertex shader = " + fileName
                                  + " does not exist");
    }
    pImpl->mVertexShaderFile = fileName;
}

void GLWiggle::setFragmentShaderFileName(const std::string &fileName)
{
    pImpl->mFragmentShaderFile.clear();
    if (!fs::exists(fileName))
    {
        throw std::invalid_argument("Fragment shader = " + fileName
                                  + " does not exist");
    }
    pImpl->mFragmentShaderFile = fileName;
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
