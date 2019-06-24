/* Open GL Area
 *
 * Gtk::GLArea is a widget that allows custom drawing using OpenGL calls.
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include <gtkmm.h>
#include "glslShader.hpp"
#include <giomm/resource.h>
#include <epoxy/gl.h>
//#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "temblor/library/dataReaders/miniseed/trace.hpp"
#include "temblor/library/dataReaders/miniseed/sncl.hpp"
#include "glWiggle.hpp"

using std::cerr;
using std::endl;
using std::string;
namespace MiniSEED = Temblor::Library::DataReaders::MiniSEED;

enum {
  X_AXIS,
  Y_AXIS,
  Z_AXIS,

  N_AXIS
};

static const GLfloat vertex_data[] = {
  0.f,   0.5f,   0.f, 1.f,
  0.5f, -0.366f, 0.f, 1.f,
 -0.5f, -0.366f, 0.f, 1.f,
};

class GLInformation
{
public:
    GLInformation()
    {
        glGetIntegerv(GL_MAJOR_VERSION, &mMajor);
        glGetIntegerv(GL_MINOR_VERSION, &mMinor);
        const char *string;
        string = reinterpret_cast<const char *> (glGetString(GL_RENDERER));
        mRenderer = std::string {string};
        string = reinterpret_cast<const char *> (glGetString(GL_VENDOR));
        mVendor = std::string {string};
        string = reinterpret_cast<const char *> (glGetString(GL_VERSION));
        mVersion = std::string {string};
        string = reinterpret_cast<const char *> (glGetString(GL_SHADING_LANGUAGE_VERSION));
        mGLSLVersion = std::string {string};
    }
    ~GLInformation() = default;
    int getMajor() const noexcept
    {
        return mMajor;
    }
    int getMinor() const noexcept
    {
        return mMinor;
    }
    void print() const noexcept
    {
        fprintf(stdout, "GL Vendor            : %s\n", mVendor.c_str());
        fprintf(stdout, "GL Renderer          : %s\n", mRenderer.c_str());
        fprintf(stdout, "GL Version (string)  : %s\n", mVersion.c_str());
        fprintf(stdout, "GL Version (integer) : %d.%d\n", mMajor, mMinor);
        fprintf(stdout, "GLSL Version         : %s\n", mGLSLVersion.c_str());
    }
private:
    std::string mGLSLVersion;
    std::string mVendor;
    std::string mVersion;
    std::string mRenderer;
    GLint mMajor = 0;
    GLint mMinor = 0;
};

#include <gtkmm/menu.h>
#include <gtkmm/menuitem.h>

class PopupMenu : public Gtk::Menu
{
public:
    PopupMenu() :
        mIIRMenuItem("IIR"),
        mFIRMenuItem("FIR")
    {
        append(mIIRMenuItem);
        append(mFIRMenuItem);
        show_all();
    }
    virtual ~PopupMenu() = default;
private:
    Gtk::MenuItem mIIRMenuItem;
    Gtk::MenuItem mFIRMenuItem;
};

class TestArea : public Gtk::Window
{
public:
    TestArea()
    {
        set_title("GL Area"),
        set_default_size(1200, 800);
        // Add a container for the GLArea
        add(mVBox);
        // Add the GLArea to the vertical box container 
        mGLWiggle.set_hexpand(true);
        mGLWiggle.set_vexpand(true);
        mGLWiggle.set_auto_render(true);
        mVBox.add(mGLWiggle); 

        //class MiniSEED::Trace trace; //data/WY.YWB.EHZ.01.mseed")
        class MiniSEED::SNCL sncl;
        sncl.setNetwork("WY");
        sncl.setStation("YWB");
        sncl.setChannel("HHZ");
        sncl.setLocationCode("01");
        set_resizable(false); // OpenGL has some weird thing about resizing and aliasing
        try
        {
            mTrace.read("data/WY.YWB.EHZ.01.mseed", sncl);
        }
        catch (const std::exception &e)
        {
            fprintf(stderr, "\n", e.what());
        }   
        auto data = mTrace.getDataPointer32i();
        std::vector<double> mData(mTrace.getNumberOfSamples());
        for (auto i=0; i<mData.size(); ++i){
            mData[i] = data[i];
        }
        mGLWiggle.setSeismogram(mData.size(), mData.data());//mData.size(), mData.data());

        mPopupMenu.accelerate(*this);
        // Set the masks for mouse events
        add_events(Gdk::BUTTON_PRESS_MASK);
        add_events(Gdk::KEY_PRESS_MASK);

    signal_key_press_event().connect(
          sigc::mem_fun(*this, &TestArea::onKeyPress), false);

        // Finally display all the widgets
        show_all();
    }
    ~TestArea() = default;
protected:
    bool onKeyPress(GdkEventKey *event)
    {
printf("here\n");
        if (event->keyval == GDK_KEY_z)
        {
printf("now\n");
            mGLWiggle.zoom();
            return true;
        }
        return false;
    }
    /// Test event
    bool on_button_press_event(GdkEventButton *event)
    {
        if (event->type == GDK_BUTTON_PRESS)
        {
            if (event->button == 1)
            {
                printf("left clicked %lf, %lf\n", event->x, event->y);
                return true;
            }
            else if (event->button == 2)
            {
                printf("zoom\n");
                return true;
            }
            else if (event->button == 3)
            {
                printf("right clicked\n");
                mPopupMenu.popup(event->button, event->time);
                return true;
            }
        }
        return false;
    }
    class GLWiggle mGLWiggle;
    class MiniSEED::Trace mTrace;
    std::vector<double> mData;
    class Gtk::Box mVBox{Gtk::ORIENTATION_VERTICAL, false};
    class PopupMenu mPopupMenu;
};

//==============================================================================//

class TestArea2 : public Gtk::Window 
{
public:
    TestArea2()
    {
        // Load the file
        //class MiniSEED::Trace trace; //data/WY.YWB.EHZ.01.mseed")
        //mGLArea.setSeismogram(mTrace.data(), mTrace.size());
        // Set up main window 
        set_title("GL Area"),
        set_default_size(600, 400);
        // Add a container for the GLArea
        add(mVBox);
        // Add the GLArea to the vertical box container 
        //mGLWiggle.set_hexpand(true);
        //mGLWiggle.set_vexpand(true);
        //mGLWiggle.set_auto_render(true);
        
        mGLArea.set_hexpand(true);
        mGLArea.set_vexpand(true);
        mGLArea.set_auto_render(true); 
        mVBox.add(mGLArea);
        // Hook up the signals for the GLArea
        mGLArea.signal_realize().connect(sigc::mem_fun(*this,
                                         &TestArea2::initializeRenderer));
        // Important that the unrealize signal calls our handler to clean up
        // GL resources _before_ the default unrealize handler is called (the "false")
        mGLArea.signal_unrealize().connect(sigc::mem_fun(*this,
                                           &TestArea2::destroyRenderer), false);
        // This does the actual drawing
        mGLArea.signal_render().connect(sigc::mem_fun(*this,
                                        &TestArea2::render), false);
        // 
        //mGLArea.button_press_signal().connect(sigc::ptr_fun(*this,
        //                                      &TestArea::on_button_press_event) );
        // Make a right-click menu
        mPopupMenu.accelerate(*this);
        // Set the masks for mouse events
        add_events(Gdk::BUTTON_PRESS_MASK);
        // Finally display all the widgets
        show_all();
    }
    virtual ~TestArea2() = default;

protected:
    /// Test event
    bool on_button_press_event(GdkEventButton *event)
    {
        if (event->type == GDK_BUTTON_PRESS)
        {
            if (event->button == 1)
            {
                printf("left clicked %lf, %lf\n", event->x, event->y);
                return true;
            }
            else if (event->button == 2)
            {
                printf("zoom\n");
                return true;
            }
            else if (event->button == 3)
            {
                printf("right clicked\n");
                mPopupMenu.popup(event->button, event->time);
                return true;
            }
        }
        return false;
    }
    /// Creates the renderer
    void initializeRenderer()
    {
        mGLArea.make_current();
        try 
        {
            mGLArea.throw_if_error();
            mShader.createVertexShaderFromFile("shaders/basic.vs");
            mShader.createFragmentShaderFromFile("shaders/basic.fs");
            mShader.makeShaderProgram();
            //init_buffers();


            //mShader.addAttribute("vColor\0");
            //mShader.addAttribute("vPosition\0");
            //init_shaders();
            initializeBuffers();
        }
        catch (const Gdk::GLError &gle)
        {
            cerr << "An error occured making the context current during realize:" << endl;
            cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << endl;
        }
        catch (const std::exception &e)
        {
            fprintf(stderr, "%s: Failed to initialize renderer\n", __func__);
        }
        // Add the attributes
        try
        {
            mShader.useProgram();
            mShader.addAttribute("VertexPosition\0");
            mShader.addAttribute("VertexColor\0");
            mShader.releaseProgram();
        }
        catch (const std::exception &e)
        {
            fprintf(stderr, "Failed to add attributes\n");
        }
GLInformation info;
info.print();
    }
    /// Deletes the renderer and frees resources
    void destroyRenderer()
    {
        mGLArea.make_current();
        try
        {
            mGLArea.throw_if_error();
            mShader.deleteShaderProgram();
            glDeleteBuffers(1, &mVBOVerticesID);
            glDeleteBuffers(1, &mVBOIndicesID);
            glDeleteBuffers(1, &mVAO);
        }
        catch (const Gdk::GLError &gle)
        {
            cerr << "An error occured making the context current during unrealize" << endl;
            cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << endl;
        }
    }
    #define BUFFER_OFFSET(i) ((char *) NULL + (i))
    void draw()
    {
        GLuint positionID = mShader["VertexPosition"];
        GLuint colorID = mShader["VertexColor"];
printf("%d %d\n", positionID, colorID);

        // Load the vertex points
printf("%d\n", mNumVertices);
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,  
                        mNumVertices*3*sizeof(GLfloat),
                        mDataVertices.data());
        // Load the colors
        glBufferSubData(GL_ARRAY_BUFFER,
                        mNumVertices*3*sizeof(GLfloat),
                        mNumVertices*4*sizeof(GLfloat),
                        mDataColors.data());

        glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
        size_t offset = 3*3*sizeof(GLfloat);
        glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0,
                              BUFFER_OFFSET(offset));

        glEnableVertexAttribArray(positionID);
        glEnableVertexAttribArray(colorID);
        mShader.useProgram();

        glEnableVertexAttribArray(positionID);
        glEnableVertexAttribArray(colorID);

        mShader.releaseProgram();
    }
    bool render(const Glib::RefPtr<Gdk::GLContext> &context)
    {
        auto allocation = get_allocation();
        int height = allocation.get_height();
        int width  = allocation.get_width();
        //float ratio = static_cast<float> (width)/static_cast<float> (height);
        //glViewport(0, 0, width, height); // Shifts triangle
        try 
        {
            mGLArea.throw_if_error();
            glClearColor(0.5, 0.5, 0.5, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //draw(); //_triangle();
            mShader.useProgram();
            glBindVertexArray(mVAOHandle);
            glDrawArrays(GL_LINE_STRIP, 0, 3 );
            mShader.releaseProgram();

            glFlush();
        }
        catch(const Gdk::GLError& gle)
        {
            cerr << "An error occurred in the render callback of the GLArea" << endl;
            cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << endl;
            return false;
        }
printf("done\n");
        return true;
    }

    void initializeBuffers(const int numVertices = 3)
    {
        mDataVertices = {-0.5f, -0.5f, 0.0f,
                          0.5f, -0.5f, 0.0f,
                          0.0f,  0.5f, 0.0f};
        mDataColors = {1.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 1.0f};
        /// Create and populate the buffer objects
        glGenBuffers(2, mVBOHandles);
        GLuint positionBufferHandle = mVBOHandles[0];
        GLuint colorBufferHandle = mVBOHandles[1];
        // Populate the position buffer
        glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
        glBufferData(GL_ARRAY_BUFFER,
                     9*sizeof(float),
                     mDataVertices.data(), //positionData,
                     GL_STATIC_DRAW);
        // Populate the color buffer
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float),
                     mDataColors.data(), //colorData,
                     GL_STATIC_DRAW);

        // Create and set-up the vertex array object
        glGenVertexArrays(1, &mVAOHandle);
        glBindVertexArray(mVAOHandle);
        // Enable the vertex attribute arrays
        glEnableVertexAttribArray(0);  // Vertex position
        glEnableVertexAttribArray(1);  // Vertex color
        // Map index 0 to the position buffer
        glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        // Map index 1 to the color buffer
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
 
/*
        mNumVertices = static_cast<GLsizeiptr> (numVertices);
        //mDataVertices.resize(3*mNumVertices);
        //mDataColors.resize(4*mNumVertices);
        mDataVertices = {-0.5f, -0.5f, 0.0f,
                          0.5f, -0.5f, 0.0f,
                          0.0f,  0.5f, 0.0f};
        mDataColors = {1.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 1.0f};
        GLsizeiptr glSize = static_cast<GLsizeiptr> (mNumVertices)
                           *(4 + 3)*sizeof(GLfloat);

for (auto i=0; i<mDataVertices.size(); i++){printf("%f\n", mDataVertices[i]);}
for (auto i=0; i<mDataColors.size(); i++){printf("%f\n", mDataColors[i]);}
        // Create the "remember all"
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);
        // Create a buffer and bind it as active
        glGenBuffers(1, &mVBO); // OpenGL generates an ID
        glBindBuffer(GL_ARRAY_BUFFER, mVBO); // Make this buffer active
        // Create the buffer, but don don't load anything yet
        glBufferData(GL_ARRAY_BUFFER, glSize, NULL, GL_STATIC_DRAW);
        // Load the vertex points
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        mNumVertices*3*sizeof(GLfloat),
                        mDataVertices.data());
        // Load the colors
        glBufferSubData(GL_ARRAY_BUFFER,
                        mNumVertices*3*sizeof(GLfloat),
                        mNumVertices*3*sizeof(GLfloat),
                        mDataColors.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
*/
    }
