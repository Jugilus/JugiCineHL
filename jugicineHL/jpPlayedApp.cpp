#include <ncine/Application.h>
#include <ncine/FileSystem.h>

#include "pugixml/pugixml.hpp"

#include "jmText.h"
#include "jmSourceContainers.h"
#include "jmFont.h"
#include "jmSourceObjectsLoader_xml.h"
#include "jmSystem.h"
#include "jmStreams.h"
#include "jmVectorShapeDrawing.h"
#include "components/jpComponentsCommon.h"
#include "components/jpMessagePanel.h"
#include "jpGameObjectsLoader_xml.h"
#include "jpUser.h"
#include "jpInput.h"
#include "jpQueries.h"
#include "jpSystemVarious.h"
#include "gui/jpGuiCommon.h"
#include "gui/jpGuiSystem.h"
#include "jpEntitySystem.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"



namespace jugimap{


PlayedApp::PlayedApp()
{

}

PlayedApp::~PlayedApp()
{

    DeleteGuiSupportingObjects();
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

    //jugimap::TextLibrary::path = "data/text/";¸¸¸¸

    jugimap::settings.setYorientation(jugimap::Yorientation::UP);

    //if(appConfigurationLoader_xml.loadApplicationBaseParameters(this, mMessage)==false){
    if(loadApplicationBaseParameters(mMessage)==false){
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
    mCustomComponentFactoryManager->addFactory(new GuiSystemFactory("guiSystem"));
    mCustomComponentFactoryManager->addFactory(new MessagePanelFactory("messagePanel"));
    //mCustomComponentFactoryManager->addFactory(new CharacterSystemFactory("characterSystem"));

    mSignalParserManager.reset(new SignalParserManager());
    mSignalParserManager->addAndStoreSignalParser(new CoreSignalsParser(
    {"STATES", "USERS", "INPUT_COMMAND", "INPUT", "SETTINGS", "ANIMATED_OBJECT", "GFX"}));


    //if(appConfigurationLoader_xml.loadApplicationGlobalData(this)==false){
    if(loadApplicationGlobalData()==false){
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

    /*
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
    */

    for(Scene *s : mScenes){
        //if(appConfigurationLoader_xml.loadSceneBaseData(s)==false){
        if(loadScene(static_cast<PlayedScene*>(s), LoadedContent::SCENE_COUNT_OBJECTS)==false){
            mError = AppError::INIT_ERROR;
            return;
        }
    }

    for(Scene *s : mScenes){
        //if(appConfigurationLoader_xml.loadSceneBaseData(s)==false){
        if(loadScene(static_cast<PlayedScene*>(s), LoadedContent::SCENE_LOGIC_DATA)==false){
            mError = AppError::INIT_ERROR;
            return;
        }
    }

    mInitializationStatus |= static_cast<int>(AppInitializationStatus::SCENE_LOGIC_CONFIGURATION_DATA_LOADED);


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

        mState = AppState::STARTING_SCENE;

        /*
        while(true){
            if(mActiveScene->startingPhaseUpdate()==false){
                break;
            }
        }
        mState = AppState::SCENE_FADE_IN;

        mMessage += " 4";
        */

    }else if(mState==AppState::STARTING_SCENE){

        //if(mActiveScene->isBuilt()==false) return;

         //ManageSuspendedTime(_framePeriod*1000);

         //---
         gSignalUpdater.preUpdateSignals();

         //---

         while(true){
             if(mActiveScene->startingPhaseUpdate()==false){
                 break;
             }
         }

         //if(mActiveScene->startingPhaseUpdate()==false){
             mState = AppState::SCENE_FADE_IN;
         //}

         gSignalUpdater.postUpdateSignals();


    }else if(mState==AppState::SCENE_FADE_IN){

        mState = AppState::SCENE_RUNNING;


    }else if(mState==AppState::SCENE_RUNNING){

        mMessage = "AppState::SCENE_RUNNING 1";

        if(mPaused){
            return;
        }

        ManageSuspendedTime(_framePeriod*1000);

        //---
        gSignalUpdater.preUpdateSignals();
        mInputSystem->preUpdate();

        mActiveScene->preUpdate();

        mActiveScene->update();

        mActiveScene->postUpdate();

        mDelayedTaskManager->update();

        gSignalUpdater.postUpdateSignals();

        mMessage += " 2";

    }else if(mState==AppState::SCENE_FADE_OUT){


    }else if(mState==AppState::REMOVING_SCENE){


    }

    //App::update(_framePeriod);

}



bool PlayedApp::loadApplicationBaseParameters(std::string &dbgText)
{

    std::string filePath = CurrentProject.gameCfgDir + "/_application.xml";


    //---
    dbgSystem.addMessage("Loading application base parameters from file '_application.xml' ...");


    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());


    if(result.status==pugi::xml_parse_status::status_ok){
        dbgText = "OK Path : " + filePath;

        pugi::xml_node rootNode = doc.child("application");
        if(rootNode.empty()==false){
            mName = rootNode.attribute("name").as_string("MyGame");

            for(pugi::xml_node n = rootNode.first_child(); n; n = n.next_sibling()){
                std::string nodeName = std::string(n.name());

                if(nodeName=="saveDataDirName"){
                    mSaveDataDirName = n.attribute("name").as_string("jugicineGame");

                }else if(nodeName=="designResolution"){
                    mDesignResolution.width = n.attribute("width").as_int(1200);
                    mDesignResolution.height = n.attribute("height").as_int(800);

                    mDesignResolution.widthMin = n.attribute("widthMin").as_int(mDesignResolution.width);
                    mDesignResolution.widthMax = n.attribute("widthMax").as_int(mDesignResolution.width);
                    if(mDesignResolution.widthMin != mDesignResolution.width ||
                       mDesignResolution.widthMax != mDesignResolution.width)
                    {
                        mDesignResolution.useWidthMinMax = true;
                    }

                    mDesignResolution.heightMin = n.attribute("heightMin").as_int(mDesignResolution.height);
                    mDesignResolution.heightMax = n.attribute("heightMax").as_int(mDesignResolution.height);
                    if(mDesignResolution.heightMin != mDesignResolution.height ||
                       mDesignResolution.heightMax != mDesignResolution.height)
                    {
                        mDesignResolution.useHeightMinMax = true;
                    }


                }else if(nodeName=="scenes"){
                    for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                        std::string childNodeName = std::string(nChild.name());

                        if(childNodeName=="scene"){
                            std::string name = nChild.attribute("name").as_string("");
                            Scene * scene = new PlayedScene(name, app);
                            scenes().push_back(scene);

                            if(nChild.attribute("startingScene").empty()==false){
                                bool startingScene = nChild.attribute("startingScene").as_bool(false);
                                if(startingScene){
                                    setActiveScene(scene);
                                }
                            }
                        }
                    }

                }else if(nodeName=="language"){
                    if(loadApplication_LanguagesCfg(n, dbgText)==false){
                        return false;
                    }

                }
            }

        }

    }else{
        dbgText = "Path error: " + filePath;
    }

