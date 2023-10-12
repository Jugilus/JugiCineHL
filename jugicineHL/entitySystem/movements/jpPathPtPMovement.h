#ifndef PLAYER__MOVEMENT__PATH_PTP_MOVEMENT__H
#define PLAYER__MOVEMENT__PATH_PTP_MOVEMENT__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jmTimer.h"
#include "jpGlobal.h"
#include "jpSpeedGenerators.h"
#include "jpAngularVelocity.h"
#include "jpMovementBase.h"
//#include "jpPathMovement.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class Entity;
class PlayedScene;
class Animation;
class BoolSignal;
class VectorShape;




enum class PathPTPMovementState
{

    UNKNOWN =           0,
    STANDING =          1,
    IDLE =              2,
    TURNING_AROUND =    3,
    MOVING =            4,

};

PathPTPMovementState GetPathPTPMovementStateFromString(const std::string &state);




enum class PathPTPRotationState
{
    STANDING_STILL,
    MOVING,
    UNKNOWN
};


PathPTPRotationState GetPathPTPRotationStateFromString(const std::string &state);


//-----------------------------------------------------------------------------------------


struct PathPTPMovementCfg : public MovementEngineCfg
{

    PathPTPMovementCfg(MovementEngineFactory *_factory, const std::string &_name) : MovementEngineCfg(_factory, _name){}
    bool initCfg(const pugi::xml_node &_node);

    float maxSpeed = 5.0f;              // meters
    float accDistance = 1.0f;           // meters,seconds

    float maxAngularSpeed = 0.0f;          // degrees per second
    float angularAccDistance = 0.0;     // degrees, seconds


    float idleAnimationDelay = 7.0;

    std::string moveForward;
    std::string moveBackward;
    std::string continueMovement;
    std::string aniStandingForward;
    std::string aniStandingBackward;
    std::string aniIdleForward;
    std::string aniIdleBackward;
    std::string aniTurningToForward;
    std::string aniTurningToBackward;
    std::string aniMovingForward;
    std::string aniMovingBackward;

    std::string rotateCW;
    std::string rotateCCW;

};

//------------------------------------------------------------------------------------------




struct PathPTPMovementData : public MovementEngineData
{

    ~PathPTPMovementData();

    bool initConnections(PlayedScene *_scene, Entity *_actor);

    PathPTPMovementCfg *cfg = nullptr;     // LINK

    VectorShape *vectorShape = nullptr;         // LINK

    //--- parameters set in the editor
    bool continousCircularMovement = false;
    int currentPathPointIndex = 0;              // obtained from the sprite position
    int startingShapePointIndex = -1;
    int endShapePointIndexOffset = -1;
    float duration = 0.0f;


    //--- signals
    SignalQuery moveForward;                     // link to trigger object bellow OR input command for the player controlled actors
    SignalQuery moveBackward;
    BoolSignal sigMoveForwardObj;                 // signal object for computer controlled actors
    BoolSignal sigMoveBackwardObj;                // signal object for computer controlled actors

    BoolSignal sigDisabled;


    //--- rotation
    float startAngle = 0.0f;                    // degrees
    float endAngle = 0.0f;                      // degrees
    RotationDirection startRotationDirection = RotationDirection::NONE;
    bool continousCircularRotation = false;

    SignalQuery rotateCW;                   // link to trigger object bellow OR input command for the player controlled actors
    SignalQuery rotateCCW;
    BoolSignal sigRotateCWObj;                 // trigger object for computer controlled actors
    BoolSignal sigRotateCCWObj;                // trigger object for computer controlled actors

    BoolSignal sigRotateDisabled;


    //--- animations
    AnimationInstance * aniStandingForward = nullptr;
    AnimationInstance * aniStandingBackward = nullptr;
    AnimationInstance * aniIdleForward = nullptr;
    AnimationInstance * aniIdleBackward = nullptr;
    AnimationInstance * aniTurningToForward = nullptr;
    AnimationInstance * aniTurningToBackward = nullptr;
    AnimationInstance * aniMovingForward = nullptr;
    AnimationInstance * aniMovingBackward = nullptr;

};


//----------------------------------------------------------------------------------


class VGPathPTPMovement
{
public:

    void init(PathPTPMovementData &data, Direction _direction);
    void initMovement(PathPTPMovementData &data, Direction _direction, bool _continueFullSpeed = false);
    b2Vec2 updateVelocity(float _timeStep, bool _finishFullSpeed);
    void stop();

    b2Vec2 unitVelocity(){ return  mUnitVelocity; }
    b2Vec2 velocity(){ return  mVelocity; }
    float speed(){ return mSpeedGenerator.speed();}
    float passedDistance(){ return mSpeedGenerator.passedDistance(); }

    //bool isMovementAllowed(){ return mEndIndex != -1; }
    //bool isMovementAllowed(){ return (mSegment.index2 != -1 || mContinousCircularMovement==true) ; }
    int currentIndex(){ return mCurrentIndex; }
    b2Vec2 currentPoint();
    b2Vec2 currentPosition(){ return mCurrentPos; }

