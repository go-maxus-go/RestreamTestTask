#pragma once

#include <string>
#include <memory>

#include <QRect>
#include <QImage>


namespace Model
{

struct Slide
{
    enum class State;

    Slide(QRect rect, QString path, std::shared_ptr<const QImage> image, State state = State::Added)
        : rect{std::move(rect)}
        , path{std::move(path)}
        , image{std::move(image)}
        , state{state}
    {}
    QRect rect;
    QString path;
    std::shared_ptr<const QImage> image;
    enum class State { Added, Loading, Ready } state = State::Added;
};

} // namespace Model
