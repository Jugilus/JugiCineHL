#ifndef PLAYER__DATA__DATA_BASE_H
#define PLAYER__DATA__DATA_BASE_H

#include <assert.h>
#include <string>
#include <vector>

//#include "jmSignal.h"
//#include "jmStorage.h"

//#include "jpLogicState.h"
//#include "jpLogicStateCfg.h"
//#include "jpGlobal.h"

#include "jmTween.h"
#include "jmCommonFunctions.h"
#include "jmSignal.h"

#include "jpGlobal.h"


namespace pugi {

class xml_node;

}



namespace jugimap{

class PlayedScene;
class Sprite;
class SourceSprite;
class TextSegment;
class AniSpriteObject;



bool parseIntData(const std::string &sValue, int & value);
bool parseFloatData(const std::string &sValue, float & value);


//=============================================================================================

enum class ItemDataType
{
    UNKNOWN,
    INT,
    FLOAT,
    BOOL,
    SPRITE,
    TEXT_STRING,
    TEXT_SEGMENT,
    ANI_SPRITE_OBJECT,
    TWEEN

};




class ItemData : public BaseObject
{
public:
    static ItemData* create(const std::string &_name, ItemDataType _type);
    static ItemData* copy(ItemData *_srcData);
    static void copyContent(ItemData *_srcData, ItemData *_dstData);


    ItemData(const std::string &_name, ItemDataType _type);
    virtual ~ItemData(){}

    virtual bool initCfg(const pugi::xml_node &_node) = 0;
    virtual bool initConnections(PlayedScene *_scene){ return true; }
    virtual bool setupInternalDataConnections(Item *_parentItem){ return true; }


    const std::string & name(){ return mName;}
    ItemDataType type() const { return mType;}
    std::string valueString();


    SignalCallback *callback(){ return mCallback; }
    void setCallback(SignalCallback * _callback ){ mCallback = _callback; }


protected:
    std::string mName;
    ItemDataType mType;
    SignalCallback *mCallback = nullptr;        // LINK

};



class IntItemData : public ItemData
{
public:

    IntItemData(const std::string &_name);

    bool initCfg(const pugi::xml_node &_node) override;


    int value(){ return mValue; }
    int minValue(){ return mMinValue; }
    int maxValue(){ return mMaxValue; }

    void setValue(int _value);
    bool isValue(int _value){ return mValue == _value; }

    void _setMinValue(int _value){ mMinValue = _value; Clamp(mValue, mMinValue, mMaxValue); }
    void _setMaxValue(int _value){ mMaxValue = _value; Clamp(mValue, mMinValue, mMaxValue); }

private:
    int mValue = 0;
    int mMinValue = 0;
    int mMaxValue = 0;

};


class FloatItemData : public ItemData
{
public:
    FloatItemData(const std::string &_name);

    bool initCfg(const pugi::xml_node &_node) override;


    float value(){ return mValue; }
    float minValue(){ return mMinValue; }
    float maxValue(){ return mMaxValue; }

    void setValue(float _value);
    bool isValue(float _value){ return mValue == _value; }

    void _setMinValue(float _value){ mMinValue = _value; Clamp(mValue, mMinValue, mMaxValue); }
    void _setMaxValue(float _value){ mMaxValue = _value; Clamp(mValue, mMinValue, mMaxValue); }

private:
    float mValue = 0.0f;
    float mMinValue = 0.0f;
    float mMaxValue = 0.0f;
};


class BoolItemData : public ItemData
{
public:
    BoolItemData(const std::string &_name);

    bool initCfg(const pugi::xml_node &_node) override;


    bool isValue(bool _value){ return mValue == _value; }
    bool value(){ return mValue; }
    void setValue(bool _value){ mValue = _value; }

private:
    bool mValue = 0.0;
};



class SpriteData : public ItemData
{
public:
    SpriteData(const std::string &_name);
    SpriteData(const SpriteData & _src);
    SpriteData & operator = (const SpriteData & _src);
    ~SpriteData();

    bool initCfg(const pugi::xml_node &_node) override;
    bool initConnections(PlayedScene *_scene) override;

    Sprite * sprite(){ return mSprite; }
    void setSpriteOwned(bool _spriteOwned){ mSpriteOwned = _spriteOwned; }
    bool isSpriteOwned(){ return mSpriteOwned; }

private:
    std::string mSourceSpriteName;

    //----
    Sprite * mSprite = nullptr;             // LINK or OWNED
    bool mSpriteOwned = false;              // if sprite is shown on a layer it is owned by the layer, otherwise by the SpriteData

};



class TextData : public ItemData
{
public:
    TextData(const std::string &_name);

    bool initCfg(const pugi::xml_node &_node) override;
    bool initConnections(PlayedScene *_scene) override;

    const std::string & text(){ return mText; }


private:
    std::string mTextCfg;

    //----
    std::string mText;

};



class TextSegmentData : public ItemData
{
public:

    TextSegmentData(const std::string &_name);

    bool initCfg(const pugi::xml_node &_node) override;
    bool initConnections(PlayedScene *_scene) override;

    TextSegment *textSegment(){ return mTextSegment; }


private:
    std::string mTextCfg;

    //----
    TextSegment *mTextSegment = nullptr;        // LINK

};



class AniSpriteObjectData : public ItemData
{
public:

    AniSpriteObjectData(const std::string &_name);
    AniSpriteObjectData(const AniSpriteObjectData & _src);
    AniSpriteObjectData& operator =(const AniSpriteObjectData & _src);
    ~AniSpriteObjectData();

    bool initCfg(const pugi::xml_node &_node) override;
    bool initConnections(PlayedScene *_scene) override;
    bool setupInternalDataConnections(Item *_item) override;

    void start(Vec2f _pos);
    void update();
    void end();
    bool isAnimationStalled();
    bool isAnimationStopped();

    AniSpriteObject *aniSpriteObject(){ return mAniSpriteObject; }

private:
    //std::string mAniSpriteObjectCfg;

    //----
    AniSpriteObject *mAniSpriteObject = nullptr;        // OWNED

    std::string mText;
    std::string mDataNameForText;
    ItemData *mDataForText = nullptr;                           // LINK

};


class TweenData : public ItemData
{
public:

    TweenData(const std::string &_name);

    bool initCfg(const pugi::xml_node &_node) override;
    bool initConnections(PlayedScene *_scene) override;

    Tween & tween(){ return mTween; }


private:
    Tween mTween;

};







}


#endif
