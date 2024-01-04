#ifndef JUGIMAP_GLOBAL_H
#define JUGIMAP_GLOBAL_H

#include <chrono>
#include <string>
#include "ncine/Rect.h"
#include "jmCommon.h"






namespace jugimap {


/// \addtogroup Globals
///  @{







enum class Yorientation : int
{
    DOWN = 0,
    UP = 1,
    NOT_DEFINED = -1

};

std::string GetYorientationString(Yorientation _value);
Yorientation GetYorientationFromString(const std::string &_value);
Yorientation GetYorientationFromInt(int _value);




/// The kinds of file.
enum class FileKind : int
{
    SINGLE_IMAGE = 1,                   ///< Single image file.
    TILE_SHEET_IMAGE = 2,               ///< Tile sheet image file.
    SPRITE_SHEET_IMAGE = 3,             ///< %Sprite sheet image file.
    SPINE_FILE = 4,                     ///< Spine file.
    SPRITER_FILE = 5,                   ///< Spriter file.
    SPRITE_TEXT_CONTAINER = 6,          ///< Container of text sprite source items.
    BITMAP_FONT = 7,                    ///< Bitmap font file (.fnt)
    //DUMMY = 10,
    NOT_DEFINED = -1                     ///< The file kind not defined.
};

inline bool IsFileImage(FileKind _kind){ return _kind==FileKind::SINGLE_IMAGE || _kind==FileKind::TILE_SHEET_IMAGE || _kind==FileKind::SPRITE_SHEET_IMAGE;}
FileKind GetFileKindFromInt(int _kind);


/// The kinds of sprite. Used as identifier for Sprite objects.
enum class SpriteType : int
{
    STANDARD = 0,                       ///< Standard sprite - StandardSprite object.
    COMPOSITE = 1,                       ///< Composed sprite - ComposedSprite object.
    TEXT = 2,                           ///< Text sprite - TextSprite object.
    SPINE = 3,                          ///< Spine sprite - SkeletonSprite object.
    SPRITER = 4,                        ///< Spriter sprite - SkeletonSprite object.
    NOT_DEFINED = -1                     ///< The sprite kind not defined.
};

SpriteType GetSpriteTypeFromString(const std::string &_type);
std::string GetSpriteTypeString(SpriteType _type);

SpriteType GetSpriteTypeFromFileKind(FileKind _fileKind);



/// The types of map.
enum class MapType
{
    WORLD,                  ///< World map.
    PARALLAX,               ///< Parallax map.
    SCREEN,                 ///< Screen map.
    NOT_DEFINED
};


MapType GetMapTypeFromString(const std::string &_type);
std::string GetMapTypeString(MapType _type);



/// The kinds of layer. Used as identifier for Layer objects.
enum class LayerKind
{
    MAP,
    SPRITE_LAYERS_GROUP,
    VECTOR_LAYERS_GROUP,
    SPRITE_LAYER,                     ///< %Sprite layer - SpriteLayer object.
    VECTOR_LAYER,                     ///< Vector layer - VectorLayer object.
    NOT_DEFINED                     ///< The layer kind not defined.
};



/// The types of layer. Used to distinguish layers for world, parallax and screen maps.
enum class LayerType
{
    WORLD,
    PARALLAX,
    PARALLAX_STRETCHING_SINGLE_SPRITE,
    SCREEN,
    SCREEN_STRETCHING_SINGLE_SPRITE
};


enum class SpriteLayerContent : int
{
    GRID_SPRITES = 0,
    FREE_SPRITES = 1
};

SpriteLayerContent GetSpriteLayerContentFromInt(int _content);
SpriteLayerContent GetSpriteLayerContentFromString(const std::string &_content);
std::string GetSpriteLayerContentString(SpriteLayerContent _content);




/// The stretching variants.
enum class StretchingVariant
{
    XY_TO_WORLD_SIZE,
    XY_TO_VIEWPORT_SIZE
};


/// The align factors for the X direction.
enum class AlignX
{
    LEFT,                               ///< Align left.
    MIDDLE,                             ///< Align to middle.
    RIGHT                               ///< Align right.
};

/// The align factors for the Y direction.
enum class AlignY
{
    TOP,                                ///< Align top.
    MIDDLE,                             ///< Align to middle.
    BOTTOM                              ///< Align bottom.
};


/// The blend modes for the openGL pixel blending.
enum class Blend
{
    SOLID = 0,                              ///< Solid (cover) blend.
    ALPHA = 1,                              ///< Alpha blend.
    MULTIPLY = 2,                           ///< Multiply (darken) blend.
    ADDITIVE = 3,                           ///< Additive (lighten) blend.
    NOT_DEFINED = -1                        ///< The blend not defined.

};

Blend GetBlendFromString(const std::string &_blend);
Blend GetBlendFromInt(int _blend);
std::string GetBlendString(Blend _blend);



enum class AbsoluteOrRelativeKind
{
    ABS = 0,
    REL = 1

};

AbsoluteOrRelativeKind GetPositionKindFromString(const std::string &_positionKind);
AbsoluteOrRelativeKind GetPositionKindFromInt(int _positionKind);
std::string GetPositionKindString(AbsoluteOrRelativeKind _positionKind);




/// The blend modes for the shader based pixel blending which simulate photoshop blending modes.
enum class ColorOverlayBlend : int
{
    SIMPLE_MULTIPLY = 0,                    ///< Simple multiply (not a real blending).
    NORMAL = 1,                             ///< Normal mode.
    MULTIPLY = 2,                           ///< Multiply mode.
    LINEAR_DODGE = 3,                       ///< Linear dodge mode.
    COLOR = 4,                              ///< Color mode.
    NOT_DEFINED = -1                         ///< The blend not defined.
};


ColorOverlayBlend GetColorOverlayBlendFromString(const std::string &_blend);
ColorOverlayBlend GetColorOverlayBlendFromInt(int _blend);
std::string GetColorOverlayBlendString(ColorOverlayBlend _blend);







/// The kinds of font.
//enum class FontKind
//{
    //TRUE_TYPE,                          ///< True type font.
//    BITMAP_FONT__FNT,
//    NOT_DEFINED                         ///< The font kind not defined.
//};


/// The kinds of geometric shape. Used as identifier for GeometricShape and VectorShape objects.

/*
enum class ShapeKind
{
    POLYLINE,                   ///< Polyline - PolyLineShape object.
    BEZIER_POLYCURVE,           ///< Bezier polycurve - BezierShape object.
    ELLIPSE,                    ///< Ellipse - EllipseShape object.
    SINGLE_POINT,               ///< Single point - SinglePointShape object.
    NOT_DEFINED                 ///< The shape kind not defined.
};
*/



/// The camera kind.
enum class CameraKind : int
{
    SCREEN = 0,
    WORLD = 1,
    NOT_DEFINED = -1
};


//==================================================================================


enum class ShapeKind : int
{
    RECTANGLE = 1,
    POLYLINE = 2,
    BEZIER_POLYCURVE = 3,
    ELLIPSE = 4,
    SINGLE_POINT = 5,
    NOT_DEFINED = -1
};


std::string GetShapeKindStringForGUI(ShapeKind _kind);
std::string GetShapeKindStringForFile(ShapeKind _kind);
ShapeKind GetShapeKindFromInt(int _kind);
ShapeKind GetShapeKindFromFileString(const std::string &_kind);
//std::array<std::string, 6> shapeKindStrings = {"", "rectangle", "polyline", "bezier polycurve", "ellipse", "single point"};
//std::array<std::string, 6> TEShape::kindStringsForCfgFile = {"", "rectangle", "polyline", "bezierPolycurve", "ellipse", "singlePoint"};



//==================================================================================


enum class EasingKind : int
{
    LINEAR = 0,
    EASE_START_QUAD = 1,
    EASE_END_QUAD = 2,
    EASE_START_END_QUAD = 3,
    EASE_START_CUBIC = 4,
    EASE_END_CUBIC = 5,
    EASE_START_END_CUBIC = 6,
    EASE_START_SINE = 7,
    EASE_END_SINE = 8,
    EASE_START_END_SINE = 9,
    CONSTANT = 10
};


std::string GetEasingKindString(EasingKind _easingKind);
EasingKind GetEasingKindFromString(const std::string &_kind);
EasingKind GetEasingKindFromInt(int _kind);


//==================================================================================


/// The kinds of animation. Used as identifier for Animation objects.
enum class AnimationKind : int
{

