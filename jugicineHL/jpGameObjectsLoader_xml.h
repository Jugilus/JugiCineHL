#ifndef JM_GAME_OBJECTS_LOADER_XML_H
#define JM_GAME_OBJECTS_LOADER_XML_H


#include <string>
#include <vector>
#include "jmGlobal.h"


namespace pugi {
class xml_node;
}


namespace jugimap {


class App;
class Scene;
class PlayedApp;
class PlayedScene;
class SourceImage;
class SourceSprite;
class CompositeSprite;
class SourceSet;
class SourceGroup;
class SourceLibrary;
class TimelineAnimation;
class AnimationTrack;
class AnimationMember;
class FrameAnimation;
struct CParameter;
class StatesList;
class LogicState;
class Action;
class ItemsGroup;


class AppConfigurationLoader_xml
{
public:

    //bool loadGeneralData(App * _app);
    //bool loadLogicData(App * _app);

    bool loadApplicationBaseParameters(PlayedApp * app, std::string &dbgText);
    bool loadApplicationGlobalData(PlayedApp * app);

    bool loadSceneBaseData(Scene *scene);
    bool loadSceneLogicData(PlayedScene *playerScene);

private:


    bool loadScene_LogicData(jugimap::PlayedScene *playerScene, const std::string &sceneFilePath);
    bool loadScene_GeneralData(Scene *scene, const std::string &sceneFilePath);
    bool loadScene_SceneMaps(Scene *scene, pugi::xml_node &node);
    bool loadScene_OnInit(PlayedScene *playerScene, const pugi::xml_node &node);
    bool loadScene_OnStart(PlayedScene *playerScene, const pugi::xml_node &node);
    bool loadScene_OnUpdate(PlayedScene *playerScene, const pugi::xml_node &node);

    //bool loadEngineControllerCfg(PlayedScene *playerScene, EntityControllerCfg &enginesControllerCfg, const pugi::xml_node &node);
    //bool loadBehaviorStateCfg(BehaviorStateCfg &behaviorStateCfg, const pugi::xml_node &node);

    //bool loadTaskHandlerCfg(PlayedScene *playerScene, TaskControllerCfg &taskHandlerCfg, const pugi::xml_node &node);
    //bool loadSourceEntityCfg(PlayedScene *playerScene, SourceEntityCfg &sourceEntityCfg, const pugi::xml_node &node);

    //bool buildBehaviorStateAsDirectionCopy(BehaviorState *sourceState, BehaviorState *destState);

    //bool loadEnvironmentObjectBehaviorCfg(const std::string &sourceEntityName, EnvironmentObjectBehaviorCfg &cfg, const pugi::xml_node &node);

    //bool loadEntityCategories(PlayedScene *playerScene, const pugi::xml_node &node);

    bool loadGameState(LogicState *state, pugi::xml_node &node);
    bool loadScene_ItemsGroup(ItemsGroup *itemsGroup, pugi::xml_node &node);

    void readParameter(CParameter &pv, pugi::xml_node &node);
    std::string rootNodeName(const std::string &filePath, std::string &rootNodeAttribute);


    bool loadApplication_LanguagesCfg(PlayedApp * app, pugi::xml_node &node, std::string &dbgText);


    //App * mApp = nullptr;

};



extern AppConfigurationLoader_xml appConfigurationLoader_xml;




}













#endif // JMGameObjectsLoader_xml_H
