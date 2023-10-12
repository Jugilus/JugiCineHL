#ifndef PLAYER__MOVEMENT__GROUND_MOVEMENT__H
#define PLAYER__MOVEMENT__GROUND_MOVEMENT__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jmCommon.h"
#include "jpGlobal.h"
#include "jpVariables.h"
#include "jpSpeedGenerators.h"
#include "jpMovementBase.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class Entity;
class Sprite;
class FrameAnimationInstance;
class AnimationPlayer;
class PlayedScene;
class Animation;
class BoolSignal;
struct FixtureUserData;
struct SourceFixture;






enum class GroundMovementState
{
    STANDING,
    IDLE,
    TURNING_AROUND,
    MOVING,
    STEEP_SLOPE_STUMBLING,
    UNKNOWN

};

GroundMovementState GetGroundMovementStateFromString(const std::string &state);
std::string GetGroundMovementStateString(GroundMovementState);

//------------------------------------------------------------------------------------



struct GroundMovementCfg : public MovementEngineCfg
{
public:
    static const float minMaxSlope;
    static const float maxMaxSlope;

    GroundMovementCfg(MovementEngineFactory *_factory, const std::string &_name) : MovementEngineCfg(_factory, _name){}

    bool initCfg(const pugi::xml_node &_node) override;
    void collectCustomSensors(std::vector<CustomSensorCfg> &_customSensors) override;



    float maxSpeed = 5.0f;                  // meters
    float accDistance = 0.0f;               // meters,seconds
    float maxSlope = 45.0f;                 // degrees
    //b2Vec2 maxSlopeNormal{0.0f, 0.0f};
    float yMinNormalForMaxSlope = 0.707f;
    float jumpXSpeedInheritFactor = 0.5f;
    float animationSpeedFactor = 1.0f;
    float steepSlopeSpeed = 7.0f;

    float idleAnimationDelay = 7.0;


    std::string sigMoveLeft;
    std::string sigMoveRight;

    std::string aniStandingLeft;
    std::string aniStandingRight;
    std::string aniIdleLeft;
    std::string aniIdleRight;
    std::string aniTurningToLeft;
    std::string aniTurningToRight;
    std::string aniMovingLeft;
    std::string aniMovingRight;
    std::string aniSteepSlopeStaggeringLeft;
    std::string aniSteepSlopeStaggeringRight;

    Direction bodyOrientationStandingLeft = Direction::LEFT;
    Direction bodyOrientationStandingRight = Direction::RIGHT;
    Direction bodyOrientationMovingLeft = Direction::LEFT;
    Direction bodyOrientationMovingRight = Direction::RIGHT;


    //---

    enum class BaseState
    {
        STANDING,
        MOVING
    };


    struct CustomStateCfg
    {

        bool initCfg(const pugi::xml_node &_node);


        std::string name;
        BaseState baseState = BaseState::STANDING;

        std::string sigTrigger;

        std::string aniRight;
        std::string aniLeft;

        //---
        std::string contextTrigger;

        //---
        CustomSensorCfg customSensorCfg;
    };

    std::vector<CustomStateCfg>customStates;

};



struct GroundMovementCustomStateData
{

    ~GroundMovementCustomStateData();
    bool initConnections(PlayedScene *_scene, Entity *_actor);


    GroundMovementCfg::CustomStateCfg *cfg = nullptr;          //LINK
    SignalQuery trigger;
    AnimationInstance * aniLeft = nullptr;                  // OWNED
    AnimationInstance * aniRight = nullptr;                 // OWNED

    //---
    SignalQuery contextTrigger;

    //---
    SourceFixture *triggeringExtraShapeSourceFixture = nullptr;
    bool extraShapeFixtureCreated = false;

};



struct GroundMovementData : public GroundMovementDataBase
{
public:

    ~GroundMovementData() override;

    bool initConnections(PlayedScene *_scene, Entity *_actor);

    GroundMovementCfg * cfg = nullptr;          // LINK


    //--- animations
    AnimationInstance * aniStandingLeft = nullptr;      // OWNED
    AnimationInstance * aniStandingRight = nullptr;      // OWNED
    AnimationInstance * aniIdleLeft = nullptr;
    AnimationInstance * aniIdleRight = nullptr;
    AnimationInstance * aniTurningToLeft = nullptr;
    AnimationInstance * aniTurningToRight = nullptr;
    AnimationInstance * aniMovingLeft = nullptr;
    AnimationInstance * aniMovingRight = nullptr;
    AnimationInstance * aniSteepSlopeStaggeringLeft = nullptr;
    AnimationInstance * aniSteepSlopeStaggeringRight = nullptr;

    std::vector<GroundMovementCustomStateData>customStandingStates;
    std::vector<GroundMovementCustomStateData>customMovingStates;


};



//------------------------------------------------------------------------------------------


class VGGroundMovement
{
public:

    void startMovement(GroundMovementCfg &cfg, Direction _direction, float _startSpeed, SlidingGroundCfg *_slidingCfg);
    b2Vec2 updateVelocity(float _timeStep, Direction _inputDirection, int _blockedDirections, b2Vec2 _slopeNormal);
    void stop();

    Direction direction() const { return mDirection; }
    b2Vec2 inputUnitVelocity() const { return  mInputUnitVelocity; }
    b2Vec2 unitVelocity() const { return  mUnitVelocity; }
    b2Vec2 velocity(){ return  mVelocity; }
    float speed(){ return  mSpeedGenerator.speed(); }
    SpeedGenerator &speedGenerator(){ return mSpeedGenerator; }


private:
    Direction mDirection = Direction::NONE;
    SpeedGenerator mSpeedGenerator;

