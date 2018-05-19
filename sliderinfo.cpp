#include "sliderinfo.h"

SliderInfo::SliderInfo(int idx,bool* hasChanged, int min, int max, int value, int denom, bool isdivide, bool enabled, std::string name) :
    SliderInfo(idx, hasChanged, min, max, value, denom, isdivide,  enabled,  name, false){
}

SliderInfo::SliderInfo(int idx,bool* hasChanged, int min, int max, int value, int denom, bool isdivide, bool enabled, std::string name, bool isOdd){
    this->idx = idx;
    this->hasChanged = hasChanged;
    this->min = min;
    this->max = max;
    this->value = value;
    this->denom = denom;
    this->isdivide = isdivide;
    this->enabled = enabled;
    this->name = name;
    this->isOdd = isOdd;
}

void SliderInfo::changed_Slider(int value){
    if( isOdd){
        value = value % 2 ? value : --value;
    }
    this->value = value;
    *hasChanged = true;
}

int SliderInfo::getIdx(){
    return this->idx;
}

void SliderInfo::setIdx(int idx){
    this->idx = idx;
}

int SliderInfo::getMin(){
    return this->min;
}

void SliderInfo::setMin(int i){
    this->min = i;
}

int SliderInfo::getMax(){
    return this->max;
}

void SliderInfo::setMax(int i){
    this->max = i;
}

int SliderInfo::getValue(){
    return this->value;
}

void SliderInfo::setValue(int value){
    this->value = value;
}

int SliderInfo::getDenom(){
    return this->denom;
}

void SliderInfo::setDenom(int denom){
    this->denom = denom;
}

bool SliderInfo::getIsDivide(){
    return this->isdivide;
}

void SliderInfo::setIsDivide(bool isdivide){
    this->isdivide = isdivide;
}

bool SliderInfo::getEnable(){
    return this->enabled;
}

void SliderInfo::setEnabled(bool b){
    this->enabled = b;
}

std::string SliderInfo::getName(){
    return this->name;
}

void SliderInfo::setName(std::string name){
    this->name = name;
}

void SliderInfo::setControls(QLabel* label, QSlider* slider){
    this->label = label;
    this->slider = slider;
}

void SliderInfo::setEnabledCtrls(bool b){
    this->label->setVisible(b);
    this->label->setEnabled(b);
    this->slider->setVisible(b);
    this->slider->setEnabled(b);
}

QSlider* SliderInfo::getSlider(){
    return this->slider;
}
