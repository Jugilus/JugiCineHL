#ifndef PLAYER__ENTITY__ENTITY_PHASE__H
#define PLAYER__ENTITY__ENTITY_PHASE__H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <limits>

#include "jmAnimationCommon.h"
#include "jmCommon.h"
#include "jmStorage.h"
#include "jpQueries.h"
#include "jpEntityGlobal.h"


namespace pugi{
class xml_node;

}


namespace jugimap{


class PlayedScene;
class EntityPhasesHandler;
class EntityPhase;
class SpriteLayer;

class GfxObject;
class AniSpriteObject;

class SourceEntity;
class SpawnerAndSpawned;


/*
 *
enum class EntityPhaseType
{
    UNKNOWN,
    START,
    RUNNING,
    END

};




struct EntityPhaseCfg
{

    EntityPhaseCfg(EntityPhaseType _type) : mType(_type){}
    bool initCfg(const pugi::xml_node &_node);


    EntityPhaseType mType = EntityPhaseType::UNKNOWN;
    std::string name;
    float alphaFadeDurationS = 0.0f;
    std::vector<std::string>mAniSpriteObjects;

};


class EntityPhase : public BaseObject
{
public:

    ~EntityPhase();

    bool build(PlayedScene *_scene, EntityPhaseCfg *_cfg, EntityPhasesHandler *_parent);
    bool initConnections(PlayedScene *_scene);

    void start(Vec2f _pos);
    void update();
    bool areAnimationsStalled();
    bool isFinished();



    EntityPhaseType type(){ return mCfg->mType; }
    EntityPhaseCfg * cfg(){ return mCfg; }
    Tween & tween(){ return mTween;}
    ItemData * findData(const std::string &_name, bool _setErrorMessage=true);


private:
    EntityPhaseCfg * mCfg = nullptr;
    EntityPhasesHandler *mParentObject = nullptr;
    std::vector<AniSpriteObject*>mAniSpriteObjects;         // OWNED
    Tween mTween;

};


//-------------------------------------------------------------------------------------------


struct EntityPhasesHandlerCfg
{

    EntityPhasesHandlerCfg(const std::string &_name) : mName(_name){}
    bool initCfg(const pugi::xml_node &_node);


    const std::string & name(){ return mName; }


    std::string mName;
    std::vector<EntityPhaseCfg>mPhaseCfgs;

};


class EntityPhasesHandler
{
public:

    bool build(PlayedScene *_scene, EntityPhasesHandlerCfg *_cfg);
    bool initConnections(PlayedScene *_scene);

    const std::string & name(){ return mCfg->name(); }
    EntityPhase *entityPhase(EntityPhaseType _type);

private:
    EntityPhasesHandlerCfg *mCfg = nullptr;
    std::vector<EntityPhase>mEntityPhases;

};

*/

//-------------------------------------------------------------------------------------------


class EntityLifespanHandler
{
public:

    void addEntityForErase(Entity *_entity);
    void addSourceEntityForSpawning(SpawnerAndSpawned * _sas);

    void update();

    void releaseEntityFromPool(Entity *_entity);
    void removeEntityFromWorld(Entity *_entity);


private:
    std::vector<Entity*>mEntitiesForErase;
    std::vector<std::pair<Entity*, EntityPhase*>>mEntitiesWithEndPhase;

    std::vector<std::pair<Entity*, Item*>>mEntitiesWithVanishEffect;

    std::vector<SpawnerAndSpawned*>mSourceEntitiesForSpawning;      // LINKS
    //ObjectSignal mSigRemoveEntity;
    //SignalStorage mSignalStorage;
    //SignalIdentifiers mSignalIdentifiers;

    void update_erasingEntities();
    void update_spawningEntities();

};




}






#endif
