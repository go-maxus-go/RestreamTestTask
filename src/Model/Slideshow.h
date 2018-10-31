#pragma once

#include "Fwd.h"
#include "Slide.h"


namespace Model
{

struct Slideshow
{
    int width = 1280; // hardcode =(
    int height = 720;
    std::vector<SlidePtrC> slides;
};

} // namespace Model