private:

    //std::string mVertexProgram = "#version 330\nlayout(location = 0) in vec4 position;\nuniform mat4 mvp;\nvoid main() {\ngl_Position = mvp * position;\n}";
    std::string mVertexProgram = " \
#version 410\nin vec4 vPosition; in vec4 vColor; out vec4 color;\n \
void main() { color = vColor; gl_Position = vPosition; }";
    std::string mFragmentProgram = "#version 410\nout vec4 outputColor;\nvoid main() {\nfloat lerpVal = gl_FragCoord.y / 500.0f;\noutputColor = mix(vec4(0.0f, 0.85f, 0.35f, 1.0f), vec4(0.0f, 0.85f, 0.35f, 1.0f), lerpVal);\n}";

int numVertices = 3;

    class GLSLShader mShader;
    class Gtk::GLArea mGLArea;
    class Gtk::Box mVBox{Gtk::ORIENTATION_VERTICAL, false};
    class PopupMenu mPopupMenu;

    GLuint mVBOHandles[2];
    GLuint mVAOHandle;

    GLsizeiptr mNumVertices = 0;
    std::vector<GLfloat> mDataVertices;
    std::vector<GLfloat> mDataColors;
    GLuint mVBO = 0;
    
    GLuint mVBOVerticesID = 0;
    GLuint mVBOIndicesID = 0;
    GLuint mVAO = 0;
};

