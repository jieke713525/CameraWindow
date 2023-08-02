#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <QMutex>
#include <QImage>
#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>
#include <imageutil.h>
using namespace Pylon;
class VideoThread : public QThread
{
    Q_OBJECT
private:
    CBaslerUniversalInstantCamera *camera;
    QMutex lock;
    ImageUtil *imageutil=new ImageUtil();
public:
    VideoThread(CBaslerUniversalInstantCamera *camera);
    explicit VideoThread(QObject *parent = nullptr);
protected:
    void run();
public:
    int height,width;
    void pause();
    void resume();
    void end();
signals:
    void sendImage(QImage image);
};

#endif // VIDEOTHREAD_H
