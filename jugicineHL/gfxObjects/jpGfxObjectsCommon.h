#ifndef PLAYER__GFXOBJECTS__COMMON__H
#define PLAYER__GFXOBJECTS__COMMON__H


#include <memory>

#include "jmAnimationCommon.h"
#include "jmSignal.h"
#include "jpSettings.h"
#include "jpVariables.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class Sprite;
class TextSprite;
class SpriteLayer;
class PlayedScene;
class VectorShape;





enum class GfxObjectType
{
    UNKNOWN,
    ANIMATED_SPRITE,
    SPRITES_OBJECT,
    TEXT_SPRITE,
    RELATIVE_MOVER

};



enum class DataConnectionType
{
    UNKNOWN,
    TEXT
};


struct DataConnectionCfg
{

    DataConnectionCfg(DataConnectionType _type, const std::string &_value) : type(_type), data(_value){}

    DataConnectionType type = DataConnectionType::UNKNOWN;
    std::string data;
};



struct GfxObjectCfg
{

    GfxObjectCfg(const std::string &_name, GfxObjectType _type) : mName(_name), mType(_type) {}

    virtual bool initCfg(const pugi::xml_node &_node){ return true; }


    const std::string & name(){ return mName; }
    GfxObjectType type(){ return mType; }


    std::string mName;
    GfxObjectType mType = GfxObjectType::UNKNOWN;

    //SimpleNoNameStorage<ItemData*>data;                    // OWNED
    //std::vector<DataConnectionCfg>dataConnections;

};



class GfxObject : public SignalCallback
{
public:
    GfxObject();
    GfxObject(GfxObjectCfg *_cfg, BaseObject *_parentObject);
    GfxObject(const GfxObject &_src);


    virtual ~GfxObject();

    //virtual bool build(PlayedScene* _scene, GfxObjectCfg* _cfg, BaseObject *_parentObject);
    virtual bool initConnections(PlayedScene* _scene) = 0;


    const std::string &name(){ return mCfg->name(); }
    GfxObjectType type(){ return mCfg->type(); }
    //PlayedScene* parentPlayerScene(){ return mParentPlayerScene; }
    BaseObject* parentObject(){ return mParentObject;}

    //std::vector<std::pair<DataConnectionType,ItemData*>> &data(){ return mData; }

    //ItemData* findData(const std::string &_name, bool _setErrorMessage = true);

    void _setParentObject(BaseObject *_parentObject){ mParentObject = _parentObject; }

    virtual void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true);
    virtual void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage=true);



protected:
    GfxObjectCfg *mCfg = nullptr;                                   // LINK
    BaseObject *mParentObject = nullptr;                            // LINK

    //std::vector<std::pair<DataConnectionType,ItemData*>>mData;      // OWNED

};



//--------------------------------------------------------------------------------




struct AniSpriteObjectCfg : public GfxObjectCfg
{

    AniSpriteObjectCfg(const std::string &_name) : GfxObjectCfg(_name, GfxObjectType::ANIMATED_SPRITE){}
    bool initCfg(const pugi::xml_node &_node) override;

    std::string sourceSprite;
    std::string animation;
    std::string layer;
    std::string map;


};


class AniSpriteObject : public GfxObject
{
public:

    AniSpriteObject(GfxObjectCfg *_cfg, BaseObject *_parentObject);
    AniSpriteObject(const AniSpriteObject &_src);

    ~AniSpriteObject();

    //bool build(PlayedScene *_scene, GfxObjectCfg *_cfg, BaseObject *_parent) override;
    bool initConnections(PlayedScene *_scene) override;

    void start(Vec2f _pos);
    void update();
    void end();
    bool isAnimationStalled();
    bool isAnimationStopped();


    Sprite *sprite(){ return mSprite; }


private:
    Sprite *mSprite = nullptr;                          // OWNED or LINK if the sprite is on layer
    bool mSpriteOwned = true;
    SpriteLayer *mSpriteLayer = nullptr;                // LINK
    AnimationInstance *mAnimationInstance = nullptr;    // OWNED
    AnimationPlayer mAnimationPlayer;

};


//--------------------------------------------------------------------------------


class GSpritesObject : public GfxObject
{
public:

    GSpritesObject(const pugi::xml_node &_node);
    GSpritesObject(std::vector<void*>_objects, GSpritesObjectType _objType);

    ~GSpritesObject() override;

    virtual bool initConnections(PlayedScene* _scene);

