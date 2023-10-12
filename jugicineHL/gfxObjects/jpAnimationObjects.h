#ifndef PLAYER__GFXOBJECTS__ANIMATION_OBJECTS__H
#define PLAYER__GFXOBJECTS__ANIMATION_OBJECTS__H


#include <memory>
#include "jmAnimationCommon.h"
#include "jmSignal.h"

#include "jpSettings.h"
#include "jpVariables.h"



namespace pugi{
class xml_node;
}

namespace jugimap{


class SourceSprite;
class Sprite;
class AnimationInstance;
class AnimationPlayer;
class PlayedScene;
class Animation;
struct SignalQuery;
struct SignalSetter;




class AnimationObject
{
public:


    AnimationObject(const std::string &_name, Sprite *_sprite) : mName(_name), mSprite(_sprite){}
    AnimationObject(const std::string &_name, const std::vector<Sprite*>_sprites);

    bool initConnections(PlayedScene *_scene);

    const std::string & name() const { return mName; }


    Sprite * sprite(){ return mSprite; }
    std::vector<Sprite*> & sprites(){ return mSprites; }

    void preUpdate();
    void update();
    void postUpdate();

    AnimationPlayer & animationPlayer(){ return mAnimationPlayer; }

    void obtainSignal_signalQuery(SignalQuery &_signalQuery, const std::string &_path, bool _setErrorMessage=true);
    void obtainSignal_signalSetter(SignalSetter &_signalSetter, const std::string &_path, bool _setErrorMessage=true);



private:
    std::string mName;
    AnimationInstance *mAnimationInstance = nullptr;    // OWNED
    AnimationPlayer mAnimationPlayer;

    BoolSignal mSigDisabled;

    //
    Sprite *mSprite = nullptr;
    std::vector<Sprite*>mSprites;

};





class AnimationManager
{
public:


    ~AnimationManager();
    bool create(PlayedScene *_scene);
    bool initConnections(PlayedScene *_scene);

    void preUpdate();
    void update();
    void postUpdate();

    AnimationObject * getAnimationObject(const std::string &_name, bool _setErrorMessage = true);
    std::vector<AnimationObject*> & animationObjects(){ return  mAnimationObjects; }

private:

    std::vector<AnimationObject*>mAnimationObjects;


};







}






#endif
