#ifndef PLAYER__MOVEMENT_BASE__H
#define PLAYER__MOVEMENT_BASE__H

#include <memory>
#include <string>
#include <vector>

#include "jmSignal.h"

#include "jpEntityGlobal.h"



namespace pugi{
class xml_node;
}

namespace jugimap{

class PlayedScene;
class AnimationPlayer;
class AnimationInstance;
class Entity;
class MovementEngineFactory;
class MovementEngine;
struct SignalQuery;
struct SignalSetter;
class Signal;
class ParsedSignalPath;
//struct EntitySignalStrings;

struct SlidingGroundCfg;
struct ConveyorGroundCfg;


struct CustomSensorCfg
{
    std::string customSensorCategory;
    int customSensorExtraShapeID = 1;
    int extraShapeActiveOnAnimationKeyData = -1;        // -1 means the fixture is permanently active

};



struct MovementEngineCfg
{

    MovementEngineCfg(MovementEngineFactory *_factory, const std::string &_name) : factory(_factory), name(_name){}
    virtual ~MovementEngineCfg(){}

    virtual bool initCfg(const pugi::xml_node &_node) = 0;
    virtual void collectCustomSensors(std::vector<CustomSensorCfg> &_customSensors){}


    MovementEngineFactory * factory = nullptr;     // LINK
    std::string name;


};



struct JumpMovementCfgBase : public MovementEngineCfg
{

    JumpMovementCfgBase(MovementEngineFactory *_factory, const std::string &_name) : MovementEngineCfg(_factory, _name){}

    bool addCeilingSensor = false;

};

//-------------------------------------------------------------------------


struct MovementEngineData
{

    virtual ~MovementEngineData(){}

    MovementEngineFactory * factory = nullptr;     // LINK


};


class MovementEngine
{
public:

    MovementEngine(MovementEngineFactory * _factory) : mFactory(_factory){}

    virtual ~MovementEngine(){}


    MovementEngineFactory* factory(){ return mFactory; }
    MovementEngineType type();

    virtual bool init(Entity* _entity){ mParentEntity = _entity; return true; }
    virtual bool start(MovementEngineData *_data) = 0;
    virtual b2Vec2 update(EngineUpdateParameters &eup) = 0;

    virtual void createDataObjects(std::vector<MovementEngineCfg*>& _cfgs) = 0;
    virtual bool initDataObjectsConnections(PlayedScene *_scene, Entity *_actor) = 0;
    virtual void stop() = 0;

    virtual MovementEngineData* getMovementEngineData(const std::string &_name, bool _setErrorMessage) = 0;
    virtual MovementEngineData* currentData() = 0;

    virtual void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true);
    virtual void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage=true);



    Direction direction(){ return mDirection; }
    //IntSignal directionSignal(){ return mDirectionSignal; }

    b2Vec2 velocity(){ return mVelocity; }
    b2Vec2 forwardVelocity(){ return mForwardVelocity; }
    float angularVelocity(){ return mAngularVelocity; }

    bool isMoving(){ return (mVelocity.x != 0.0f || mVelocity.y != 0.0f); }

    void setExtraVelocity(b2Vec2 _extraVelocity){ mExtraVelocity = _extraVelocity; }
    b2Vec2 extraVelocity(){ return mExtraVelocity; }

    float stateEllapsedTimeS(){ return mStateEllapsedTimeS; }
    void setStateEllapsedTimeS(float _value){ mStateEllapsedTimeS = _value; }

    void resetAnimationPlayer();
    void updateAnimationPlayer();
    bool isCurrentAnimationStalled();

    //void preUpdateSignals(){ Signal::preUpdateSignals(mSignals); }
    //void postUpdateSignals(){ Signal::postUpdateSignals(mSignals); }
    void resetSignals(){ Signal::resetSignals(mSignals); }

    std::vector<Signal*> &signals(){ return mSignals; }

    //void setUserControllsEnabled(bool _enabled){ mUserControllsEnabled = _enabled; }

    bool fixedPositionSet(){ return mFixedPositionSet; }
    b2Vec2 fixedPosition(){ return mFixedPosition; }

    //IntBitsSignal & inputCommandsSignal(){ return mSigInputCommands; }



protected:
    MovementEngineFactory * mFactory = nullptr;     // LINK
    Entity *mParentEntity = nullptr;                     // LINK

    //BehaviorEngineType mType = BehaviorEngineType::UNKNOWN;
    Direction mDirection = Direction::NONE;
    IntSignal mDirectionSignal;
    //Direction mBodyOrientation = Direction::NONE;
    //IntBitsSignal mSigInputCommands;

    float mStateEllapsedTimeS = 0.0f;
    b2Vec2 mVelocity{0.0f, 0.0f};
    b2Vec2 mForwardVelocity{0.0f, 0.0f};

    float mAngularVelocity = 0.0f;
    b2Vec2 mExtraVelocity{0.0f, 0.0f};

    AnimationPlayer *mAnimationPlayer = nullptr;                // LINK
    AnimationInstance *mActiveAnimationInstance = nullptr;      // LINK

    bool mPrestarted = false;

    b2Vec2 mFixedPosition{0.0f, 0.0f};
    bool mFixedPositionSet = false;

    std::vector<Signal*>mSignals;                           // LINKS



};



