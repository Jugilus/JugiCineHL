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
#include "jpPlayedScene.h"
#include "jpInput.h"
#include "jpUtilities.h"
#include "jpCommands.h"

#include "data/jpItem.h"
#include "data/jpDataUtilities.h"
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


void EntitySignalsParser::reset()
{

    mThisLogicState = nullptr;
    mEntitySystem = nullptr;
    mThisEntity = nullptr;
    mOriginEntity = nullptr;
    mOriginSourceEntity = nullptr;
    mOriginObtainer = nullptr;

    mAccessorNotUsed = false;
}


bool EntitySignalsParser::parseObject(PlayedScene *_scene, SignalParsingInfo &_spi)
{

    reset();

    mThisEntity = static_cast<Entity*>(_spi.rootParentObject);
    mThisLogicState = _spi.logicState;
    mEntitySystem = dynamic_cast<EntitySystem*>(_scene->componentsGroup()->getComponent("entitySystem"));
    if(mEntitySystem==nullptr){
        dbgSystem.addMessage("Entity system not found!");
        return false;
    }

    if(_spi.originObjectPath.size()>7 && _spi.originObjectPath.substr(0,7)=="ENTITY:"){

        if(parseOriginEntity(_scene, _spi.originObjectPath)==false){
            return false;
        }
        _spi.originObjectObtainer = mOriginObtainer;


    }else if(_spi.originObjectPath.size()>14 && _spi.originObjectPath.substr(0,14)=="SOURCE_ENTITY:"){
        std::string sourceEntity =_spi.originObjectPath.substr(14);

        SourceEntity *se =mEntitySystem->sourceEntitiesGroup()->getSourceEntitiy(sourceEntity);
        if(se==nullptr){
            return false;
        }
        _spi.originObject = se;


    }else{
        dbgSystem.addMessage("Error parsing origin object path '" + _spi.originObjectPath +"'!");
        return false;
    }

    return true;
}


bool EntitySignalsParser::parseSignalAccessorNEW(PlayedScene *_scene, SignalParsingInfo &_spi, const std::string &_signalValue)
{

    reset();

    mThisEntity = static_cast<Entity*>(_spi.rootParentObject);
    mThisLogicState = _spi.logicState;
    mEntitySystem = dynamic_cast<EntitySystem*>(_scene->componentsGroup()->getComponent("entitySystem"));
    if(mEntitySystem==nullptr){
        dbgSystem.addMessage("Entity system not found!");
        return false;
    }


    if(_spi.originObjectPath.size()>7 && _spi.originObjectPath.substr(0,7)=="ENTITY:"){

        if(parseEntitySignal(_scene, _spi, _signalValue)==false){
            return false;
        }


    }else if(_spi.originObjectPath=="ENTITY_MANAGER"){

        if(parseEntityManagerSignal(_scene, _spi, _signalValue)==false){
            return false;
        }

    }else{
        dbgSystem.addMessage("Error parsing origin object path '" + _spi.originObjectPath +"'!");
        return false;
    }

    return true;
}


bool EntitySignalsParser::parseOriginEntity(PlayedScene *_scene, const std::string &_originPath)
{


    if(_originPath.size()>7 && _originPath.substr(0,7)=="ENTITY:"){

        std::string originEntityName = _originPath.substr(7);

        if(originEntityName=="THIS"){
            mOriginEntity = mThisEntity;
            mOriginSourceEntity = mOriginEntity->sourceEntity();


        }else if(originEntityName.size()>10 && originEntityName.substr(0,10)=="CONTACTED:"){
            originEntityName = originEntityName.substr(10);
            mOriginSourceEntity = mEntitySystem->sourceEntitiesGroup()->getSourceEntitiy(originEntityName);
            if(mOriginSourceEntity==nullptr){
                return false;
            }

            mOriginObtainer = new ContactedEntityObtainer(mThisEntity, mOriginSourceEntity, nullptr);

        }else{

            CompositeSprite *rootCompositeSprite = mThisEntity->mapElement().rootCompositeSprite();

            if(rootCompositeSprite){
                DummyFunction();
            }
            if(originEntityName=="groundSwitch_doorA2"){
                DummyFunction();
            }

            // when another entity is the origin it is found via 'linkedGroupID' which must be different than 0
            // and the same as for thisEntity !
            if(rootCompositeSprite==nullptr && mThisEntity->linkedGroupID()==0){
                dbgSystem.addMessage("An entity '"+ mThisEntity->sourceEntity()->sourceEntityCfg()->name +"' must have defined 'eLinkedGroupID' parameter in order to connect with other entities with the same 'eLinkedGroupID' parameter!");
                return false;
            }

            std::vector<std::string>entityNameParts = StdString::splitString(originEntityName, ":");

            std::string name = originEntityName;
            int linkedEntityID = 0;

            if(entityNameParts.size()>1){
                name = entityNameParts[0];
                if(StdString::integerNumber(entityNameParts[1], linkedEntityID)==false){
                    return false;
                }
                //linkedEntityID = StdString::stringToInt(entityNameParts[1], 0);
            }

            mOriginEntity = mEntitySystem->getActor(name, mThisEntity->linkedGroupID(), linkedEntityID, rootCompositeSprite);


            if(mOriginEntity==nullptr){

                // this bellow handle situation when you have varring number of linked entities, which can make designing linking easier
                if(linkedEntityID>1){
                    mAccessorNotUsed = true;
                    dbgSystem.removeLastMessage();

                }else{
                    return false;
                }

            }else{

                mOriginSourceEntity = mOriginEntity->sourceEntity();
            }
        }

    }else{
        dbgSystem.addMessage("Wrong origin path '" + _originPath +"' !");
        return false;
    }


    return true;

}


