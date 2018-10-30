#pragma once

#include <memory>

#include "Obs/Fwd.h"
#include "Model/Fwd.h"

#include "SlideCtrl.h"


namespace Obs { template<class T> class Holder; }
namespace Model { struct Slideshow; }

namespace Logic
{
class SlideCtrl;

class SlideshowCtrl
{
public:
    SlideshowCtrl(std::shared_ptr<SlideCtrl> slideController);
    ~SlideshowCtrl();

private:
    Model::SlideshowPtr m_slideshow;
    std::shared_ptr<SlideCtrl> m_slideCtrl;
    std::unique_ptr<Obs::Holder<Model::Slideshow>> m_obsHolder;
    class Observer;
    std::shared_ptr<Observer> m_observer;
};

} // namespace Logic
