#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmCommonFunctions.h"
#include "jmStandardSprite.h"
#include "jmSystem.h"
#include "jmVectorShape.h"

#include "jpUtilities.h"
#include "jpPlayedApp.h"

#include "jpB2World.h"
#include "jpB2Body.h"
#include "jpEntity.h"
#include "jpEntityUtilities.h"
#include "jpSourceEntity.h"
#include "movements/jpPathPtPMovement.h"
#include "jpTransporter.h"



namespace jugimap{



//-----------------------------------------------------------------------------------------------


int GetTransporterStatusFromString(const std::string &_type)
{


    //if(_type=="OPERATING"){
    //    return TransporterStatus::OPERATING;

   // }else
    if(_type=="MOVING"){
        return TransporterStatus::MOVING;

    }else if(_type=="DIRECTION_FORWARD"){
        return TransporterStatus::DIRECTION_FORWARD;

    }else if(_type=="DIRECTION_BACKWARD"){
        return TransporterStatus::DIRECTION_BACKWARD;

    }else if(_type=="NO_PASSENGER"){
        return TransporterStatus::NO_PASSENGER;

    }else if(_type=="PASSENGER_PARTLY_ON"){
        return TransporterStatus::PASSENGER_PARTLY_ON;

    }else if(_type=="PASSENGER_FULLY_ON"){
        return TransporterStatus::PASSENGER_FULLY_ON;

    }

    return TransporterStatus::UNKNOWN;

}


std::vector<NamedValue>gTransporterStatusNamedValues
{
    { "MOVING", static_cast<int>(TransporterStatus::MOVING) },
    { "DIRECTION_FORWARD", static_cast<int>(TransporterStatus::DIRECTION_FORWARD) },
    { "DIRECTION_BACKWARD", static_cast<int>(TransporterStatus::DIRECTION_BACKWARD) },
    { "NO_PASSENGER", static_cast<int>(TransporterStatus::NO_PASSENGER) },
    { "PASSENGER_PARTLY_ON", static_cast<int>(TransporterStatus::PASSENGER_PARTLY_ON) },
    { "PASSENGER_FULLY_ON", static_cast<int>(TransporterStatus::PASSENGER_FULLY_ON) }
};


/*
int GetTransporterStatusFromString_signalSetter(const std::string &_type)
{


    if(_type=="OPERATING"){
        return TransporterStatus::OPERATING;
    }

    return TransporterStatus::UNKNOWN;


}
*/

bool TETransporterCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading task engine of type '" + factory->typeSignature() + "' and name '" + name + "'...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="parameters"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="waitingDurationAtPoints"){
                    waitingDurationAtPointsS = a.as_float(waitingDurationAtPointsS);

                }else if(attributeName=="startDelay"){
                    startDelayS = a.as_float(startDelayS);

                }else if(attributeName=="movingMode"){

                    std::string sMovingMode = a.as_string("");

                    if(sMovingMode=="AUTOMATIC"){
                        movingMode = MovingMode::AUTOMATIC;

                    }else if(sMovingMode=="AUTOMATIC_WITH_PASSENGER"){
                        movingMode = MovingMode::AUTOMATIC_WITH_PASSENGER;

                    }else if(sMovingMode=="CONTROLLED_BY_PASSENGER"){
                        movingMode = MovingMode::CONTROLLED_BY_PASSENGER;

                    }else{
                        dbgSystem.addMessage("Wrong attribute value '" + sMovingMode + "'");
                        return false;
                    }

                }else if(attributeName=="disablePassengerMovement"){
                    disablePassengerMovement = a.as_bool(false);

                //}else if(attributeName=="operatingOnStart"){
                //    operatingOnStart = a.as_bool(true);

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

            if(startDelayS == -1.0){
                startDelayS = waitingDurationAtPointsS;
            }

        }else if(nodeName=="inputCommands"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="moveForward"){
                    moveForward = a.as_string();

                }else if(attributeName=="moveBackward"){
                    moveBackward = a.as_string();

                }else if(attributeName=="moveLeft"){
                    moveLeft = a.as_string();

                }else if(attributeName=="moveRight"){
                    moveRight = a.as_string();

                }else if(attributeName=="moveUp"){
                    moveUp = a.as_string();

                }else if(attributeName=="moveDown"){
                    moveDown = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else{
            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;
        }
    }

    dbgSystem.removeLastMessage();
    return true;

}


