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

    /*!
     * @brief Sets the vertex and fragment shader programs.
     * @param[in] textVertexShaderFile    The name of the text rendering
     *                                    vertex shader file.
     * @param[in] textFragmentShaderFile  The name of the text rendering
     *                                    fragment shader file.
     * @throws std::invalid_argument if either file does not exist.
     */ 
    void setTextShaderFileNames(const std::string &textVertexShaderFile,
                                const std::string &textFragmentShaderFile);

    void panRight();
    void panLeft();
    void zoom(const double xPosition =-1);//bool onKeyPress(GdkEventKey *keyEvent);
    void unZoom(const double xPosition =-1);
    void drawLinePlot(const int waveform, const float xOffset, const float xScale, const float color[4]);
    void initializeBuffers();
    void freeBuffers();
    void resetToCenter();
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
protected:
    bool onScrollEvent(GdkEventScroll *event);
    bool onButtonPress(GdkEventButton *event);
    bool on_button_press_event(GdkEventButton *event) override;
private:
    class GLWiggleImpl;
    std::unique_ptr<GLWiggleImpl> pImpl;
};

#endif
