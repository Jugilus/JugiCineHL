#include <limits>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmUtilities.h"
#include "jmSprite.h"
#include "jmTextSprite.h"
//#include "jmCompositeSprite.h"
//#include "jmStreams.h"

//#include "logic/jpLogicState.h"
//#include "jpPlayedScene.h"
//#include "jpPlayedApp.h"

//#include "jpCharacterSystem.h"
//#include "jpItemUtilities.h"
#include "jmText.h"
#include "jmMap.h"
#include "jmSceneLayout.h"

#include "gfxObjects/jpGfxObjectsCommon.h"
#include "jpUtilities.h"
#include "jpPlayedScene.h"
#include "jpItem.h"
#include "jpData.h"


namespace jugimap{





bool parseIntData(const std::string &sValue, int & value)
{

    if(sValue=="MIN_NEGATIVE"){
        value = std::numeric_limits<int>::min();

    }else if(sValue=="MAX_POSITIVE"){
        value = std::numeric_limits<int>::max();

    }else{

        if(StdString::integerNumber(sValue, value)==false){
            return false;
        }
    }

    return true;
}



bool parseFloatData(const std::string &sValue, float & value)
{

    if(sValue=="MIN_NEGATIVE"){
        value = std::numeric_limits<float>::min();

    }else if(sValue=="MAX_POSITIVE"){
        value = std::numeric_limits<float>::max();

    }else{

        if(StdString::floatNumber(sValue, value)==false){
            return false;
        }
    }

    return true;
}



/*

bool SourceItemDataException::initCfg(const pugi::xml_node &_node)
{

    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());


        if(attributeName=="case"){
            mCaseCfg = a.as_string();


        }else if(attributeName=="factor"){
            std::string sValue = a.as_string();

            if(parseFloatData(sValue, mFactor)==false){
                return false;
            }


        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }
}


bool SourceItemDataException::build(CharacterSystem *_characterSystem)
{

    std::vector<std::string>parts = StdString::splitString(mCaseCfg, "/");

    if(parts.size()!=3){
        dbgSystem.addMessage("Error parsing '"+ mCaseCfg + "'!");
        return false;
    }

    const std::string & action = parts[0];
    const std::string & actionMember = parts[1];
    const std::string & actionMemberChildItem = parts[2];




    return true;


}

*/

//====================================================================================


/*
bool ItemSourceData::initCfg_exceptions(const pugi::xml_node &_node)
{

    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="exception"){

            mExceptions.emplace_back();
            SourceItemDataException &e = mExceptions.back();

            if(e.initCfg(n)==false){
                return false;
            }


        }else{

            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;

        }
    }
}

*/


//===================================================================================


ItemData::ItemData(const std::string &_name, ItemDataType _type) : mName(_name), mType(_type)
{
    mBaseType = BaseObjectType::DATA;

}



std::string ItemData::valueString()
{

    if(mType==ItemDataType::INT){
        IntItemData *d = static_cast<IntItemData*>(this);
        return std::to_string(d->value());

    }else if(mType==ItemDataType::FLOAT){
        FloatItemData *d = static_cast<FloatItemData*>(this);
        return std::to_string(d->value());

    }else if(mType==ItemDataType::BOOL){
        BoolItemData *d = static_cast<BoolItemData*>(this);
        return std::to_string(d->value());

    }else if(mType==ItemDataType::TEXT_STRING){
        TextData *d = static_cast<TextData*>(this);
        return d->text();

    }else if(mType==ItemDataType::SPRITE){
        SpriteData *d = static_cast<SpriteData*>(this);
        if(d->sprite()){
            return d->sprite()->name();
        }else{
            return "No sprite in item!";
        }

    }else if(mType==ItemDataType::TEXT_SEGMENT){
        TextSegmentData *d = static_cast<TextSegmentData*>(this);
        return d->textSegment()->plainText();

    }else if(mType==ItemDataType::ANI_SPRITE_OBJECT){
        AniSpriteObjectData *d = static_cast<AniSpriteObjectData*>(this);
        return d->aniSpriteObject()->name();

    }else if(mType==ItemDataType::TWEEN){
        TweenData *d = static_cast<TweenData*>(this);
        return d->name();

    }

    //assert(false);
    return StdString::dummyString;
}



ItemData* ItemData::create(const std::string &_name, ItemDataType _type)
{

    if(_type==ItemDataType::INT){
        return new IntItemData(_name);

    }else if(_type==ItemDataType::FLOAT){
        return new FloatItemData(_name);

    }else if(_type==ItemDataType::BOOL){
        return new BoolItemData(_name);

    }else if(_type==ItemDataType::SPRITE){
        return new SpriteData(_name);

    }else if(_type==ItemDataType::TEXT_STRING){
        return new TextData(_name);

    }else if(_type==ItemDataType::TEXT_SEGMENT){
        return new TextSegmentData(_name);

    }else if(_type==ItemDataType::ANI_SPRITE_OBJECT){
        return new AniSpriteObjectData(_name);

    }else if(_type==ItemDataType::TWEEN){
        return new TweenData(_name);

    }

    assert(false);
    return nullptr;
}



ItemData* ItemData::copy(ItemData *_srcData)
{

    if(_srcData->type()==ItemDataType::INT){
        return new IntItemData(*(static_cast<IntItemData*>(_srcData)));

    }else if(_srcData->type()==ItemDataType::FLOAT){
        return new FloatItemData(*(static_cast<FloatItemData*>(_srcData)));

    }else if(_srcData->type()==ItemDataType::BOOL){
        return new BoolItemData(*(static_cast<BoolItemData*>(_srcData)));

    }else if(_srcData->type()==ItemDataType::SPRITE){
        return new SpriteData(*(static_cast<SpriteData*>(_srcData)));

    }else if(_srcData->type()==ItemDataType::TEXT_STRING){
        return new TextData(*(static_cast<TextData*>(_srcData)));

    }else if(_srcData->type()==ItemDataType::TEXT_SEGMENT){
        return new TextSegmentData(*(static_cast<TextSegmentData*>(_srcData)));

    }else if(_srcData->type()==ItemDataType::ANI_SPRITE_OBJECT){
        return new AniSpriteObjectData(*(static_cast<AniSpriteObjectData*>(_srcData)));

    }else if(_srcData->type()==ItemDataType::TWEEN){
        return new TweenData(*(static_cast<TweenData*>(_srcData)));

    }

    assert(false);
    return nullptr;
}


void ItemData::copyContent(ItemData *_srcData, ItemData *_dstData)
{

    if(_srcData->type()==ItemDataType::INT){
        *static_cast<IntItemData*>(_dstData) = *static_cast<IntItemData*>(_srcData);

    }else if(_srcData->type()==ItemDataType::FLOAT){
        *static_cast<FloatItemData*>(_dstData) = *static_cast<FloatItemData*>(_srcData);

    }else if(_srcData->type()==ItemDataType::BOOL){
        *static_cast<BoolItemData*>(_dstData) = *static_cast<BoolItemData*>(_srcData);

    }else if(_srcData->type()==ItemDataType::SPRITE){
        *static_cast<SpriteData*>(_dstData) = *static_cast<SpriteData*>(_srcData);

    }else if(_srcData->type()==ItemDataType::TEXT_STRING){
        *static_cast<TextData*>(_dstData) = *static_cast<TextData*>(_srcData);

    }else if(_srcData->type()==ItemDataType::TEXT_SEGMENT){
        *static_cast<TextSegmentData*>(_dstData) = *static_cast<TextSegmentData*>(_srcData);

    }else if(_srcData->type()==ItemDataType::ANI_SPRITE_OBJECT){
        *static_cast<AniSpriteObjectData*>(_dstData) = *static_cast<AniSpriteObjectData*>(_srcData);

    }else if(_srcData->type()==ItemDataType::TWEEN){
        *static_cast<TweenData*>(_dstData) = *static_cast<TweenData*>(_srcData);

    }

    assert(false);

}


IntItemData::IntItemData(const std::string &_name) : ItemData(_name, ItemDataType::INT),
        mValue(0),
        mMinValue(0),
        mMaxValue(std::numeric_limits<int>::max())
{

}



bool IntItemData::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item data '" + mName +"' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());


        if(attributeName=="name"){
            mName = a.as_string();


        }else if(attributeName=="minValue"){
            std::string sValue = a.as_string();

            if(parseIntData(sValue, mMinValue)==false){
                return false;
            }

        }else if(attributeName=="maxValue"){
            std::string sValue = a.as_string();

            if(parseIntData(sValue, mMaxValue)==false){
                return false;
            }

        }else if(attributeName=="value"){
            std::string sValue = a.as_string();

            if(parseIntData(sValue, mValue)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }

    //initCfg_exceptions(_node);

    dbgSystem.removeLastMessage();
    return true;

}


