#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmSprite.h"
#include "jmTextSprite.h"
#include "jmCommonFunctions.h"
#include "jmSceneLayout.h"
#include "jmMap.h"
#include "jmUtilities.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
#include "jpGfxObjectsCommon.h"



namespace jugimap{





GSpritesObject::GSpritesObject(const pugi::xml_node &_node)
{

    mType = BaseObjectType::GFX_OBJECT__SPRITES;

    mName = _node.attribute("name").as_string("");
    mObjectType = GetGfxObjectTypeFromString(_node.attribute("objectType").as_string(""));
    mObjectName = _node.attribute("objectName").as_string("");
    mSrcMapName = _node.attribute("objectSourceMap").as_string("");


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


//--------------------------------------------------------------------------------------



GTextSprite::GTextSprite(const pugi::xml_node &_node)
{

    mType = BaseObjectType::GFX_OBJECT__TEXT_SPRITE;

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

    dbgSystem.removeLastMessage();

    return true;

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
