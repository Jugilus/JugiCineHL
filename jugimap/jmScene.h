#ifndef JUGIMAP_SCENE_H
#define JUGIMAP_SCENE_H

#include "ncine/SceneNode.h"
#include <chrono>
#include "jmCommon.h"
#include "jmGlobal.h"
#include "jmUsingDeclarations.h"



namespace jugimap {

class ShapeDrawer;
class SceneMap;
//class GuiWidget;
class Map;
class LayerNode;
//class WidgetManager;
class App;





///\ingroup MapElements
///\brief The Scene class is the base class for the application scenes.
class Scene
{
public:
    static bool mUpdateWidgetsOnPreUpdate;


    ///\brief Constructor
    Scene(const std::string &_name, App* _parent);

    ///\brief Destructor.
    virtual ~Scene();

    /// Delete content of the scene (scene maps, logic data). The scene's node is not deleted but it is left without child nodes.
    virtual void deleteContent();


    virtual bool init(){ return true; }
    virtual void start(){};
    virtual bool startingPhaseUpdate(){ return false; }
    virtual void update(){}
    virtual void preUpdate();
    virtual void postUpdate();

    virtual bool build();
    //virtual void unbuild();


    LayerNode* node(){ return mNode; }


    App* parentApp(){ return mParentApplication; }

    bool isBuilt(){ return mBuilt; }

    const std::string & name(){ return mName; }

    std::vector<SceneMap*>& sceneMaps(){ return mSceneMaps; }

    SceneMap* getSceneMap(const std::string &_name);
    Map* getMap(const std::string &_sceneMapName);


    void addSceneMap(SceneMap* _sceneMap){ mSceneMaps.push_back(_sceneMap); }


    virtual void drawShapes(ShapeDrawer &drawer, int flags=0){}


    void _setParentApp(App *_app){ mParentApplication = _app; }
    void _setName(const std::string &_name){ mName = _name; }


    //WidgetManager* widgetManager(){ return mWidgetManager; }


    void scaleToScreenSize();
    void hideInvisibleDummies();


protected:

    bool buildSceneMaps();
    void setViewports();



    friend class App;
    friend class GameObjectsBuilder;

#ifdef JUGI_EDITOR
    friend class GameObjectsWriter_xml;
#endif

    LayerNode *mNode = nullptr;                      // owned

    std::string mName;
    App* mParentApplication = nullptr;

    bool mBuilt = false;

    std::vector<SceneMap*>mSceneMaps;                       // owned
    //WidgetManager* mWidgetManager = nullptr;                // OWNED

    Map* getMapWithRelativeFilePath(const std::string &_relativeFilePath);


};



#ifdef JUGI_EDITOR

extern Scene *CurrentScene;

#endif



}

#endif


