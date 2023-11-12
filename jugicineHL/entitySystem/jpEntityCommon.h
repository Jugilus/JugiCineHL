#ifndef PLAYER__ENTITY__ENTITY_COMMON__H
#define PLAYER__ENTITY__ENTITY_COMMON__H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <limits>

#include "jmCommon.h"

#include "jpEntityGlobal.h"


namespace pugi{
class xml_node;

}


namespace jugimap{


class WorldMapCamera;
class PlayedScene;
class Entity;
class MovementEngineData;
class SourceEntity;
class EntityGroupsManager;



class WorldCameraManager
{

public:

    WorldCameraManager();

    bool initConnections(PlayedScene *_scene);
    void setFocusEntity(Entity *_entity);
    void start();
    void update();

    void setPointedPosition(Vec2f worldPos);

    WorldMapCamera *camera(){ return mCamera;}


private:
    WorldMapCamera *mCamera = nullptr;          // LINK
    Entity *mFocusEntity = nullptr;             // LINK

    //Vec2f prevPos;

};



//------------------------------------------------------------------------


class FixtureGroup
{
public:

    //void setEntity(Entity *_entity){ mEntity =_entity; }

    bool containsFixture(b2Fixture *_fixture, int _childIndex);
    void addFixture(b2Fixture *_fixture, int _childIndex);
    int removeFixture(b2Fixture *_fixture, int _childIndex);

    //Entity* entity(){ return mEntity; }
    std::vector<std::pair<b2Fixture*,int>> & fixtures(){ return mFixtures; }

private:

    //Entity* mEntity = nullptr;       // LINK
    std::vector<std::pair<b2Fixture*,int>>mFixtures;

};

//------------------------------------------------------------------------

struct EntityShape
{
    Entity * entity = nullptr;                  // LINK
    EntityCategory * category = nullptr;        // LINK

    EntityShape(Entity* _entity, EntityCategory* _category) : entity(_entity), category(_category){}
    bool operator==(const EntityShape &other){ return entity==other.entity && category==other.category; }
    bool operator!=(const EntityShape &other){ return entity!=other.entity || category!=other.category; }

};



class TwoEntitiesContact
{
public:

    struct DualFixture
    {

        DualFixture(b2Fixture *_fixtureA, int _childIndexA, b2Fixture* _fixtureB, int _childIndexB) :
                    fixtureA(_fixtureA),
                    childIndexA(_childIndexA),
                    fixtureB(_fixtureB),
                    childIndexB(_childIndexB)
        {}

        b2Fixture * fixtureA = nullptr;   // LINK
        int childIndexA = -1;

        b2Fixture * fixtureB = nullptr;   // LINK
        int childIndexB = -1;
    };


    TwoEntitiesContact(const EntityShape &_entityShapeA, const EntityShape &_entityShapeB) : mEntityShapeA(_entityShapeA), mEntityShapeB(_entityShapeB){}

    void onBeginContact(b2Fixture *_fixtureA, int _childIndexA, b2Fixture *_fixtureB, int _childIndexB);
    void onEndContact(b2Fixture *_fixtureA, int _childIndexA, b2Fixture *_fixtureB, int _childIndexB);

    //Entity* entityA(){ return mEntityA; }
    //Entity* entityB(){ return mEntityB; }

    EntityShape & entityShapeA(){ return mEntityShapeA;}
    EntityShape & entityShapeB(){ return mEntityShapeB;}


    //bool hasContactedFixtures();


    std::vector<DualFixture> & dualFixtures(){ return mDualFixtures; }

private:

    //Entity* mEntityA = nullptr;       // LINK
    //Entity* mEntityB = nullptr;       // LINK

    EntityShape mEntityShapeA;
    EntityShape mEntityShapeB;

    std::vector<DualFixture>mDualFixtures;
    //FixtureGroup mFixturesA;
    //FixtureGroup mFixturesB;


};


struct EntityContactSignal : public BoolSignal
{

    //enum class Status
    //{
    //    PENDING,
    //    ACTIVE,
    //    CONTACT_ENDED
    //};

    EntityContactSignal(const EntityShape &_entityShapeA, const EntityShape &_entityShapeB);


    TwoEntitiesContact mTwoEntitiesContact;

    //bool mDeleteThis = false;

    //---
    bool storedSignalValues = false;
   // bool activeStored = false;
    //bool activeStartedStored = false;
    //bool activeEndedStored = false;
    bool valueStored = false;
    bool previousValueStored = false;
    bool valueChangedStored = false;

    //---

    b2Vec2 contactNormalBtoA{0.0f, 0.0f};
    b2Vec2 contactPosition{0.0f, 0.0f};

    std::string dbgName;


};



struct ContactTrigger : public BoolSignal
{

    //void setContactingEntityOnBegin(Entity *_entity, b2Fixture* _fixture, int _childIndex);
    //void setContactingEntityOnEnd(Entity *_entity, b2Fixture* _fixture, int _childIndex);

    ContactTrigger();

    void onBeginContact(const EntityShape &_entityShapeA, b2Fixture *_fixtureA, int _childIndexA, const EntityShape &_entityShapeB, b2Fixture *_fixtureB, int _childIndexB);
    void onEndConctact(const EntityShape &_entityShapeA, b2Fixture *_fixtureA, int _childIndexA, const EntityShape &_entityShapeB, b2Fixture *_fixtureB, int _childIndexB);

    void preUpdate_postBox2dStep();
    void postUpdateSignals();


    //Entity* mEntity = nullptr;       // LINK
    //std::vector<std::pair<b2Fixture*,int>>mFixtures;