void IntItemData::setValue(int _value)
{

    Clamp(_value, mMinValue, mMaxValue);

    if(mValue==_value){
        return;
    }

    mValue = _value;

    if(mCallback){
        mCallback->onDataChanged(this);
    }

}


//-------------------------------------------------------------------------------------



FloatItemData::FloatItemData(const std::string &_name): ItemData(_name, ItemDataType::FLOAT),
        mValue(0.0f),
        mMinValue(0.0f),
        mMaxValue(std::numeric_limits<float>::max())
{

}


bool FloatItemData::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item data '" + mName +"' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());


        if(attributeName=="name"){
            mName = a.as_string();


        }else if(attributeName=="minValue"){
            std::string sValue = a.as_string();

            if(parseFloatData(sValue, mMinValue)==false){
                return false;
            }

        }else if(attributeName=="maxValue"){
            std::string sValue = a.as_string();

            if(parseFloatData(sValue, mMaxValue)==false){
                return false;
            }


        }else if(attributeName=="value"){
            std::string sValue = a.as_string();

            if(parseFloatData(sValue, mValue)==false){
                return false;
            }


        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}



void FloatItemData::setValue(float _value)
{

    Clamp(_value, mMinValue, mMaxValue);

    if(mValue==_value){
        return;
    }

    mValue = _value;

    if(mCallback){
        mCallback->onDataChanged(this);
    }

}


