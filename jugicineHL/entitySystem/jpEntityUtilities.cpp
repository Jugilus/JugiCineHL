#include <assert.h>
#include <ncine/FileSystem.h>
#include <ncine/Texture.h>
#include "box2d/box2d.h"

#include "jmSystem.h"
#include "jmVectorShape.h"
#include "jmSprite.h"
#include "jmStandardSprite.h"
#include "jmSourceItem.h"
#include "jmCommonFunctions.h"

#include "jpPlayedApp.h"
#include "jpInput.h"
#include "jpUtilities.h"

#include "jpEntityCommon.h"
#include "jpB2Body.h"
#include "jpB2Fixture.h"
#include "jpB2World.h"
#include "jpEntityLogicState.h"
#include "jpSourceEntity.h"
#include "jpEntity.h"
#include "jpEntitySystem.h"
#include "task/jpTaskBase.h"
#include "movements/jpMovementBase.h"
#include "jpQueries.h"

#include "jpEntityUtilities.h"




namespace jugimap{



/*
bool _ParseSignalAccessorStyle(const std::string & syle, SignalAccessor &_signalAccessor)
{


    if(_signalAccessor.mType==SignalAccessor::Type::QUERY){

        SignalQuery &_signalQuerry = static_cast<SignalQuery&>(_signalAccessor);

        if(syle==""){
            _signalQuerry.mStyle = SignalQuery::Style::ACTIVE;

        }else{

            //_signalQuerry.mStyle = _ParseTriggerStyle(syle);


            if(syle=="START"){
                _signalQuerry.mStyle = SignalQuery::Style::ACTIVE_STARTED;

             }else if(syle=="ACTIVE"){
                _signalQuerry.mStyle = SignalQuery::Style::ACTIVE;

            }else if(syle=="END"){
               _signalQuerry.mStyle = SignalQuery::Style::ACTIVE_ENDED;

            }else if(syle=="NOT_ACTIVE"){
               _signalQuerry.mStyle = SignalQuery::Style::NOT_ACTIVE;

            }else{
                dbgSystem.addMessage("Error parsing signal query style from string'" + syle +"' !");
                return false;
            }
        }


    }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){

        SignalSetter &_signalSetter = static_cast<SignalSetter&>(_signalAccessor);

        if(syle==""){
            _signalSetter.mStyle = SignalSetter::Style::SET;

        }else{

            if(syle=="SET"){
                _signalSetter.mStyle = SignalSetter::Style::SET;

            }else if(syle=="UNSET"){
                _signalSetter.mStyle = SignalSetter::Style::UNSET;

            }else{
                dbgSystem.addMessage("Error parsing signal setter style from string'" + syle +"' !");
                return false;

            }
        }

    }

    return true;
}

*/

/*

void _ParseEntitySignal_SignalQuery(Entity *_entity, BehaviorState *_state, const std::string &_path, SignalQuery &_signalQuery)
{

    EntitySystem *entitySystem = _entity->parentEntitySystem();


    if(_path=="CONTACT" || _path=="CONTACT:ANY"){

        _signalQuery.mSignal = &_entity->contactTrigger();
        return;


    }else if(_path.substr(0,8)=="CONTACT:"){


        FilteredContactTriggersGroup &g = entitySystem->filteredContactTriggersGroup();
        FilteredContactTrigger *trigger = g.getNewFilteredContactTrigger(&_entity->contactTrigger());

        std::string contactedEntity = _path.substr(8);

        if(contactedEntity.substr(0,9)=="CATEGORY:"){


            EntityCategoriesGroup & ecs = entitySystem->entityCategoriesGroup();


            unsigned int bits = 0;

            std::string sContactedEntityCategories = contactedEntity.substr(9);
            std::vector<std::string> contactedEntityCategories = StdString::splitString(sContactedEntityCategories, "|");
            for(std::string & csName : contactedEntityCategories){
                EntityCategory *ec = ecs.getEntityCategory(csName);
                if(ec==nullptr){
                    return;
                }
                bits |= ec->contactBits;
            }


            trigger->setCategoriesBits(bits);

            _signalQuery.mSignal = trigger;
            return;


        }else{

            std::vector<std::string>parts = StdString::splitString(contactedEntity, ":");

            if(parts.size()==1){

                const std::string & entityNames = parts[0];
                std::vector<std::string>vEntitiyNames = StdString::splitString(entityNames, ",");

                for(const std::string & s : vEntitiyNames){
                    Entity *e = entitySystem->getActor(s);
                    if(e==nullptr){
                        return;
                    }
                    trigger->addSourceEntity(e->sourceEntity());
                }

                _signalQuery.mSignal = trigger;
                return;


            }else if(parts.size()==2){

                //const std::string & entityName = parts[0];
                //i std::string & entityName = parts[0];

            }

            // TO DO target entites by 'source entity' or 'entity'
            assert(false);

        }

        dbgSystem.addMessage("Error parsing '" + _path +"' ! Contacts with specified entities are not yet implemented!");
        return;



    }else if(_path.substr(0,6)=="STATE:"){

        std::string stateName = _path.substr(6);
        BehaviorState *state = nullptr;

        if(stateName=="THIS"){
            state = _state;

        }else{
            state = _entity->entityController()->findState(stateName);
            if(state==nullptr){
                return;
            }
        }

        _signalQuery.mSignal = &state->stateTrigger();


    }else if(_path.substr(0,16)=="STATE_ANIMATION:"){

        std::string stateName = _path.substr(16);
        BehaviorState *state = nullptr;

        if(stateName=="THIS"){
            state = _state;

        }else{
            state = _entity->entityController()->findState(stateName);
            if(state==nullptr){
                return;
            }
        }

        EntitySystemStateData *stateData = dynamic_cast<EntitySystemStateData *>(state->stateData());  assert(stateData);
        _signalQuery.mSignal = &stateData->animationTrigger();



    }else{

        // CUSTOM SIGNALS

        //if(_path.substr(0,19)=="TRANSPORTER_STATUS:"){


        //}else{

            _entity->obtainSignal_signalQuery(_signalQuery, _path, "");


        //}




    }


}
*/


/*
void _ParseEntitySignal_SignalSetter(Entity *_entity, BehaviorState *_state, const std::string &_path, SignalSetter &_signalSetter)
{


    //}else{

        _entity->obtainSignal_signalSetter(_signalSetter, _path, "");

    //}

    return;


}
*/


MovementEngineData* ObtainMovementEngineData(Entity* _entity, const std::string &name, bool throwError)
{

    for(MovementEngine* me : _entity->movementEngines()){
        MovementEngineData *data = me->getMovementEngineData(name, false);
        if(data){
            return data;
        }
    }


    if( throwError){
        dbgSystem.addMessage("Movement engine data '"+ name +"' not found!");
    }

    return nullptr;

}


TaskEngineData* ObtainTaskEngineData(Entity* _entity, const std::string &name, bool throwError)
{

    for(TaskEngine* me : _entity->tasks()){
        TaskEngineData *data = me->getData(name, false);
        if(data){
            return data;
        }
    }


    if( throwError){
        dbgSystem.addMessage("Task engine data '"+ name +"' not found!");
    }

    return nullptr;

}


//===============================================================================================================


void EntitySignalsParser::parseSignalAccessor(PlayedScene *_scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject *obj1, BaseObject *obj2)
{

    assert(obj1);
    Entity * _actor = static_cast<Entity*>(obj1);
    LogicState * _state = nullptr;
    if(obj2 !=nullptr){
        _state = static_cast<LogicState*>(obj2);
    }


    EntitySystem *entitySystem = _actor->parentEntitySystem();

    std::vector<std::string>pathParts = StdString::splitString(_path, "/");

    std::string signalOrigin;
    std::string signalNameAndValue;
    std::string signalStyle;


    if(pathParts.size()>0){
        signalOrigin = pathParts[0];
    }
    if(pathParts.size()>1){
        signalNameAndValue = pathParts[1];
    }
    if(pathParts.size()>2){
        signalStyle = pathParts[2];
    }


    //-----

    //if(_ParseSignalAccessorStyle(signalStyle, _signalAccessor)==false){
    //    return;
    //}


    //-----
    //if(signalOrigin=="INPUT_COMMAND"){

    //    _signalAccessor.mSignal = app->inputSystem()->getGameInputCommand(signalName);
    //    return;
    //

  //  }else

    if(signalOrigin.substr(0,7)=="ENTITY:"){


        std::string triggerOriginEntityName = signalOrigin.substr(7);
        Entity *triggerOriginEntity = nullptr;

        if(triggerOriginEntityName=="THIS"){
            triggerOriginEntity = _actor;

        }else{


            CompositeSprite *rootCompositeSprite = _actor->mapElement().rootCompositeSprite();

            if(rootCompositeSprite){
                DummyFunction();
            }
            if(triggerOriginEntityName=="groundSwitch_doorA2"){
                DummyFunction();
            }

            // when another entity is the origin it is found via 'linkedGroupID' wjich must be different than 0
            // and the same as for _actor !
            if(rootCompositeSprite==nullptr && _actor->linkedGroupID()==0){
                dbgSystem.addMessage("An entity '"+ _actor->sourceEntity()->sourceEntityCfg()->name +"' must have defined 'eLinkedGroupID' parameter in order to connect with other entities with the same 'eLinkedGroupID' parameter!");
                return;
            }

            std::vector<std::string>entityNameParts = StdString::splitString(triggerOriginEntityName, ":");

            std::string name = triggerOriginEntityName;
            int linkedEntityID = 0;

            if(entityNameParts.size()>1){
                name = entityNameParts[0];
                linkedEntityID = StdString::stringToInt(entityNameParts[1], 0);
            }

            triggerOriginEntity = entitySystem->getActor(name, _actor->linkedGroupID(), linkedEntityID, rootCompositeSprite);

            if(triggerOriginEntity==nullptr && linkedEntityID>1){
                _signalAccessor.mNotUsed = true;
                dbgSystem.removeLastMessage();
            }



            //std::vector<Entity*>entities;
            //entitySystem->collectActorsViaLinkedGroupID(entities, name, _actor->linkedGroupID());
        }

        if(triggerOriginEntity==nullptr){
            return;
        }


        //---
        if(_signalAccessor.mType==SignalAccessor::Type::QUERY){
            //_ParseEntitySignal_SignalQuery(triggerOriginEntity, _state, signalName, static_cast<SignalQuery&>(_signalAccessor));
            parse_signalQuery(_scene, triggerOriginEntity, _state, signalNameAndValue, static_cast<SignalQuery&>(_signalAccessor));

        }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){
            //_ParseEntitySignal_SignalSetter(triggerOriginEntity, _state, signalName, static_cast<SignalSetter&>(_signalAccessor));
            parse_signalSetter(_scene, triggerOriginEntity, _state, signalNameAndValue, static_cast<SignalSetter&>(_signalAccessor));

        }

        /*
        if(_signalAccessor.mSignal){

            if(_signalAccessor.mSignal->id() == static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){
                if(_state){
                    _state->customUpdatedSignals().push_back(_signalAccessor.mSignal);
                }
                _signalAccessor.mOwnedSignal = true;   // Cstom signals are owned by signal accessors !
            }
            return;
        }
        */

    }


