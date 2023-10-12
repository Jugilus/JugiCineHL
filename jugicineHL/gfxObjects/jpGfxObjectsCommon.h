#ifndef PLAYER__GFXOBJECTS__COMMON__H
#define PLAYER__GFXOBJECTS__COMMON__H


#include <memory>
#include "jpSettings.h"
#include "jpVariables.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class TextSprite;
class PlayedScene;






class GfxObject : public BaseObject
{
public:
    GfxObject(){ mType = BaseObjectType::GFX_OBJECT; }
    virtual ~GfxObject(){}

    virtual bool initConnections(PlayedScene* _scene) = 0;

    const std::string &name(){ return mName; }
    PlayedScene* parentPlayerScene(){ return mParentPlayerScene; }

protected:
    PlayedScene *mParentPlayerScene = nullptr;
    std::string mName;

};


//--------------------------------------------------------------------------------


class GSpritesObject : public GfxObject
{
public:

    GSpritesObject(const pugi::xml_node &_node);
    virtual ~GSpritesObject(){}

    virtual bool initConnections(PlayedScene* _scene);

    GfxObjectType objectType(){ return mObjectType; }
    std::vector<void*> &objects(){ return mObjects; }


protected:
    std::vector<void*>mObjects;                         // LINKS
    GfxObjectType mObjectType = GfxObjectType::NOT_DEFINED;

    //cfg
    std::string mObjectName;
    std::string mSrcMapName;

};


//--------------------------------------------------------------------------------


class GTextSprite : public GfxObject
{
public:

    GTextSprite(const pugi::xml_node &_node);
    virtual bool initConnections(PlayedScene* _scene);

    TextSprite *textSprite(){ return mTextSprite; }


private:
    TextSprite *mTextSprite = nullptr;      // LINK

    //--- cfg
    std::string mTextSpriteName;
    std::string mSourceMapName;

};


//--------------------------------------------------------------------------------


class GfxObjectsGroup
{
public:

    bool initConnections(PlayedScene* _scene);


    GfxObject* getGfxObject(const std::string &_name);
    std::vector<GfxObject*> &gfxObjects(){ return mGfxObjects; }


private:
    std::vector<GfxObject*>mGfxObjects;

};





}






#endif