bool EntitySignalsParser::parseEntitySignal(PlayedScene *_scene, SignalParsingInfo &_spi, const std::string &_signalValue)
{


    if(parseOriginEntity(_scene, _spi.originObjectPath)==false){
        return false;
    }

    if(mAccessorNotUsed){
        _spi.accessorNotUsed = true;
        return true;
    }


    _spi.originObjectObtainer = mOriginObtainer;


    //---
    std::string signalIdentifier = _spi.signalIdentifierPath;

    if(signalIdentifier=="STATE:THIS"){
        assert(mThisLogicState);
        signalIdentifier = obtainStateIdentifierString(mThisLogicState, "STATE");


    }else if(signalIdentifier=="STATE_ANIMATION:THIS"){
        assert(mThisLogicState);
        signalIdentifier = obtainStateIdentifierString(mThisLogicState, "STATE_ANIMATION");


    }else if(_spi.signalIdentifierPath.size()>8 && _spi.signalIdentifierPath.substr(0,8)=="CONTACT:"){
        signalIdentifier = _spi.signalIdentifierPath.substr(0,7);        // without the last ':'

        std::string contacted = _spi.signalIdentifierPath.substr(8);

        if(contacted=="ANY"){

            if(mOriginEntity==nullptr){
                _spi.signalObtainer = new ContactSignalObtainer(mOriginEntity, nullptr, nullptr, true);
            }

        }else if(contacted=="THIS"){
            //_spi.signalObtainer = new ContactSignalObtainer(mOriginEntity, sourceEntity, nullptr, false);
            if(mOriginEntity==nullptr){
                _spi.signalObtainer = new ContactSignalObtainer(nullptr, mThisEntity->sourceEntity(), nullptr, false);
            }

        }else{

            std::vector<std::string> parts = StdString::splitString(contacted, "|");

            if(parts.size()==1){

                const std::string & part = parts.front();

                if(part.size()>4 && part.substr(0,4)=="ENT:"){

                    std::string entityName = part.substr(4);
                    SourceEntity *sourceEntity = mEntitySystem->sourceEntitiesGroup()->getSourceEntitiy(entityName);
                    if(sourceEntity==nullptr){
                        return false;
                    }
                    _spi.signalObtainer = new ContactSignalObtainer(mOriginEntity, sourceEntity, nullptr, false);


                }else if(part.size()>4 && part.substr(0,4)=="CAT:"){

                    std::string categoryName = part.substr(4);
                    EntityCategory *ec = mEntitySystem->entityCategoriesGroup().getEntityCategory(categoryName);
                    if(ec==nullptr){
                        return false;
                    }
                    _spi.signalObtainer = new ContactSignalObtainer(mOriginEntity, nullptr, ec, false);

                }else{

                    dbgSystem.addMessage("Error parsing contact path'" + contacted + "'!");
                    return false;
                }


            }else{

                _spi.signalObtainer = new ContactSignalObtainer_multi(mOriginEntity);

                for(const std::string & part : parts){

                    if(part.size()>4 && part.substr(0,4)=="ENT:"){

                        std::string entityName = part.substr(4);
                        SourceEntity *se = mEntitySystem->sourceEntitiesGroup()->getSourceEntitiy(entityName);
                        if(se==nullptr){
                            return false;
                        }
                        static_cast<ContactSignalObtainer_multi*>(_spi.signalObtainer)->addSourceEntity(se);

                    }else if(part.size()>4 && part.substr(0,4)=="CAT:"){

                        std::string categoryName = part.substr(4);
                        EntityCategory *ec = mEntitySystem->entityCategoriesGroup().getEntityCategory(categoryName);
                        if(ec==nullptr){
                            return false;
                        }
                        static_cast<ContactSignalObtainer_multi*>(_spi.signalObtainer)->addEntityCategory(ec);

                    }else{

                        dbgSystem.addMessage("Error parsing contact path'" + contacted + "'!");
                        return false;
                    }
                }

            }

            /*
            SourceEntity *sourceEntity = mEntitySystem->sourceEntitiesGroup()->getSourceEntitiy(contacted);
            if(sourceEntity==nullptr){
                return false;
            }
            _spi.signalObtainer = new ContactSignalObtainer(mOriginEntity, sourceEntity, nullptr, false);
            */
        }
    }


    if(_spi.signalAccessorType==SignalAccessorType::QUERY){
        if(mOriginSourceEntity->signalIdentifiers().getParsingInfo_query(_spi, signalIdentifier, _signalValue)==false){
            return false;
        }

        if(mOriginEntity){
            _spi.originObject = mOriginEntity;
            _spi.signal = _spi.originObject->signalStorage()->getSignal_query(_spi.signalIndex);
        }

    }else if(_spi.signalAccessorType==SignalAccessorType::SETTER){
        if(mOriginSourceEntity->signalIdentifiers().getParsingInfo_setter(_spi, signalIdentifier, _signalValue)==false){
            return false;
        }

        if(mOriginEntity){
            _spi.originObject = mOriginEntity;
            _spi.signal = _spi.originObject->signalStorage()->getSignal_setter(_spi.signalIndex);
        }
    }

    return true;

}


