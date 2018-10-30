#pragma once

#include <memory>


namespace Logic
{

class SlideCtrl;
class SlideshowCtrl;

using SlideCtrlPtr = std::shared_ptr<SlideCtrl>;
using SlideshowCtrlPtr = std::shared_ptr<SlideshowCtrl>;

} // namespace Logic
