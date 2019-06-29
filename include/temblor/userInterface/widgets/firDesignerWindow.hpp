#ifndef TEMBLOR_USERINTERFACE_WIDGETS_FIRDESIGNERWINDOW_HPP
#define TEMBLOR_USERINTERFACE_WIDGETS_FIRDESIGNERWINDOW_HPP 1
#include <gtkmm/window.h>
namespace Temblor::UserInterface::Widgets
{
class FIRDesignerWindow : public Gtk::Window
{
public:
    /*!
     * @brief Constructor.
     */
    FIRDesignerWindow();
    /*!
     * @brief Destructor.
     */
    virtual ~FIRDesignerWindow();
protected:
    /*!
     * @brief Closes the window.
     */
    void onCloseButtonClicked();
private:
    // Container to hold everything
    Gtk::Box *mVerticalBox;
    // Filter band
    Gtk::Frame *mBandFrame;
    Gtk::Box *mBandBox;
    Gtk::RadioButton *mLowpassRadioButton;
    Gtk::RadioButton *mHighpassRadioButton;
    Gtk::RadioButton *mBandpassRadioButton;
    Gtk::RadioButton *mBandstopRadioButton;
    // Window-design
    Gtk::Frame *mWindowFrame;
    Gtk::Box *mWindowBox;
    Gtk::RadioButton *mHanningRadioButton;
    Gtk::RadioButton *mHammingRadioButton;
    Gtk::RadioButton *mBlackmanRadioButton;
    Gtk::RadioButton *mBartlettRadioButton;

    // Close the window
    Gtk::Button *mClose;
};
}
#endif
