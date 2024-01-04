#ifndef PLAYER__MOVEMENT__PATH_MOVEMENT__H
#define PLAYER__MOVEMENT__PATH_MOVEMENT__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jmTimer.h"
#include "jpGlobal.h"
#include "jpQueries.h"

#include "jpSpeedGenerators.h"
#include "jpMovementBase.h"
#include "jpEntityCommon.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

//class EPathway;
class Entity;
class PlayedScene;
class AnimationInstance;
class AnimationPlayer;
class VectorShape;
class PathMovementEngine;






enum class PathMovementState
{

    UNKNOWN =           0,
    STANDING =          1,
    IDLE =              2,
    TURNING_AROUND =    3,
    MOVING =            4,

};

PathMovementState GetPathMovementStateFromString(const std::string &state);


extern std::vector<std::pair<std::string, int>>gPathMovementStateNamedValues;

//------------------------------------------------------------------------------------



struct PathMovementCfg : public MovementEngineCfg
{

    PathMovementCfg(MovementEngineFactory *_factory, const std::string &_name) : MovementEngineCfg(_factory, _name){}
    bool initCfg(const pugi::xml_node &_node);

    float maxSpeed = 5.0f;              // meters
    float accDistance = 1.0f;           // meters,seconds

    std::vector<std::string>pathwayCategories;
    float idleAnimationDelay = 7.0;

    std::string sigMoveForward;
    std::string sigMoveBackward;

    std::string aniStandingForward;
    std::string aniStandingBackward;
    std::string aniIdleForward;
    std::string aniIdleBackward;
    std::string aniTurningToForward;
    std::string aniTurningToBackward;
    std::string aniMovingForward;
    std::string aniMovingBackward;


};


struct PathMovementData : public MovementEngineData
{

    ~PathMovementData();

    bool initConnections(PlayedScene *_scene, Entity *_actor);

    PathMovementCfg *cfg = nullptr;     // LINK

    //---
    VectorShape *vectorShape = nullptr;         // LINK

    // ---
    PathwayOrientation pathwayOrientation = PathwayOrientation::VERTICAL;
    VerticalPathwayDirection verticalPathwayDirection = VerticalPathwayDirection::UP;
    HorizontalPathwayDirection horizontalPathwayDirection = HorizontalPathwayDirection::RIGHT;
    Margin centeringMarginM;
    CenteringStyle centeringStyle = CenteringStyle::NONE;
    b2Vec2 actorPathwayContactOffsetM{0.0f, 0.0f};
    Entity * pathway = nullptr;
    std::vector<int>exitPathPointsIndexes;
    std::vector<EntityCategory*>pathwayCategories;


    //--- signals
    SignalQuery moveForward;                        // link to signal object bellow OR input command for the player controlled actors
    SignalQuery moveBackward;
    BoolSignal sigMoveForwardObj;                 // signal object for computer controlled actors
    BoolSignal sigMoveBackwardObj;                // signal object for computer controlled actors

    BoolSignal sigDisabled;



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


//------------------------------------------------------------------------------------------


class PathwayAccessedSignal : public UpdatedBoolSignal
{
public:

    PathwayAccessedSignal();

    void set(Entity *_actor, PathMovementData *_pathMovementData, PathMovementEngine* _pathMovementEngine);
    PathwayAccessedSignal(Entity *_actor, PathMovementData *_pathMovementData, PathMovementEngine* _pathMovementEngine);

    void update() override;

    void updateNEW(Entity * _Actor, PathMovementData *_pathMovementData, PathMovementEngine *_PathMovementEngine);

private:
    Entity * mActor = nullptr;
    PathMovementData *mPathMovementData = nullptr;
    PathMovementEngine *mPathMovementEngine = nullptr;

};


//------------------------------------------------------------------------------------------


class PathwayLeftSignal : public UpdatedBoolSignal
{
public:
    PathwayLeftSignal();
    PathwayLeftSignal(Entity *_actor, PathMovementData *_pathMovementData, PathMovementEngine* _pathwayEngine, MovementEngineData* _targetEngineData);

     void set(Entity *_actor, PathMovementData *_pathMovementData, PathMovementEngine* _pathMovementEngine);

    void update() override;

private:
    Entity * mActor = nullptr;
    PathMovementData *mPathMovementData = nullptr;
    PathMovementEngine *mPathwayEngine = nullptr;
    MovementEngineData *mTargetEngineData = nullptr;

};


//------------------------------------------------------------------------------------------


class PathwayPositionAtEndSignal : public UpdatedBoolSignal
{
public:
    PathwayPositionAtEndSignal();
    void set(Entity *_actor, PathMovementData *_pathMovementData, PathMovementEngine* _pathMovementEngine);

    void update() override;

private:
    Entity * mActor = nullptr;
    PathMovementData *mPathMovementData = nullptr;
    PathMovementEngine *mPathwayEngine = nullptr;

};


//------------------------------------------------------------------------------------------


class PathwayPositionAtExitPointSignal : public UpdatedBoolSignal
{
public:
    PathwayPositionAtExitPointSignal();
    void set(Entity *_actor, PathMovementData *_pathMovementData, PathMovementEngine* _pathMovementEngine);

    void update() override;

private:
    Entity * mActor = nullptr;
    PathMovementData *mPathMovementData = nullptr;
    PathMovementEngine *mPathwayEngine = nullptr;

};



//------------------------------------------------------------------------------------------


class VGPathMovement
{
public:

    bool init(PathMovementData &_data, Direction _direction, b2Vec2 _actorContactPosM);

