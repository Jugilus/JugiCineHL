#include <string>
#include <sstream>
//#include "jmObjectFactory.h"
//#include "jmSourceSprite.h"
#include "jmFont.h"
#include "jmColor.h"
//#include "jmInput.h"
#include "jmGlobal.h"

#ifdef JUGI_EDITOR
#include "../jugi/globals.h"
#endif



namespace jugimap {




std::string GetYorientationString(Yorientation _value)
{

    if(_value==Yorientation::DOWN){
        return "Down";
    }else if(_value==Yorientation::UP){
        return "Up";
    }

    return "Up";
}


Yorientation GetYorientationFromString(const std::string &_value)
{

    if(_value=="Down"){
        return Yorientation::DOWN;

    }else if(_value=="Up"){
        return Yorientation::UP;
    }

    return Yorientation::UP;

}


Yorientation GetYorientationFromInt(int _value)
{

    if(_value==static_cast<int>(Yorientation::DOWN)){
        return Yorientation::DOWN;

    }else if(_value==static_cast<int>(Yorientation::UP)){
        return Yorientation::UP;
    }

    return Yorientation::UP;

}

//==================================================================================



FileKind GetFileKindFromInt(int _kind)
{

    if(_kind == static_cast<int>(FileKind::SINGLE_IMAGE)){
        return FileKind::SINGLE_IMAGE;

    }else if(_kind == static_cast<int>(FileKind::TILE_SHEET_IMAGE)){
        return FileKind::TILE_SHEET_IMAGE;

    }else if(_kind == static_cast<int>(FileKind::SPRITE_SHEET_IMAGE)){
        return FileKind::SPRITE_SHEET_IMAGE;

    }else if(_kind == static_cast<int>(FileKind::SPRITER_FILE)){
        return FileKind::SPRITER_FILE;

    }else if(_kind == static_cast<int>(FileKind::SPINE_FILE)){
        return FileKind::SPINE_FILE;

    }else if(_kind == static_cast<int>(FileKind::SPRITE_TEXT_CONTAINER)){
        return FileKind::SPRITE_TEXT_CONTAINER;

    }

    return FileKind::NOT_DEFINED;

}

//==================================================================================


SpriteType GetSpriteTypeFromString(const std::string &_type)
{

    if(_type=="Standard"){
        return SpriteType::STANDARD;

    }else if(_type=="Composite"){
        return SpriteType::COMPOSITE;

    }else if(_type=="Text"){
        return SpriteType::TEXT;

    }else if(_type=="Spine"){
        return SpriteType::SPINE;

    }else if(_type=="Spriter"){
        return SpriteType::SPRITER;

    }

    return SpriteType::NOT_DEFINED;
}


std::string GetSpriteTypeString(SpriteType _type)
{

    if(_type==SpriteType::STANDARD){
        return "Standard";

    }else if(_type==SpriteType::COMPOSITE){
        return "Composite";

    }else if(_type==SpriteType::TEXT){
        return "Text";

    }else if(_type==SpriteType::SPINE){
        return "Spine";

    }else if(_type==SpriteType::SPRITER){
        return "Spriter";

    }

    return "Standard";

}


SpriteType GetSpriteTypeFromFileKind(FileKind _fileKind)
{

    if(_fileKind == FileKind::SINGLE_IMAGE){
        return SpriteType::STANDARD;

    }else if(_fileKind == FileKind::TILE_SHEET_IMAGE){
        return SpriteType::STANDARD;

    }else if(_fileKind == FileKind::SPRITE_SHEET_IMAGE){
        return SpriteType::STANDARD;

    }else if(_fileKind == FileKind::SPRITER_FILE){
        return SpriteType::SPRITER;

    }else if(_fileKind == FileKind::SPINE_FILE){
        return SpriteType::SPINE;

    }else if(_fileKind == FileKind::SPRITE_TEXT_CONTAINER){
        return SpriteType::TEXT;

    }

    assert(false);

    return SpriteType::NOT_DEFINED;

}

//==================================================================================


MapType GetMapTypeFromString(const std::string &_type)
{

    if(_type=="World"){
        return MapType::WORLD;

    }else if(_type=="Screen"){
        return MapType::SCREEN;

    }else if(_type=="Parallax"){
        return MapType::PARALLAX;

    }

    return MapType::NOT_DEFINED;
}



std::string GetMapTypeString(MapType _type)
{


    if(_type==MapType::WORLD){
        return "World";

    }else if(_type==MapType::SCREEN){
        return "Screen";

    }else if(_type==MapType::PARALLAX){
        return "Parallax";

    }

    return "World";

}





//==================================================================================


SpriteLayerContent GetSpriteLayerContentFromInt(int _content)
{
    if(_content==static_cast<int>(SpriteLayerContent::GRID_SPRITES)){
        return SpriteLayerContent::GRID_SPRITES;

    }else if(_content==static_cast<int>(SpriteLayerContent::FREE_SPRITES)){
        return SpriteLayerContent::FREE_SPRITES;

    }
    return SpriteLayerContent::GRID_SPRITES;
}


SpriteLayerContent GetSpriteLayerContentFromString(const std::string &_content)
{

    if(_content=="Grid sprites"){
        return SpriteLayerContent::GRID_SPRITES;

    }else if(_content=="Free sprites"){
        return SpriteLayerContent::FREE_SPRITES;
    }

    return SpriteLayerContent::GRID_SPRITES;
}


std::string GetSpriteLayerContentString(SpriteLayerContent _content)
{

    if(_content==SpriteLayerContent::GRID_SPRITES){
        return "Grid sprites";

    }else if(_content==SpriteLayerContent::FREE_SPRITES){
        return "Free sprites";
    }

    return "Grid sprites";

}


//==================================================================================


Blend GetBlendFromString(const std::string &_blend)
{

    if(_blend=="Solid"){
        return Blend::SOLID;

    }else if(_blend=="Alpha"){
        return Blend::ALPHA;

    }else if(_blend=="Additive"){
        return Blend::ADDITIVE;

    }else if(_blend=="Multiply"){
        return Blend::MULTIPLY;
    }

    return Blend::ALPHA;
}



Blend GetBlendFromInt(int _blend)
{

    if(_blend==static_cast<int>(Blend::SOLID)){
        return Blend::SOLID;

    }else if(_blend==static_cast<int>(Blend::ALPHA)){
        return Blend::ALPHA;

    }else if(_blend==static_cast<int>(Blend::ADDITIVE)){
        return Blend::ADDITIVE;

    }else if(_blend==static_cast<int>(Blend::MULTIPLY)){
        return Blend::MULTIPLY;
    }

    return Blend::ALPHA;

}


std::string GetBlendString(Blend _blend)
{

    if(_blend==Blend::SOLID){
        return "Solid";

    }else if(_blend==Blend::ALPHA){
        return "Alpha";

    }else if(_blend==Blend::ADDITIVE){
        return "Additive";

    }else if(_blend==Blend::MULTIPLY){
        return "Multiply";

    }

    return "Alpha";

}


//==================================================================================


AbsoluteOrRelativeKind GetPositionKindFromString(const std::string &_positionKind)
{

    if(_positionKind=="absolute"){
        return AbsoluteOrRelativeKind::ABS;

    }else if(_positionKind=="relative"){
        return AbsoluteOrRelativeKind::REL;

    }

    return AbsoluteOrRelativeKind::ABS;

}


AbsoluteOrRelativeKind GetPositionKindFromInt(int _positionKind)
{

    if(_positionKind == static_cast<int>(AbsoluteOrRelativeKind::ABS)){
        return AbsoluteOrRelativeKind::ABS;

    }else if(_positionKind == static_cast<int>(AbsoluteOrRelativeKind::REL)){
        return AbsoluteOrRelativeKind::REL;

    }

    return AbsoluteOrRelativeKind::ABS;

}


std::string GetPositionKindString(AbsoluteOrRelativeKind _positionKind)
{

    if(_positionKind == AbsoluteOrRelativeKind::ABS){
        return "absolute";

    }else if(_positionKind == AbsoluteOrRelativeKind::REL){
        return "relative";

    }

    return "absolute";

}



//==================================================================================




ColorOverlayBlend GetColorOverlayBlendFromString(const std::string &_blend)
{

    if(_blend=="Normal"){
        return ColorOverlayBlend::NORMAL;

    }else if(_blend=="Multiply"){
        return ColorOverlayBlend::MULTIPLY;

    }else if(_blend=="Linear dodge"){
        return ColorOverlayBlend::LINEAR_DODGE;

    }else if(_blend=="Color"){
        return ColorOverlayBlend::COLOR;

    }else if(_blend=="Simple multiply"){
        return ColorOverlayBlend::SIMPLE_MULTIPLY;

    }

    return ColorOverlayBlend::SIMPLE_MULTIPLY;
}


std::string GetColorOverlayBlendString(ColorOverlayBlend _blend)
{

    if(_blend==ColorOverlayBlend::NORMAL){
        return "Normal";

    }else if(_blend==ColorOverlayBlend::MULTIPLY){
        return "Multiply";

    }else if(_blend==ColorOverlayBlend::LINEAR_DODGE){
        return "Linear dodge";

    }else if(_blend==ColorOverlayBlend::COLOR){
        return "Color";

    }else if(_blend==ColorOverlayBlend::SIMPLE_MULTIPLY){
        return "Simple multiply";

    }

    return "Simple multiply";

}


ColorOverlayBlend GetColorOverlayBlendFromInt(int _blend)
{


    if(_blend==static_cast<int>(ColorOverlayBlend::NORMAL)){
        return ColorOverlayBlend::NORMAL;

    }else if(_blend==static_cast<int>(ColorOverlayBlend::MULTIPLY)){
        return ColorOverlayBlend::MULTIPLY;

    }else if(_blend==static_cast<int>(ColorOverlayBlend::LINEAR_DODGE)){
        return ColorOverlayBlend::LINEAR_DODGE;

    }else if(_blend==static_cast<int>(ColorOverlayBlend::COLOR)){
        return ColorOverlayBlend::COLOR;

    }else if(_blend==static_cast<int>(ColorOverlayBlend::SIMPLE_MULTIPLY)){
        return ColorOverlayBlend::SIMPLE_MULTIPLY;

    }

    return ColorOverlayBlend::SIMPLE_MULTIPLY;

}


//==================================================================================



WidgetType GetGuiWidgetKindFromString(const std::string &_kind)
{

    if(_kind=="button"){
        return WidgetType::BUTTON;

    }else if(_kind=="bar"){
        return WidgetType::BAR;

    }else if(_kind=="slider"){
        return WidgetType::SLIDER;

    }else if(_kind=="textField"){
        return WidgetType::TEXT_FIELD;

    }else if(_kind=="textInput"){
        return WidgetType::TEXT_INPUT;

    }else if(_kind=="table"){
        return WidgetType::TABLE;

    }

    return WidgetType::NOT_DEFINED;
}


std::string GetGuiWidgetKindString(WidgetType _kind)
{

    if(_kind==WidgetType::BUTTON){
        return "button";

    }else if(_kind==WidgetType::BAR){
        return "bar";

    }else if(_kind==WidgetType::SLIDER){
        return "slider";

    }else if(_kind==WidgetType::TEXT_FIELD){
        return "textField";

    }else if(_kind==WidgetType::TEXT_INPUT){
        return "textInput";

    }else if(_kind==WidgetType::TABLE){
        return "table";

    }

    return "unknownWidget";

}


//==================================================================================



std::string GetTextHandleVariantString(TextHandleVariant _handle)
{

    if(_handle==TextHandleVariant::CENTER){
        return "center";

    }else if(_handle==TextHandleVariant::LEFT_TOP){
        return "left-top";

    }else if(_handle==TextHandleVariant::TOP){
        return "top";

    }else if(_handle==TextHandleVariant::RIGHT_TOP){
        return "right-top";

    }else if(_handle==TextHandleVariant::RIGHT){
        return "right";

    }else if(_handle==TextHandleVariant::RIGHT_BOTTOM){
        return "right-bottom";

    }else if(_handle==TextHandleVariant::BOTTOM){
        return "bottom";

    }else if(_handle==TextHandleVariant::LEFT_BOTTOM){
        return "left-bottom";

    }else if(_handle==TextHandleVariant::LEFT){
        return "left";

    }

    return "center";
}


TextHandleVariant GetTextHandleVariantFromString(const std::string &_handle)
{

    if(_handle=="center"){
        return TextHandleVariant::CENTER;

    }else if(_handle=="left-top"){
        return TextHandleVariant::LEFT_TOP;

    }else if(_handle=="top"){
        return TextHandleVariant::TOP;

    }else if(_handle=="right-top"){
        return TextHandleVariant::RIGHT_TOP;

    }else if(_handle=="right"){
        return TextHandleVariant::RIGHT;

    }else if(_handle=="right-bottom"){
        return TextHandleVariant::RIGHT_BOTTOM;

    }else if(_handle=="bottom"){
        return TextHandleVariant::BOTTOM;

    }else if(_handle=="left-bottom"){
        return TextHandleVariant::LEFT_BOTTOM;

    }else if(_handle=="left"){
        return TextHandleVariant::LEFT;

    }

    return TextHandleVariant::CENTER;
}


TextHandleVariant GetTextHandleVariantFromInt(int _id)
{

    if(_id==(int)TextHandleVariant::CENTER){
        return TextHandleVariant::CENTER;

    }else if(_id==(int)TextHandleVariant::LEFT_TOP){
        return TextHandleVariant::LEFT_TOP;

    }else if(_id==(int)TextHandleVariant::TOP){
        return TextHandleVariant::TOP;

    }else if(_id==(int)TextHandleVariant::RIGHT_TOP){
        return TextHandleVariant::RIGHT_TOP;

    }else if(_id==(int)TextHandleVariant::RIGHT){
        return TextHandleVariant::RIGHT;

    }else if(_id==(int)TextHandleVariant::RIGHT_BOTTOM){
        return TextHandleVariant::RIGHT_BOTTOM;

    }else if(_id==(int)TextHandleVariant::BOTTOM){
        return TextHandleVariant::BOTTOM;

    }else if(_id==(int)TextHandleVariant::LEFT_BOTTOM){
        return TextHandleVariant::LEFT_BOTTOM;

    }else if(_id==(int)TextHandleVariant::LEFT){
        return TextHandleVariant::LEFT;
    }

    return TextHandleVariant::CENTER;

}


Vec2f GetRelativeHandleFromTextHandleVariant(TextHandleVariant _thv, Yorientation _yOrientation)
{
    switch (_thv)
    {
    default:
    case TextHandleVariant::CENTER:
        return Vec2f(0.5f, 0.5f);

    case TextHandleVariant::LEFT_TOP:
        return (_yOrientation==Yorientation::UP)? Vec2f(0.0f, 1.0f) : Vec2f(0.0f, 0.0f);

    case TextHandleVariant::TOP:
        return (_yOrientation==Yorientation::UP)? Vec2f(0.5f, 1.0f) : Vec2f(0.5f, 0.0f);

    case TextHandleVariant::RIGHT_TOP:
        return (_yOrientation==Yorientation::UP)? Vec2f(1.0f, 1.0f) : Vec2f(1.0f, 0.0f);

    case TextHandleVariant::RIGHT:
        return Vec2f(1.0f, 0.5f);

    case TextHandleVariant::RIGHT_BOTTOM:
        return (_yOrientation==Yorientation::UP)? Vec2f(1.0f, 0.0f) : Vec2f(1.0f, 1.0f);

    case TextHandleVariant::BOTTOM:
        return (_yOrientation==Yorientation::UP)? Vec2f(0.5f, 0.0f) : Vec2f(0.5f, 1.0f);

    case TextHandleVariant::LEFT_BOTTOM:
        return (_yOrientation==Yorientation::UP)? Vec2f(0.0f, 0.0f) : Vec2f(0.0f, 1.0f);

    case TextHandleVariant::LEFT:
        return Vec2f(0.0f, 0.5f);

    }

}


void GetHandleFromTextHandleVariant(TextHandleVariant _thv, int width, int height, int &xHandle, int &yHandle, Yorientation _yOrientation)
{

    Vec2f relHandle = GetRelativeHandleFromTextHandleVariant(_thv, _yOrientation);
    xHandle = relHandle.x * width;
    yHandle = relHandle.y * height;

}



//==================================================================================


std::string GetShapeKindStringForGUI(ShapeKind _kind)
{

    if(_kind==ShapeKind::RECTANGLE){
        return "rectangle";

    }else if(_kind==ShapeKind::POLYLINE){
        return "polyline";

    }else if(_kind==ShapeKind::BEZIER_POLYCURVE){
        return "bezier polycurve";

    }else if(_kind==ShapeKind::ELLIPSE){
        return "ellipse";

    }else if(_kind==ShapeKind::SINGLE_POINT){
        return "single point";

    }

    return "not defined";

}


std::string GetShapeKindStringForFile(ShapeKind _kind)
{

    if(_kind==ShapeKind::RECTANGLE){
        return "rectangle";

    }else if(_kind==ShapeKind::POLYLINE){
        return "polyline";

    }else if(_kind==ShapeKind::BEZIER_POLYCURVE){
        return "bezierPolycurve";

    }else if(_kind==ShapeKind::ELLIPSE){
        return "ellipse";

    }else if(_kind==ShapeKind::SINGLE_POINT){
        return "singlePoint";

    }

    return "notDefined";

}


ShapeKind GetShapeKindFromInt(int _kind)
{

    if(_kind == static_cast<int>(ShapeKind::RECTANGLE)){
        return ShapeKind::RECTANGLE;

    }else if(_kind == static_cast<int>(ShapeKind::POLYLINE)){
        return ShapeKind::POLYLINE;

    }else if(_kind == static_cast<int>(ShapeKind::BEZIER_POLYCURVE)){
        return ShapeKind::BEZIER_POLYCURVE;

    }else if(_kind == static_cast<int>(ShapeKind::ELLIPSE)){
        return ShapeKind::ELLIPSE;

    }else if(_kind == static_cast<int>(ShapeKind::SINGLE_POINT)){
        return ShapeKind::SINGLE_POINT;

    }

    return ShapeKind::NOT_DEFINED;

}


ShapeKind GetShapeKindFromFileString(const std::string &_kind)
{

    if(_kind == "rectangle"){
        return ShapeKind::RECTANGLE;

    }else if(_kind == "polyline"){
        return ShapeKind::POLYLINE;

    }else if(_kind == "bezierPolycurve"){
        return ShapeKind::BEZIER_POLYCURVE;

    }else if(_kind == "ellipse"){
        return ShapeKind::ELLIPSE;

    }else if(_kind == "singlePoint"){
        return ShapeKind::SINGLE_POINT;

    }

    return ShapeKind::NOT_DEFINED;
}


//==================================================================================


std::string GetEasingKindString(EasingKind _easingKind)
{

    if(_easingKind==EasingKind::LINEAR){
        return "Linear";

    }else if(_easingKind==EasingKind::EASE_START_QUAD){
        return "Ease start quad";

    }else if(_easingKind==EasingKind::EASE_END_QUAD){
        return "Ease end quad";

    }else if(_easingKind==EasingKind::EASE_START_END_QUAD){
        return "Ease start and end quad";

    }else if(_easingKind==EasingKind::EASE_START_CUBIC){
        return "Ease start cubic";

    }else if(_easingKind==EasingKind::EASE_END_CUBIC){
        return "Ease end cubic";

    }else if(_easingKind==EasingKind::EASE_START_END_CUBIC){
        return "Ease start and end cubic";

    }else if(_easingKind==EasingKind::EASE_START_SINE){
        return "Ease start sine";

    }else if(_easingKind==EasingKind::EASE_END_SINE){
        return "Ease end sine";

    }else if(_easingKind==EasingKind::EASE_START_END_SINE){
        return "Ease start and end sine";

    }else if(_easingKind==EasingKind::CONSTANT){
        return "Constant";

    }

    return "";
}


EasingKind GetEasingKindFromString(const std::string &_kind)
{

    if(_kind=="Linear"){
        return EasingKind::LINEAR;

    }else if(_kind=="Ease start" || _kind=="Ease start quad" ){
        return EasingKind::EASE_START_QUAD;

    }else if(_kind=="Ease end" || _kind=="Ease end quad"){
        return EasingKind::EASE_END_QUAD;

    }else if(_kind=="Ease start and end" || _kind=="Ease start and end quad"){
        return EasingKind::EASE_START_END_QUAD;

    }else if(_kind=="Ease start cubic"){
        return EasingKind::EASE_START_CUBIC;

    }else if(_kind=="Ease end cubic"){
        return EasingKind::EASE_END_CUBIC;

    }else if(_kind=="Ease start and end cubic"){
        return EasingKind::EASE_START_END_CUBIC;

    }else if(_kind=="Ease start sine"){
        return EasingKind::EASE_START_SINE;

    }else if(_kind=="Ease end sine"){
        return EasingKind::EASE_END_SINE;

    }else if(_kind=="Ease start and end sine"){
        return EasingKind::EASE_START_END_SINE;

    }else if(_kind=="Constant"){
        return EasingKind::CONSTANT;

    }

    return EasingKind::LINEAR;

}


EasingKind GetEasingKindFromInt(int _kind)
{

    if(_kind==0){
        return EasingKind::LINEAR;

    }else if(_kind==1){
        return EasingKind::EASE_START_QUAD;

    }else if(_kind==2){
        return EasingKind::EASE_END_QUAD;

    }else if(_kind==3){
        return EasingKind::EASE_START_END_QUAD;

    }else if(_kind==4){
        return EasingKind::EASE_START_CUBIC;

    }else if(_kind==5){
        return EasingKind::EASE_END_CUBIC;

    }else if(_kind==6){
        return EasingKind::EASE_START_END_CUBIC;

    }else if(_kind==7){
        return EasingKind::EASE_START_SINE;

    }else if(_kind==8){
        return EasingKind::EASE_END_SINE;

    }else if(_kind==9){
        return EasingKind::EASE_START_END_SINE;

    }else if(_kind==10){
        return EasingKind::CONSTANT;

    }

    return EasingKind::LINEAR;

}


//==================================================================================


AnimationTrackKind GetAnimationTrackKindFromInt(int _value)
{

    if(_value == 0){
        return AnimationTrackKind::TRANSLATION;

    }else if(_value == 1){
        return AnimationTrackKind::SCALING;

    }else if(_value == 2){
        return AnimationTrackKind::ROTATION;

    }else if(_value == 3){
        return AnimationTrackKind::ALPHA_CHANGE;

    }else if(_value == 4){
        return AnimationTrackKind::OVERLAY_COLOR_CHANGE;

    }else if(_value == 5){
        return AnimationTrackKind::PATH_MOVEMENT;

    }else if(_value == 6){
        return AnimationTrackKind::FRAME_CHANGE;

    }else if(_value == 7){
        return AnimationTrackKind::FRAME_ANIMATION;

    }else if(_value == 8){
        return AnimationTrackKind::TIMELINE_ANIMATION;

    }else if(_value == 9){
        return AnimationTrackKind::FLIP_HIDE;

    }else if(_value == 20){
        return AnimationTrackKind::META;

    }else{
        return AnimationTrackKind::NOT_DEFINED;

    }

}


std::string GetAnimationTrackKindString(AnimationTrackKind _kind)
{

    if(_kind==AnimationTrackKind::TRANSLATION){
        return "Translation";

    }else if (_kind==AnimationTrackKind::SCALING){
        return "Scaling";

    }else if (_kind==AnimationTrackKind::ROTATION){
        return "Rotation";

    }else if (_kind==AnimationTrackKind::ALPHA_CHANGE){
        return "Alpha";

    }else if (_kind==AnimationTrackKind::OVERLAY_COLOR_CHANGE){
        return "Overlay color";

    }else if (_kind==AnimationTrackKind::PATH_MOVEMENT){
        return "Path movement";

    }else if (_kind==AnimationTrackKind::FRAME_CHANGE){
        return "Frame change";

    }else if (_kind==AnimationTrackKind::FRAME_ANIMATION){
        return "Frame animation";

    }else if (_kind==AnimationTrackKind::TIMELINE_ANIMATION){
        return "Timeline animation";

    }else if (_kind==AnimationTrackKind::FLIP_HIDE){
        return "Flip hide";

    }else if (_kind==AnimationTrackKind::META){
        return "Meta";
    }

    return "Not defined";

}


AnimationTrackKind GetAnimationTrackKindFromString(const std::string &_kind)
{

    if(_kind=="Translation"){
        return AnimationTrackKind::TRANSLATION;

    }else if (_kind=="Scaling"){
        return AnimationTrackKind::SCALING;

    }else if (_kind=="Rotation"){
        return AnimationTrackKind::ROTATION;

    }else if (_kind=="Alpha"){
        return AnimationTrackKind::ALPHA_CHANGE;

    }else if (_kind=="Overlay color"){
        return AnimationTrackKind::OVERLAY_COLOR_CHANGE;

    }else if (_kind=="Path movement"){
        return AnimationTrackKind::PATH_MOVEMENT;

    }else if (_kind=="Frame change"){
        return AnimationTrackKind::FRAME_CHANGE;

    }else if (_kind=="Frame animation"){
        return AnimationTrackKind::FRAME_ANIMATION;

    }else if (_kind=="Timeline animation"){
        return AnimationTrackKind::TIMELINE_ANIMATION;

    }else if (_kind=="Flip hide"){
        return AnimationTrackKind::FLIP_HIDE;

    }else if (_kind=="Meta"){
        return AnimationTrackKind::META;

    }else{

        return AnimationTrackKind::NOT_DEFINED;
    }

}


int GetOrderOfAnimationTrackKind(AnimationTrackKind _kind)
{

    if (_kind==AnimationTrackKind::META){
        return 1;

    }else if(_kind==AnimationTrackKind::TRANSLATION){
        return 2;

    }else if(_kind==AnimationTrackKind::PATH_MOVEMENT){
        return 3;

    }else if (_kind==AnimationTrackKind::SCALING){
        return 4;

    }else if (_kind==AnimationTrackKind::FLIP_HIDE){
        return 5;

    }else if (_kind==AnimationTrackKind::ROTATION){
        return 6;

    }else if (_kind==AnimationTrackKind::ALPHA_CHANGE){
        return 7;

    }else if (_kind==AnimationTrackKind::OVERLAY_COLOR_CHANGE){
        return 8;

    }else if (_kind==AnimationTrackKind::FRAME_CHANGE){
        return 9;

    }else if (_kind==AnimationTrackKind::FRAME_ANIMATION){
        return 10;

    }else if (_kind==AnimationTrackKind::TIMELINE_ANIMATION){
        return 11;

    }

    return -1;
}


std::string GetPlayingAnimationStateString(AnimationPlayerState _value)
{

    if(_value==AnimationPlayerState::IDLE){
        return "idle";

    }else if(_value==AnimationPlayerState::PLAYING){
        return "playing";

    }else if(_value==AnimationPlayerState::STALLED){
        return "stalled";

    }else if(_value==AnimationPlayerState::WAITING_TO_START){
        return "waiting to start";

    }else if(_value==AnimationPlayerState::WAITING_TO_REPEAT){
        return "waiting to repeat";

    }

    return "none";
}


//==================================================================================




/*

TEProjectInfo CurrentProject;
//TEProjectInfo PreviousProject;
//TEProjectInfo StoredProject;
//TEProjectInfo LoadedProject;
//TEProjectInfo ReplacementProject;
//TEProjectInfo CurrentProjectStored;

//TEProjectInfo *CurrentProjectNEW =  nullptr;
//TEProjectInfo _currentProjectNew;



int Time::GetPassedNetTimeMS()
{
#ifdef JUGI_EDITOR
    return jugi::glob::TimerGlobal.elapsed();
#else
    return passedMicroseconds/1000.0 - suspendedTimeMS;
#endif

}



Settings settings;
Time time;




jugimap::BoundingBoxFloat Y_Up_to_Y_Down::convertRect(jugimap::BoundingBoxFloat r)
{
    return BoundingBoxFloat({r.min.x, settings.GetScreenSize().y -r.max.y},
                 {r.max.x, settings.GetScreenSize().y-r.min.y});
}


jugimap::BoundingBoxInt Y_Up_to_Y_Down::convertRect(jugimap::BoundingBoxInt r)
{
    return BoundingBoxInt({r.min.x, settings.GetScreenSize().y -r.max.y},
                 {r.max.x, settings.GetScreenSize().y-r.min.y});
}


jugimap::BoundingBoxFloat Y_Up_to_Y_Down::convertRect(ncine::Rectf r)
{
    return BoundingBoxFloat({r.x, settings.GetScreenSize().y - (r.y + r.h)},
                 {r.x+r.w, settings.GetScreenSize().y -r.y});
}


jugimap::BoundingBoxInt Y_Up_to_Y_Down::convertRect(ncine::Recti r)
{
    return BoundingBoxInt({r.x, settings.GetScreenSize().y - (r.y + r.h)},
                 {r.x+r.w, settings.GetScreenSize().y -r.y});
}

*/

/*
void DeleteGlobalObjects()
{

    //SourceGraphics::deleteGlobalData();
    //delete dummyTextSourceSprite;
    //if(objectFactory) delete objectFactory;
    fontsLibrary.deleteContent();
    textColorsLibrary.deleteContent();



}
*/



}
