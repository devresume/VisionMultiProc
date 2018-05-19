#include "procconfiginfo.h"
#include <vector>

ProcConfigInfo::ProcConfigInfo(int* refCurProc, int procIdx, std::string name)
{
    this->refCurProc = refCurProc;
    this->procIdx = procIdx;
    this->name = name;
    this->sliders = new std::vector<SliderInfo*>();
}

void ProcConfigInfo::radioToggled(bool value){
    *this->refCurProc = procIdx;
    this->enableAllCtrls(value);
}

void ProcConfigInfo::addSlider(SliderInfo* slider){
    this->sliders->push_back(slider);
}

SliderInfo* ProcConfigInfo::getSlider(int idx){
    return this->sliders->at(idx);
}

std::vector<SliderInfo*>* ProcConfigInfo::getSliders(){
    return this->sliders;
}

int ProcConfigInfo::getIdx(){
    return this->procIdx;
}

std::string ProcConfigInfo::getName(){
    return this->name;
}


QRadioButton* ProcConfigInfo::getRadio(){
    return this->radio;
}

void ProcConfigInfo::setRadio(QRadioButton* radio){
    this->radio = radio;
}

void ProcConfigInfo::enableAllCtrls(bool enabled){
    for(std::vector<SliderInfo*>::iterator iter = sliders->begin(); iter != sliders->end(); ++iter ){
        (*iter)->setEnabledCtrls(enabled);
    }
}
