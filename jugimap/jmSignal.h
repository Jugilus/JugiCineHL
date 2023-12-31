#ifndef JUGIMAP_SIGNAL_H
#define JUGIMAP_SIGNAL_H

#include <memory>
#include <vector>
#include <bitset>
#include <type_traits>

#include "jmCommonFunctions.h"

namespace jugimap {



/// \addtogroup Common
/// @{


enum class SignalType
{
    BOOL,
    INT,
    FLOAT,
    BITSET,
    STRING,
    BASE_OBJECT,
    UNKNOWN

};


class SignalUpdater;
class TextStreamWriter;
class Signal;
class ItemData;
class Item;
class BaseObject;






struct SignalID
{
    //static const unsigned char UPDATED_BOOL_SIGNAL = 1;
    static const unsigned char UPDATED_ON_SIGNAL_QUERY = 2;
};



class SignalCallback
{
public:
    virtual ~SignalCallback(){}

    virtual bool onSetBySignalSetter(Signal *_signal){ return true; }
    virtual bool onSignalSet(Signal *_signal){ return true; }

    virtual bool onDataChanged(ItemData *_data){ return true; }
    virtual bool onChildItemChanged(Item *_parentItem, Item *addedChildItem, Item *removedChildItem, Item *newSelectedItem){ return true; }


};


//---------------------------------------------------------------------------

/*
struct NamedValue
{

    NamedValue(const std::string &_name, SignalType _type) : name(_name), type(_type){}

    std::string name;
    SignalType type;
};
*/


template< typename T>
struct ExtNamedValue
{
    ExtNamedValue(const std::string &_name, T _value) : name(_name), value(_value) {}

    std::string name;
    T value = 0;
};


typedef ExtNamedValue<bool> NamedBoolValue;
typedef ExtNamedValue<int> NamedIntValue;
typedef ExtNamedValue<float> NamedFloatValue;
typedef ExtNamedValue<unsigned int> NamedBitflagsValue;
typedef ExtNamedValue<std::string> NamedStringValue;


//---------------------------------------------------------------------------

struct SignalExtraData
{

    virtual ~SignalExtraData(){}

};


template< typename T>
struct TSignalExtraData : public SignalExtraData
{

    ~TSignalExtraData() override
    {
        if(namedValues && namedValuesOwned){
            delete namedValues;
        }
    }


    T minValue;
    T maxValue;
    std::vector<std::pair<std::string, T>> *namedValues;          // LINK or OWNED
    bool namedValuesOwned = false;

};


typedef TSignalExtraData<bool> BoolSignalExtraData;
typedef TSignalExtraData<int> IntSignalExtraData;
typedef TSignalExtraData<float> FloatSignalExtraData;
typedef TSignalExtraData<unsigned int> BitflagsSignalExtraData;
typedef TSignalExtraData<std::string> StringSignalExtraData;



//---------------------------------------------------------------------------


class Signal
{
public:
    friend class SignalUpdater;


    //Signal(SignalType _type, unsigned char _id) : mType(_type), mID(_id){}
    Signal(const std::string &_name, SignalType _type, unsigned char _id) : mName(_name), mType(_type), mID(_id){}

    virtual ~Signal();

    SignalType type(){ return mType; }
    unsigned char id(){ return mID;}
    void setId(unsigned char _id){ mID = _id; }


    static void resetSignals(std::vector<Signal*> &_signals);

    std::string getDbgInfo();

    void resetValueFromString(const std::string &_value);
    void writeValueAsIniEntry(TextStreamWriter &_writer, bool writeFloatAsInt=false);

    const std::string & name(){ return mName; }
    void setName(const std::string &_name){ mName = _name; }


    SignalCallback *callback(){ return mCallback; }
    void setCallback(SignalCallback * _callback ){ mCallback = _callback; }

    BaseObject *dataObject(){ return mDataObject; }
    void setDataObject(BaseObject* _dataObject){ mDataObject = _dataObject; }


    //SignalExtraData *extraData(){ return mExtraData; }

