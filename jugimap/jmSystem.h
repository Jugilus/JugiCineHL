#ifndef JUGIMAP_SYSTEM_H
#define JUGIMAP_SYSTEM_H

#include <chrono>
#include <string>
#include "ncine/Rect.h"
#include "jmGlobal.h"
#include "jmCommon.h"






namespace jugimap {




struct TEProjectInfo
{
    std::string gameRootDir;               // game root path
    std::string sourceCfgDir;
    std::string gameCfgDir;

};


extern TEProjectInfo CurrentProject;


///\brief Settings
///
/// The Settings class stores jugimap configuration parameters.
class Settings
{
public:


    //void SetEngine(Engine _engine){ engine = _engine; }
    //Engine GetEngine(){ return engine; }



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Set the screen size to the given *_screenSize*.
    ///
    /// This function must be called at the game initialization before loading any maps.
    ///
    /// \see GetScreenSize
    void SetScreenSize(Vec2i _screenSize){ screenSize = _screenSize; }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Returns the screen size.
    ///
    /// \see SetScreenSize
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Vec2i GetScreenSize(){ return screenSize; }

    //---------------

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Set to *true* if the y coordinate in the used game engine points up.
    ///
    /// This function must be called at the game initialization before loading any maps.
    /// The default value is *false*.
    ///
    ///\see IsYCoordinateUp
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //void SetYCoordinateUp(bool _yCoordinateUp){ yCoordinateUp = _yCoordinateUp;}


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Returns true if the y coordinate points up; if it points down returns false.
    ///
    /// \see SetYCoordinateUp
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bool IsYCoordinateUp()
    //{

//#ifdef JUGI_EDITOR

//    return false;

//#else
    //return yCoordinateUp;

//#endif

    //}

   Yorientation yOrientation(){ return mYorientation; }


   void setYorientation(Yorientation _yOrientation ){ mYorientation = _yOrientation; }


   float appScale(){ return mAppScale; }


   void setAppScale(float _appScale) { mAppScale = _appScale; }


    //---------------

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Set to *true* if the Collider objects should be used for the sprites collision detection.
    ///
    /// This option is usable only for engines which provide collision system.
    /// The function must be called at the game initialization before loading any maps.
    /// Collider objects are used for the Sprite interface functions for collision detection. Some engines offer a collision system which is not
    /// available via colliders. In that case you can disable colliders and use solely the engine methods for all collision
    /// detection tasks in your game.
    ///
    /// The default value is *true*.
    ///
    /// \see SpriteCollidersEnabled
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void EnableSpriteColliders(bool _spriteCollidersEnabled){ spriteCollidersEnabled = _spriteCollidersEnabled; }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Returns true if the Collider objects are used for the sprites collision detection; otherwise returns false.
    ///
    /// \see EnableSpriteColliders
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool SpriteCollidersEnabled(){ return spriteCollidersEnabled; }

    //---------------

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Set to *false* if the engine physics are available in the used engine but disabled.
    ///
    /// This parameter has no effect with engines which does not provide a physics simulation or it can not be disabled.
    ///
    /// The default value is *true*.
    ///
    /// \see EnginePhysicsEnabled
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void EnableEnginePhysics(bool _enginePhysicsEnabled){ enginePhysicsEnabled = _enginePhysicsEnabled; }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Returns *true* if the engine physics system is enabled; otherwise returns *false*.
    ///
    /// \see EnableEnginePhysics
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool EnginePhysicsEnabled(){ return enginePhysicsEnabled; }

    //---------------

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Set to *true* to enable the *lerp drawing* technique; otherwsie set to false.
    ///
    /// This option can be used with engines which update game logic with fixed time step.
    ///
    /// *Lerp drawing* describes a technique for reducing the stutter in scrolling or sprite movements which occurs
    /// when the game logic rate and the game drawing rate are not synchronized.

