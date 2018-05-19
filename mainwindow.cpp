#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    hasChanged = new bool(false);
    procConfigInfo = new std::vector<ProcConfigInfo*>();

    initConfigInto();

    // harris cached circle
    drawTemp = cv::Mat(13, 13, CV_8UC4, cv::Scalar(0));
    cv::circle(drawTemp, cv::Point( 6, 6 ), 5, cv::Scalar(0, 0, 255, 255), 2, 8, 0);

    initControls();
    layoutControls();

    createMapCtrls();

    connect(loadBtn, SIGNAL(clicked(bool)), this, SLOT(on_loadBtnFile_clicked()));

    recreateFeatures();
    recreateHarris();
    canny = cv::cuda::createCannyEdgeDetector(1, 50);
    kp = std::vector<cv::KeyPoint>();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::createMapCtrls(){
    for(std::vector<ProcConfigInfo*>::iterator iter = procConfigInfo->begin(); iter != procConfigInfo->end(); ++iter){
        radio = new QRadioButton(tr((*iter)->getName().c_str()));
        (*iter)->setRadio(radio);
        connect(radio, SIGNAL(toggled(bool)),(*iter), SLOT(radioToggled(bool)));
//        radio->setVisible(true);
        procRadioLayout->addWidget(radio);
        for(std::vector<SliderInfo*>::iterator iter2 = (*iter)->getSliders()->begin(); iter2 != (*iter)->getSliders()->end(); ++iter2){
            slider = new QSlider(Qt::Orientation::Horizontal);
            slider->setMinimum((*iter2)->getMin());
            slider->setMaximum((*iter2)->getMax());
            sliderLbl = new QLabel(tr((*iter2)->getName().c_str()));
            slider->setValue((*iter2)->getValue());
            slider->setVisible(false);
            sliderLbl->setVisible(false);
            inputsLayout->addRow(sliderLbl, slider);
            (*iter2)->setControls(sliderLbl, slider);
            connect(slider, SIGNAL(valueChanged(int)), (*iter2), SLOT(changed_Slider(int)));
        }

    }

}

void MainWindow::initControls(){
    lblProcessed = new QLabel();
    lblProcessed->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lblOriginal = new QLabel();
    lblOriginal->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    groupBox = new QGroupBox(tr("Process Type"));



    loadBtn = new QPushButton(tr("Load Video"));
    pauseBtn = new QPushButton(tr("Pause Video"));

}

void MainWindow::layoutControls(){
    QHBoxLayout* vidLayout = new QHBoxLayout;
    vidLayout->addWidget(lblProcessed);
    vidLayout->addWidget(lblOriginal);

    procRadioLayout = new QHBoxLayout;

    groupBox->setLayout(procRadioLayout);

    inputsLayout = new QFormLayout;

    QHBoxLayout* btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(loadBtn);
    btnsLayout->addWidget(pauseBtn);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addLayout(vidLayout, 0, 0);
    mainLayout->addWidget(groupBox, 1, 0);
    mainLayout->addLayout(inputsLayout, 2, 0);
    mainLayout->addLayout(btnsLayout, 3, 0);

    QWidget *window = new QWidget();
            window->setLayout(mainLayout);

    setCentralWidget(window);
}

