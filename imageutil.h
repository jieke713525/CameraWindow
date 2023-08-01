#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H

#include <QImage>
#include <QObject>
#include <pylon/PylonIncludes.h>
#include <pylon/ImageFormatConverter.h>
#include <pylon/PylonImage.h>
using namespace Pylon;
class ImageUtil
{
public:
    ImageUtil();
public:
    CImageFormatConverter m_formatConverter;
    CPylonImage pylonImage;
    QImage toImage(CGrabResultPtr ptr);
};

#endif // IMAGEUTIL_H