bool TETransporterData::initConnections(PlayedScene *_scene, Entity *_entity)
{

    dbgSystem.addMessage("Init connections for  the data object of '" + cfg->name + "' ...");


    // triggers
    /*
    moveForward = ObtainTriggerFromPath(pathMovementCfg->cfg->moveForward, _scene);
    if(moveForward==nullptr){
        return false;
    }

    moveBackward = ObtainTriggerFromPath(pathMovementCfg->cfg->moveBackward, _scene);
    if(moveBackward==nullptr){
        return false;
    }
    */

    SignalParserManager* parser = app->signalParserManager();

    if(cfg->moveForward.empty()==false){
        //ObtainSignalFromPath(cfg->moveForward, _scene, _entity, nullptr, moveForward);
        parser->parseSignalAccessor(_scene, cfg->moveForward, moveForward, _entity);

        if(moveForward.isValid()==false){
            return false;
        }
    }


    if(cfg->moveBackward.empty()==false){
        //ObtainSignalFromPath(cfg->moveBackward, _scene, _entity, nullptr, moveBackward);
        parser->parseSignalAccessor(_scene, cfg->moveBackward, moveBackward, _entity);
        if(moveBackward.isValid()==false){
            return false;
        }
    }

    if(cfg->moveLeft.empty()==false){
        //ObtainSignalFromPath(cfg->moveLeft, _scene, _entity, nullptr, moveLeft);
        parser->parseSignalAccessor(_scene, cfg->moveLeft, moveLeft, _entity);
        if(moveLeft.isValid()==false){
            return false;
        }
    }

    if(cfg->moveRight.empty()==false){
        //ObtainSignalFromPath(cfg->moveRight, _scene, _entity, nullptr, moveRight);
        parser->parseSignalAccessor(_scene, cfg->moveRight, moveRight, _entity);
        if(moveRight.isValid()==false){
            return false;
        }
    }

    if(cfg->moveUp.empty()==false){
        //ObtainSignalFromPath(cfg->moveUp, _scene, _entity, nullptr, moveUp);
        parser->parseSignalAccessor(_scene, cfg->moveUp, moveUp, _entity);
        if(moveUp.isValid()==false){
            return false;
        }
    }

    if(cfg->moveDown.empty()==false){
        //ObtainSignalFromPath(cfg->moveDown, _scene, _entity, nullptr, moveDown);
        parser->parseSignalAccessor(_scene, cfg->moveDown, moveDown, _entity);
        if(moveDown.isValid()==false){
            return false;
        }
    }

    if(_entity->mapElement().parameters().exists("eTransporterDisabledOnStart")){
        disabled = _entity->mapElement().parameters().boolValue("eTransporterDisabledOnStart", false);

    }


    //---
    dbgSystem.removeLastMessage();
    return true;
}



//===================================================================================


void TETransporter::init(Entity *_entity)
{

    mParentEntity = _entity;

    mSignals.push_back(&mSigStatus);

}


bool TETransporter::start(TaskEngineData *_data)
{

    assert(_data->factory == mFactory);
    mCurrentData = static_cast<TETransporterData*>(_data);

    mStopDuration = mCurrentData->cfg->waitingDurationAtPointsS;
    mStopDuration = mParentEntity->sprite()->parameters().floatValue("eStopDuration", mStopDuration);
    mStartDelay = mCurrentData->cfg->startDelayS;
    mStartDelay = mParentEntity->sprite()->parameters().floatValue("eStartingDelay", mStartDelay);

    /*
    Signal *vs = mParentEntity->getSignal(SignalID::TRANSPORTER_STATUS);
    assert(vs);
    assert(vs->type()==ValueStateType::INT_BITS);
    mStatusSignal = static_cast<IntBitsSignal*>(vs);
    */

    //if(mCurrentData->cfg->operatingOnStart){
    //if(mCurrentData->disabled==false){
        //mSigStatus.setFlags_onNextFrame(TransporterStatus::OPERATING, true);
    //}

    //mEnabled = !mCurrentData->disabled;
    mSigEnabled.reset(!mCurrentData->disabled);

    mFirstMove = true;

    return true;
}


