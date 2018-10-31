#pragma once

#include <map>
#include <memory>
#include <vector>

#include <QRect>
#include <QObject>
#include <QString>

#include "Obs/Fwd.h"
#include "Model/Fwd.h"


template<class T> class QFuture;

namespace Obs { template<class T> class Holder; }
namespace Model { struct Slide; }

namespace Logic
{

class SlideCtrl : public QObject
{
    Q_OBJECT
public:
    SlideCtrl();
    ~SlideCtrl();

public:
    bool addSlide(QRect rect, QString path);

    void attach(Obs::SlideObsPtr);
    void detach(Obs::SlideObsPtr);

private slots:
    void processLoadedImage(bool result);

private:
    std::vector<Model::SlidePtr> m_slides;
    std::unique_ptr<Obs::Holder<Model::Slide>> m_obsHolder;
    std::map<std::shared_ptr<QImage>, Model::SlidePtr> m_image2slide;
    std::vector<QFuture<void>> m_futures;
};

} // namespace Logic