//-------------------------------------------------------------------------------------



BoolItemData::BoolItemData(const std::string &_name) : ItemData(_name, ItemDataType::BOOL),
        mValue(false)
{

}



bool BoolItemData::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item data '" + mName +"' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            mName = a.as_string();


        }else if(attributeName=="value"){
            std::string sValue = a.as_string();

            if(StdString::boolValue(sValue, mValue)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


//=====================================================================================


SpriteData::SpriteData(const std::string &_name) : ItemData(_name, ItemDataType::SPRITE)
{

}


SpriteData::SpriteData(const SpriteData & _src) : ItemData(_src)
{

    mSourceSpriteName = _src.mSourceSpriteName;
    if(_src.mSprite){
        mSprite = _src.mSprite->makePassiveCopy();
    }

    //mSpriteOwned = _src.mSpriteOwned;
    mSpriteOwned = true;
}


SpriteData & SpriteData::operator = (const SpriteData & _src)
{
    if(&_src==this){
        return *this;
    }

    ItemData::operator=(_src);

    //---
    mSourceSpriteName = _src.mSourceSpriteName;
    if(_src.mSprite){
        mSprite = _src.mSprite->makePassiveCopy();
    }

    mSpriteOwned = _src.mSpriteOwned;

    return *this;

}


SpriteData::~SpriteData()
{

    if(mSprite && mSpriteOwned){
        delete mSprite;
    }

}


bool SpriteData::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item data '" + mName +"' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            mName = a.as_string();

        }else if(attributeName=="sourceSprite"){
            mSourceSpriteName = a.as_string();

        /*
        }else if(attributeName=="sprite"){
            mExistingSpriteName = a.as_string();

        }else if(attributeName=="map"){
            mMap = a.as_string();

        }else if(attributeName=="visible"){
            std::string sValue = a.as_string();
            if(StdString::boolValue(sValue, mVisible)==false){
                return false;
            }

        }else if(attributeName=="frame"){
            std::string sValue = a.as_string();
            if(StdString::integerNumber(sValue, mFrame)==false){
                return false;
            }
        */
        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }

    if(mSourceSpriteName.empty()){
        dbgSystem.addMessage("No source sprite specified!");
        return false;
    }

    /*
    if(mSourceSpriteName.empty() == mExistingSpriteName.empty()){
        dbgSystem.addMessage("Ambigous sprite specification! Only one object, source sprite or sprite must be specified!");
        return false;
    }
    if(mExistingSpriteName.empty()==false && mMap.empty()){
        dbgSystem.addMessage("Missing map attribute!");
        return false;
    }
    */

    dbgSystem.removeLastMessage();
    return true;

}


