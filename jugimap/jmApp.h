#ifndef JUGIMAP_APP_H
#define JUGIMAP_APP_H

#include <memory>
#include "ncine/SceneNode.h"
#include "ncine/TextNode.h"
#include <chrono>
#include "jmCommon.h"
#include "jmVectorShapeDrawing.h"
#include "jmUsingDeclarations.h"



namespace jugimap {


class ShapeDrawer;
class Scene;
class SceneManager;
class EngineApp;
class LayerNode;



struct DesignResolution
{
    int width = 1200;
    int height = 800;

    bool useWidthMinMax = false;
    int widthMin = 1100;
    int widthMax = 1300;

    bool useHeightMinMax = false;
    int heightMin = 800;
    int heightMax = 800;


    bool operator==(DesignResolution &_other);
    bool operator!=(DesignResolution &_other){ return !operator==(_other); }

    Vec2i designResolution(){ return Vec2i(width, height); }


};




///\ingroup MapElements
///\brief The App is the base class of the application.
class App
{
public:


    ///\brief Constructor.
    App();

    ///\brief Destructor.
    virtual ~App();

    virtual void deleteContent();


    ///\brief Initialize the application.

    virtual void preInit(){}
    virtual void init(){}
    ///\brief Update the application logic. The given *_framePeriod* is a period of one frame in seconds.
    virtual void update(float _framePeriod);
    virtual void drawShapes(int _flags = 0);

    void onResizeWindow(int width, int height);


    ///\brief Set the active scene.
    virtual bool setActiveScene(Scene *_scene);

    ///\brief Returns the active scene or nullptr if none.
    Scene* activeScene(){ return mActiveScene; }

    Scene* getScene(const std::string & _name);

    ///\brief Returns a reference to the vector of stored scenes.
    std::vector<Scene*>& scenes(){ return mScenes; }

    const std::string & name(){ return mName; }
    const std::string & saveDataDirName(){ return mSaveDataDirName; }

    DesignResolution &designResolution(){ return mDesignResolution; }

    LayerNode *node(){ return mNode; }

    ShapeDrawer* shapeDrawer(){ return mShapeDrawer.get(); }

    const std::string &saveDataDir(){ return mSaveDataDir; }




protected:

    virtual void ManageSuspendedTime(float _updatePeriodMS);


    ///\brief Update
    /// the active scene with the given fixed rate update time *_updatePeriodMS*.
    ///
    /// This function can be used with engines which allow developers to manually manage the game root loop.
    //virtual void UpdateViaFixedTimeStep(float _fixedTimeStepMS);


//private:
    friend class AppConfigurationLoader_xml;
#ifdef JUGI_EDITOR
    friend class GameObjectsWriter_xml;
    friend class jugi::gui::TApplicationDialog;
#endif

    std::string mName = "My Game";
    std::string mSaveDataDirName = "MyGame";
    std::string mSaveDataDir;
    DesignResolution mDesignResolution;

    LayerNode *mNode = nullptr;                     // OWNDED

    std::vector<Scene*>mScenes;                     // OWNED
    Scene* mActiveScene = nullptr;                  // LINK

    std::unique_ptr<ShapeDrawer>mShapeDrawer;

    //bool mStartingUpdate = true;

    //---
    double passedTimeMS = 0;
    double passedTimeMSPrevious = 0;
    std::vector<float>storedUpdatePeriods;
    int sizeStoredUpdatePeriods = 8;
    float accumulatorMS = 0.0;


    float GetAverageUpdatePeriod(float _currentUpdatePeriodMS);


    //ncine::TextNode *errorMessageLabel = nullptr;       // LINK

};





void DebugListSceneTree(ncine::SceneNode* node, int level=0);


void SetSystemMouseCursorVisible(bool _visible);



//extern App *gameApp;




/*

///\ingroup MapElements
///\brief The EngineApp class defines engine specific application properties.
class EngineApp
{
public:

    EngineApp(App *_app) : app(_app){}
    virtual ~EngineApp(){}


    ///\brief Interface function for running engine specific code before the application initialization.
    virtual void PreInit(){}


    ///\brief Interface function for running engine specific code after the application initialization.
    virtual void PostInit(){}


    ///\brief Set the visibility of the system mouse cursor.
    virtual void SetSystemMouseCursorVisible(bool _visible);


    ///\brief Returns the application object.
    App* GetApp(){ return app; }


protected:
    App *app = nullptr;             // LINK

};
*/


}

#endif


