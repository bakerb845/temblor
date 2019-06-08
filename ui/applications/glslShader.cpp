#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <fstream>
//#include <GL/glew.h>
#include <epoxy/gl.h>
#include "glslShader.hpp"

namespace
{
std::string loadFromFile(const std::string &fileName)
{   
    std::ifstream fp;
    fp.open(fileName.c_str(), std::ios_base::in);
    if (fp)
    {   
        std::string line, buffer;
        while (std::getline(fp, line))
        {   
            buffer.append(line);
            buffer.append("\r\n");
        }
        return buffer;
    }
    else 
    {   
        throw std::invalid_argument("File " + fileName + " does not exist");
    }
}
}

class GLSLShader::GLSLShaderImpl
{
public:
    GLuint mProgram = 0;
    GLuint mVertexShader = 0;
    GLuint mFragmentShader = 0;
    GLuint mGeometryShader = 0;
    std::map<std::string, GLuint> mAttributeList;
};

/// Constructor
GLSLShader::GLSLShader() :
    pImpl(std::make_unique<GLSLShaderImpl> ())
{
}

/// Delete a shader program
void GLSLShader::deleteShaderProgram() noexcept
{
    if (glIsProgram(pImpl->mProgram)){glDeleteProgram(pImpl->mProgram);}
}

/// Destructor 
GLSLShader::~GLSLShader()
{    
    if (glIsShader(pImpl->mVertexShader))
    {
        glDeleteShader(pImpl->mVertexShader);
    }
    if (glIsShader(pImpl->mFragmentShader))
    {
        glDeleteShader(pImpl->mFragmentShader);
    }
    if (glIsShader(pImpl->mGeometryShader))
    {
        glDeleteShader(pImpl->mGeometryShader);
    }
    if (glIsProgram(pImpl->mProgram))
    {
        glDeleteProgram(pImpl->mProgram);
    }
}

/// Create shader programs from file
void GLSLShader::createVertexShaderFromFile(const std::string &fileName)
{
    std::string source = loadFromFile(fileName);
    createVertexShader(source);
}

void GLSLShader::createFragmentShaderFromFile(const std::string &fileName)
{
    std::string source = loadFromFile(fileName);
    createFragmentShader(source);
}

void GLSLShader::createGeometryShaderFromFile(const std::string &fileName)
{
    std::string source = loadFromFile(fileName);
    createGeometryShader(source);
}

/// Create a vertex shader from source
void GLSLShader::createVertexShader(const std::string &source)
{
    if (glIsShader(pImpl->mVertexShader))
    {
        glDeleteShader(pImpl->mVertexShader);
    }
    // Get a shader ID
    pImpl->mVertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Bind source to shader ID
    auto pSrc = source.c_str();
    glShaderSource(pImpl->mVertexShader, 1, (const GLchar **) &pSrc, NULL);
    // Compile the source code
    glCompileShader(pImpl->mVertexShader);
    // Error check
    GLint status;
    glGetShaderiv(pImpl->mVertexShader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(pImpl->mVertexShader, GL_INFO_LOG_LENGTH,
                       &infoLogLength);
        std::string infoLog(infoLogLength, ' ');
        auto msg = reinterpret_cast<GLchar *> (infoLog.data());
        glGetProgramInfoLog(pImpl->mVertexShader, infoLogLength, NULL, msg);
        fprintf(stderr, "%s:%d: Error compiling shader: %s\n",
                __func__, __LINE__, infoLog.c_str());
        throw std::invalid_argument("Failed to create vertex shader program");
    }
}

/// Create a fragment shader from source
void GLSLShader::createFragmentShader(const std::string &source)
{
    if (glIsShader(pImpl->mFragmentShader))
    {
        glDeleteShader(pImpl->mFragmentShader);
    }
    // Get a shader ID
    pImpl->mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Bind source to shader ID
    auto pSrc = source.c_str();
    glShaderSource(pImpl->mFragmentShader, 1, (const GLchar **) &pSrc, NULL);
    // Compile the source code
    glCompileShader(pImpl->mFragmentShader);
    // Error check
    GLint status;
    glGetShaderiv(pImpl->mFragmentShader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(pImpl->mFragmentShader, GL_INFO_LOG_LENGTH,
                       &infoLogLength);
        std::string infoLog(infoLogLength, ' ');
        auto msg = reinterpret_cast<GLchar *> (infoLog.data());
        glGetProgramInfoLog(pImpl->mFragmentShader, infoLogLength, NULL, msg);
        fprintf(stderr, "%s:%d: Error compiling shader: %s\n",
                __func__, __LINE__, infoLog.c_str());
        throw std::invalid_argument("Failed to create fragment shader program");
    }
}

