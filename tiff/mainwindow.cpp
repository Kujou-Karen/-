#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>  //头文件
#include <QDebug>
#include <QImageReader>
#include <QImageWriter>
#include <QWheelEvent>
#include <QScrollBar>
#include <QPaintEvent>
#include <QImage>
#include <QPainter>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startPos = QPoint(0, 0);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    File_Num = 0;
    file_path = QFileDialog::getExistingDirectory(this, "请选择文件路径...");
    QDir *dir=new QDir(file_path);
    QStringList filter;
    filter<<"*.tif";
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    File_Num_Max = fileInfo->count();
    for(int i = 0;i<fileInfo->count(); i++)
    {
        qDebug()<<fileInfo->at(i).filePath();
        qDebug()<<fileInfo->at(i).fileName();
    }
    OpenImage(fileInfo->at(File_Num).filePath());
    file_path_MAX = fileInfo->at(File_Num).filePath();
}

void MainWindow::OpenImage(QString(name))
{
//    QPixmap qimage(name);
//    ui->label->setPixmap(qimage);
//    ui->label->show();

//    int scalHeight = ui->label->height();
//    int scalWidth = qimage.scaledToHeight(scalHeight).width();//根据label高度计算得到控件目标宽度

//    if(ui->label->width() < scalWidth)
//    {//再次调整，确保图片能够在控件中完整显示
//        scalWidth = ui->label->width();
//        scalHeight = qimage.scaledToWidth(scalWidth).height();
//    }

//    ui->label->setPixmap(qimage.scaled(scalWidth,scalHeight,Qt::KeepAspectRatio));

    Image.load(name);
            pix = pix.fromImage(Image);

            crtPix = pix;
            pixW = Image.width();            //图片宽
            pixH = Image.height();           //图片高
            qDebug()<<name<<pixW<<pixH;
            this->setWindowTitle("图片浏览器("+name+")");
            onResetClicked();

}



void MainWindow::on_pushButton_2_clicked()
{
    File_Num--;
    QDir *dir=new QDir(file_path);
    QStringList filter;
    filter<<"*.tif";
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));

    if(File_Num<0)
    {
        File_Num = fileInfo->count()-1;
    }
    OpenImage(fileInfo->at(File_Num).filePath());
    file_path_MAX = fileInfo->at(File_Num).filePath();
}

void MainWindow::on_pushButton_3_clicked()
{
    File_Num++;
    QDir *dir=new QDir(file_path);
    QStringList filter;
    filter<<"*.tif";
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    if(File_Num>=fileInfo->count())
    {
        File_Num = 0;
    }
    OpenImage(fileInfo->at(File_Num).filePath());
    file_path_MAX = fileInfo->at(File_Num).filePath();
}





