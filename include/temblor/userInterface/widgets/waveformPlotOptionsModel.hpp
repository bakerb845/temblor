#ifndef TEMBLOR_USERINTERFACE_WIDGETS_WAVEFORMPLOTOPTIONSMODELS_HPP
#define TEMBLOR_USERINTERFACE_WIDGETS_WAVEFORMPLOTOPTIONSMODELS_HPP 1
#include <memory>
namespace Temblor::UserInterface::Widgets
{
/*!
 * @class WaveformPlotOptionsModel "waveformPlotOptionsModel.hpp"
 * @brief Defines the model that controls options that go into creating the
 *        the waveform plot.
 * @copyright Ben Baker (University of Utah) distributed under the MIT license.
 */
class WaveformPlotOptionsModel
{
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Constructor.
     */
    WaveformPlotOptionsModel();
    /*!
     * @brief Copy constructor.
     * @param[in] options  The waveform plot options from which to initialize
     *                     this class.
     */
    WaveformPlotOptionsModel(const WaveformPlotOptionsModel &options);
    /*!
     * @brief Move constructor.
     * @param[in,out] options  The waveform plot options from which to 
     *                         initialize this class.
     *                         On exit, options' behavior is undefined.
     */
    WaveformPlotOptionsModel(WaveformPlotOptionsModel &&options) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Default destructor.
     */
    ~WaveformPlotOptionsModel();
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] options  The waveform plot options class to copy.
     * @result A deep copy of the waveform plot options class.
     */
    WaveformPlotOptionsModel& 
    operator=(const WaveformPlotOptionsModel &options);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] options  The waveform plot options to move.
     *                         On exit, options' behavior will be defined.
     */
    WaveformPlotOptionsModel&
    operator=(WaveformPlotOptionsModel &&options) noexcept;
    /*! @} */

    /*! @name Maximum number of visible waveforms
     * @{
     */
    /*!
     * @brief Sets the maximum number of waveforms that will be visible in a 
     *        waveform plot.  For example, if this is 8 and there are 20 
     *        waveforms then 8 waveforms will be visible in the plot.
     * @param[in] maximumNumberOfVisibleWaveforms  The maximum number of
     *                                             visible waveforms. 
     * @throws std::invalid_argument if maximumNumberOfVisibleWaveforms is not
     *         positive.
     */
    void setMaximumNumberOfVisibleWaveforms(
         const int maximumNumberOfVisibleWaveforms);
    /*! 
     * @brief Gets the maximum number of waveforms that will be visible in a 
     *        waveform plot.
     * @result The maximum number of waveforms that will be visible in a 
     *         waveform plot.
     */
    int getMaximumNumberOfVisibleWaveforms() const noexcept;
    /*! @} */

private:
    class WaveformPlotOptionsModelImpl;
    std::unique_ptr<WaveformPlotOptionsModelImpl> pImpl;
};
}
#endif
