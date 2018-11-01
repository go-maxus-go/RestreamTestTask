#pragma once

#include <memory>

#include "Model/Fwd.h"


namespace Obs
{

template<class T> class Holder;
template<class T> struct Observer;

using SlideObs = Observer<Model::Slide>;
using SlideshowObs = Observer<Model::Slideshow>;

using SlideObsPtr = std::shared_ptr<SlideObs>;
using SlideshowObsPtr = std::shared_ptr<SlideshowObs>;

} // namespace Obs
