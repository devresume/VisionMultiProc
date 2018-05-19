#ifndef PROCCONFIGINFO_H
#define PROCCONFIGINFO_H
#include <vector>
#include <string>
#include <QRadioButton>
#include "sliderinfo.h"

class ProcConfigInfo : public QObject
{
    Q_OBJECT
public slots:
    void radioToggled(bool value);
signals:
    void toggled(bool value);
private:
    bool* hasChanged;
    int* refCurProc;
    int procIdx;
    std::string name;
    QRadioButton* radio;
    std::vector<SliderInfo*>* sliders;
public:
    ProcConfigInfo(int* refCurProc, int procIdx, std::string name);
    void addSlider(SliderInfo* slider);
    SliderInfo* getSlider(int idx);
    std::vector<SliderInfo*>* getSliders();
    int getIdx();
    std::string getName();
    QRadioButton* getRadio();
    void setRadio(QRadioButton* radio);
    void enableAllCtrls(bool enabled);
};

#endif // PROCCONFIGINFO_H
