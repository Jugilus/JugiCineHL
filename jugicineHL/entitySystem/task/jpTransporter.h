#ifndef PLAYER__ENTITY_SYSTEM__TASK__TRANSPORTER__H
#define PLAYER__ENTITY_SYSTEM__TASK__TRANSPORTER__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jmTimer.h"
#include "jmSignal.h"
#include "jpQueries.h"
#include "jpGlobal.h"
#include "jpTaskBase.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class PlayedScene;
class Animation;
class BoolSignal;
class VectorShape;
class Entity;
//class PathPTPMovementEngine;
struct EntityCategory;






struct TransporterStatus
{
    static const int UNKNOWN = 0;
    static const int OPERATING =             1 << 0;
    static const int MOVING =                1 << 1;
    static const int DIRECTION_FORWARD =     1 << 2;
    static const int DIRECTION_BACKWARD =    1 << 3;
    static const int NO_PASSENGER =          1 << 4;
    static const int PASSENGER_PARTLY_ON =   1 << 5;
    static const int PASSENGER_FULLY_ON =    1 << 6;

};

int GetTransporterStatusFromString(const std::string &_type);

int GetTransporterStatusFromString_signalSetter(const std::string &_type);



//------------------------------------------------------------------------------------



enum class RunningMode
{
    OFF,
    RUNNING,

};


enum class RepeatMode
{
    MOVE_TO_END_POINT,
    MOVE_TO_END_POINT_AND_BACK,
    REPEAT_FOREVER,

};


enum class MovingMode
{
    AUTOMATIC,
    AUTOMATIC_WITH_PASSENGER,
    CONTROLLED_BY_PASSENGER

};



struct TETransporterCfg : public TaskEngineCfg
{

    TETransporterCfg(TaskEngineFactory *_factory, const std::string &_name) : TaskEngineCfg(_factory, _name){}

    bool initCfg(const pugi::xml_node &_node);


    float waitingDurationAtPointsS = 2.0f;
    float startDelayS = -1.0f;

    //bool operatingOnStart = true;
    //RepeatMode repeatMode = RepeatMode::REPEAT_FOREVER;

    //std::string repeatMode;
    //std::string transporterMode;
    MovingMode movingMode = MovingMode::AUTOMATIC;

    std::string activatingActorsOnPlatformByCategory;

    //----
    std::string moveForward;
    std::string moveBackward;
    std::string moveLeft;
    std::string moveRight;
    std::string moveUp;
    std::string moveDown;



    bool disablePassengerMovement = false;


};


struct TETransporterData : public TaskEngineData
{

    ~TETransporterData() override {}

    bool initConnections(PlayedScene *_scene, Entity *_entity);

    TETransporterCfg *cfg = nullptr;     // LINK

    bool disabled = false;
    float startingWaitingDurationS = -1.0f;

    //BoolValueState *activatingTrigger = nullptr;                   // LINK
    //TriggerResult activatingTriggerStyle = TriggerResult::ACTIVE;
    //TransporterMode startMode = TransporterMode::AUTOMATIC;


    SignalQuery moveForward;
    SignalQuery moveBackward;
    SignalQuery moveLeft;
    SignalQuery moveRight;
    SignalQuery moveUp;
    SignalQuery moveDown;


    //std::vector<EntityCategory*>activatingActors;

    // parameters from editor
    //VectorShape * path = nullptr;                   // LINK
    //int currentIndex = 0;
    //Direction direction = Direction::FORWARD;


};



//------------------------------------------------------------------------------------------



enum class TransporterState
{

    OFF,
    ON

};




class TETransporter : public TaskEngine
{
public:

    TETransporter(TaskEngineFactory *_deciderFactory) : TaskEngine(_deciderFactory) {}

    //void init(PointToPointMovementTaskData *_data, Entity *_actor);

    void init(Entity *_entity) override;
    bool start(TaskEngineData *_data) override;

    void createDataObjects(std::vector<TaskEngineCfg*> &_cfgs) override;
    bool initDataObjectsConnections(PlayedScene *_scene, Entity *_actor) override;

    TaskEngineData* getData(const std::string &_name, bool _setErrorMessage) override;
    TaskEngineData* currentData() override { return mCurrentData;}


    void update(TaskUpdateParameters _tup) override;

    //void setPathShape(VectorShape *_path){ mPath = _path; }
    //VectorShape *pathShape(){ return mPath; }

    TETransporterData * currentDataObject(){ return mCurrentData; }
    //std::vector<PointToPointMovementTaskData>& dataObjects(){ return mDataObjects; }
    TETransporterData* getDataObject(const std::string &_name, bool _setErrorMessage = true);

    void obtainSignal_signalQuery(SignalQuery &_signalQuery, const std::string &_dataName, const std::string &_signalName, const std::string &_signalValue, bool _setErrorMessage=true) override;
    void obtainSignal_signalSetter(SignalSetter &_signalSetter, const std::string &_dataName, const std::string &_signalName, const std::string &_signalValue, bool _setErrorMessage = true) override;


private:
    TETransporterData *mCurrentData = nullptr;
    std::vector<TETransporterData>mDataObjects;          // stored

    //Entity *mParentEntity = nullptr;                    // LINK

    //Entity *mPassenger = nullptr;
    std::vector<Entity*>mPendingPassengers;
    std::vector<Entity*>mPassengers;


   // bool mDoMoveInput = false;

    float mStartDelay = 0.0f;
    float mStopDuration = 0.0f;
    bool mFirstMove = false;
    float mStandStillTimer = 0.0f;

    //VectorShape *mPath = nullptr;
    //PointToPointPoints mPoints;
    //int mCurrentIndex = 0;

    //b2Vec2 mPstart;
    //b2Vec2 mPend;

    IntBitsSignal mSigStatus;

    int mStatusFlags = 0;


    TransporterState mState = TransporterState::OFF;

    //void setNextPathSegment();


    void update_OFF(TaskUpdateParameters _tup);
    void update_ON(TaskUpdateParameters _tup);


    void doComputerControlledMovement();
    void doManualMovemenent();

    void getPendingPassengers(EntityCategory::ShapeRole _shapeRole, bool &_standingStill);



};


//------------------------------------------------------------------------------------------


class TransporterFactory : public TaskEngineFactory
{
public:

    TransporterFactory(const std::string &_typeSignature, TaskType _type) : TaskEngineFactory(_typeSignature, _type){}

    TaskEngineCfg* createTaskEngineCfg(const std::string &_name) override;
    TaskEngine * createTaskEngine() override;


};








}






#endif