void TETransporter::createDataObjects(std::vector<TaskEngineCfg*> &_cfgs)
{

    mDataObjects.resize(_cfgs.size());
    for(unsigned int i=0; i<_cfgs.size(); i++){
        assert(_cfgs[i]->factory == mFactory);
        mDataObjects.at(i).factory = mFactory;
        mDataObjects.at(i).cfg = static_cast<TETransporterCfg*>(_cfgs[i]);
    }

}


bool TETransporter::initDataObjectsConnections(PlayedScene *_scene, Entity *_actor)
{

    for(TETransporterData& d : mDataObjects){
        if(d.initConnections(_scene, _actor)==false){
            return false;
        }
    }

    return true;

}


TaskEngineData* TETransporter::getData(const std::string &_name, bool _setErrorMessage)
{

    for(TETransporterData &d : mDataObjects){
        if(d.cfg->name == _name){
            return &d;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A 'PointToPointMovementTaskData' data configuration with name '" + _name + "' not found!");
    }

    return nullptr;

}


void TETransporter::update(TaskUpdateParameters _tup)
{

    if(mParentEntity->currentEngine()==nullptr || mParentEntity->currentEngine()->type()==MovementEngineType::NO_ENGINE_SET){
        return;
    }
    assert(mParentEntity->currentEngine()->type()==MovementEngineType::PATH_POINT_TO_POINT_MOVEMENT);
    PathPTPMovementEngine *engine = static_cast<PathPTPMovementEngine*>(mParentEntity->currentEngine());
    PathPTPMovementData *engData = static_cast<PathPTPMovementData*>(engine->currentData());
    Direction direction = engine->direction();
    VectorShape *path = engData->vectorShape;

    mStatusFlags = 0;
    //if(mSigStatus.active(TransporterStatus::OPERATING)){        // OPERATING bit is set from outside
    //    mStatusFlags |= TransporterStatus::OPERATING;
    //}

    //mEnabled = mSigEnabled.active();


    if(direction==Direction::FORWARD){
        mStatusFlags |= TransporterStatus::DIRECTION_FORWARD;

    }else if(direction==Direction::BACKWARD){
         mStatusFlags |= TransporterStatus::DIRECTION_BACKWARD;

    }else{
        assert(false);
    }


    //----
    if(mState==TransporterState::OFF){
        update_OFF(_tup);

    }else if(mState==TransporterState::ON){
        update_ON(_tup);

    }


    //----
    mSigStatus.setValue_onNextFrame(mStatusFlags);


    /*
    if(mActor->sourceEntity()->name()=="fancyPlatformA"){
        std::string text;
        if(mStatusSignal->active(TransporterStatus::NO_PASSENGER)){
            text += "NO_PASSENGER";
        }
        if(mStatusSignal->active(TransporterStatus::PASSENGER_PARTLY_ON)){
            text += "  PASSENGER_PARTLY_ON";
        }
        if(mStatusSignal->active(TransporterStatus::PASSENGER_FULLY_ON)){
            text += "  PASSENGER_FULLY_ON";
        }
        print(text);

    }
    */


    //----
    mStateEllapsedTimeS += _tup.timeStep;

}


void TETransporter::update_OFF(TaskUpdateParameters _tup)
{

    //if(mSigStatus.active(TransporterStatus::OPERATING)){
    if(mSigEnabled.active(true)){
        mState = TransporterState::ON;
        mStateEllapsedTimeS = 0.0f;
    }

}


void TETransporter::update_ON(TaskUpdateParameters _tup)
{


    if(mSigEnabled.active(true)==false){
        if(mSigStatus.notActive(TransporterStatus::MOVING)){
            mState = TransporterState::OFF;
            mStateEllapsedTimeS = 0.0f;
            return;
        }
    }


    PathPTPMovementEngine *engine = static_cast<PathPTPMovementEngine*>(mParentEntity->currentEngine());
    PathPTPMovementData *engData = static_cast<PathPTPMovementData*>(engine->currentData());
    Direction direction = engine->direction();


    //Entity *passenger = nullptr;


    if(engine->state()==PathPTPMovementState::STANDING){

        if(mCurrentData->cfg->movingMode==MovingMode::AUTOMATIC){

            float waitDurationS = (mFirstMove == false)? mStopDuration : mStartDelay;

            if(engine->stateEllapsedTimeS()>= waitDurationS){
                //mDoMoveInput = true;
                doComputerControlledMovement();
            }


        }else if(mCurrentData->cfg->movingMode==MovingMode::AUTOMATIC_WITH_PASSENGER){

            //--- re-enable movement of possible current passenger
            if(mCurrentData->cfg->disablePassengerMovement && mPassengers.empty()==false){
                for(Entity* e : mPassengers){
                    e->statusFlagsRef() &= ~EntityStatusFlags::DISABLED_USER_CONTROLLS;
                }
                mPassengers.clear();
            }

            bool standingStill = true;
            getPendingPassengers(EntityCategory::ShapeRole::MAIN_SHAPE, standingStill);

            if(mStatusFlags & TransporterStatus::PASSENGER_FULLY_ON){

                if(standingStill==false){
                    mStandStillTimer = 0.0f;
                }
                mStandStillTimer +=_tup.timeStep;
                if(mStandStillTimer >= mStartDelay){

                    mPassengers = mPendingPassengers;

                    if(mCurrentData->cfg->disablePassengerMovement){
                        for(Entity* e : mPassengers){
                            e->statusFlagsRef() |= EntityStatusFlags::DISABLED_USER_CONTROLLS;
                        }
                    }

                    //mStatusFlags &= ~TransporterStatus::PASSENGER_PARTLY_ON;
                    //mStatusFlags |= TransporterStatus::PASSENGER_FULLY_ON;

                    //mDoMoveInput = true;

                    doComputerControlledMovement();
                }

                //mStatusFlags &= ~TransporterStatus::PASSENGER_PARTLY_ON;
                //mStatusFlags |= TransporterStatus::PASSENGER_FULLY_ON;

            }else{
                mStandStillTimer = 0.0f;
            }



            //--- enable movement again
            //if(passenger){
            //    if(mCurrentData->cfg->disablePassengerMovement){
            //        passenger->statusFlagsRef() &= ~static_cast<int>(EntityStatusFlags::DISABLED_USER_CONTROLLS);
            //    }

            //}else{

            //    mStatusFlags |= TransporterStatus::NO_PASSENGER;
            //}


        }else if(mCurrentData->cfg->movingMode==MovingMode::CONTROLLED_BY_PASSENGER){

            bool standingStill = true;
            getPendingPassengers(EntityCategory::ShapeRole::MAIN_SHAPE, standingStill);
            if(mStatusFlags & TransporterStatus::PASSENGER_FULLY_ON){
                doManualMovemenent();
            }

        }


    }else if(engine->state()==PathPTPMovementState::MOVING){

        mFirstMove = false;
        //mDoMoveInput = false;
        mStandStillTimer = 0.0f;
        mStatusFlags |= TransporterStatus::MOVING;

        engData->sigMoveForwardObj.reset();
        engData->sigMoveBackwardObj.reset();
    }


    /*
    if(mDoMoveInput){

        doComputerControlledMovement();


    }else{
        engData->moveForwardObj.Reset();
        engData->moveBackwardObj.Reset();
    }
    */


    //--- rotation
    if(engine->angularVelocityGenerator().isActive()){

        if(engine->rotationState()==PathPTPRotationState::STANDING_STILL){
            engData->sigRotateCWObj.setValue(true);

        }else if(engine->rotationState()==PathPTPRotationState::MOVING){


        }
    }

}


void TETransporter::doComputerControlledMovement()
{


    PathPTPMovementEngine *engine = static_cast<PathPTPMovementEngine*>(mParentEntity->currentEngine());
    PathPTPMovementData *engData = static_cast<PathPTPMovementData*>(engine->currentData());
    Direction direction = engine->direction();
    VectorShape *path = engData->vectorShape;
    assert(path);


    //if(passenger && mCurrentData->cfg->disablePassengerMovement){
    //    passenger->statusFlagsRef() |= static_cast<int>(EntityStatusFlags::DISABLED_USER_CONTROLLS);
    //}

    if(engine->velocityGenerator().usingPathSegment()==false && path->isClosed()){

        if(direction==Direction::FORWARD){
            engData->sigMoveForwardObj.setValue(true);
         }else if(direction==Direction::BACKWARD){
            engData->sigMoveBackwardObj.setValue(true);
        }

    }else{

        int currentIndex = engine->currentPathIndex();

        if(direction==Direction::FORWARD){
            //if(currentIndex < path->pathPoints().size()-1){
            if(currentIndex < engine->velocityGenerator().endIndex()){
                engData->sigMoveForwardObj.setValue(true);
            }else{
                engData->sigMoveBackwardObj.setValue(true);
            }

        }else if(direction==Direction::BACKWARD){
            //if(currentIndex > 0 ){
            if(currentIndex >  engine->velocityGenerator().startIndex()){
                engData->sigMoveBackwardObj.setValue(true);
            }else{
                engData->sigMoveForwardObj.setValue(true);
            }
        }
    }

}


void TETransporter::doManualMovemenent()
{

    PathPTPMovementEngine *engine = static_cast<PathPTPMovementEngine*>(mParentEntity->currentEngine());
    PathPTPMovementData *engData = static_cast<PathPTPMovementData*>(engine->currentData());
    VectorShape *path = engData->vectorShape;
    assert(path);


    int currentIndex = engine->currentPathIndex();

    if(mCurrentData->moveForward.active()){

        if(currentIndex < engine->velocityGenerator().endIndex()){
            engData->sigMoveForwardObj.setValue(true);
        }

    }else if(mCurrentData->moveBackward.active()){

        if(currentIndex >  engine->velocityGenerator().startIndex()){
            engData->sigMoveBackwardObj.setValue(true);
        }
    }


}


void TETransporter::getPendingPassengers(EntityCategory::ShapeRole _shapeRole, bool &_standingStill)
{


    mPendingPassengers.clear();

    //Entity *passenger = nullptr;

    for(EntityContactSignal *ect : mParentEntity->contactTrigger().contactedEntitiesTriggers()){
        assert(ect->mTwoEntitiesContact.entityShapeA().entity == mParentEntity);
        if(ect->mTwoEntitiesContact.entityShapeB().category->shapeRole != _shapeRole){
            continue;
        }

        Entity *possiblePassenger = ect->mTwoEntitiesContact.entityShapeB().entity;
        if(possiblePassenger->lift() != mParentEntity){
            continue;
        }

        //if(passenger && possiblePassenger!=passenger){       // should not be possible to be the same
            //return nullptr;
        //    continue;
        //}


        // passenger must be whoole on transporter !
        b2Vec2 passengerPos = possiblePassenger->body()->B2Body()->GetPosition();
        float passengerLeft = possiblePassenger->body()->sourceBody()->centerToLeftDistance();
        float passengerRight = possiblePassenger->body()->sourceBody()->centerToRightDistance();
        b2Vec2 transporterPos = mParentEntity->body()->B2Body()->GetPosition();
        float transporterLeft = mParentEntity->body()->sourceBody()->centerToLeftDistance();
        float transporterRight = mParentEntity->body()->sourceBody()->centerToRightDistance();

        if(passengerPos.x-passengerLeft < transporterPos.x-transporterLeft || passengerPos.x+passengerRight > transporterPos.x+transporterRight){
            mStatusFlags |= TransporterStatus::PASSENGER_PARTLY_ON;
            //return nullptr;
            continue;

        }else{
            mStatusFlags |= TransporterStatus::PASSENGER_FULLY_ON;
        }


        // passenger must stand still on transporter !
        //bool standingStill = false;
        //if(possiblePassenger->movementEngineType()==BehaviorEngineType::GROUND_MOVEMENT){
        if(possiblePassenger->currentEngine()->type()==MovementEngineType::GROUND_MOVEMENT){
            //GroundMovementEngine *gme = static_cast<GroundMovementEngine*>(possiblePassenger->currentEngine());
            //if(!(gme->state()==GroundMovementState::STANDING || gme->state()==GroundMovementState::IDLE)){
            //    _standingStill = false;
            //}
            if(possiblePassenger->statusFlagsRef() & EntityStatusFlags::MOVING_ON_GROUND){
                _standingStill = false;
            }
        }
        /*
        if(standingStill==false){
            mStatusFlags |= TransporterStatus::PASSENGER_PARTLY_ON;
            mStatusFlags &= ~TransporterStatus::PASSENGER_FULLY_ON;
            continue;
        }
        */


        if(StdVector::contains(mPendingPassengers, possiblePassenger)==false){
            mPendingPassengers.push_back(possiblePassenger);
        }
    }


    if(mStatusFlags & TransporterStatus::PASSENGER_PARTLY_ON){
        mStatusFlags &= ~TransporterStatus::PASSENGER_FULLY_ON;
    }

    if((mStatusFlags & TransporterStatus::PASSENGER_PARTLY_ON)==0 && (mStatusFlags & TransporterStatus::PASSENGER_FULLY_ON)==0){
        mStatusFlags |= TransporterStatus::NO_PASSENGER;
    }


}


TETransporterData* TETransporter::getDataObject(const std::string &_name, bool _setErrorMessage)
{

    for(TETransporterData &d : mDataObjects){
        if(d.cfg->name == _name){
            return &d;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A data instance of 'PointToPointMovementTaskCfg' object with name '" + _name + "' not found!");
    }

    return nullptr;

}


void TETransporter::obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage)
{

    const std::string & taskCfgName = _psp.signalNamePartAt(1);
    const std::string & signalName = _psp.signalNamePartAt(2);

    TETransporterData *engineData = nullptr;

    if(taskCfgName.empty()==false){
        for(TETransporterData &d : mDataObjects){
            if(d.cfg->name == taskCfgName){

                engineData = &d;

                if(signalName=="MOVE_FORWARD"){
                    _psp.obtainValue(_signalQuery, d.moveForward.mSignal);

                }else if(signalName=="MOVE_BACKWARD"){
                    _psp.obtainValue(_signalQuery, d.moveBackward.mSignal);

                }else if(signalName=="MOVE_LEFT"){
                    _psp.obtainValue(_signalQuery, d.moveLeft.mSignal);

                }else if(signalName=="MOVE_RIGHT"){
                    _psp.obtainValue(_signalQuery, d.moveRight.mSignal);

                }else if(signalName=="MOVE_UP"){
                    _psp.obtainValue(_signalQuery, d.moveUp.mSignal);

                }else if(signalName=="MOVE_DOWN"){
                    _psp.obtainValue(_signalQuery, d.moveDown.mSignal);

                }
                break;
            }
        }

        if(_signalQuery.mSignal){
            return;
        }
    }

    if(signalName=="STATUS"){
        _psp.obtainValue(_signalQuery, &mSigStatus, &gTransporterStatusNamedValues);

        /*
        _signalQuery.mSignal = &mSigStatus;
        _signalQuery.mIntValue = GetTransporterStatusFromString(_psp.signalValue);

        if(_signalQuery.mIntValue == TransporterStatus::UNKNOWN){
            dbgSystem.addMessage("Unknown signal flag value '" + _psp.signalValue +" ' !");
            return;
        }
        */

    }else if(signalName=="ENABLED"){
        //_signalQuery.mSignal = &mSigEnabled;
        _psp.obtainValue(_signalQuery, &mSigEnabled);

    }


    if(_signalQuery.mSignal==nullptr && _setErrorMessage){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown!");
    }

}


void TETransporter::obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage )
{


    const std::string & taskCfgName = _psp.signalNamePartAt(1);
    const std::string & signalName = _psp.signalNamePartAt(2);

    TETransporterData *engineData = nullptr;

    if(taskCfgName.empty()==false){
        for(TETransporterData &d : mDataObjects){
            if(d.cfg->name == taskCfgName){
                engineData = &d;
                break;
            }
        }
        if(_signalSetter.mSignal){
            return;
        }
    }

    if(signalName=="ENABLED"){
        //_signalSetter.mSignal = &mSigEnabled;

        //if(_psp.getBoolValue(_signalSetter.mBoolValue)==false){
        //    return;
        //}
        _psp.obtainValue(_signalSetter, &mSigEnabled);
    }


    if(_signalSetter.mSignal==nullptr && _setErrorMessage){
        dbgSystem.addMessage("Get signal '" + _psp.signalFullName() + "' error! The signal is unknown or not available for setting!");
    }

}




//=============================================================================================================


TaskEngineCfg* TransporterFactory::createTaskEngineCfg(const std::string &_name)
{
    return new TETransporterCfg(this, _name);
}


TaskEngine* TransporterFactory::createTaskEngine()
{
    return new TETransporter(this);
}



}
