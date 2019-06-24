#include <cstdio>
#include <cstdlib>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/radiobutton.h>

#include <gtkmm/window.h>

  /*! 
     * @brief Defines the filter passband type.
     */
    enum class BandType
    {   
        LOWPASS,  /*!< Lowpass filter. */
        HIGHPASS, /*!< Highpass filter. */
        BANDPASS, /*!< Bandpass filter. */
        BANDSTOP  /*!< Bandstop (notch) filter. */
    };  
    /*! 
     * @brief Defines the window to use in the FIR design.
     */
    enum class WindowType
    {   
        HAMMING,      /*!< Hamming window design. */
        HANNING,      /*!< Hanning window design. */
        BLACKMAN_OPT, /*!< Optimal Blackman window design. */
        BARTLETT      /*!< Bartlett window design. */
    }; 

class FIRDesignerModel
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Constructor.
     */
    FIRDesignerModel();
    /*!
     * @brief Copy constructor.
     * @param[in] firModel  The FIR designer model class to initialize from.
     */
    FIRDesignerModel(const FIRDesignerModel &firModel);
    /*!
     * @brief Move constructor.
     * @param[in,out] firModel  The FIR model to initialize from.  firModel's
     *                          behavior is undefined on exit.
     */
    FIRDesignerModel(FIRDesignerModel &&firModel) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy operator.
     * @param[in] firModel  The FIR designer model to copy.
     * @result A deep copy of the FIR designer model class.
     */
    FIRDesignerModel& operator=(const FIRDesignerModel &firModel);
    /*!
     * @brief Move operator.
     * @param[in,out] firModel  The FIR designer model whose memory is moved.
     *                          On exit, firModel's behavior is undefined.
     * @result The moved variant of firModel.
     */
    FIRDesignerModel& operator=(FIRDesignerModel &&firModel) noexcept;

    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor
     */
    ~FIRDesignerModel();
    /*! @} */

    /*! @name Sampling Rate
     * @{
     */
    /*!
     * @brief Sets the sampling rate.
     * @param[in] samplingRate  The sampling rate in Hz.
     * @throws std::invalid_argument if sampling rate is not positive.
     * @note This will invalidate the critical frequencies.
     */
    void setSamplingRate(const double samplingRate); 
    /*!
     * @brief Gets the sampling rate.
     * @result The sampling rate in Hz.
     */
    double getSamplingRate() const noexcept;
    /*!
     * @brief Gets the Nyquist frequency.
     * @result The Nyquist frequency in Hz.
     */
    double getNyquistFrequency() const noexcept;
    /*! @} */

    /*! @name Filter Length
     * @{
     */
    /*!
     * @brief Sets the number of taps in the FIR filter.
     * @param[in] filterLength   The number of FIR filter taps.
     * @throws std::invalid_argument if filterLength is not positive.
     */
    void setFilterLength(const int filterLength);
    /*!
     * @brief Gets the number of filter taps.
     * @result The length of the FIR filter.
     */
    int getFilterLength() const noexcept;
    /*! @} */

    /*!
     * @brief Sets the low critical frequency for lowpass and highpass filters..
     * @param[in] fc  The critical frequency in Hz.
     * @throws std::invalid_argument if fcLow is not in the
     *         range (0, \c getHighNyquistFrequency) or the filter band 
     *         is not lowpass or highpass.
     */
    void setCriticalFrequency(const double fc);
    /*!
     * @brief Sets the critical frequencies for bandpass and bandstop filters.
     * @param[in] fc  The low and high critical frequency in Hz.
     * @throws std::invalid_argument if fc.first is not positive or fc.first
     *         is greater than or equal to fc.second,
     *         or if fc.second is greater than or equal to 
     *         \c getNyquistFrequency(), or
     *         the filter bnad type is not bandpass or bandstop.
     */
    void setCriticalFrequencies(const std::pair<double, double> fc);
    /*!
     * @brief Gets the critical frequency.
     * @result The critical frequency in Hz for lowpass and highpass filters,
     * @throws std::runtime_error if this was not set or the filter band
     *         type is bandpass or bandstop.
     */
    double getCriticalFrequency() const;
    /*!
     * @brief Gets the critical frequencies for bandpass and bandstop filter
      *       deisgn
     * @result The critical frequencies in Hz.
     * @throws std::runtime_error if this was not set or the filter band type
     *         is not bandpass or bandstop.
     */
    std::pair<double, double> getCriticalFrequencies() const;

    /*! @name Passband
     * @{
     */
    /*!
     * @brief Sets the filter band type.
     * @param[in] bandType  The filter band type.
     * @note This will invalidate the critical frequencies.
     */
    void setBandType(const BandType bandType) noexcept;
    /*!
     * @brief Gets the filter band type.
     * @result The filter band type.
     */
    BandType getBandType() const noexcept;
    /*! @} */

    /*! @name Window Type
     * @{
     */
    /*!
     * @brief Sets the window type to use in design.
     * @param[in] windowType  The window type to use in design.
     */
    void setWindowType(const WindowType windowType) noexcept;
    /*!
     * @brief Gets the window type.
     * @result The window type to use in design.
     */
    WindowType getWindowType() const noexcept;
    /*! @} */
    

private:
    class FIRDesignerModelImpl;
    std::unique_ptr<FIRDesignerModelImpl> pImpl;

};

class FIRDesignerModel::FIRDesignerModelImpl
{
public:
    double mSamplingRate = 1;
    double mLowCriticalFrequency =-1.0;
    double mHighCriticalFrequency =-1.0;
    int mFilterLength = 100;
    BandType mBandType = BandType::LOWPASS;
    WindowType mWindowType = WindowType::HANNING;
};

