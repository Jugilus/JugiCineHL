#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmSceneLayout.h"
#include "jmCommonFunctions.h"
#include "jmStandardSprite.h"
#include "jmUtilities.h"
#include "items/jpItemsCommon.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"
#include "jpUtilities.h"
//#include "jpActionsCommon.h"

#include "jpEntityLogicState.h"
#include "jpB2Body.h"
#include "jpEntitySystem.h"
#include "jpSourceEntity.h"
#include "jpEntity.h"
#include "jpEntityCommon.h"



namespace jugimap{






WorldCameraManager::WorldCameraManager()
{

}


bool WorldCameraManager::initConnections(PlayedScene *_scene)
{

    for(SceneMap* sm : _scene->sceneMaps()){
        if(sm->map()->camera()->GetKind()== CameraKind::WORLD){
            mCamera = static_cast<WorldMapCamera*>(sm->map()->camera());
            break;
        }
    }

    return true;
}


void WorldCameraManager::setFocusEntity(Entity *_entity)
{
    mFocusEntity = _entity;
    /*
    if(mFocusEntity->type()==EntityType::ACTOR){
        Actor *a = static_cast<Actor*>(mFocusEntity);
        StandardSprite *s = a->stdSprite();
        prevPos = s->position();
    }
    */
}



void WorldCameraManager::start()
{



}


void WorldCameraManager::update()
{

    if(mFocusEntity){
        //if(mFocusEntity->type()==EntityType::ACTOR){
        //if(mFocusEntity->type()==EntityType::ACTOR){

            //Entity *a = mFocusEntity;
            Sprite *s = mFocusEntity->sprite();
            //mCamera->SetPointedPosition(prevPos);
            //prevPos = s->absPosition();
            mCamera->SetPointedPosition(s->position());
       // }
    }

}


void WorldCameraManager::setPointedPosition(Vec2f worldPos)
{

    mCamera->SetPointedPosition(worldPos);
}


//-------------------------------------------------------------------------------------------




bool FixtureGroup::containsFixture(b2Fixture *_fixture, int _childIndex)
{

    for(const std::pair<b2Fixture *,int> & p : mFixtures){
        if(p.first == _fixture && p.second == _childIndex){
            return true;
        }
    }

    return false;
}


void FixtureGroup::addFixture(b2Fixture *_fixture, int _childIndex)
{

    //if(containsFixture(_fixture, _childIndex)==false){
        mFixtures.push_back({_fixture,_childIndex});
    //}

}


int FixtureGroup::removeFixture(b2Fixture *_fixture, int _childIndex)
{

    int numRemoved = 0;

    for(int i = int(mFixtures.size())-1; i>=0; i-- ){
        const std::pair<b2Fixture *,int> &p = mFixtures[i];
        if(p.first == _fixture && p.second == _childIndex){
            mFixtures.erase(mFixtures.begin()+i);
            numRemoved ++;

        }
    }

    return numRemoved;

}


//---------------------------------------------------------------------------------

void TwoEntitiesContact::onBeginContact(b2Fixture *_fixtureA, int _childIndexA, b2Fixture *_fixtureB, int _childIndexB)
{

    //mFixturesA.addFixture(_fixtureA, _childIndexA);
    //mFixturesB.addFixture(_fixtureB, _childIndexB);

    mDualFixtures.push_back({_fixtureA, _childIndexA, _fixtureB, _childIndexB});

}


void TwoEntitiesContact::onEndContact(b2Fixture *_fixtureA, int _childIndexA, b2Fixture *_fixtureB, int _childIndexB)
{

    //int numRemovedA = mFixturesA.removeFixture(_fixtureA, _childIndexA);
    //int numRemovedB = mFixturesB.removeFixture(_fixtureB, _childIndexB);

    //if(numRemovedA==0){
    //    DummyFunction();
    //}

    //assert(numRemovedA==1);
    //assert(numRemovedB==1);

    int numRemoved = 0;

    for(int i = int(mDualFixtures.size())-1; i>=0; i-- ){
        DualFixture &df = mDualFixtures[i];
        if(df.fixtureA==_fixtureA && df.childIndexA==_childIndexA &&  df.fixtureB==_fixtureB && df.childIndexB==_childIndexB){
            mDualFixtures.erase(mDualFixtures.begin()+i);
            numRemoved ++;

        }
    }

    assert(numRemoved==1);

}


//bool TwoEntitiesContact::hasContactedFixtures()
//{


//    return !mDualFixtures.empty();

//}


//---------------------------------------------------------------------------------



EntityContactSignal::EntityContactSignal(const EntityShape &_entityShapeA, const EntityShape &_entityShapeB) :
    mTwoEntitiesContact(_entityShapeA, _entityShapeB)
{
    setId(static_cast<unsigned int>(SignalID::ENTITY_CONTACT_TRIGGER));

    dbgName = _entityShapeA.entity->sourceEntity()->sourceEntityCfg()->name + "  -  " + _entityShapeB.entity->sourceEntity()->sourceEntityCfg()->name;

}


ContactTrigger::ContactTrigger()
{
    setId(static_cast<unsigned int>(SignalID::CONTACT_TRIGGER));
}


void ContactTrigger::onBeginContact(const EntityShape &_entityShapeA, b2Fixture *_fixtureA, int _childIndexA, const EntityShape &_entityShapeB, b2Fixture *_fixtureB, int _childIndexB)
{

    for(EntityContactSignal *ect : mContactedEntitiesTriggers){

        if(ect->mTwoEntitiesContact.entityShapeA()==_entityShapeA && ect->mTwoEntitiesContact.entityShapeB()==_entityShapeB){
            ect->mTwoEntitiesContact.onBeginContact(_fixtureA, _childIndexA, _fixtureB, _childIndexB);

            if(ect->storedSignalValues)      // Possible as there may be contacts ended and began between the same fixtures within the the one frame!
            {
                //ect->_setValue(ect->activeStored);
                //ect->_setActiveStarted(ect->activeStartedStored);
                //ect->_setActiveEnded(ect->activeEndedStored);

                ect->_setValue(ect->valueStored);
                ect->_setPreviousValue(ect->previousValueStored);
                ect->_setValueChanged(ect->valueChangedStored);
                ect->storedSignalValues = false;

                ect->setValue(true);
            }
            return;
        }
    }

    EntityContactSignal *ect = new EntityContactSignal(_entityShapeA, _entityShapeB);
    ect->mTwoEntitiesContact.onBeginContact(_fixtureA, _childIndexA, _fixtureB, _childIndexB);
    //ect->setValue(true);
    mContactedEntitiesTriggers.push_back(ect);

}


void ContactTrigger::onEndConctact(const EntityShape &_entityShapeA, b2Fixture* _fixtureA, int _childIndexA, const EntityShape &_entityShapeB, b2Fixture *_fixtureB, int _childIndexB)
{

    for(EntityContactSignal *ect : mContactedEntitiesTriggers){

        if(ect->mTwoEntitiesContact.entityShapeA()==_entityShapeA && ect->mTwoEntitiesContact.entityShapeB()==_entityShapeB){
            ect->mTwoEntitiesContact.onEndContact(_fixtureA, _childIndexA, _fixtureB, _childIndexB);

            if(ect->mTwoEntitiesContact.dualFixtures().empty()){

                // Storing the signal values in case the same contact will began again within the same frame !!
                //ect->activeStored = ect->active();
                //ect->activeStartedStored = ect->activeStarted();
                //ect->activeEndedStored = ect->activeEnded();
                ect->valueStored = ect->value();
                ect->previousValueStored = ect->previousValue();
                ect->valueChangedStored = ect->valueChanged();
                ect->storedSignalValues = true;

                ect->setValue(false);                    // EntityContactTrigger will be deleted on postUpdate !
            }
        }
    }

}



void ContactTrigger::preUpdate_postBox2dStep()
{

    if(mContactedEntitiesTriggers.empty()){
        return;
    }


    //----  delete contacts without contacted fixtures
    for(int i = int(mContactedEntitiesTriggers.size())-1; i>=0; i-- ){
        EntityContactSignal *ect = mContactedEntitiesTriggers[i];
        if(ect->mTwoEntitiesContact.dualFixtures().empty()){
            assert(ect->active(true)==false);

            print("Ending contact between entityA '" + ect->mTwoEntitiesContact.entityShapeA().entity->sourceEntity()->name() +
                  "' (cat = " + ect->mTwoEntitiesContact.entityShapeA().category->name+
                  ") and  entityB '" + ect->mTwoEntitiesContact.entityShapeB().entity->sourceEntity()->name() +
                  "' (cat = " + ect->mTwoEntitiesContact.entityShapeB().category->name + ") !");

            mContactedEntitiesTriggers.erase(mContactedEntitiesTriggers.begin()+i);
            delete ect;
        }
    }


    //---

    mNumContacted = 0;

    for(EntityContactSignal *ect : mContactedEntitiesTriggers){

        Entity * entityA = ect->mTwoEntitiesContact.entityShapeA().entity;
        Entity * entityB = ect->mTwoEntitiesContact.entityShapeB().entity;
        Margin & marginA = entityA->sourceEntity()->sourceEntityCfg()->centeringMargin;
        Margin & marginB = entityB->sourceEntity()->sourceEntityCfg()->centeringMargin;

        bool insideMarginX = true;

        if(marginA.xMarginActive || marginB.xMarginActive){
            b2Vec2 posA = entityA->body()->B2Body()->GetPosition();
            float xPosAMin = posA.x;
            float xPosAMax = posA.x;

            if(marginA.xMarginActive){
                xPosAMin -= marginA.left;
                xPosAMax += marginA.right;
            }

            b2Vec2 posB = entityB->body()->B2Body()->GetPosition();
            float xPosBMin = posB.x;
            float xPosBMax = posB.x;

            if(marginB.xMarginActive){
                xPosBMin -= marginB.left;
                xPosBMax += marginB.right;
            }

            insideMarginX =  xPosBMin <= xPosAMax  &&  xPosBMax >= xPosAMin;
        }


        bool insideMarginY = true;

        if(marginA.yMarginActive || marginB.yMarginActive){

            b2Vec2 posA = entityA->body()->B2Body()->GetPosition();
            float yPosAMin = posA.y;
            float yPosAMax = posA.y;

            if(marginA.yMarginActive){
                yPosAMin -= marginA.bottom;
                yPosAMax += marginA.top;
            }

            b2Vec2 posB = entityB->body()->B2Body()->GetPosition();
            float yPosBMin = posB.y;
            float yPosBMax = posB.y;

            if(marginB.yMarginActive){
                yPosBMin -= marginB.bottom;
                yPosBMax += marginB.top;
            }

            insideMarginY =  yPosBMin <= yPosAMax  &&  yPosBMax >= yPosAMin;
        }

        bool insideMargin = insideMarginX && insideMarginY;

        if(insideMargin){

            if(ect->activeStarted(true)==false && ect->active(true)==false && ect->activeEnded(true)==false){     // not yet set !
                ect->setValue(true);

                print("Inside margin contact with entityA '" + ect->mTwoEntitiesContact.entityShapeA().entity->sourceEntity()->name() +
                      "' (cat = " + ect->mTwoEntitiesContact.entityShapeA().category->name+
                      ") and  entityB '" + ect->mTwoEntitiesContact.entityShapeB().entity->sourceEntity()->name() +
                      "' (cat = " + ect->mTwoEntitiesContact.entityShapeB().category->name + ") !");

            }

        }else{
            if(ect->active(true)){
                ect->setValue(false);

                print("Out of margin contact with entityA '" + ect->mTwoEntitiesContact.entityShapeA().entity->sourceEntity()->name() +
                      "' (cat = " + ect->mTwoEntitiesContact.entityShapeA().category->name+
                      ") and  entityB '" + ect->mTwoEntitiesContact.entityShapeB().entity->sourceEntity()->name() +
                      "' (cat = " + ect->mTwoEntitiesContact.entityShapeB().category->name + ") !");

            }
        }

        if(ect->active(true)){
            mNumContacted ++;
        }
    }


    setValue(mNumContacted>0);

}


void ContactTrigger::postUpdateSignals()
{

    if(mContactedEntitiesTriggers.empty()){
        return;
    }


    /*
    //---- delete ended contacts
    for(int i = int(mContactedEntitiesTriggers.size())-1; i>=0; i-- ){
        EntityContactSignal *ect = mContactedEntitiesTriggers[i];
        //if(ect->mDeleteThis){
        if(ect->mTwoEntitiesContact.dualFixtures().empty()){
            assert(ect->active()==false);

            print("Ending contact between entityA '" + ect->mTwoEntitiesContact.entityShapeA().entity->sourceEntity()->name() +
                  "' (cat = " + ect->mTwoEntitiesContact.entityShapeA().category->name+
                  ") and  entityB '" + ect->mTwoEntitiesContact.entityShapeB().entity->sourceEntity()->name() +
                  "' (cat = " + ect->mTwoEntitiesContact.entityShapeB().category->name + ") !");

            mContactedEntitiesTriggers.erase(mContactedEntitiesTriggers.begin()+i);
            delete ect;
        }
    }
    */

    //----
    //postUpdate();
    for(EntityContactSignal *ect : mContactedEntitiesTriggers){
        ect->contactNormalBtoA.SetZero();
        ect->contactPosition.SetZero();
        //ect->postUpdate();
    }

    //----
    //if(mNumContacted==0){
    //    reset();
    //}


}


EntityContactSignal * ContactTrigger::findEntityContactTriggerWithEntityB(Entity *_entityB)
{

    for(EntityContactSignal *ect : mContactedEntitiesTriggers){
        if(ect->mTwoEntitiesContact.entityShapeB().entity== _entityB){
            return ect;
        }
    }

    return nullptr;

}


EntityContactSignal * ContactTrigger::findEntityContactSignalWithEntityRoleA(EntityRole _entityRoleA)
{

    for(EntityContactSignal *ect : mContactedEntitiesTriggers){
        if(ect->mTwoEntitiesContact.entityShapeA().category->role == _entityRoleA){
            return ect;
        }
    }

    return nullptr;

}


//--------------------------------------------------------------------------------------------


FilteredContactSignal::FilteredContactSignal(ContactTrigger* _contactTrigger) : mContactTrigger(_contactTrigger)
{
    setId(static_cast<unsigned int>(SignalID::FILTERED_CONTACT_TRIGGER));
    categoriesBits = std::numeric_limits< unsigned int>::max();

}


void FilteredContactSignal::preUpdate_postContactTriggerUpdate()
{

    EntityContactSignal *filteredEntityContactTrigger = nullptr;

    for(EntityContactSignal *ect : mContactTrigger->contactedEntitiesTriggers()){

        //unsigned int bits = contactedEntity->sourceEntity()->sourceEntityCfg()->category->contactBits;
        //if((bits & categoriesBits)==0){
        //    continue;
        //}
        if(categoriesBits != 0){
            bool categoryFound = false;
            if(ect->mTwoEntitiesContact.dualFixtures().size()==2){
                DummyFunction();
            }
            for(TwoEntitiesContact::DualFixture &df : ect->mTwoEntitiesContact.dualFixtures()){

                b2Fixture * contactedFixture = df.fixtureB;
                FixtureUserData *fud = reinterpret_cast<FixtureUserData *>(contactedFixture->GetUserData().pointer);
                if(fud->category->contactBits & categoriesBits){
                    categoryFound = true;
                    break;
                }
            }
            if(categoryFound==false){
                continue;
            }
        }

        Entity * contactedEntity = ect->mTwoEntitiesContact.entityShapeB().entity;

        if(sourceEntities.empty()==false){
            bool sourceEntityFound = false;
            for(SourceEntity *se : sourceEntities){
                if(contactedEntity->sourceEntity()==se){
                    sourceEntityFound = true;
                    break;
                }
            }
            if(sourceEntityFound==false){
                continue;
            }
        }

        if(entities.empty()==false){
            bool entityFound = false;
            for(Entity *e : entities){
                if(contactedEntity==e){
                    entityFound = true;
                    break;
                }
            }
            if(entityFound==false){
                continue;
            }
        }

        filteredEntityContactTrigger = ect;
        break;
    }

    //---
    if(filteredEntityContactTrigger){
        //_setActive(filteredEntityContactTrigger->active());
        //_setActiveStarted(filteredEntityContactTrigger->activeStarted());
        //_setActiveEnded(filteredEntityContactTrigger->activeEnded());
        setValue(filteredEntityContactTrigger->active(true));

    }else{
        reset();
    }

}


void FilteredContactSignal::postUpdateSignals()
{

    postUpdate();

}




//==========================================================================



FilteredContactSignalsStorage::~FilteredContactSignalsStorage()
{

    for(FilteredContactSignal* fct : mFilteredContactSignals){
        delete fct;
    }
}



FilteredContactSignal* FilteredContactSignalsStorage::getNewFilteredContactSignal(ContactTrigger* _contactTrigger)
{

    mFilteredContactSignals.push_back(new FilteredContactSignal(_contactTrigger));
    return mFilteredContactSignals.back();

}



void FilteredContactSignalsStorage::preUpdate_postContactSignalsPreUpdate()
{

    for(FilteredContactSignal* fct : mFilteredContactSignals){
        fct->preUpdate_postContactTriggerUpdate();
    }

}


void FilteredContactSignalsStorage::postUpdate()
{

    for(FilteredContactSignal* fct : mFilteredContactSignals){
        fct->postUpdateSignals();
    }

}


//=================================================================================================


/*
int CustomEntityIntSignal::value(const std::string &valueName)
{

    for(const std::pair<std::string,int> &v : mValues){
        if(v.first==valueName){
            return v.second;
        }
    }

    return 0;
}




int CustomEntityIntBitsSignal::value(const std::string &valueName)
{

    for(const std::pair<std::string,int> &v : mValues){
        if(v.first==valueName){
            return v.second;
        }
    }

    return 0;
}
*/

//=================================================================================================


void EntityMovingGroup::setEntities(std::vector<Entity *> _members, Entity *_leader)
{
    mMembers = _members;
    mLeader = _leader;
    mLeader->statusFlagsRef() |= EntityStatusFlags::GROUP_LEADER;

    assert(StdVector::contains(mMembers, mLeader));

    for(Entity* e : mMembers){
        e->setEntityGroup(this);

    }

}


void EntityMovingGroup::setLeader(Entity *_entity)
{

    if(mLeader){
        mLeader->statusFlagsRef() &= ~ EntityStatusFlags::GROUP_LEADER;
    }

    mLeader = _entity;
    mLeader->statusFlagsRef() |= EntityStatusFlags::GROUP_LEADER;

    assert(StdVector::contains(mMembers, mLeader));
}


void EntityMovingGroup::addMember(Entity* _entity)
{

    mMembers.push_back(_entity);
    _entity->setEntityGroup(this);

}


void EntityMovingGroup::removeEntity(Entity* _entity)
{

    bool removed = StdVector::removeElement(mMembers, _entity);
    assert(removed);

    if(_entity == mLeader){
        mLeader = nullptr;
    }

}


//=================================================================================================


EntityGroupsManager::~EntityGroupsManager()
{

    for(EntityMovingGroup* eg : mEntityGroups){
        delete  eg;
    }

}


EntityMovingGroup* EntityGroupsManager::storeNewEntityGroup(EntityMovingGroup* _entityGroup)
{

    mEntityGroups.push_back(_entityGroup);
    _entityGroup->_setParentManager(this);
    return _entityGroup;

}


void EntityGroupsManager::deleteEntityGroup(EntityMovingGroup*_entityGroup)
{

    bool removed = StdVector::removeElement(mEntityGroups, _entityGroup);
    assert(removed);

    for(Entity* e : _entityGroup->members()){
        e->setEntityGroup(nullptr);
        e->statusFlagsRef() &= ~ EntityStatusFlags::GROUP_LEADER;
    }

    delete _entityGroup;

}



//=================================================================================================


bool EntityCategoriesGroup::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading entity categories ...");


