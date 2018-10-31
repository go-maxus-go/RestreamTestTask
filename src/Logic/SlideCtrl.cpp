#include "SlideCtrl.h"

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

void SlideCtrl::selectLeft()
{
    if (m_slides.empty() || m_slides[0]->isSelected)
        return;

    for (size_t i = 0; i < m_slides.size(); ++i)
    {
        auto & slide = m_slides[i];
        if (slide->isSelected)
        {
            slide->isSelected = false;
            m_obsHolder->notifyUpdated(slide);
            auto & prevSlide = m_slides[i - 1];
            prevSlide->isSelected = true;
            m_obsHolder->notifyUpdated(prevSlide);
            return;
        }
    }
}

void SlideCtrl::selectRight()
{
    for (size_t i = 0; i < m_slides.size(); ++i)
    {
        auto & slide = m_slides[i];
        if (slide->isSelected && i < m_slides.size() - 1)
        {
            slide->isSelected = false;
            m_obsHolder->notifyUpdated(slide);
            auto & nextSlide = m_slides[i + 1];
            nextSlide->isSelected = true;
            m_obsHolder->notifyUpdated(nextSlide);
            return;
        }
    }
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
