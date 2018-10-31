#include "SlideshowCtrl.h"

#include <random>

#include <QDir>

#include "Obs/Holder.h"
#include "Obs/Observer.h"

#include "Model/Slide.h"
#include "Model/Slideshow.h"

#include "SlideCtrl.h"


namespace Logic
{

class SlideshowCtrl::Observer : public Obs::Observer<Model::Slide>
{
public:
    Observer(SlideshowCtrl & ctrl) : m_ctrl{ctrl}
    {}

protected:
    void created(Model::SlidePtrC object) override
    {
        m_ctrl.m_slideshow->slides.push_back(object);
    }

private:
    SlideshowCtrl & m_ctrl;
};

SlideshowCtrl::SlideshowCtrl(SlideCtrlPtr slideCtrl)
    : m_slideCtrl{std::move(slideCtrl)}
    , m_obsHolder{std::make_unique<Obs::Holder<Model::Slideshow>>()}
    , m_observer{std::make_shared<SlideshowCtrl::Observer>(*this)}
{
    m_slideCtrl->attach(m_observer);
}

SlideshowCtrl::~SlideshowCtrl()
{
    m_slideCtrl->detach(m_observer);
}

void SlideshowCtrl::attach(Obs::SlideshowObsPtr obs)
{
    m_obsHolder->attach(obs);
}
void SlideshowCtrl::detach(Obs::SlideshowObsPtr obs)
{
    m_obsHolder->detach(obs);
}

void SlideshowCtrl::loadSlideshow()
{
    m_slideshow = std::make_shared<Model::Slideshow>();
    m_obsHolder->notifyCreated(m_slideshow);

    const auto imageFolder = QDir::currentPath() + QDir::separator() + "images" + QDir::separator();
    const auto files = QDir(imageFolder).entryList(
        QDir::Filter::Files | QDir::Filter::NoDotAndDotDot);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomX(0, m_slideshow->width - m_defaultSlideSize);
    std::uniform_int_distribution<> randomY(0, m_slideshow->height - m_defaultSlideSize);

    auto randomRect = [this, &randomX, &randomY, &gen]() -> std::pair<QRect, bool>
    {
        // use 1000 trials to generate new rect
        // I know this is not the best way to solve the problem
        for (int i = 0; i < 1000; ++i)
        {
            const QRect rect(randomX(gen), randomY(gen), m_defaultSlideSize, m_defaultSlideSize);
            bool isIntersection = false;
            for (const auto & slide : m_slideshow->slides)
            {
                if (slide->rect.intersects(rect))
                {
                    isIntersection = true;
                    break;
                }
            }
            if (!isIntersection)
                return {rect, true};
        }
        return {QRect(), false};
    };

    for (const auto & file : files)
    {
        const auto rectResult = randomRect();
        if (!rectResult.second)
            break;
        m_slideCtrl->addSlide(rectResult.first, imageFolder + file);
    }

    m_obsHolder->notifyUpdated(m_slideshow);
}

} // namespace Logic
