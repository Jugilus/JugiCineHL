#include <string>
#include <sstream>
#include <ncine/Application.h>
#include <ncine/Viewport.h>
#ifdef JUGI_EDITOR
#include "eFramework/projectManager.h"
#endif
#include "jmSystem.h"
#include "jmMapLoader_bin.h"
#include "jmSourceSprite.h"
#include "jmLayerElement.h"
#include "jmUtilities.h"
#include "jmMap.h"
#include "jmSprite.h"
#include "jmFont.h"
#include "jmInput.h"
#include "jmCamera.h"
#include "jmGuiCommon.h"
#include "jmVectorShapeDrawing.h"
#include "jmSceneLayout.h"
#include "jmApp.h"
#include "jmScene.h"



namespace jugimap {



/*
Scene::Scene()
{
    engineScene = objectFactory->NewEngineScene(this);
}
*/

bool Scene::mUpdateWidgetsOnPreUpdate = true;


Scene::Scene(const std::string &_name, App *_parent)
{

    mName = _name;
    mParentApplication = _parent;
    mNode= new LayerNode(nullptr);
    mNode->scene = this;


    //mParentApplication = gameApp;

}


//Scene::Scene(App * _app)
//{
//    type_ = ncine::Object::ObjectType::SCENENODE;
//    mParentApplication = _app;
//}



Scene::~Scene()
{

    if(mWidgetManager){
        delete mWidgetManager;
    }

    for(SceneMap * sm : mSceneMaps){
        delete sm;
    }

    delete mNode;

}


void Scene::deleteContent()
{

    //ncine::Viewport *viewport = &ncine::theApplication().rootViewport();
    //viewport->setNextViewport(nullptr);

    //ncine::Viewport *viewport = &ncine::theApplication().screenViewport();


    if(mWidgetManager){
        delete mWidgetManager;
    }

    ncine::Viewport::chain().clear();

    //----
    for(SceneMap * sm : mSceneMaps){
        //sm->deleteContent();
        delete sm;
    }
    mSceneMaps.clear();


    mBuilt = false;

    //----
    //if(mCustomScene){
    //    delete mCustomScene;
    //}

}


/*
bool Scene::Init()
{


    bool initialized = false;

    if(mCustomScene){
        initialized = mCustomScene->Init();
    }

    return initialized;
}
*/


void Scene::start()
{
    //node()->update(1);

    //if(mCustomScene){
    //    mCustomScene->Start();
    //}
}



//void Scene::Update()
//{
    //if(mCustomScene){
    //    mCustomScene->Update();
    //}
//}


void Scene::preUpdate()
{
    //----
    if(mouse.cursorSprite()){
        mouse.cursorSprite()->setPosition(Vec2f(mouse.posX(), mouse.posY()));
    }


    guiKeyboardAndJoystickInput.Update();

    guiCursorDeviceInput._SetCursorScreenPosition(Vec2f(mouse.posX(), mouse.posY()));
    guiCursorDeviceInput._SetCursorPressed(mouse.isButtonPressed(MouseButton::LEFT));
    guiCursorDeviceInput._SetCursorDown(mouse.isButtonDown(MouseButton::LEFT));

    for(SceneMap *sm : mSceneMaps){
    //for(SceneNode *n : children()){
        //LayerNode *node = dynamic_cast<LayerNode*>(n);   assert(node);   assert(node->layerElement->kind()==LayerKind::MAP);

        //jugimap::Map *m = static_cast<jugimap::Map*>(node->layerElement);
        //if(m->isVisible()==false) continue;

        if(sm->isVisible()==false) continue;

        jugimap::Map *map = sm->map();

        if(map->isHidden()) continue;

        Vec2f cursorInMapPosition = map->camera()->MapPointFromScreenPoint(guiCursorDeviceInput.GetCursorScreenPosition());
        //m->_SetCursorInMapPosition(cursorInMapPosition);
        guiCursorDeviceInput._SetCursorInMapPosition(cursorInMapPosition);



        //if(mUpdateWidgetsOnPreUpdate){
        //    map->updateWidgets();
        //}
        if(mWidgetManager){
            mWidgetManager->updateWidgets();
        }

    }

    //----
    time.UpdatePassedTimeMS();

}


void Scene::postUpdate()
{


    //for(SceneNode *n : children()){
    //    LayerNode *node = dynamic_cast<LayerNode*>(n);   assert(node);   assert(node->layerElement->kind()==LayerKind::MAP);
    //    jugimap::Map *m = static_cast<jugimap::Map*>(node->layerElement);
    //    if(m->isHidden()) continue;
    for(SceneMap *sm : mSceneMaps){
        if(sm->isVisible()==false) continue;
        jugimap::Map *map = sm->map();
        if(map->isHidden()) continue;

        map->updateCameraDependentObjects();
    }


    //----
    //for(SceneNode *n : children()){
    //    LayerNode *node = dynamic_cast<LayerNode*>(n);   assert(node);   assert(node->layerElement->kind()==LayerKind::MAP);
    //    jugimap::Map *m = static_cast<jugimap::Map*>(node->layerElement);

    for(SceneMap *sm : mSceneMaps){
        sm->map()->camera()->ClearChangeFlags();
    }

    mouse.resetPerUpdate();
    keyboard.resetPerUpdate();
    touch.resetPerUpdate();
    for(Joystick &gc: joysticks) gc.resetPerUpdate();
    GuiWidget::ResetInteractedPerUpdate();


}








/*

void Scene::setCustomScene(CustomScene *_customScene)
{
    mCustomScene = _customScene;
    mCustomScene->mScene = this;

}
*/

bool Scene::build()
{

    if(mBuilt) return true;


    //----
    if(buildSceneMaps()==false){
        return false;
    }
    scaleToScreenSize();
    setViewports();
    hideInvisibleDummies();


    //----
    //if(mCustomScene){

    //    if(mCustomScene->Init()==false){
    //        return false;
    //    }
    //}

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



    mBuilt = true;

    return true;

}


// unbuild() do the following
//  - delete content of scene maps
//  - delete


/*
void Scene::unbuild()
{

    //if(mCustomScene){
    //    mCustomScene->unbuild();
    //}

    //----
    //ncine::Viewport *viewport = &ncine::theApplication().rootViewport();
    //viewport->setNextViewport(nullptr);

    //ncine::Viewport *viewport = &ncine::theApplication().screenViewport();
    ncine::Viewport::chain().clear();


    for(SceneMap * sm : mSceneMaps){
        sm->deleteContent();
    }

    mBuilt = false;
}
*/


bool Scene::buildSceneMaps()
{

    dbgSystem.addMessage("buildSceneMaps for scene '" + mName + "'");

    Vec2i designResolution = Vec2i(mParentApplication->designResolution().width, mParentApplication->designResolution().height);


    //---- load and init maps
    for(SceneMap* sm : mSceneMaps){

        //---check if duplicate maps are used for world or screen map (dual play screen)
        if(sm->info.mapType == MapType::WORLD){
            sm->mMap = getMapWithRelativeFilePath(sm->info.mapRelativeFilePath);
            if(sm->mMap){
                sm->mSharedMap = true;
            }
        }

#ifdef JUGI_EDITOR
        if(sm->mMap==nullptr){
            Map* map = projectManager.getMapByRelativeFilePath(sm->info.mapRelativeFilePath);
            if(map){
                sm->mMap = new Map(*map);
            }
        }
#endif

        if(sm->mMap==nullptr){
            std::string mapFilePath = CurrentProject.gameRootDir + "/" + sm->info.mapRelativeFilePath;
            if(CurrentProject.gameRootDir.empty()){
                mapFilePath = sm->info.mapRelativeFilePath;
            }

            sm->mMap = new Map(sm->info.name);
            if(mapLoader_bin.loadMapFile(mapFilePath, sm->mMap)==false){
                dbgSystem.addMessage("Error loading map '" + mapFilePath + "' ! Error message: " + mapLoader_bin.loadMessage);
                return false;
            }
            //scene->node()->addChildNode(sm->mMap->node());
        }

        if(sm->mSharedMap==false){
            sm->mMap->init(sm->info.mapType);
        }
    }


    //----
    for(SceneMap* sm : mSceneMaps){

        if(sm->info.mapType == MapType::PARALLAX) continue;

        BoundingBoxFloat viewportRect = BoundingBoxIntToFloat(sm->info.viewportRect(designResolution));
        ncine::Recti ncViewportRect(viewportRect.min.x, viewportRect.min.y,  viewportRect.GetWidth(), viewportRect.GetHeight());

        if(sm->info.mapType == MapType::WORLD){

            if(sm->mSharedMap==false){
                sm->mMap->initAsWorldMap();
            }
            //sm->mCamera = new WorldMapCamera(viewportRect, sm->mMap->worldMapSize());
            sm->mCamera = new WorldMapCamera();


        }else if(sm->info.mapType == MapType::SCREEN){

            Vec2i mapDesignSize = Vec2fToVec2i(viewportRect.GetSize());
            if(sm->info.name=="InfoMap"){

                sm->map()->updateGeometry();
                mapDesignSize = BoundingBoxFloatToInt(sm->map()->boundingBox()).GetSize();
            }
            sm->mMap->initAsScreenMap(mapDesignSize);
            //sm->mCamera = new ScreenMapCamera(mapDesignSize, viewportRect.min);
            sm->mCamera = new ScreenMapCamera();
        }

        //----
        sm->mMap->setCamera(sm->mCamera);

        sm->mNcineViewport = new ncine::Viewport();
        //sm->mNcineViewport->setViewportRect(ncViewportRect);
        //sm->mNcineViewport->setScissorRect(ncViewportRect);
        sm->mNcineViewport->setCamera(sm->mCamera->viewportCamera());
        //sm->mNcineViewport->setRootNode(sm->mMap->node());

        sm->setVisible(sm->info.parameters.boolValue("hidden")==false);

    }

    //---- build parallax maps
    for(SceneMap* sm : mSceneMaps){
        if(sm->info.mapType == MapType::PARALLAX){

            SceneMap* smWorld = getSceneMap(sm->info.worldSceneMapForParallaxMap);
            if(smWorld == nullptr){
                dbgSystem.addMessage("GameObjectsBuilder::buildScene error! World map: '" + sm->info.worldSceneMapForParallaxMap
                                         + "' for parallax map not found!");
                return false;
            }

            BoundingBoxFloat viewportRect = BoundingBoxIntToFloat(smWorld->info.viewportRect(designResolution));
            ncine::Recti ncViewportRect(viewportRect.min.x, viewportRect.min.y,  viewportRect.GetWidth(), viewportRect.GetHeight());

            sm->mMap->initAsParallaxMap(smWorld->mMap->worldMapSize());

            sm->mMap->setCamera(smWorld->mCamera);

            sm->mNcineViewport = new ncine::Viewport();
            //sm->mNcineViewport->setViewportRect(ncViewportRect);
            //sm->mNcineViewport->setScissorRect(ncViewportRect);
            sm->mNcineViewport->setCamera(smWorld->mCamera->viewportCamera());
            //sm->mNcineViewport->setRootNode(sm->mMap->node());

            sm->setVisible(sm->info.parameters.boolValue("hidden")==false);
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


void Scene::setViewports()
{


    /*
    ncine::Viewport *viewport = &ncine::theApplication().rootViewport();

    for(int i = int(mSceneMaps.size())-1; i>=0; i--){
        SceneMap* sm = mSceneMaps[i];

        viewport->setNextViewport(sm->viewport());
        viewport = sm->viewport();
    }
    */



    //ncine::Viewport *screenViewport = &ncine::theApplication().screenViewport();
    ncine::Viewport::chain().clear();

    for(int i = int(mSceneMaps.size())-1; i>=0; i--){
    //for(int i = 0; i<scene->sceneMaps().size(); i++){
        SceneMap* sm = mSceneMaps[i];

        ncine::Viewport::chain().pushBack(sm->mNcineViewport);
        //viewport->setNextViewport(sm->mNcineViewport);
        //viewport = sm->mNcineViewport;
    }




}


void Scene::scaleToScreenSize()
{

    Vec2i screenSize = settings.GetScreenSize();
    Vec2i designResolution = Vec2i(mParentApplication->designResolution().width, mParentApplication->designResolution().height);

    float r = screenSize.x / float(screenSize.y);

    BoundingBoxInt scaledDesignRect;

    Vec2i appDesignSize(mParentApplication->designResolution().width, mParentApplication->designResolution().height);
    float rDesign = appDesignSize.x / float(appDesignSize.y);
    float scale = 1.0f;

    if(r>rDesign){
        scale = screenSize.y/float(appDesignSize.y);
        scaledDesignRect.min.x = (screenSize.x - scale*appDesignSize.x)/2;
        scaledDesignRect.min.y = 0;
        scaledDesignRect.max.x = screenSize.x - (screenSize.x - scale*appDesignSize.x)/2;
        scaledDesignRect.max.y = screenSize.y;

    }else{
        scale = screenSize.x/float(appDesignSize.x);
        scaledDesignRect.min.x = 0;
        scaledDesignRect.min.y = (screenSize.y - scale*appDesignSize.y)/2;
        scaledDesignRect.max.x = screenSize.x;
        scaledDesignRect.max.y = screenSize.y - (screenSize.y - scale*appDesignSize.y)/2;
    }

    settings.setAppScale(scale);


    //---
    for(SceneMap* sm : mSceneMaps){

        if(sm->info.mapType == MapType::PARALLAX) continue;

        BoundingBoxInt viewportRectUnscaled = sm->info.viewportRect(designResolution);
        BoundingBoxInt viewportRect = sm->info.viewportRect(scaledDesignRect);
        ncine::Recti ncViewportRect(viewportRect.min.x, viewportRect.min.y,  viewportRect.GetWidth(), viewportRect.GetHeight());

        if(sm->info.mapType == MapType::WORLD){

            static_cast<WorldMapCamera*>(sm->camera())->Init(BoundingBoxIntToFloat(viewportRect), sm->map()->worldMapSize());


        }else if(sm->info.mapType == MapType::SCREEN){

            Vec2i mapDesignSize = viewportRect.GetSize();
            if(sm->info.name=="InfoMap"){

            //    sm->map()->updateGeometry();
                mapDesignSize = BoundingBoxFloatToInt(sm->map()->boundingBox()).GetSize();
            }
            static_cast<ScreenMapCamera*>(sm->camera())->Init(mapDesignSize, Vec2iToVec2f(viewportRect.min));
        }

        //sm->viewport()->camera()->setOrthoProjection(viewportRectUnscaled.min.x, viewportRectUnscaled.max.x, designResolution.y - viewportRectUnscaled.max.y, designResolution.y - viewportRect.min.y);

        //----
        //sm->viewport()->camera()->setOrthoProjection(0, ncViewportRect.w, 0, ncViewportRect.h);

        sm->viewport()->setViewportRect(ncViewportRect);
        sm->viewport()->setScissorRect(ncViewportRect);
        sm->viewport()->camera()->setOrthoProjection(ncViewportRect.x,ncViewportRect.x+ncViewportRect.w, ncViewportRect.y, ncViewportRect.y+ncViewportRect.h);


        print("scaleToScreenSize scene '" + mName +" ncViewportRect x=" + std::to_string(ncViewportRect.x) + " y="+ std::to_string(ncViewportRect.y) +
              "  w=" + std::to_string(ncViewportRect.w) + " h="+ std::to_string(ncViewportRect.h));

        //sm->viewport()->resize(ncViewportRect.w, ncViewportRect.h);
        //sm->viewport()->camera()->setOrthoProjection(0, screenSize.x, 0, screenSize.y);
        //sm->viewport()->camera()->setOrthoProjection(0, screenSize.x, 0, screenSize.y);
        //sm->viewport()->camera()->setOrthoProjection(viewportRect.min.x, viewportRect.max.x, screenSize.y - viewportRect.max.y, screenSize.y - viewportRect.min.y);
        //sm->viewport()->camera()->setOrthoProjection(viewportRect.min.x, viewportRect.max.x, viewportRect.min.y, viewportRect.max.y);

    }


    //--- parallax maps
    for(SceneMap* sm : mSceneMaps){
        if(sm->info.mapType == MapType::PARALLAX){

            SceneMap* smWorld = getSceneMap(sm->info.worldSceneMapForParallaxMap);
            assert(smWorld);

            BoundingBoxInt viewportRect = smWorld->info.viewportRect(scaledDesignRect);
            ncine::Recti ncViewportRect(viewportRect.min.x, viewportRect.min.y,  viewportRect.GetWidth(), viewportRect.GetHeight());

            sm->viewport()->setViewportRect(ncViewportRect);
            sm->viewport()->setScissorRect(ncViewportRect);


        }
    }




}


void Scene::hideInvisibleDummies()
{
    if(settings.hideInvisibleDummies()){

        std::vector<Sprite*>sprites;
        for(SceneMap *sm : mSceneMaps){
            CollectSpritesWithConstParameter(sm->map(), sprites, "invisibleDummy");
        }

        for(Sprite *s : sprites){
            s->setVisible(false);
        }
    }
}




SceneMap* Scene::getSceneMap(const std::string &_name)
{

    for(SceneMap* sm : mSceneMaps){
        if(sm->info.name == _name){
            return sm;
        }
    }

    return nullptr;

}


Map* Scene::getMap(const std::string &_sceneMapName)
{

    for(SceneMap* sm : mSceneMaps){
        if(sm->info.name == _sceneMapName){
            return sm->map();
        }
    }

    return nullptr;

}


//void Scene::drawShapes(ShapeDrawer &drawer)
//{

    //if(mCustomScene){
    //    mCustomScene->drawShapes(drawer);
    //}

    //for(ncine::SceneNode* n : children()){
    //    Map *m = static_cast<Map*>(n);
    //    m->drawShapes(drawer);
    //}

//}



Map* Scene::getMapWithRelativeFilePath(const std::string &_relativeFilePath)
{

    for(SceneMap* sm : mSceneMaps){
        if(sm->info.mapRelativeFilePath == _relativeFilePath){
            return sm->mMap;
        }
    }

    return nullptr;

}



#ifdef JUGI_EDITOR

Scene *CurrentScene = nullptr;

#endif



}
