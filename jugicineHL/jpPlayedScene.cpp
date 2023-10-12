#include <chrono>
#include <assert.h>
#include <sstream>

#include "jmCommonFunctions.h"
#include "jpComponentsCommon.h"
#include "jpGfxObjectsCommon.h"
#include "items/jpItemsCommon.h"
#include "actions/jpActionsCommon.h"
#include "actions/jpTransition.h"
#include "gfxObjects/jpAnimationObjects.h"

#include "jpSystemVarious.h"
#include "jpPlayedScene.h"



namespace jugimap{



using namespace jugimap;



PlayedScene::PlayedScene(const std::string &_name, App *_parentApp) : Scene(_name, _parentApp)
{

    BaseObject::mType = BaseObjectType::PLAYER_SCENE;

    mInitState.reset(new LogicState("_onInitState", this));
    mStartState.reset(new LogicState("_onStartState", this));
    mUpdateState.reset(new LogicState("_onUpdateState", this));

    mItemsLibrary.reset(new ItemsLibrary());
    mTransitionsGroup.reset(new TransitionsGroup());
    mComponentsGroup.reset(new ComponentsGroup());
    mOverlayComponentsGroup.reset(new ComponentsGroup());
    mGfxObjectsGroup.reset(new GfxObjectsGroup());
    mWidgetManager = new ExtWidgetManager();

    mAnimationManager.reset(new AnimationManager());

    //mEntitySystem.reset(new EntitySystem());
    //mSourceEntitiesGroup.reset(new SourceEntitiesGroup());


}


PlayedScene::~PlayedScene()
{

    //if(mInitState){
    //    delete mInitState;
    //}

    //if(mStartState){
    //    delete mStartState;
    //}

    //if(mUpdateState){
    //    delete  mUpdateState;
    //}

    //if(mItemsLibrary){
    //    delete mItemsLibrary;
    //}

    //if(mTransitionsGroup){
    //    delete mTransitionsGroup;
    //}

    //if(mComponentsGroup){
    //    delete mComponentsGroup;
    //}

    //if(mOverlayComponentsGroup){
    //    delete mOverlayComponentsGroup;
   // }

    //if(mGfxObjectsGroup){
    //    delete mGfxObjectsGroup;
    //}

    //mVariables.deleteContent();

}


void PlayedScene::deleteContent()
{

    mInitState.reset(new LogicState("_onInitState", this));
    mStartState.reset(new LogicState("_onStartState", this));
    mUpdateState.reset(new LogicState("_onUpdateState", this));

    mVariables.deleteContent();
    mTriggers.deleteContent();
    mRunningLongActions.clear();

    Scene::deleteContent();
}



// PlayerScene::init is called after the scene has been build with Scene::build
bool PlayedScene::init()
{

    //Scene::Init();

    //bool initialized = false;

    //if(mCustomScene){
    //    initialized = mCustomScene->Init();
    //}

    //return initialized;


    mUpdateWidgetsOnPreUpdate = false;


    if(mAnimationManager->create(this)==false){
        return false;
    }

    if(initActionTasksAndEvents()==false){
        return false;
    }


    mInitState->update(UpdateMode::NORMAL);

    return true;
}


// initActionTasksAndEvents() is called AFTER the maps have been loaded and initialized
bool PlayedScene::initActionTasksAndEvents()
{

    // the order of initialization is important
    //----
    if(mGfxObjectsGroup->initConnections(this)==false){
        return false;
    }

    //if(mSourceEntitiesGroup->create(this)==false){
    //    return false;
    //}


    //---- first initialize items as they can set parameters used by logic
    if(mItemsLibrary->initItems(this)==false){
        return false;
    }

    if(mTransitionsGroup->init(this)==false){
        return false;
    }


    //---- components should be initialized last
    if(mOverlayComponentsGroup->initConnections(this)==false){
        return false;
    }

    if(mComponentsGroup->initConnections(this)==false){
        return false;
    }


    if(mAnimationManager->initConnections(this)==false){
        return false;
    }


    //----
    if(mInitState->initConnections(this)==false){
        return false;
    }

    if(mStartState->initConnections(this)==false){
        return false;
    }

    if(mUpdateState->initConnections(this)==false){
        return false;
    }


    //if(mEntitySystem->createSourceEntities(this)==false){
    //    return false;
    //}

    //if(mEntitySystem->createMapEntities(this)==false){
    //    return false;
    //}
    //if(mEntitySystem->initConections(this)==false){
    //    return false;
   // }

    return true;

}


bool PlayedScene::updateLogicData(PlayedScene *newPSData)
{


    // we replace current logic states with new one
    // before that we try to sync the new active states with current one

    //for(LogicState *ls : mUpdateState->childStates()){
    //    LogicState* lsReplacement = newPSData->mUpdateState->getChildState(ls->name());
   //     if(lsReplacement){
    //        ls->syncReplacementActiveState(lsReplacement);
   //     }
   // }
    mUpdateState->syncReplacementActiveState(newPSData->mUpdateState.get());


    //delete mInitState;
    //mInitState = newPSData->mInitState;
    mInitState.reset(newPSData->mInitState.get());
    newPSData->mInitState.reset(nullptr);

    //delete mStartState;
    mStartState.reset(newPSData->mStartState.get());
    newPSData->mStartState.reset(nullptr);

    //delete mUpdateState;
    mUpdateState.reset(newPSData->mUpdateState.get());
    newPSData->mUpdateState.reset(nullptr);


    //mInitState->updateLogicData(newPSData->initState());

    //mStartState->updateLogicData(newPSData->startState());

   // mUpdateState->updateLogicData(newPSData->updateState());

    //if(init()==false){
    //    return false;
    //}


    return true;
}


void PlayedScene::start()
{

    mStartState->update(UpdateMode::NORMAL);

}


bool PlayedScene::startingPhaseUpdate()
{

    return mUpdateState->startingPhaseUpdate();

}


void PlayedScene::preUpdate()
{

    Scene::preUpdate();

    mAnimationManager->preUpdate();

    //---
    mActivatedTriggersUpdater.update();


}


void PlayedScene::update()
{

    //LogicState::longActionActive = mUpdateState->hasLongActionActive();
    //if(LogicState::longActionActive){
    //    print("longActionActive");
    //}

    UpdateMode updateMode = UpdateMode::NORMAL;
    if(mModalOverlayComponents){
        updateMode = UpdateMode::MODAL_OVERLAY;
    }

    mUpdateState->update(updateMode);

    mAnimationManager->update();


    for(Component* c : mRunningOverlayComponents){
        c->update(updateMode);
    }

}


void PlayedScene::postUpdate()
{
    Scene::postUpdate();
    mAnimationManager->postUpdate();

    //---
    mActivatedTriggersUpdater.resetPerUpdateFlags();

}


void PlayedScene::startOverlayComponent(OverlayComponent *_component)
{

    mRunningOverlayComponents.push_back(_component);
    _component->start();

    //---
    mModalOverlayComponents = false;
    for(OverlayComponent* c : mRunningOverlayComponents){
        if(c->modal()){
            mModalOverlayComponents = true;
            break;
        }
    }

}


void PlayedScene::endOverlayComponent(OverlayComponent *_component)
{

    StdVector::removeElement(mRunningOverlayComponents, _component);
    _component->onStateEnded();

    //---
    mModalOverlayComponents = false;
    for(OverlayComponent* c : mRunningOverlayComponents){
        if(c->modal()){
            mModalOverlayComponents = true;
            break;
        }
    }

}


void PlayedScene::addLongRunningAction(BaseObject *_action)
{

    if(StdVector::contains(mRunningLongActions, _action)==false){
        mRunningLongActions.push_back(_action);
    }

    mWidgetManager->setWidgetsActive(mRunningLongActions.empty());

}


void PlayedScene::removeLongRunningAction(BaseObject* _action)
{

    StdVector::removeElement(mRunningLongActions, _action);

    mWidgetManager->setWidgetsActive(mRunningLongActions.empty());

}


void PlayedScene::drawShapes(jugimap::ShapeDrawer &_drawer, int _flags)
{

    mUpdateState->drawShapes(_drawer, _flags);

}








}
