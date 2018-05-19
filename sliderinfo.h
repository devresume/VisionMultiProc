#ifndef SLIDERINFO_H
#define SLIDERINFO_H

#include <QObject>
#include <QSlider>
#include <QLabel>
#include <string>

class SliderInfo : public QObject
{
    Q_OBJECT
private:
    int idx, min, max, value, denom;
    bool* hasChanged;
    bool enabled;
    bool isdivide;
    std::string name;
    QSlider* slider;
    QLabel* label;
    bool isOdd;
public slots:
    void changed_Slider(int value);

signals:
    void valueChanged(int newValue);
public:
    SliderInfo(int idx, bool* hasChanged, int min, int max, int value, int denom, bool isdivide, bool enabled, std::string name);
    SliderInfo(int idx, bool* hasChanged, int min, int max, int value, int denom, bool isdivide, bool enabled, std::string name, bool isOdd);
    int getIdx();
    void setIdx(int idx);
    int getMin();
    void setMin(int i);
    int getMax();
    void setMax(int i);
    int getValue();
    void setValue(int value);
    int getDenom();
    void setDenom(int denom);
    bool getIsDivide();
    void setIsDivide(bool isdivide);
    bool getEnable();
    void setEnabled(bool b);
    std::string getName();
    void setName(std::string name);
    void setControls(QLabel* label, QSlider* slider);
    void setEnabledCtrls(bool enabled);
    QSlider* getSlider();

};

#endif // SLIDERINFO_H
