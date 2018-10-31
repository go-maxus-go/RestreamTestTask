#include "SlideCtrl.h"

#include <cmath>

#include <algorithm>

#include <QImage>
#include <QtConcurrent/QtConcurrentRun>

#include "Obs/Holder.h"
#include "Obs/Observer.h"
#include "Model/Slide.h"
#include "Utils/ImageLoader.h"


namespace Logic
{

static int s_thumbnailSize = 90;

static Model::SlidePtr findClosestSlide(Model::SlidePtr slide, std::vector<Model::SlidePtr> slides)
{
    if (slides.empty())
        return slide;

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

bool SlideCtrl::addSlide(QRect rect, QString path)
{
    auto image = std::make_shared<QImage>();
    auto slide = std::make_shared<Model::Slide>(std::move(rect), path, image, Model::Slide::State::Added);
    if (m_slides.empty())
        slide->isSelected = true;
    m_slides.push_back(slide);
    m_image2slide[image] = slide;
    m_obsHolder->notifyCreated(slide);

    auto loader = new Utils::ImageLoader(std::move(path), std::move(image), s_thumbnailSize, this);
    connect(loader, &Utils::ImageLoader::loaded, this, &SlideCtrl::processLoadedImage, Qt::QueuedConnection);

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

void SlideCtrl::selectTop()
{
    if (m_slides.empty())
        return;

    auto selectedSlideIt = std::find_if(m_slides.begin(), m_slides.end(),
        [](const auto & v){ return v->isSelected; });
    if (selectedSlideIt == m_slides.end())
        return; // TODO: process this case

    auto selectedSlide = *selectedSlideIt;
    std::vector<Model::SlidePtr> topSlides;
    std::copy_if(m_slides.begin(), m_slides.end(), std::back_inserter(topSlides),
        [&selectedSlide](const auto & v){ return selectedSlide->rect.y() > v->rect.y(); });

    moveSelection(selectedSlide, findClosestSlide(selectedSlide, topSlides));
}

void SlideCtrl::selectLeft()
{
    if (m_slides.empty())
        return;

    auto selectedSlideIt = std::find_if(m_slides.begin(), m_slides.end(),
        [](const auto & v){ return v->isSelected; });
    if (selectedSlideIt == m_slides.end())
        return; // TODO: process this case

    auto selectedSlide = *selectedSlideIt;
    std::vector<Model::SlidePtr> leftSlides;
    std::copy_if(m_slides.begin(), m_slides.end(), std::back_inserter(leftSlides),
        [&selectedSlide](const auto & v){ return selectedSlide->rect.x() > v->rect.x(); });

    moveSelection(selectedSlide, findClosestSlide(selectedSlide, leftSlides));
}

void SlideCtrl::selectRight()
{
    if (m_slides.empty())
        return;

    auto selectedSlideIt = std::find_if(m_slides.begin(), m_slides.end(),
        [](const auto & v){ return v->isSelected; });
    if (selectedSlideIt == m_slides.end())
        return; // TODO: process this case

    auto selectedSlide = *selectedSlideIt;
    std::vector<Model::SlidePtr> rightSlides;
    std::copy_if(m_slides.begin(), m_slides.end(), std::back_inserter(rightSlides),
        [&selectedSlide](const auto & v){ return selectedSlide->rect.x() < v->rect.x(); });

    moveSelection(selectedSlide, findClosestSlide(selectedSlide, rightSlides));
}

void SlideCtrl::selectBottom()
{
    if (m_slides.empty())
        return;

    auto selectedSlideIt = std::find_if(m_slides.begin(), m_slides.end(),
        [](const auto & v){ return v->isSelected; });
    if (selectedSlideIt == m_slides.end())
        return; // TODO: process this case

    auto selectedSlide = *selectedSlideIt;
    std::vector<Model::SlidePtr> bottomSlides;
    std::copy_if(m_slides.begin(), m_slides.end(), std::back_inserter(bottomSlides),
        [&selectedSlide](const auto & v){ return selectedSlide->rect.y() < v->rect.y(); });

    moveSelection(selectedSlide, findClosestSlide(selectedSlide, bottomSlides));
}

void SlideCtrl::moveSelection(Model::SlidePtr from, Model::SlidePtr to)
{
    from->isSelected = false;
    m_obsHolder->notifyUpdated(from);
    to->isSelected = true;
    m_obsHolder->notifyUpdated(to);
}

void SlideCtrl::processLoadedImage(bool result)
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