void MainWindow::initConfigInto(){
    int procoffset = 0;
    int slideroffset = 0;

    //Canny
    ProcConfigInfo* tempProc = new ProcConfigInfo(&this->curProc, procoffset++, "Canny Edge");
    SliderInfo* tempslider = new SliderInfo(slideroffset++, this->hasChanged, 0, 100, 50, 0, false, false, "Low Threshold");
    tempProc->addSlider(tempslider);
    tempslider = new SliderInfo(slideroffset++, this->hasChanged, 0, 300, 200, 0, false, false, "High Threshold");
    tempProc->addSlider(tempslider);
    tempslider = new SliderInfo(slideroffset++, this->hasChanged, 5, 500, 60, 0, false, false, "Play Speed");
    tempProc->addSlider(tempslider);
    this->procConfigInfo->push_back(tempProc);

    //Harris
    tempProc = new ProcConfigInfo(&this->curProc, procoffset++, "Harris Corner");
    tempslider = new SliderInfo(slideroffset++, this->hasChanged, 1, 11, 2, 0, false, false, "Block Size");
    tempProc->addSlider(tempslider);
    tempslider = new SliderInfo(slideroffset++, this->hasChanged, 5, 31, 3, 0, false, false, "Appeture Size", true);
    tempProc->addSlider(tempslider);
    tempslider = new SliderInfo(slideroffset++, this->hasChanged, 2, 10, 4, 100, true, false, "Free Value");
    tempProc->addSlider(tempslider);
    tempslider = new SliderInfo(slideroffset++, this->hasChanged, 5, 500, 60, 0, false, false, "Play Speed");
    tempProc->addSlider(tempslider);
    this->procConfigInfo->push_back(tempProc);

    //sift
    tempProc = new ProcConfigInfo(&this->curProc, procoffset++, "SIFT Keypoint");
    tempslider = new SliderInfo(slideroffset++, this->hasChanged, 0, 20, 10, 0, false, false, "Count Best KP's");
    tempProc->addSlider(tempslider);
    tempslider = new SliderInfo(slideroffset++, this->hasChanged, 3, 6, 3, 0, false, false, "Octacve Layers");
    tempProc->addSlider(tempslider);
    tempslider = new SliderInfo(slideroffset++, this->hasChanged, 1, 10, 4, 100, true, false, "Contrast Threshold");
    tempProc->addSlider(tempslider);
    tempslider = new SliderInfo(slideroffset++, this->hasChanged, 1, 20, 10, 0, false, false, "Edge Threshold");
    tempProc->addSlider(tempslider);
    tempslider = new SliderInfo(slideroffset++, this->hasChanged, 2, 20, 16, 10, true, false, "Sigma");
    tempProc->addSlider(tempslider);
    tempslider = new SliderInfo(slideroffset++, this->hasChanged, 5, 500, 60, 0, false, false, "Play Speed");
    tempProc->addSlider(tempslider);
    this->procConfigInfo->push_back(tempProc);

}

void MainWindow::on_loadBtnFile_clicked()
{
    vidFile = QFileDialog::getOpenFileName(this, tr("Open Video"), "./", tr("All files (*.*)"));
    capWebcam = cv::VideoCapture(vidFile.toStdString());
    double fps = capWebcam.get(cv::CAP_PROP_FPS);

    procConfigInfo->at(CANNY_IDX)->getRadio()->setChecked(true);

    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(callProcess()));
    tmrTimer->start(1000 / fps);

}



void MainWindow::callProcess(){
    ProcConfigInfo* tempProc = procConfigInfo->at(curProc);
    tmrTimer->setInterval(tempProc->getSliders()->at(tempProc->getSliders()->size() - 1)->getValue());
    if(curProc == CANNY_IDX){
        if(*hasChanged){
            recreateCanny();
            *hasChanged = false;
        }
        cannnyEdge_detect();
    } else if(curProc == HARRIS_IDX){
        if(*hasChanged){
            recreateHarris();
            *hasChanged = false;
        }
        cornerHarris_detect();
    } else if(curProc == SIFT_IDX){
        if(*hasChanged){
            recreateFeatures();
            *hasChanged = false;
        }
        keyPointsDetect();
    }
}

void MainWindow::recreateFeatures(){
//    int procIdx = 2;
//    int k = procIdx * MAX_SLIDERS;
//    int offset = 0;



//    int nfeatures = (*(sliderVals + procIdx + offset++));
//    int nOctaveLayers = (*(sliderVals + procIdx + offset++));
//    double contrastThreshold = ((double)(*(sliderVals + procIdx + offset++)) / double(100));
//    double edgeThreshold = ((double)(*(sliderVals + procIdx + offset++)));
//    double sigma = ((double)(*(sliderVals + procIdx + offset++)) / double(10));
    ProcConfigInfo* tempProc = procConfigInfo->at(SIFT_IDX);

    features = cv::xfeatures2d::SIFT::create(tempProc->getSlider(0)->getValue(), tempProc->getSlider(1)->getValue(),
    ((double)tempProc->getSlider(2)->getValue()) / ((double)tempProc->getSlider(2)->getDenom()),
    (double)tempProc->getSlider(3)->getValue(),
    ((double)tempProc->getSlider(4)->getValue()) / ((double)tempProc->getSlider(4)->getDenom()));
}

