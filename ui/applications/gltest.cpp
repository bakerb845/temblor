#include <cstdio>
#include <cstdlib>
#include "gltest.hpp"

/*
ExampleApplication::ExampleApplication()
: Gtk::Application("org.gtkmm.examples.application", Gio::APPLICATION_HANDLES_OPEN)
{
}

Glib::RefPtr<ExampleApplication> ExampleApplication::create()
{
  return Glib::RefPtr<ExampleApplication>(new ExampleApplication());
}

ExampleAppWindow* ExampleApplication::create_appwindow()
{
  auto appwindow = new ExampleAppWindow();

  // Make sure that the application runs for as long this window is still open.
  add_window(*appwindow);

  // Gtk::Application::add_window() connects a signal handler to the window's
  // signal_hide(). That handler removes the window from the application.
  // If it's the last window to be removed, the application stops running.
  // Gtk::Window::set_application() does not connect a signal handler, but is
  // otherwise equivalent to Gtk::Application::add_window().

  // Delete the window when it is hidden.
  appwindow->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this,
    &ExampleApplication::on_hide_window), appwindow));

  return appwindow;
}

void ExampleApplication::on_activate()
{
  // The application has been started, so let's show a window.
  auto appwindow = create_appwindow();
  appwindow->present();
}

void ExampleApplication::on_open(const Gio::Application::type_vec_files& files,
  const Glib::ustring& )
{
  // The application has been asked to open some files,
  // so let's open a new view for each one.
  ExampleAppWindow* appwindow = nullptr;
  auto windows = get_windows();
  if (windows.size() > 0)
    appwindow = dynamic_cast<ExampleAppWindow*>(windows[0]);

  if (!appwindow)
    appwindow = create_appwindow();

  for (const auto& file : files)
    appwindow->open_file_view(file);

  appwindow->present();
}

void ExampleApplication::on_hide_window(Gtk::Window* window)
{
  delete window;
}


ExampleAppWindow::ExampleAppWindow()
: Gtk::ApplicationWindow()
{
}

void ExampleAppWindow::open_file_view(const Glib::RefPtr<Gio::File>& )
{
}
*/

TestApplicationWindow::TestApplicationWindow() :
    Gtk::ApplicationWindow(),
    mVbox(Gtk::ORIENTATION_VERTICAL),
    mCloseButton("Close", true)
{
    set_title("Test Window");
    set_default_size(512, 512);

    //TestGLArea *area = new TestGLArea();
    //add(*area);
    mCloseButton.signal_clicked().connect(sigc::mem_fun(*this, &Gtk::Window::close));
    
    mGLArea.set_hexpand(true);
    mGLArea.set_vexpand(true);
    mGLArea.set_halign(Gtk::ALIGN_FILL);
    mGLArea.set_valign(Gtk::ALIGN_FILL);
    //mGLArea.set_size_request(640, 340);
    //mGLArea.signal_unrealize().connect(sigc::mem_fun(*this, &TestGLArea::unrealize), false);
    mGLArea.set_auto_render(true);
    mVbox.add(mGLArea);

/*
    mGLArea.signal_realize().connect(sigc::mem_fun(*this, &TestGLArea::realize));
    mGLArea.signal_unrealize().connect(sigc::mem_fun(*this, &TestGLArea::unrealize), false);
    mGLArea.signal_render().connect(sigc::mem_fun(*this, &TestGLArea::render), false);
*/
    

    mVbox.add(mCloseButton);
    add(mVbox);
    show_all_children();
}

TestApplicationWindow::~TestApplicationWindow() = default;

//----------------------------------------------------------------------------//
/// Constructor
TestApplication::TestApplication() :
    Gtk::Application("org.gtkmm.examples.application")
{
}
/// Handles the on_activate command 
void TestApplication::on_activate()
{
    auto appWindow = createApplicationWindow();
    appWindow->present(); 
}
/// Get a new instance of application
Glib::RefPtr<TestApplication> TestApplication::create()
{
    return Glib::RefPtr<TestApplication> (new TestApplication());
}
/// Creates a new instance of the application window
TestApplicationWindow *TestApplication::createApplicationWindow()
{
    auto appWindow = new TestApplicationWindow();
    add_window(*appWindow);
    appWindow->signal_hide().connect(sigc::bind<Gtk::Window *>
                      (sigc::mem_fun(*this, &TestApplication::onHideWindow),
                      appWindow));
    return appWindow;
}
/// Delete event
void TestApplication::onHideWindow(Gtk::Window *window)
{
    delete window;
}

//----------------------------------------------------------------------------//
#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif
//#include <glm/glm.h>
//#include <epoxy/gl.h>
//#include <GLFW/glfw3.h>

TestGLArea::TestGLArea()
{
    set_required_version(3, 3);
/*
    set_hexpand(true);
    set_vexpand(true);
    set_auto_render(true);
    //glViewport(0, 0, 100, 100);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 

    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.6f, 0.f);
    glEnd();
*/

    
/*
    GLfloat vertex_data[] = {0.f,   0.5f,   0.f, 1.f,
                             0.5f, -0.366f, 0.f, 1.f,
                            -0.5f, -0.366f, 0.f, 1.f};
*/
    signal_realize().connect(sigc::mem_fun(*this, &TestGLArea::realize));
    signal_unrealize().connect(sigc::mem_fun(*this, &TestGLArea::unrealize), false);
    signal_render().connect(sigc::mem_fun(*this, &TestGLArea::render), false);
}

static const GLfloat vertex_data[] = {
  0.f,   0.5f,   0.f, 1.f,
  0.5f, -0.366f, 0.f, 1.f,
 -0.5f, -0.366f, 0.f, 1.f,
};

TestGLArea::~TestGLArea()
{
    //if (glfwInit()){glfwTerminate();}
}

void TestGLArea::unrealize()
{
    make_current();
    try
    {
        throw_if_error();
        //glDeleteBuffers(1, mVao);
        //glDeleteProgram(mProgram);
    }
    catch (const Gdk::GLError &gle)
    {
        fprintf(stderr, "Error unrealizing\n");
    }
}

void TestGLArea::realize()
{
    make_current();
   
/*
    if (!glfwInit())
    {
        fprintf(stderr, "gflwInit failed\n");
        return;
    }
*/

    try
    {
        throw_if_error();
        // init_buffers()
        // init_shaders()
        //enable anti-aliasing
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_POINT_SMOOTH);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    }
    catch (const Gdk::GLError &gle)
    {
        fprintf(stderr, "Error initializing\n"); 
    }
}

bool TestGLArea::render(const Glib::RefPtr<Gdk::GLContext> &context)
{
    auto allocation = get_allocation();
    int height = allocation.get_height();
    int width = allocation.get_width();
printf("%d %d\n", height, width);
    auto ratio = static_cast<float> (width)/static_cast<float> (height);
    try
    {
        throw_if_error();
        glViewport(0, 0, width, height);
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Set up the camera
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBegin(GL_POINTS);
            glColor4f(1.f, 1.f, 1.f, 1.0);
            glVertex3f(0.0, 0.0, 0.0);
            glEnd();
        }

        glFlush();
        return true;
    }
    catch (const Gdk::GLError &gle)
    {
        fprintf(stderr, "Error plotting\n");
        return false;
    } 
}

int main2(int argc, char *argv[])
{
    auto app = TestApplication::create();
    app->run(argc, argv); 
    return EXIT_SUCCESS;
}