class Example_GLArea : public Gtk::Window
{
public:
  Example_GLArea();
  ~Example_GLArea() override;

protected:
  Gtk::Box m_VBox {Gtk::ORIENTATION_VERTICAL, false};
  Gtk::GLArea m_GLArea;
  Gtk::Box m_Controls {Gtk::ORIENTATION_VERTICAL, false};
  Gtk::Button m_Button {"Quit"};

  GLuint m_Vao {0};
  GLuint m_Buffer {0};
  GLuint m_Program {0};
  GLuint m_Mvp {0};

  std::vector<float> m_RotationAngles;

  void on_axis_value_change(int axis, const Glib::RefPtr<Gtk::Adjustment>& adj);

  void realize();
  void unrealize();
  bool render(const Glib::RefPtr<Gdk::GLContext>& context);

  Gtk::Box* create_axis_slider_box(int axis);
  void init_buffers();
  void init_shaders();
  void draw_triangle();
};

Gtk::Window* do_glarea()
{
  return new Example_GLArea();
}

Example_GLArea::Example_GLArea() : m_RotationAngles(N_AXIS, 0.0f)
{
  set_title("GL Area");
  set_default_size(400, 600);

  m_VBox.property_margin() = 12;
  m_VBox.set_spacing(6);
  add(m_VBox);

  m_GLArea.set_hexpand(true);
  m_GLArea.set_vexpand(true);
  m_GLArea.set_auto_render(true);
  //m_GLArea.can_focus(false);
  m_VBox.add(m_GLArea);

  // Connect gl area signals
  m_GLArea.signal_realize().connect(sigc::mem_fun(*this, &Example_GLArea::realize));
  // Important that the unrealize signal calls our handler to clean up
  // GL resources _before_ the default unrealize handler is called (the "false")
  m_GLArea.signal_unrealize().connect(sigc::mem_fun(*this, &Example_GLArea::unrealize), false);
  m_GLArea.signal_render().connect(sigc::mem_fun(*this, &Example_GLArea::render), false);

  m_VBox.add(m_Controls);
  m_Controls.set_hexpand(true);

  for(int i = 0 ; i < N_AXIS ; ++i)
  {
    auto sliderBox = create_axis_slider_box(i);
    m_Controls.add(*sliderBox);
  }

  m_Button.set_hexpand(true);
  m_VBox.add(m_Button);
  // Connect clicked to close of window
  m_Button.signal_clicked().connect(sigc::mem_fun(*this, &Gtk::Window::close));

show_all(); //N.B. had to change here
}

