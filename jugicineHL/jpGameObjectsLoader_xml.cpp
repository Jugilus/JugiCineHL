#include <memory>
#include <ncine/FileSystem.h>
#include "pugixml/pugixml.hpp"

#include "jmSystem.h"
#include "jmCommon.h"
#include "jmCommonFunctions.h"
#include "jmGlobal.h"
#include "jmScene.h"
#include "jmApp.h"
#include "jmMap.h"
#include "jmTextSprite.h"
#include "jmSceneLayout.h"
#include "jmStreams.h"
#include "jmText.h"

#include "items/jpItemsCommon.h"
#include "items/jpItemSlotData.h"
#include "logic/jpLogicState.h"
#include "logic/jpFunction.h"
#include "logic/jpTransition.h"
#include "data/jpItem.h"

#include "components/jpComponentsCommon.h"
#include "jpSceneLogicState.h"
#include "jpObjectFactory.h"
#include "jpGfxObjectsCommon.h"
#include "jpCompound.h"
#include "jpUserSelector.h"
#include "jpInputCustomizer.h"
#include "jpInput.h"
#include "jpPlayedApp.h"
#include "jpPlayedScene.h"

#include "jpGameObjectsLoader_xml.h"



namespace jugimap {




/*
bool AppConfigurationLoader_xml::loadApplicationBaseParameters(PlayedApp *app, std::string &dbgText)
{

    std::string filePath = CurrentProject.gameCfgDir + "/_application.xml";


    //---
    dbgSystem.addMessage("load application base parameters from file '_application.xml'");


    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());


    if(result.status==pugi::xml_parse_status::status_ok){
        dbgText = "OK Path : " + filePath;

        pugi::xml_node rootNode = doc.child("application");
        if(rootNode.empty()==false){
            app->mName = rootNode.attribute("name").as_string("MyGame");

            for(pugi::xml_node n = rootNode.first_child(); n; n = n.next_sibling()){
                std::string nodeName = std::string(n.name());

                if(nodeName=="saveDataDirName"){
                    app->mSaveDataDirName = n.attribute("name").as_string("jugicineGame");

                }else if(nodeName=="designResolution"){
                    app->mDesignResolution.width = n.attribute("width").as_int(1200);
                    app->mDesignResolution.height = n.attribute("height").as_int(800);

                    app->mDesignResolution.widthMin = n.attribute("widthMin").as_int(app->mDesignResolution.width);
                    app->mDesignResolution.widthMax = n.attribute("widthMax").as_int(app->mDesignResolution.width);
                    if(app->mDesignResolution.widthMin != app->mDesignResolution.width ||
                       app->mDesignResolution.widthMax != app->mDesignResolution.width)
                    {
                        app->mDesignResolution.useWidthMinMax = true;
                    }

                    app->mDesignResolution.heightMin = n.attribute("heightMin").as_int(app->mDesignResolution.height);
                    app->mDesignResolution.heightMax = n.attribute("heightMax").as_int(app->mDesignResolution.height);
                    if(app->mDesignResolution.heightMin != app->mDesignResolution.height ||
                       app->mDesignResolution.heightMax != app->mDesignResolution.height)
                    {
                        app->mDesignResolution.useHeightMinMax = true;
                    }


                }else if(nodeName=="scenes"){
                    for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                        std::string childNodeName = std::string(nChild.name());

                        if(childNodeName=="scene"){
                            std::string name = nChild.attribute("name").as_string("");
                            Scene * scene = new PlayedScene(name, app);
                            app->scenes().push_back(scene);

                            if(nChild.attribute("startingScene").empty()==false){
                                bool startingScene = nChild.attribute("startingScene").as_bool(false);
                                if(startingScene){
                                    app->setActiveScene(scene);
                                }
                            }
                        }
                    }

                }else if(nodeName=="language"){
                    if(loadApplication_LanguagesCfg(app, n, dbgText)==false){
                        return false;
                    }

                }
            }

        }

    }else{
        dbgText = "Path error: " + filePath;
    }

    jugimap::settings.SetScreenSize(Vec2i(app->mDesignResolution.width, app->mDesignResolution.height));

    //---
    if(app->scenes().empty()){
        dbgSystem.addMessage("No scenes found in '" + app->name() + "' (file '" + filePath + "')!");
        return false;

    }


    dbgSystem.removeLastMessage();

    return true;

}
*/


/*
bool AppConfigurationLoader_xml::loadApplicationGlobalData(PlayedApp *app)
{

    std::string filePath = CurrentProject.gameCfgDir + "/_application.xml";


    //---
    dbgSystem.addMessage("load application global data from file '_application.xml'");


    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());


    if(result.status==pugi::xml_parse_status::status_ok){

        pugi::xml_node rootNode = doc.child("application");
        if(rootNode.empty()==false){
            app->mName = rootNode.attribute("name").as_string("MyGame");

            for(pugi::xml_node n = rootNode.first_child(); n; n = n.next_sibling()){
                std::string nodeName = std::string(n.name());

                if(nodeName=="variable"){

                    std::string varName = n.attribute("name").as_string("");
                    std::string varValue = n.attribute("value").as_string("");
                    std::string varType = n.attribute("type").as_string("");
                    VariableType type = GetVariableTypeFromString(varType);


                    if(app->globalVariables()->addVariable(varName, varValue, type)==nullptr){
                        return false;
                    }


                }else if(nodeName=="inputCommands"){

                    if(app->inputSystem()->initCfg(n)==false){
                        return false;
                    }

                }
            }
        }
    }

    jugimap::settings.SetScreenSize(Vec2i(app->mDesignResolution.width, app->mDesignResolution.height));

    //---
    if(app->scenes().empty()){
        dbgSystem.addMessage("No scenes found in '" + app->name() + "' (file '" + filePath + "')!");
        return false;

    }

    dbgSystem.removeLastMessage();

    return true;

}
*/


/*

bool AppConfigurationLoader_xml::loadSceneBaseData(Scene *scene)
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

                    std::string attibName = "name";
                    bool errorReadingFile = false;
                    std::string rootName = rootNodeName(filePathScene, attibName, errorReadingFile);

                    if(errorReadingFile){
                        return false;
                    }

                    if(rootName=="scene" && attibName==scene->name()){
                        if(loadScene_GeneralData(scene, filePathScene)==false){
                            return false;
                        }
                    }
                }
            }
        }
    }


    //---
    if(scene->sceneMaps().empty()){
        dbgSystem.addMessage("Error! Scene '"+scene->name()+"' does not have defined any scene maps!");
        assert(false);
        return false;
    }


    return true;
}


bool AppConfigurationLoader_xml::loadScene_GeneralData(Scene *scene, const std::string &sceneFilePath)
{

    dbgSystem.addMessage("Reading xml file '" + sceneFilePath +"' ...");

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(sceneFilePath.c_str());


    if(result.status==pugi::xml_parse_status::status_ok){

        pugi::xml_node rootNode = doc.child("scene");
        if(rootNode.empty()==false){

            scene->_setName(rootNode.attribute("name").as_string(scene->name().c_str()));

            //---
            dbgSystem.addMessage("Loading scene '" + scene->name() + "' base logic data from file '" + sceneFilePath +"' ...");

            //---
            for(pugi::xml_node n = rootNode.first_child(); n; n = n.next_sibling()){

                std::string nodeName = std::string(n.name());

                if(nodeName=="sceneMaps"){
                    if(loadScene_SceneMaps(scene, n)==false){
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


bool AppConfigurationLoader_xml::loadScene_SceneMaps(Scene *scene, pugi::xml_node &node)
{

    dbgSystem.addMessage("Loading 'scene maps' ...");


    for(pugi::xml_node n = node.first_child(); n; n = n.next_sibling()){
        if(std::string(n.name())=="sceneMap"){

            SceneMap * sm = new SceneMap();
            sm->info.name = n.attribute("name").value();
            sm->info.mapRelativeFilePath = n.attribute("mapFile").value();
            sm->info.mapType = GetMapTypeFromString(n.attribute("mapType").as_string("World"));
            sm->info.worldSceneMapForParallaxMap = n.attribute("worldSceneMapForParrallaxMap").as_string("");

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childName = std::string(nChild.name());
                if(childName=="viewport"){
                    sm->info.xMinRel = nChild.attribute("xMinRel").as_int(0);
                    sm->info.xMinAbs = nChild.attribute("xMinAbs").as_int(0);
                    sm->info.yMinRel = nChild.attribute("yMinRel").as_int(0);
                    sm->info.yMinAbs = nChild.attribute("yMinAbs").as_int(0);
                    sm->info.xMaxRel = nChild.attribute("xMaxRel").as_int(100);
                    sm->info.xMaxAbs = nChild.attribute("xMaxAbs").as_int(0);
                    sm->info.yMaxRel = nChild.attribute("yMaxRel").as_int(100);
                    sm->info.yMaxAbs = nChild.attribute("yMaxAbs").as_int(0);

                }else if(childName=="parameter"){
                    sm->info.parameters.push_back(CParameter());
                    CParameter &p = sm->info.parameters.back();
                    readParameter(p, nChild);
                }
            }

            //---
            scene->sceneMaps().push_back(sm);
        }
    }

    if(scene->sceneMaps().empty()){
        dbgSystem.addMessage("No maps found in scene '" + scene->name() +"'!");
    }


    dbgSystem.removeLastMessage();
    return true;
}


bool AppConfigurationLoader_xml::loadSceneLogicData(PlayedScene *playerScene)
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

                    std::string attibName = "name";
                    //std::string rootName = rootNodeName(filePathScene, attibName);
                    bool errorReadingFile = false;
                    std::string rootName = rootNodeName(filePathScene, attibName, errorReadingFile);

                    if(errorReadingFile){
                        return false;
                    }

                    if(rootName=="scene" && attibName==playerScene->name()){

                        if(loadScene_LogicData(playerScene, filePathScene)==false){
                            return false;
                        }

                    //}else if(rootName=="__errorReadingXmlFile"){

                    //    //---
                    //    dbgSystem.addMessage("Error reading xml file '" + filePathScene + "'");
                    //    return false;
                    }
                }
            }
        }
    }


    return true;

}


bool AppConfigurationLoader_xml::loadScene_LogicData(PlayedScene *playerScene, const std::string &sceneFilePath)
{

    dbgSystem.addMessage("Reading xml file '" + sceneFilePath +"' ...");
    //std::string filePath = CurrentProject.gameCfgDir + "/_scenes/" + scene->mName + ".xml";

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(sceneFilePath.c_str());


    DbgSystem & dbgSystemTMP = dbgSystem;


    if(result.status==pugi::xml_parse_status::status_ok){

        pugi::xml_node rootNode = doc.child("scene");
        if(rootNode.empty()==false){

            playerScene->_setName(rootNode.attribute("name").as_string(playerScene->name().c_str()));

            //---
            dbgSystem.addMessage("Loading scene '" + playerScene->name() + "' main logic data from file '" + sceneFilePath +"' ...");
            //dbgSystem.addMessage("rootNode = '" + std::string(rootNode.name())+ "'  first child node ='" +  std::string(rootNode.first_child().name()) + "'");

            //---
            //for(pugi::xml_node n = rootNode.first_child(); n; n = n.next_sibling()){
            //    dbgSystem.addMessage("child node '" + std::string(n.name()) + "'");
            //}
            //---
            for(pugi::xml_node n = rootNode.first_child(); n; n = n.next_sibling()){

                std::string nodeName = std::string(n.name());

                if(nodeName.length()>5 && nodeName.substr(nodeName.length()-5)=="_SKIP"){
                    continue;
                }

                if(nodeName=="sceneMaps"){

                    // already managed


                }else if(nodeName=="init"){

                    SceneLogicStateCfg * cfg = playerScene->initStateCfg();

                    if(cfg->initCfg(n)==false){
                        return false;
                    }

                    //if(loadScene_OnInit(playerScene, n)==false){
                    //    return false;
                    //}

                }else if(nodeName=="start"){

                    SceneLogicStateCfg * cfg = playerScene->startStateCfg();

                    if(cfg->initCfg(n)==false){
                        return false;
                    }

                    //if(loadScene_OnStart(playerScene, n)==false){
                    //    return false;
                    //}

                }else if(nodeName=="update"){

                    SceneLogicStateCfg * cfg = playerScene->updateStateCfg();

                    if(cfg->initCfg(n)==false){
                        return false;
                    }

                    //if(loadScene_OnUpdate(playerScene, n)==false){
                    //    return false;
                    //}


                //------
                }else if(nodeName=="itemsGroup"){
                    ItemsGroup *ig = new ItemsGroup(n.attribute("name").as_string(""), playerScene);
                    if(loadScene_ItemsGroup(ig, n)==false){
                        delete ig;
                        return false;
                    }
                    playerScene->itemsLibrary()->addItemsGroup(ig);


                //------


                //------
                }else if(nodeName=="transitions"){

                    for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                        std::string childNodeName = std::string(nChild.name());

                        if(childNodeName=="transition"){
                            Transition * t = new Transition(playerScene);
                            if(t->initCfg(nChild)==false){
                                delete t;
                                return false;
                            }
                            playerScene->transitionsGroup()->transitions().push_back(t);
                        }
                    }


                //------
                }else if(nodeName=="components"){

                    for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                        std::string childNodeName = std::string(nChild.name());

                        Component *c = actionFactory->createComponent(childNodeName, nChild);

                        if(c){
                            playerScene->componentsGroup()->components().push_back(c);
                        }else{
                            dbgSystem.addMessage("Component '" + childNodeName + "' not found!");
                            return false;
                        }
                    }

                    //if(loadScene_Components(playerScene, n)==false){
                    //    return false;
                    //}

                }else if(nodeName=="compounds"){

                    if(playerScene->compoundStorage()->initCfg(playerScene, n)==false){
                        return false;
                    }

                //------



                }else if(nodeName=="items"){

                    SourceItemsStorage *sourceItemsStorage = playerScene->sourceItemsStorage();

                    if(sourceItemsStorage->initCfg(n)==false){
                        return false;
                    }


                //------
                }else if(nodeName=="variable"){

                    std::string varName = n.attribute("name").as_string("");
                    std::string varValue = n.attribute("value").as_string("");
                    std::string varType = n.attribute("type").as_string("");
                    VariableType type = GetVariableTypeFromString(varType);
                    if(playerScene->variables().addVariable(varName, varValue, type)==nullptr){
                        return false;
                    }


                //------
                }else if(nodeName=="trigger"){
                    std::string tName = n.attribute("name").as_string("");
                    if(playerScene->triggers().addTrigger(tName)==nullptr){
                        return false;
                    }

                }else{

                    // check for custom components

                    Component * c = playerScene->componentsGroup()->getComponent(nodeName, false);

                    if(c==nullptr){

                        CustomComponentFactoryManager *m = app->customComponentFactoryManager();
                        c = m->createCustomComponent(nodeName, false);

                        if(c){
                            playerScene->componentsGroup()->components().push_back(c);
                        }else{

                            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
                            return false;
                        }
                    }

                    if(c->initCfg(playerScene, n)==false){
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

*/


/*
bool AppConfigurationLoader_xml::loadScene_OnInit(PlayedScene *playerScene, const pugi::xml_node &node)
{

    dbgSystem.addMessage("Loading 'init' ...");

    for(pugi::xml_node n = node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

            // auto actions
            Action* a = new Action("", playerScene->initState());
            if(a->createTriggersAndCommands_anonymousAction(n)==false){
                return false;
            }
            playerScene->initState()->actions().push_back(a);
        //}
    }

    dbgSystem.removeLastMessage();

    return true;
}


bool AppConfigurationLoader_xml::loadScene_OnStart(PlayedScene *playerScene, const pugi::xml_node &node)
{

     dbgSystem.addMessage("Loading 'start' ...");

    for(pugi::xml_node n = node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

            // auto actions
            Action* a = new Action("", playerScene->startState());
            if(a->createTriggersAndCommands_anonymousAction(n)==false){
                return false;
            }
            playerScene->startState()->actions().push_back(a);

        //}
    }

    dbgSystem.removeLastMessage();

    return true;
}
*/


/*
bool AppConfigurationLoader_xml::loadScene_OnUpdate(PlayedScene *playerScene, const pugi::xml_node &node)
{

     dbgSystem.addMessage("Loading 'update' ...");

    for(pugi::xml_node n = node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="state"){
            LogicState* state = new LogicState(n.attribute("name").as_string(), playerScene->updateState());
            if(loadGameState(state, n)==false){
                return false;
            }

            playerScene->updateState()->childStates().push_back(state);

       }else{

        }
    }

    dbgSystem.removeLastMessage();

    return true;
}
*/


/*
bool AppConfigurationLoader_xml::loadGameState(LogicState *state, pugi::xml_node &node)
{

    dbgSystem.addMessage("loadGameState '" + state->name() +"'");


    for(pugi::xml_attribute a = node.first_attribute(); a; a = a.next_attribute()){
        std::string attributeName =std::string(a.name());

        if(attributeName=="name"){

        }else if(attributeName=="compound"){
            state->mCompoundName = a.as_string();

        }else{

            dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + std::string(node.name()) +"' !");
            return false;
        }
    }



    for(pugi::xml_node n = node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="use"){

            //---- WIDGETS
            std::string widgets = n.attribute("widgets").as_string("");
            StdString::appendNonEmptyString(widgets, n.attribute("buttons").as_string(""), ",");
            StdString::appendNonEmptyString(widgets, n.attribute("sliders").as_string(""), ",");
            StdString::appendNonEmptyString(widgets, n.attribute("bars").as_string(""), ",");
            StdString::appendNonEmptyString(widgets, n.attribute("textFields").as_string(""), ",");
            StdString::appendNonEmptyString(widgets, n.attribute("textInputs").as_string(""), ",");
            StdString::appendNonEmptyString(widgets, n.attribute("tables").as_string(""), ",");

            if(widgets.empty()==false){
                state->mWidgetNames = new std::vector<std::string>();
                std::vector<std::string>widgetNames = StdString::splitString(widgets, ",");
                for(const std::string &s : widgetNames){
                    if(StdVector::contains(*state->mWidgetNames, s)==false){
                        state->mWidgetNames->emplace_back(s);
                    }
                }
            }

            //---- COMPONENTS
            std::string components = n.attribute("components").as_string("");

            if(components.empty()==false){
                state->mComponentNames = new std::vector<std::string>();
                std::vector<std::string>componentNames = StdString::splitString(components, ",");
                for(const std::string &s : componentNames){
                    if(StdVector::contains(*state->mComponentNames, s)==false){
                        state->mComponentNames->emplace_back(s);
                    }
                }
            }


        }else if(nodeName=="state"){        // child states
            //LogicState* childState = new LogicState(n.attribute("name").as_string(), state);
            //if(loadGameState(childState, n)==false){
            //    return false;
            //}
            //state->childStates().push_back(childState);

        }else{

            // anonymous actions

            Action* a = new Action(state);

            //if(a->createTriggersAndCommands_anonymousAction(n)==false){
            //    delete a;
            //    return false;
            //}

            state->mActions.push_back(a);
        }
    }

    dbgSystem.removeLastMessage();

    return true;
}
*/


/*
bool AppConfigurationLoader_xml::loadScene_ItemsGroup(ItemsGroup *itemsGroup, pugi::xml_node &node)
{

    dbgSystem.addMessage("load items for items group '" + itemsGroup->name() +"'");


    for(pugi::xml_node n = node.first_child(); n; n = n.next_sibling()){

        std::string nodeName = std::string(n.name());

        if(nodeName=="item"){

            GameItem *i = new GameItem(n.attribute("name").as_string(""), itemsGroup);

            for(pugi::xml_node nChild = n.first_child(); nChild; nChild = nChild.next_sibling()){
                std::string childModeName = std::string(nChild.name());

                if(childModeName=="guiSlotData"){

                    i->mGuiSlotData = new GuiSlotData(i);

                    if(i->mGuiSlotData->createSlotDataComponents(nChild) == false){
                        return false;
                    }

                }else if(childModeName=="onSelected"){

                    //i->mDoOnSelected = new Action("onItemSelected", i);
                    //if(i->mDoOnSelected->createCommands_anonymousAction(nChild) == false){
                    //    return false;
                    //}

                }
            }

            //---
            itemsGroup->items().push_back(i);

        }
    }

    dbgSystem.removeLastMessage();

    return true;

}
*/


/*
void AppConfigurationLoader_xml::readParameter(CParameter &pv, pugi::xml_node &node)
{

    pv.name = node.attribute("name").value();
    pv.value = node.attribute("value").value();

    pv.type = CParameter::typeFromString(node.attribute("type").value());
    if(pv.type==CParameter::typeBOOLEAN){
        pv.active = (StdString::stringToInt(pv.value, 1)==0)? false : true;
        pv.value = "";
    }

    pv.valueMin = node.attribute("min").value();
    pv.valueMax = node.attribute("max").value();

    std::string _valuesSet = node.attribute("valuesSet").value();
    if(_valuesSet.empty()==false){
        pv.valuesSet = StdString::splitString(_valuesSet, ",");
    }

    if(pv.type!=CParameter::typeBOOLEAN){
        if(node.attribute("active").empty()==false){
            pv.active = node.attribute("active").as_bool(true);
        }
    }

}

*/


/*
bool AppConfigurationLoader_xml::loadApplication_LanguagesCfg(PlayedApp * app, pugi::xml_node &node, std::string &dbgText)
{

    dbgSystem.addMessage("load languages cfg");


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
*/


/*
std::string AppConfigurationLoader_xml::rootNodeName(const std::string &filePath, std::string &rootNodeAttribute, bool &error)
{

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());

    if(result.status==pugi::xml_parse_status::status_ok){
        for(pugi::xml_node n = doc.first_child(); n; n = n.next_sibling()){
            if(n.type()==pugi::node_element){
                rootNodeAttribute = n.attribute(rootNodeAttribute.c_str()).as_string("");

                return std::string(n.name());
            }
        }

    }else{
        error = true;
        dbgSystem.addMessage("Error reading xml file '" + filePath + "'");
    }

    return "";
}
*/


//AppConfigurationLoader_xml appConfigurationLoader_xml;




}
