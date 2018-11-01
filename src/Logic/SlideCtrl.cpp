#include "SlideCtrl.h"

#include <cmath>

#include <algorithm>

#include <QRect>
#include <QImage>
#include <QString>
#include <QtConcurrent/QtConcurrentRun>

#include "Obs/Holder.h"
#include "Obs/Observer.h"
#include "Model/Slide.h"
#include "Utils/ImageLoader.h"


namespace Logic
{

namespace
{

Model::SlidePtr findClosestSlide(Model::SlidePtr slide, const std::vector<Model::SlidePtr> & slides)
{
    if (slides.empty())
        return nullptr;

    auto minDistance = -1.0;
    auto closestSlide = *slides.begin();
    for (const auto & s : slides)
    {
        const auto dx = slide->rect.x() - s->rect.x();
        const auto dy = slide->rect.y() - s->rect.y();

        const auto distance = sqrt(dx * dx + dy * dy);
        if (distance < minDistance || minDistance < 0)
        {
            minDistance = distance;
            closestSlide = s;
        }
    }

    return closestSlide;
}

} // anonymous namespace


SlideCtrl::SlideCtrl()
    : m_obsHolder(std::make_unique<Obs::Holder<Model::Slide>>())
{}

SlideCtrl::~SlideCtrl()
{
    for (auto & future : m_futures)
    {
        if (future.isRunning())
            future.cancel();
    }
}

bool SlideCtrl::addSlide(const QRect & rect, const QString & path)
{
    auto image = std::make_shared<QImage>();
    auto slide = std::make_shared<Model::Slide>(rect, path, image, Model::Slide::State::Added);
    if (m_slides.empty())
        slide->isSelected = true;
    m_slides.push_back(slide);
    m_image2slide[image] = slide;
    m_obsHolder->notifyCreated(slide);

    const int thumbnailSize = qMin(rect.width(), rect.height());
    auto loader = new Utils::ImageLoader(path, std::move(image), thumbnailSize, this);
    connect(loader, &Utils::ImageLoader::loadStarted, this, &SlideCtrl::processLoadStarted);
    connect(loader, &Utils::ImageLoader::loaded, this, &SlideCtrl::processImageLoaded);

    m_futures.emplace_back(QtConcurrent::run([loader](){ loader->load(); }));

    return true;
}

void SlideCtrl::attach(Obs::SlideObsPtr obs)
{
    m_obsHolder->attach(obs);
}
void SlideCtrl::detach(Obs::SlideObsPtr obs)
{
    m_obsHolder->detach(obs);
}

void SlideCtrl::moveSelection(Direction direction)
{
    auto slide = selectedSlide();
    if (slide == nullptr)
        return; // TODO: process this case

    std::function<bool(const Model::SlidePtr &)> condition = [](auto){ return false; };
    switch (direction)
    {
    case Direction::Top:
        condition = [&slide](const auto & v){ return slide->rect.y() > v->rect.y(); };
        break;
    case Direction::Left:
        condition = [&slide](const auto & v){ return slide->rect.x() > v->rect.x(); };
        break;
    case Direction::Right:
        condition = [&slide](const auto & v){ return slide->rect.x() < v->rect.x(); };
        break;
    case Direction::Bottom:
        condition = [&slide](const auto & v){ return slide->rect.y() < v->rect.y(); };
        break;
    }
    std::vector<Model::SlidePtr> slides;
    std::copy_if(m_slides.begin(), m_slides.end(), std::back_inserter(slides), condition);
    moveSelection(slide, findClosestSlide(slide, slides));
}

Model::SlidePtr SlideCtrl::selectedSlide() const
{
    if (m_slides.empty())
        return nullptr;
    auto it = std::find_if(m_slides.begin(), m_slides.end(),
        [](const auto & v){ return v->isSelected; });
    if (it == m_slides.end())
        return nullptr;
    return *it;
}

void SlideCtrl::moveSelection(Model::SlidePtr from, Model::SlidePtr to)
{
    if (from == nullptr || to == nullptr)
        return;

    from->isSelected = false;
    m_obsHolder->notifyUpdated(from);
    to->isSelected = true;
    m_obsHolder->notifyUpdated(to);
}

void SlideCtrl::processLoadStarted()
{
    auto loader = qobject_cast<Utils::ImageLoader*>(sender());
    const auto it = m_image2slide.find(loader->image());
    if (it == m_image2slide.end())
        return;
    auto & slide = it->second;
    slide->state = Model::Slide::State::Loading;
    m_obsHolder->notifyUpdated(slide);
}

void SlideCtrl::processImageLoaded(bool result)
{
    sender()->deleteLater();
    auto loader = qobject_cast<Utils::ImageLoader*>(sender());
    const auto image = loader->image();
    const auto it = m_image2slide.find(image);
    if (it == m_image2slide.end())
        return;

    *image = loader->thumbnail();

    auto & slide = it->second;
    slide->state = result
        ? Model::Slide::State::Ready
        : Model::Slide::State::Failed;
    m_obsHolder->notifyUpdated(slide);
}

} // namespace Logic
