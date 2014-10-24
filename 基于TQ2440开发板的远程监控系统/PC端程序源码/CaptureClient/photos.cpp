#include "photos.h"
#include "ui_photos.h"
#include <QFileDialog>
#include <QDirIterator>
#include <QListIterator>
#include <QMessageBox>
#include <QPainter>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QMouseEvent>
#include <QPushButton>

#define RATIO 2


Photos::Photos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Photos)
{
    ui->setupUi(this);

    position = 0;
    //��ͼ����ʼ����
    original_x = 60;
    original_y = 18;
    //ͼƬ�Ĵ�С
    image_width = 600;
    image_height = 300;

    transform_flag = 0;   //ͼƬ�ܷ��洰��仯���仯�ı�־
    //����Ĵ�С
    widget_width = QWidget::width();
    widget_height = QWidget::height();

    //mouse_flag = 0;
    //��ʼʱ��ͼƬ���봰��߽�ľ���
    static_height = 74;
    static_width =  128;

    translateLanguage();
    designUI();

    //this->setMouseTracking(true);


    connect(ui->open_pushButton, SIGNAL(clicked()), this, SLOT(openFiles()));
    connect(ui->previous_pushButton, SIGNAL(clicked()), this, SLOT(previousPhoto()));
    connect(ui->next_pushButton, SIGNAL(clicked()), this, SLOT(nextPhoto()));
    connect(ui->previous_Button, SIGNAL(clicked()), this, SLOT(previousPhoto()));
    connect(ui->next_Button, SIGNAL(clicked()), this, SLOT(nextPhoto()));
    connect(ui->expand_pushButton, SIGNAL(clicked()), this, SLOT(expandImage()));
    connect(ui->narrow_pushButton, SIGNAL(clicked()), this, SLOT(narrowImage()));
    connect(ui->delete_pushButton, SIGNAL(clicked()), this, SLOT(deleteImage()));
    connect(ui->adjust_pushButton, SIGNAL(clicked()), this, SLOT(adjustPhoto()));



}

//���ð�ť����ʾ������
void Photos::translateLanguage()
{
    ui->adjust_pushButton->setToolTip(tr("��Ӧ����"));
    ui->open_pushButton->setToolTip(tr("���ļ�"));
    ui->previous_pushButton->setToolTip(tr("��һ��"));
    ui->next_pushButton->setToolTip(tr("��һ��"));
    ui->expand_pushButton->setToolTip(tr("�Ŵ�"));
    ui->narrow_pushButton->setToolTip(tr("��С"));
    ui->previous_Button->setToolTip(tr("��һ��"));
    ui->next_Button->setToolTip(tr("��һ��"));
    ui->delete_pushButton->setToolTip(tr("ɾ��"));
}

