#pragma once

#include "Fwd.h"
#include "Slide.h"


namespace Model
{

struct Slideshow
{
    int width;
    int height;
    std::vector<SlidePtrC> slides;
};

} // namespace Model