    FRAME_ANIMATION = 0,            ///< Frame animation - FrameAnimation object.
    TIMELINE_ANIMATION = 1,         ///< Timeline animation - TimelineAnimation object.
    NOT_DEFINED = -1

};


/// The kinds of animation track. Used as identifier for AnimationTrack and AnimationKey classes.
enum class AnimationTrackKind : int
{
    TRANSLATION = 0,                ///< Translation.
    SCALING = 1,                    ///< Scaling.
    ROTATION = 2,                   ///< Rotation.
    ALPHA_CHANGE = 3,               ///< Alpha change.
    OVERLAY_COLOR_CHANGE = 4,       ///< Overlay color change.
    PATH_MOVEMENT = 5,              ///< Path movement.
    FRAME_CHANGE = 6,               ///< Frame change.
    FRAME_ANIMATION = 7,            ///< Frame animation.
    TIMELINE_ANIMATION = 8,         ///< Timeline animation.
    FLIP_HIDE = 9,                  ///< Flip, hide.
    META = 20,                      ///< Meta.
    NOT_DEFINED = -1                ///< Not defined.

};


AnimationTrackKind GetAnimationTrackKindFromInt(int _value);
std::string GetAnimationTrackKindString(AnimationTrackKind _kind);
AnimationTrackKind GetAnimationTrackKindFromString(const std::string &_kind);
int GetOrderOfAnimationTrackKind(AnimationTrackKind _kind);



/// The states of the animation player.
enum class AnimationPlayerState
{
    IDLE,                       ///< The player is idle and has assigned no animation instance.
    PLAYING,                    ///< The player is playing and updating animation instance.
    STALLED,                    ///< The player is playing but animation instance is not being updated. This a case when an animation is not being looped and remains in its end state.
    PAUSED,                     ///< The player has been paused.
    WAITING_TO_START,           ///< The player is waiting to start animation due to the *startDelay* parameter.
    WAITING_TO_REPEAT           ///< The player is waiting to repeat animation due to the *repeat* parameter.
};


std::string GetPlayingAnimationStateString(AnimationPlayerState _value);


///\ingroup Animation
/// \brief The bit-mask flags of animation player.
struct AnimationPlayerFlags
{

