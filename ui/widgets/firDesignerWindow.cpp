#include <cstdio>
#include <cstdlib>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/window.h>
#include "temblor/userInterface/widgets/firDesignerModel.hpp"
#include "temblor/userInterface/widgets/firDesignerWindow.hpp"

using namespace Temblor::UserInterface::Widgets;
/*
class FIRDesigner : public Gtk::Window
{
public:
    FIRDesigner();
    virtual ~FIRDesigner();
protected:
    void onCloseButtonClicked();
private:
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
*/

FIRDesignerWindow::FIRDesignerWindow()
{
    set_title("FIR Designer");
    mVerticalBox = Gtk::make_managed<Gtk::Box> (Gtk::ORIENTATION_VERTICAL);

    mBandFrame = Gtk::make_managed<Gtk::Frame> ("Filter Passband");
    mBandBox = Gtk::make_managed<Gtk::Box> (Gtk::ORIENTATION_VERTICAL);
    mLowpassRadioButton = Gtk::make_managed<Gtk::RadioButton> ("Lowpass");
    mHighpassRadioButton = Gtk::make_managed<Gtk::RadioButton> ("Highpass");
    mBandpassRadioButton = Gtk::make_managed<Gtk::RadioButton> ("Bandpass");
    mBandstopRadioButton = Gtk::make_managed<Gtk::RadioButton> ("Bandstop");
    mBandBox->pack_start(*mLowpassRadioButton);
    mBandBox->pack_start(*mHighpassRadioButton);
    mBandBox->pack_start(*mBandpassRadioButton);
    mBandBox->pack_start(*mBandstopRadioButton);
    mHighpassRadioButton->join_group(*mLowpassRadioButton);
    mBandpassRadioButton->join_group(*mLowpassRadioButton);
    mBandstopRadioButton->join_group(*mLowpassRadioButton);
    mLowpassRadioButton->set_active(true);
    mBandFrame->add(*mBandBox);

    mWindowFrame = Gtk::make_managed<Gtk::Frame> ("Window Function");
    mWindowBox = Gtk::make_managed<Gtk::Box> (Gtk::ORIENTATION_VERTICAL);
    mHanningRadioButton = Gtk::make_managed<Gtk::RadioButton> ("Hanning");
    mHammingRadioButton = Gtk::make_managed<Gtk::RadioButton> ("Hamming");
    mBlackmanRadioButton = Gtk::make_managed<Gtk::RadioButton> ("Blackman");
    mBartlettRadioButton = Gtk::make_managed<Gtk::RadioButton> ("Bartlett");
    mWindowBox->pack_start(*mHanningRadioButton);
    mWindowBox->pack_start(*mHammingRadioButton);
    mWindowBox->pack_start(*mBlackmanRadioButton);
    mWindowBox->pack_start(*mBartlettRadioButton);
    mHammingRadioButton->join_group(*mHanningRadioButton);
    mBlackmanRadioButton->join_group(*mHanningRadioButton);
    mBartlettRadioButton->join_group(*mHanningRadioButton);
    mHanningRadioButton->set_active(true);
    mWindowFrame->add(*mWindowBox);
 
    /// Give user a way out of here
    mClose = Gtk::make_managed<Gtk::Button> ("_Close");
    mClose->signal_clicked().connect(sigc::mem_fun(
                *this, &FIRDesignerWindow::onCloseButtonClicked));

  
    mVerticalBox->pack_start(*mBandFrame);
    mVerticalBox->pack_start(*mWindowFrame);
    mVerticalBox->pack_start(*mClose);
    add(*mVerticalBox);

    /// Don't let user stray from this window until done configurating
    show_all_children();
}

FIRDesignerWindow::~FIRDesignerWindow() = default;

/// Close the window
void FIRDesignerWindow::onCloseButtonClicked()
{
    close();
}
