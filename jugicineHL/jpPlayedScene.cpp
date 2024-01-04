#include <chrono>
#include <assert.h>
#include <sstream>
#include "pugixml/pugixml.hpp"

#include "jmCommonFunctions.h"
#include "jmMap.h"
#include "jmCamera.h"
#include "jmScene.h"
#include "jmSceneLayout.h"

#include "jpObjectFactory.h"
#include "data/jpItem.h"
#include "jpSceneLogicState.h"
#include "jpComponentsCommon.h"
#include "jpGfxObjectsCommon.h"
#include "items/jpItemsCommon.h"
#include "logic/jpLogicState.h"
#include "logic/jpFunction.h"
#include "logic/jpTransition.h"
#include "gfxObjects/jpAnimationObjects.h"
#include "gfxObjects/jpCompound.h"
#include "gfxObjects/jpGfxObjectsCommon.h"

#include "gui/jpGuiCommon.h"
#include "gui/jpGuiSystem.h"

#include "jpPlayedApp.h"
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
    //mGfxObjectsGroup.reset(new GfxObjectsGroup());

    mAnimationManager.reset(new AnimationManager());
    mCompoundStorage.reset(new CompoundStorage());

    mSourceItemsStorage.reset(new SourceItemsStorage());


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

    if(mSourceItemsStorage->buildObjects(this)==false){
        return false;
    }

    if(mComponentsGroup->buildObjects(this)==false){
        return false;
    }

    if(mCompoundStorage->buildObjects(this)==false){
        return false;
    }



    //if(mFunctionStorage->buildObjects(this)==false){
    //    return false;
    //}


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


    if(mSourceItemsStorage->initConnections(this)==false){
        return false;
    }

    // the order of initialization is important
    //----
    //if(mGfxObjectsGroup->initConnections(this)==false){
    //    return false;
    //}
    for(const std::string & s : mAddGfxObjects){
        GfxObjectCfg *cfg = mGfxObjectCfgStorage.getObject(s);
        if(cfg==nullptr){
            return false;
        }
        if(cfg->type()==GfxObjectType::TEXT_SPRITE){
            mGfxObjectStorage.addObject(new GTextSprite(cfg, this));
        }
    }



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


    return true;

}



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



