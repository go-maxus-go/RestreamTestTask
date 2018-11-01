#pragma once

#include <memory>


namespace Obs
{

template<class T>
struct Observer
{
    virtual ~Observer() = default;

    virtual void created(std::shared_ptr<const T>) {}
    virtual void updated(std::shared_ptr<const T>) {}
    virtual void removed(std::shared_ptr<const T>) {}
};

} // namespace Obs
