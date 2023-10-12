#include "jmTimelineAnimation.h"



namespace jugimap{


AnimationKey * AnimationKey::Create(AnimationTrack *_animationTrack)
{

    AnimationTrackKind kind = _animationTrack->GetKind();

    if(kind==AnimationTrackKind::TRANSLATION){
        return new AKTranslation(_animationTrack);

    }else if (kind==AnimationTrackKind::SCALING){
        return new AKScaling(_animationTrack);

    }else if (kind==AnimationTrackKind::ROTATION){
        return new AKRotation(_animationTrack);

    }else if (kind==AnimationTrackKind::ALPHA_CHANGE){
        return new AKAlphaChange(_animationTrack);

    }else if (kind==AnimationTrackKind::OVERLAY_COLOR_CHANGE){
        return new AKOverlayColorChange(_animationTrack);

    }else if (kind==AnimationTrackKind::PATH_MOVEMENT){
        return new AKPathMovement(_animationTrack);

    }else if (kind==AnimationTrackKind::FRAME_CHANGE){
        return new AKFrameChange(_animationTrack);

    }else if (kind==AnimationTrackKind::FRAME_ANIMATION){
        return new AKFrameAnimation(_animationTrack);

    }else if (kind==AnimationTrackKind::TIMELINE_ANIMATION){
        return new AKTimelineAnimation(_animationTrack);

    }else if (kind==AnimationTrackKind::FLIP_HIDE){
        return new AKFlipHide(_animationTrack);

    }else if (kind==AnimationTrackKind::META){
        return new AKMeta(_animationTrack);
    }

    return nullptr;
}


AnimationKey * AnimationKey::Copy(AnimationKey* _srcAnimationKey)
{

    AnimationTrackKind kind = _srcAnimationKey->GetKind();

    if(kind==AnimationTrackKind::TRANSLATION){
        return new AKTranslation(*static_cast<AKTranslation*>(_srcAnimationKey));

    }else if (kind==AnimationTrackKind::SCALING){
        return new AKScaling(*static_cast<AKScaling*>(_srcAnimationKey));

    }else if (kind==AnimationTrackKind::ROTATION){
        return new AKRotation(*static_cast<AKRotation*>(_srcAnimationKey));

    }else if (kind==AnimationTrackKind::ALPHA_CHANGE){
        return new AKAlphaChange(*static_cast<AKAlphaChange*>(_srcAnimationKey));

    }else if (kind==AnimationTrackKind::OVERLAY_COLOR_CHANGE){
        return new AKOverlayColorChange(*static_cast<AKOverlayColorChange*>(_srcAnimationKey));

    }else if (kind==AnimationTrackKind::PATH_MOVEMENT){
        return new AKPathMovement(*static_cast<AKPathMovement*>(_srcAnimationKey));

    }else if (kind==AnimationTrackKind::FRAME_CHANGE){
        return new AKFrameChange(*static_cast<AKFrameChange*>(_srcAnimationKey));

    }else if (kind==AnimationTrackKind::FRAME_ANIMATION){
        return new AKFrameAnimation(*static_cast<AKFrameAnimation*>(_srcAnimationKey));

    }else if (kind==AnimationTrackKind::TIMELINE_ANIMATION){
        return new AKTimelineAnimation(*static_cast<AKTimelineAnimation*>(_srcAnimationKey));

    }else if (kind==AnimationTrackKind::FLIP_HIDE){
        return new AKFlipHide(*static_cast<AKFlipHide*>(_srcAnimationKey));

    }else if (kind==AnimationTrackKind::META){
        return new AKMeta(*static_cast<AKMeta*>(_srcAnimationKey));
    }

    return nullptr;

}


AnimationTrackKind AnimationKey::GetKind()
{
    return animationTrack->GetKind();
}



 //========================================================================================================



 AnimationTrack::AnimationTrack(const AnimationTrack &ka)
 {

     disabled = ka.disabled;
     kind = ka.kind;
     name = ka.name;
     tp = ka.tp;
     countKindAni = ka.countKindAni;
     listOrder = ka.listOrder;

     //--- keys
     for(AnimationKey *ak : ka.animationKeys){
         AnimationKey *akCopy = AnimationKey::Copy(ak);
         akCopy->animationTrack = this;
         animationKeys.push_back(akCopy);
     }

 }


 AnimationTrack& AnimationTrack::operator=(const AnimationTrack &ka)
 {
     if(this==&ka) return *this;

     disabled = ka.disabled;
     kind = ka.kind;
     name = ka.name;
     tp = ka.tp;
     countKindAni = ka.countKindAni;
     listOrder = ka.listOrder;


     for(AnimationKey *ak : animationKeys){
         delete ak;
     }
     animationKeys.clear();

     for(AnimationKey *ak : ka.animationKeys){
         AnimationKey *akCopy = AnimationKey::Copy(ak);
         akCopy->animationTrack = this;
         animationKeys.push_back(akCopy);
     }

     return *this;
 }