    dbgSystem.addMessage("Error parsing '" + _path +"' !");
    return;



}


/*

void EntitySignalsParser::parse_signalQuery(PlayedScene *_scene, Entity *_entity, LogicState *_state, const std::string &_signalNameAndValue, SignalQuery &_signalQuery)
{

    EntitySystem *entitySystem = _entity->parentEntitySystem();


    ParsedSignalPath psp(_signalNameAndValue);


    if(_signalNameAndValue=="CONTACT" || _signalNameAndValue=="CONTACT:ANY"){

        _signalQuery.mSignal = &_entity->contactTrigger();
        return;


    }else if(_signalNameAndValue.substr(0,8)=="CONTACT:"){


        FilteredContactTriggersGroup &g = entitySystem->filteredContactTriggersGroup();
        FilteredContactTrigger *trigger = g.getNewFilteredContactTrigger(&_entity->contactTrigger());

        std::string contactedEntity = _signalNameAndValue.substr(8);

        if(contactedEntity.substr(0,9)=="CATEGORY:"){

            EntityCategoriesGroup & ecs = entitySystem->entityCategoriesGroup();

            unsigned int bits = 0;

            std::string sContactedEntityCategories = contactedEntity.substr(9);
            std::vector<std::string> contactedEntityCategories = StdString::splitString(sContactedEntityCategories, "|");
            for(std::string & csName : contactedEntityCategories){
                EntityCategory *ec = ecs.getEntityCategory(csName);
                if(ec==nullptr){
                    return;
                }
                bits |= ec->contactBits;
            }


            trigger->setCategoriesBits(bits);

            _signalQuery.mSignal = trigger;
            return;


        }else{

            std::vector<std::string>parts = StdString::splitString(contactedEntity, ":");

            if(parts.size()==1){

                const std::string & entityNames = parts[0];
                std::vector<std::string>vEntitiyNames = StdString::splitString(entityNames, ",");

                for(const std::string & s : vEntitiyNames){
                    Entity *e = entitySystem->getActor(s);
                    if(e==nullptr){
                        return;
                    }
                    trigger->addSourceEntity(e->sourceEntity());
                }

                _signalQuery.mSignal = trigger;
                return;


            }else if(parts.size()==2){

                //const std::string & entityName = parts[0];
                //i std::string & entityName = parts[0];

            }

            // TO DO target entites by 'source entity' or 'entity'
            assert(false);

        }

        dbgSystem.addMessage("Error parsing '" + _signalNameAndValue +"' ! Contacts with specified entities are not yet implemented!");
        return;



    }else if(_signalNameAndValue.substr(0,6)=="STATE:"){

        std::string stateName = _signalNameAndValue.substr(6);
        LogicState *state = nullptr;

        if(stateName=="THIS"){
            state = _state;

        }else{
            //state = _entity->entityController()->findState(stateName);
            //state = _entity->entityController()->findChildStateViaPath(stateName);
            //state = ObtainLogicStateFromPath(_scene, _state, stateName);
            state = ObtainLogicStateFromPath_SeekFromRootState(_scene, _entity->entityController(), stateName);
            if(state==nullptr){
                return;
            }
        }

        //_signalQuery.mSignal = &state->stateTrigger();
        _signalQuery.mSignal = &state->stateSignal();

    }else if(_signalNameAndValue.substr(0,16)=="STATE_ANIMATION:"){

        std::string stateName = _signalNameAndValue.substr(16);
        LogicState *state = nullptr;

        if(stateName=="THIS"){
            state = _state;

        }else{
            state = _entity->entityController()->findChildState(stateName);
            if(state==nullptr){
                return;
            }
        }
        EntityCustomLogicState *entityCustomState = dynamic_cast<EntityCustomLogicState *>(state->customState());  assert(entityCustomState);
        _signalQuery.mSignal = &entityCustomState->animationTrigger();


    }else{

        // engine and custom entity signals

        _entity->obtainSignal_signalQuery(_signalQuery, _signalNameAndValue, "");

    }
}

*/