    jugimap::settings.SetScreenSize(Vec2i(mDesignResolution.width, mDesignResolution.height));

    //---
    if(scenes().empty()){
        dbgSystem.addMessage("No scenes found in '" + mName + "' (file '" + filePath + "')!");
        return false;

    }

    dbgSystem.removeLastMessage();

    return true;

}



bool PlayedApp::loadApplicationGlobalData()
{

    std::string filePath = CurrentProject.gameCfgDir + "/_application.xml";

    //---
    dbgSystem.addMessage("Loading application global data from file '_application.xml' ...");


    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());


    if(result.status==pugi::xml_parse_status::status_ok){

        pugi::xml_node rootNode = doc.child("application");
        if(rootNode.empty()==false){
            mName = rootNode.attribute("name").as_string("MyGame");

            for(pugi::xml_node n = rootNode.first_child(); n; n = n.next_sibling()){
                std::string nodeName = std::string(n.name());

                if(nodeName=="variable"){

                    std::string varName = n.attribute("name").as_string("");
                    std::string varValue = n.attribute("value").as_string("");
                    std::string varType = n.attribute("type").as_string("");
                    VariableType type = GetVariableTypeFromString(varType);

                    if(globalVariables()->addVariable(varName, varValue, type)==nullptr){
                        return false;
                    }

                }else if(nodeName=="inputCommands"){
                    if(inputSystem()->initCfg(n)==false){
                        return false;
                    }
                }
            }
        }
    }

    jugimap::settings.SetScreenSize(Vec2i(mDesignResolution.width, mDesignResolution.height));

    //---
    if(scenes().empty()){
        dbgSystem.addMessage("No scenes found in '" + mName + "' (file '" + filePath + "')!");
        return false;

    }

    dbgSystem.removeLastMessage();

    return true;

}



