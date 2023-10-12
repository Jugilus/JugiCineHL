#ifndef PLAYED_APP__H
#define PLAYED_APP__H


#include <memory>
#include "jmTimer.h"
#include "jmApp.h"
#include "jpGlobal.h"

namespace jugimap{



class ShapeDrawer;
class PlayedScene;
class UserManager;
class VariableManager;
class SystemSettings;
class DelayedTaskManager;
class InputSystem;
class EntitySystem;
class CustomComponentFactoryManager;
class SignalParserManager;



class PlayedApp : public App
{
public:

    PlayedApp();
    virtual ~PlayedApp() override;

    void preInit() override;
    void init() override;

    void update(float _framePeriod) override;

    void setPaused(bool _paused){ mPaused = _paused; }


    VariableManager *globalVariables(){ return mGlobalVariables.get(); }
    VariableManager *systemVariables(){ return mSystemVariables.get(); }
    UserManager *usersDatabase(){ return mUsersDatabase.get(); }
    SystemSettings* systemSettings(){ return mSystemSettings.get();}
    DelayedTaskManager* delayedTaskManager(){ return mDelayedTaskManager.get(); }
    InputSystem *inputSystem(){ return mInputSystem.get(); }
    //InputSystem *inputSystem(){ return mInputSystem; }
    //EntitySystem* entitySystem(){ return mEntitySystem.get(); }
    CustomComponentFactoryManager* customComponentFactoryManager(){ return mCustomComponentFactoryManager.get(); }
    SignalParserManager* signalParserManager(){ return mSignalParserManager.get(); }

    AppError error(){ return mError; }

    void _setInitializationStatus(AppInitializationStatus _status ){ mInitializationStatus = static_cast<int>(_status);}


    const std::string message(){ return mMessage; }

private:

    int mInitializationStatus = static_cast<int>(AppInitializationStatus::NONE);
    AppError mError = AppError::NONE;
    AppState mState = AppState::LOADING_INITIAL_DATA;
    bool mPaused = false;

    bool mLoadSourceLibraryExternalDataOnInit = true;

    std::unique_ptr<UserManager>mUsersDatabase;
    std::unique_ptr<VariableManager>mGlobalVariables;
    std::unique_ptr<VariableManager>mSystemVariables;
    std::unique_ptr<SystemSettings>mSystemSettings;
    std::unique_ptr<DelayedTaskManager>mDelayedTaskManager;
    std::unique_ptr<InputSystem>mInputSystem;
    //std::unique_ptr<EntitySystem> mEntitySystem;
    std::unique_ptr<CustomComponentFactoryManager>mCustomComponentFactoryManager;
    std::unique_ptr<SignalParserManager>mSignalParserManager;

    std::string mMessage;
};


extern PlayedApp *app;



}


#endif