    bool stored_forValueSetOnNextFrame(){ return mStored_forValueSetOnNextFrame; }


protected:
    SignalType mType = SignalType::UNKNOWN;
    std::string mName;
    unsigned char mID = 0;

    bool mStored_forValueSetOnNextFrame = false;
    bool mStored_forPostUpdate = false;

    SignalCallback * mCallback = nullptr;           // LINK
    //SignalExtraData * mExtraData = nullptr;

    BaseObject * mDataObject = nullptr;

};


class SignalUpdater
{
public:

    void addSignal_forValueSetOnNextFrame(Signal *_signal);
    void addSignal_forPostUpdate(Signal *_signal);

    void preUpdateSignals();
    void postUpdateSignals();

    void removeSignal_forValueSetOnNextFrame(Signal *_signal);
    void removeSignal_forPostUpdate(Signal *_signal);

private:
    std::vector<Signal*>mPreUpdateSignals;
    std::vector<Signal*>mPostUpdateSignals;

};


extern SignalUpdater gSignalUpdater;




template <typename T, SignalType _type, typename TSignalExtraData>
class NumSignal : public Signal
{
public:
    NumSignal(unsigned char _id=0) : Signal("", _type, _id),
      mValue(), mPreviousValue(), mDefaultValue(), mValue_setOnNextFrame()
    {
    }

    NumSignal(const std::string &_name, T _value, unsigned char _id=0) : Signal(_name, _type, _id) ,
       mValue(_value), mPreviousValue(_value), mDefaultValue(_value), mValue_setOnNextFrame()
    {
    }

    ~NumSignal()
    {
        if(mExtraData){
            delete mExtraData;
        }
    }


    bool active(T _value){ return _value==mValue; }
    bool activeStarted(T _value){ return _value==mValue && mValueChanged==true; }
    bool activeEnded(T _value){ return _value==mPreviousValue && mValueChanged==true; }
    bool activeChanged(T _value){ return activeStarted(_value) || activeEnded(_value); }
    bool notActive(T _value){ return _value!=mValue; }


    T value(){ return mValue; }
    T previousValue(){ return mPreviousValue; }
    bool valueChanged(){ return mValueChanged; }
    T defaultValue(){ return mDefaultValue; }
    T value_setOnNextFrame(){ return mValue_setOnNextFrame;}
    T value_currentOrNextIfSet(){return (mStored_forValueSetOnNextFrame)? mValue_setOnNextFrame : mValue; }


    void setValue(T _value)
    {

        if(_value == mValue) return;

        mValue = _value;
        mValueChanged = (mValue != mPreviousValue);

        if(mStored_forPostUpdate==false){
            gSignalUpdater.addSignal_forPostUpdate(this);
            mStored_forPostUpdate = true;
        }

        //---
        if(mCallback){
            mCallback->onSignalSet(this);
        }

    }


    void setValue_onNextFrame(T _value)
    {

        if(_value == mValue && mStored_forValueSetOnNextFrame==false) return;

        mValue_setOnNextFrame = _value;

        if(mStored_forValueSetOnNextFrame==false){
            gSignalUpdater.addSignal_forValueSetOnNextFrame(this);
            mStored_forValueSetOnNextFrame = true;
        }

    }


    void reset(T _value)
    {
        mValue = mPreviousValue = _value;
        mValueChanged = false;
    }


    void reset()
    {
        reset(mDefaultValue);
    };


    void setDefaultValue(T _value)
    {
        mDefaultValue = _value;
    }


    void postUpdate()
    {
        mPreviousValue = mValue;
        mValueChanged = false;
    }


    void _setValue(T _value)
    {
        mValue = _value;
    }


    void _setPreviousValue(T _value)
    {
        mPreviousValue = _value;
    }


    void _setValueChanged(bool _valueChanged)
    {
        mValueChanged = _valueChanged;
    }



    //SignalExtraData *extraData(){ return mExtraData; }


    void setExtraData(TSignalExtraData* _extraData){ mExtraData = _extraData; }

    TSignalExtraData * createExtraData()
    {
        if(mExtraData){
            delete mExtraData;
        }
        mExtraData = new TSignalExtraData();
        return mExtraData;
    }

