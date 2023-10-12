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

#include "jpEntityCommon.h"
#include "jpB2Body.h"
#include "jpB2Fixture.h"
#include "jpB2World.h"
#include "scripting/jpBehavior.h"
#include "jpSourceEntity.h"
#include "jpEntity.h"
#include "jpEntitySystem.h"
#include "task/jpTaskBase.h"
#include "movements/jpMovementBase.h"
#include "jpQueries.h"

#include "jpEntityUtilities.h"




namespace jugimap{




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

            /*
            assert(_proposedTrigger);
            assert(_proposedTrigger->id==static_cast<unsigned char>(TriggerID::TARGET_ENTITIES));


            FilteredContactTrigger *te = static_cast<FilteredContactTrigger*>(_proposedTrigger);

            EntityCategoriesGroup & ecs = app->entitySystem()->entityCategoriesGroup();

            std::string sContactedEntityCategories = contactedEntity.substr(9);
            std::vector<std::string> contactedEntityCategories = StdString::splitString(sContactedEntityCategories, ",");
            for(std::string & csName : contactedEntityCategories){
                EntityCategory *ec = ecs.getEntityCategory(csName);
                if(ec==nullptr){
                    return nullptr;
                }
                te->categoriesBits |= ec->mContactBits;
            }

            return _proposedTrigger;
            */

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
            state = _entity->entityController()->state(stateName);
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
            state = _entity->entityController()->state(stateName);
            if(state==nullptr){
                return;
            }
        }

        _signalQuery.mSignal = &state->animationTrigger();

    /*
    }else if(_path.substr(0,7)=="ENGINE:"){

        std::string subPath = _path.substr(7);
        std::vector<std::string>parts = StdString::splitString(subPath, ":");
        if(parts.size()!=2){
            dbgSystem.addMessage("Wrong subPath '" + subPath +"' !");
            return;
        }

        const std::string & engineName = parts[0];
        const std::string & triggerName = parts[1];

        _signalQuery.mSignal = _ParseEngineTrigger(_entity, engineName, triggerName);

    */
    }else{

        // CUSTOM SIGNALS

        //if(_path.substr(0,19)=="TRANSPORTER_STATUS:"){


        //}else{

            _entity->obtainSignal_signalQuery(_signalQuery, _path, "");


        //}




    }


}



void _ParseEntitySignal_SignalSetter(Entity *_entity, BehaviorState *_state, const std::string &_path, SignalSetter &_signalSetter)
{
    /*
    if(_path.substr(0,19)=="TRANSPORTER_STATUS:"){

        _signalSetter.mSignal = _ParseEntitySignal(_entity, SignalID::TRANSPORTER_STATUS);
        if(_signalSetter.mSignal==nullptr){
            return;
        }

        std::string sValue = _path.substr(19);
        _signalSetter.mIntValue = GetTransporterStatusFromString_signalSetter(sValue);
        if(_signalSetter.mIntValue == TransporterStatus::UNKNOWN){
            dbgSystem.addMessage("Transporter status '" + sValue +" ' is unknown or unsupported by the signal setter!");
            return;
        }
    */

    //}else{

        _entity->obtainSignal_signalSetter(_signalSetter, _path, "");

    //}

    return;


}


