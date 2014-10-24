#ifndef CAPTURE_H
#define CAPTURE_H

#include <QWidget>
#include <QImage>
#include <QTimer>
//#include <opencv2/opencv.hpp>
#include <qprocess.h>     // ??linux?????
#include <photos.h>
#include <QUdpSocket>
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QBuffer>
#include <QDateTime>
#include "TQInputMethod.h"
#include <QWSInputMethod>

#include "posix_qextserialport.h"
#include <QTimer>
namespace Ui {
    class Capture;
}

class Capture : public QWidget
{
    Q_OBJECT

public:
    explicit Capture(QWidget *parent = 0);
    void C_translateLanguage();
    bool cam_init();
    bool start_capture();
    void video_play();
    bool close_cam();
    ~Capture();
    Posix_QextSerialPort *myCom;
    //void C_translateLanguage();   //���ð�ť��ʾ������
   // void videoWrite(IplImage *frame);


public slots:
    void openCapture();    //������ͷ
    void closeCapture();   //�ر�����ͷ
    void readFrame();      //��ȡ�������֡
    void openPhotos();     //��ͼƬ�����
    void closePhotos();    //�ر�ͼƬ�����
    void takePhotos();     //����
    void readData();       //��ȡPC�˷���������

private:
    Ui::Capture *ui;
    int first_open_cam_flag;
    int open_cam_flag;
    QProcess process;//????QProcess
    QImage image;
    QPixmap pixmap;
    //QImage img;           //�������֡ת�����ͼƬ
    QTimer *timer;        //��ʱ��
    //CvCapture *capture;   //����ͷ
    //IplImage *frame;      //֡
    Photos *photos;       //Photos��Ķ���
    //CvVideoWriter *video;
    QUdpSocket* send_socket;
    const static quint16 port = 45454;  //�˿�
    int num ;            //��������
    int send_frame_flag; //�Ƿ���PC�˷�����Ƶ֡
    ///111
    int take_photo_flag;

    QTimer *readTimer;
    int serial_open_flag;

    int picNum;
    ///111

private slots:
    void on_serial_btn_clicked();
    //void on_pushButton_clicked();
    void on_image_close_pushButton_clicked();
    void paintEvent(QPaintEvent *);
    void readMyCom();

};

#endif // CAPTURE_H