void EntitySignalsParser::parse_signalQuery(PlayedScene *_scene, Entity *_entity, LogicState *_state, const std::string &_signalNameAndValue, SignalQuery &_signalQuery)
{

    EntitySystem *entitySystem = _entity->parentEntitySystem();


    ParsedSignalPath psp(_signalNameAndValue);


    if(psp.signalNamePartAt(0)=="CONTACT"){

        if(psp.signalNamePartAt(1)=="ANY" || psp.signalNamePartAt(1)==""){
            psp.obtainValue(_signalQuery, &_entity->contactTrigger());
            return;

        }else if(psp.signalNamePartAt(1)=="CATEGORY"){

            FilteredContactSignalsStorage &g = entitySystem->filteredContactTriggersGroup();
            FilteredContactSignal *trigger = g.getNewFilteredContactSignal(&_entity->contactTrigger());

            EntityCategoriesGroup & ecs = entitySystem->entityCategoriesGroup();

            unsigned int bits = 0;

            std::string sContactedEntityCategories = psp.signalNamePartAt(2);
            std::vector<std::string> contactedEntityCategories = StdString::splitString(sContactedEntityCategories, "|");
            for(std::string & csName : contactedEntityCategories){
                EntityCategory *ec = ecs.getEntityCategory(csName);
                if(ec==nullptr){
                    return;
                }
                bits |= ec->contactBits;
            }


            trigger->setCategoriesBits(bits);

            psp.obtainValue(_signalQuery, trigger);

            return;


        }
        /*
         * else{

            FilteredContactTriggersGroup &g = entitySystem->filteredContactTriggersGroup();
            FilteredContactTrigger *trigger = g.getNewFilteredContactTrigger(&_entity->contactTrigger());
            std::string contactedEntity = psp.signalNamePartAt(2);

            std::vector<std::string>parts = StdString::splitString(contactedEntity, ":");

            if(parts.size()==1){

                const std::string & entityNames = parts[0];
                std::vector<std::string>vEntitiyNames = StdString::splitString(entityNames, ",");

                for(const std::string & s : vEntitiyNames){
                    Entity *e = entitySystem->getActor(s);
                    if(e==nullptr){
                        return;
                    }
                    trigger->addSourceEntity(e->sourceEntity());
                }

                _signalQuery.mSignal = trigger;
                return;


            }else if(parts.size()==2){

                //const std::string & entityName = parts[0];
                //i std::string & entityName = parts[0];

            }

            // TO DO target entites by 'source entity' or 'entity'
            assert(false);

        }
        */

        dbgSystem.addMessage("Error parsing '" + _signalNameAndValue +"' ! Contacts with specified entities are not yet implemented!");
        return;



    }else if(psp.signalNamePartAt(0)=="STATE"){

        std::string stateName = psp.signalNamePartAt(1);
        LogicState *state = nullptr;

        if(stateName=="THIS"){
            state = _state;

        }else{
            state = ObtainLogicStateFromPath_SeekFromRootState(_scene, _entity->entityController(), stateName);
            if(state==nullptr){
                return;
            }
        }

        psp.obtainValue(_signalQuery, &state->stateSignal());


    }else if(psp.signalNamePartAt(0)=="STATE_ANIMATION"){

        std::string stateName = psp.signalNamePartAt(1);
        LogicState *state = nullptr;

        if(stateName=="THIS"){
            state = _state;

        }else{
            state = ObtainLogicStateFromPath_SeekFromRootState(_scene, _entity->entityController(), stateName);
            if(state==nullptr){
                return;
            }
        }
        EntityCustomLogicState *entityCustomState = dynamic_cast<EntityCustomLogicState *>(state->customState());
        if(entityCustomState){
            psp.obtainValue(_signalQuery, &entityCustomState->animationSignal());

        }else{
            dbgSystem.addMessage("Error parsing '" + _signalNameAndValue +"'! Animation signal does not exists on the specified state!");
        }

    }else{

        // engine and custom entity signals

        _entity->obtainSignal_signalQuery(_signalQuery, psp, "");

    }


}



