/*
#ifndef GTKMM_EXAMPLEAPPLICATION_H
#define GTKMM_EXAMPLEAPPLICATION_H

#include <gtkmm.h>

class ExampleAppWindow;

class ExampleApplication: public Gtk::Application
{
protected:
  ExampleApplication();

public:
  static Glib::RefPtr<ExampleApplication> create();

protected:
  // Override default signal handlers:
  void on_activate() override;
  void on_open(const Gio::Application::type_vec_files& files,
    const Glib::ustring& hint) override;

private:
  ExampleAppWindow* create_appwindow();
  void on_hide_window(Gtk::Window* window);
};

class ExampleAppWindow : public Gtk::ApplicationWindow
{
public:
  ExampleAppWindow();

  void open_file_view(const Glib::RefPtr<Gio::File>& file);
};

#endif
*/

#ifndef TEST_APP
#define TEST_APP

#include <gtkmm/glarea.h>
class TestGLArea : public Gtk::GLArea
{
public:
    TestGLArea(); 
    virtual ~TestGLArea() override;
    void realize();
    void unrealize();
    bool render(const Glib::RefPtr<Gdk::GLContext>& context);
};

class TestApplicationWindow;
#include <gtkmm/application.h>


class TestApplication : public Gtk::Application
{
protected:
    TestApplication();
    void on_activate() override;
public:
    static Glib::RefPtr<TestApplication> create();
private:
    TestApplicationWindow *createApplicationWindow();
    void onHideWindow(Gtk::Window *window);
};

#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
class TestApplicationWindow : public Gtk::ApplicationWindow
{
public:
    TestApplicationWindow();
    virtual ~TestApplicationWindow();
protected:
    TestGLArea mGLArea;
    Gtk::Box mVbox; 
    Gtk::Button mCloseButton;
};


#endif
