#include <string>
#include <sstream>
#include "jmCommonFunctions.h"
#include "jmCommon.h"
#include "jmStreams.h"
#include "jmSystem.h"
#include "jmSignal.h"


namespace jugimap {




void SignalUpdater::addSignal_forValueSetOnNextFrame(Signal *_signal)
{
    mPreUpdateSignals.push_back(_signal);
}


void SignalUpdater::addSignal_forPostUpdate(Signal *_signal)
{

    mPostUpdateSignals.push_back(_signal);
}


void SignalUpdater::preUpdateSignals()
{

    if(mPreUpdateSignals.empty()){
        return;
    }

    for(Signal* s : mPreUpdateSignals){

        if(s->type()==SignalType::BOOL){
            BoolSignal *sig = static_cast<BoolSignal*>(s);
            sig->setValue(sig->value_setOnNextFrame());
            //bs->preUpdate();

        }else if(s->type()==SignalType::INT){
            IntSignal* sig = static_cast<IntSignal*>(s);
            sig->setValue(sig->value_setOnNextFrame());


        }else if(s->type()==SignalType::FLOAT){
            FloatSignal* sig = static_cast<FloatSignal*>(s);
            sig->setValue(sig->value_setOnNextFrame());


        }else if(s->mType==SignalType::BITSET){
            BitsetSignal* sig = static_cast<BitsetSignal*>(s);
            sig->setValue(sig->value_setOnNextFrame());

            //bs->preUpdate();

        }else if(s->type()==SignalType::STRING){

            StringSignal* sig = static_cast<StringSignal*>(s);
            sig->setValue(sig->value_setOnNextFrame());
            //ss->preUpdate();

        }else if(s->type()==SignalType::BASE_OBJECT){

            ObjectSignal* sig = static_cast<ObjectSignal*>(s);
            sig->setValue(sig->value_setOnNextFrame());
            //ss->preUpdate();

        }

        s->mStored_forValueSetOnNextFrame = false;
    }

    mPreUpdateSignals.clear();

}


void SignalUpdater::postUpdateSignals()
{

    if(mPostUpdateSignals.empty()){
        return;
    }

    for(Signal* s : mPostUpdateSignals){

        if(s->type()==SignalType::BOOL){
            BoolSignal *sig = static_cast<BoolSignal*>(s);
            sig->postUpdate();

        }else if(s->type()==SignalType::INT){
            IntSignal* sig = static_cast<IntSignal*>(s);
            sig->postUpdate();

        }else if(s->type()==SignalType::FLOAT){
            FloatSignal* sig = static_cast<FloatSignal*>(s);
            sig->postUpdate();

        }else if(s->mType==SignalType::BITSET){
            BitsetSignal* sig = static_cast<BitsetSignal*>(s);
            sig->postUpdate();

        }else if(s->type()==SignalType::STRING){
            StringSignal* sig = static_cast<StringSignal*>(s);
            sig->postUpdate();

        }else if(s->type()==SignalType::BASE_OBJECT){
            ObjectSignal* sig = static_cast<ObjectSignal*>(s);
            sig->postUpdate();
        }

        s->mStored_forPostUpdate = false;
    }


    mPostUpdateSignals.clear();

}


void SignalUpdater::removeSignal_forValueSetOnNextFrame(Signal *_signal)
{

    StdVector::removeElement(mPreUpdateSignals, _signal);
    _signal->mStored_forValueSetOnNextFrame = false;

}


void SignalUpdater::removeSignal_forPostUpdate(Signal *_signal)
{

    StdVector::removeElement(mPostUpdateSignals, _signal);
    _signal->mStored_forPostUpdate = false;

}


SignalUpdater gSignalUpdater;



//==============================================================================



Signal::~Signal()
{

    if(mStored_forValueSetOnNextFrame){
        DummyFunction();
        gSignalUpdater.removeSignal_forValueSetOnNextFrame(this);
    }

    if(mStored_forPostUpdate){
        DummyFunction();
        gSignalUpdater.removeSignal_forPostUpdate(this);
    }


}


void Signal::resetSignals(std::vector<Signal*> &_signals)
{

    for(Signal* s : _signals){

        if(s->mType==SignalType::BOOL){
            static_cast<BoolSignal*>(s)->reset();

        }else if(s->mType==SignalType::INT){
            static_cast<IntSignal*>(s)->reset();

        }else if(s->mType==SignalType::FLOAT){
            static_cast<FloatSignal*>(s)->reset();

        }else if(s->mType==SignalType::BITSET){
            static_cast<BitsetSignal*>(s)->reset();

        }else if(s->mType==SignalType::STRING){
            static_cast<StringSignal*>(s)->reset();

        }else if(s->mType==SignalType::BASE_OBJECT){
            static_cast<ObjectSignal*>(s)->reset();

        }
    }

}


std::string Signal::getDbgInfo()
{

    if(mType==SignalType::BOOL){
        BoolSignal *s = static_cast<BoolSignal*>(this);
        return mName + "  value=" + std::to_string(s->value()) + " previousValue="+std::to_string(s->previousValue()) +
                " mValueChanged=" + std::to_string(s->valueChanged()) + " mValue_setOnNextFrame=" + std::to_string(s->value_setOnNextFrame());

    }else if(mType==SignalType::INT){
        IntSignal *s = static_cast<IntSignal*>(this);
        return mName + "  value=" + std::to_string(s->value()) + " previousValue="+std::to_string(s->previousValue()) +
                " mValueChanged=" + std::to_string(s->valueChanged()) + " mValue_setOnNextFrame=" + std::to_string(s->value_setOnNextFrame());

    }else if(mType==SignalType::FLOAT){
        FloatSignal *s = static_cast<FloatSignal*>(this);
        return mName + "  value=" + std::to_string(s->value()) + " previousValue="+std::to_string(s->previousValue()) +
                " mValueChanged=" + std::to_string(s->valueChanged()) + " mValue_setOnNextFrame=" + std::to_string(s->value_setOnNextFrame());

    }else if(mType==SignalType::BITSET){
        BitsetSignal *s = static_cast<BitsetSignal*>(this);


    }else if(mType==SignalType::STRING){
        StringSignal *s = static_cast<StringSignal*>(this);
        return mName + "  value=" + s->value() + " previousValue="+s->previousValue() +
                " mValueChanged=" + std::to_string(s->valueChanged()) + " mValue_setOnNextFrame=" + s->value_setOnNextFrame();

    }

    return "";

}


void Signal::resetValueFromString(const std::string &_value)
{

    if(mType==SignalType::BOOL){
        BoolSignal *s = static_cast<BoolSignal*>(this);
        bool value = StdString::stringToBool(_value);
        s->reset(value);

    }else if(mType==SignalType::INT){
        IntSignal *s = static_cast<IntSignal*>(this);
        int value = StdString::stringToInt(_value, 0);
        s->reset(value);

    }else if(mType==SignalType::FLOAT){
        FloatSignal *s = static_cast<FloatSignal*>(this);
        float value = StdString::stringToFloat(_value, 0);
        s->reset(value);

    }else if(mType==SignalType::BITSET){
        BitsetSignal *s = static_cast<BitsetSignal*>(this);
        int value = StdString::stringToInt(_value, 0);
        s->reset(value);

    }else if(mType==SignalType::STRING){
        StringSignal *s = static_cast<StringSignal*>(this);
        s->reset(_value);

    }

}


void Signal::writeValueAsIniEntry(TextStreamWriter &_writer, bool writeFloatAsInt)
{

    if(mType==SignalType::BOOL){
        BoolSignal *s = static_cast<BoolSignal*>(this);

        _writer.WriteLine(mName + ": " + std::to_string(s->value()));

    }else if(mType==SignalType::INT){
        IntSignal *s = static_cast<IntSignal*>(this);

        _writer.WriteLine(mName + ": " + std::to_string(s->value()));

    }else if(mType==SignalType::FLOAT){
        FloatSignal *s = static_cast<FloatSignal*>(this);

        if(writeFloatAsInt){
            _writer.WriteLine(mName + ": " + std::to_string(int(s->value())));
        }else{
            _writer.WriteLine(mName + ": " + std::to_string(s->value()));
        }


    }else if(mType==SignalType::BITSET){
        BitsetSignal *s = static_cast<BitsetSignal*>(this);

        _writer.WriteLine(mName + ": " + std::to_string(s->value()));

    }else if(mType==SignalType::STRING){
        StringSignal *s = static_cast<StringSignal*>(this);

        _writer.WriteLine(mName + ": " + (s->value()));
    }

}


//======================================================================================


BitsetSignal::BitsetSignal(const std::string &_name, unsigned int _value, unsigned char _id) :  Signal(_name, SignalType::BITSET, _id)
{
    mDefaultValue = mValue = mPreviousValue = _value;
}


BitsetSignal::~BitsetSignal()
{

    if(mExtraData){
        delete mExtraData;
    }

}


void BitsetSignal::setValue(unsigned int _value)
{

    if(_value == mValue) return;

    mValue = _value;

    mValueStarted = mValueEnded = 0;

    if(mValue != mPreviousValue){

        for(unsigned int i=0; i<mNumBits; i++){
            unsigned int flag = (unsigned int)1 << i;

            bool state = (mValue & flag) != 0;
            bool statePrevious = (mPreviousValue & flag) != 0;

            if(state != statePrevious){

                if(state==true){
                    mValueStarted |= flag;
                    mValueEnded &= ~flag;

                }else{
                    mValueStarted &= ~flag;
                    mValueEnded |= flag;
                }
            }
        }
    }


    if(mStored_forPostUpdate==false){
        gSignalUpdater.addSignal_forPostUpdate(this);
        mStored_forPostUpdate = true;
    }

    //---
    if(mCallback){
        mCallback->onSignalSet(this);
    }

}


void BitsetSignal::setValue_onNextFrame(unsigned int _value)
{

    if(_value == mValue && mStored_forValueSetOnNextFrame==false) return;

    mValue_setOnNextFrame = _value;

    if(mStored_forValueSetOnNextFrame==false){
        gSignalUpdater.addSignal_forValueSetOnNextFrame(this);
        mStored_forValueSetOnNextFrame = true;
    }

}


void BitsetSignal::setFlags(unsigned int _flags, bool _state)
{

    unsigned int value = mValue;

    if(_state==true){
        value |= _flags;
    }else{
        value &= ~_flags;
    }

    setValue(value);

}


void BitsetSignal::setFlags_onNextFrame(unsigned int _flags, bool _state)
{

    unsigned int value = mValue;

    if(mStored_forValueSetOnNextFrame){
        value = mValue_setOnNextFrame;
    }

    if(_state==true){
        value |= _flags;
    }else{
        value &= ~_flags;
    }

    setValue_onNextFrame(value);

}


void BitsetSignal::postUpdate()
{

    mPreviousValue = mValue;
    mValueStarted = mValueEnded = 0;

}


void BitsetSignal::reset()
{
    mValue = mPreviousValue = mDefaultValue;
    mValueStarted = mValueEnded = 0;

}


void BitsetSignal::reset(unsigned int _allFlags)
{

    mValue = mPreviousValue = _allFlags;
    mValueStarted = mValueEnded = 0;

}


void BitsetSignal::setDefaultValue(unsigned int _value)
{
    mDefaultValue = _value;
}


BitflagsSignalExtraData *BitsetSignal::createExtraData()
{

    if(mExtraData){
        delete mExtraData;
    }
    mExtraData = new BitflagsSignalExtraData();

    return mExtraData;

}



//===================================================================================


StringSignalExtraData *StringSignal::createExtraData()
{

    if(mExtraData){
        delete mExtraData;
    }

    mExtraData = new StringSignalExtraData();
    return mExtraData;

}




}