bool SpriteData::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing item data '" + mName +"' ...");


    if(mSourceSpriteName.empty()==false){
        SourceSprite *sourceSprite = FindSourceSpriteWithName(mSourceSpriteName);
        if(sourceSprite==nullptr){
            return false;
        }

        mSprite = Sprite::makePassiveSprite(sourceSprite);
        mSpriteOwned = true;


    }
    /*
    else if(mExistingSpriteName.empty()==false && mMap.empty()==false){

        SceneMap *sm = _scene->getSceneMap(mMap);
        if(sm==nullptr){
            return false;
        }
        std::vector<Sprite*>sprites;
        CollectSpritesWithName(sm->map(), sprites, mExistingSpriteName);

        if(sprites.empty()){
            dbgSystem.addMessage("Sprite '" + mExistingSpriteName + "' not found!");
            return false;

        }else if(sprites.size()>1){
            dbgSystem.addMessage("Ambigous result! More than one sprite found with name '" + mExistingSpriteName + "'!");
            return false;
        }

        mSprite = sprites.front();
        mSpriteOwned = false;

    }
    */

    dbgSystem.removeLastMessage();
    return true;

}



//=====================================================================================



TextData::TextData(const std::string &_name) : ItemData(_name, ItemDataType::TEXT_STRING)
{

}


bool TextData::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item data '" + mName + "' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            mName = a.as_string();

        }else if(attributeName=="text"){
            mTextCfg = a.as_string();

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


bool TextData::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing item data '" + mName +"' ...");


    if(ObtainPlainText(mTextCfg, mText)==false){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}



//=====================================================================================



TextSegmentData::TextSegmentData(const std::string &_name) : ItemData(_name, ItemDataType::TEXT_SEGMENT)
{

}


bool TextSegmentData::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item data '" + mName + "' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            mName = a.as_string();

        }else if(attributeName=="text"){
            mTextCfg = a.as_string();

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


bool TextSegmentData::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing item data '" + mName +"' ...");


    mTextSegment = ObtainTextSegment2(mTextCfg);
    if(mTextSegment==nullptr){
        return false;
    }

    dbgSystem.removeLastMessage();
    return true;

}




//=====================================================================================



AniSpriteObjectData::AniSpriteObjectData(const std::string &_name) : ItemData(_name, ItemDataType::ANI_SPRITE_OBJECT)
{

}


AniSpriteObjectData::AniSpriteObjectData(const AniSpriteObjectData & _src) : ItemData(_src)
{

    mAniSpriteObject = new AniSpriteObject(*_src.mAniSpriteObject);
    mAniSpriteObject->_setParentObject(this);

    mText = _src.mText;
    mDataNameForText = _src.mDataNameForText;
    mDataForText = nullptr;                           // LINK

}


AniSpriteObjectData & AniSpriteObjectData::operator=(const AniSpriteObjectData &_src)
{

    if(&_src==this){
        return *this;
    }

    ItemData::operator=(_src);

    //---
    mAniSpriteObject = new AniSpriteObject(*_src.mAniSpriteObject);
    mAniSpriteObject->_setParentObject(this);

    mText = _src.mText;
    mDataNameForText = _src.mDataNameForText;
    mDataForText = nullptr;                           // LINK

    return *this;

}



