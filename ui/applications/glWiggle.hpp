#ifndef GLWIGGLE_HPP
#define GLWIGGLE_HPP
#include <gtkmm/glarea.h>
#include <memory>

class GLSLShader;

class GLWiggle : public Gtk::GLArea
{
public:
    /*!
     * @brief Constructor.
     */
    GLWiggle();
    /*!
     * @brief Destructor.
     */
    virtual ~GLWiggle();

    /*! @name Shader Programs
     * @{
     */
    /*!
     * @brief Sets the vertex shader.
     * @param[in] fileName  The name of the file containing the vertex shader.
     * @throws std::invalid_argument if the file cannot be found.
     */
    void setVertexShaderFileName(const std::string &fileName);
    /*!
     * @brief Sets the fragment shader.
     * @param[in] fileName  The name of the file containing the fragment shader.
     * @throws std::invalid_argument if the file cannot be found.
     */
    void setFragmentShaderFileName(const std::string &fileName);
    /*! @} */

    void setSeismogram(const int npts, const double x[]);

    void drawLinePlot();
    void initializeBuffers();
    void freeBuffers();
    /*! @name Gtk Signals
     * @{
     */
    /*!
     * @brief This is called when we realize the widget.
     * @note This is connected to signal_realize().
     */
    void initializeRenderer();
    /*!
     * @brief Performs the rendering.
     * @note This is connected to signal_render().
     */
    bool render(const Glib::RefPtr<Gdk::GLContext> &context);
    /*!
     * @brief This is called when we unrealize the widget.
     * @note This is connected to signal_unrealize().
     */
    void destroyRenderer();
    /*! @} */
private:
    class GLWiggleImpl;
    std::unique_ptr<GLWiggleImpl> pImpl;
};

#endif
