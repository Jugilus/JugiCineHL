#ifndef JUGIMAP_SIGNAL_H
#define JUGIMAP_SIGNAL_H


#include <vector>


namespace jugimap {



/// \addtogroup Common
/// @{







enum class ValueStateType
{
    BOOL,
    INT,
    INT_BITS,
    UNKNOWN

};


class Signal
{
public:
    Signal(ValueStateType _type, unsigned char _id) : mType(_type), mID(_id){}

    ValueStateType type(){ return mType; }
    unsigned char id(){ return mID;}
    void setId(unsigned char _id){ mID = _id; }


    static void preUpdateSignals(std::vector<Signal*> &_signals);
    static void postUpdateSignals(std::vector<Signal*> &_signals);
    static void resetSignals(std::vector<Signal*> &_signals);


protected:
    ValueStateType mType = ValueStateType::UNKNOWN;
    unsigned char mID = 0;

};



class BoolSignal : public Signal
{
public:
    BoolSignal(unsigned char _id=0) : Signal(ValueStateType::BOOL, _id){}

    bool active(){ return mValue; }
    bool activeStarted(){ return mValueStarted; }
    bool activeEnded(){ return mValueEnded; }
    bool notActive(){ return !(mValue); }


    void _Set(bool _active);


    void _set()
    {
        if(mValue == false){
            mValueEnded = mValue;
            mValue = mValueStarted = true;
        }
    }

    void _unset()
    {
        if(mValue == true){
            mValueEnded = mValue;
            mValue = mValueStarted = false;
        }
    }


    void _setOnNextFrame()
    {
        mNextValueSet = true;
    }

    void _unsetOnNextFrame()
    {
        mNextValueUnset = true;
    }


    void preUpdate()
    {
        if(mNextValueSet){
            _set();

        }else if(mNextValueUnset){
            _unset();
        }

        mNextValueSet = mNextValueUnset = false;
    }
    void postUpdate(){ mValueStarted = mValueEnded = false; }
    void reset(){ mValueStarted = mValue = mValueEnded = false; }



    void _setActive(bool _active) { mValue = _active; }
    void _setActiveStarted(bool _activeStarted) { mValueStarted = _activeStarted; }
    void _setActiveEnded(bool _activeEnded){ mValueEnded=_activeEnded; }


private:
    bool mValue = false;
    bool mValueStarted = false;
    bool mValueEnded = false;

    //---
    bool mNextValueSet = false;
    bool mNextValueUnset = false;

};



class IntSignal : public Signal
{
public:
    IntSignal(unsigned char _id=0) : Signal(ValueStateType::INT, _id){}

    bool active(int _valueQuery){ return _valueQuery==mValue; }
    bool activeStarted(int _valueQuery){ return _valueQuery==mValueStarted; }
    bool activeEnded(int _valueQuery){ return _valueQuery==mValueEnded; }
    bool notActive(int _valueQuery){ return _valueQuery!=mValue; }


    void _set(int _value)
    {
        if(_value != mValue){
            mValueEnded = mValue;
            mValue = mValueStarted = _value;
        }
    }

    void _unset(int _value)
    {
        if(_value == mValue){
            mValueEnded = mValue;
            mValue = mValueStarted = 0;
        }
    }

    void _setFullValue(int _value){ mValue = _value; }


    void _setOnNextFrame(int _value)
    {
        mNextValue = _value;
        mNextValueSet = true;
    }

    void _unsetOnNextFrame(int _value)
    {
        mNextValue = _value;
        mNextValueUnset = true;
    }

    void preUpdate()
    {
        if(mNextValueSet){
            _set(mNextValue);

        }else if(mNextValueUnset){
            _unset(mNextValue);
        }

        mNextValueSet = mNextValueUnset = false;
    }

    void postUpdate(){ mValueStarted = mValueEnded = 0; }
    void reset(){ mValueStarted = mValue = mValueEnded = 0; }



private:

    int mValue = 0;
    int mValueStarted = 0;
    int mValueEnded = 0;

    //---
    unsigned int mNextValue = 0;
    bool mNextValueSet = false;
    bool mNextValueUnset = false;

};


class IntBitsSignal : public Signal
{
public:
    IntBitsSignal(unsigned char _id=0) : Signal(ValueStateType::INT_BITS, _id){}

    bool active(unsigned int _valueQuery){ return mValue & _valueQuery; }
    bool activeStarted(unsigned int _valueQuery){ return mValueStarted & _valueQuery; }
    bool activeEnded(unsigned int _valueQuery){ return mValueEnded & _valueQuery; }
    bool notActive(unsigned int _valueQuery){ return (mValue & _valueQuery)==0;}


    void _setFlag(unsigned int _value)
    {
        if((mValue & _value)==0){
            mValueEnded &= ~_value;
            mValue |= _value;
            mValueStarted |= _value;
        }
    }

    void _setFlagOnNextFrame(unsigned int _value)
    {
        mNextValue = _value;
        mNextValueSet = true;
    }



    void _unsetFlag(unsigned int _value)
    {
        if(mValue & _value){
            mValueEnded |= _value;
            mValue &= ~_value;
            mValueStarted &= ~_value;
        }
    }

    void _unsetFlagOnNextFrame(unsigned int _value)
    {
        mNextValue = _value;
        mNextValueUnset = true;
    }


    void _setFullValue(unsigned int _value)
    {
        if(mValue != _value){
            mValueEnded = mValue;
            mValue = _value;
            mValueStarted = mValue;
        }
    }


    void _setFullValueOnNextFrame(unsigned int _value)
    {
        mNextValue = _value;
        mNextValueSetFullValue = true;
    }


    void _setAllFlags(unsigned int _value)
    {
        for(unsigned int i=0; i<32; i++){
            unsigned int flag = (unsigned int)1 << i;
            if((flag & _value)){
                _setFlag(flag);
            }else{
                _unsetFlag(flag);
            }
        }
    }


    void _setAllFlagsOnNextFrame(unsigned int _value)
    {
        mNextValue = _value;
        mNextValueSetFullValue = true;
    }



    void preUpdate()
    {
        if(mNextValueSet){
            _setFlag(mNextValue);

        }else if(mNextValueUnset){
            _unsetFlag(mNextValue);

        }else if(mNextValueSetFullValue){
            //_setFullValue(mNextValue);
            _setAllFlagsOnNextFrame(mNextValue);
        }

        mNextValueSet = mNextValueUnset = mNextValueSetFullValue = false;
        mNextValue = 0;
    }


    void postUpdate(){ mValueStarted = mValueEnded = 0; }

    void reset(){ mValueStarted = mValue = mValueEnded = 0; }


private:

    unsigned int mValue = 0;
    unsigned int mValueStarted = 0;
    unsigned int mValueEnded = 0;

    //---
    unsigned int mNextValue = 0;
    bool mNextValueSet = false;
    bool mNextValueUnset = false;
    bool mNextValueSetFullValue = false;

};

/// @}      //end addtogroup Common

}




#endif


