#pragma once

#include <map>
#include <memory>
#include <vector>

#include <QObject>

#include "Obs/Fwd.h"
#include "Model/Fwd.h"


class QRect;
class QString;

template<class T> class QFuture;

namespace Logic
{

class SlideCtrl : public QObject
{
    Q_OBJECT
public:
    SlideCtrl();
    ~SlideCtrl();

public:
    bool addSlide(const QRect & rect, const QString & path);

    void attach(Obs::SlideObsPtr);
    void detach(Obs::SlideObsPtr);

    enum class Direction { Top, Left, Right, Bottom };
    void moveSelection(Direction);

private:
    Model::SlidePtr selectedSlide() const;
    void moveSelection(Model::SlidePtr from, Model::SlidePtr to);

private slots:
    void processLoadStarted();
    void processImageLoaded(bool result);

private:
    std::vector<Model::SlidePtr> m_slides;
    std::unique_ptr<Obs::Holder<Model::Slide>> m_obsHolder;
    std::map<std::shared_ptr<QImage>, Model::SlidePtr> m_image2slide;
    std::vector<QFuture<void>> m_futures;
};

} // namespace Logic