    void setVisible(bool _visible);

    GSpritesObjectType spritesObjectType(){ return mSpritesObjectType; }
    std::vector<void*> &objects(){ return mObjects; }

    BoolSignal & visibleSignal(){ return mSigVisible; }

    bool onSignalSet(Signal *_signal) override;

    virtual void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true);
    virtual void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage=true);



protected:
    std::string mName;
    std::vector<void*>mObjects;                         // LINKS
    GSpritesObjectType mSpritesObjectType = GSpritesObjectType::NOT_DEFINED;

    BoolSignal mSigVisible;

    //cfg
    std::string mObjectName;
    std::string mSrcMapName;

    void _setVisible(bool _visible);

};


//--------------------------------------------------------------------------------


struct GTextSpriteCfg : public GfxObjectCfg
{

    GTextSpriteCfg(const std::string &_name) : GfxObjectCfg(_name, GfxObjectType::TEXT_SPRITE){}

    bool initCfg(const pugi::xml_node &_node) override;



    std::string sourceSprite;
    std::string layer;
    std::string textSprite;
    std::string map;
};



class GTextSprite : public GfxObject
{
public:

    GTextSprite(GfxObjectCfg *_cfg, BaseObject *_parentObject) : GfxObject(_cfg, _parentObject){}
    //GTextSprite(const pugi::xml_node &_node);

    //bool build(PlayedScene *_scene, GfxObjectCfg *_cfg, BaseObject *_parent) override;
    virtual bool initConnections(PlayedScene* _scene);

    TextSprite *textSprite(){ return mTextSprite; }

    //void setVisible(bool _visible);

    //void onSetBySignalSetter(Signal *_signal) override;

    StringSignal & textSignal(){ return mSigText; }
    BoolSignal & visibleSignal(){ return mSigVisible; }


    bool onSignalSet(Signal *_signal) override;


    virtual void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true);
    virtual void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage=true);



private:
    //GTextSpriteCfg *mCfg = nullptr;
    //BaseObject *mParentObject = nullptr;

    TextSprite *mTextSprite = nullptr;      // LINK
    StringSignal mSigText;
    BoolSignal mSigVisible;


    //--- cfg
    //std::string mTextSpriteName;
    //std::string mSourceMapName;

};



//--------------------------------------------------------------------------------



class PathMover
{
public:

    void start(Vec2f _startPos, std::vector<PathPoint> *_pathPoints, float _pathLength, float _speed);
    Vec2f updateVelocity(float _timeStep);
    Vec2f unitVelocity(){ return mUnitVelocity; }
    Vec2f currentPosition(){ return mCurrentPos; }
    bool isFinished();


private:
    float mCurrentDistanceP = 0.0f;
    int mCurrentIndex = -1;
    int mStartIndex = -1;
    int mEndIndex = -1;
    //VectorShape *mPath = nullptr;       // LINK
    Vec2f mCurrentPosP;
    Vec2f mUnitVelocity;
    float mPassedDistance = 0.0f;
    float mDistance = 0.0f;
    float mSpeed = 0.0f;
    Vec2f mVelocity;
    Vec2f mCurrentPos;

    std::vector<PathPoint> * mPathPoints = nullptr;      // LINK
    float mPathLength = 0.0f;

    bool obtainUnitVelocity(float passedDistanceP);

};


class FollowMover
{
public:

    //void start(Vec2f _startPos, float _startAngle, Vec2f _endPos, float _speed);
    void start(Vec2f _startPos, Vec2f _startUnitVelocity, Vec2f _endPos);
    Vec2f updateVelocity(float _timeStep, Vec2f _currentPos, Vec2f _targetPos, float _speed);

    bool isTargetReached(float threeshold = 10);

private:
    float mAngle_current = 0.0;
    float mAngle_target = 0.0;
    float mRotationSpeed = 180;
    float mRotationSpeedDef = 180;

   // float mStartAngle=0.0f;
    //Vec2f mStartUnitVelocity;
    Vec2f mUnitVelocity;

    Vec2f mCurrentPos;
    //Vec2f mStartPos;
    //Vec2f mEndPos;

    Vec2f mTargetPos;
    float mDistCurrentTarget = 0.0f;
    //float mTargetDistance = 0.0f;

    //float mSpeed = 0.0f;
    Vec2f mVelocity;

    //float fSpeed = 1.0f;
    float mDistNear = 0.0f;


    bool mFollowDynamicPos = true;
    bool mFollowingDynamicPos = false;
    bool mFinished = false;

};