void EntitySignalsParser::parse_signalSetter(PlayedScene *_scene, Entity *_entity, LogicState *_state, const std::string &_signalNameAndValue, SignalSetter &_signalSetter)
{

    ParsedSignalPath psp(_signalNameAndValue);

    // engine and custom entity signals

    _entity->obtainSignal_signalSetter(_signalSetter, psp, "");

}


//--------------------------------------------------------------------------------------------

/*
 *
bool EntitySignalStrings::parse(const std::string &_path)
{

    std::vector<std::string> parts = StdString::splitString(_path, ":");

    if(parts.size()>0){
        if(parts[0]=="ENGINE"){
            engineCfgName = parts[1];
             if(parts.size()>2){
                 signalName = parts[2];
             }
             if(parts.size()>3){
                 signalValue = parts[3];
            }

        }else if(parts[0]=="TASK"){
             taskCfgName = parts[1];
             if(parts.size()>2){
                 signalName = parts[2];
             }
             if(parts.size()>3){
                 signalValue = parts[3];
             }

        }else{

            signalName = parts[0];
            if(parts.size()>1){
                signalValue = parts[1];
            }
        }
    }

    if(signalName.empty()){
        dbgSystem.addMessage("Error parsing signal path '" + _path + "'!");
        return false;
    }

    // parsing value state for bool signals
    std::vector<std::string> parts2 = StdString::splitString(signalName, "=");
    if(parts2.size()==2){
        signalName = parts2[0];
        signalValueState = parts2[1];
    }

    // parsing value state for int signals and bitflag signals
    if(signalValue.empty()==false){
        parts2 = StdString::splitString(signalValue, "=");
        if(parts2.size()==2){
            signalValue = parts2[0];
            signalValueState = parts2[1];
        }
    }

    return true;

}


bool EntitySignalStrings::getBoolValue(bool &_state, bool _setErrorMessage) const
{

    if(signalValueState=="TRUE" || signalValueState=="ON" || signalValueState=="1"){
        _state = true;
        return true;

    }else if(signalValueState=="FALSE" || signalValueState=="OFF" || signalValueState=="0"){
        _state = false;
        return true;
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("Wrong signal value '" + signalValueState +" ' !");
    }

    return false;

}


bool EntitySignalStrings::getIntValue(int &_value) const
{

    _value = StdString::stringToInt(signalValueState, 0);

    return true;

}

*/


}