    TSignalExtraData *extraData(){ return mExtraData; }


    /*
    std::string getDbgInfo() override
    {
        //return "value=" + std::to_string(mValue) + " valueEnded="+std::to_string(mValueEnded) + " valueStarted="+std::to_string(mValueStarted) +
        //        " mValueEndedStartedIsValid=" + std::to_string(mValueChanged) + " mValue_setOnNextFrame=" + std::to_string(mValue_setOnNextFrame);


        if(std::is_same<T, std::string>::value){
            return dbgName + "  value=" + std::to_string(mValue) + " previousValue="+std::to_string(mPreviousValue) +
                    " mValueChanged=" + std::to_string(mValueChanged) + " mValue_setOnNextFrame=" + std::to_string(mValue_setOnNextFrame);
        }
    }
    */

    //SignalCallback *callback(){ return mCallback; }
    //void setCallback(SignalCallback * _callback ){ mCallback = _callback; }


private:

    //T mValue = 0;
    //T mPreviousValue = 0;
    //T mDefaultValue = 0;
    //T mValue_setOnNextFrame = 0;

    T mValue;
    T mPreviousValue;
    T mDefaultValue;
    T mValue_setOnNextFrame;

    //---
    bool mValueChanged = false;


    TSignalExtraData *mExtraData = nullptr;