 AnimationTrack::~AnimationTrack()
 {
     for(AnimationKey *ak : animationKeys){
         delete ak;
     }
 }



 //======================================================================================================


 AnimationMember::~AnimationMember()
 {
     for(AnimationTrack* ka : animationTracks){
         delete ka;
     }
 }


 bool AnimationMember::DeleteEmptyTracks()
 {

     bool deleted = false;

     if(animationTracks.empty()) return deleted;

     for(int i = int(animationTracks.size())-1; i>=0; i-- ){
         AnimationTrack* ka = animationTracks.at(i);
         if(ka->GetAnimationKeys().empty()){
             animationTracks.erase(animationTracks.begin()+i);
             delete ka;
             deleted = true;
         }
     }

     return deleted;
 }


 AnimationTrack* AnimationMember::FindAnimationTrack(AnimationTrackKind _kind)
 {

     for(AnimationTrack* ta : animationTracks){
         if(ta->GetKind()==_kind){
             return ta;
         }
     }

     return nullptr;
 }



 //======================================================================================================


 TimelineAnimation::TimelineAnimation(SourceSprite *_sourceObject) : Animation(_sourceObject)
 {
     kind = AnimationKind::TIMELINE_ANIMATION;
 }


 TimelineAnimation::TimelineAnimation(const std::string &_name, SourceSprite *_sourceObject) : Animation(_name, _sourceObject)
 {
     kind = AnimationKind::TIMELINE_ANIMATION;
     animationMembers.push_back(new AnimationMember());
 }


/*
 ETimelineAnimation::ETimelineAnimation(const ETimelineAnimation &taSrc) : EAnimation(taSrc)
 {

     for(EAnimationMember *amSrc : taSrc.animationMembers){

         EAnimationMember *am = new EAnimationMember();
         am->nameID = amSrc->nameID;
         am->disabled = amSrc->disabled;
         animationMembers.push_back(am);

         for(EAnimationTrack *atSrc : amSrc->animationTracks){
             if(atSrc->GetKind()==AnimationTrackKind::META) continue;   // in the editor this tAnimation is temporarilly stored in the edited vector

             EAnimationTrack *at = new EAnimationTrack(*atSrc);
             at->animation = this;
             am->animationTracks.push_back(at);
         }
     }

     if(taSrc.metaAnimationTrack){
         metaAnimationTrack = new EAnimationTrack(*taSrc.metaAnimationTrack);
         metaAnimationTrack->animation = this;
     }

 }
*/

 TimelineAnimation::TimelineAnimation(const TimelineAnimation &taSrc) : Animation(taSrc)
 {

#ifdef JUGI_EDITOR
     scaleDuration = taSrc.scaleDuration;
#endif

     for(AnimationMember *amSrc : taSrc.animationMembers){

         AnimationMember *am = new AnimationMember();
         am->nameID = amSrc->nameID;
         am->disabled = amSrc->disabled;
         animationMembers.push_back(am);

         for(AnimationTrack *atSrc : amSrc->animationTracks){
             if(atSrc->GetKind()==AnimationTrackKind::META) continue;   // in the editor this tAnimation is temporarilly stored in the edited vector

             AnimationTrack *at = new AnimationTrack(*atSrc);
             at->animation = this;
             am->animationTracks.push_back(at);

#ifdef JUGI_EDITOR
             for(int i=0; i<atSrc->animationKeys.size(); i++){
                 if(taSrc.editedKey==atSrc->animationKeys[i]){
                     editedKey = at->animationKeys[i];
                 }
             }
#endif
         }
     }

     if(taSrc.metaAnimationTrack){
         metaAnimationTrack = new AnimationTrack(*taSrc.metaAnimationTrack);
         metaAnimationTrack->animation = this;

#ifdef JUGI_EDITOR
         for(int i=0; i<taSrc.metaAnimationTrack->animationKeys.size(); i++){
             if(taSrc.editedKey==taSrc.metaAnimationTrack->animationKeys[i]){
                 editedKey = metaAnimationTrack->animationKeys[i];
             }
         }
#endif
     }

 }


/*
 *
 ETimelineAnimation& ETimelineAnimation::operator=(const ETimelineAnimation &taSrc)
 {

     if(this==&taSrc) return *this;

    EAnimation::operator=(taSrc);


     //--- keys
     for(EAnimationMember *mas : animationMembers){
         delete mas;
     }
     animationMembers.clear();


     for(EAnimationMember *amSrc : taSrc.animationMembers){

         EAnimationMember *am = new EAnimationMember();
         am->nameID = amSrc->nameID;
         am->disabled = amSrc->disabled;
         animationMembers.push_back(am);

         for(EAnimationTrack *atSrc : amSrc->animationTracks){
             if(atSrc->GetKind()==AnimationTrackKind::META) continue;

             EAnimationTrack *at = new EAnimationTrack(*atSrc);
             at->animation = this;
             am->animationTracks.push_back(at);
         }
     }

     if(metaAnimationTrack){
         delete metaAnimationTrack;
         metaAnimationTrack = nullptr;
     }

     if(taSrc.metaAnimationTrack){
         metaAnimationTrack = new EAnimationTrack(*taSrc.metaAnimationTrack);
         metaAnimationTrack->animation = this;
     }

     return *this;
 }

*/


