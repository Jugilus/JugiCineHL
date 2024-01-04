#include <assert.h>
#include "jmCommonFunctions.h"
#include "jmGlobal.h"
#include "jmSprite.h"
#include "jmSourceSprite.h"
#include "jmFrameAnimation.h"



namespace jugimap{

/*

FrameAnimation::FrameAnimation(const FrameAnimation &fa)
{

    name = fa.name;
    sourceSprite = fa.sourceSprite;
    loopCount = fa.loopCount;
    loopForever = fa.loopForever;
    scaleDuration = fa.scaleDuration;
    parameters = fa.parameters;
    dataFlags = fa.dataFlags;

    for(AnimationFrame *af : fa.frames){
        frames.push_back(new AnimationFrame(*af));
    }
}


FrameAnimation& FrameAnimation::operator=(const FrameAnimation &fa)
{

    if(this==&fa) return *this;

    name = fa.name;
    sourceSprite = fa.sourceSprite;
    loopCount = fa.loopCount;
    loopForever = fa.loopForever;
    scaleDuration = fa.scaleDuration;
    parameters = fa.parameters;
    dataFlags = fa.dataFlags;

    for(AnimationFrame *af : frames){
        delete af;
    }
    frames.clear();

    for(AnimationFrame *af : fa.frames){
        frames.push_back(new AnimationFrame(*af));
    }
    return *this;
}


FrameAnimation::~FrameAnimation()
{
    for(AnimationFrame *af : frames){
        delete af;
    }
}

*/



//--------------------------------------------------------------------------------------------------------


FrameAnimation::FrameAnimation(SourceSprite* _sourceObject) : Animation(_sourceObject)
{
    kind = AnimationKind::FRAME_ANIMATION;

}



FrameAnimation::FrameAnimation(SourceSprite* _sourceObject, const std::string &_nameID) : Animation(_nameID, _sourceObject)
{
    kind = AnimationKind::FRAME_ANIMATION;

}



FrameAnimation::FrameAnimation(const FrameAnimation &fa) : Animation(fa)
{

    scaleDuration = fa.scaleDuration;

    //----
    for(AnimationFrame *af : fa.frames){
        frames.push_back(new AnimationFrame(*af));
        if(af == fa.editedFrame){
            editedFrame = frames.back();
        }
    }
}


FrameAnimation& FrameAnimation::operator=(const FrameAnimation &fa)
{

    if(this==&fa) return *this;

    Animation::operator=(fa);

    //----
    scaleDuration = fa.scaleDuration;

    for(AnimationFrame *af : frames){
        delete af;
    }
    frames.clear();

    for(AnimationFrame *af : fa.frames){
        frames.push_back(new AnimationFrame(*af));
        if(af == fa.editedFrame){
            editedFrame = frames.back();
        }
    }

    return *this;
}


FrameAnimation::~FrameAnimation()
{
    for(AnimationFrame *af : frames){
        delete af;
    }
}


/*
void FrameAnimation::SetFramesMStime()
{

    int t = 0;
    for(AnimationFrame* af : frames){
        af->msTimePoint =  t;
        t += af->duration;
    }
}
*/


int FrameAnimation::ObtainDuration()
{

    duration = 0;
    for(AnimationFrame *af : frames){
        af->msTimePoint =  duration;
        duration += af->duration;
    }

    return duration;
}



/*
EFrameAnimation::EFrameAnimation(const EFrameAnimation &fa) : EAnimation(fa)
{

    for(TAnimationFrame *af : fa.frames){
        frames.push_back(new TAnimationFrame(*af));
    }
}
*/

/*
EFrameAnimation& EFrameAnimation::operator=(const EFrameAnimation &fa)
{

    if(this==&fa) return *this;

    EAnimation::operator=(fa);

    //----
    for(TAnimationFrame *af : frames){
        delete af;
    }
    frames.clear();

    for(TAnimationFrame *af : fa.frames){
        frames.push_back(new TAnimationFrame(*af));
    }

    return *this;
}
*/




//==============================================================================


FrameAnimationInstance::FrameAnimationInstance(FrameAnimation *_animation, Sprite *_sprite)
{

    kind = AnimationKind::FRAME_ANIMATION;
    animation = _animation;
    bp = animation->GetBaseParameters();
    //static_cast<EFrameAnimation*>(animation)->SetFramesMStime();

    sprite = _sprite;
    sprite->createAnimatedPropertiesIfNone();

}


FrameAnimationInstance::FrameAnimationInstance(FrameAnimation *_animation, std::vector<Sprite*>&_sprites)
{

    kind = AnimationKind::FRAME_ANIMATION;
    animation = _animation;
    bp = animation->GetBaseParameters();
    //static_cast<EFrameAnimation*>(animation)->SetFramesMStime();

    sprites = new std::vector<Sprite*>();
    *sprites = _sprites;
    for(Sprite* o : *sprites){
        o->createAnimatedPropertiesIfNone();
    }

}


FrameAnimationInstance::~FrameAnimationInstance()
{
    if(sprites){
        delete sprites;
    }
}


void FrameAnimationInstance::SetStartingFrameIndex(int _startingFrameIndex)
{

    startingFrameIndex = _startingFrameIndex;

    // get 'startPlayTimeOffset'
    std::vector<AnimationFrame*>&frames = static_cast<FrameAnimation*>(animation)->GetFrames();
    int t = 0;
    for(int i=0; i<frames.size(); i++){
        if(i==startingFrameIndex){
            startPlayTimeOffset = t;
            break;
        }
        t += frames[i]->duration;
    }

}


void FrameAnimationInstance::OnPlayingStart()
{

    std::vector<AnimationFrame*>&frames = static_cast<FrameAnimation*>(animation)->GetFrames();

    activeFrameIndex = startingFrameIndex;
    if(activeFrameIndex<0 || activeFrameIndex>=frames.size()){
        activeFrameIndex = 0;
    }
    //activeAnimationFrame = frames[activeFrameIndex];

}


void FrameAnimationInstance::OnPlayingStop()
{

    //std::vector<AnimationFrame*>&frames = static_cast<FrameAnimation*>(animation)->GetFrames();

    activeFrameIndex = 0;
    //activeAnimationFrame = frames[activeFrameIndex];

}


int FrameAnimationInstance::Update(int msTimePoint, int _flags)
{

    int returnFlag = 0;


    ap.Reset();

    //print("A activeFrameIndex:" + std::to_string(activeFrameIndex) +"  msTimePoint:" + std::to_string(msTimePoint));

    std::vector<AnimationFrame*>&frames = static_cast<FrameAnimation*>(animation)->GetFrames();

    //---- manage activeFrameIndex on animation loop
    if(msTimePoint < frames[activeFrameIndex]->msTimePoint){
        activeFrameIndex = 0;
    }

    //----
    for(int i=activeFrameIndex; i<int(frames.size()); i++){
        if(i+1 < frames.size()){
            if(msTimePoint >= frames[i]->msTimePoint && msTimePoint < frames[i+1]->msTimePoint){
                activeFrameIndex = i;
                break;
            }
        }else{
            activeFrameIndex = i;
            break;
        }
    }

    //print("B activeFrameIndex:" + std::to_string(activeFrameIndex) +"  msTimePoint:" + std::to_string(msTimePoint));

    Update(frames[activeFrameIndex]);


    if(ap.changeFlags!=0){
        returnFlag |= AnimationPlayerFlags::ANIMATED_PROPERTIES_CHANGED;
    }

    return returnFlag;
}


/*
void EFrameAnimationInstance::Update(int _frameIndex)
{

    std::vector<AnimationFrame*>&frames = static_cast<FrameAnimation*>(animation)->GetFrames();

    Update(frames[_frameIndex]);
}
*/


void FrameAnimationInstance::Update(AnimationFrame *_animationFrame)
{

    //assert(activeAnimationFrame);
    //assert(_animationFrame);

    if(_animationFrame==nullptr){
        ap.Reset();
        return;
    }

    //if(_animationFrame == activeAnimationFrame) return;


    //if(_animationFrame->texture != activeAnimationFrame->texture){
        ap.sourceImage = _animationFrame->sourceImage;
        ap.changeFlags |= Sprite::Property::TEXTURE;
        ap.changeFlags |= Sprite::Property::FRAME;     // temporary
    //}

    //if(_animationFrame->offset.Equals(activeAnimationFrame->offset)==false){
        //ap.translation = _animationFrame->offset;
        ap.aniFrameOffset = _animationFrame->offset;
        ap.changeFlags |= Sprite::Property::POSITION;
    //}

    //if(_animationFrame->flip.Equals(activeAnimationFrame->flip)==false){
        ap.flip = _animationFrame->flip;
        ap.changeFlags |= Sprite::Property::FLIP;
    //}

    //activeAnimationFrame = _animationFrame;

        //print("ap.graphicItem:" + ap.graphicItem->name());

}



void FrameAnimationInstance::UpdateAnimatedSprites(bool _resetSpriteAnimatedProperties)
{

    if(_resetSpriteAnimatedProperties){
        if(sprite){
            sprite->resetAnimatedProperties();;

        }else if(sprites){
            for(Sprite *o : *sprites){
                o->resetAnimatedProperties();;
            }
        }
    }

    if(sprite){

        if(sprite->sourceSprite()->name()=="spriteA"){
            DummyFunction();
        }

        sprite->applyAnimatedProperties(ap);

    }else if(sprites){
        for(Sprite *o : *sprites){
            o->applyAnimatedProperties(ap);
        }
    }
}



void FrameAnimationInstance::ResetAnimatedProperties()
{
    ap.Reset();
}


int FrameAnimationInstance::currentFrameIntData()
{

    std::vector<AnimationFrame*>&frames = static_cast<FrameAnimation*>(animation)->GetFrames();
    return frames[activeFrameIndex]->dataFlags;

}




//================================================================================================


/*

void FrameAnimationPlayer::SetFrameAnimation(FrameAnimation * _frameAnimation)
{
    assert(state==stateIDLE);

    frameAnimation = _frameAnimation;

}


bool FrameAnimationPlayer::Play(FrameAnimation * _frameAnimation, int _indexFrame)
{
    Reset();
    frameAnimation = _frameAnimation;
    return Play(_indexFrame);
}


bool FrameAnimationPlayer::Play(int _indexFrame)
{
    if(frameAnimation==nullptr || frameAnimation->frames.empty()) return false;

    indexCurrentFrame = _indexFrame;
    countLoop = 0;

    if(frameAnimation->loopForever==false && frameAnimation->repeatAnimation && frameAnimation->startAtRepeatTime){
        msRepeat = GetRepeatTime(time.GetPassedNetTimeMS());
        state = stateWAITING_TO_REPEAT;

    }else{
        msFrameStartTime = time.GetPassedNetTimeMS();
        state = statePLAYING;
    }

    return true;
}


void FrameAnimationPlayer::Pause()
{
    if(state==stateIDLE || state==statePAUSED) return;

    stateStored = state;
    msTimeStored = time.GetPassedNetTimeMS();
    state = statePAUSED;
}


void FrameAnimationPlayer::Resume()
{
    if(state!=statePAUSED) return;

    state = stateStored;
    msFrameStartTime += time.GetPassedNetTimeMS() - msTimeStored;
}


void FrameAnimationPlayer::Stop()
{
    state = stateIDLE;
}


int FrameAnimationPlayer::Update()
{
    if(state==stateIDLE) return 0;

    assert(frameAnimation && frameAnimation->frames.empty()==false);


    int returnFlag = 0;

    if(state==statePLAYING){

        int msCurrentTime = time.GetPassedNetTimeMS();
        if(msCurrentTime-msFrameStartTime >= frameAnimation->frames[indexCurrentFrame]->duration * frameAnimation->scaleDuration){
            indexCurrentFrame ++;
            msFrameStartTime = msCurrentTime;


            if(indexCurrentFrame>=frameAnimation->frames.size()){

                if(frameAnimation->loopForever){
                    indexCurrentFrame = 0;

                }else{
                    countLoop++;
                    if(countLoop<frameAnimation->loopCount){
                        indexCurrentFrame = 0;

                    }else{
                        indexCurrentFrame--;

                        if(frameAnimation->repeatAnimation){
                            msRepeat = GetRepeatTime(msCurrentTime);
                            state = stateWAITING_TO_REPEAT;

                        }else{

                            msRepeat = -1;
                            state = stateLOOP_END;

                        }
                    }
                }
            }
            returnFlag = flagFRAME_CHANGED;
        }

    }else if(state==stateWAITING_TO_REPEAT){


        int msCurrentTime = time.GetPassedNetTimeMS();
        if(msCurrentTime>=msRepeat){
            indexCurrentFrame = 0;
            countLoop = 0;
            msFrameStartTime = msCurrentTime;
            state = statePLAYING;
            //---
            returnFlag = flagFRAME_CHANGED;
        }
    }

    return returnFlag;
}


void FrameAnimationPlayer::Reset()
{
    frameAnimation = nullptr;
    state = stateIDLE;
}


int FrameAnimationPlayer::GetRepeatTime(int _msCurrentTime)
{
    int msRepeatPeriod = frameAnimation->repeat_PeriodEnd-frameAnimation->repeat_PeriodStart;
    if(msRepeatPeriod<0) msRepeatPeriod = 0;
    //float fRnd = agk::Random(0,1000)/1000.0;
    float fRnd =  fRand(0.0, 1.0);
    int msTime = _msCurrentTime + frameAnimation->repeat_PeriodStart + msRepeatPeriod * fRnd;

    //qDebug() << "repeat time: " << (FrameAnimation->repeat_PeriodStart + msRepeatPeriod * fRnd)/1000.0;

    return msTime;
}


//==============================================================================================


void StandardSpriteFrameAnimationPlayer::OnChangedAnimationFrame()
{

    AnimationFrame *frame =  frameAnimationPlayer.GetActiveFrame();

    if(sprite->GetKind()==SpriteKind::STANDARD){
        StandardSprite* ss = static_cast<StandardSprite*>(sprite);

        if(frame->image){
            if(frame->flip != ss->GetFlip()){
                ss->SetFlip(frame->flip);
            }
            if(frame->offset.Equals(ss->GetAnimationFrameOffset())==false){
                ss->SetAnimationFrameOffset(frame->offset);
            }
        }
        ss->SetActiveImage(frame->image);

    }

}



//==============================================================================================



void SpritesFrameAnimationPlayer::OnChangedAnimationFrame()
{

    AnimationFrame *frame =  frameAnimationPlayer.GetActiveFrame();


    for(Sprite *s : sprites){
        if(s->GetKind()==SpriteKind::STANDARD){
            StandardSprite* ss = static_cast<StandardSprite*>(s);

            if(frame->image){
                if(frame->flip != ss->GetFlip()){
                    ss->SetFlip(frame->flip);
                }
                if(frame->offset.Equals(ss->GetAnimationFrameOffset())==false){
                    ss->SetAnimationFrameOffset(frame->offset);
                }
            }
            ss->SetActiveImage(frame->image);
        }
    }
}

*/


}