    //---
    b2Vec2 mInputUnitVelocity{0.0f, 0.0f};
    b2Vec2 mUnitVelocity{0.0f, 0.0f};
    b2Vec2 mVelocity{0.0f, 0.0f};

};


//------------------------------------------------------------------------------------------


class ESlopeSliding
{
public:

    //bool init(GroundMovementCfg &cfg);

    void setActiveSlidingGroundCfg(SlidingGroundCfg *_cfg);
    SlidingGroundCfg * slidingGroundCfg(){ return mActiveSlidingGroundCfg; }
    bool isActive(){ return (mActiveSlidingGroundCfg != nullptr); }
    void stop();


    b2Vec2 updateVelocity(float _timeStep, b2Vec2 _prevEngineVelocity, b2Vec2 _slopeNormal);

    b2Vec2 unitVelocity(){ return mUnitVelocity; }
    b2Vec2 velocity(){ return  mVelocity; }
    float speed(){ return  mSpeed; }

    Direction direction() const { return mDirection; }

private:
    //bool mActive = false;
    SlidingGroundCfg *mActiveSlidingGroundCfg = nullptr;

    float mSpeedAt45deg = 0.0f;
    float mSpeedMax = 0.0f;
    float mSpeed = 0.0f;

    float mAccDistanceAt45degSpeed = 0.0f;
    float mDecelerationDistance = 0.0f;
    float mDeceleration = 0.0f;

    Direction mDirection = Direction::NONE;

    b2Vec2 mSlopeNormal{0.0f, 0.0f};
    b2Vec2 mUnitVelocity{0.0f, 0.0f};
    b2Vec2 mVelocity{0.0f, 0.0f};

};


//------------------------------------------------------------------------------------------


class EGroundConveyor
{
public:

    void setActiveConveyorCfg(ConveyorGroundCfg *_cfg);
    ConveyorGroundCfg * conveyorGroundCfg(){ return mActiveConveyorCfg; }
    bool isActive(){ return (mActiveConveyorCfg!=nullptr); }

    b2Vec2 updateVelocity(float _timeStep, int _blockedDirections, b2Vec2 _slopeNormal);
    void stop();

    b2Vec2 velocity(){ return  mVelocity; }
    float speed(){ return  mSpeed; }
    Direction direction() const { return mDirection; }

private:
    ConveyorGroundCfg *mActiveConveyorCfg = nullptr;
    float mSpeed = 0.0f;
    Direction mDirection = Direction::NONE;

    b2Vec2 mDirectionUnitVelocity{0.0f, 0.0f};
    b2Vec2 mVelocity{0.0f, 0.0f};

};



//------------------------------------------------------------------------------------------


class GroundMovementEngine : public GroundMovementEngineBase
{
public:

    GroundMovementEngine(MovementEngineFactory * _factory) : GroundMovementEngineBase(_factory){}

    void createDataObjects(std::vector<MovementEngineCfg*> &_cfgs) override;
    bool initDataObjectsConnections(PlayedScene *_scene, Entity *_actor) override;

    bool init(Entity *_entity) override;
    bool start(MovementEngineData *_data) override;
    b2Vec2 update(EngineUpdateParameters &eup) override;
    void stop() override;

    MovementEngineData* getMovementEngineData(const std::string &_name, bool _setErrorMessage) override;


    //void setGroundNormal(b2Vec2 _groundNormal){ mGroundNormal = _groundNormal;}
    //b2Vec2 groundNormal(){ return mGroundNormal;}

    //void setGroundSliding(SlidingGroundCfg * _cfg){ mSlopeSliding.setActiveSlidingGroundCfg(_cfg); }
    //void setGroundConveyor(ConveyorGroundCfg *_cfg){ mGroundConveyor.setActiveConveyorCfg(_cfg); }


    GroundMovementState state(){ return mState; }

    MovementEngineData* currentData() override { return mCurrentData; }
    //std::vector<GroundMovementData> & groundMovementDatas(){ return mGroundMovementDatas; }
    //GroundMovementData* getGroundMovementData(const std::string &_name, bool _setErrorMessage = true);
    VGGroundMovement & groundMovement(){ return mGroundMovement; }


    Direction getBodyOrientation();


private:
    GroundMovementState mState = GroundMovementState::STANDING;
    GroundMovementCustomStateData *mCustomState = nullptr;

    VGGroundMovement mGroundMovement;
    ESlopeSliding mSlopeSliding;
    EGroundConveyor mGroundConveyor;

    Direction mInputDirection;
    //b2Vec2 mGroundNormal;
    GroundMovementData* mCurrentData = nullptr;        // LINK
    std::vector<GroundMovementData>mGroundMovementDatas;          // stored

    //----
    IntSignal mSigState;


    void setState(GroundMovementState _state)
    {
        mState = _state;
        mSigState._setOnNextFrame(static_cast<int>(mState));
    }


    void update_stateSTANDING(EngineUpdateParameters &eup);
    void update_stateMOVING(EngineUpdateParameters &eup);
    void update_stateTURNING_AROUND(EngineUpdateParameters &eup);

    void updateGroundMovement(EngineUpdateParameters &eup);
    void updateSteepSlopeSliding(EngineUpdateParameters &eup);
    void updateSliding(EngineUpdateParameters &eup);
    void updateConveyor(EngineUpdateParameters &eup);

    void updateCustomState(EngineUpdateParameters &eup);

};



//------------------------------------------------------------------------------------------


class GroundMovementFactory : public MovementEngineFactory
{
public:

    GroundMovementFactory(const std::string &_typeSignature, MovementEngineType _type) : MovementEngineFactory(_typeSignature, _type){}

    MovementEngineCfg* createMovementEngineCfg(const std::string &_name) override;
    GroundMovementEngine* createMovementEngine() override;


};



}






#endif