    for(pugi::xml_node nChild = _node.first_child(); nChild; nChild = nChild.next_sibling()){
        std::string childNodeName = std::string(nChild.name());

        if(childNodeName=="mainCategory" || childNodeName=="extraCategory"){

            EntityCategory::ShapeRole shape = EntityCategory::ShapeRole::UNKNOWN;
            if(childNodeName=="mainCategory"){
                shape = EntityCategory::ShapeRole::MAIN_SHAPE;
            }else if(childNodeName=="extraCategory"){
                shape = EntityCategory::ShapeRole::EXTRA_SHAPE;
            }
            std::string name = nChild.attribute("name").as_string("");
            std::string sRole = nChild.attribute("role").as_string("");
            EntityRole role = GetEntityRoleFromString(sRole);

            if(role==EntityRole::UNKNOWN){
                dbgSystem.addMessage("Wrong 'body' parameter: '" + sRole + "' !");
                return false;
            }

            if(addEntityCategory(name, role, shape)==false){
                return false;
            }

            //int id = nChild.attribute("id").as_int(0);
            //if(id>=0 && id<int(contactCategories.mCollisionCategoryNames.size())){
            //    contactCategories.mCollisionCategoryNames[id] = nChild.attribute("name").as_string("");
            //}

        }else if(childNodeName=="contactFilter"){

            std::string category = nChild.attribute("category").as_string("");
            EntityCategory *ec = getEntityCategory(category);
            if(ec==nullptr){
                return false;
            }

            std::string contacts = nChild.attribute("contacts").as_string("");
            if(contacts.empty()){
                ec->contactMask = allContactsBits();

            }else{

                ec->contactMask = 0;

                std::vector<std::string> vContacts = StdString::splitString(contacts,",");

                for(std::string &categoryName : vContacts){
                    EntityCategory *ecContact = getEntityCategory(categoryName);
                    if(ecContact==nullptr){
                        return false;
                    }
                    if(ecContact==ec){
                        continue;
                    }

                    ec->contactMask |= ecContact->contactBits;
                }

            }

            std::string skipContacts = nChild.attribute("skipContacts").as_string("");
            if(skipContacts.empty()==false){

                ec->contactMask = allContactsBits();

                std::vector<std::string> vSkipContacts = StdString::splitString(skipContacts,",");

                for(std::string &categoryName : vSkipContacts){
                    EntityCategory *ecSkipContact = getEntityCategory(categoryName);
                    if(ecSkipContact==nullptr){
                        return false;
                    }
                    //if(ecSkipContact==ec){
                    //    continue;
                    //}

                    ec->contactMask &= ~ecSkipContact->contactBits;
                }
            }

        }else{

            dbgSystem.addMessage("Unknown node '" + childNodeName + "' !");
            return false;

        }
    }