/// Create a geometry shader from source
void GLSLShader::createGeometryShader(const std::string &source)
{
    if (glIsShader(pImpl->mGeometryShader))
    {
        glDeleteShader(pImpl->mGeometryShader);
    }
    // Get a shader ID
    pImpl->mGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    // Bind source to shader ID
    auto pSrc = source.c_str();
    glShaderSource(pImpl->mGeometryShader, 1, (const GLchar **) &pSrc, NULL);
    // Compile the source code
    glCompileShader(pImpl->mGeometryShader);
    // Error check
    GLint status;
    glGetShaderiv(pImpl->mGeometryShader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(pImpl->mGeometryShader, GL_INFO_LOG_LENGTH,
                       &infoLogLength);
        std::string infoLog(infoLogLength, ' ');
        auto msg = reinterpret_cast<GLchar *> (infoLog.data());
        glGetProgramInfoLog(pImpl->mGeometryShader, infoLogLength, NULL, msg);
        fprintf(stderr, "%s:%d: Error compiling shader: %s\n",
                __func__, __LINE__, infoLog.c_str());
        throw std::invalid_argument("Failed to throw geometry shader program");
    }
}

/// Makes the shader program
void GLSLShader::makeShaderProgram()
{
    // Get an ID that exists for the life of this program
    pImpl->mProgram = glCreateProgram();
    // Attach shaders
    if (glIsShader(pImpl->mVertexShader))
    {
        glAttachShader(pImpl->mProgram, pImpl->mVertexShader);
    }
    if (glIsShader(pImpl->mFragmentShader))
    {
        glAttachShader(pImpl->mProgram, pImpl->mFragmentShader);
    }
    if (glIsShader(pImpl->mGeometryShader))
    {
        glAttachShader(pImpl->mProgram, pImpl->mGeometryShader);
    }
    // Link the program 
    glLinkProgram(pImpl->mProgram);
    // Check for errors
    GLint status;
    glGetProgramiv(pImpl->mProgram, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(pImpl->mProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::string infoLog(infoLogLength, ' ');
        auto msg = reinterpret_cast<GLchar *> (infoLog.data());
        glGetProgramInfoLog (pImpl->mProgram, infoLogLength, NULL, msg);
        fprintf(stderr, "%s:%d: Error compiling program: %s\n",
                __func__, __LINE__, infoLog.c_str());
        throw std::runtime_error("Failed to compile program");
    }
    // After the program is compiled flag these for deletion 
    if (glIsShader(pImpl->mVertexShader))
    {
        glDeleteShader(pImpl->mVertexShader);
    }
    if (glIsShader(pImpl->mFragmentShader))
    {
        glDeleteShader(pImpl->mFragmentShader);
    }
    if (glIsShader(pImpl->mGeometryShader))
    {
        glDeleteShader(pImpl->mGeometryShader);
    }
}

/// Add an attribute
void GLSLShader::addAttribute(const std::string &attribute)
{
    if (!glIsProgram(pImpl->mProgram))
    {
        throw std::runtime_error("Program not yet compiled");
    }
    GLint attrId = glGetAttribLocation(pImpl->mProgram, attribute.c_str());
    if (attrId == GL_INVALID_OPERATION)
    {
        fprintf(stderr, "May have failed to get attriblocation\n");
    }
    pImpl->mAttributeList[attribute] = attrId;
}

/// Get an attribute ID
uint32_t GLSLShader::operator[](const std::string &attribute)
{
    static_assert(sizeof(uint32_t) == sizeof(GLuint));
    return static_cast<uint32_t> (pImpl->mAttributeList[attribute]);
}

/// User the shader program
void GLSLShader::useProgram()
{
    if (glIsProgram(pImpl->mProgram)){glUseProgram(pImpl->mProgram);}
}

/// Unuse the shader program
void GLSLShader::releaseProgram() noexcept
{
    glUseProgram(0);
}


