#include <ncine/Application.h>
#include <ncine/FileSystem.h>
#include "jmSourceObjectsLoader_xml.h"
#include "jmSourceContainers.h"
#include "jmFont.h"
#include "jmSystem.h"
#include "jmVectorShapeDrawing.h"

#include "gui/jpGuiCommon.h"
#include "jpGameObjectsLoader_xml.h"
#include "jpPlayedScene.h"
#include "jpPlayedScene.h"
#include "jpPlayedApp.h"

#include "jpPlayer.h"



namespace jugimap{




Player::Player()
{

    mApp.reset(new PlayedApp());
    app = mApp.get();               // set global pointer link

}



void Player::preInit()
{
    mApp->preInit();
}



void Player::init()
{
    mApp->init();

}


void Player::update(float _framePeriod)
{

    showMessage("Player::update");


    int debugDrawFlags = 0;
    if(mShowEntityShapes){
        debugDrawFlags |= DebugDrawFlags::SHAPES;
    }



    if(mApp->error()!= AppError::NONE){
        mState = State::ERROR;
    }


    if(mState==State::NORMAL){

        mApp->update(_framePeriod);


        mApp->drawShapes(debugDrawFlags);

        showMessage(mApp->message());


    }else if(mState==State::RELOAD_APP_PART_1){

        // Delete all source, map, logic data.... and reset possible dangling link pointers
        // After deleting the application should be ready to be initialized again.


        mApp->deleteContent();
        sourceLibrary.deleteContent();
        fontsLibrary.deleteContent();
        textColorsLibrary.deleteContent();
        DeleteGuiSupportingObjects();
        DeleteInputSupportingObjects();

        //mActiveScene = nullptr;
        mApp->_setInitializationStatus(AppInitializationStatus::APP_BASE_CONFIGURATION_DATA_LOADED);

        //---

        mApp->shapeDrawer()->begin();

        Vec2f pos(100, settings.GetScreenSize().y-100);
        mApp->shapeDrawer()->drawText("RELOADING GAME !", pos);

        mApp->shapeDrawer()->end();

        //---
        mState = State::RELOAD_APP_PART_2;

        mApp->update(_framePeriod);


    }else if(mState==State::RELOAD_APP_PART_2){

        mApp->init();
        mState = State::NORMAL;

        //---
        //mApp->entitySystem()->doDebugDraw(mShowEntityShapes);

    }else if(mState==State::ERROR){
        showErrorMessage();

       if(mApp->message().empty()==false){
           showMessage(mApp->message());
       }
    }





    //----
    if(mShowOverlay){

        mApp->shapeDrawer()->begin();

        mApp->shapeDrawer()->setOutlineColor(ColorRGBA(0,0,0,128));
        mApp->shapeDrawer()->drawRectFilled(BoundingBoxFloat(30,30, 300, 300));

        mApp->shapeDrawer()->end();

    }

    if(mShowGui && mState==State::NORMAL){


        if(ImGui::Begin("Player")){
            /*
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
            */

            ImGui::Checkbox("Show entity shapes", &mShowEntityShapes);
            //if( mApp->entitySystem()){
            //    mApp->entitySystem()->doDebugDraw(mShowEntityShapes);
            //}

            ImGui::Checkbox("Pause", &mPaused);
            mApp->setPaused(mPaused);


        }
        ImGui::End();
    }

}


/*
bool Player::updateActiveSceneLogic()
{


    PlayedScene* activePlayerScene = static_cast<PlayedScene*>(mApp->activeScene());

    std::unique_ptr<PlayedScene>temporarySceneForLogicData;
    temporarySceneForLogicData.reset(new PlayedScene(activePlayerScene->name(), mApp.get()));


    if(appConfigurationLoader_xml.loadSceneLogicData(temporarySceneForLogicData.get())==false){
        mState = State::ERROR;
        return false;
    }
    if(activePlayerScene->updateLogicData(temporarySceneForLogicData.get())==false){
        mState = State::ERROR;
        return false;
    }

    if(activePlayerScene->initConnections()==false){
        mState = State::ERROR;
        return false;
    }


    return true;

}
*/

void Player::showErrorMessage()
{

    ImVec2 size(600.0f, 400.0f);
    ImGui::SetNextWindowSize(size, ImGuiCond_None);
    //ImVec2 pos(settings.GetScreenSize().x/2 - size.x/2, settings.GetScreenSize().y/2 - size.y/2);
    //ImVec2 pos(700 - size.x/2, 450 - size.y/2);
    ImVec2 pos(300, 900 - size.y);
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

    }
    ImGui::End();

}


void Player::showMessage(const std::string &_text)
{

    return;

    ImVec2 size(600.0f, 400.0f);
    ImGui::SetNextWindowSize(size, ImGuiCond_None);
    ImVec2 pos(50, 900 - size.y);
    ImGui::SetNextWindowPos(pos, ImGuiCond_None);

    if(ImGui::Begin("Message!", nullptr, ImGuiWindowFlags_Modal)){
        ImGui::Text("%s", _text.c_str());
    }
    ImGui::End();

}



Player *player = nullptr;


}