void MainWindow::keyPointsDetect(){
    capWebcam.read(matOriginal);
    if(matOriginal.empty() == true) return;

    // output to original label
    cv::resize(matOriginal, matOriginalOut1, cv::Size(lblOriginal->width(),lblOriginal->height()));
    cv::cvtColor(matOriginalOut1, matOriginalColorOut1, cv::COLOR_BGR2RGB);
    QImage qimgProcessed2((uchar*)matOriginalColorOut1.data, matOriginalColorOut1.cols, matOriginalColorOut1.rows, matOriginalColorOut1.step, QImage::Format_RGB888);
    lblOriginal->setPixmap(QPixmap::fromImage(qimgProcessed2));

    cv::resize(matOriginal, matOriginal2, cv::Size(lblProcessed->width(),lblProcessed->height()));
    cv::cvtColor( matOriginal2, matOriginal3, cv::COLOR_BGR2GRAY );


    features->detect(matOriginal3, kp);

    cv::drawKeypoints(matOriginal3, kp, matProcessed);

    cv::cvtColor(matProcessed, matOriginalColorOut2, cv::COLOR_BGR2RGB);
    QImage qimgProcessed((uchar*)matOriginalColorOut2.data, matOriginalColorOut2.cols, matOriginalColorOut2.rows, matOriginalColorOut2.step, QImage::Format_RGB888);
    lblProcessed->setPixmap(QPixmap::fromImage(qimgProcessed));
}

void MainWindow::recreateHarris(){
    ProcConfigInfo* tempProc = procConfigInfo->at(HARRIS_IDX);
    harris = cv::cuda::createHarrisCorner(CV_32FC1, tempProc->getSlider(0)->getValue(), tempProc->getSlider(1)->getValue(),((double)tempProc->getSlider(2)->getValue()) / ((double)tempProc->getSlider(2)->getDenom()), cv::BORDER_DEFAULT);
}

void MainWindow::cornerHarris_detect( )
{
    capWebcam.read(matOriginal);
    if(matOriginal.empty() == true) return;

    // output to original label
    cv::resize(matOriginal, matOriginalOut1, cv::Size(lblOriginal->width(),lblOriginal->height()));
    cv::cvtColor(matOriginalOut1, matOriginalColorOut1, cv::COLOR_BGR2RGB);
    QImage qimgProcessed2((uchar*)matOriginalColorOut1.data, matOriginalColorOut1.cols, matOriginalColorOut1.rows, matOriginalColorOut1.step, QImage::Format_RGB888);
    lblOriginal->setPixmap(QPixmap::fromImage(qimgProcessed2));

    cv::resize(matOriginal, matOriginal2, cv::Size(lblProcessed->width(),lblProcessed->height()));

    cv::cvtColor( matOriginal2, matOriginal3, cv::COLOR_BGR2GRAY );

//    gpu_src2.create(matOriginal3.size(), CV_32FC1);
    gpu_src.upload(matOriginal3);
    gpu_src.convertTo(gpu_src2, CV_32FC1);


    gpu_src3 = cv::cuda::GpuMat(gpu_src2.size(), CV_32FC1);
    matOriginal4 = cv::Mat::zeros(matOriginal2.size(), CV_32FC1);
    gpu_src3.upload(matOriginal4);

//    gpu_src2.upload(dst);

    // Detector parameters
//    int blockSize = 2;
//    int apertureSize = 3;
//    double k = 0.04;

    // Detecting corners
//    cv::cornerHarris( src_gray, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT );

        harris->compute(gpu_src2, gpu_src3);



//     gpu_src3.download(matProcessedOut);

    /// Normalizing
//    cv::cuda::normalize( gpu_src3, gpu_src4, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::cuda::GpuMat() );
    gpu_src3.download(dst_norm);

//    cv::normalize( dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );


    cv::convertScaleAbs( dst_norm, dst_norm_scaled );

    cv::cvtColor( dst_norm_scaled, dst_norm_scaled_color, cv::COLOR_GRAY2BGRA );

    /// Drawing a circle around corners
    for( int j = 0; j < dst_norm.rows && j < dst_norm_scaled_color.size().height - 13 ; j++ )
    {
        for( int i = 0; i < dst_norm.cols && i < dst_norm_scaled_color.size().width - 13; i++ )
        {
            if( (int) dst_norm.at<float>(j,i) > thresh )
            {
                //cv::circle( dst_norm_scaled_color, cv::Point( i, j ), 5, cv::Scalar( 0, 0, 255 ), 2, 8, 0 );

                drawTemp.copyTo(dst_norm_scaled_color.rowRange(j, j + 13).colRange(i,i + 13), drawTemp);
            }
        }
    }

  /// Showing the result
//  namedWindow( corners_window, CV_WINDOW_AUTOSIZE );
//  imshow( corners_window, dst_norm_scaled );

    cv::cvtColor(dst_norm_scaled_color, matOriginalColorOut2, cv::COLOR_BGRA2RGB);
    QImage qimgProcessed((uchar*)matOriginalColorOut2.data, matOriginalColorOut2.cols, matOriginalColorOut2.rows, matOriginalColorOut2.step, QImage::Format_RGB888);
    lblProcessed->setPixmap(QPixmap::fromImage(qimgProcessed));
}
void MainWindow::recreateCanny(){
    ProcConfigInfo* tempProc = procConfigInfo->at(CANNY_IDX);
    canny->setLowThreshold(tempProc->getSlider(0)->getValue());
    canny->setHighThreshold(tempProc->getSlider(1)->getValue());
}

