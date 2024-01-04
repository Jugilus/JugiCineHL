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

#include "jpEntityManager.h"




namespace jugimap{


/*

void EntityManager::spawnEntity(SourceEntity* _sourceEntity)
{

    Entity *entity = _sourceEntity->entityPool().acquireObject();
    entity->addToWorld();
    //mEntities.push_back(entity);

}


void EntityManager::removeEntity(Entity* _entity)
{

    //StdVector::removeElement(mEntities, _entity);
    _entity->removeFromWorld();
    _entity->sourceEntity()->entityPool().releaseObject(_entity);

}


void EntityManager::transferItem(Entity *_srcEntity, Item *_srcItem, Entity *_dstEntity, Item *_dstItem)
{


}

*/



}