Example_GLArea::~Example_GLArea()
{
}

void Example_GLArea::on_axis_value_change(int axis, const Glib::RefPtr<Gtk::Adjustment>& adj)
{
  m_RotationAngles[axis] = adj->get_value();
  m_GLArea.queue_draw();
}

void Example_GLArea::realize()
{
  m_GLArea.make_current();
  try
  {
    m_GLArea.throw_if_error();
    init_buffers();
    init_shaders();
  }
  catch(const Gdk::GLError& gle)
  {
    cerr << "An error occured making the context current during realize:" << endl;
    cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << endl;
  }
}

void Example_GLArea::unrealize()
{
  m_GLArea.make_current();
  try
  {
    m_GLArea.throw_if_error();

    // Delete buffers and program
    glDeleteBuffers(1, &m_Vao);
    glDeleteProgram(m_Program);
  }
  catch(const Gdk::GLError& gle)
  {
    cerr << "An error occured making the context current during unrealize" << endl;
    cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << endl;
  }
}

bool Example_GLArea::render(const Glib::RefPtr<Gdk::GLContext>& /* context */)
{
  auto allocation = get_allocation();
  int height = allocation.get_height();
  int width  = allocation.get_width();
  float ratio = static_cast<float> (width)/static_cast<float> (height);
  glViewport(0, 0, width, height); // Shifts triangle
  try
  {
    m_GLArea.throw_if_error();
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);


/*
m_GLArea.make_current();
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glPointSize(100);
            glBegin(GL_POINTS);
            glColor4f(1.f, 1.f, 1.f, 1.0);
            glVertex3f(0.0, 0.0, 0.0);
            glEnd(); 
glFlush();
*/


    draw_triangle();

    glFlush();

    return true;
  }
  catch(const Gdk::GLError& gle)
  {
    cerr << "An error occurred in the render callback of the GLArea" << endl;
    cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << endl;
    return false;
  }
}