 TimelineAnimation& TimelineAnimation::operator=(const TimelineAnimation &taSrc)
 {

     if(this==&taSrc) return *this;

    Animation::operator=(taSrc);

#ifdef JUGI_EDITOR
     scaleDuration = taSrc.scaleDuration;
#endif

     //--- keys
     for(AnimationMember *mas : animationMembers){
         delete mas;
     }
     animationMembers.clear();


     for(AnimationMember *amSrc : taSrc.animationMembers){

         AnimationMember *am = new AnimationMember();
         am->nameID = amSrc->nameID;
         am->disabled = amSrc->disabled;
         animationMembers.push_back(am);

         for(AnimationTrack *atSrc : amSrc->animationTracks){
             if(atSrc->GetKind()==AnimationTrackKind::META) continue;

             AnimationTrack *at = new AnimationTrack(*atSrc);
             at->animation = this;
             am->animationTracks.push_back(at);

#ifdef JUGI_EDITOR
             for(int i=0; i<atSrc->animationKeys.size(); i++){
                 if(taSrc.editedKey==atSrc->animationKeys[i]){
                     editedKey = at->animationKeys[i];
                 }
             }
#endif
         }
     }

     if(metaAnimationTrack){
         delete metaAnimationTrack;
         metaAnimationTrack = nullptr;
     }

     if(taSrc.metaAnimationTrack){
         metaAnimationTrack = new AnimationTrack(*taSrc.metaAnimationTrack);
         metaAnimationTrack->animation = this;

#ifdef JUGI_EDITOR
         for(int i=0; i<taSrc.metaAnimationTrack->animationKeys.size(); i++){
             if(taSrc.editedKey==taSrc.metaAnimationTrack->animationKeys[i]){
                 editedKey = metaAnimationTrack->animationKeys[i];
             }
         }
#endif
     }

     return *this;
 }

/*
 ETimelineAnimation::~ETimelineAnimation()
 {

     if(metaAnimationTrack){
         delete metaAnimationTrack;
     }

     for(EAnimationMember* kas : animationMembers){
         delete kas;
     }

 }
 */


 TimelineAnimation::~TimelineAnimation()
 {

     if(metaAnimationTrack){

#ifdef JUGI_EDITOR
         if(activeAnimationMember){
             std::vector<AnimationTrack*>& editedKeyAnimations = activeAnimationMember->animationTracks;
             if(editedKeyAnimations.empty()==false && editedKeyAnimations.front()==metaAnimationTrack){
                 editedKeyAnimations.erase(editedKeyAnimations.begin());
             }

         }
#endif

         delete metaAnimationTrack;
     }

     for(AnimationMember* kas : animationMembers){
         delete kas;
     }

 }




 AnimationMember* TimelineAnimation::GetRootAnimationMember()
 {
     assert(animationMembers.empty()==false);

     return animationMembers.front();
 }


 AnimationMember* TimelineAnimation::GetChildAnimationMember(const std::string &_childNameID)
 {

     for(int i = 1; i<animationMembers.size(); i++){
         if(animationMembers[i]->nameID ==_childNameID){
             return animationMembers[i];
         }
     }
     return nullptr;
 }


 void TimelineAnimation::ObtainDuration()
 {

     duration = 0;

     for(AnimationMember *mas : animationMembers){
         for(AnimationTrack *ka : mas->animationTracks){
             for(AnimationKey *Key : ka->animationKeys){
                 if(Key->time > duration) duration = Key->time;
             }
         }
     }

     if(metaAnimationTrack){
         for(AnimationKey *Key : metaAnimationTrack->animationKeys){
             if(Key->time > duration) duration = Key->time;
         }
     }
 }



}