    dbgSystem.removeLastMessage();

    return true;


}



bool EntityCategoriesGroup::addEntityCategory(const std::string &_name, EntityRole _role, EntityCategory::ShapeRole _shapeRole)
{

    if(mEntityCategories.size() > mCountMax-1){
        dbgSystem.addMessage("Exceeded maximum number of entity categories ( " + std::to_string(mCountMax) +")!");
        return false;
    }

    mEntityCategories.emplace_back(EntityCategory());
    EntityCategory &ec = mEntityCategories.back();

    ec.name = _name;
    ec.shapeRole = _shapeRole;
    ec.role = _role;


    //----
    if(_role==EntityRole::ACTOR){
        ec.sensor = false;
        ec.mB2BodyType = b2BodyType::b2_dynamicBody;

    }else if(_role==EntityRole::BLOCKING_OBJECT){
        ec.sensor = false;
        ec.mB2BodyType = b2BodyType::b2_staticBody;

    }else if(_role==EntityRole::BLOCKING_ENVIRONMENT){
        ec.sensor = false;
        ec.mB2BodyType = b2BodyType::b2_staticBody;

    }else if(_role==EntityRole::MOVABLE_OBJECT){
        ec.sensor = false;
        ec.mB2BodyType = b2BodyType::b2_dynamicBody;

    }else if(_role==EntityRole::TRANSPORTER){
        ec.sensor = false;
        ec.mB2BodyType = b2BodyType::b2_kinematicBody;

    }else if(_role==EntityRole::PASSABLE_OBJECT){
        ec.sensor = true;
        ec.mB2BodyType = b2BodyType::b2_staticBody;

    }else if(_role==EntityRole::PASSABLE_AREA){
        ec.sensor = true;
        ec.mB2BodyType = b2BodyType::b2_staticBody;

    }else if(_role==EntityRole::PATHWAY){
        ec.sensor = true;
        ec.mB2BodyType = b2BodyType::b2_staticBody;

    }else if(_role==EntityRole::GROUND_SENSOR){
        ec.sensor = true;
        ec.mB2BodyType = b2BodyType::b2_dynamicBody;

    }else if(_role==EntityRole::CEILING_SENSOR){
        ec.sensor = true;
        ec.mB2BodyType = b2BodyType::b2_dynamicBody;

    }else if(_role==EntityRole::CUSTOM_SENSOR){
        ec.sensor = true;
        ec.mB2BodyType = b2BodyType::b2_dynamicBody;

    }else if(_role==EntityRole::OBJECT_WITHOUT_SHAPE){

    }else{
        assert(false);
    }


    if(ec.mB2BodyType==b2BodyType::b2_staticBody){
        ec.updateOrder = 1;
    }else if(ec.mB2BodyType==b2BodyType::b2_kinematicBody){
        ec.updateOrder = 2;
    }else if(ec.mB2BodyType==b2BodyType::b2_dynamicBody){
        ec.updateOrder = 3;
    }


    //---
    ec.contactBits = (unsigned int)1 << (mEntityCategories.size()-1);
    ec.contactMask = allContactsBits();

    return true;

}


