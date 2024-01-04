#ifndef PLAYER__GFXOBJECTS__MOVING__H
#define PLAYER__GFXOBJECTS__MOVING__H


#include <memory>

#include "jmAnimationCommon.h"
#include "jmSignal.h"
#include "jpSettings.h"
#include "jpVariables.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class Sprite;
class TextSprite;
class SpriteLayer;
class PlayedScene;




enum class MovingType
{
    UNKNOWN,
    LINEAR,
    SPRITES_OBJECT,
    TEXT_SPRITE,

};



struct MovingCfg
{

    MovingCfg(const std::string &_name, MovingType _type) : mName(_name), mType(_type) {}

    virtual bool initCfg(const pugi::xml_node &_node){ return true; }


    const std::string & name(){ return mName; }
    MovingType type(){ return mType; }


    std::string mName;
    MovingType mType = MovingType::UNKNOWN;


};



class Moving : public SignalCallback
{
public:
    Moving();
    Moving(MovingCfg *_cfg, BaseObject *_parentObject);

    virtual ~Moving();

    virtual void update() = 0;

    //virtual bool build(PlayedScene* _scene, GfxObjectCfg* _cfg, BaseObject *_parentObject);
    virtual bool initConnections(PlayedScene* _scene) = 0;


    const std::string &name(){ return mCfg->name(); }
    MovingType type(){ return mCfg->type(); }
    BaseObject* parentObject(){ return mParentObject;}

    void _setParentObject(BaseObject *_parentObject){ mParentObject = _parentObject; }


protected:
    MovingCfg *mCfg = nullptr;                                   // LINK
    BaseObject *mParentObject = nullptr;                        // LINK

    Vec2f mVelocity;
    AnimationInstance *mAniInstance = nullptr;                  // OWNED;

};


//========================================================================================

struct LinearMovingCfg : public MovingCfg
{

    LinearMovingCfg(const std::string &_name) : MovingCfg(_name, MovingType::LINEAR){}
    bool initCfg(const pugi::xml_node &_node) override;

    std::string sourceSprite;
    std::string animation;
    std::string layer;
    std::string map;

};



class LinearMoving : public Moving
{
public:
    LinearMoving(){}
    LinearMoving(MovingCfg *_cfg, BaseObject *_parentObject) : Moving(_cfg, _parentObject){}
    //Moving(const Moving &_src);

    virtual ~LinearMoving(){}

    virtual void update(){}

    //virtual bool build(PlayedScene* _scene, GfxObjectCfg* _cfg, BaseObject *_parentObject);
    virtual bool initConnections(PlayedScene* _scene){ return true; }


    const std::string &name(){ return mCfg->name(); }
    MovingType type(){ return mCfg->type(); }
    BaseObject* parentObject(){ return mParentObject;}

    void _setParentObject(BaseObject *_parentObject){ mParentObject = _parentObject; }


protected:
    MovingCfg *mCfg = nullptr;                                   // LINK
    BaseObject *mParentObject = nullptr;                        // LINK

    Vec2f mVelocity;

};






}






#endif
