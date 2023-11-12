#ifndef PLAYER__ENTITIES__SOURCE_ENTITIES__H
#define PLAYER__ENTITIES__SOURCE_ENTITIES__H

#include <memory>
#include <string>
#include <vector>
#include "box2d.h"
#include "jmCommon.h"
#include "jpEntityGlobal.h"
#include "jpEntityLogicState.h"


namespace pugi{
class xml_node;
}


namespace jugimap{



class PlayedScene;
class EntitySystem;
class SourceSprite;
class SourceBody;
class SpriteLayer;
class FixtureUserDataGroup;
struct EntityCategory;
struct EntityLogicStateCfg;
//struct EnginesControllerCfg;
//struct TaskControllerCfg;
//struct EnvironmentObjectBehaviorCfg;


//-------------------------------------------------------------------------------

struct ActorPathwayContactPointOffset
{
    std::string actorName;
    b2Vec2 offsetM{0.0f, 0.0f};
};



struct PathwaySourceEntityCfg
{

    bool initCfg(const pugi::xml_node &_node);

    b2Vec2 getContactPointOffsetForActor(const std::string &_actorName);


    PathwayOrientation orientation = PathwayOrientation::VERTICAL;
    //float centeringMarginM = 1.0f;
    CenteringStyle centeringStyle = CenteringStyle::NONE;

    std::vector<ActorPathwayContactPointOffset>actorsPathwayContactPointOffsets;

};






//-------------------------------------------------------------------------------


struct SourceEntityCfg
{

    //SourceEntityCfg(EntityType _type, const std::string &_name, EntityBodyType _bodyType) : type(_type), bodyType(_bodyType), name(_name){}
    //~SourceEntityCfg();

    SourceEntityCfg(const std::string &_name, EntityCategory * _entityCategory) :
        name(_name),
        category(_entityCategory)
    {}

    bool initCfg(const pugi::xml_node &_node);


    std::string name;
    EntityCategory *category = nullptr;
    //EntityBodyProperties entityBodyProperties;
    EnumEntityMapElement entityMapElement = EnumEntityMapElement::UNKNOWN;



    //EntityType type = EntityType::UNKNOWN;
    //EntityBodyType bodyType = EntityBodyType::SOLID;

    EntityLogicStateCfg *enginesControllerCfg = nullptr;  //LINK
    std::string startState;
    //bool ownedEnginesControllerCfg = false;

    EntityLogicStateCfg *taskControllerCfg = nullptr;               // LINK
    std::string taskHandlerStartState;

    //----
    std::unique_ptr<PathwaySourceEntityCfg>mPathwaySourceEntityCfg;

    //----
    //std::unique_ptr<EnginesControllerCfg>ownedEnginesControllerCfg;
    std::vector<CustomSensorSourceEntityCfg>mCustomSensorSourceEntityCfgs;

    //----
    bool passableFromBellow = false;
    Margin centeringMargin;

    //----
    //unsigned int collisionCategoryBits = 0;

    bool fixedRotation = true;

    EntitySystem * entitySystem = nullptr;  // LINK

};


//-------------------------------------------------------------------------------


/*
class SourceEntitiesCfgsGroup
{
public:

    SourceEntityCfg* getSourceEntitiyCfg(const std::string &_name, bool _setErrorMessage = true);
    std::vector<SourceEntityCfg> &sourceEntitiesCfgs(){ return mSourceEntitiesCfgs; }

private:
    std::vector<SourceEntityCfg>mSourceEntitiesCfgs;

};
*/

//-------------------------------------------------------------------------------


class SourceEntity
{
public:


    SourceEntity(SourceEntityCfg *_sourceEntityCfg);
    bool createSourceBody(PlayedScene *_scene, SourceSprite *_sourceSprite, SpriteLayer *_spriteLayer);

    //EntityType type() const { return mSourceEntityCfg->type; }
    EntityRole entityBodyKind(); //{ return mSourceEntityCfg->category;}
    const std::string & name(){ return mSourceEntityCfg->name; }
    SourceEntityCfg* sourceEntityCfg() const { return mSourceEntityCfg; }
    SourceSprite * sourceSprite() const { return mSourceSprite; }
    SourceBody* sourceBody() const { return mSourceBody.get(); }


private:
    //EntityType mBodyRole = EntityType::NOT_DEFINED;

    SourceEntityCfg * mSourceEntityCfg = nullptr;       // LINK
    SourceSprite * mSourceSprite = nullptr;             // LINK

    std::unique_ptr<SourceBody>mSourceBody;
};



//-------------------------------------------------------------------------------


class SourceEntitiesGroup
{
public:

    SourceEntitiesGroup();
    ~SourceEntitiesGroup();

    bool create(PlayedScene *_scene);

    SourceEntity* getSourceEntitiy(const std::string &_name, bool _setErrorMessage = true);
    std::vector<SourceEntity> &sourceEntities(){ return mSourceEntities; }

    SourceEntityCfg* getSourceEntitiyCfg(const std::string &_name, bool _setErrorMessage = true);
    std::vector<SourceEntityCfg> &sourceEntitiesCfgs(){ return mSourceEntitiesCfgs; }


private:
    std::vector<SourceEntity>mSourceEntities;
    std::vector<SourceEntityCfg>mSourceEntitiesCfgs;
    //std::unique_ptr<FixtureUserDataGroup> mFixtureUserDataGroup;

    bool createSingleSpriteSourceEntities(PlayedScene *_scene);
    bool createSpriteLayerSourceEntities(PlayedScene *_scene);
    bool createVectorShapeSourceEntities(PlayedScene *_scene);
    //void createSourceEntitiesCfgsForSpriteLayerEntities(PlayedScene *_scene, EntityBodyKind _entityType);


};


//-------------------------------------------------------------------------------




}






#endif
