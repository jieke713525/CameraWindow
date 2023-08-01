#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <QWidget>
#include <QImage>
#include <QMatrix>
#include "imageutil.h"
#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>
#include <videothread.h>
#include <QLabel>
using namespace Pylon;

namespace Ui {
class CameraWindow;
}

class CameraWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CameraWindow(QWidget *parent = nullptr);
    ~CameraWindow();

private:
    Ui::CameraWindow *ui;
    CBaslerUniversalInstantCamera *camera=
            new CBaslerUniversalInstantCamera();
    ImageUtil *imageutil=
            new ImageUtil();
    VideoThread *videothread;
    QLabel *label=new QLabel();
public:
    void initUI();
    void updateInfo();
    DeviceInfoList_t getAllDevice();
private slots:
    void on_open_clicked();
    void on_close_clicked();
    void on_p1val_valueChanged(int value);
    void on_p2val_valueChanged(int value);
    void on_p3val_valueChanged(int value);
    void on_p1set_clicked();
    void on_p2set_clicked();
    void on_p3set_clicked();
    void on_singleshot_clicked();
    void on_continstart_clicked();
    void on_continend_clicked();
};

#endif // CAMERAWINDOW_H