    static const int NONE = 0;                          ///< None.

    // Play
    static const int DISCARD_ANIMATION_QUEUE = 1;       ///< Stop and discard animations in queue and start the new animation immediately. This is an input flag for AnimationQueuePlayer::Play.
    static const int SKIP_SUBPLAYER_UPDATING = 2;       ///< Not available.



    // Return flags
    static const int PLAYER_STARTED = 1;                ///< The return flag for AnimationQueuePlayer::Play command. If the player already plays the given animation this flag is not returned.
    static const int PLAYER_UPDATED = 2;                ///< The return flag for AnimationQueuePlayer::Update command. If the player is in the in the idle state this flag is not returned.
    static const int PLAYER_STOPPED = 4;                ///< The return flag for AnimationQueuePlayer::Stop command. If the player is already in the idle state this flag is not returned.
    static const int ANIMATION_INSTANCE_UPDATED = 8;    ///< The return flag for AnimationQueuePlayer::Play and AnimationQueuePlayer::Update commands when the animation update has been performed.
    static const int ANIMATED_PROPERTIES_CHANGED = 16;  ///< The return flag for AnimationQueuePlayer::Play and AnimationQueuePlayer::Update commands when the animated properties have been changed.
    static const int META_KEY_CHANGED = 32;             ///< The return flag for AnimationQueuePlayer::Play and AnimationQueuePlayer::Update commands when the meta animation key changes.

};


//==================================================================================


/// The kinds of widget. Used as identifier for GuiWidget objects.
enum class WidgetType
{
    NOT_DEFINED,
    BUTTON,                 ///< Button - GuiButton object.
    SLIDER,                 ///< Slider - GuiSlider object.
    BAR,                    ///< Bar - GuiBar object.
    TEXT_FIELD,             ///< Text field - GuiTextField object.
    TEXT_INPUT,              ///< Text input - GuiTextInput object.
    SLOT,
    TABLE

};


WidgetType GetGuiWidgetKindFromString(const std::string &_kind);
std::string GetGuiWidgetKindString(WidgetType _kind);


/// Predefined handles for TextSprite objects.
enum class TextHandleVariant
{
    CENTER          = 0,
    LEFT_TOP        = 1,
    TOP             = 2,
    RIGHT_TOP       = 3,
    RIGHT           = 4,
    RIGHT_BOTTOM    = 5,
    BOTTOM          = 6,
    LEFT_BOTTOM     = 7,
    LEFT            = 8
};


std::string GetTextHandleVariantString(TextHandleVariant _handle);
TextHandleVariant GetTextHandleVariantFromString(const std::string &_handle);
TextHandleVariant GetTextHandleVariantFromInt(int _id);


/// \brief Convert TextHandleVariant to relative handle with coordinates in range 0 - 1.0.
Vec2f GetRelativeHandleFromTextHandleVariant(TextHandleVariant _thv, Yorientation _yOrientation);
void GetHandleFromTextHandleVariant(TextHandleVariant _thv, int width, int height, int &xHandle, int &yHandle, Yorientation _yOrientation);





namespace cons {

static const int MARKED = 1;
static const int PIXMAP_UPDATE_REQUIRED = 2;
static const int MARKED_FOR_DELETE = 4;
static const int GENERATE_SPRITE = 8;
static const int USED_IN_SPRITE = 16;
static const int NEW_SOURCE_IMAGE = 32;
static const int TEXT_LABEL_CHANGED = 64;
static const int SAVE_SOURCE_COMPOSITE_SPRITE = 128;



static const int NOT_USED_IN_SPRITE = 1024;

const int SKIP_SAVING_PARAMETERS = 1;
const int UPDATE_LAYERS_BOUNDING_BOXES = 4;


const int saveID_DUMMY_SIF = -10;
const int saveID_DUMMY_SIF_TEXT_SPRITE = -15;
const int saveID_COMPOSITE_SPRITE_SOURCE_IMAGE_DUMMY = -20;
const int saveID_DUMMY_EMPTY_COMPOSED_SPRITE = -30;
const int saveID_DUMMY_EMPTY_FRAME = -40;
const int saveID_DUMMY_MISSING_TEXTURE = -50;
const int saveID_DUMMY_MISSING_SOURCE_SPRITE = -60;

}


/// Engine identification value.
enum class Engine
{
    NOT_DEFINED,
    AGK,
    SFML,
    Cocos2Dx,
    nCine,
};


//==================================================================================================


/*
struct TEProjectInfo
{
    std::string gameRootDir;               // game root path
    //std::string gameRootDirRelative;
    std::string sourceCfgDir;
    //std::string sourceCfgDirRelative;
    std::string gameCfgDir;


};


extern TEProjectInfo CurrentProject;
//extern TEProjectInfo PreviousProject;
//extern TEProjectInfo StoredProject;
//extern TEProjectInfo LoadedProject;
//extern TEProjectInfo ReplacementProject;
//extern TEProjectInfo CurrentProjectStored;

//extern TEProjectInfo *CurrentProjectNEW;
//extern TEProjectInfo _currentProjectNew;



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
    void SetErrorMessage(const std::string &_errorMessage){ errorMessage = _errorMessage; }

    void AddToErrorMessage(const std::string &_errorMessage){ errorMessage = errorMessage + _errorMessage; }

    ///\brief Returns the global error message.
    std::string &GetErrorMessage(){ return errorMessage; }


    std::vector<std::string>& errorMessages(){ return mErrorMessages; }

    void addErrorMessage(const std::string &_errorMessage){ mErrorMessages.push_back(_errorMessage); }



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




    //---------------
    ///\brief Returns the sign of rotation. This sign depends of the direction of y coordinate.
    //int GetRotationSignForYdir(){ return (yCoordinateUp)? 1 : -1; }


protected:

    Engine engine = Engine::NOT_DEFINED;

    Vec2i screenSize;
    float mAppScale = 1.0f;
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

    std::vector<std::string>mErrorMessages;

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
        if(started==false){
            initTimePoint = std::chrono::high_resolution_clock::now();
            started = true;
        }
        passedMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - initTimePoint).count();
        return passedMicroseconds/1000.0;
    }


    ///\brief Add suspended time in milliseconds.
    ///
    /// Suspended time is time when application is suspended. This function is called from SceneManager::Update and should not be called manually.
    void AddSuspendedTimeMS(int _suspendedTimeMS)
    {
        suspendedTimeMS += _suspendedTimeMS;
    }



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Returns passed net time in milliseconds.
    ///
    /// This function returns the net passed time which is the full passed time with deducted suspended time.
    ///\see UpdatePassedTimeMS()
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int GetPassedNetTimeMS();




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




private:
    std::chrono::high_resolution_clock::time_point initTimePoint;
    unsigned long long passedMicroseconds = 0;

    int suspendedTimeMS = 0;
    bool started = false;
    float logicTimeStepMS;

};

extern Time time;




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

*/

/// @}*    //end of group Globals



}

#endif


