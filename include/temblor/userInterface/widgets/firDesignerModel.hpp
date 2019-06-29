#ifndef TEMBLOR_USERINTERFACE_WIDGETS_WAVEFORMPLOTOPTIONSMODELS_HPP
#define TEMBLOR_USERINTERFACE_WIDGETS_WAVEFORMPLOTOPTIONSMODELS_HPP 1
#include <memory>
namespace Temblor::UserInterface::Widgets
{
/*!
 * @class FIRDesignerModel "firDesignModel.hpp"
 * @brief Defines the FIR window-based filter design options.
 * @copyright Ben Baker (U Utah) distributed under the MIT license.
 */
class FIRDesignerModel
{
public:
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
};
#endif