void Photos::designUI()
{
    QPalette palette;
    this->setAutoFillBackground(true);
    palette.setColor(QPalette::Background, QColor(255,255,255));
    this->setPalette(palette);


    disableButton();
    //��ư�ť�ķ����ʽ
    ui->previous_Button->setStyleSheet("border-style: flat;");
    ui->next_Button->setStyleSheet("border-style: flat;");
    ui->adjust_pushButton->setStyleSheet("border-style: flat;");
    ui->adjust_pushButton->setStyleSheet("QPushButton{border-image:url(:/Image/adjust1.jpg)}QPushButton:hover:!pressed{border-image:url(:/Image/adjust2.jpg);font-size:20px;}QPushButton:hover:pressed {border-image:url(:/Image/adjust2.jpg);font-size:20px;}");
    ui->expand_pushButton->setStyleSheet("border-style: flat;");
    ui->expand_pushButton->setStyleSheet("QPushButton{border-image:url(:/Image/expand1.jpg)}QPushButton:hover:!pressed{border-image:url(:/Image/expand2.jpg);font-size:20px;}QPushButton:hover:pressed {border-image:url(:/Image/expand2.jpg);font-size:20px;}");
    ui->narrow_pushButton->setStyleSheet("border-style: flat;");
    ui->narrow_pushButton->setStyleSheet("QPushButton{border-image:url(:/Image/narrow1.jpg)}QPushButton:hover:!pressed{border-image:url(:/Image/narrow2.jpg);font-size:20px;}QPushButton:hover:pressed {border-image:url(:/Image/narrow2.jpg);font-size:20px;}");
    ui->previous_pushButton->setStyleSheet("border-style: flat;");
    ui->previous_pushButton->setStyleSheet("QPushButton{border-image:url(:/Image/previous1.jpg)}QPushButton:hover:!pressed{border-image:url(:/Image/previous2.jpg);font-size:20px;}QPushButton:hover:pressed {border-image:url(:/Image/previous2.jpg);font-size:20px;}");
    ui->next_pushButton->setStyleSheet("border-style: flat;");
    ui->next_pushButton->setStyleSheet("QPushButton{border-image:url(:/Image/next1.jpg)}QPushButton:hover:!pressed{border-image:url(:/Image/next2.jpg);font-size:20px;}QPushButton:hover:pressed {border-image:url(:/Image/next2.jpg);font-size:20px;}");
    ui->open_pushButton->setStyleSheet("border-style: flat;");
    ui->open_pushButton->setStyleSheet("QPushButton{border-image:url(:/Image/open1.jpg)}QPushButton:hover:!pressed{border-image:url(:/Image/open2.jpg);font-size:20px;}QPushButton:hover:pressed {border-image:url(:/Image/open2.jpg);font-size:20px;}");
    ui->delete_pushButton->setStyleSheet("border-style: flat;");
    ui->delete_pushButton->setStyleSheet("QPushButton{border-image:url(:/Image/delete1.jpg)}QPushButton:hover:!pressed{border-image:url(:/Image/delete2.jpg);font-size:20px;}QPushButton:hover:pressed {border-image:url(:/Image/delete2.jpg);font-size:20px;}");
}
void Photos::disableButton()
{
    ui->adjust_pushButton->setDisabled(true);
    ui->expand_pushButton->setDisabled(true);
    ui->narrow_pushButton->setDisabled(true);
    ui->previous_pushButton->setDisabled(true);
    ui->previous_Button->setDisabled(true);
    ui->next_Button->setDisabled(true);
    ui->next_pushButton->setDisabled(true);
    ui->delete_pushButton->setDisabled(true);
}
void Photos::enableButton()
{
    ui->expand_pushButton->setDisabled(false);
    ui->narrow_pushButton->setDisabled(false);
    ui->previous_pushButton->setDisabled(false);
    ui->previous_Button->setDisabled(false);
    ui->next_Button->setDisabled(false);
    ui->next_pushButton->setDisabled(false);
    ui->delete_pushButton->setDisabled(false);
    ui->adjust_pushButton->setDisabled(false);
}
//����ͼƬ
void Photos::setPixmap(QString imageName)
{
    pixmap.load(imageName);
    update();

}

//��ʾͼƬ
void Photos::paintEvent(QPaintEvent *)
{
   QPainter painter(this);
   painter.drawPixmap(original_x, original_y, image_width, image_height, pixmap);

}


//��ͼƬ�ļ���
void Photos::openFiles()
{


    QString path = QFileDialog::getOpenFileName(this,"","/","*.*");
    if( "" != path)
    {
        position = 0;
        imageList.clear();
        imagePath = path;
        //��"/"Ϊ�ָ���־�����ַ�����ֱ��浽stringList
        QStringList stringList = imagePath.split("/");
        //��ȡͼƬ�����ļ��е�·��
        stringList.removeLast();
        dirPath = stringList.join("/");
        setPixmap(imagePath);    //����ͼƬ
        enableButton();          //�ð�ť����
        scanFile(dirPath);       //�����ļ���
    }



}

//�����ļ����µ�ͼƬ
void Photos::scanFile(QString path)
{
    QDirIterator dirIterator(path,QDir::Files|QDir::NoSymLinks);
    while (dirIterator.hasNext())
    {
        QString tmpFile = dirIterator.next();
        QStringList list = tmpFile.split(".");
        QString expandedName = list.takeLast();
        if("jpg" == expandedName || "JPG" == expandedName)
        imageList << tmpFile;

    }
    getPosition();

}