bool PlayedScene::loadScene_LogicData(const std::string &sceneFilePath)
{

    dbgSystem.addMessage("Reading xml file '" + sceneFilePath +"' ...");

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(sceneFilePath.c_str());

    DbgSystem & dbgSystemTMP = dbgSystem;


    if(result.status==pugi::xml_parse_status::status_ok){

        pugi::xml_node rootNode = doc.child("scene");
        if(rootNode.empty()==false){

            mName = rootNode.attribute("name").as_string(mName.c_str());
            //---
            dbgSystem.addMessage("Loading scene '" + mName + "' main logic data from file '" + sceneFilePath +"' ...");

            for(pugi::xml_node n = rootNode.first_child(); n; n = n.next_sibling()){

                std::string nodeName = std::string(n.name());

                if(nodeName.length()>5 && nodeName.substr(nodeName.length()-5)=="_SKIP"){
                    continue;
                }

                if(nodeName=="sceneMaps"){

                    for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                        std::string childNodeName = std::string(nChild.name());

                        if(childNodeName=="sceneMap"){
                            SceneMap * sm = new SceneMap();
                            sm->info.name = nChild.attribute("name").value();

                            if(sm->info.loadCfg(nChild)==false){
                                return false;
                            }
                            mSceneMaps.push_back(sm);

                        }else{
                            dbgSystem.addMessage("Unknown node '" + childNodeName + "' not found!");
                            return false;
                        }
                    }



                //------
                }else if(nodeName=="init"){

                    if(mInitStateCfg->initCfg(n)==false){
                        return false;
                    }

                //------
                }else if(nodeName=="start"){

                    if(mStartStateCfg->initCfg(n)==false){
                        return false;
                    }


                //------
                }else if(nodeName=="update"){

                    if(mUpdateStateCfg->initCfg(n)==false){
                        return false;
                    }


                //------
                }else if(nodeName=="itemsGroup"){

                    ItemsGroup *ig = new ItemsGroup(n.attribute("name").as_string(""), this);

                    if(ig->initCfg(n)==false){
                        delete ig;
                        return false;
                    }
                    mItemsLibrary->addItemsGroup(ig);


                //------
                }else if(nodeName=="gfxObjects"){

                    for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                        std::string childNodeName = std::string(nChild.name());

                        if(childNodeName=="spritesObject"){
                            //mGfxObjectsGroup->gfxObjects().push_back(new GSpritesObject(nChild));
                            assert(false);

                        }else if(childNodeName=="textSpriteObject"){
                            //mGfxObjectsGroup->gfxObjects().push_back(new GTextSprite(nChild));

                            std::string name = nChild.attribute("name").as_string();

                            GTextSpriteCfg *cfg = new GTextSpriteCfg(name);
                            if(cfg->initCfg(nChild)==false){
                                return false;
                            }
                            if(mGfxObjectCfgStorage.addObject(cfg)==false){
                                return false;
                            }


                        }else if(childNodeName=="aniSpriteObject"){
                            //mGfxObjectsGroup->gfxObjects().push_back(new GTextSprite(nChild));

                            std::string name = nChild.attribute("name").as_string();

                            AniSpriteObjectCfg *cfg = new AniSpriteObjectCfg(name);
                            if(cfg->initCfg(nChild)==false){
                                return false;
                            }
                            if(mGfxObjectCfgStorage.addObject(cfg)==false){
                                return false;
                            }

                        }else if(childNodeName=="relativeMover"){
                            //mGfxObjectsGroup->gfxObjects().push_back(new GTextSprite(nChild));

                            std::string name = nChild.attribute("name").as_string();

                            GORelativeMoverCfg *cfg = new GORelativeMoverCfg(name);
                            if(cfg->initCfg(nChild)==false){
                                return false;
                            }
                            if(mGfxObjectCfgStorage.addObject(cfg)==false){
                                return false;
                            }
                        }
                    }


                //------
                }else if(nodeName=="transitions"){

                    for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                        std::string childNodeName = std::string(nChild.name());

                        if(childNodeName=="transition"){
                            Transition * t = new Transition(this);
                            if(t->initCfg(nChild)==false){
                                delete t;
                                return false;
                            }
                            mTransitionsGroup->transitions().push_back(t);
                        }
                    }


                //------
                }else if(nodeName=="components"){

                    for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                        std::string childNodeName = std::string(nChild.name());

                        Component *c = actionFactory->createComponent(childNodeName, nChild);

                        if(c){
                            mComponentsGroup->components().push_back(c);
                        }else{
                            dbgSystem.addMessage("Component '" + childNodeName + "' not found!");
                            return false;
                        }
                    }


                //------
                }else if(nodeName=="compounds"){

                    if(mCompoundStorage->initCfg(this, n)==false){
                        return false;
                    }


                //------
                }else if(nodeName=="items"){

                    if(mSourceItemsStorage->initCfg(n)==false){
                        return false;
                    }

                //------
                }else if(nodeName=="add"){

                    for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                        std::string attributeName =std::string(a.name());

                        if(attributeName=="gfxObjects"){
                            std::string sValue = a.as_string();
                            mAddGfxObjects = StdString::splitString(sValue, ",");

                        }else{

                            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                            return false;
                        }
                    }

                    if(mSourceItemsStorage->initCfg(n)==false){
                        return false;
                    }

                }else{

                    // check for custom components

                    Component * c = mComponentsGroup->getComponent(nodeName, false);

                    if(c==nullptr){

                        CustomComponentFactoryManager *m = app->customComponentFactoryManager();
                        c = m->createCustomComponent(nodeName, false);

                        if(c){
                           mComponentsGroup->components().push_back(c);
                        }else{

                            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
                            return false;
                        }
                    }

                    if(c->initCfg(this, n)==false){
                        return false;
                    }


                }
            }

            //---
            dbgSystem.removeLastMessage();
        }

    }else{

        dbgSystem.addMessage("Error reading xml file '" + sceneFilePath + "'");
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;

}


bool PlayedScene::preloadScene_countObjects(const std::string &sceneFilePath)
{

    dbgSystem.addMessage("Reading xml file '" + sceneFilePath +"' ...");

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(sceneFilePath.c_str());

    DbgSystem & dbgSystemTMP = dbgSystem;


    if(result.status==pugi::xml_parse_status::status_ok){

        pugi::xml_node rootNode = doc.child("scene");
        if(rootNode.empty()==false){

            mName = rootNode.attribute("name").as_string(mName.c_str());
            //---
            dbgSystem.addMessage("Counting objects in scene '" + mName + "' ...");

            for(pugi::xml_node n = rootNode.first_child(); n; n = n.next_sibling()){

                std::string nodeName = std::string(n.name());

                if(nodeName.length()>5 && nodeName.substr(nodeName.length()-5)=="_SKIP"){
                    continue;
                }

                if(nodeName=="items"){

                    if(mSourceItemsStorage->preload_countObjects(n)==false){
                        return false;
                    }

                }
            }

            //---
            dbgSystem.removeLastMessage();
        }

    }else{

        dbgSystem.addMessage("Error reading xml file '" + sceneFilePath + "'");
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;

}





}
