#ifndef GLSLSHADER_HPP
#define GLSLSHADER_HPP 1
#include <memory>
#include <string>

class GLSLShader
{
public:
    /*! @name Constructors
     * @{
     */
    /*!  
     * @brief Constructor.
     */
    GLSLShader();
    /*! @} */

    /*!  
     * @brief Destructor.
     */
    ~GLSLShader();
    /*!
     * @brief Deletes the currently compiled shader program.
     * @sa \c makeShaderProgram()
     */
    void deleteShaderProgram() noexcept;

    /*!  
     * @brief Creates a vertex shader a shader file.
     * @param[in] fileName  The file with the source code that defines
     *                      the vertex shader.
     * @throws std::invalid_argument if the vertex shader cannot be compiled
     *         or if the file does not exist.
     */
    void createVertexShaderFromFile(const std::string &fileName);
    /*!  
     * @brief Creates a fragment shader a shader file.
     * @param[in] fileName  The file with the source code that defines
     *                      the fragment shader.
     * @throws std::invalid_argument if the fragment shader cannot be compiled
     *         or if the file does not exist.
     */
    void createFragmentShaderFromFile(const std::string &fileName);
    /*!  
     * @brief Creates a geometry shader a shader file.
     * @param[in] fileName  The file with the source code that defines
     *                      the geometry shader.
     * @throws std::invalid_argument if the geometry shader cannot be compiled
     *         or if the file does not exist.
     */
    void createGeometryShaderFromFile(const std::string &fileName);

    /*!
     * @brief Creates a vertex shader a string.
     * @param[in] source  The source code that defines the vertex shader.
     * @throws std::invalid_argument if the vertex shader cannot be compiled.
     */
    void createVertexShader(const std::string &source);
    /*! 
     * @brief Creates a fragment shader a string.
     * @param[in] source  The source code that defines the fragment shader.
     * @throws std::invalid_argument if the fragment shader cannot be compiled.
     */
    void createFragmentShader(const std::string &source);
    /*! 
     * @brief Creates a geometry shader a string.
     * @param[in] source  The source code that defines the geometry shader.
     * @throws std::invalid_argument if the geometry shader cannot be compiled.
     */
    void createGeometryShader(const std::string &source);

    /*!
     * @brief Makes the shader program.
     * @throws std::runtime_error if the vertex and fragment shader are
     *         not yet created or if there is an error linking the program.
     * @note This is called after the shaders have been created.
     * @sa \c createVertexShader() \c createFragmentShader()
     */
    void makeShaderProgram();

    /*!
     * @brief Adds an attribute so we can later link the data to the shader
     *        program.
     * @param[in] attribute   The name of the attribute to add.
     * @throws std::runtime_error if the attribute ID cannot be found. 
     */
    void addAttribute(const std::string &attribute);
    /*!
     * @brief Adds a uniform variable so we can later link the uniform
     *        data to the shader program.
     * @param[in] attribute   The name of the uniform attribute to add.
     * @throws std::runtime_error if the uniform ID cannot be found.
     */
    void addUniform(const std::string &uniform);

    /*!
     * @brief Teturns the location of the attribute.
     * @param[in] attribute  The name of the attribute.
     * @result The attribute location in the shader.
     */
    uint32_t operator[](const std::string &attribute);
    /*!
     * @brief An index that returns the location of the uniform.
     * @param[in] uniofrm  The name of the uniform.
     * @result The uniform location in the shader. 
     */
    uint32_t operator()(const std::string &uniform);
    /*!
     * @brief Sets the shader program on the GPU.
     */
    void useProgram();
    /*!
     * @brief Releases the shader program on the GPU.
     */
    void releaseProgram() noexcept;
private:
    class GLSLShaderImpl;
    std::unique_ptr<GLSLShaderImpl> pImpl;
};

#endif
