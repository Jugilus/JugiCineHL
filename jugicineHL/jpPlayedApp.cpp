#include <ncine/Application.h>
#include <ncine/FileSystem.h>
#include "jmGuiText.h"
#include "jmSourceContainers.h"
#include "jmFont.h"
#include "jmSourceObjectsLoader_xml.h"
#include "jmSystem.h"
#include "jmStreams.h"
#include "jmVectorShapeDrawing.h"
#include "components/jpComponentsCommon.h"
#include "jpGameObjectsLoader_xml.h"
#include "jpUser.h"
#include "jpInput.h"
#include "jpQueries.h"
#include "jpSystemVarious.h"
#include "jpEntitySystem.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"



namespace jugimap{


PlayedApp::PlayedApp()
{

}

PlayedApp::~PlayedApp()
{

}


void PlayedApp::preInit()
{
    //--- set fixed root directories where game data is stored

    std::vector<std::string> &dbgErrorMessages = dbgSystem.errorMessages();
    TEProjectInfo &dbgCurrentProject = CurrentProject;
    //bool ok = ncine::FileSystem::setCurrentDir("c:/Development/JugiMapProject/JugiCine/examples/Testbed");



#ifdef __EMSCRIPTEN__
    CurrentProject.gameRootDir = "";
        CurrentProject.sourceCfgDir = "data/sourceCfg";
        CurrentProject.gameCfgDir = "data/_game";

#else
    CurrentProject.gameRootDir = ncine::FileSystem::currentDir().data();
    CurrentProject.sourceCfgDir = CurrentProject.gameRootDir + "/data/sourceCfg";
    CurrentProject.gameCfgDir = CurrentProject.gameRootDir + "/data/_game";
#endif

    //jugimap::TextLibrary::path = "data/text/";

    jugimap::settings.setYorientation(jugimap::Yorientation::UP);

    if(appConfigurationLoader_xml.loadApplicationBaseParameters(this, mMessage)==false){
        mError = AppError::PREINIT_ERROR;
        //mMessage = "loadApplicationBaseParameters ERROR ";
        return;
    }
    //mError = AppError::PREINIT_ERROR;
    //return;

    //--- create save data directory
    mSaveDataDir = ncine::FileSystem::savePath().data();
    StdString::removeEndSlash(mSaveDataDir);
    mSaveDataDir = mSaveDataDir + "/" + mSaveDataDirName;

    //if((ncine::FileSystem::exists(savePath.data()) && ncine::FileSystem::isDirectory(savePath.data()))==false){

#ifndef __EMSCRIPTEN__
    if(ncine::FileSystem::isDirectory(mSaveDataDir.data())==false){
        if(ncine::FileSystem::createDir(mSaveDataDir.data())==false){
            dbgSystem.addMessage("Save data directory '" + mSaveDataDir +"' could not be created!");
            mError = AppError::PREINIT_ERROR;
            return;
        }
    }
#endif

    //---
    mInitializationStatus |= static_cast<int>(AppInitializationStatus::APP_BASE_CONFIGURATION_DATA_LOADED);
    assert(dbgErrorMessages.empty());

}



void PlayedApp::init()
{

    //mMessage = "PlayedApp::init() 1 ";

    //mMessage = "CurrentProject.gameRootDir="+CurrentProject.gameRootDir;

    std::vector<std::string> &dbgErrorMessages = dbgSystem.errorMessages();
    SourceLibrary &dbgSourceLibrary = sourceLibrary;

    if(!(mInitializationStatus & static_cast<int>(AppInitializationStatus::APP_BASE_CONFIGURATION_DATA_LOADED))){
        return;
    }

    //mMessage = "PlayedApp::init() 2 ";



    //--- GLOBAL DATA

    // create and load system settings before loading global data which include text !
    mSystemSettings.reset(new SystemSettings(this));
#ifndef __EMSCRIPTEN__
    if(mSystemSettings->loadSettingsFromIniFile()==false){
        mSystemSettings->saveSettingsIniFile();
    }
#endif


    sourceObjectsLoader_xml.loadGlobalAndSystemSourceObjects();


    if(sourceLibrary.loadGlobalExternalData()==false){
        mError = AppError::INIT_ERROR;
         return;
    }

    mMessage = "PlayedApp::init() 4 ";

    //mError = AppError::DBG_ERROR;
    //return;


    //----
    mShapeDrawer.reset(new ShapeDrawer());
    mSystemVariables.reset(new VariableManager());
    mGlobalVariables.reset(new VariableManager());
    mDelayedTaskManager.reset(new DelayedTaskManager());
    mInputSystem.reset(new InputSystem(this));
    mUsersDatabase.reset(new UserManager(this));
    //mEntitySystem.reset(new EntitySystem());

    mCustomComponentFactoryManager.reset(new CustomComponentFactoryManager());
    mCustomComponentFactoryManager->addFactory(new EntitySystemFactory("entitySystem"));

    mSignalParserManager.reset(new SignalParserManager());
    mSignalParserManager->addSignalParser(new CoreSignalsParser({"INPUT_COMMAND", "ANIMATED_OBJECT"}));





    if(appConfigurationLoader_xml.loadApplicationGlobalData(this)==false){
        mError = AppError::INIT_ERROR;
        return;
    }

    //----
    if(mInputSystem->initConnections()==false){
        mError = AppError::INIT_ERROR;
        return;
    }


    mMessage = "PlayedApp::init() 5 ";

    //----
    if(mUsersDatabase->loadUsers()==false){
        mUsersDatabase->setDefaultUser();
    }

    //--- SCENES

    //---- load base configuration data for scenes
    for(Scene *s : mScenes){
        if(appConfigurationLoader_xml.loadSceneBaseData(s)==false){
            mError = AppError::INIT_ERROR;
            return;
        }
    }
    mInitializationStatus |= static_cast<int>(AppInitializationStatus::SCENE_BASE_CONFIGURATION_DATA_LOADED);


    //system.init();
    mMessage = "PlayedApp::init() 5 ";


    //---- load logic configuration data for scenes
    for(Scene *s : mScenes){
        if(appConfigurationLoader_xml.loadSceneLogicData(static_cast<PlayedScene*>(s))==false){
            mError = AppError::INIT_ERROR;
            return;
        }
    }
    mInitializationStatus |= static_cast<int>(AppInitializationStatus::SCENE_LOGIC_CONFIGURATION_DATA_LOADED);
    assert(dbgErrorMessages.empty());




    //---- load source objects
    sourceObjectsLoader_xml.loadSourceGroups();
    sourceLibrary.initSourceObjects();
    mInitializationStatus |= static_cast<int>(AppInitializationStatus::SOURCE_LIBRARY_LOADED);
    assert(dbgErrorMessages.empty());


    //---- load external data for all source objects
    if(mLoadSourceLibraryExternalDataOnInit){
        if(sourceLibrary.loadExternalData()==false){
            mError = AppError::INIT_ERROR;
            return;
        }
        mInitializationStatus |= static_cast<int>(AppInitializationStatus::SOURCE_LIBRARY_ENTIRE_EXTERNAL_DATA_LOADED);
    }
    assert(dbgErrorMessages.empty());


    //---- input commands (after the books source data has been loaded
    // global source data should be probably loaded separately earlier so this function could be called earlier
    //if(mInputSystem->initConnectionsAndCreateProfiles()==false){
    //    mState = State::ERROR;
    //    return;
    //}

    //mInputSystem->saveProfilesIniFile();


    //----
    if(scenes().empty()){
        dbgSystem.addMessage("There is no scene in the application!");
        mError = AppError::INIT_ERROR;
        return;
    }


    if(mActiveScene==nullptr){
        Scene *scene = scenes().front();
        if(setActiveScene(scene)==false){
            mError = AppError::INIT_ERROR;
            return;
        }
    }


    mState = AppState::LOADING_SCENE;




    //----
    //if(settingsLoaded==false){
    //    mSystemSettings->saveSettingsIniFile();
    //}

    //---
    mError = AppError::NONE;

    //mTicker.start(1)

}


void PlayedApp::update(float _framePeriod)
{

    if(mActiveScene==nullptr){
        return;
    }
    if(mError != AppError::NONE){
        return;
    }

    //if(mActiveScene==nullptr || mActiveScene->isBuilt()==false) return;


    if(mState==AppState::LOADING_INITIAL_DATA){

        DummyFunction();

        mMessage = "AppState::LOADING_INITIAL_DATA";

    }else if(mState==AppState::LOADING_SCENE){

        mMessage = "AppState::LOADING_SCENE 1";

        if(mActiveScene->build()==false){
            dbgSystem.addMessage("Scene '" + mActiveScene->name() +"' map building error!");
            mError = AppError::INIT_ERROR;
            return;
        }
        DebugListSceneTree(mNode);

        mMessage += " 2";
        //----
        if(mActiveScene->init()==false){
            dbgSystem.addMessage("Scene '" + mActiveScene->name() +"' initialization error!");
            mError = AppError::INIT_ERROR;
            return;
        }

        mMessage += " 3";

        //---- reset time variables !
        //passedTimeMS = passedTimeMSPrevious = time.UpdatePassedTimeMS();      // this need to be examined when updating time via timeStep method
        //storedUpdatePeriods.clear();

        //----
        //mActiveScene->start();

        //mState = AppState::STARTING_SCENE;


        while(true){
            if(mActiveScene->startingPhaseUpdate()==false){
                break;
            }
        }
        mState = AppState::SCENE_FADE_IN;

        mMessage += " 4";

    }else if(mState==AppState::STARTING_SCENE){

        if(mActiveScene->isBuilt()==false) return;

        ManageSuspendedTime(_framePeriod*1000);
        //---
        if(mActiveScene->startingPhaseUpdate()==false){
            mState = AppState::SCENE_FADE_IN;
        }


    }else if(mState==AppState::SCENE_FADE_IN){

        mState = AppState::SCENE_RUNNING;


    }else if(mState==AppState::SCENE_RUNNING){

        mMessage = "AppState::SCENE_RUNNING 1";

        if(mPaused){
            return;
        }

        ManageSuspendedTime(_framePeriod*1000);
        //---
        mActiveScene->preUpdate();
        mInputSystem->update();

        mActiveScene->update();

        mActiveScene->postUpdate();

        mDelayedTaskManager->update();

        mMessage += " 2";

    }else if(mState==AppState::SCENE_FADE_OUT){


    }else if(mState==AppState::REMOVING_SCENE){


    }




    //App::update(_framePeriod);




}


/*
    if(mState==State::NORMAL){

        App::update(_framePeriod);
        mDelayedTaskManager->update();

    }else if(mState==State::RELOAD_APP_PART_1){

        // Delete all source, map, logic data.... and reset possible dangling link pointers
        // After deleting the application should be ready to be initialized again.

        deleteContent();
        sourceLibrary.deleteContent();
        fontsLibrary.deleteContent();
        textColorsLibrary.deleteContent();
        DeleteGuiSupportingObjects();
        DeleteInputSupportingObjects();

        mActiveScene = nullptr;
        mInitializationStatus = static_cast<int>(InitializationStatus::APP_BASE_CONFIGURATION_DATA_LOADED);

        //---

        mShapeDrawer->begin();

        Vec2f pos(100, settings.GetScreenSize().y-100);
        mShapeDrawer->drawText("RELOADING GAME !", pos);

        mShapeDrawer->end();

        //---
        mState = State::RELOAD_APP_PART_2;

        App::update(_framePeriod);


    }else if(mState==State::RELOAD_APP_PART_2){

        init();
        mState = State::NORMAL;

    }else if(mState==State::ERROR){

        showErrorMessage();
    }





    //----
    if(mShowOverlay){

        mShapeDrawer->begin();

        mShapeDrawer->setOutlineColor(ColorRGBA(0,0,0,128));
        mShapeDrawer->drawRectFilled(BoundingBoxFloat(30,30, 300, 300));

        mShapeDrawer->end();

    }

    if(mShowGui && mState==State::NORMAL){


        if(ImGui::Begin("Player")){
            if (ImGui::Button("Reload Game")){

                mState = State::RELOAD_APP_PART_1;

            }

            if (ImGui::Button("Reload Logic")){

                if(updateActiveSceneLogic()==false){
                    mState = State::ERROR;
                }
            }

            ImGui::Checkbox("Debug", &mShowOverlay);

            if(ImGui::Button("Test Message Box")){
                 mState = State::ERROR;
            }

        }
        ImGui::End();
    }

}



bool PlayedApp::updateActiveSceneLogic()
{

    PlayedScene* activePlayerScene = static_cast<PlayedScene*>(mActiveScene);

    std::unique_ptr<PlayedScene>temporarySceneForLogicData;
    temporarySceneForLogicData.reset(new PlayedScene(activePlayerScene->name(), this));


    if(appConfigurationLoader_xml.loadSceneLogicData(temporarySceneForLogicData.get())==false){
        mState = State::ERROR;
        return false;
    }
    if(activePlayerScene->updateLogicData(temporarySceneForLogicData.get())==false){
        mState = State::ERROR;
        return false;
    }

    if(activePlayerScene->initActionTasksAndEvents()==false){
        mState = State::ERROR;
        return false;
    }


    return true;

}


void PlayedApp::showErrorMessage()
{

    ImVec2 size(400.0f, 400.0f);
    ImGui::SetNextWindowSize(size, ImGuiCond_None);
    ImVec2 pos(settings.GetScreenSize().x/2 - size.x/2, settings.GetScreenSize().y/2 - size.y/2);
    ImGui::SetNextWindowPos(pos, ImGuiCond_None);

    if(ImGui::Begin("Message!", nullptr, ImGuiWindowFlags_Modal)){

        if(dbgSystem.errorMessages().empty()==false){
            for(const std::string &s : dbgSystem.errorMessages()){
                ImGui::Text("%s", s.c_str());
            }

        }else{
            ImGui::Text("Undefined error!");
        }

        ImGui::Dummy(ImVec2(20,20));

        ImGui::Separator();

        ImGui::Dummy(ImVec2(20,20));

        if(ImGui::Button("Close")){
             mState = State::NORMAL;
        }


        ImGui::End();
    }

}

*/


PlayedApp *app = nullptr;


}