//=========================================================================================


struct GroundMovementDataBase : public MovementEngineData
{

    float yMinNormalForMaxSlope = 0.707f;

    //------ SIGNALS
    SignalQuery moveLeft;
    SignalQuery moveRight;
    BoolSignal sigMoveLeftObj;
    BoolSignal sigMoveRightObj;
    BoolSignal sigDisabled;

};


class GroundMovementEngineBase : public MovementEngine
{
public:


    GroundMovementEngineBase(MovementEngineFactory * _factory);


    void setGroundSliding(SlidingGroundCfg * _cfg){ mSlidingGroundCfg = _cfg; }
    void setGroundConveyor(ConveyorGroundCfg *_cfg){ mConveyorGroundCfg = _cfg; }
    b2Vec2 groundNormal(){ return mGroundNormal;}

protected:
    b2Vec2 mGroundNormal{0.0f, 0.0f};

    SlidingGroundCfg* mSlidingGroundCfg = nullptr;
    ConveyorGroundCfg* mConveyorGroundCfg = nullptr;

};


//=========================================================================================


class DummyMovementEngine : public MovementEngine
{
public:
    DummyMovementEngine(MovementEngineFactory * _factory) : MovementEngine(_factory){}

    bool init(Entity* ) override { return false; }
    bool start(MovementEngineData *) override { return false; }
    b2Vec2 update(EngineUpdateParameters &) override{ return b2Vec2(0.0f, 0.0f); }
    void createDataObjects(std::vector<MovementEngineCfg*>& ) override {}
    bool initDataObjectsConnections(PlayedScene *, Entity *) override { return false; }
    void stop() override {}
    MovementEngineData* getMovementEngineData(const std::string &, bool ) override { return nullptr; }
    MovementEngineData* currentData() override { return nullptr; }


};



//------------------------------------------------------------------------------------

class MovementEngineFactory
{
public:

    virtual ~MovementEngineFactory(){}


    MovementEngineFactory(const std::string &_typeSignature, MovementEngineType _type) : mTypeSignature(_typeSignature), mType(_type){}

    const std::string & typeSignature(){ return mTypeSignature;}
    MovementEngineType type(){ return mType; }


    virtual MovementEngineCfg* createMovementEngineCfg(const std::string &_name) = 0;
    virtual MovementEngine* createMovementEngine() = 0;

    //virtual void obtainCustomSignal_signalQuery(SignalQuery &_signalQuery, const std::string &_signalName, const std::string &_signalValue){}
    //virtual void obtainCustomSignal_signalSetter(SignalSetter &_signalSetter, const std::string &_signalName, const std::string &_signalValue){}


protected:
    std::string mTypeSignature;
    MovementEngineType mType = MovementEngineType::UNKNOWN;

};



class DummyMovementEngineFactory : public MovementEngineFactory
{
public:

    DummyMovementEngineFactory(const std::string &_typeSignature, MovementEngineType _type) : MovementEngineFactory(_typeSignature, _type){}

    MovementEngineCfg* createMovementEngineCfg(const std::string &) override { return nullptr;}
    MovementEngine* createMovementEngine() override { return nullptr;}
};


//------------------------------------------------------------------------------------


class MovementEnginesManager
{
public:

    MovementEnginesManager();
    ~MovementEnginesManager();

    void addMovementEngineCreator(MovementEngineFactory *_engineCreator);

    MovementEngineCfg* addMovementEngineCfg(const std::string &_typeSignature, const std::string &_name, bool _setErrorMessage=true);
    MovementEngine* createMovementEngine(const std::string &_typeSignature, bool _setErrorMessage=true);
    MovementEngineCfg* getMovementEngineCfg(const std::string &_name, bool _setErrorMessage=true);
    DummyMovementEngine* getDummyMovementEngine(){ return mDummyMovementEngine; }

private:
    std::vector<MovementEngineFactory*>mEngineCreators;
    std::vector<MovementEngineCfg*>mMovementEngineCfgs;
    DummyMovementEngineFactory * mDummyEngineFactory = nullptr;
    DummyMovementEngine *mDummyMovementEngine = nullptr;
};


//-------------------------------------------------------------------------------------------


struct PathPointToPointSegment
{
    int index1 = -1;
    int index2 = -1;
    int index3 = -1;
    float distance12 = 0.0f;
    float distance23 = 0.0f;

    //b2Vec2 p1{0.0f, 0.0f};
    //b2Vec2 p2{0.0f, 0.0f};
    //b2Vec2 pNext{0.0f, 0.0f};

    //bool p2Exists = true;
    //bool pNextExists = true;

};





}






#endif