Gtk::Box* Example_GLArea::create_axis_slider_box(int axis)
{
  auto box = Gtk::manage(new Gtk::Box{Gtk::ORIENTATION_HORIZONTAL, false});

  const char* text;

  switch(axis)
  {
    case X_AXIS:
    {
      text = "X axis";
      break;
    }
    case Y_AXIS:
    {
      text = "Y axis";
      break;
    }
    case Z_AXIS:
    {
      text = "Z axis";
      break;
    }
    default:
    {
      g_assert_not_reached();
    }
  }

  auto label = Gtk::manage(new Gtk::Label{text});
  box->add(*label);
  label->show();

  auto adj = Gtk::Adjustment::create(0.0, 0.0, 360.0, 1.0, 12.0, 0.0);

  adj->signal_value_changed().connect(
    sigc::bind(sigc::mem_fun(*this, &Example_GLArea::on_axis_value_change), axis, adj)
                                      );
  auto slider = Gtk::manage(new Gtk::Scale{adj, Gtk::ORIENTATION_HORIZONTAL});
  box->add(*slider);
  slider->set_hexpand(true);
  slider->show();

  box->show();

  return box;
}

void Example_GLArea::init_buffers()
{
  glGenVertexArrays(1, &m_Vao);
  glBindVertexArray(m_Vao);

  glGenBuffers(1, &m_Buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static GLuint create_shader(int type, const char *src)
{
  auto shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  int status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if(status == GL_FALSE)
  {
    int log_len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

    string log_space(log_len+1, ' ');
    glGetShaderInfoLog(shader, log_len, nullptr, (GLchar*)log_space.c_str());

    cerr << "Compile failure in " <<
      (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
      " shader: " << log_space << endl;

    glDeleteShader(shader);

    return 0;
  }

  return shader;
}

void Example_GLArea::init_shaders()
{
  std::string vertex_program("#version 330\nlayout(location = 0) in vec4 position;\nuniform mat4 mvp;\nvoid main() {\ngl_Position = mvp * position;\n}");
 //std::string vertex_program("#version 330\nin vec3 position;\nin vec3 color;\nuniform mat4 mvp;\nsmooth out vec4 vertexColor;\nvoid main() {\ngl_Position = mvp * vec4(position, 1.0);\nvertexColor = vec4(color, 1.0);\n}");
/*
  auto vshader_bytes = Gio::Resource::lookup_data_global("/glarea/glarea-vertex.glsl");
  if(!vshader_bytes)
  {
    cerr << "Failed fetching vertex shader resource" << endl;
    m_Program = 0;
    return;
  }
  gsize vshader_size {vshader_bytes->get_size()};
*/
  auto vertex = create_shader(GL_VERTEX_SHADER, vertex_program.c_str());
//                         (const char*)vshader_bytes->get_data(vshader_size));

  if(vertex == 0)
  {
    m_Program = 0;
    return;
  }

  std::string fragment_program("#version 330\nout vec4 outputColor;\nvoid main() {\nfloat lerpVal = gl_FragCoord.y / 500.0f;\noutputColor = mix(vec4(0.0f, 0.85f, 0.35f, 1.0f), vec4(0.0f, 0.85f, 0.35f, 1.0f), lerpVal);\n}");
/*
  auto fshader_bytes = Gio::Resource::lookup_data_global("/glarea/glarea-fragment.glsl");
  if(!fshader_bytes)
  {
    cerr << "Failed fetching fragment shader resource" << endl;
    glDeleteShader(vertex);
    m_Program = 0;
    return;
  }
  gsize fshader_size {fshader_bytes->get_size()};
*/
  auto fragment = create_shader(GL_FRAGMENT_SHADER, fragment_program.c_str());
//                           (const char*)fshader_bytes->get_data(fshader_size));

  if(fragment == 0)
  {
    glDeleteShader(vertex);
    m_Program = 0;
    return;
  }

  m_Program = glCreateProgram();
  glAttachShader(m_Program, vertex);
  glAttachShader(m_Program, fragment);

  glLinkProgram(m_Program);

  int status;
  glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
  if(status == GL_FALSE)
  {
    int log_len;
    glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &log_len);

    string log_space(log_len+1, ' ');
    glGetProgramInfoLog(m_Program, log_len, nullptr, (GLchar*)log_space.c_str());

    cerr << "Linking failure: " << log_space << endl;

    glDeleteProgram(m_Program);
    m_Program = 0;
  }
  else
  {
    /* Get the location of the "mvp" uniform */
    m_Mvp = glGetUniformLocation(m_Program, "mvp");

    glDetachShader(m_Program, vertex);
    glDetachShader(m_Program, fragment);
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

static void compute_mvp(float *res,
                        float phi,
                        float theta,
                        float psi)
{
  float x       {phi * ((float)G_PI / 180.f)};
  float y       {theta * ((float)G_PI / 180.f)};
  float z       {psi * ((float)G_PI / 180.f)};
  float c1      {cosf (x)};
  float s1      {sinf (x)};
  float c2      {cosf (y)};
  float s2      {sinf (y)};
  float c3      {cosf (z)};
  float s3      {sinf (z)};
  float c3c2    {c3 * c2};
  float s3c1    {s3 * c1};
  float c3s2s1  {c3 * s2 * s1};
  float s3s1    {s3 * s1};
  float c3s2c1  {c3 * s2 * c1};
  float s3c2    {s3 * c2};
  float c3c1    {c3 * c1};
  float s3s2s1  {s3 * s2 * s1};
  float c3s1    {c3 * s1};
  float s3s2c1  {s3 * s2 * c1};
  float c2s1    {c2 * s1};
  float c2c1    {c2 * c1};

  /* apply all three rotations using the three matrices:
   *
   * ⎡  c3 s3 0 ⎤ ⎡ c2  0 -s2 ⎤ ⎡ 1   0  0 ⎤
   * ⎢ -s3 c3 0 ⎥ ⎢  0  1   0 ⎥ ⎢ 0  c1 s1 ⎥
w
   * ⎣   0  0 1 ⎦ ⎣ s2  0  c2 ⎦ ⎣ 0 -s1 c1 ⎦
   */
  res[0] = c3c2;  res[4] = s3c1 + c3s2s1;  res[8] = s3s1 - c3s2c1; res[12] = 0.f;
  res[1] = -s3c2; res[5] = c3c1 - s3s2s1;  res[9] = c3s1 + s3s2c1; res[13] = 0.f;
  res[2] = s2;    res[6] = -c2s1;         res[10] = c2c1;          res[14] = 0.f;
  res[3] = 0.f;   res[7] = 0.f;           res[11] = 0.f;           res[15] = 1.f;
}

void Example_GLArea::draw_triangle()
{
  float mvp[16];

  compute_mvp(mvp,
              0, //m_RotationAngles[X_AXIS],
              0, //m_RotationAngles[Y_AXIS],
              m_RotationAngles[Z_AXIS]);

  glUseProgram(m_Program);

  glUniformMatrix4fv(m_Mvp, 1, GL_FALSE, &mvp[0]);

  glBindBuffer(GL_ARRAY_BUFFER, m_Vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
}

#include <gtkmm/main.h>

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create();

  //Example_GLArea window; //DemoWindow window;
  TestArea window;

  return app->run(window, argc, argv);
}