//��ȡ��ͼƬλ��
void Photos::getPosition()
{
    QListIterator<QString> it(imageList);
    while(it.hasNext())
    {
        if(imagePath == it.next())
        {
            break;
        }
        else
        {
            position++;
        }

    }
}

//��һ��ͼƬ
void Photos::previousPhoto()
{
    if( 0 == imageList.size())
    {
        QMessageBox::critical(this,tr("���ش�����Ϣ"),
                              tr("����ͼƬ����!"),QMessageBox::Ok);
        return;
    }
    position = (position - 1 + imageList.size()) % (imageList.size());
    imagePath = imageList.at(position);
    setPixmap(imagePath);
}

//��һ��ͼƬ
void Photos::nextPhoto()
{
    if( 0 == imageList.size())
    {
        QMessageBox::critical(this,tr("���ش�����Ϣ"),
                              tr("����ͼƬ����!"),QMessageBox::Ok);
        return;
    }
    position = (position + 1 + imageList.size()) % (imageList.size());
    imagePath = imageList.at(position);
    setPixmap(imagePath);
}

//�Ŵ�ͼƬ
void Photos::expandImage()
{
    if( 0 == imageList.size())
    {
        QMessageBox::critical(this,tr("���ش�����Ϣ"),
                              tr("����ͼƬ����!"),QMessageBox::Ok);
        return;
    }
    transform_flag = 1;
    int i,j,k;

    for(int times = 0; times < 25; times++)
    {
        image_width += RATIO;
        image_height += RATIO;
        original_x -= (RATIO/2);
        original_y -= (RATIO/2);
        //size_width -= RATIO;
        //size_height -= RATIO;
        update();
        this->repaint(original_x, original_y, image_width, image_height);
        for(i = 0; i < 800; i++)
            for(j = 0; j < 100; j++)
                for(k = 0; k < 45; k++);
    }
    //�ж���С��ť�Ƿ���Ч������Ч����ʹ����Ч
    if(!ui->narrow_pushButton->isEnabled())
    {
        ui->narrow_pushButton->setDisabled(false);
    }
}

//��СͼƬ
void Photos::narrowImage()
{
    if( 0 == imageList.size())
    {
        QMessageBox::critical(this,tr("���ش�����Ϣ"),
                              tr("����ͼƬ����!"),QMessageBox::Ok);
        return;
    }
    transform_flag = 1;
    int i,j,k;

    if((image_height - (RATIO*25)) >= 150)
    {
        for(int times = 0; times < 25; times++)
        {

            image_width -=  RATIO;
            image_height -= RATIO;
            original_x += (RATIO/2);
            original_y += (RATIO/2);
            //size_width += RATIO;
            //size_height += RATIO;
            update();
            this->repaint(original_x, original_y, image_width, image_height);
            for(i = 0; i < 800; i++)
                for(j = 0; j < 100; j++)
                    for(k = 0; k < 45; k++);
        }
    }
    //��ͼƬ��С����С�޶�ʱ��ʹ��С��ť��Ч
    if(image_height <= 150)
    {
        ui->narrow_pushButton->setDisabled(true);
    }
}