/*

void ObtainSignalFromPath(const std::string &_path, PlayedScene *_scene, Entity *_actor, BehaviorState *_state, SignalAccessor &_signalAccessor)
{

    EntitySystem *entitySystem = _actor->parentEntitySystem();

    std::vector<std::string>pathParts = StdString::splitString(_path, "/");

    std::string signalOrigin;
    std::string signalName;
    std::string signalStyle;


    if(pathParts.size()>0){
        signalOrigin = pathParts[0];
    }
    if(pathParts.size()>1){
        signalName = pathParts[1];
    }
    if(pathParts.size()>2){
        signalStyle = pathParts[2];
    }


    //-----

    if(_ParseSignalAccessorStyle(signalStyle, _signalAccessor)==false){
        return;
    }


    //-----
    if(signalOrigin=="INPUT_COMMAND"){

        _signalAccessor.mSignal = app->inputSystem()->getGameInputCommand(signalName);
        return;


    }else if(signalOrigin.substr(0,7)=="ENTITY:"){


        std::string triggerOriginEntityName = signalOrigin.substr(7);
        Entity *triggerOriginEntity = nullptr;

        if(triggerOriginEntityName=="THIS"){
            triggerOriginEntity = _actor;

        }else{

            // when another entity is the origin it is found via 'linkedGroupID' wjich must be different than 0
            // and the same as for _actor !
            if(_actor->linkedGroupID()==0){
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

            triggerOriginEntity = entitySystem->getActor(name, _actor->linkedGroupID(), linkedEntityID);
        }

        if(triggerOriginEntity==nullptr){
            return;
        }


        //---
        if(_signalAccessor.mType==SignalAccessor::Type::QUEERY){
            _ParseEntitySignal_SignalQuery(triggerOriginEntity, _state, signalName, static_cast<SignalQuery&>(_signalAccessor));

        }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){
            _ParseEntitySignal_SignalSetter(triggerOriginEntity, _state, signalName, static_cast<SignalSetter&>(_signalAccessor));
        }

        if(_signalAccessor.mSignal){
            return;
        }

    }


    dbgSystem.addMessage("Error parsing '" + _path +"' !");
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


void EntitySignalsParser::parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, void *obj1, void *obj2, void *obj3)
{


    Entity * _actor = static_cast<Entity*>(obj1);
    BehaviorState * _state = nullptr;
    if(obj2 !=nullptr){
        _state = static_cast<BehaviorState*>(obj2);
    }


    EntitySystem *entitySystem = _actor->parentEntitySystem();

    std::vector<std::string>pathParts = StdString::splitString(_path, "/");

    std::string signalOrigin;
    std::string signalName;
    std::string signalStyle;


    if(pathParts.size()>0){
        signalOrigin = pathParts[0];
    }
    if(pathParts.size()>1){
        signalName = pathParts[1];
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
            parse_signalQuery(triggerOriginEntity, _state, signalName, static_cast<SignalQuery&>(_signalAccessor));

        }else if(_signalAccessor.mType==SignalAccessor::Type::SETTER){
            //_ParseEntitySignal_SignalSetter(triggerOriginEntity, _state, signalName, static_cast<SignalSetter&>(_signalAccessor));
            parse_signalSetter(triggerOriginEntity, _state, signalName, static_cast<SignalSetter&>(_signalAccessor));

        }

        if(_signalAccessor.mSignal){

            if(_signalAccessor.mSignal->id() == static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){
                if(_state){
                    _state->customEntitySignals().push_back(_signalAccessor.mSignal);
                }
                _signalAccessor.mOwnedSignal = true;   // Cstom signals are owned by signal accessors !
            }
            return;
        }

    }


    dbgSystem.addMessage("Error parsing '" + _path +"' !");
    return;



}



void EntitySignalsParser::parse_signalQuery(Entity *_entity, BehaviorState *_state, const std::string &_path, SignalQuery &_signalQuery)
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
            state = _entity->entityController()->state(stateName);
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
            state = _entity->entityController()->state(stateName);
            if(state==nullptr){
                return;
            }
        }

        _signalQuery.mSignal = &state->animationTrigger();


    }else{

        // engine and custom entity signals

        _entity->obtainSignal_signalQuery(_signalQuery, _path, "");

    }


}



void EntitySignalsParser::parse_signalSetter(Entity *_entity, BehaviorState *_state, const std::string &_path, SignalSetter &_signalSetter)
{

    // engine and custom entity signals

    _entity->obtainSignal_signalSetter(_signalSetter, _path, "");

}




}













