#include <cstdio>
#include <cstdlib>
#include <string>
#include "iirFilterParameters.hpp"

class IIRFilterParameters::IIRFilterParametersImpl
{
public:
    int mOrder = 1;
    IIRFilterAnalogPrototype mPrototype = IIRFilterAnalogPrototype::BESSEL;
    IIRPassband mPassband = IIRPassband::LOWPASS;
};

/// Constructors
IIRFilterParameters::IIRFilterParameters() :
    pImpl(std::make_unique<IIRFilterParametersImpl> ())
{
}

IIRFilterParameters::IIRFilterParameters(const IIRFilterParameters &parameters)
{
    *this = parameters;
}

/// Operators
IIRFilterParameters&
IIRFilterParameters::operator=(const IIRFilterParameters &parameters)
{
    if (&parameters == this){return *this;}
    pImpl = std::make_unique<IIRFilterParametersImpl> (*parameters.pImpl);
    return *this;
}

/// Destructors
IIRFilterParameters::~IIRFilterParameters() = default;

void IIRFilterParameters::clear()
{
    pImpl->mOrder = 1;
    pImpl->mPrototype = IIRFilterAnalogPrototype::BESSEL;
    pImpl->mPassband = IIRPassband::LOWPASS;
}
 
/// Order
void IIRFilterParameters::setOrder(const int order)
{
    if (order < 0)
    {
        throw std::invalid_argument("order = "
                                  + std::to_string(order)
                                  + " must be positive");
    }
    pImpl->mOrder = order;
}

int IIRFilterParameters::getOrder() const noexcept
{
    return pImpl->mOrder;
}

/// Prototype
void IIRFilterParameters::setPrototype(
    const IIRFilterAnalogPrototype &prototype) noexcept
{
    pImpl->mPrototype = prototype;
}

IIRFilterAnalogPrototype IIRFilterParameters::getPrototype() const noexcept
{
    return pImpl->mPrototype;
}

/// Passband
void IIRFilterParameters::setPassband(const IIRPassband passband) noexcept
{
    pImpl->mPassband = passband;
}

IIRPassband IIRFilterParameters::getPassband() const noexcept
{
    return pImpl->mPassband;
}
//----------------------------------------------------------------------------//

IIRFilterParametersIObserver::~IIRFilterParametersIObserver() = default;

/*
void IIRFilterParametersIObserver::update() //const int order)
{
//    mParameters.setOrder(order);
}
*/

//----------------------------------------------------------------------------//

IIRFilterParametersISubject::IIRFilterParametersISubject() = default;

IIRFilterParametersISubject::~IIRFilterParametersISubject() = default;

void IIRFilterParametersISubject::subscribe(
    IIRFilterParametersIObserver *observer)
{
    if (observer)
    {
        mObservers.push_back(observer);
    }
}

void IIRFilterParametersISubject::unsubscribe(
    IIRFilterParametersIObserver *observer)
{
    auto position = mObservers.begin();
    for (auto it = mObservers.begin(); it != mObservers.end(); ++it)
    {
        if (*it == observer)
        {
            mObservers.erase(it);
        }
    }
}

void IIRFilterParametersISubject::notify(const IIRFilterParameters &parameters)
{
    for (auto thisObserver : mObservers)
    {
        thisObserver->update(parameters);
    }
}

//----------------------------------------------------------------------------//

IIRFilterParametersSubject::IIRFilterParametersSubject() = default;

IIRFilterParametersSubject::~IIRFilterParametersSubject() = default;

void IIRFilterParametersSubject::setOrder(const int order)
{
    if (order < 0)
    {
        throw std::invalid_argument("order = " + std::to_string(order)
                                  + " must be positive\n");
    }
    mParameters.setOrder(order);
    notify(mParameters);
}

int IIRFilterParametersSubject::getOrder() const noexcept 
{
    return mParameters.getOrder();
}

/*
void IIRFilterParametersSubject::notify()
{
    for (auto thisObserver : mObservers)
    {
        thisObserver->update();//Order(order);
    }
}
*/

//----------------------------------------------------------------------------//

IIRFilterParametersObserver::IIRFilterParametersObserver(
    IIRFilterParametersSubject *subject) :
    mSubject(subject)
{
    mSubject->subscribe(this);
    mParameters = mSubject->getParameters();
}

IIRFilterParametersObserver::~IIRFilterParametersObserver()
{
    mSubject->unsubscribe(this);
}

void IIRFilterParametersObserver::update(const IIRFilterParameters &parameters) 
{
    mParameters = parameters;
}

//----------------------------------------------------------------------------//

int main()
{
    IIRFilterParametersSubject iirParametersSubject;
    IIRFilterParametersObserver observer1(&iirParametersSubject);
    IIRFilterParametersObserver observer2(&iirParametersSubject);
    IIRFilterParametersObserver observer3(&iirParametersSubject);

    //iirParametersSubject.subscribe(&observer1);
    //iirParametersSubject.subscribe(&observer2);
    //iirParametersSubject.subscribe(&observer3);
    // Update the values on the subject - this should propagate to all subjects
printf("%d\n", observer1.getOrder());
    iirParametersSubject.setOrder(4);
   printf("Observer 1 order: %d\n", observer1.getOrder());
   printf("Observer 2 order: %d\n", observer2.getOrder());
    // 
//printf("%d\n", observer1.getOrder());

    iirParametersSubject.unsubscribe(&observer2);
    iirParametersSubject.unsubscribe(&observer1);
    return 0;
}
