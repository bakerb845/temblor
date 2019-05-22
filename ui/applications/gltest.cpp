#include <cstdio>
#include <cstdlib>
#include "gltest.hpp"

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
  const Glib::ustring& /* hint */)
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

void ExampleAppWindow::open_file_view(const Glib::RefPtr<Gio::File>& /* file */)
{
}

/*
TestApplicationWindow::TestApplicationWindow() :
    Gtk::ApplicationWindow()
{
}

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
void onHideWindow(Gtk::Window *window)
{
    delete window;
}
*/

/*
TestGLArea::TestGLArea() = default;

TestGLArea::~TestGLArea() = default;
*/

int main(int argc, char *argv[])
{
    auto app = ExampleApplication::create();
    app->run(argc, argv); 
    return EXIT_SUCCESS;
}