    //SignalCallback *mCallback = nullptr;

};



typedef NumSignal<bool, SignalType::BOOL, BoolSignalExtraData> BoolSignal;
typedef NumSignal<int, SignalType::INT, IntSignalExtraData> IntSignal;
typedef NumSignal<float, SignalType::FLOAT, FloatSignalExtraData> FloatSignal;


//typedef NumSignal<std::string, SignalType::STRING> StringSignal;


/*

class BoolSignal : public Signal
{
public:
    BoolSignal(unsigned char _id=0) : Signal(SignalType::BOOL, _id){}
    BoolSignal(bool _value, unsigned char _id=0) : Signal(SignalType::BOOL, _id){ mDefaultValue = mValue = mPreviousValue = _value; }


    bool active(bool _valueQuery=true){ return _valueQuery==mValue; }
    //bool activeStarted(bool _valueQuery=true){ return _valueQuery==mValueStarted && mValueChanged==true; }
    bool activeStarted(bool _valueQuery=true){ return _valueQuery==mValue && mValueChanged==true; }
    //bool activeEnded(bool _valueQuery=true){ return _valueQuery==mValueEnded && mValueChanged==true; }
    bool activeEnded(bool _valueQuery=true){ return _valueQuery==mPreviousValue && mValueChanged==true; }
    bool activeChanged(bool _valueQuery=true){ return activeStarted(_valueQuery) || activeEnded(_valueQuery); }
    bool notActive(bool _valueQuery=true){ return _valueQuery!=mValue; }


    bool value(){ return mValue; }
    bool previousValue(){ return mPreviousValue; }
    bool valueChanged(){ return mValueChanged; }
    bool defaultValue(){ return mDefaultValue; }
    bool value_setOnNextFrame(){ return mValue_setOnNextFrame;}


    void setValue(bool _value)
    {

        if(_value == mValue) return;

        mValue = _value;

        if(mValue != mPreviousValue){
            //mValueStarted = mValue;
            //mValueEnded = mPreviousValue;
            mValueChanged = true;

        }else{
            //mValueStarted = mValueEnded = 0;
            mValueChanged = false;
        }

        if(mStored_forPostUpdate==false){
            gSignalUpdater.addSignal_forPostUpdate(this);
            mStored_forPostUpdate = true;
        }

    }

    void setValue_onNextFrame(bool _value)
    {

        if(_value == mValue && mStored_forValueSetOnNextFrame==false) return;

        mValue_setOnNextFrame = _value;

        if(mStored_forValueSetOnNextFrame==false){
            gSignalUpdater.addSignal_forValueSetOnNextFrame(this);
            mStored_forValueSetOnNextFrame = true;
        }

    }


    void reset(bool _value)
    {
        mValue = mPreviousValue = _value;
        //mValueStarted = mValueEnded = 0;
        mValueChanged = false;
    }


    void reset()
    {
        reset(mDefaultValue);
    };


    void setDefaultValue(bool _value)
    {
        mDefaultValue = _value;
    }


    void postUpdate()
    {
        mPreviousValue = mValue;
        //mValueStarted = mValueEnded = 0;
        mValueChanged = false;

    }


    void _setValue(bool _value)
    {

        if(dbgName.empty()==false){
            DummyFunction();
        }

        mValue = _value;
    }


    void _setPreviousValue(bool _value)
    {
        mPreviousValue = _value;
    }


    void _setValueChanged(bool _valueChanged)
    {
        mValueChanged = _valueChanged;
    }


    std::string getDbgInfo() override
    {
        //return "value=" + std::to_string(mValue) + " valueEnded="+std::to_string(mValueEnded) + " valueStarted="+std::to_string(mValueStarted) +
        //        " mValueEndedStartedIsValid=" + std::to_string(mValueChanged) + " mValue_setOnNextFrame=" + std::to_string(mValue_setOnNextFrame);

        return dbgName + "  value=" + std::to_string(mValue) + " previousValue="+std::to_string(mPreviousValue) +
                " mValueChanged=" + std::to_string(mValueChanged) + " mValue_setOnNextFrame=" + std::to_string(mValue_setOnNextFrame);

    }


private:

    bool mValue = 0;

    //bool mValueStarted = 0;
    //bool mValueEnded = 0;
    bool mValueChanged = false;

    //---
    bool mValue_setOnNextFrame = 0;

    //---
    bool mPreviousValue = 0;

    bool mDefaultValue = 0;

};


class IntSignal : public Signal
{
public:
    IntSignal(unsigned char _id=0) : Signal(SignalType::INT, _id){}
    IntSignal(int _value, unsigned char _id=0) : Signal(SignalType::INT, _id){ mDefaultValue = mValue = mPreviousValue = _value; }


    bool active(int _valueQuery){ return _valueQuery==mValue; }
    bool activeStarted(int _valueQuery){ return _valueQuery==mValueStarted && mValueEndedStartedIsValid==true; }
    bool activeEnded(int _valueQuery){ return _valueQuery==mValueEnded && mValueEndedStartedIsValid==true; }
    bool activeChanged(int _valueQuery){ return activeStarted(_valueQuery) || activeEnded(_valueQuery); }
    bool notActive(int _valueQuery){ return _valueQuery!=mValue; }


    int value(){ return mValue; }
    int origValue(){ return mPreviousValue; }
    int defaultValue(){ return mDefaultValue; }
    int value_setOnNextFrame(){ return mValue_setOnNextFrame;}


    void setValue(int _value)
    {

        if(_value == mValue) return;

        mValue = _value;

        if(mValue != mPreviousValue){
            mValueStarted = mValue;
            mValueEnded = mPreviousValue;
            mValueEndedStartedIsValid = true;

        }else{
            mValueStarted = mValueEnded = 0;
            mValueEndedStartedIsValid = false;
        }

        if(mStored_forPostUpdate==false){
            gSignalUpdater.addSignal_forPostUpdate(this);
            mStored_forPostUpdate = true;
        }

    }


    void setValue_onNextFrame(int _value)
    {

        if(_value == mValue && mStored_forValueSetOnNextFrame==false) return;

        mValue_setOnNextFrame = _value;

        if(mStored_forValueSetOnNextFrame==false){
            gSignalUpdater.addSignal_forValueSetOnNextFrame(this);
            mStored_forValueSetOnNextFrame = true;
        }
    }


    void reset(int _value)
    {
        mValue = mPreviousValue = _value;
        mValueStarted = mValueEnded = 0;
        mValueEndedStartedIsValid = false;
    }


    void reset()
    {
        reset(mDefaultValue);
    };


    void setDefaultValue(int _value)
    {
        mDefaultValue = _value;
    }


    void postUpdate()
    {
        mPreviousValue = mValue;
        mValueStarted = mValueEnded = 0;
        mValueEndedStartedIsValid = false;
    }


    void _setActive(int _active)
    {
        mValue = _active;
    }


    void _setActiveStarted(int _activeStarted)
    {
        mValueStarted = _activeStarted;
    }


    void _setActiveEnded(int _activeEnded)
    {
        mValueEnded = _activeEnded;
    }


    void _setValuesManually(int _active, int _activeStarted, int _activeEnded)
    {

        mValue = mPreviousValue = _active;
        mValueStarted = _activeStarted;
        mValueEnded = _activeEnded;

        if(mValueStarted || mValueEnded){
            mValueEndedStartedIsValid = true;
        }else{
            mValueEndedStartedIsValid = false;
        }

        //if(mStored_forPostUpdate==false){
        //    gSignalUpdater.addSignal_forPostUpdate(this);
        //    mStored_forPostUpdate = true;
        //}
    }


    std::string getDbgInfo() override
    {
        return "value=" + std::to_string(mValue) + " valueEnded="+std::to_string(mValueEnded) + " valueStarted="+std::to_string(mValueStarted) +
                " mValueEndedStartedIsValid=" + std::to_string(mValueEndedStartedIsValid) + " mValue_setOnNextFrame=" + std::to_string(mValue_setOnNextFrame);
    }


private:

    int mValue = 0;

    int mValueStarted = 0;
    int mValueEnded = 0;
    bool mValueEndedStartedIsValid = false;

    //---
    int mValue_setOnNextFrame = 0;

    //---
    int mPreviousValue = 0;

    int mDefaultValue = 0;

};

*/



class BitsetSignal : public Signal
{
public:
    BitsetSignal(unsigned char _id=0) : Signal("", SignalType::BITSET, _id){}
    BitsetSignal(const std::string &_name, unsigned int _value, unsigned char _id=0);

