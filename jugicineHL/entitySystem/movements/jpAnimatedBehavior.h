#ifndef PLAYER__BEHAVIOR__ANIMATED_BEHAVIOR__H
#define PLAYER__BEHAVIOR__ANIMATED_BEHAVIOR__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jmAnimationCommon.h"
#include "jmTimer.h"
#include "jpGlobal.h"
#include "jpSpeedGenerators.h"
#include "jpMovementBase.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class Sprite;
//class EPathway;
class Entity;
class PlayedScene;
class AnimationInstance;
class AnimationPlayer;
class BoolSignal;
class VectorShape;
class AnimationObject;




enum class AnimatedBehaviorState
{
    STANDING,
    IDLE,
    TURNING_AROUND,
    MOVING,
    UNKNOWN

};


//-------------------------------------------------------------------------------------


struct AnimatedMovementCfg : public MovementEngineCfg
{

    AnimatedMovementCfg(MovementEngineFactory *_factory, const std::string &_name) : MovementEngineCfg(_factory, _name){}
    bool initCfg(const pugi::xml_node &_node) override;


    std::vector<std::string> animations;

};



struct AnimatedMovementData : public MovementEngineData
{

    bool initConnections(PlayedScene *_scene, Entity *_actor);

    AnimatedMovementCfg *cfg = nullptr;     // LINK


    BoolSignal sigDisabled;


    std::vector<AnimationInstance *>animations;

};



//------------------------------------------------------------------------------------------




class AnimatedMovementEngine : public MovementEngine
{
public:

    AnimatedMovementEngine(MovementEngineFactory * _factory);

    void createDataObjects(std::vector<MovementEngineCfg*> &_cfgs) override;
    bool initDataObjectsConnections(PlayedScene *_scene, Entity *_actor) override;

    bool init(Entity *_entity) override;
    bool start(MovementEngineData *_data) override;
    b2Vec2 update(EngineUpdateParameters &eup) override;
    void stop() override;
    MovementEngineData* getMovementEngineData(const std::string &_name, bool _setErrorMessage) override;

    AnimatedBehaviorState state(){ return mState; }

    MovementEngineData* currentData() override { return mCurrentData; }
    //std::vector<AnimatedBehaviorData> & animatedBehaviorDatas(){ return mAniMovementDatas; }
    //AnimatedBehaviorData* getAnimatedBehaviorData(const std::string &_name, bool _setErrorMessage = true);

    //b2Vec2 currentPositionM(){ return mVelocityGenerator.currentPositionM(); }
    //bool isPathwayFinished(bool _activeGroundMoveCommand);



private:
    AnimatedBehaviorState mState = AnimatedBehaviorState::STANDING;
    //VGPathMovement mVelocityGenerator;
    Direction mInputDirection;

    AnimatedMovementData* mCurrentData = nullptr;                   // LINK

    std::vector<AnimatedMovementData>mAniMovementDatas;          // stored

    AnimationObject* mAniObject = nullptr;                      // LINK
    Sprite *mActorSprite = nullptr;                             // LINK
    Sprite *mAnimatedSprite = nullptr;                          // LINK
    Sprite *mTracedSprite = nullptr;                             // LINK
    Vec2f mPrevPos{0.0f, 0.0f};

    //AnimationPlayer mBehAniPlayer;
    //AnimationInstance *mBehAniInstance = nullptr;             // OWNED



    //bool tracingActive = false;


};





//------------------------------------------------------------------------------------------


class AnimatedMovementFactory : public MovementEngineFactory
{
public:

    AnimatedMovementFactory(const std::string &_typeSignature, MovementEngineType _type) : MovementEngineFactory(_typeSignature, _type){}

    AnimatedMovementCfg* createMovementEngineCfg(const std::string &_name) override;
    AnimatedMovementEngine* createMovementEngine() override;


};












}






#endif