EntityCategory *EntityCategoriesGroup::getEntityCategory(const std::string &_name, bool _setErrorMessage)
{

    for(EntityCategory &ec : mEntityCategories){
        if(ec.name == _name){
            return &ec;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("An entity category '" + _name + "' not found!");
    }

    return nullptr;

}


EntityCategory *EntityCategoriesGroup::getEntityCategory(EntityRole _entityBodyKind, bool _setErrorMessage)
{

    for(EntityCategory &ec : mEntityCategories){
        if(ec.role == _entityBodyKind){
            return &ec;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("An entity category with body kind'" + GetEntityRoleString(_entityBodyKind) + "' not found!");
    }

    return nullptr;

}



/*

int EntityCategoriesGroup::getCollisionCategoryBitIndex(const std::string &_name, bool _setErrorMessage)
{


    for(int i=1; i< mEntityCategories.size(); i++){
        if(mEntityCategories[i] == _name){
            return i;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A collision category '" + _name + "' not found!");
    }

    return -1;

}



int EntityCategoriesGroup::getContactCategoryBit(const std::string &_name, bool _setErrorMessage)
{


    for(int i=1; i< mContactCategoryNames.size(); i++){
        if(mContactCategoryNames[i] == _name){
            return i;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("A contact category '" + _name + "' not found!");
    }

    return -1;

}

*/


//--------------------------------------------------------------------------------------------



void EDirection::setCardinalDirection(Direction _direction)
{


    if(_direction==Direction::UP){


    }



}



void EDirection::setDirection(b2Vec2 _dirVector)
{

}



void EDirection::setOpposingDirection()
{


}

}