    ~BitsetSignal();

    bool active(unsigned int _value){ return (mValue & _value)!=0; }
    bool activeStarted(unsigned int _value){ return (mValueStarted & _value)!=0; }
    bool activeEnded(unsigned int _value){ return (mValueEnded & _value)!=0; }
    bool activeChanged(unsigned int _value){ return (mValueStarted & _value)!=0  || (mValueEnded & _value)!=0; }
    bool notActive(unsigned int _value){ return (mValue & _value)==0;}

    bool valueChanged(){ return mValue != mPreviousValue;}

    unsigned int value(){ return mValue; }
    unsigned int origValue(){ return mPreviousValue; }
    unsigned int defaultValue(){ return mDefaultValue; }
    unsigned int value_setOnNextFrame(){ return mValue_setOnNextFrame;}
    unsigned int value_currentOrNextIfSet(){return (mStored_forValueSetOnNextFrame)? mValue_setOnNextFrame : mValue; }

    void setValue(unsigned int _value);
    void setValue_onNextFrame(unsigned int _value);

    void setFlags(unsigned int _value, bool _state);
    void setFlags_onNextFrame(unsigned int _flags, bool _state);

    void postUpdate();
    void reset();
    void reset(unsigned int _allFlags);

    void setDefaultValue(unsigned int _value);


    //BitflagsSignalExtraData *extraData(){ return mExtraData; }
    void setExtraData(BitflagsSignalExtraData* _extraData){ mExtraData = _extraData; }

    BitflagsSignalExtraData * createExtraData();

    BitflagsSignalExtraData *extraData(){ return mExtraData; }

    //void setCallback(BitsetSignalCallback * _callback ){ mCallback = _callback; }


private:
    unsigned int mValue = 0;
    unsigned int mValueStarted = 0;
    unsigned int mValueEnded = 0;

    unsigned int mValue_setOnNextFrame = 0;

    unsigned int mPreviousValue = 0;
    unsigned int mDefaultValue = 0;

    unsigned char mNumBits = 32;

    //BitsetSignalCallback *mCallback = nullptr;

    BitflagsSignalExtraData * mExtraData = nullptr;

};


class StringSignal : public Signal
{
public:
    StringSignal(unsigned char _id=0) : Signal("", SignalType::STRING, _id),
      mValue(), mPreviousValue(), mDefaultValue(), mValue_setOnNextFrame()
    {
    }

