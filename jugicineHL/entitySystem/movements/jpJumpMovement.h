#ifndef PLAYER__MOVEMENT__JUMP_MOVEMENT__H
#define PLAYER__MOVEMENT__JUMP_MOVEMENT__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jpGlobal.h"
#include "jpSpeedGenerators.h"
#include "jpMovementBase.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class Entity;
class Sprite;
class PlayedScene;
class Animation;
class AnimationInstance;
class FrameAnimationInstance;
class BoolSignal;
struct EntityContactSignal;



enum class JumpMovementState : int
{
    UNKNOWN         = 0,
    TAKING_OFF      = 1,
    ASCENDING       = 2,
    DESCENDING      = 3,
    LANDING         = 4,
    LANDED          = 5

};

JumpMovementState GetJumpMovementStateFromString(const std::string &state);


//------------------------------------------------------------------------------------


struct JumpMovementCfg : public JumpMovementCfgBase
{

    static const float minIntermediateHeightRatio;
    static const float maxIntermediateHeightRatio;
    static const float minIntermediateSpeedRatio;
    static const float maxIntermediateSpeedRatio;

    JumpMovementCfg(MovementEngineFactory *_factory, const std::string &_name) : JumpMovementCfgBase(_factory, _name){}

    bool initCfg(const pugi::xml_node &_node) override;


    float maxHeight = 5.0f;              // in meters
    float pushOffSpeed = 10.0f;
    float intermediateHeightRatio = 0.7f;
    float intermediateSpeedRatio = 0.8;

    float xMaxSpeed = 3.0;
    float xAccDistance = 0.0f;
    float xGroundSpeedInheritFactor = 0.0f;

    float pushImpulseSpeedFactor = 0.7f;
    float maxSlope = 90.0f;
    float maxSlopeNormalY = 0.0f;


    // signals
    std::string sigStartJump;
    std::string sigMoveLeft;
    std::string sigMoveRight;

    // animations
    std::string aniTakeOffLeft;
    std::string aniTakeOffRight;
    std::string aniAscendingLeft;
    std::string aniAscendingRight;
    std::string aniAtMaxHeightLeft;
    std::string aniAtMaxHeightRight;
    std::string aniDescendingLeft;
    std::string aniDescendingRight;
    std::string aniLandingLeft;
    std::string aniLandingRight;

};


struct JumpMovementData : public MovementEngineData
{


    ~JumpMovementData() override;

    bool initConnections(PlayedScene *_scene, Entity *_actor);

    JumpMovementCfg *cfg = nullptr;         // LINK

    //----
    SignalQuery startJump;                  // LINK
    SignalQuery moveLeft;                   // LINK
    SignalQuery moveRight;                  // LINK
    BoolSignal sigStartJumpObj;
    BoolSignal sigMoveLeftObj;
    BoolSignal sigMoveRightObj;

    BoolSignal sigDisabled;


    //----
    AnimationInstance * aniTakeOffLeft = nullptr;           // OWNED
    AnimationInstance * aniTakeOffRight = nullptr;
    AnimationInstance * aniAscendingLeft = nullptr;
    AnimationInstance * aniAscendingRight = nullptr;
    AnimationInstance * aniAtMaxHeightLeft = nullptr;
    AnimationInstance * aniAtMaxHeightRight = nullptr;
    AnimationInstance * aniDescendingLeft = nullptr;
    AnimationInstance * aniDescendingRight = nullptr;
    AnimationInstance * aniLandingLeft = nullptr;
    AnimationInstance * aniLandingRight = nullptr;

};


//------------------------------------------------------------------------------------------


class VGJumpMovement
{
public:


    void start(JumpMovementCfg &_cfg, Direction _xDirection, b2Vec2 _inheritedVelocity, bool _startWithDescending=false);

    b2Vec2 updateVelocity(float _timeStep, Direction _inputXDirection, int _blockedDirections);

    void stop();

    JumpSpeedGenerator::State state(){ return mJumpSpeedGenerator.state(); }
    Direction xDirection(){ return mXDirection; }
    b2Vec2 velocity(){ return  mVelocity; }

    //void setDistToCeiling(float _distToCeiling){ mJumpSpeedGenerator.setDistToCeiling(_distToCeiling); }
    //float distToBlocked(){ return mJumpSpeedGenerator.distToCeiling(); }
    //void setState(State _state){ mState = _state; }

    JumpSpeedGenerator & jumpSpeedGenerator(){ return mJumpSpeedGenerator; }


private:
    //State mState = State::ON_GROUND;
    Direction mXDirection = Direction::NONE;

    JumpSpeedGenerator mJumpSpeedGenerator;
    SpeedGenerator mXSpeedGenerator;

    //---
    b2Vec2 mVelocity{0.0f, 0.0f};

    void dbgPrint();

};


//------------------------------------------------------------------------------------------


class JumpMovementEngine : public MovementEngine
{
public:

    JumpMovementEngine(MovementEngineFactory * _factory);

    void createDataObjects(std::vector<MovementEngineCfg*> &_cfgs) override;
    bool initDataObjectsConnections(PlayedScene *_scene, Entity *_actor) override;

    bool init(Entity *_entity) override;
    //void start(JumpMovementData *_jumpMovementCfg, Direction _xDirection, AnimationPlayer *_animationPlayer, b2Vec2 _actorVelocity, bool _startWithDescending);
    bool start(MovementEngineData *_data) override;

    //b2Vec2 update(int _blockedDirections, UpdateMode _updateMode);
    b2Vec2 update(EngineUpdateParameters &eup) override;
    void stop() override;

    MovementEngineData * currentData() override { return mCurrentData; }
    MovementEngineData* getMovementEngineData(const std::string &_name, bool _setErrorMessage) override;
    void obtainSignal_signalQuery(SignalQuery &_signalQuery, const std::string &_data, const std::string &_signalName, const std::string &_signalValue, bool _setErrorMessage=true) override;
    void obtainSignal_signalSetter(SignalSetter &_signalSetter, const std::string &_data, const std::string &_signalName, const std::string &_signalValue, bool _setErrorMessage = true) override;

    JumpMovementState state(){ return mState; }
    //Direction direction(){ return mXDirection; }

    //std::vector<JumpMovementData> &jumpMovementDatas(){ return mJumpMovementDatas; }
    //JumpMovementData* getJumpMovementData(const std::string &_name, bool _setErrorMessage = true);
    VGJumpMovement &jumpMovement(){ return mJumpMovement; }

private:

    JumpMovementState mState = JumpMovementState::TAKING_OFF;
    VGJumpMovement mJumpMovement;
    JumpMovementData *mCurrentData = nullptr;                // LINK
    std::vector<JumpMovementData>mJumpMovementDatas;

    //----
    IntSignal mSigState;



    void setState(JumpMovementState _state)
    {
        mState = _state;
        mSigState._setOnNextFrame(static_cast<int>(mState));
    }


    void manageApproachingCeiling();


};



//------------------------------------------------------------------------------------------


class JumpMovementFactory : public MovementEngineFactory
{
public:

    JumpMovementFactory(const std::string &_typeSignature, MovementEngineType _type) : MovementEngineFactory(_typeSignature, _type){}

    JumpMovementCfg* createMovementEngineCfg(const std::string &_name) override;
    JumpMovementEngine* createMovementEngine() override;


};







}






#endif