enum class MoverKind
{
    UNKNOWN,
    LINEAR_MOVER,
    FOLLOWER,
    PARABOLA,
    PATH
};



struct GORelativeMoverCfg : public GfxObjectCfg
{

    GORelativeMoverCfg(const std::string &_name) : GfxObjectCfg(_name, GfxObjectType::RELATIVE_MOVER){}
    bool initCfg(const pugi::xml_node &_node) override;


    struct DirectionCfg
    {
        Direction parentDirection = Direction::NONE;
        float movingDirectionAngleDeg = 0.0f;
        std::string animation;
        std::string pathName;
        VectorShape *mPath = nullptr;               // LINK
        float distanceP = 0.0f;
        float heightP = 0.0f;

        std::vector<PathPoint>pathPoints;
        float pathLength = 0.0f;
        float pathLengthMax = 4000.0f;
    };


    MoverKind kind = MoverKind::LINEAR_MOVER;
    float speed = 0.0;                          // pixel/s
    //int distanceP = 0;
    std::vector<DirectionCfg>directions;
    std::vector<std::string>tags;

    float fieldSpeedAngleDeg = 0.0f;
    float fieldSpeed = 0.0f;


};


enum class MoverState
{
    IDLE,
    MOVING,
};


class GORelativeMover : public GfxObject
{
public:

    struct DirectionMover
    {
        GORelativeMoverCfg::DirectionCfg *mCfg = nullptr;       // LINK
        AnimationInstance *mAnimationInstance = nullptr;        // OWNED
    };


    GORelativeMover(GfxObjectCfg *_cfg, BaseObject *_parentObject, Sprite *_sprite);
    GORelativeMover(const GORelativeMover &_src);

    ~GORelativeMover();

    MoverKind kind(){ return static_cast<GORelativeMoverCfg*>(mCfg)->kind; }
    //bool build(PlayedScene *_scene, GfxObjectCfg *_cfg, BaseObject *_parent) override;
    bool initConnections(PlayedScene *_scene) override;
    //bool initConnections(PlayedScene *_scene);

    void start(Vec2f _pos, Direction _parentDirection, BaseObject *_followedObject);
    Vec2f update(float _timeStep, Vec2f _currentPos);
    void finish();
    //void end();
    //bool isAnimationStalled();
    //bool isAnimationStopped();

    AnimationInstance* currentAnimationInstance();
    //Sprite *sprite(){ return mSprite; }
    bool isActive(){ return mCurrentDirectionMover!=nullptr; }

    MoverState state(){ return mState; }
    std::vector<std::string> &tags(){ return static_cast<GORelativeMoverCfg*>(mCfg)->tags; }

    BaseObject *followedObject(){ return mFollowedObject; }

private:
    //Sprite *mSprite = nullptr;                          // OWNED or LINK if the sprite is on layer
    //bool mSpriteOwned = true;
    //SpriteLayer *mSpriteLayer = nullptr;                // LINK
    //AnimationInstance *mAnimationInstance = nullptr;    // OWNED
    MoverState mState = MoverState::IDLE;

    Sprite *mSprite = nullptr;                              // LINK
    std::vector<DirectionMover>mDirectionMovers;
    DirectionMover *mCurrentDirectionMover = nullptr;        //LINK


    MoverKind mKind = MoverKind::UNKNOWN;
    float mSpeed = 0.0f;
    //Vec2f mStartPos;
    //Vec2f mPos;
    //Vec2f mEndPos;

    //float mTargetDistance = 0.0f;
    //Vec2f mCurrentPos;
    //Vec2f mTargetPos;
    //float mAngle = 0.0f;
   // Vec2f mStartUnitVelocity;
    Vec2f mUnitVelocity;


    //int mPathSegment = 0;

    Vec2f mUnitEndPos;
    Vec2f mVelocity;

    //----
    std::unique_ptr<PathMover>mPathMover;
    std::unique_ptr<FollowMover>mFollowMover;

    BaseObject *mFollowedObject = nullptr;

};


//--------------------------------------------------------------------------------


class GfxObjectsGroup
{
public:

    bool initConnections(PlayedScene* _scene);
    bool build(PlayedScene *_scene);

    GfxObject* getGfxObject(const std::string &_name);
    std::vector<GfxObject*> &gfxObjects(){ return mGfxObjects; }


private:
    std::vector<GfxObject*>mGfxObjects;


};






}






#endif
