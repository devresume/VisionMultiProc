#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "procconfiginfo.h"

#include <string>
#include <vector>

#include <QMainWindow>
#include <QLabel>
#include <QRadioButton>
#include <QGroupBox>
#include <QSlider>
#include <QPushButton>
#include <QSignalMapper>
#include <QFormLayout>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/xfeatures2d.hpp"

#define MAX_SLIDERS 6
#define NUM_PROCESSORS 3
#define CANNY_IDX 0
#define HARRIS_IDX 1
#define SIFT_IDX 2

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loadBtnFile_clicked();
     void callProcess();

//    void radio1Toggled(bool val);
//    void radio2Toggled(bool val);
//    void radio3Toggled(bool val);

//    void on_Change_Slider1(int i);
//    void on_Change_Slider2(int i);
//    void on_Change_Slider3(int i);
//    void on_Change_Slider4(int i);
//    void on_Change_Slider5(int i);
//    void on_Change_Slider6(int i);

private:
    void initControls();
    void layoutControls();
    void initConfigInto();
    void createMapCtrls();

    void keyPointsDetect();
    void recreateCanny();
    void cannnyEdge_detect();
    void cornerHarris_detect();
    void recreateHarris();
    void recreateFeatures();

    Ui::MainWindow *ui;

    int curProc = 0;
    bool* hasChanged;
//    int* sliderVals;

    cv::Mat drawTemp;

    QLabel* lblProcessed;
    QLabel* lblOriginal;

    QGroupBox *groupBox;
    QRadioButton* radio;
//    QRadioButton* radio2;
//    QRadioButton* radio3;

    QLabel* sliderLbl;
//    QLabel* sliderLbl2;
//    QLabel* sliderLbl3;
//    QLabel* sliderLbl4;
//    QLabel* sliderLbl5;
//    QLabel* sliderLbl6;

    QSlider* slider;
//    QSlider* slider2;
//    QSlider *slider3;
//    QSlider *slider4;
//    QSlider *slider5;
//    QSlider *slider6;

    QPushButton* loadBtn;
    QPushButton* pauseBtn;


    cv::VideoCapture capWebcam;
    cv::Mat matOriginal;
    cv::Mat matProcessed;
    cv::Mat matProcessed2;
    cv::Mat matProcessedOut;
    cv::Mat matOriginalOut;
    cv::Mat resizeOriginal;
    cv::Mat matOriginal2;
    cv::Mat matOriginal3;
    cv::Mat matOriginal4;
    cv::Mat matDetected;

    cv::Mat matOriginalOut1;
    cv::Mat matOriginalColorOut1;
    cv::Mat matOriginalColorOut2;

    cv::cuda::GpuMat gpu_src;
    cv::cuda::GpuMat gpu_src2;
    cv::cuda::GpuMat gpu_src3;
    cv::cuda::GpuMat gpu_src4;

    cv::Mat dst;
    cv::Mat dst_norm;
    cv::Mat dst_norm_scaled;
    cv::Mat dst_norm_scaled_color;

    cv::cuda::GpuMat resizeProcessed;
    cv::cuda::GpuMat gpuMatOriginal;
    cv::cuda::GpuMat gpuMatOriginalResized;
    cv::cuda::GpuMat gpuMatProcessed;
    cv::cuda::GpuMat gpuMatProcessed2;
    cv::cuda::GpuMat gpuMatProcessed3;
    cv::cuda::GpuMat gpuMatProcessedOut;


    cv::Ptr<cv::cuda::CannyEdgeDetector> canny;
    cv::Ptr<cv::cuda::CornernessCriteria> harris;
    cv::Ptr<cv::xfeatures2d::SIFT> features;

    std::vector<cv::KeyPoint> kp;

    // For harris corner detection
    cv::cuda::GpuMat src, src_gray;

    std::vector<cv::Vec3f> vecCircles;
    std::vector<cv::Vec3f>::iterator itrCircles;

    QTimer* tmrTimer;
    QSlider* horizontalSlider;
    QString vidFile;

    int thresh = 200;
    int max_thresh = 255;

    QHBoxLayout* procRadioLayout;
    QFormLayout* inputsLayout;
    std::vector<ProcConfigInfo*>* procConfigInfo;

};

#endif // MAINWINDOW_H
