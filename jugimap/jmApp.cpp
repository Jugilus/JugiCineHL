#include <string>
#include <sstream>

#include "ncine/Application.h"
#include "ncine/IInputManager.h"
#include "ncine/FileSystem.h"
#include "jmSystem.h"
#include "jmGlobal.h"
#include "jmSourceObjectsLoader_xml.h"
//#include "jmGameObjectsBuilder.h"
#include "jmSourceContainers.h"
#include "jmSourceItem.h"
#include "jmSourceSprite.h"
#include "jmScene.h"
#include "jmSceneLayout.h"
#include "jmVectorShapeDrawing.h"
#include "jmMap.h"
#include "jmVectorLayer.h"
#include "jmSpriteLayer.h"
#include "jmFont.h"
#include "jmInput.h"
#include "jmSystemSourceObjects.h"
#include "jmCamera.h"
//#include "jmGuiCommon.h"
#include "jmLayerElement.h"
#include "jmApp.h"





namespace jugimap {





bool DesignResolution::operator==(DesignResolution &_other)
{

    if(width != _other.width) return false;
    if(height != _other.height) return false;
    if(useWidthMinMax != _other.useWidthMinMax) return false;
    if(widthMin != _other.widthMin) return false;
    if(widthMax != _other.widthMax) return false;
    if(useHeightMinMax != _other.useHeightMinMax) return false;
    if(heightMin != _other.heightMin) return false;
    if(heightMax != _other.heightMax) return false;

    return true;

}








App::App()
{
    //engineApp = objectFactory->NewEngineApp(this);
    //ncine::theApplication().rootNode().addChildNode(this);
     //type_ = ncine::Object::ObjectType::SCENENODE;
    mNode = new LayerNode(nullptr);
    mNode->app = this;



}




App::~App()
{

    jugimap::settings.SetAppTerminated(true);

    //delete engineApp;

    for(Scene* s : mScenes){
        delete s;
    }

    delete mNode;


    //--- delete other data
    sourceLibrary.deleteContent();
    fontsLibrary.deleteContent();
    textColorsLibrary.deleteContent();
    //DeleteGuiSupportingObjects();
    DeleteInputSupportingObjects();
    systemSourceObjects.deleteContent();

}


void App::deleteContent()
{

    for(Scene* s : mScenes){
        s->deleteContent();
    }

    mActiveScene = nullptr;

}


void App::update(float _framePeriod)
{

    if(mActiveScene==nullptr || mActiveScene->isBuilt()==false) return;

    ManageSuspendedTime(_framePeriod*1000);

    //if(mStartingUpdate){
    //    mStartingUpdate = mActiveScene->startingUpdate();

    //}


    //---
    mActiveScene->preUpdate();
    mActiveScene->update();
    mActiveScene->postUpdate();
}


Scene* App::getScene(const std::string & _name)
{

    for(Scene* s : mScenes){
        if(_name == s->name()){
            return s;
        }
    }

    return nullptr;
}



bool App::setActiveScene(Scene *_scene)
{

    assert(_scene);

    if(_scene==mActiveScene) return true;

    mActiveScene = _scene;

    //----
    /*

    if(mActiveScene->build()==false){
        return false;
    }

    DebugListSceneTree(mNode);


    //----
    if(mActiveScene->init()==false){
        return false;
    }


    //---- reset time variables !
    passedTimeMS = passedTimeMSPrevious = time.UpdatePassedTimeMS();
    storedUpdatePeriods.clear();


    //----
    mActiveScene->start();

    */

    return true;

}



void App::ManageSuspendedTime(float _updatePeriodMS)
{

    //if(currentScene == nullptr) return;

    //return;     //this need to be examined when updating passed time via timeStep method


    //--- Verify frame time for abnormal values
    _updatePeriodMS = jugimap::ClampValue(_updatePeriodMS, 0.0f, 200.0f);     // safety clamp
    float avgUpdatePeriodMS = GetAverageUpdatePeriod(_updatePeriodMS);

    //DbgOutput("updatePeriodMS:" + std::to_string(_updatePeriodMS)+ " avgUpdatePeriodMS:"+std::to_string(avgUpdatePeriodMS));

    if(_updatePeriodMS > 3 * avgUpdatePeriodMS){
        _updatePeriodMS = avgUpdatePeriodMS;
        //DbgOutput("UpdatePeriodMS too big! Clamped to average value:"+std::to_string(avgUpdatePeriodMS));
    }

    time.SetLogicTimeMS(_updatePeriodMS);


    /*
    //--- Manage abnormal pauses due to application suspension or dragging the window around in Windows OS which stops updating.
    passedTimeMS = time.UpdatePassedTimeMS();
    int deltaPassedTimeMS = passedTimeMS - passedTimeMSPrevious;
    if(deltaPassedTimeMS > 10*_updatePeriodMS){                 // Big spike in frame time indicates abnormal pause.
        time.AddSuspendedTimeMS(deltaPassedTimeMS);
        //DbgOutput("Big Spike in passed time! Added to suspendedTimeMS:"+std::to_string(deltaPassedTimeMS));
    }
    passedTimeMSPrevious = passedTimeMS;
    */


    // Update scene logic
    //---------------------------------------
    //currentScene->PreUpdate();
    //currentScene->Update();
    //currentScene->PostUpdate();

}


/*
void App::UpdateViaFixedTimeStep(float _fixedTimeStepMS)
{

    if(currentScene == nullptr) return;


    time.SetLogicTimeMS(_fixedTimeStepMS);

    //----
    passedTimeMS = time.UpdatePassedTimeMS();

    double passedDeltaTimeMS = passedTimeMS - passedTimeMSPrevious;
    float frameTimeMS = jugimap::ClampValue(passedDeltaTimeMS, 0.0, 200.0);     // safety clamp
    float avgFrameTime = GetAverageUpdatePeriod(frameTimeMS);

    //DbgOutput("frameTimeMS:" + std::to_string(frameTimeMS)+ " avgFrameTime:"+std::to_string(avgFrameTime));

    // manage unexpected spikes in frame rate - due to suspension or dragging the window around in Windows (stops updating)
    if(passedDeltaTimeMS > avgFrameTime*10){
        frameTimeMS = avgFrameTime;
        time.AddSuspendedTimeMS(passedDeltaTimeMS);
        DbgOutput("Big Spike in passed time! Added to suspendedTimeMS:"+std::to_string(frameTimeMS));
    }
    passedTimeMSPrevious = passedTimeMS;

    accumulatorMS += frameTimeMS;

    //---
    while(accumulatorMS >= _fixedTimeStepMS){

        currentScene->PreUpdate();
        currentScene->Update();
        currentScene->PostUpdate();

        //---
        accumulatorMS -= _fixedTimeStepMS;
    }
    jugimap::settings.SetLerpDrawingFactor(accumulatorMS / _fixedTimeStepMS);

}
*/


float App::GetAverageUpdatePeriod(float _currentUpdatePeriodMS)
{

    if(storedUpdatePeriods.size()<sizeStoredUpdatePeriods-1){
        storedUpdatePeriods.push_back(_currentUpdatePeriodMS);

    }else{
        for(int i=0; i<storedUpdatePeriods.size()-1; i++){
            storedUpdatePeriods[i] = storedUpdatePeriods[i+1];
        }
        storedUpdatePeriods[storedUpdatePeriods.size()-1] = _currentUpdatePeriodMS;

    }

    float sumFrameTime = 0.0;
    for(int i=0; i<storedUpdatePeriods.size(); i++){
        sumFrameTime += storedUpdatePeriods[i];
    }

    float avgUpdatePeriodMS = sumFrameTime/storedUpdatePeriods.size();

    return avgUpdatePeriodMS;

}


/*
void App::AddShapeDrawer(Drawer *_drawer)
{
    shapeDrawers.push_back(_drawer);
}
*/

void App::onResizeWindow(int width, int height)
{

    settings.SetScreenSize(Vec2i(width, height));
    for(Scene* s : mScenes){
        s->scaleToScreenSize();
    }


}



void App::drawShapes(int _flags)
{

    if(mActiveScene==nullptr) return;
    if(mShapeDrawer==nullptr) return;


    //if(mShapeDrawer.mShapes.empty()) return;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(jugimap::settings.GetScreenSize().x, jugimap::settings.GetScreenSize().y));

    ImGui::Begin("My shapes", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);
    mShapeDrawer->mDrawList = ImGui::GetWindowDrawList();

    //---

    //mShapeDrawer.clearDrawingList();

    for(Scene* s : mScenes){
        s->drawShapes(*mShapeDrawer, _flags);
    }

    //mShapeDrawer.drawShapes();


    //---
    ImGui::End();



}


//App *gameApp = nullptr;

void DebugListSceneTree(ncine::SceneNode* node, int level)
{
    if(node->type()==ncine::Object::ObjectType::SCENENODE){

        if(dynamic_cast<LayerNode*>(node) && static_cast<LayerNode*>(node)->app){

            App* app = static_cast<LayerNode*>(node)->app;

            std::string spacing;
            for(int i=0; i<=level; i++){
                spacing += "  ";
            }

            print( spacing +  "app");

            if(app->scenes().empty()==false){
                for(Scene* s: app->scenes()){
                    DebugListSceneTree(s->node(), level+1);
                }
            }

        }else if(dynamic_cast<LayerNode*>(node) && static_cast<LayerNode*>(node)->scene){

            Scene* scene = static_cast<LayerNode*>(node)->scene;

            std::string spacing;
            for(int i=0; i<=level; i++){
                spacing += "  ";
            }

            print( spacing +  "scene:" + scene->name());

            if(scene->isBuilt()==false){
                print( spacing +  "Scene was not built!");

            }else{
                if(scene->sceneMaps().empty()==false){
                    for(SceneMap *sm : scene->sceneMaps()){
                        DebugListSceneTree(sm->map()->node(), level+1);
                    }
                }
            }



        }else if(dynamic_cast<LayerNode*>(node) && static_cast<LayerNode*>(node)->layerElement->kind()==LayerKind::MAP){

            Map* m = static_cast<Map*>(static_cast<LayerNode*>(node)->layerElement);

            std::string spacing;
            for(int i=0; i<=level; i++){
                spacing += "  ";
            }

            print( spacing +  m->name());

            if(node->children().isEmpty()==false){

                for(ncine::SceneNode *n : node->children()){
                    DebugListSceneTree(n, level+1);
                }
            }

        }else if(dynamic_cast<LayerNode*>(node) &&
                 (static_cast<LayerNode*>(node)->layerElement->kind()==LayerKind::SPRITE_LAYERS_GROUP)){

            SpriteLayersGroup* lg = static_cast<SpriteLayersGroup*>(static_cast<LayerNode*>(node)->layerElement);

            std::string spacing;
            for(int i=0; i<=level; i++){
                spacing += "  ";
            }

            print( spacing +  lg->name());

            if(node->children().isEmpty()==false){

                for(ncine::SceneNode *n : node->children()){
                    DebugListSceneTree(n, level+1);
                }
            }

        }else if(dynamic_cast<LayerNode*>(node) &&
                 (static_cast<LayerNode*>(node)->layerElement->kind()==LayerKind::VECTOR_LAYERS_GROUP)){

            VectorLayersGroup* lg = static_cast<VectorLayersGroup*>(static_cast<LayerNode*>(node)->layerElement);

            std::string spacing;
            for(int i=0; i<=level; i++){
                spacing += "  ";
            }

            print( spacing +  lg->name());

            if(node->children().isEmpty()==false){

                for(ncine::SceneNode *n : node->children()){
                    DebugListSceneTree(n, level+1);
                }
            }

        }else if(dynamic_cast<LayerNode*>(node) &&
                 dynamic_cast<Layer*>(static_cast<LayerNode*>(node)->layerElement)){

            Layer* l = static_cast<Layer*>(static_cast<LayerNode*>(node)->layerElement);

            std::string spacing;
            for(int i=0; i<=level; i++){
                spacing += "  ";
            }

            print( spacing +  l->name()+" : " + std::to_string(node->children().size())+" children");

            if(node->children().isEmpty()==false){

                for(ncine::SceneNode *n : node->children()){
                    DebugListSceneTree(n, level+1);
                }
            }

        }

    }

}


void SetSystemMouseCursorVisible(bool _visible)
{
    if(_visible){
        ncine::theApplication().inputManager().setMouseCursorMode(ncine::IInputManager::MouseCursorMode::NORMAL);
    }else{
        ncine::theApplication().inputManager().setMouseCursorMode(ncine::IInputManager::MouseCursorMode::HIDDEN);
    }

}



}