    bool initMovement(PathMovementData &_data, Direction _direction, b2Vec2 _actorContactPosM);
    b2Vec2 updateVelocity(float _timeStep, Direction _inputDirection, int _blockedDirections, b2Vec2 _actorContactPosM);
    void stop();

    b2Vec2 unitVelocity(){ return  mUnitVelocity; }
    b2Vec2 velocity(){ return  mVelocity; }
    float speed(){ return mSpeedGenerator.speed();}
    //float passedDistance(){ return mSpeedGenerator.passedDistance(); }

    //bool isMovementAllowed(){ return mEndIndex != -1; }
    //bool isMovementAllowed(){ return (mSegment.index2 != -1 || mContinousCircularMovement==true) ; }
    int currentIndex(){ return mCurrentIndex; }
    b2Vec2 currentPoint();
    b2Vec2 currentPositionM(){ return mCurrentPosM; }
    Vec2f currentPositionP(){ return mCurrentPosP; }
    float currentDistanceP(){ return mCurrentDistanceP; }

    //PointToPointSpeedGenerator::State state(){ return mSpeedGenerator.state(); }

    //PathPointToPointSegment & segment(){return  mSegment;}
    bool isPathwayStart();
    bool isPathwayEnd();

    bool obtainCurrentPositionOnPath(Vec2f _actorContactPosP);

private:
    Direction mDirection = Direction::NONE;
    ConstSpeedGenerator mSpeedGenerator;

    //float mDistance = 0.0f;

    //---
    VectorShape *mPath = nullptr;       //LINK
    PathwayOrientation mPathwayOrientation = PathwayOrientation::VERTICAL;

    //PathPointToPointSegment mSegment;
    int mStep = 1;

    int mCurrentIndex = 0;

    b2Vec2 mCurrentPosM{0.0f, 0.0f};
    Vec2f mCurrentPosP{0.0f, 0.0f};
    float mCurrentDistanceM = 0.0f;
    float mCurrentDistanceP = 0.0f;


    b2Vec2 mUnitVelocity{0.0f, 0.0f};
    b2Vec2 mVelocity{0.0f, 0.0f};

    void obtainUnitVelocity(float newDistanceP);
    //void obtainUnitVelocity_Slow(float passedDistanceP);
    //void obtainUnitVelocity_OpenPath_Quick(float passedDistanceP);

    //int obtainNextIndex(int _startIndex, float &_distance);




    //int obtainCurrentIndex();


};


class PathMovementEngine : public MovementEngine
{
public:

    PathMovementEngine(MovementEngineFactory * _factory);

    bool initDataObjectsConnections(PlayedScene *_scene, Entity *_actor) override;
    void createDataObjects(std::vector<MovementEngineCfg*> &_cfgs) override;
    void collectSignalsForLUT(SignalStorage &_storage) override;

    bool init(Entity *_entity) override;
    //void init(PathMovementData *_data, Direction _direction, b2Vec2 _actorPosM);
    bool start(MovementEngineData *_data) override;

    bool preStartIfPathwayAccessed(PathMovementData *_data, Direction _direction, AnimationPlayer *_animationPlayer);
    b2Vec2 update(EngineUpdateParameters &eup) override;
    void stop() override;

    MovementEngineData* getMovementEngineData(const std::string &_name, bool _setErrorMessage) override;
    void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true) override;
    void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage = true) override;


    PathMovementState state(){ return mState; }

    MovementEngineData* currentData() override { return mCurrentData; }
    MovementEngineCfg* currentCfg() override { return mCurrentData->cfg; }
   // std::vector<PathMovementData> & pathMovementDatas(){ return mPathMovementDatas; }
    //PathMovementData* getPathMovementData(const std::string &_name, bool _setErrorMessage = true);

    //int currentPathIndex(){ return mVelocityGenerator.currentIndex(); }
    //b2Vec2 currentPathPoint(){ return mVelocityGenerator.currentPoint(); };
    b2Vec2 currentPositionM(){ return mVelocityGenerator.currentPositionM(); }

    bool isPathwayFinished(bool _activeGroundMoveCommand);
    //bool isPathwayAccessed(PathMovementData *_data, b2Vec2 _actorPosM);

    VGPathMovement & velocityGenerator(){ return mVelocityGenerator; }



private:
    PathMovementState mState = PathMovementState::STANDING;
    VGPathMovement mVelocityGenerator;
    Direction mInputDirection;
    PathMovementData* mCurrentData = nullptr;                   // LINK

    std::vector<PathMovementData>mPathMovementDatas;          // stored

    //----
    IntSignal mSigState;
    PathwayAccessedSignal mSigPathwayAccessed;
    //PathwayLeftSignal mSigPathwayLeft;
    PathwayPositionAtEndSignal mSigPathwayPositionAtEnd;
    PathwayPositionAtExitPointSignal mSigPathwayPositionAtExitPoint;


    void setState(PathMovementState _state)
    {
        mState = _state;
        mSigState.setValue_onNextFrame(static_cast<int>(mState));
    }


    void update_stateSTANDING(EngineUpdateParameters &eup);
    void update_stateMOVING(EngineUpdateParameters &eup);
    void update_stateTURNING_AROUND(EngineUpdateParameters &eup);


};



//------------------------------------------------------------------------------------------


class PathMovementFactory : public MovementEngineFactory
{
public:

    PathMovementFactory(const std::string &_typeSignature, MovementEngineType _type) : MovementEngineFactory(_typeSignature, _type){}

    PathMovementCfg* createMovementEngineCfg(const std::string &_name) override;
    PathMovementEngine* createMovementEngine() override;

    //void obtainCustomSignal_signalQuery(SignalQuery &_signalQuery, const std::string &_signalName, const std::string &_signalValue) override;



};














}






#endif
