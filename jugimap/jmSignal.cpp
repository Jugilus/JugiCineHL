#include <string>
#include <sstream>
#include "jmCommonFunctions.h"
#include "jmCommon.h"
#include "jmSignal.h"



namespace jugimap {





void Signal::preUpdateSignals(std::vector<Signal*> &_signals)
{
    for(Signal* s : _signals){

        if(s->mType==ValueStateType::BOOL){
            static_cast<BoolSignal*>(s)->preUpdate();

        }else if(s->mType==ValueStateType::INT){
            static_cast<IntSignal*>(s)->preUpdate();

        }else if(s->mType==ValueStateType::INT_BITS){
            static_cast<IntBitsSignal*>(s)->preUpdate();

        }
    }


}


void Signal::postUpdateSignals(std::vector<Signal*> &_signals)
{

    for(Signal* s : _signals){

        if(s->mType==ValueStateType::BOOL){
            static_cast<BoolSignal*>(s)->postUpdate();

        }else if(s->mType==ValueStateType::INT){
            static_cast<IntSignal*>(s)->postUpdate();

        }else if(s->mType==ValueStateType::INT_BITS){
            static_cast<IntBitsSignal*>(s)->postUpdate();

        }
    }
}

void Signal::resetSignals(std::vector<Signal*> &_signals)
{

    for(Signal* s : _signals){

        if(s->mType==ValueStateType::BOOL){
            static_cast<BoolSignal*>(s)->reset();

        }else if(s->mType==ValueStateType::INT){
            static_cast<IntSignal*>(s)->reset();

        }else if(s->mType==ValueStateType::INT_BITS){
            static_cast<IntBitsSignal*>(s)->reset();

        }
    }
}




void BoolSignal::_Set(bool _active)
{

    if(_active==true){
        if(mValue==false){
            mValue = true;
            mValueStarted = true;
            mValueEnded = false;
        }
    }else{
        if(mValue==true){
            mValue = false;
            mValueStarted = false;
            mValueEnded = true;
        }
    }

}





}
