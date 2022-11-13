#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int File_Num;
    int File_Num_Max;

    void OpenImage(QString(name));

    QString file_path;

    QString file_path_MAX;
    QPoint startPos;

    Mat image_Mat;
    Mat gray_src;


   void paintEvent(QPaintEvent *event);
   void    onResetClicked();

   bool event(QEvent * event);

   void wheelEvent(QWheelEvent* e);     //鼠标滑轮事件

   QImage Mat2QImage(const Mat& mat);       //Mat2QImage格式转换
       Mat QImageToMat(QImage image);           //QImage2Mat格式转换
       QImage ImageAdjustment(QImage image);    //改变图片的亮度和对比度
       QImage ImageAdjustment1(QImage image);    //改变图片的亮度和对比度

   QImage Image;

   int pixW;            //图片宽
   int pixH;            //图片高
   int action;          //动作(放大,缩小,移动...)


       float contrast = 1;



   float brightness = 0;
   bool isBriAndConChanged = false;
   QPixmap  pix;
       QPixmap  crtPix;
       QPoint offset;           //一次的图片偏移值
           QPoint Alloffset;          //总偏移

   enum  Type {
       None          = 0,
       Amplification ,
       Shrink,
       Lift,
       Right,
       Up,
       Down,
       Move,
       Reset,
       Setrgb
   };



   float ratio;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();


    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_pushButton_11_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
