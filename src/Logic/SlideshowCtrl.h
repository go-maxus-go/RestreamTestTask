#pragma once

#include <memory>

#include "Obs/Fwd.h"
#include "Model/Fwd.h"

#include "Fwd.h"


namespace Logic
{

class SlideCtrl;

class SlideshowCtrl
{
public:
    SlideshowCtrl(SlideCtrlPtr slideController);
    ~SlideshowCtrl();

    void attach(Obs::SlideshowObsPtr);
    void detach(Obs::SlideshowObsPtr);

    void loadSlideshow();

private:
    Model::SlideshowPtr m_slideshow;
    std::shared_ptr<SlideCtrl> m_slideCtrl;
    std::unique_ptr<Obs::Holder<Model::Slideshow>> m_obsHolder;
    class Observer;
    std::shared_ptr<Observer> m_observer;
    const int m_defaultSlideSize = 100;
};

} // namespace Logic
