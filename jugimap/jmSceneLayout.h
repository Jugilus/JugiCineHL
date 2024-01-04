#ifndef JM_SCENE_LAYOUT_H
#define JM_SCENE_LAYOUT_H



#include "jmCommon.h"
#include "jmGlobal.h"
#include "jmParameter.h"
#include "jmUsingDeclarations.h"


namespace ncine {
class Viewport;
}

namespace pugi {
class xml_node;
}


namespace jugimap {

class Scene;
class Map;
class App;
class Camera;



struct SceneMapInfo
{

    bool loadCfg(const pugi::xml_node &_node);



    bool operator==(SceneMapInfo &other);
    bool operator!=(SceneMapInfo &other) { return !operator==(other); }


    std::string name;
    std::string mapRelativeFilePath;
    std::string worldSceneMapForParallaxMap;


    // viewport
    int xMinRel = 0;
    int xMinAbs = 0;

    int yMinRel = 0;
    int yMinAbs = 0;

    int xMaxRel = 100;
    int xMaxAbs = 0;

    int yMaxRel = 100;
    int yMaxAbs = 0;

    MapType mapType = MapType::WORLD;
    //CameraKind cameraKind = CameraKind::WORLD;

    CParameters parameters;

    //bool visibleOnStart = true;



    BoundingBoxInt viewportRect(Vec2i appDesignResolution);
    BoundingBoxFloat viewportRectFloat(Vec2i appDesignResolution);

    BoundingBoxInt viewportRect(BoundingBoxInt appBoundingBox);

};




class SceneMap
{
public:


    SceneMapInfo info;



    ~SceneMap();


    void deleteContent();


    bool isVisible();
    void setVisible(bool _visible);



    Map* map(){ return mMap; }
    ncine::Viewport *viewport(){ return mNcineViewport; }
    Camera * camera(){ return mCamera; }


private:
    friend class GameObjectsBuilder;
    friend class Scene;

    //---

    bool mSharedMap = false;
    Map *mMap = nullptr;                                   // ?
    Camera *mCamera = nullptr;
    ncine::Viewport *mNcineViewport = nullptr;
    //---

};











}

#endif