    EntityContactSignal *findEntityContactTriggerWithEntityB(Entity* _entityB);
    EntityContactSignal *findEntityContactTriggerWithEntityRoleB(EntityRole _entityRoleB);
    EntityContactSignal *findEntityContactSignalWithEntityRoleA(EntityRole _entityRoleA);
    std::vector<EntityContactSignal*> &contactedEntitiesTriggers(){ return mContactedEntitiesTriggers; }





private:
    //---
    std::vector<EntityContactSignal*>mContactedEntitiesTriggers;
    int mNumContacted = 0;

};



struct FilteredContactSignal : public BoolSignal
{

    FilteredContactSignal(ContactTrigger* _contactTrigger);

    void setCategoriesBits(unsigned int _bits){ categoriesBits = _bits;}
    void addSourceEntity(SourceEntity *_sourceEntity){ sourceEntities.push_back(_sourceEntity); }
    void addEntity(Entity *_entity){ entities.push_back(_entity); }

    void setSourceEntities(std::vector<SourceEntity*> &_sourceEntities){ sourceEntities = _sourceEntities;}
    void setEntities(std::vector<Entity*> & _entities){ entities = _entities; }

    void preUpdate_postContactTriggerUpdate();
    void postUpdateSignals();


private:
    ContactTrigger* mContactTrigger = nullptr;          // LINK

    //---
    unsigned int categoriesBits = 0;
    std::vector<SourceEntity*> sourceEntities;          // LINKS
    std::vector<Entity*> entities;                      // LINKS (only entities with groupID or identifierID)

};



//==========================================================================





/*

class CustomEntityIntSignal : public IntSignal
{
public:

    CustomEntityIntSignal() : IntSignal(static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){}
    virtual void update() = 0;

    int value(const std::string &valueName);

//private:
    //std::map<std::string, int>mValues;

};



class CustomEntityIntBitsSignal : public BitsetSignal
{
public:

    CustomEntityIntBitsSignal() : BitsetSignal(static_cast<unsigned char>(SignalID::CUSTOM_ENTITY_SIGNAL)){}
    virtual void update() = 0;

    int value(const std::string &valueName);

//private:
    //std::map<std::string, int>mValues;

};

*/

//==========================================================================



class FilteredContactSignalsStorage
{
public:


    ~FilteredContactSignalsStorage();
    FilteredContactSignal* getNewFilteredContactSignal(ContactTrigger* _contactTrigger);

    void preUpdate_postContactSignalsPreUpdate();
    void postUpdate();


private:

    std::vector<FilteredContactSignal*>mFilteredContactSignals;


};








//=====================================================================================

enum class EntityGroupType
{
    UNKNOWN,
    MOVING_MOVABLE_OBJECT
};




class EntityMovingGroup
{
public:


    virtual ~EntityMovingGroup(){}

    EntityMovingGroup(EntityGroupType _type) : mType(_type){}

    EntityGroupType type(){ return mType;}

    void setEntities(std::vector<Entity*>_members, Entity* _leader);

    Entity* leader(){ return mLeader; }
    void setLeader(Entity *_entity);
    void addMember(Entity* _entity);
    void removeEntity(Entity* _entity);

    std::vector<Entity*>& members(){ return mMembers;}

    //virtual void preUpdate(UpdateMode _updateMode){}
    virtual void update_Movement(UpdateMode &_updateMode) = 0;
    //virtual void postUpdate(UpdateMode _updateMode);

    bool membersCollision(){ return mMembersCollision;}

    void _setParentManager(EntityGroupsManager *_parentManager){ mParentGroupsManager = _parentManager; }

protected:
    EntityGroupType mType = EntityGroupType::UNKNOWN;
    Entity *mLeader = nullptr;
    std::vector<Entity*>mMembers;     // links

    bool mMembersCollision = false;

    EntityGroupsManager *mParentGroupsManager = nullptr;    //LINK

};



class EntityGroupsManager
{
public:

    ~EntityGroupsManager();

    EntityMovingGroup* storeNewEntityGroup(EntityMovingGroup* _entityGroup);
    void deleteEntityGroup(EntityMovingGroup* _entityGroup);

    std::vector<EntityMovingGroup*> &entityMovingGroups(){ return mEntityGroups; }


private:
    std::vector<EntityMovingGroup*>mEntityGroups;     // OWNED

};




class EntityCategoriesGroup
{
public:

    bool initCfg(const pugi::xml_node &_node);


    bool addEntityCategory(const std::string &_name, EntityRole _role, EntityCategory::ShapeRole _shapeRole);
    EntityCategory * getEntityCategory(const std::string &_name, bool _setErrorMessage=true);
    EntityCategory * getEntityCategory(EntityRole _entityBodyKind, bool _setErrorMessage=true);

    std::vector<EntityCategory>& entityCategories(){ return mEntityCategories; }

    unsigned int allContactsBits(){ return std::numeric_limits< unsigned int>::max();}

    //int getCollisionCategoryBitIndex(const std::string &_name, bool _setErrorMessage=true);
    //int getContactCategoryBit(const std::string &_name, bool _setErrorMessage=true);

private:

    int mCountMax = 32;     //
    std::vector<EntityCategory>mEntityCategories;         // 1 - 32


};



//------------------------------------------------------------------------


class EDirection
{
public:

    enum class Mode
    {
        CARDINAL,
        PATH,
        ANGLE_BASED
    };


    Direction direction(){ return mDirection; }
    void setCardinalDirection(Direction _direction);
    void setDirection(b2Vec2 _dirVector);

    void setOpposingDirection();


private:
    Mode mMode = Mode::CARDINAL;
    Direction mDirection = Direction::NONE;
    b2Vec2 mDirVector{0.0f, 0.0f};

};





}






#endif
