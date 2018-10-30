#pragma once

#include <memory>
#include <vector>

#include <QRect>
#include <QString>

#include "Obs/Fwd.h"
#include "Model/Fwd.h"


namespace Obs { template<class T> class Holder; }
namespace Model { struct Slide; }

namespace Logic
{

class SlideCtrl
{
    using ObsPtr = std::shared_ptr<Obs::Observer<Model::Slide>>;
public:
    SlideCtrl();
    ~SlideCtrl();

public:
    bool addSlide(QRect rect, QString path);

    void attach(Obs::SlideObsPtr);
    void detach(Obs::SlideObsPtr);

private:
    std::vector<Model::SlidePtr> m_slides;
    std::unique_ptr<Obs::Holder<Model::Slide>> m_obsHolder;
};

} // namespace Logic