    /// The *lerp drawing* means that we obtain the actual drawing position of sprites as an interpolated point between
    /// the previous and the current position. The in-between point is calculated via the **lerpFactor** which should be set in the fixed rate logic
    /// of the main game loop.
    ///
    /// An important function related to *lerp drawing* is a function called **CaptureForLerpDrawing*. It is available for maps, layers and sprites.
    /// Capture for *lerp drawing* means that you set the sprite's previous position variable to the same value as the current position.
    /// This function must be called at initialization and whenever a sprite changes
    /// its position abruptly - the change is not a part of animated (smooth) movement.
    ///
    /// If the *CaptureForLerpDrawing* is not called correctly there may be weird sprite placements in the game.
    /// It may be a good idea to temporary disable *useLerpDrawing* during development and when everything works as intended
    /// we activate it to improve smoothness.
    ///
    /// The default value is *false*.
    ///
    /// \see LerpDrawingEnabled
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //void EnableLerpDrawing(bool _lerpDrawingEnabled){ lerpDrawingEnabled = _lerpDrawingEnabled; }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Returns *true* if the *lerp drawing* technique is used; otherwise returns *false*.
    ///
    /// see EnableLerpDrawing
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bool LerpDrawingEnabled(){ return lerpDrawingEnabled; }

    //---------------

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Set the interpolating factor for *lerp drawing*.
    ///
    /// The *lerp drawing factor* is needed if *lerp drawing* is active and the engine supports it.
    /// **Important:** This function must be called in the game update function in order to regularly provide the current factor.
    ///
    /// \see GetLerpDrawingFactor
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //void SetLerpDrawingFactor(float _lerpDrawingFactor){ lerpDrawingFactor = _lerpDrawingFactor; }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Returns the interpolating factor for *Lerp Drawing*.
    ///
    ///\see SetLerpDrawingFactor
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //float GetLerpDrawingFactor(){ return lerpDrawingFactor; }



    //---------------

    ///\brief Set the global error message.
    //void SetErrorMessage(const std::string &_errorMessage){ errorMessage = _errorMessage; }

    //void AddToErrorMessage(const std::string &_errorMessage){ errorMessage = errorMessage + _errorMessage; }

    ///\brief Returns the global error message.
    //std::string &GetErrorMessage(){ return errorMessage; }



    //void addErrorMessage(const std::string &_errorMessage){ mErrorMessages.push_back(_errorMessage); }



    //---------------
    ///\brief Set the *z-order* step. Used by engines which use a z-order factor for drawing order of sprites.
    void SetZOrderStep(int _zOrderStep){ zOrderStep = _zOrderStep; }

    ///\brief Returns the *z-order* step.
    int GetZOrderStep(){ return zOrderStep; }



    //---------------
    ///\brief Set the starting *map z-order*. Used by engines which use a z-order factor for drawing order of sprites.
    void SetMapZOrderStart(int _mapZOrderStart){ mapZOrderStart = _mapZOrderStart; }

    ///\brief Returns the starting *map z-order*.
    int GetMapZOrderStart(){ return mapZOrderStart; }


    //---------------
    ///\brief Set the *map z-order* step. Used by engines which use a z-order factor for drawing order of sprites.
    void SetMapZOrderStep(int _mapZOrderStep){ mapZOrderStep = _mapZOrderStep; }

    ///\brief Returns the *map z-order* step.
    int GetMapZOrderStep(){ return mapZOrderStep; }


    ///\brief Mark application as terminated on exit.
    void SetAppTerminated(bool _appTerminated){ appTerminated = _appTerminated;}


    ///\brief Returns true if application is marked as terminated; otherwise returns false.
    bool IsAppTerminated(){ return appTerminated; }

    void setHideInvisibleDummies(bool _hide){ mHideInvisibleDummies = _hide; }
    bool hideInvisibleDummies(){ return mHideInvisibleDummies; }


    //---------------
    ///\brief Returns the sign of rotation. This sign depends of the direction of y coordinate.
    //int GetRotationSignForYdir(){ return (yCoordinateUp)? 1 : -1; }


protected:

    Engine engine = Engine::NOT_DEFINED;

    Vec2i screenSize;
    float mAppScale = 1.0f;
    bool mHideInvisibleDummies = true;
    //bool yCoordinateUp;

