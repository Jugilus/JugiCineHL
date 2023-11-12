#include <chrono>
#include <assert.h>
#include <sstream>

#include "jmCommonFunctions.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmScene.h"
#include "jmSceneLayout.h"

#include "jpSceneLogicState.h"
#include "jpComponentsCommon.h"
#include "jpGfxObjectsCommon.h"
#include "items/jpItemsCommon.h"
#include "logic/jpLogicState.h"
#include "logic/jpTransition.h"
#include "gfxObjects/jpAnimationObjects.h"
#include "gfxObjects/jpCompound.h"

#include "gui/jpGuiCommon.h"
#include "gui/jpGuiSystem.h"

#include "jpSystemVarious.h"
#include "jpPlayedScene.h"



namespace jugimap{



using namespace jugimap;



PlayedScene::PlayedScene(const std::string &_name, App *_parentApp) : Scene(_name, _parentApp)
{

    BaseObject::mBaseType = BaseObjectType::PLAYER_SCENE;

    mInitStateCfg.reset(new SceneLogicStateCfg("_onInitState"));
    mStartStateCfg.reset(new SceneLogicStateCfg("_onStartState"));
    mUpdateStateCfg.reset(new SceneLogicStateCfg("_onUpdateState"));

    mInitState.reset(new LogicState(mInitStateCfg.get(), this));
    mStartState.reset(new LogicState(mStartStateCfg.get(), this));
    mUpdateState.reset(new LogicState(mInitStateCfg.get(), this));



    mItemsLibrary.reset(new ItemsLibrary());
    mTransitionsGroup.reset(new TransitionsGroup());
    mComponentsGroup.reset(new ComponentsGroup());
    //mOverlayComponentsGroup.reset(new ComponentsGroup());
    mGfxObjectsGroup.reset(new GfxObjectsGroup());
    //mWidgetManager = new WidgetManager();

    mAnimationManager.reset(new AnimationManager());
    mCompoundStorage.reset(new CompoundStorage());

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

    //mInitState.reset(new LogicState("_onInitState", this));
    //mStartState.reset(new LogicState("_onStartState", this));
    //mUpdateState.reset(new LogicState("_onUpdateState", this));

    mVariables.deleteContent();
    mTriggers.deleteContent();
    mRunningLongActions.clear();

    Scene::deleteContent();
}


bool PlayedScene::build()
{

    if(Scene::build()==false){
        return false;
    }

    /*
    if(mWidgetManager){
        for(SceneMap *sm : mSceneMaps){
            if(mWidgetManager->createWidgets(sm->map())==false){
                return false;
            }

            //---
            sm->map()->changeEllipsesToBezierCurves(1);
        }
        mWidgetManager->setWidgetsToInitialState();
    }
    */


    //if(mOverlayComponentsGroup->buildObjects(this)==false){
    //    return false;
    //}

    if(mComponentsGroup->buildObjects(this)==false){
        return false;
    }

    if(mCompoundStorage->buildObjects(this)==false){
        return false;
    }


    if(mInitState->build(mInitStateCfg.get())==false){
        return false;
    }
    if(mStartState->build(mStartStateCfg.get())==false){
        return false;
    }
    if(mUpdateState->build(mUpdateStateCfg.get())==false){
        return false;
    }



    mBuilt = true;

    return true;
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

    if(initConnections()==false){
        return false;
    }

    mUpdateMode.loopAllStateActions = true;
    mInitState->update(mUpdateMode);

    return true;
}


// initActionTasksAndEvents() is called AFTER the maps have been loaded and initialized
bool PlayedScene::initConnections()
{

    // the order of initialization is important
    //----
    if(mGfxObjectsGroup->initConnections(this)==false){
        return false;
    }

    //if(mSourceEntitiesGroup->create(this)==false){
    //    return false;
    //}
    if(mCompoundStorage->initConnections(this)==false){         // initilaize before components as components may refer to components in compounds
        return false;
    }


    //---- first initialize items as they can set parameters used by logic
    if(mItemsLibrary->initConnections(this)==false){
        return false;
    }

    if(mTransitionsGroup->initConnections(this)==false){
        return false;
    }


    //---- components should be initialized last
    //if(mOverlayComponentsGroup->initConnections(this)==false){
    //    return false;
    //}

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


/*
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
*/


void PlayedScene::start()
{

    mUpdateMode.loopAllStateActions = true;
    mStartState->update(mUpdateMode);

}


bool PlayedScene::startingPhaseUpdate()
{

    return mUpdateState->startingPhaseUpdate();

}


void PlayedScene::preUpdate()
{

    Scene::preUpdate();


    mUpdateMode.loopAllStateActions = false;
    mUpdateMode.modalOverlay = mModalOverlayComponents;

    //if(mModalOverlayComponents){
    //    updateMode = UpdateMode::MODAL_OVERLAY;
    //}

    //---
    /*
    guiKeyboardAndJoystickInput.Update();

    guiCursorDeviceInput._SetCursorScreenPosition(Vec2f(mouse.posX(), mouse.posY()));
    guiCursorDeviceInput._SetCursorPressed(mouse.isButtonPressed(MouseButton::LEFT));
    guiCursorDeviceInput._SetCursorDown(mouse.isButtonDown(MouseButton::LEFT));

    for(SceneMap *sm : mSceneMaps){

        if(sm->isVisible()==false) continue;

        jugimap::Map *map = sm->map();

        if(map->isHidden()) continue;


        Vec2f cursorInMapPosition = map->camera()->MapPointFromScreenPoint(guiCursorDeviceInput.GetCursorScreenPosition());
        guiCursorDeviceInput._SetCursorInMapPosition(cursorInMapPosition);

        if(mWidgetManager){
            mWidgetManager->updateWidgets();
        }

    }
    */

    mAnimationManager->preUpdate();

    //---
    mActivatedTriggersUpdater.update();


    mUpdateState->preUpdate(mUpdateMode);


}


void PlayedScene::update()
{

    //LogicState::longActionActive = mUpdateState->hasLongActionActive();
    //if(LogicState::longActionActive){
    //    print("longActionActive");
    //}


    mUpdateMode.loopAllStateActions = false;
    mUpdateMode.modalOverlay = mModalOverlayComponents;

    mUpdateState->update(mUpdateMode);

    mAnimationManager->update();


    //for(Component* c : mRunningOverlayComponents){
    //    c->update(updateMode);
    //}

    for(OverlayState* os : mOverlayStates){
        os->compound()->update(mUpdateMode);
    }

}


void PlayedScene::postUpdate()
{
    Scene::postUpdate();

    mUpdateMode.loopAllStateActions = false;
    mUpdateMode.modalOverlay = mModalOverlayComponents;


    GuiWidget::ResetInteractedPerUpdate();

    mAnimationManager->postUpdate();

    //---
    mActivatedTriggersUpdater.resetPerUpdateFlags();

    mUpdateState->postUpdate(mUpdateMode);

}

/*
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
*/

/*
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
*/


void PlayedScene::startOverlayCompound(Compound *_previosCompound, Compound *_currentCompound)
{

    OverlayState *overlayState = new OverlayState(_currentCompound);
    overlayState->compoundTransition()->init(_previosCompound, _currentCompound);

    mOverlayStates.push_back(overlayState);

    //_previosCompound->start( overlayState->compoundTransition());
    _currentCompound->start( overlayState->compoundTransition());

    //---
    mModalOverlayComponents = false;
    for(OverlayState* s : mOverlayStates){
        if(s->compound()->modal()){
            mModalOverlayComponents = true;
            break;
        }
    }

}


void PlayedScene::endOverlayCompound(Compound *_compound)
{

    for(int i = int(mOverlayStates.size())-1; i>=0; i-- ){
        if( mOverlayStates[i]->compound()== _compound ){
            OverlayState *overlayState = mOverlayStates[i];
            mOverlayStates.erase(mOverlayStates.begin()+i);
            delete  overlayState;
        }
    }

    _compound->onStateEnded();

    //---
    mModalOverlayComponents = false;
    for(OverlayState* s : mOverlayStates){
        if(s->compound()->modal()){
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

    guiSystem()->setEnabled(mRunningLongActions.empty());

}


void PlayedScene::removeLongRunningAction(BaseObject* _action)
{

    StdVector::removeElement(mRunningLongActions, _action);

    guiSystem()->setEnabled(mRunningLongActions.empty());

}


void PlayedScene::drawShapes(jugimap::ShapeDrawer &_drawer, int _flags)
{

    mUpdateState->drawShapes(_drawer, _flags);

}




GuiSystem* PlayedScene::guiSystem()
{


    GuiSystem *guiSystem = dynamic_cast<GuiSystem*>(mComponentsGroup->getComponent("guiSystem"));   assert(guiSystem);

    return guiSystem;

}






}