FIRDesignerModel::FIRDesignerModel() :
    pImpl(std::make_unique<FIRDesignerModelImpl> ())
{
}

FIRDesignerModel::FIRDesignerModel(const FIRDesignerModel &firModel)
{
    *this = firModel;
}

FIRDesignerModel::FIRDesignerModel(FIRDesignerModel &&firModel) noexcept
{
    *this = std::move(firModel);
}

FIRDesignerModel& FIRDesignerModel::operator=(const FIRDesignerModel &firModel)
{
    if (&firModel == this){return *this;}
    pImpl = std::make_unique<FIRDesignerModelImpl> (*firModel.pImpl);
    return *this;
}

FIRDesignerModel&
FIRDesignerModel::operator=(FIRDesignerModel &&firModel) noexcept
{
    if (&firModel == this){return *this;}
    pImpl = std::move(firModel.pImpl);
    return *this;
}


FIRDesignerModel::~FIRDesignerModel() = default;

void FIRDesignerModel::setSamplingRate(const double samplingRate)
{
    if (samplingRate <= 0)
    {   
        throw std::invalid_argument("Filter length = " +
                                    std::to_string(samplingRate) +
                                    " must be positive");
    }
    pImpl->mLowCriticalFrequency =-1;
    pImpl->mHighCriticalFrequency =-1;
    pImpl->mSamplingRate = samplingRate;
}

double FIRDesignerModel::getSamplingRate() const noexcept
{
    return pImpl->mSamplingRate;
}

double FIRDesignerModel::getNyquistFrequency() const noexcept
{
    return pImpl->mSamplingRate/2;
}

void FIRDesignerModel::setFilterLength(const int filterLength)
{
    if (filterLength < 1)
    {
        throw std::invalid_argument("Filter length = " +
                                    std::to_string(filterLength) +
                                    " must be positive");
    }
    pImpl->mFilterLength = filterLength;
}

int FIRDesignerModel::getFilterLength() const noexcept
{
    return pImpl->mFilterLength;
}

void FIRDesignerModel::setBandType(const BandType bandType) noexcept
{
    pImpl->mLowCriticalFrequency = -1;
    pImpl->mHighCriticalFrequency = -1;
    pImpl->mBandType = bandType;
}

BandType FIRDesignerModel::getBandType() const noexcept
{
    return pImpl->mBandType;
}

void FIRDesignerModel::setWindowType(const WindowType windowType) noexcept
{
    pImpl->mWindowType = windowType;
}

WindowType FIRDesignerModel::getWindowType() const noexcept
{
    return pImpl->mWindowType;
}

void FIRDesignerModel::setCriticalFrequency(const double fc)
{
    if (pImpl->mBandType == BandType::BANDPASS || pImpl->mBandType == BandType::BANDSTOP)
    {
        throw std::invalid_argument("This is for lowpass/highpass filters");
    }
    if (fc <= 0 || fc >= getNyquistFrequency())
    {
        throw std::invalid_argument("fc = " + std::to_string(fc) + 
                                    "must be in range (0," +
                                    std::to_string(getNyquistFrequency()) +
                                    ")");
    }
    pImpl->mLowCriticalFrequency = fc;
}

double FIRDesignerModel::getCriticalFrequency() const
{
    if (pImpl->mBandType == BandType::BANDPASS ||
        pImpl->mBandType == BandType::BANDSTOP)
    {   
        throw std::runtime_error("This is for lowpass/highpass filters");
    }
    if (pImpl->mLowCriticalFrequency < 0)
    {
        throw std::runtime_error("setCriticalFrequency never called");
    }
    return pImpl->mLowCriticalFrequency;
}

void FIRDesignerModel::setCriticalFrequencies(const std::pair<double, double> fc)
{
    if (pImpl->mBandType == BandType::LOWPASS || 
        pImpl->mBandType == BandType::HIGHPASS)
    {
        throw std::invalid_argument("This is for bandpass/bandstop filters");
    }
    if (fc.second >= getNyquistFrequency())
    {
        throw std::invalid_argument("fc.second = " +
                                    std::to_string(fc.second) +
                                    " must be less than " +
                                    std::to_string(getNyquistFrequency()));
    }
    if (fc.first <= 0 || fc.first >= fc.second)
    {
        throw std::invalid_argument("fc.first = " + 
                                    std::to_string(fc.first) +
                                    " must be in range (0," + 
                                    std::to_string(fc.second) + ")");
    }
    pImpl->mLowCriticalFrequency = fc.first;
    pImpl->mHighCriticalFrequency = fc.second;
}

std::pair<double, double>
FIRDesignerModel::getCriticalFrequencies() const
{
    if (pImpl->mBandType == BandType::LOWPASS ||
        pImpl->mBandType == BandType::HIGHPASS)
    {
        throw std::runtime_error("This is for bandpass/bandstop filters");
    } 
    if (pImpl->mLowCriticalFrequency < 0)
    {
        throw std::runtime_error("setCriticalFrequencies never called");
    }
    return std::pair<double, double> (pImpl->mLowCriticalFrequency,
                                      pImpl->mHighCriticalFrequency);
}

class FIRDesigner : public Gtk::Window
{
public:
    /*!
     * @brief Constructor.
     */
    FIRDesigner();
    /*!
     * @brief Destructor.
     */
    virtual ~FIRDesigner();
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

FIRDesigner::FIRDesigner()
{
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
    mHanningRadioButton->set_active(true);
    mWindowFrame->add(*mWindowBox);
     
}

FIRDesigner::~FIRDesigner() = default;
