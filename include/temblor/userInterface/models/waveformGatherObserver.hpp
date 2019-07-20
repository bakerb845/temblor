#ifndef TEMBLOR_USERINTERFACE_MODELS_WAVEFORMGATHER_HPP
#define TEMBLOR_USERINTERFACE_MODELS_WAVEFORMGATHER_HPP 1
#include <memory>
namespace Temblor::UserInterface::Models
{
/*!
 * @brief Defines the abstract interface for the waveform gather observer.
 */
class WaveformGatherIObserver
{
public:
    /*!
     * @brief Destructor.
     */
    virtual ~WaveformGatherIObserver();
    /*!
     * @brief Defines the behavior whenever the mdoel is updated.
     */
    virtual void update(const WaveformGather &gather) = 0;
};
/*!
 * @brief Defines the abstract interface for the waveform gather subject.
 */
class WaveformGatherISubject
{
public:
    /*!
     * @brief Constructor.
     */
    WaveformGatherISubject();
    /*!
     * @brief Destructor.
     */
    virtual ~WaveformGatherISubject();

    /*!
     * @brief Allows an observer to subscribe to the subject.
     * @param[in] observer  The observer class subscribing to the subject.
     */
    virtual void subscribe(WaveformGatherIObserver *observer);
    /*!
     * @brief Allows an observer to unsusbscribe from the subject.
     * @param[in] observer  The observer class to unsubscribe.
     */
    virtual void unsubscribe(WaveformGatherIObserver *observer);
    /*!
     * @brief When the subject is modified this will notify all subscribers.
     * @param[in] gather   The waveform gather which has been updated.
     */
    virtual void notify(const WaveformGather &gather); 
private:
    typedef std::list<WaveformGatherIObserver *> ObserverList;
    ObserverList mObservers;
};
/*!
 * @brief Defines the waveform gather subject.
 */
class WaveformGatherSubject : public WaveformGatherISubject
{
public:
    /*!
     * @brief Constructor.
     */
    WaveformGatherSubject();
    /*!
     * @brief Destructor.
     */
    virtual ~WaveformGatherSubject();

    /*!
     * @brief Get maximum sampling rate.
     * @result The maximum sampling rate in Hz.
     * @throws std::runtime_error if there are no waveforms.
     * @sa getNumberOfWaveforms()
     */
    double getMaximumSamplingRate() const;
    /*!
     * @brief Gets the number of waveforms.
     * @result The number of waveforms in the gather.
     */
    int getNumberOfWaveforms() const noexcept;

    /*!
     * @brief Appends a waveform.
     * @param[in] waveform   The waveform to append to the gather.
     * @throws std::invalid_argument if the waveform does not have a SNCL,
     *         has an invalid sampling rate, or no data points.
     * @note If the waveform exists then it will be overwritten.
     */
    void addWaveform( );
    //void addMultiChannelWaveform(const int nComponents, );

    
private:
    std::unique_ptr<WaveformGather> mGather;
};

}
#endif
