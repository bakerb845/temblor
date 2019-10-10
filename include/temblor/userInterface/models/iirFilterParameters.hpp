#ifndef TEMBLOR_USERINTERFACE_MODELS_IIRFILTERPARAMETERS_HPP
#define TEMBLOR_USERINTERFACE_MODELS_IIRFILTERPARAMETERS_HPP 1
#include <memory>
#include <list>

/*!
 * @brief Defines the IIR filter analog prototype.
 */
enum class IIRFilterAnalogPrototype
{
    BUTTERWORTH, /*!< Butterworth filter design.  This has a maximally flat amplitude response.  */
    BESSEL,      /*!< Bessel filter design.  This has a maximally flat group delay. */
    CHEBYSHEV1,  /*!< Chebyshev I filter design.  This has ripples in the pass band. */
    CHEBYSHEV2   /*!< Chebyshev II filter design.  This has ripples in the stop band. */
};

/*!
 * @brief Defines the IIR filter passband.
 */
enum class IIRPassband
{
    LOWPASS,   /*!< Lowpass filter design. */
    HIGHPASS,  /*!< Highpass filter design. */
    BANDPASS,  /*!< Bandpass filter design. */
    BANDSTOP   /*!< Bandstop (notch) filter design. */
};

/*!
 * @brief Defines the IIR filter parameters.
 * @copyright Ben Baker distributed under the MIT license.
 */
class IIRFilterParameters
{
public:
    /*! @name Constructors
     */
    /*!
     * @brief Default constructor.
     */
    IIRFilterParameters();
    /*!
     * @brief Copy constructor.
     * @param[in] parameters  The IIR filter parameters from which to
     *                        initialize this class.
     */
    IIRFilterParameters(const IIRFilterParameters &parameters);
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] parameters  The IIR filter parameters.
     * @result A deep copy of the parameters.
     */
    IIRFilterParameters& operator=(const IIRFilterParameters &parameters);
    /*! @} */
   
    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Default destructor.
     */
    ~IIRFilterParameters();
    /*!
     * @brief Resets the filter to its default state.
     */
    void clear();
    /*! @} */

    /*!
     * @brief Sets the filter order.  For example, if the order is 1 then
     *        there will be 2 poles.
     * @param[in] order   The filter order.
     * @throws std::invalid_argument if the order is not positive.
     */
    void setOrder(const int order);
    /*!
     * @brief Gets the filter order.
     * @result The filter order.
     */
    int getOrder() const noexcept;

    /*!
     * @brief Sets the analog prototype.
     * @param[in] prototype  The analog prototype.
     */
    void setPrototype(const IIRFilterAnalogPrototype &prototype) noexcept;
    /*!
     * @brief Gets the analog prototype.
     * @result The analog prototype.
     */
    IIRFilterAnalogPrototype getPrototype() const noexcept;

    /*!
     * @brief Sets the filter pass band.
     * @param[in] passband  The filter pass band.
     */
    void setPassband(const IIRPassband passband) noexcept;
    /*!
     * @brief Gets the filter pass band.
     * @result The filter passband.
     */
    IIRPassband getPassband() const noexcept;
private:
    class IIRFilterParametersImpl;
    std::unique_ptr<IIRFilterParametersImpl> pImpl;
};

/*!
 * @brief Abstract interface for the observer class.
 */
class IIRFilterParametersIObserver
{
public:
    //IIRFilterParametersIObserver() = default;
    /*!
     * @brief The destructor.
     */
    virtual ~IIRFilterParametersIObserver();
    /*!
     * @brief Updates the filter order.
     */
    virtual void update(const IIRFilterParameters &parameters) = 0;//Order(const int order); // = 0;
//private:
//  class IIRFilterParameters mParameters;
};

/*!
 * @brief Abstract interface for the subject class.
 */
class IIRFilterParametersISubject
{
public:
    IIRFilterParametersISubject();
    virtual ~IIRFilterParametersISubject();
    /*!
     */
    virtual void subscribe(IIRFilterParametersIObserver *observer);
    virtual void unsubscribe(IIRFilterParametersIObserver *observer);
    virtual void notify(const IIRFilterParameters &parameters);
private:
    typedef std::list<IIRFilterParametersIObserver *> ObserverList;
    ObserverList mObservers;
};

/*!
 * @brief Defines the IIR filter parameters.
 * @copyright Ben Baker distributed under the MIT license.
 */
class IIRFilterParametersSubject : public IIRFilterParametersISubject//,
//                                   public IIRFilterParameters
{
public:
    IIRFilterParametersSubject();
    virtual ~IIRFilterParametersSubject();
    /*!
     * @brief Sets the filter order.
     * @param[in] order   The filter order.  This must be positive.
     * @throws std::invalid_argument if the order is invalid.
     */
    void setOrder(const int order);
    /*!
     * @brief Gets the filter order.
     * @result The filter order.
     */
    int getOrder() const noexcept;
    IIRFilterParameters getParameters(){return mParameters;}
protected:
    //virtual void notify();
private:
    IIRFilterParameters mParameters;
};

class IIRFilterParametersObserver : public IIRFilterParametersIObserver
{
public:
    IIRFilterParametersObserver(IIRFilterParametersSubject *subject);
    ~IIRFilterParametersObserver();
    void update(const IIRFilterParameters &parameters) override;
    int getOrder() const noexcept{return mParameters.getOrder();}
private:
    IIRFilterParameters mParameters;
    IIRFilterParametersSubject *mSubject = nullptr;
};
 
#endif