    Yorientation mYorientation = Yorientation::DOWN;

    bool spriteCollidersEnabled = true;
    //bool lerpDrawingEnabled = false;
    bool enginePhysicsEnabled = true;
    std::string errorMessage;
    //float lerpDrawingFactor = 1.0;
    int zOrderStep = 10;
    int mapZOrderStart = 10000;
    int mapZOrderStep = 1000;

    bool appTerminated = false;



};


/// The Settings object.
extern Settings settings;


///\brief Time
///
/// The Time class stores required time parameters.
class Time
{
public:


    ///\brief Updates and returns the passed time in milliseconds.
    ///
    /// This function must called at the begining of logic update.
    /// \see GetPassedNetTimeMS
    double UpdatePassedTimeMS()
    {
        /*
        if(started==false){
            initTimePoint = std::chrono::high_resolution_clock::now();
            started = true;
        }
        passedMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - initTimePoint).count();
        return passedMicroseconds/1000.0;
        */

        if(started==false){
            initTimePoint = std::chrono::high_resolution_clock::now();
           // passedMicroseconds = 0;
            started = true;
            //return passedMicroseconds/1000.0; ;
            passedTimeS = 0.0;
            return 0.0;
        }

        static long long int passedMicrosecondsChrono = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - initTimePoint).count();


        passedTimeS += mTimeStepS;
        return passedTimeS * 1000.0f;
    }


    ///\brief Add suspended time in milliseconds.
    ///
    /// Suspended time is time when application is suspended. This function is called from SceneManager::Update and should not be called manually.
    void AddSuspendedTimeMS(int _suspendedTimeMS)
    {
        //suspendedTimeMS += _suspendedTimeMS;
    }



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Returns passed net time in milliseconds.
    ///
    /// This function returns the net passed time which is the full passed time with deducted suspended time.
    ///\see UpdatePassedTimeMS()
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int GetPassedNetTimeMS();

    //int GetPassedNetTimeS();




    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Set the logic time step in milliseconds.
    ///
    /// This function must called at the begining of logic update.
    /// Depending of the engine this time step can be the frame time or fixed logic time step.
    ///\see GetLogicTimeMS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void SetLogicTimeMS(float _logicTimeStepMS){ logicTimeStepMS = _logicTimeStepMS; }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Returns the frame time in milliseconds.
    ///
    ///\see SetLogicTimeMS
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    float GetLogicTimeMS(){ return logicTimeStepMS; }



    void setTimeStep(float _timeStep){ mTimeStepS = _timeStep; }
    float timeStep(){ return mTimeStepS; }




private:
    std::chrono::high_resolution_clock::time_point initTimePoint;
    //unsigned long long passedMicroseconds = 0;
    //unsigned long long passedMicroseconds = 0;
    double passedTimeS = 0.0;

    int suspendedTimeMS = 0;
    bool started = false;
    float logicTimeStepMS;
    float mTimeStepS = 1.0f/60.0f;

};


extern Time time;


//----------------------------------------------------------------------------------------



class DbgSystem
{
public:

    void addMessage(const std::string &_message);
    void removeLastMessage();


    std::vector<std::string>& errorMessages(){ return mErrorMessages; }

    void setActive(bool _active){ mActive = _active; }
    bool isActive(){ return mActive; }

private:
    bool mActive = true;
    std::vector<std::string>mErrorMessages;
};


extern DbgSystem dbgSystem;



//----------------------------------------------------------------------------------------



struct Y_Up_to_Y_Down
{

    static jugimap::BoundingBoxFloat convertRect(jugimap::BoundingBoxFloat r);

    static jugimap::BoundingBoxInt convertRect(jugimap::BoundingBoxInt r);

    static jugimap::BoundingBoxFloat convertRect(ncine::Rectf r);

    static jugimap::BoundingBoxInt convertRect(ncine::Recti r);
};






/// Delete global jugimap objects.
///
/// This function must be called at the application exits.
//void DeleteGlobalObjects();



/// @}*    //end of group Globals



}

#endif