    PointToPointSpeedGenerator::State state(){ return mSpeedGenerator.state(); }

    PathPointToPointSegment & segment(){return  mSegment;}

    bool usingPathSegment(){ return mUsingPathSegment; }

    int startIndex(){ return mStartIndex; }
    int endIndex(){ return mEndIndex; }

    bool isActive(){ return mActive; }


private:

    PointToPointSpeedGenerator mSpeedGenerator;

    //---
    bool mActive = false;

    VectorShape *mPath = nullptr;       //LINK
    PathPointToPointSegment mSegment;

    bool mContinousCircularMovement = false;
    int mStep = 1;

    int mCurrentIndex = 0;

    b2Vec2 mCurrentPos{0.0f, 0.0f};
    Vec2f mCurrentPosP{0.0f, 0.0f};

    float mCurrentDistanceP = 0.0f;

    b2Vec2 mUnitVelocity{0.0f, 0.0f};
    b2Vec2 mVelocity{0.0f, 0.0f};

    bool mUsingPathSegment = false;
    float mPathSegmentLength = 0.0f;
    int mStartIndex = 0;
    int mEndIndex = -1;

    void obtainUnitVelocity(float passedDistanceP);
    int obtainNextIndex(int _startIndex, float &_distance);


};


//------------------------------------------------------------------------------------------




class PathPTPMovementEngine : public MovementEngine
{
public:

    //enum class State
    //{
    //    WAITING,
    //    MOVING,
    //};

    PathPTPMovementEngine(MovementEngineFactory * _factory);

    bool initDataObjectsConnections(PlayedScene *_scene, Entity *_actor) override;
    void createDataObjects(std::vector<MovementEngineCfg*> &_cfgs) override;


    bool init(Entity *_entity) override;
    //void start(PathMovementData *_data, Direction _direction, Entity *_entity);
    bool start(MovementEngineData *_data) override;
    b2Vec2 update(EngineUpdateParameters &eup) override;
    void stop() override;

    MovementEngineData* getMovementEngineData(const std::string &_name, bool _setErrorMessage) override;

    PathPTPMovementState state(){ return mState; }
    //Direction direction(){ return mDirection; }

    MovementEngineData* currentData() override { return mCurrentData; }
    //std::vector<PathPTPMovementData> & pathMovementDatas(){ return mPathMovementDatas; }
    //PathPTPMovementData* getPathMovementData(const std::string &_name, bool _setErrorMessage = true);

    int currentPathIndex(){ return mVelocityGenerator.currentIndex(); }
    b2Vec2 currentPathPoint(){ return mVelocityGenerator.currentPoint(); }
    b2Vec2 currentPosition(){ return mVelocityGenerator.currentPosition(); }

    VGPathPTPMovement & velocityGenerator(){ return mVelocityGenerator; }
    //bool reachedNewStandingPoint(){ return mReachedNewStandingPoint; }
    //PointToPointHandler* pointToPointHandler(){ return mPtPHandler; }

    AngularVelocityPtP & angularVelocityGenerator(){ return mAngularVelocityGenerator; }
    PathPTPRotationState rotationState(){ return mRotationState; }
    RotationDirection rotationDirection(){ return mRotationDirection; }


private:
    PathPTPMovementState mState = PathPTPMovementState::STANDING;
    VGPathPTPMovement mVelocityGenerator;
    AngularVelocityPtP mAngularVelocityGenerator;


    PathPTPMovementData* mCurrentData = nullptr;                   // LINK
    std::vector<PathPTPMovementData>mPathMovementDatas;          // stored

    Direction mInputDirection = Direction::NONE;

    //--- rotation
    PathPTPRotationState mRotationState = PathPTPRotationState::STANDING_STILL;
    RotationDirection mRotationDirection = RotationDirection::CW;
    RotationDirection mRotationInputDirection = RotationDirection::NONE;


    //--- signals
    IntSignal mSigState;


    //---
    void updateTranslation(EngineUpdateParameters &eup);

    void update_stateSTANDING(EngineUpdateParameters &eup);
    void update_stateMOVING(EngineUpdateParameters &eup);
    void update_stateTURNING_AROUND(EngineUpdateParameters &eup);

    void updateRotation(EngineUpdateParameters &eup);


    void setState(PathPTPMovementState _state)
    {
        mState = _state;
        mSigState._setOnNextFrame(static_cast<int>(mState));
    }


};



//------------------------------------------------------------------------------------------


class PathPTPMovementFactory : public MovementEngineFactory
{
public:

    PathPTPMovementFactory(const std::string &_typeSignature, MovementEngineType _type) : MovementEngineFactory(_typeSignature, _type){}

    PathPTPMovementCfg* createMovementEngineCfg(const std::string &_name) override;
    PathPTPMovementEngine* createMovementEngine() override;


};


















}






#endif