    StringSignal(const std::string &_name, const std::string & _value, unsigned char _id=0) : Signal(_name, SignalType::STRING, _id) ,
       mValue(_value), mPreviousValue(_value), mDefaultValue(_value), mValue_setOnNextFrame()
    {
    }


    bool active(const std::string &_value){ return _value==mValue; }
    bool activeStarted(const std::string &_value){ return _value==mValue && mValueChanged==true; }
    bool activeEnded(const std::string &_value){ return _value==mPreviousValue && mValueChanged==true; }
    bool activeChanged(const std::string &_value){ return activeStarted(_value) || activeEnded(_value); }
    bool notActive(const std::string &_value){ return _value!=mValue; }


    const std::string & value(){ return mValue; }
    const std::string & previousValue(){ return mPreviousValue; }
    bool valueChanged(){ return mValueChanged; }
    const std::string & defaultValue(){ return mDefaultValue; }
    const std::string & value_setOnNextFrame(){ return mValue_setOnNextFrame;}
    const std::string & value_currentOrNextIfSet(){return (mStored_forValueSetOnNextFrame)? mValue_setOnNextFrame : mValue; }

    void setValue(const std::string &_value)
    {

        if(_value == mValue) return;

        mValue = _value;
        mValueChanged = (mValue != mPreviousValue);

        if(mStored_forPostUpdate==false){
            gSignalUpdater.addSignal_forPostUpdate(this);
            mStored_forPostUpdate = true;
        }

        if(mCallback){
            mCallback->onSignalSet(this);
        }

    }


    void setValue_onNextFrame(const std::string &_value)
    {

        if(_value == mValue && mStored_forValueSetOnNextFrame==false) return;

        mValue_setOnNextFrame = _value;

        if(mStored_forValueSetOnNextFrame==false){
            gSignalUpdater.addSignal_forValueSetOnNextFrame(this);
            mStored_forValueSetOnNextFrame = true;
        }

    }


    void reset(const std::string &_value)
    {
        mValue = mPreviousValue = _value;
        mValueChanged = false;
    }


    void reset()
    {
        reset(mDefaultValue);
    };


    void setDefaultValue(const std::string &_value)
    {
        mDefaultValue = _value;
    }


    void postUpdate()
    {
        mPreviousValue = mValue;
        mValueChanged = false;
    }


    void _setValue(const std::string &_value)
    {
        mValue = _value;
    }


    void _setPreviousValue(const std::string &_value)
    {
        mPreviousValue = _value;
    }


    void _setValueChanged(bool _valueChanged)
    {
        mValueChanged = _valueChanged;
    }


    //StringSignalExtraData *extraData(){ return static_cast<StringSignalExtraData*>(mExtraData); }
    void setExtraData(StringSignalExtraData* _extraData){ mExtraData = _extraData; }

    StringSignalExtraData * createExtraData();
    StringSignalExtraData * extraData(){ return mExtraData; }


    /*
    std::string getDbgInfo() override
    {
        //return "value=" + std::to_string(mValue) + " valueEnded="+std::to_string(mValueEnded) + " valueStarted="+std::to_string(mValueStarted) +
        //        " mValueEndedStartedIsValid=" + std::to_string(mValueChanged) + " mValue_setOnNextFrame=" + std::to_string(mValue_setOnNextFrame);


        if(std::is_same<T, std::string>::value){
            return dbgName + "  value=" + std::to_string(mValue) + " previousValue="+std::to_string(mPreviousValue) +
                    " mValueChanged=" + std::to_string(mValueChanged) + " mValue_setOnNextFrame=" + std::to_string(mValue_setOnNextFrame);
        }
    }
    */


    //StringSignalCallback *callback(){ return mCallback; }
    //void setCallback(StringSignalCallback * _callback ){ mCallback = _callback; }


private:

    std::string mValue;
    std::string mPreviousValue;
    std::string mDefaultValue;
    std::string mValue_setOnNextFrame;

    //---
    bool mValueChanged = false;

