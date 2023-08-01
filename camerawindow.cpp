#include "camerawindow.h"
#include "ui_camerawindow.h"

CameraWindow::CameraWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraWindow)
{
    ui->setupUi(this);
    initUI();
    videothread=new VideoThread(camera);
    connect(videothread,&VideoThread::sendImage,
            this,[=](QImage image){
        label->setPixmap(QPixmap::fromImage(image));
        ui->pictureArea->setWidget(label);
    });
}

CameraWindow::~CameraWindow()
{
    delete ui;
}

void CameraWindow::initUI()
{
    PylonInitialize();
    DeviceInfoList_t list=getAllDevice();
    DeviceInfoList_t::iterator it;
    for (it=list.begin();it!=list.end();++it) {
        ui->devicelist->addItem(it->GetFriendlyName().c_str());
    }
    {
        ui->open->setEnabled(true);
        ui->close->setEnabled(false);
        ui->p1val->setEnabled(false);
        ui->p1set->setEnabled(false);
        ui->p2val->setEnabled(false);
        ui->p2set->setEnabled(false);
        ui->p3val->setEnabled(false);
        ui->p3set->setEnabled(false);
        ui->singleshot->setEnabled(false);
        ui->save->setEnabled(false);
        ui->continstart->setEnabled(false);
        ui->continend->setEnabled(false);
    }
}

void CameraWindow::updateInfo()
{
    QString info;
    info.append("FriendlyName:"+camera->GetDeviceInfo().GetFriendlyName()+"\n\n");
    info.append("AcquisitionFrameRate:"+camera->AcquisitionFrameRate.ToString()+"\n\n");
    info.append("Gain:"+camera->Gain.ToString()+"\n\n");
    info.append("ExposureTime:"+camera->ExposureTime.ToString()+"\n\n");
    ui->info->setText(info);
}

DeviceInfoList_t CameraWindow::getAllDevice()
{
    CTlFactory& factory=CTlFactory::GetInstance();
    DeviceInfoList_t list;
    factory.EnumerateDevices(list);
    return list;
}

void CameraWindow::on_open_clicked()
{
    int index=ui->devicelist->currentIndex();
    camera->Attach(CTlFactory::GetInstance().CreateDevice(getAllDevice()[index]));
    camera->Open();
    ui->p1val->setMaximum(camera->AcquisitionFrameRate.GetMax());
    ui->p1val->setMinimum(camera->AcquisitionFrameRate.GetMin());
    ui->p2val->setMaximum(camera->Gain.GetMax());
    ui->p2val->setMinimum(camera->Gain.GetMin());
    ui->p3val->setMaximum(camera->ExposureTime.GetMax());
    ui->p3val->setMinimum(camera->ExposureTime.GetMin());
    {
        ui->open->setEnabled(false);
        ui->close->setEnabled(true);
        ui->p1val->setEnabled(true);
        ui->p1set->setEnabled(true);
        ui->p2val->setEnabled(true);
        ui->p2set->setEnabled(true);
        ui->p3val->setEnabled(true);
        ui->p3set->setEnabled(true);
        ui->singleshot->setEnabled(true);
        ui->save->setEnabled(false);
        ui->continstart->setEnabled(true);
        ui->continend->setEnabled(false);
    }
    updateInfo();
}

void CameraWindow::on_close_clicked()
{
    camera->Close();
    camera->DestroyDevice();
    {
        ui->open->setEnabled(true);
        ui->close->setEnabled(false);
        ui->p1val->setEnabled(false);
        ui->p1set->setEnabled(false);
        ui->p2val->setEnabled(false);
        ui->p2set->setEnabled(false);
        ui->p3val->setEnabled(false);
        ui->p3set->setEnabled(false);
        ui->singleshot->setEnabled(false);
        ui->save->setEnabled(false);
        ui->continstart->setEnabled(false);
        ui->continend->setEnabled(false);
    }
}

void CameraWindow::on_p1val_valueChanged(int value)
{
    QString s;
    s.append("设置相机的帧率为:").append(QString::number(value)).append("帧/秒");
    ui->p11->setText(s);
}

void CameraWindow::on_p2val_valueChanged(int value)
{
    QString s;
    s.append("设置相机的增益为:").append(QString::number(value));
    ui->p22->setText(s);
}
void CameraWindow::on_p3val_valueChanged(int value)
{
    QString s;
    s.append("设置相机的曝光时间为:").append(QString::number(value)).append("微秒");
    ui->p33->setText(s);
}

void CameraWindow::on_p1set_clicked()
{
    camera->AcquisitionFrameRate=ui->p1val->value();
    updateInfo();
}

void CameraWindow::on_p2set_clicked()
{
    camera->Gain=ui->p2val->value();
    updateInfo();
}

void CameraWindow::on_p3set_clicked()
{
    camera->ExposureTime=ui->p3val->value();
    updateInfo();
}

void CameraWindow::on_singleshot_clicked()
{
    {
        ui->open->setEnabled(false);
        ui->close->setEnabled(true);
        ui->p1val->setEnabled(true);
        ui->p1set->setEnabled(true);
        ui->p2val->setEnabled(true);
        ui->p2set->setEnabled(true);
        ui->p3val->setEnabled(true);
        ui->p3set->setEnabled(true);
        ui->singleshot->setEnabled(true);
        ui->save->setEnabled(true);
        ui->continstart->setEnabled(true);
        ui->continend->setEnabled(false);
    }
    camera->StartGrabbing(1);
    CGrabResultPtr ptr;
    camera->RetrieveResult(5000,ptr,TimeoutHandling_ThrowException);
    if(ptr->GrabSucceeded()){
        QImage image=imageutil->toImage(ptr);
        QImage im=image.scaled(QSize(label->width(),
                                     label->height()),
                               Qt::IgnoreAspectRatio,
                               Qt::SmoothTransformation);
        label->setPixmap(QPixmap::fromImage(im));
        ui->pictureArea->setWidget(label);
    }
    camera->StopGrabbing();
}

void CameraWindow::on_continstart_clicked()
{
    if(videothread->flag==0){
        videothread->height=label->height();
        videothread->width=label->width();
        videothread->start();
    }
    else if(videothread->flag==1){
        videothread->resume();
    }
    {
        ui->open->setEnabled(false);
        ui->close->setEnabled(true);
        ui->p1val->setEnabled(true);
        ui->p1set->setEnabled(true);
        ui->p2val->setEnabled(true);
        ui->p2set->setEnabled(true);
        ui->p3val->setEnabled(true);
        ui->p3set->setEnabled(true);
        ui->singleshot->setEnabled(false);
        ui->save->setEnabled(false);
        ui->continstart->setEnabled(false);
        ui->continend->setEnabled(true);
    }
}

void CameraWindow::on_continend_clicked()
{
    videothread->pause();
    {
        ui->open->setEnabled(false);
        ui->close->setEnabled(true);
        ui->p1val->setEnabled(true);
        ui->p1set->setEnabled(true);
        ui->p2val->setEnabled(true);
        ui->p2set->setEnabled(true);
        ui->p3val->setEnabled(true);
        ui->p3set->setEnabled(true);
        ui->singleshot->setEnabled(false);
        ui->save->setEnabled(false);
        ui->continstart->setEnabled(true);
        ui->continend->setEnabled(false);
    }
}
