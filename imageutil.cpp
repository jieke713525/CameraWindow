#include "imageutil.h"

ImageUtil::ImageUtil()
{

}

QImage ImageUtil::toImage(CGrabResultPtr ptr)
{
    QImage Qimg;
    m_formatConverter.Convert(pylonImage, ptr);
    uchar * din = (uchar *)(pylonImage.GetBuffer());
    if(IsMono(pylonImage.GetPixelType()))
    {
        Qimg = QImage(din, ptr->GetWidth(), ptr->GetHeight(), QImage::Format_Indexed8);
    }
    if(IsRGB(pylonImage.GetPixelType()))
    {
        Qimg = QImage(din, ptr->GetWidth(), ptr->GetHeight(), QImage::Format_RGB888);
    }
    return Qimg;
}
