#pragma once

#include <memory>


namespace Model
{

struct Slide;
struct Slideshow;

using SlidePtr = std::shared_ptr<Slide>;
using SlidePtrC = std::shared_ptr<const Slide>;

using SlideshowPtr = std::shared_ptr<Slideshow>;
using SlideshowPtrC = std::shared_ptr<const Slideshow>;

} // namespace Model