AniSpriteObjectData::~AniSpriteObjectData()
{

    if(mAniSpriteObject){
        delete mAniSpriteObject;
    }

}


bool AniSpriteObjectData::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item data '" + mName + "' ...");


    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            mName = a.as_string();

        }else if(attributeName=="text"){
            mText = a.as_string();

        }else if(attributeName=="dataForText"){
            mDataNameForText = a.as_string();

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


bool AniSpriteObjectData::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing item data '" + mName +"' ...");


    // mName must be the same as for the gfx object !
    GfxObjectCfg *cfg = _scene->gfxObjectCfgStorage().getObject(mName);
    if(cfg==nullptr){
        return false;
    }
    if(cfg->type()!=GfxObjectType::ANIMATED_SPRITE){
        dbgSystem.addMessage("Gfx object '" + mName +"' is not an 'animated sprite'!");
        return false;
    }

    mAniSpriteObject = new AniSpriteObject(cfg, this);
    if(mAniSpriteObject->initConnections(_scene)==false){
        return false;
    }

    if(mAniSpriteObject->sprite()->type()==SpriteType::TEXT){
        TextSprite *ts = static_cast<TextSprite*>(mAniSpriteObject->sprite());
        if(mText.empty()==false){
            ts->setText(mText);
        }
    }

    dbgSystem.removeLastMessage();
    return true;

}


bool AniSpriteObjectData::setupInternalDataConnections(Item *_item)
{

    if(mDataNameForText.empty()==false){

        Item *i = _item;
        while(i){
            mDataForText = i->findData(mDataNameForText, false);
            if(mDataForText){
                break;
            }
            i = i->parentItem();
        }
    }

    return true;

}


void AniSpriteObjectData::start(Vec2f _pos)
{

    if(mDataForText){
        if(mAniSpriteObject->sprite()->type()==SpriteType::TEXT){
            TextSprite *ts = static_cast<TextSprite*>(mAniSpriteObject->sprite());
            ts->setText(mDataForText->valueString());
        }
    }

    mAniSpriteObject->start(_pos);

}


void AniSpriteObjectData::update()
{
    mAniSpriteObject->update();

}


void AniSpriteObjectData::end()
{
    mAniSpriteObject->end();
}


bool AniSpriteObjectData::isAnimationStalled()
{
    return mAniSpriteObject->isAnimationStalled();

}


bool AniSpriteObjectData::isAnimationStopped()
{
    return mAniSpriteObject->isAnimationStopped();

}




//=====================================================================================



TweenData::TweenData(const std::string &_name) : ItemData(_name, ItemDataType::TWEEN)
{

}



bool TweenData::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading item data '" + mName + "' ...");


    float startValue = 0.0f;
    float endValue = 1.0f;
    float durationS = 1.0f;

    for(pugi::xml_attribute a = _node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){
            mName = a.as_string();

        }else if(attributeName=="start"){
            std::string sValue = a.as_string();
            if(StdString::floatNumber(sValue, startValue)==false){
                return false;
            }

        }else if(attributeName=="end"){
            std::string sValue = a.as_string();
            if(StdString::floatNumber(sValue, endValue)==false){
                return false;
            }

        }else if(attributeName=="duration"){
            std::string sValue = a.as_string();
            if(StdString::floatNumber(sValue, durationS)==false){
                return false;
            }

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(_node.name()) +"' !");
            return false;
        }
    }

    mTween.Init(startValue, endValue, durationS, EasingKind::LINEAR);


    dbgSystem.removeLastMessage();
    return true;

}


bool TweenData::initConnections(PlayedScene *_scene)
{

    dbgSystem.addMessage("Initializing item data '" + mName +"' ...");



    dbgSystem.removeLastMessage();
    return true;

}



}
