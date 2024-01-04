#ifndef PLAYER__PLAYER_SCENE_H
#define PLAYER__PLAYER_SCENE_H

#include <memory>
#include "jpSettings.h"
#include "jpVariables.h"
#include "jmScene.h"
#include "jmStorage.h"



namespace jugimap{


class GuiSystem;
class TransitionsGroup;
class ItemsLibrary;
class LogicState;
class Action;
//class ActionTrigger;
class AppConfigurationLoader_xml;
//class WidgetManager;
class Component;
//class OverlayComponent;
class ComponentsGroup;
class GfxObjectsGroup;
class AnimationManager;
class CompoundStorage;
class Compound;
class OverlayState;
struct SceneLogicStateCfg;
//class EntitySystem;
//class SourceEntitiesGroup;
class SourceItemsStorage;
class FunctionStorage;
struct GfxObjectCfg;
class GfxObject;





class PlayedScene : public jugimap::Scene, public BaseObject
{
public:
    friend class AppConfigurationLoader_xml;

    enum class InitializationStatus : int
    {

        NONE = 0,
        GENERAL_DATA_LOADED = 1,
        LOGIC_DATA_LOADED = 2,
        LOGIC_DATA_INITIALIZED = 4,
        SOURCE_LIBRARY_LOADED = 8,
        SOURCE_LIBRARY_ENTIRE_EXTERNAL_DATA_LOADED = 16

    };

    PlayedScene(const std::string &_name, App* _parentApp);
    ~PlayedScene();
    void deleteContent() override;


    //---
    bool build() override;
    virtual bool init() override;
    virtual void start() override;
    virtual bool startingPhaseUpdate() override;
    virtual void preUpdate() override;
    virtual void update() override;
    virtual void postUpdate() override;
    virtual void drawShapes(jugimap::ShapeDrawer &_drawer, int _flags=0) override;



    bool initConnections();

    //void startOverlayComponent(OverlayComponent *_component);
    //void endOverlayComponent(OverlayComponent *_component);

    void startOverlayCompound(Compound *_previosCompound, Compound *_currentCompound);
    void endOverlayCompound(Compound *_compound);


    //bool updateLogicData(PlayedScene *newPSData);


    LogicState* initState(){ return mInitState.get(); }
    LogicState* startState(){ return mStartState.get(); }
    LogicState* updateState(){ return mUpdateState.get(); }

    ItemsLibrary* itemsLibrary(){ return mItemsLibrary.get(); }
    TransitionsGroup* transitionsGroup(){ return mTransitionsGroup.get(); }

    //---
    std::vector<BaseObject*>& runningLongActions(){ return mRunningLongActions; }
    void addLongRunningAction(BaseObject* _action);
    void removeLongRunningAction(BaseObject *_action);

    //---
    VariableManager& variables(){ return  mVariables; }
    VariableManager& localVariablesStorage(){ return  mLocalVariablesStorage; }
    TriggerVariableManager& triggers(){ return  mTriggers; }
    ActivatedTriggersUpdater& activatedTriggersUpdater(){ return mActivatedTriggersUpdater; }

    //---
    ComponentsGroup* componentsGroup(){ return mComponentsGroup.get(); }
    //GfxObjectsGroup* gfxObjectsGroup(){ return mGfxObjectsGroup.get(); }
    SimpleStorage<GfxObjectCfg*> & gfxObjectCfgStorage(){ return mGfxObjectCfgStorage; }
    SimpleStorage<GfxObject*> & gfxObjectStorage(){ return mGfxObjectStorage; }
    AnimationManager* animationManager(){ return mAnimationManager.get(); }

    GuiSystem * guiSystem();

    CompoundStorage* compoundStorage(){ return mCompoundStorage.get(); }
    SourceItemsStorage* sourceItemsStorage(){ return mSourceItemsStorage.get(); }
    //FunctionStorage* functionStorage(){ return mFunctionStorage.get(); }

    //SimpleStorage<SceneLogicStateCfg*> & initStateCfgStorage(){ return mInitStateCfgStorage; }
    //SimpleStorage<SceneLogicStateCfg*> & startStateCfgStorage(){ return mStartStateCfgStorage; }
    //SimpleStorage<SceneLogicStateCfg*> & updateStateCfgStorage(){ return mUpdateStateCfgStorage; }

    SceneLogicStateCfg* initStateCfg(){ return mInitStateCfg.get(); }
    SceneLogicStateCfg* startStateCfg(){ return mStartStateCfg.get(); }
    SceneLogicStateCfg* updateStateCfg(){ return mUpdateStateCfg.get(); }

    std::vector<OverlayState*> & overlayStates(){ return mOverlayStates;}

    bool loadScene_LogicData(const std::string &sceneFilePath);
    bool preloadScene_countObjects(const std::string &sceneFilePath);

private:
    InitializationStatus mInitializationStatus = InitializationStatus::NONE;

    std::unique_ptr<LogicState> mInitState;
    std::unique_ptr<LogicState> mStartState;
    std::unique_ptr<LogicState> mUpdateState;

    UpdateMode mUpdateMode;

    std::unique_ptr<ItemsLibrary> mItemsLibrary;
    std::unique_ptr<TransitionsGroup> mTransitionsGroup;

    std::vector<BaseObject*>mRunningLongActions;            // LINKS

    VariableManager mVariables;
    TriggerVariableManager mTriggers;

    VariableManager mLocalVariablesStorage;

    //std::vector<OverlayComponent*>mRunningOverlayComponents;    // LINKS
    bool mModalOverlayComponents= false;

     std::vector<Compound*>mRunningOverlayCompounds;    // LINKS

    //---
    ActivatedTriggersUpdater mActivatedTriggersUpdater;

    std::unique_ptr<ComponentsGroup> mComponentsGroup;
    //std::unique_ptr<ComponentsGroup> mOverlayComponentsGroup;
    //std::unique_ptr<GfxObjectsGroup> mGfxObjectsGroup;
    SimpleStorage<GfxObjectCfg*>mGfxObjectCfgStorage{"GfxObjectCfgStorage"};
    SimpleStorage<GfxObject*>mGfxObjectStorage{"GfxObjectStorage"};

    std::unique_ptr<AnimationManager>mAnimationManager;

    std::unique_ptr<CompoundStorage>mCompoundStorage;

    std::unique_ptr<SourceItemsStorage>mSourceItemsStorage;
    //std::unique_ptr<FunctionStorage>mFunctionStorage;

    //SimpleStorage<SceneLogicStateCfg*>mInitStateCfgStorage;
    //SimpleStorage<SceneLogicStateCfg*>mStartStateCfgStorage;
    //SimpleStorage<SceneLogicStateCfg*>mUpdateStateCfgStorage;

    std::unique_ptr<SceneLogicStateCfg>mInitStateCfg;
    std::unique_ptr<SceneLogicStateCfg>mStartStateCfg;
    std::unique_ptr<SceneLogicStateCfg>mUpdateStateCfg;

    //WidgetManager * mWidgetManager = nullptr;
    std::vector<OverlayState*>mOverlayStates;


    //-----
    std::vector<std::string>mAddGfxObjects;




};



}






#endif
