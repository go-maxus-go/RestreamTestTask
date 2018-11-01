#pragma once

#include <set>
#include <memory>

#include "Observer.h"


namespace Obs
{

template<class T>
class Holder
{
    using ObsPtr = std::shared_ptr<Observer<T>>;
public:
    virtual ~Holder() = default;

    void attach(ObsPtr obs)
    {
        m_observers.insert(obs);
    }
    void detach(ObsPtr obs)
    {
        auto it = m_observers.find(obs);
        if (it != m_observers.end())
            m_observers.erase(it);
    }

    void notifyCreated(std::shared_ptr<const T> object)
    {
        for (const auto & obs: m_observers)
            obs->created(object);
    }
    void notifyUpdated(std::shared_ptr<const T> object)
    {
        for (const auto & obs: m_observers)
            obs->updated(object);
    }
    void notifyRemoved(std::shared_ptr<const T> object)
    {
        for (const auto & obs: m_observers)
            obs->removed(object);
    }

private:
    std::set<ObsPtr> m_observers;
};

} // namespace Obs
