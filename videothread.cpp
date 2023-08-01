#include "videothread.h"

VideoThread::VideoThread(QObject *parent) : QThread(parent)
{

}
VideoThread::VideoThread(CBaslerUniversalInstantCamera *camera)
{
    this->camera=camera;
}
void VideoThread::run()
{
    flag=1;
    camera->StartGrabbing();
    CGrabResultPtr ptr;
    while(camera->IsGrabbing()){
        lock.lock();
        camera->RetrieveResult(5000,ptr,TimeoutHandling_ThrowException);
        if(ptr->GrabSucceeded()){
            QImage image=imageutil->toImage(ptr);
            QImage im=image.scaled(width,
                                   height,
                                   Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation);
            emit sendImage(im);
        }
        lock.unlock();
    }
}

void VideoThread::pause()
{
    lock.lock();
}

void VideoThread::resume()
{
    lock.unlock();
}

void VideoThread::end()
{
    camera->StopGrabbing();
    flag=2;
    QThread::quit();
}