void MainWindow::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);

     //设置渲染方式
     painter.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing|QPainter::TextAntialiasing);
     painter.drawRect(ui->widget->x()-1,ui->widget->y()-1,ui->widget->width()+1,ui->widget->height()+1); //画框

     if(Image.isNull())
     {
      return;
     }

     int NowW = ratio *pixW;
     int NowH = ratio *pixH;

     if(action==MainWindow::Shrink)   //缩小
     {
           ratio-=0.05*ratio;
         if(ratio<0.018)
           ratio = 0.01;

         /*显示比例*/
        // QString str;
        // str=str.setNum(ratio*100)+"%";
         //str.sprintf("%.0f%",ratio*100);
         //label.setText(str) ;
        // qDebug()<<"缩小:"<<ratio;
     }
     else  if(action==MainWindow::Amplification) //放大
     {

          ratio+=0.05*ratio;
        if(ratio>4.5)
          ratio = 5.000;

         /*显示比例*/
//         QString str;
//         str=str.setNum(ratio*100)+"%";;
//         //str.sprintf("%.0f%",ratio*100);
//         label.setText(str);
//         qDebug()<<"放大:"<<ratio;
     }


     if(action==MainWindow::Amplification || action==MainWindow::Shrink || action==MainWindow::Reset)      //更新图片
     {
       NowW = ratio *pixW;
       NowH = ratio *pixH;


       crtPix= pix.scaled(NowW, NowH,Qt::KeepAspectRatio,Qt::SmoothTransformation); //重新装载

       action=MainWindow::None;
     }

     if(action==MainWindow::Move)                    //移动
     {
         int offsetx=Alloffset.x()+offset.x();
         Alloffset.setX(offsetx);

         int offsety=Alloffset.y()+offset.y();
         Alloffset.setY(offsety);
         action=MainWindow::None;
     }


     if(isBriAndConChanged)
             {
                 Image = ImageAdjustment(Image);
                 isBriAndConChanged = false;

             }

     if(abs(Alloffset.x())>=(ui->widget->width()/2 + NowW/2 -10))    //限制X偏移值
     {
         if(Alloffset.x()>0)
             Alloffset.setX(ui->widget->width()/2 + NowW/2 -10);
         else
          Alloffset.setX(-ui->widget->width()/2 + -NowW/2 +10);

     }
     if(abs(Alloffset.y())>=(ui->widget->height()/2 + NowH/2 -10))    //限制Y偏移值
     {
         if(Alloffset.y()>0)
             Alloffset.setY(ui->widget->height()/2 + NowH/2 -10);
         else
          Alloffset.setY(-ui->widget->height()/2 + -NowH/2 +10);

     }

     int x = ui->widget->width()/2 + Alloffset.x() -NowW/2;
     if(x<0)
         x=0;

     int y = ui->widget->height()/2 + Alloffset.y() -NowH/2;
     if(y<0)
         y=0;

     int  sx = NowW/2 - ui->widget->width()/2 - Alloffset.x();
     if(sx<0)
         sx=0;

     int  sy = NowH/2 - ui->widget->height()/2 - Alloffset.y();
     if(sy<0)
         sy=0;

     int w =(NowW - sx)>ui->widget->width()? ui->widget->width() : (NowW - sx);
     if(w>(ui->widget->width()-x))
         w = ui->widget->width()-x;

     int h =(NowH - sy)>ui->widget->height()? ui->widget->height() : (NowH - sy);
     if(h>(ui->widget->height()-y))
         h = ui->widget->height()-y;

     if(action==MainWindow::Setrgb)                    //移动
     {
         Image = ImageAdjustment1(Image);
         action=MainWindow::None;

     }



     painter.drawTiledPixmap(x+ui->widget->x(),y+ui->widget->y(),w,h,crtPix,sx,sy);  //绘画图形
     QRect picRect(x+ui->widget->x(), y+ui->widget->y(),w, h);
             painter.drawImage(picRect, Image);



 }


void MainWindow::onResetClicked()
 {
   action=MainWindow::Reset;
   Alloffset.setX(0);
   Alloffset.setY(0);
   ratio = 1.000;
  // label.setText("100%");
   this->update();
 }


//鼠标按下拖动事件
 bool MainWindow::event(QEvent * event)
 {
     static bool press=false;
     static QPoint PreDot;
    //检测鼠标按下
     if(event->type() == QEvent::MouseButtonPress)
     {
            QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);

            //判断鼠标是否是左键按下,且鼠标位置是否在绘画区域
            //if(mouse->button()==Qt::LeftButton &&Paint.contains(mouse->pos()))
                if(mouse->button()==Qt::LeftButton)
            {
                press=true;
                QApplication::setOverrideCursor(Qt::OpenHandCursor); //设置鼠标样式

                PreDot = mouse->pos();
            }

     }
     //检测鼠标松开
     else if(event->type() == QEvent::MouseButtonRelease)
     {
         QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);

         //判断鼠标是否是左键释放,且之前是在绘画区域
         if(mouse->button()==Qt::LeftButton && press )
         {
             QApplication::setOverrideCursor(Qt::ArrowCursor); //改回鼠标样式
             press=false;
         }
     }
    //检测鼠标移动
      if(event->type() == QEvent::MouseMove)              //移动图片
      {
           if(press)
          {
             QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);

             offset.setX(mouse->x() - PreDot.x());
             offset.setY(mouse->y() - PreDot.y());
             PreDot = mouse->pos();
             action = MainWindow::Move;

             this->update();
          }
      }
     return QWidget::event(event);
 }

 //鼠标滑轮事件
  void MainWindow::wheelEvent(QWheelEvent* event)
  {
   if (event->angleDelta().y()>0) {      //上滑,缩小

      action=MainWindow::Shrink;
      this->update();
   } else {                    //下滑,放大
       action=MainWindow::Amplification;
       this->update();
   }

   event->accept();
  }



