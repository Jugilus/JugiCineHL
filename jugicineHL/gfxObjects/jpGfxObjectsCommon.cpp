#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmSpriteLayer.h"
#include "jmTextSprite.h"
#include "jmCommonFunctions.h"
#include "jmSceneLayout.h"
#include "jmMap.h"
#include "jmUtilities.h"

#include "jpQueries.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
#include "jpGfxObjectsCommon.h"



namespace jugimap{



GfxObject::~GfxObject()
{

}


void GfxObject::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{
}



void GfxObject::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{
}


//============================================================================================



GSpritesObject::~GSpritesObject()
{

}


GSpritesObject::GSpritesObject(const pugi::xml_node &_node)
{

    //mBaseType = BaseObjectType::GFX_OBJECT__SPRITES;

    mName = _node.attribute("name").as_string("");
    mObjectType = GetGfxObjectTypeFromString(_node.attribute("objectType").as_string(""));
    mObjectName = _node.attribute("objectName").as_string("");
    mSrcMapName = _node.attribute("objectSourceMap").as_string("");

}


GSpritesObject::GSpritesObject(std::vector<void*>_objects, GfxObjectType _objType)
{
    mObjects = _objects;
    mObjectType = _objType;
}



bool GSpritesObject::initConnections(PlayedScene *_scene)
{

    mParentPlayerScene = _scene;

    dbgSystem.addMessage("init sprites object '" + mName + "'");


    if(mObjectType==GfxObjectType::NOT_DEFINED){
        dbgSystem.addMessage("Wrong object type!");
        return false;
    }

    if(collectObjects(mObjects, _scene, mObjectType, mObjectName, mSrcMapName)==false){
        return false;
    }

    dbgSystem.removeLastMessage();

    return true;

}


void GSpritesObject::setVisible(bool _visible)
{
     _setVisible(_visible);
     mSigVisible.reset(_visible);
}


void GSpritesObject::_setVisible(bool _visible)
{

    if(mObjectType==GfxObjectType::SPRITE){

        for(void* obj : mObjects){
            Sprite * sprite = static_cast<Sprite*>(obj);
            sprite->setVisible(_visible);
        }

    }else if(mObjectType==GfxObjectType::SPRITE_LAYER){

        for(void* obj : mObjects){
            SpriteLayer * spriteLayer = static_cast<SpriteLayer*>(obj);
            spriteLayer->setVisible(_visible);
        }

    }else if(mObjectType==GfxObjectType::MAP){

        for(void* obj : mObjects){
            Map * map = static_cast<Map*>(obj);
            map->setVisible(_visible);
        }

    }

}


bool GSpritesObject::onSignalSet(Signal *_signal)
{

    if(_signal==&mSigVisible){
        bool value = mSigVisible.value();
        _setVisible(value);
        return true;
    }

    return false;
}


void GSpritesObject::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="VISIBLE"){
        _psp.obtainValue(_signalQuery, &mSigVisible);
    }

    if(_setErrorMessage && _signalQuery.mSignal==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}



void GSpritesObject::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="VISIBLE"){
        _psp.obtainValue(_signalSetter, &mSigVisible);
    }

    if(_setErrorMessage && _signalSetter.mSignal==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }
}



//--------------------------------------------------------------------------------------



GTextSprite::GTextSprite(const pugi::xml_node &_node)
{

    //mBaseType = BaseObjectType::GFX_OBJECT__TEXT_SPRITE;

    mName = _node.attribute("name").as_string("");
    mTextSpriteName = _node.attribute("textSprite").as_string("");
    mSourceMapName = _node.attribute("sourceMap").as_string("");

}


bool GTextSprite::initConnections(PlayedScene* _scene)
{

    dbgSystem.addMessage("init '"+ mName + "'");


    std::vector<Sprite*>sprites;
    for(SceneMap *sm : _scene->sceneMaps()){
        if(sm->map()->type()==MapType::SCREEN){
            CollectSpritesWithName(sm->map(), sprites, mTextSpriteName);
            if(sprites.empty()==false){
                break;
            }
        }
    }
    if(sprites.empty()==false && sprites.front()->type()==SpriteType::TEXT){
        mTextSprite = static_cast<TextSprite*>(sprites.front());

        //if(app->usersDatabase()->activeUser()){
        //    mTextSprite->setText(app->usersDatabase()->activeUser()->name());
        //}

    }else{
        dbgSystem.addMessage("Text sprite with name '" + mTextSpriteName + "' not found!");
    }


    //---
    mSigVisible.setCallback(this);
    mSigText.setCallback(this);


    dbgSystem.removeLastMessage();
    return true;
}



//void GTextSprite::setVisible(bool _visible)
//{
//    mTextSprite->setVisible(_visible);
//}

/*
void GTextSprite::onSetBySignalSetter(Signal *_signal)
{

    if(_signal==&mSigText){
        const std::string & value = mSigText.value_currentOrNextIfSet();
        mTextSprite->setText(value);

    }else if(_signal==&mSigTextVisible){
        bool value = mSigTextVisible.value_currentOrNextIfSet();
        mTextSprite->setVisible(value);

    }



}
*/


bool GTextSprite::onSignalSet(Signal *_signal)
{

    if(_signal==&mSigText){
        const std::string & value = mSigText.value();
        mTextSprite->setText(value);
        return true;

    }else if(_signal==&mSigVisible){
        bool value = mSigVisible.value();
        mTextSprite->setVisible(value);
        return true;
    }

    return false;
}



void GTextSprite::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="TEXT"){
        _psp.obtainValue(_signalQuery, &mSigText);

    }else if(_psp.signalFullName()=="VISIBLE"){
        _psp.obtainValue(_signalQuery, &mSigVisible);
    }

    if(_setErrorMessage && _signalQuery.mSignal==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}



void GTextSprite::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    if(_psp.signalFullName()=="TEXT"){
        _psp.obtainValue(_signalSetter, &mSigText);

    }else if(_psp.signalFullName()=="VISIBLE"){
        _psp.obtainValue(_signalSetter, &mSigVisible);
    }

    if(_setErrorMessage && _signalSetter.mSignal==nullptr){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}




//--------------------------------------------------------------------------------------



bool GfxObjectsGroup::initConnections(PlayedScene* _scene)
{

    dbgSystem.addMessage("init components");


    for(GfxObject* o : mGfxObjects){
        if(o->initConnections(_scene)==false){
            return false;
        }
    }


    //---
    dbgSystem.removeLastMessage();

    return true;
}



GfxObject* GfxObjectsGroup::getGfxObject(const std::string &_name)
{


    for(GfxObject* o : mGfxObjects){
        if(o->name()==_name){
            return o;
        }
    }

    dbgSystem.addMessage("Gfx object '" + _name +"' not found!");

    return nullptr;

}





}
