#include "ImageLoader.h"

#include "Model/Slide.h"


namespace Utils
{

ImageLoader::ImageLoader(
    QString path,
    std::shared_ptr<QImage> image,
    int thumbnailSize,
    QObject * parent)
    : QObject{parent}
    , m_path{std::move(path)}
    , m_image{std::move(image)}
    , m_thumbnailSize{thumbnailSize}
{}
ImageLoader::~ImageLoader() = default;

void ImageLoader::load()
{
    const bool res = m_image->load(m_path);
    if (res)
    {
        m_thumbnail = m_image->width() > m_image->height()
            ? m_image->scaledToWidth(m_thumbnailSize)
            : m_image->scaledToHeight(m_thumbnailSize);
    }
    emit loaded(res);
}

std::shared_ptr<QImage> ImageLoader::image() const
{
    return m_image;
}

QImage ImageLoader::thumbnail() const
{
    return m_thumbnail;
}

} // namespace Utils
