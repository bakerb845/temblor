#ifndef TEMBLOR_USERINTERFACE_MODELS_WAVEFORMGATHER_HPP
#define TEMBLOR_USERINTERFACE_MODELS_WAVEFORMGATHER_HPP 1
#include <memory>

namespace Temblor::UserInterface::Models
{
/*!
 * @brief Defines the waveform gather model.
 */
class WaveformGather
{
public:
    /*!
     * @brief Constructor
     */
    WaveformGather();

    /*!
     * @brief Destructor
     */
    ~WaveformGather();

    /*!
     * @brief Adds a waveform to the gather.
     */
    void addWaveform( );

private:
    //WaveformGather& operator=(const WaveformGather &) = delete;
    class WaveformGatherImpl;
    std::unique_ptr<WaveformGatherImpl> pImpl;
};
}
#endif