void MainWindow::on_pushButton_4_clicked()
{
    action=MainWindow::Shrink;
       this->update();
}

void MainWindow::on_pushButton_5_clicked()
{
    action=MainWindow::Amplification;
       this->update();
}

void MainWindow::on_pushButton_8_clicked()
{
    action=MainWindow::Move;
    offset.setX(-20);
    offset.setY(0);

    this->update();
}

void MainWindow::on_pushButton_9_clicked()
{
    action=MainWindow::Move;
       offset.setX(20) ;
       offset.setY(0) ;

       this->update();
}

void MainWindow::on_pushButton_6_clicked()
{
    action=MainWindow::Move;
       offset.setX(0);
       offset.setY(-20);

       this->update();
}

void MainWindow::on_pushButton_7_clicked()
{
    action=MainWindow::Move;
       offset.setX(0);
       offset.setY(20);
       this->update();
}

void MainWindow::on_pushButton_10_clicked()
{
    action=MainWindow::Reset;
       Alloffset.setX(0);
       Alloffset.setY(0);
       ratio = 1.000;
       //label.setText("100%");
       this->update();
}



void MainWindow::on_horizontalSlider_sliderMoved(int position)
{

    brightness = position;
    isBriAndConChanged = true;
    update();

}

QImage MainWindow::Mat2QImage(const Mat &mat)
{
    //8位，通道数为1
    if (mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        image.setColorCount(256);
        for (int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }

        uchar *pSrc = mat.data;
        for (int row = 0; row < mat.rows; row++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    //8位，通道数为3
    else if (mat.type() == CV_8UC3)
    {
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    //8位，通道数为4
    else if (mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";

        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    //32位，通道数为3
    else if (mat.type() == CV_32FC3)
    {
        cv::Mat temp;
        mat.convertTo(temp, CV_8UC3);

        const uchar *pSrc = (const uchar*)temp.data;
        QImage image(pSrc, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

Mat MainWindow::QImageToMat(QImage image)
{
    cv::Mat mat;
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

QImage MainWindow::ImageAdjustment(QImage image)
{
//    Mat image_Mat = QImageToMat(image);
    image_Mat = cv::imread(file_path_MAX.toLocal8Bit().toStdString());
    image_Mat.convertTo(image_Mat, -1,static_cast<double>(contrast),static_cast<double>(brightness));
    return Mat2QImage(image_Mat);
}

void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{
    contrast = position;
        isBriAndConChanged = true;
        update();
}

void MainWindow::on_pushButton_11_clicked()
{
    action=MainWindow::Setrgb;
       this->update();
}

QImage MainWindow::ImageAdjustment1(QImage image)
{

    image_Mat = cv::imread(file_path_MAX.toLocal8Bit().toStdString());
    cvtColor(image_Mat, gray_src, CV_BGR2GRAY);
    int height = gray_src.rows;
        int width = gray_src.cols;
        for (int row = 0; row < height; row++)
        {
            for (int col = 0; col < width; col++)
            {
                int gray = gray_src.at<uchar>(row, col);
                gray_src.at<uchar>(row, col) = 255 - gray;
            }
        }

    return Mat2QImage(gray_src);
}