void MainWindow::cannnyEdge_detect()
{

    capWebcam.read(matOriginal);
    if(matOriginal.empty() == true) return;

    cv::resize(matOriginal, matOriginalOut1, cv::Size(lblOriginal->width(),lblOriginal->height()));
    cv::cvtColor(matOriginalOut1, matOriginalColorOut1, cv::COLOR_BGR2RGB);
    QImage qimgProcessed2((uchar*)matOriginalColorOut1.data, matOriginalColorOut1.cols, matOriginalColorOut1.rows, matOriginalColorOut1.step, QImage::Format_RGB888);
    lblOriginal->setPixmap(QPixmap::fromImage(qimgProcessed2));

    cv::resize(matOriginal, matOriginal2, cv::Size(lblProcessed->width(),lblProcessed->height()));

    cv::cvtColor(matOriginal2, matOriginal3, cv::COLOR_BGR2GRAY); // produces out2, a one-channel image (CV_8UC1)



//    GaussianBlur(gpuMatProcessed2, gpuMatProcessed3, cv::Size(slidervalue, slidervalue), 0, 0);
    cv::blur(matOriginal3, matDetected, cv::Size(3,3) );

    gpuMatOriginal.upload(matDetected);


    canny->detect(gpuMatOriginal, gpuMatProcessedOut);

    gpuMatProcessedOut.download(matProcessedOut);

    //gpuMatOriginalResized.download(matOriginalOut);

    //cv::Canny(gpuMatProcessed, gpuMatProcessed, slidervalue2, slidervalue2, 3);
    //cv::gpu::GaussianBlur(gpuMatOriginal, gpuMatProcessed, slidervalue, slidervalue);
    //cv::inRange(matOriginal, cv::Scalar(0,0,175), cv::Scalar(100,100,256), matProcessed);
    //cv::GaussianBlur(matProcessed, matProcessed, cv::Size(5,5), 1.5);
    //cv::HoughCircles(matProcessed, vecCircles, CV_HOUGH_GRADIENT, 2, matProcessed.rows / 4, 100, 50, 10, 400);
//    for( itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++){
//        ui->txtXYRadius->appendPlainText(QString("ball position x =") + QString::number((*itrCircles)[0]).rightJustified(4, ' ') +
//                QString(", y = ") + QString::number((*itrCircles)[1]).rightJustified(4, ' ') +
//                QString(", radius = ") + QString::number((*itrCircles)[2], 'f', 3).rightJustified(7, ' '));
//        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), 3, cv::Scalar(0,255,0), CV_FILLED);
//        cv::circle(matOriginal, cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), (int)(*itrCircles)[2], cv::Scalar(0,255,0), 3);
//    }
    //cv::cvtColor(matOriginal, matOriginal,CV_BGR2RGB);



    //QImage qimgOriginal((uchar*)matOriginalOut.data, matOriginalOut.cols, matOriginalOut.rows, matOriginalOut.step, QImage::Format_RGB888);
    QImage qimgProcessed((uchar*)matProcessedOut.data, matProcessedOut.cols, matProcessedOut.rows, matProcessedOut.step, QImage::Format_Indexed8);
    //ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));
    lblProcessed->setPixmap(QPixmap::fromImage(qimgProcessed));
}
