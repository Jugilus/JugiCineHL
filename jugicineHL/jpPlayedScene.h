#ifndef PLAYER__PLAYER_SCENE_H
#define PLAYER__PLAYER_SCENE_H

#include <memory>
#include "jpSettings.h"
#include "jpVariables.h"
#include "jmScene.h"



namespace jugimap{


class TransitionsGroup;
class ItemsLibrary;
class LogicState;
class Action;
class ActionTrigger;
class AppConfigurationLoader_xml;
class WidgetManager;
class Component;
class OverlayComponent;
class ComponentsGroup;
class GfxObjectsGroup;
class AnimationManager;
//class EntitySystem;
//class SourceEntitiesGroup;



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
    virtual bool init() override;
    virtual void start() override;
    virtual bool startingPhaseUpdate() override;
    virtual void preUpdate() override;
    virtual void update() override;
    virtual void postUpdate() override;
    virtual void drawShapes(jugimap::ShapeDrawer &_drawer, int _flags=0) override;



    bool initActionTasksAndEvents();

    void startOverlayComponent(OverlayComponent *_component);
    void endOverlayComponent(OverlayComponent *_component);

    bool updateLogicData(PlayedScene *newPSData);


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
    ComponentsGroup* overlayComponentsGroup(){ return mOverlayComponentsGroup.get(); }
    GfxObjectsGroup* gfxObjectsGroup(){ return mGfxObjectsGroup.get(); }
    //SourceEntitiesGroup* sourceEntitiesGroup(){ return mSourceEntitiesGroup.get(); }
    AnimationManager* animationManager(){ return mAnimationManager.get(); }



private:
    InitializationStatus mInitializationStatus = InitializationStatus::NONE;

    std::unique_ptr<LogicState> mInitState;
    std::unique_ptr<LogicState> mStartState;
    std::unique_ptr<LogicState> mUpdateState;


    std::unique_ptr<ItemsLibrary> mItemsLibrary;
    std::unique_ptr<TransitionsGroup> mTransitionsGroup;

    std::vector<BaseObject*>mRunningLongActions;            // LINKS

    VariableManager mVariables;
    TriggerVariableManager mTriggers;

    VariableManager mLocalVariablesStorage;

    std::vector<OverlayComponent*>mRunningOverlayComponents;    // LINKS
    bool mModalOverlayComponents= false;

    //---
    ActivatedTriggersUpdater mActivatedTriggersUpdater;

    std::unique_ptr<ComponentsGroup> mComponentsGroup;
    std::unique_ptr<ComponentsGroup> mOverlayComponentsGroup;
    std::unique_ptr<GfxObjectsGroup> mGfxObjectsGroup;

    std::unique_ptr<AnimationManager>mAnimationManager;


    //std::unique_ptr<EntitySystem> mEntitySystem;
    //std::unique_ptr<SourceEntitiesGroup> mSourceEntitiesGroup;

};



}






#endif
