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

class SignalQuery;
class SignalSetter;
class ParsedSignalPath;
struct SignalParsingInfo ;




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

    void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true);
    void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage=true);

    bool obtainSignal(SignalParsingInfo &_spi, bool _setErrorMessage=true);


private:
    std::string mName;
    AnimationInstance *mAnimationInstance = nullptr;    // OWNED
    AnimationPlayer mAnimationPlayer;

    BoolSignal mSigEnabled;
    bool mPaused = false;

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

/*
struct AnimationObjectSignalStrings
{

    std::string animationObject;
    std::string signalName;
    std::string signalValue;
    std::string signalValueState;

    bool parse(const std::string &_path);
    bool getBoolValue(bool &_state, bool _setErrorMessage=true) const;

};

*/


}






#endif