bool EntitySignalsParser::parseEntityManagerSignal(PlayedScene *_scene, SignalParsingInfo &_spi, const std::string &_signalValue)
{

    //---
    std::string signalIdentifier = _spi.signalIdentifierPath;

    if(_spi.signalAccessorType==SignalAccessorType::QUERY){
        if(mEntitySystem->signalIdentifiers().getParsingInfo_query(_spi, signalIdentifier, _signalValue)==false){
            return false;
        }

        _spi.originObject = mEntitySystem;
        _spi.signal = _spi.originObject->signalStorage()->getSignal_query(_spi.signalIndex);

    }else if(_spi.signalAccessorType==SignalAccessorType::SETTER){
        if(mEntitySystem->signalIdentifiers().getParsingInfo_setter(_spi, signalIdentifier, _signalValue)==false){
            return false;
        }

        _spi.originObject = mEntitySystem;
        _spi.signal = _spi.originObject->signalStorage()->getSignal_setter(_spi.signalIndex);

    }

    return true;

}


bool EntitySignalsParser::parseItemDataPairNEW(PlayedScene *_scene, ItemDataParsingHelper &_dpi)
{

    reset();

    mThisEntity = static_cast<Entity*>(_dpi.rootParentObject);
    mThisLogicState = _dpi.logicState;
    mEntitySystem = dynamic_cast<EntitySystem*>(_scene->componentsGroup()->getComponent("entitySystem"));
    if(mEntitySystem==nullptr){
        dbgSystem.addMessage("Entity system not found!");
        return false;
    }


    if(parseOriginEntity(_scene, _dpi.originObjectPath)==false){
        return false;
    }

    if(mAccessorNotUsed){
        _dpi.accessorNotUsed = true;
        return true;
    }


    //----
    SourceItem *sourceItem = _scene->sourceItemsStorage()->findSourceItem(mOriginSourceEntity->sourceEntityCfg()->item);
    if(sourceItem==nullptr){
        return false;
    }

    if(_dpi.dataAccessorType==DataAccessorType::DATA_QUERY){
        if(sourceItem->itemDataIdentifiers().getParsingHelper(_dpi, _dpi.itemAndDataPath)==false){
            return false;
        }

    }else if(_dpi.dataAccessorType==DataAccessorType::DATA_SETTER){
        if(sourceItem->itemDataIdentifiers().getParsingHelper(_dpi, _dpi.itemAndDataPath)==false){
            return false;
        }

    }else if(_dpi.dataAccessorType==DataAccessorType::DATA_TO_TEXT){
        if(sourceItem->itemDataIdentifiers().getParsingHelper(_dpi, _dpi.itemAndDataPath)==false){
            return false;
        }

    }else if(_dpi.dataAccessorType==DataAccessorType::ITEM_QUERY){
        if(sourceItem->itemDataIdentifiers().getParsingHelper(_dpi, _dpi.itemAndDataPath)==false){
            return false;
        }

    }else if(_dpi.dataAccessorType==DataAccessorType::ITEM_MOVER){
        if(sourceItem->itemDataIdentifiers().getParsingHelper(_dpi, _dpi.itemAndDataPath)==false){
            return false;
        }
    }

    //----
    _dpi.accesorData->originObject = mOriginEntity;
    _dpi.accesorData->originObjectObtainer = mOriginObtainer;

    // item and data are not obtained immediately becouse the items in origin object may change at runtime
    /*
    if(mOriginEntity){
        _dpi.accesorData->originObject = mOriginEntity;
        _dpi.accesorData->item = _dpi.accesorData->originObject->item()->findFirstChildItem(_dpi.accesorData->sourceItem);
        if(_dpi.accesorData->item==nullptr){
            dbgSystem.addMessage("Item with path '" + _dpi.itemAndDataPath + "' not found within entity '" + mOriginSourceEntity->name() + "'!");
            return false;
        }

        if(_dpi.accesorData->item && _dpi.accesorData->dataIndex != -1){
             _dpi.accesorData->data = _dpi.accesorData->item->dataStorage().at(_dpi.accesorData->dataIndex);
        }

        //_dpi.signal = _spi.signalOriginObject->signalStorage()->getSignal_query(_spi.signalIndex);
    }
    */

    return true;
}


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
                _signalAccessor._setNotUsed(true);
                dbgSystem.removeLastMessage();
            }


            //std::vector<Entity*>entities;
            //entitySystem->collectActorsViaLinkedGroupID(entities, name, _actor->linkedGroupID());
        }

        if(triggerOriginEntity==nullptr){
            return;
        }


        //---
        if(_signalAccessor.type()==SignalAccessorType::QUERY){
            parse_signalQuery(_scene, triggerOriginEntity, _state, signalNameAndValue, static_cast<SignalQuery&>(_signalAccessor));

        }else if(_signalAccessor.type()==SignalAccessorType::SETTER){
            parse_signalSetter(_scene, triggerOriginEntity, _state, signalNameAndValue, static_cast<SignalSetter&>(_signalAccessor));

        }

    }

}


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


