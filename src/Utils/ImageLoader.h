#pragma once

#include <QImage>
#include <QObject>

#include "Model/Fwd.h"


namespace Utils
{

class ImageLoader : public QObject
{
    Q_OBJECT
public:
    ImageLoader(
        QString path,
        std::shared_ptr<QImage> image,
        int thumbnailSize,
        QObject * parent = nullptr);
    ~ImageLoader();

public:
    void load();

    std::shared_ptr<QImage> image() const;
    QImage thumbnail() const;

signals:
    bool loadStarted();
    bool loaded(bool);

private:
    const QString m_path;
    std::shared_ptr<QImage> m_image;
    QImage m_thumbnail;
    const int m_thumbnailSize;
};

} // namespace Utils