//ɾ��ͼƬ
void Photos::deleteImage()
{
    if( 0 == imageList.size())
    {
        QMessageBox::critical(this,tr("���ش�����Ϣ"),
                              tr("����ͼƬ����!"),QMessageBox::Ok);
        return;
    }
    QMessageBox::StandardButton select = QMessageBox::question(NULL,"Warning",
                 tr("ȷ��Ҫɾ����Ƭ��"),QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if(QMessageBox::Yes == select)
    {

        imageList.takeAt(position);
        QDir dir(dirPath);
        QFileInfoList fileList;
        QFileInfo curFile;
        QStringList filters;

        filters << "*.jpg" << "*.JPG";

        //��ȡָ��·���µ������ļ�
        fileList = dir.entryInfoList(filters, QDir::Dirs|QDir::Files
                                   | QDir::Readable | QDir::Writable
                                   | QDir::Hidden | QDir::NoDotAndDotDot
                                   , QDir::Name);

        if(fileList.size()>0)
        {

            curFile = fileList[position];
            QFile tmpFile(curFile.filePath());
            tmpFile.remove();
            fileList.removeAt(position);

        }

        if(0 == imageList.size())
        {

            QMessageBox::critical(this,tr("���ش�����Ϣ"),
                                  tr("����ͼƬ����!"),QMessageBox::Ok);
            return;
        }

        //ɾ��ͼƬ�Ժ�ÿ��ͼƬ��λ�þͻ��Զ���һ��������Ȼ�������λ�õ�ͼƬ����ɾ��ͼƬ����һ��ͼƬ
        imagePath = imageList.at(position);
        setPixmap(imagePath);

    }
}

//ͼƬ�洰��仯���仯
void Photos::resizeEvent(QResizeEvent *)
{

    int tmp_width, tmp_height;
    //���ʵʱ����ߴ�
    tmp_width = QWidget::width();
    tmp_height = QWidget::height();


    //��transform_flagΪ1ʱ��ͼ���洰��仯ʧЧ
    if(0 == transform_flag)
    {
        if(tmp_width != widget_width )
        {

            image_width = tmp_width - static_width;
            update();
        }
        if(tmp_height != widget_height)
        {
            image_height = tmp_height - static_height;
            update();
        }
    }
    else
    {
        if(tmp_width > widget_width)
        {
            original_x += (tmp_width - widget_width+1)/2;
        }
        else{
            original_x += (tmp_width - widget_width-1)/2;
        }
        if(tmp_height > widget_height)
        {
            original_y += (tmp_height - widget_height+1)/2;
        }
        else{
            original_y += (tmp_height - widget_height-1)/2;
        }
        update();
    }
    widget_width = tmp_width;
    widget_height = tmp_height;


}

void Photos::adjustPhoto()
{
    int tmp_width, tmp_height;
    tmp_width = QWidget::width();
    tmp_height = QWidget::height();
    image_width = tmp_width - static_width;
    image_height = tmp_height - static_height;
    original_x = 60;
    original_y = 18;
    transform_flag = 0;
    update();
}

void Photos::closeEvent(QCloseEvent *)
{
    position = 0;
    original_x = 60;
    original_y = 18;
    image_width = QWidget::width() - static_width;
    image_height = QWidget::height() - static_height;
    transform_flag = 0;
    imageList.clear();
    imagePath.clear();
    disableButton();

    pixmap.fill(Qt::white);
    this->repaint(0, 0, QWidget::width(), QWidget::height());
    //update();

    emit(closeImage());
    this->close();
}

/*void Photos::mousePressEvent(QMouseEvent *event)
{
    /*if(image_width > widget_width || image_height > widget_height)
    {
        if(event->button()== Qt::LeftButton)
        {
            mouse_flag = 1;
        }
    }
    qDebug()<<widget_width;
    qDebug()<<image_width;
}

void Photos::mouseMoveEvent(QMouseEvent *event)
{
    if(image_width > widget_width || image_height > widget_height)
    {

        int final_x, final_y;
        final_x = final_y = 0;
        if((event->button() & Qt::LeftButton) && mouse_flag == 1)
        {
            original_x += (event->x() - final_x);
            original_y += (event->y() - final_y);
            qDebug()<< event->x();
            qDebug()<< event->y();
            //abs_x = qAbs(abs_x);
            //abs_y = qAbs(abs_y);
            this->repaint(original_x, original_y, image_width, image_height);
            update();
        }

        if(event->button() == Qt::LeftButton)
        {
            final_x = event->x();
            final_y = event->y();
            mouse_flag = 1;

        }
        else
        {
            mouse_flag = 0;
        }
        //if()
    }
}
*/

Photos::~Photos()
{
    delete ui;

}