    //---
    StringSignalExtraData *mExtraData = nullptr;

};



class ObjectSignal : public Signal
{
public:
    ObjectSignal(unsigned char _id=0) : Signal("", SignalType::BASE_OBJECT, _id),
      mValue(), mPreviousValue(), mDefaultValue(), mValue_setOnNextFrame()
    {
    }

    ObjectSignal(const std::string &_name, BaseObject *_value, unsigned char _id=0) : Signal(_name, SignalType::STRING, _id) ,
       mValue(_value), mPreviousValue(_value), mDefaultValue(_value), mValue_setOnNextFrame()
    {
    }


    bool active(BaseObject* _value){ return _value==mValue; }
    bool activeStarted(BaseObject* _value){ return _value==mValue && mValueChanged==true; }
    bool activeEnded(BaseObject* _value){ return _value==mPreviousValue && mValueChanged==true; }
    bool activeChanged(BaseObject* _value){ return activeStarted(_value) || activeEnded(_value); }
    bool notActive(BaseObject* _value){ return _value!=mValue; }


    BaseObject*  value(){ return mValue; }
    BaseObject*  previousValue(){ return mPreviousValue; }
    bool valueChanged(){ return mValueChanged; }
    BaseObject*  defaultValue(){ return mDefaultValue; }
    BaseObject*  value_setOnNextFrame(){ return mValue_setOnNextFrame;}
    BaseObject*  value_currentOrNextIfSet(){return (mStored_forValueSetOnNextFrame)? mValue_setOnNextFrame : mValue; }


    void setValue(BaseObject* _value)
    {

        if(_value == mValue) return;

        mValue = _value;
        mValueChanged = (mValue != mPreviousValue);

        if(mStored_forPostUpdate==false){
            gSignalUpdater.addSignal_forPostUpdate(this);
            mStored_forPostUpdate = true;
        }

        if(mCallback){
            mCallback->onSignalSet(this);
        }

    }


    void setValue_onNextFrame(BaseObject* _value)
    {

        if(_value == mValue && mStored_forValueSetOnNextFrame==false) return;

        mValue_setOnNextFrame = _value;

        if(mStored_forValueSetOnNextFrame==false){
            gSignalUpdater.addSignal_forValueSetOnNextFrame(this);
            mStored_forValueSetOnNextFrame = true;
        }

    }


    void reset(BaseObject* _value)
    {
        mValue = mPreviousValue = _value;
        mValue_setOnNextFrame = _value;
        mValueChanged = false;
    }


    void reset()
    {
        reset(mDefaultValue);
    };


    void setDefaultValue(BaseObject* _value)
    {
        mDefaultValue = _value;
    }


    void postUpdate()
    {
        mPreviousValue = mValue;
        mValueChanged = false;
    }


    void _setValue(BaseObject* _value)
    {
        mValue = _value;
    }


    void _setPreviousValue(BaseObject* _value)
    {
        mPreviousValue = _value;
    }


    void _setValueChanged(bool _valueChanged)
    {
        mValueChanged = _valueChanged;
    }



private:

    BaseObject* mValue = nullptr;
    BaseObject* mPreviousValue = nullptr;
    BaseObject* mDefaultValue = nullptr;
    BaseObject* mValue_setOnNextFrame = nullptr;

    //---
    bool mValueChanged = false;


};







/*
class SignalParsingStorage
{
public:

    void setIdentifiers(std::vector<SignalIdentifier>* _identifiers);

    void add(Signal* _signal, const std::string &_identifier, std::vector<std::pair<std::string, unsigned int>> *_extra);

    void getParsingInfo(SignalParsingInfo &_signalInfo, const std::string &_signaId, const std::string &_extraId, bool _setErrorMessage=true);
    void getParsingInfo(SignalParsingInfo &_signalInfo, const std::string &_path, bool _setErrorMessage=true);

    Signal *getSignal(int _index);

    std::vector<Signal*>& signals(){ return mSignals; }
    std::vector<SignalIdentifier>* identifiers(){ return mIdentifiers; }


private:
    std::vector<Signal*>mSignals;                       // LINKS
    std::vector<SignalIdentifier>*mIdentifiers = nullptr;     // LINK

};

*/





/// @}      //end addtogroup Common

}




#endif