bool PlayedApp::loadScene(PlayedScene *_scene, LoadedContent _loadedContent)
{

    std::string dirPath = CurrentProject.gameCfgDir;

    if(ncine::FileSystem::isDirectory(dirPath.c_str())==false){
        return false;
    }

    ncine::FileSystem::Directory dir(dirPath.c_str());


    while(true){
        const char *_fileName = dir.readNext();
        if(_fileName==nullptr){
            break;
        }
        std::string fileName(_fileName);
        if(fileName.empty()==false && fileName.substr(0, 1)=="."){
             continue;
        }
        std::string filePath = dirPath + "/" + fileName;
        if(ncine::FileSystem::isDirectory(filePath.c_str())){

            // scene directory
            ncine::FileSystem::Directory dirScene(filePath.c_str());

            while(true){

                const char *_fileNameScene = dirScene.readNext();
                if(_fileNameScene==nullptr){
                    break;
                }
                std::string fileNameScene(_fileNameScene);
                if(fileNameScene.empty()==false && fileNameScene.substr(0, 1)=="."){
                     continue;
                }
                std::string filePathScene = filePath + "/" + fileNameScene;
                std::string suffix = StdString::suffixFromFilePath(filePathScene);

                if(ncine::FileSystem::isFile(filePathScene.c_str())){

                    std::string rootNodeName;
                    std::string rootNodeAttribute_name = "name";

                    if(rootNodeName_xml(filePathScene, rootNodeName, rootNodeAttribute_name)==false){
                        return false;
                    }

                    if(rootNodeName=="scene" && rootNodeAttribute_name==_scene->name()){

                        if(_loadedContent==LoadedContent::SCENE_COUNT_OBJECTS){

                             if(_scene->preloadScene_countObjects(filePathScene)==false){
                                return false;
                            }

                        }else if(_loadedContent==LoadedContent::SCENE_LOGIC_DATA){

                            if(_scene->loadScene_LogicData(filePathScene)==false){
                                return false;
                            }

                        }
                    }
                }
            }
        }
    }


    //---
    if(_loadedContent==LoadedContent::SCENE_LOGIC_DATA && _scene->sceneMaps().empty()){
        dbgSystem.addMessage("Error! Scene '"+_scene->name()+"' does not have defined any scene maps!");
        assert(false);
        return false;
    }

}


bool PlayedApp::rootNodeName_xml(const std::string &filePath, std::string &rootNodeName, std::string &rootNodeAttribute)
{

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());

    if(result.status==pugi::xml_parse_status::status_ok){
        for(pugi::xml_node n = doc.first_child(); n; n = n.next_sibling()){
            if(n.type()==pugi::node_element){
                rootNodeAttribute = n.attribute(rootNodeAttribute.c_str()).as_string("");

                rootNodeName = std::string(n.name());
            }
        }

    }else{
        dbgSystem.addMessage("Error reading xml file '" + filePath + "'");
        return false;
    }

    return true;
}


bool PlayedApp::loadApplication_LanguagesCfg(pugi::xml_node &node, std::string &dbgText)
{

    dbgSystem.addMessage("Loading languages cfg ...");


    std::string textDirectory = node.attribute("textDirectory").as_string("");
    std::string defLanguage = node.attribute("defLanguage").as_string("");

    std::string dirPath = CurrentProject.gameRootDir + "/" + textDirectory;
    //if(CurrentProject.gameRootDir.empty()){
    //    dirPath = textDirectory;
    //}

    if(ncine::FileSystem::isDirectory(dirPath.c_str())==false){
        dbgText = "Wrong directory: " + dirPath;
        dbgSystem.addMessage("Directory '" + dirPath + "' is not valid!");
        return false;
    }else{
        dbgText = "Ok directory: " + dirPath;
    }

    textLibrary.setRelativeTextsPath(textDirectory);

    ncine::FileSystem::Directory dir(dirPath.c_str());


    while(true){
        const char *_fileName = dir.readNext();
        if(_fileName==nullptr){
            break;
        }
        std::string fileName(_fileName);
        if(fileName.empty()==false && fileName.substr(0, 1)=="."){
             continue;
        }
        std::string filePath = dirPath + "/" + fileName;
        if(ncine::FileSystem::isDirectory(filePath.c_str())){

            textLibrary.languages().push_back(fileName);
            if(fileName==defLanguage){
                textLibrary.setActiveLanguage(fileName);
            }
        }
    }


    if(textLibrary.languages().empty()){
        dbgSystem.addMessage("No language directories found within defined text directory '" + textDirectory + "'!");
        return false;
    }
    if(textLibrary.activeLanguage().empty()){
        dbgSystem.addMessage("Default language directory '" +defLanguage+ "' not found!");
        return false;
    }


    dbgSystem.removeLastMessage();
    return true;
}




PlayedApp *app = nullptr;


}