void EntitySignalsParser::parseItemDataPair(PlayedScene *scene, const std::string &_path, ItemDataPair &_itemDataPair, ParseItemMode _parseItemMode, BaseObject *obj1, BaseObject *obj2)
{

    assert(obj1);
    Entity *thisEntity = dynamic_cast<Entity*>(obj1);
    LogicState * _state = nullptr;
    if(obj2 !=nullptr){
        _state = static_cast<LogicState*>(obj2);
    }


    //EntitySystem *entitySystem = thisEntity->parentEntitySystem();

    EntitySystem *entitySystem = dynamic_cast<EntitySystem*>(scene->componentsGroup()->getComponent("entitySystem"));
    if(entitySystem==nullptr){
        dbgSystem.addMessage("Entity system not found!");
        return;
    }


    std::vector<std::string>pathParts = StdString::splitString(_path, "/");

    std::string origin;
    std::string itemAndDataName;

    if(pathParts.size()>0){
        origin = pathParts[0];
    }
    if(pathParts.size()>1){
        itemAndDataName = pathParts[1];
    }


    if(origin.substr(0,7)=="ENTITY:"){

        std::string entityName = origin.substr(7);
        Entity *entity = nullptr;

        if(entityName=="THIS"){
            entity = thisEntity;

        }else{
            entity = entitySystem->getActor(entityName);
        }

        if(entity==nullptr){
            return;
        }


        //---
        Item *rootItem = entity->item();

        if(rootItem==nullptr){
            dbgSystem.addMessage("Entity '" + entityName + "' does not have assigned an item!");
            return;
        }

        rootItem->findChildItemDataViaPath(itemAndDataName, _itemDataPair, _parseItemMode);

    }

}


bool EntitySignalsParser::parseObjectName(PlayedScene *_scene, PrintParsingHelper &_pph)
{

    mThisEntity = static_cast<Entity*>(_pph.rootParentObject);
    mThisLogicState = _pph.logicState;

    if(parseOriginEntity(_scene, _pph.originObject)==false){
        return false;
    }

    _pph.originObtainer = mOriginObtainer;


    if(_pph.textKind=="OBJECT_NAME"){
        _pph.text = mOriginSourceEntity->name();

    }else if(_pph.textKind=="ITEM"){

        if(mOriginEntity){
            Item *rootItem = mOriginEntity->item();

            if(rootItem==nullptr){
                dbgSystem.addMessage("Entity '" + mOriginEntity->sourceEntity()->name() + "' does not have assigned an item!");
                return false;
            }

            rootItem->findChildItemDataViaPath(_pph.originData, _pph.itemDataPair, ParseItemMode::DATA);
        }

    }

    return true;


}


}













