#ifndef JUGIMAP_TIMELINE_ANIMATION_H
#define JUGIMAP_TIMELINE_ANIMATION_H

#include <vector>
#include "jmParameter.h"
#include "jmAnimationCommon.h"
#include "jmGlobal.h"



namespace jugimap{



class TimelineAnimation;
class AnimationTrack;
class AnimationMember;
class MapLoader_bin;



class AnimationKey
{
public:

    friend class MapLoader_bin;
    friend class AnimationTrack;
    friend class TimelineAnimation;
    friend class AnimationTrackState;
    friend class SourceObjectsLoader_xml;
    friend class SourceObjectsWriter_xml;



    static AnimationKey * Create(AnimationTrack *_animationTrack);
    static AnimationKey * Copy(AnimationKey* _srcAnimationKey);


    AnimationKey(AnimationTrack *_keyAnimation) : animationTrack(_keyAnimation){}
    AnimationKey(AnimationTrack *_keyAnimation, int _time) : animationTrack(_keyAnimation), time(_time){}
    virtual ~AnimationKey(){}

    virtual void CopyFrom(AnimationKey *_key) = 0;
    virtual void CopyFrom(AnimationKey *_aniKeyDataStart, AnimationKey *_aniKeyDataEnd, float blend) = 0;

    AnimationTrack * GetAnimationTrack(){ return animationTrack; }
    AnimationTrackKind GetKind();

    int GetTime(){ return time; }
    Easing &GetEasing(){ return easing; }


private:
    AnimationTrack *animationTrack = nullptr;     // LINK
    int time = 0;
    Easing easing;

    //--- editor
    float timeUnscaled = 0;
    Vec2f posMarker;
    AnimationKey* prevKey = nullptr;               // LINK
    AnimationKey* nextKey = nullptr;               // LINK




#ifdef JUGI_EDITOR
    friend class jugi::gui::TAnimationKeysWidget;
    friend class jugi::gui::TimelineAnimationEditorGUI;
    friend class jugi::gui::TimelineAnimationEditor_AnimationWidgets;
    friend class jugi::gui::TimelineAnimationEditor_AniTrackWidgets;
    friend class jugi::gui::TimelineAnimationEditor_KeyWidgets;
#endif

};


class AKTranslation : public AnimationKey
{
public:

    AKTranslation(AnimationTrack *_keyAnimation) : AnimationKey(_keyAnimation){}
    AKTranslation(AnimationTrack *_keyAnimation, int _time) : AnimationKey(_keyAnimation, _time){}

    virtual void CopyFrom(AnimationKey *_key) override
    {
        position = static_cast<AKTranslation*>(_key)->position;
    }

    virtual void CopyFrom(AnimationKey *_aniKeyDataStart, AnimationKey *_aniKeyDataEnd, float blend) override
    {
        AKTranslation *kStart = static_cast<AKTranslation*>(_aniKeyDataStart);
        AKTranslation *kEnd = static_cast<AKTranslation*>(_aniKeyDataEnd);

        position.x = kStart->position.x + blend * (kEnd->position.x - kStart->position.x);
        position.y = kStart->position.y + blend * (kEnd->position.y - kStart->position.y);
    }


    Vec2f position;
};


class AKScaling : public AnimationKey
{
public:

    AKScaling(AnimationTrack *_keyAnimation) : AnimationKey(_keyAnimation){}
    AKScaling(AnimationTrack *_keyAnimation, int _time) : AnimationKey(_keyAnimation, _time){}


    virtual void CopyFrom(AnimationKey *_key) override
    {
        scale = static_cast<AKScaling*>(_key)->scale;
        uniform = static_cast<AKScaling*>(_key)->uniform;
    }

    virtual void CopyFrom(AnimationKey *_aniKeyDataStart, AnimationKey *_aniKeyDataEnd, float blend) override
    {
        AKScaling *kStart = static_cast<AKScaling*>(_aniKeyDataStart);
        AKScaling *kEnd = static_cast<AKScaling*>(_aniKeyDataEnd);

        scale.x = kStart->scale.x + blend * (kEnd->scale.x - kStart->scale.x);
        scale.y = kStart->scale.y + blend * (kEnd->scale.y - kStart->scale.y);
        uniform = kStart->uniform;
    }


    Vec2f scale = Vec2f(1.0f, 1.0f);
    bool uniform = true;
};


class AKFlipHide : public AnimationKey
{
public:

    AKFlipHide(AnimationTrack *_keyAnimation) : AnimationKey(_keyAnimation){}
    AKFlipHide(AnimationTrack *_keyAnimation, int _time) : AnimationKey(_keyAnimation, _time){}


    virtual void CopyFrom(AnimationKey *_key) override
    {
        flip = static_cast<AKFlipHide*>(_key)->flip;
        hidden = static_cast<AKFlipHide*>(_key)->hidden;
    }

    virtual void CopyFrom(AnimationKey *_aniKeyDataStart, AnimationKey *_aniKeyDataEnd, float blend) override
    {
        AKFlipHide *kStart = static_cast<AKFlipHide*>(_aniKeyDataStart);
        AKFlipHide *kEnd = static_cast<AKFlipHide*>(_aniKeyDataEnd);

        flip = kStart->flip;
        hidden = kStart->hidden;
    }


    Vec2i flip;
    bool hidden = false;
};


class AKRotation : public AnimationKey
{
public:

    AKRotation(AnimationTrack *_keyAnimation) : AnimationKey(_keyAnimation){}
    AKRotation(AnimationTrack *_keyAnimation, int _time) : AnimationKey(_keyAnimation, _time){}


    virtual void CopyFrom(AnimationKey *_key) override
    {
        rotation = static_cast<AKRotation*>(_key)->rotation;
    }

    virtual void CopyFrom(AnimationKey *_aniKeyDataStart, AnimationKey *_aniKeyDataEnd, float blend) override
    {
        AKRotation *kStart = static_cast<AKRotation*>(_aniKeyDataStart);
        AKRotation *kEnd = static_cast<AKRotation*>(_aniKeyDataEnd);

        rotation = kStart->rotation + blend * (kEnd->rotation - kStart->rotation);
    }


    float rotation = 0.0;
};


class AKAlphaChange : public AnimationKey
{
public:

    AKAlphaChange(AnimationTrack *_keyAnimation) : AnimationKey(_keyAnimation){}
    AKAlphaChange(AnimationTrack *_keyAnimation, int _time) : AnimationKey(_keyAnimation, _time){}


    virtual void CopyFrom(AnimationKey *_key) override
    {
        alpha = static_cast<AKAlphaChange*>(_key)->alpha;
    }

    virtual void CopyFrom(AnimationKey *_aniKeyDataStart, AnimationKey *_aniKeyDataEnd, float blend) override
    {
        AKAlphaChange *kStart = static_cast<AKAlphaChange*>(_aniKeyDataStart);
        AKAlphaChange *kEnd = static_cast<AKAlphaChange*>(_aniKeyDataEnd);

        alpha = kStart->alpha + blend * (kEnd->alpha - kStart->alpha);
    }


    float alpha = 1.0;
};


class AKOverlayColorChange : public AnimationKey
{
public:

    AKOverlayColorChange(AnimationTrack *_keyAnimation) : AnimationKey(_keyAnimation){}
    AKOverlayColorChange(AnimationTrack *_keyAnimation, int _time) : AnimationKey(_keyAnimation, _time){}


    virtual void CopyFrom(AnimationKey *_key) override
    {
        color = static_cast<AKOverlayColorChange*>(_key)->color;
    }

    virtual void CopyFrom(AnimationKey *_aniKeyDataStart, AnimationKey *_aniKeyDataEnd, float blend) override
    {
        AKOverlayColorChange *kStart = static_cast<AKOverlayColorChange*>(_aniKeyDataStart);
        AKOverlayColorChange *kEnd = static_cast<AKOverlayColorChange*>(_aniKeyDataEnd);

        color.r = kStart->color.r + blend * (kEnd->color.r - kStart->color.r);
        color.g = kStart->color.g + blend * (kEnd->color.g - kStart->color.g);
        color.b = kStart->color.b + blend * (kEnd->color.b - kStart->color.b);
        color.a = kStart->color.a + blend * (kEnd->color.a - kStart->color.a);
    }


    ColorRGBA color;
};


class AKPathMovement : public AnimationKey
{
public:

    AKPathMovement(AnimationTrack *_keyAnimation) : AnimationKey(_keyAnimation){}
    AKPathMovement(AnimationTrack *_keyAnimation, int _time) : AnimationKey(_keyAnimation, _time){}


    virtual void CopyFrom(AnimationKey *_key) override
    {
        relDistance = static_cast<AKPathMovement*>(_key)->relDistance;
    }

    virtual void CopyFrom(AnimationKey *_aniKeyDataStart, AnimationKey *_aniKeyDataEnd, float blend) override
    {
        AKPathMovement *kStart = static_cast<AKPathMovement*>(_aniKeyDataStart);
        AKPathMovement *kEnd = static_cast<AKPathMovement*>(_aniKeyDataEnd);

        relDistance = kStart->relDistance + blend * (kEnd->relDistance - kStart->relDistance);
    }


    float relDistance = 0.0f;
    // editor
    float speed = 0;         // pixels/second

};


class AKFrameChange : public AnimationKey
{
public:

    AKFrameChange(AnimationTrack *_keyAnimation) : AnimationKey(_keyAnimation){}
    AKFrameChange(AnimationTrack *_keyAnimation, int _time) : AnimationKey(_keyAnimation, _time){}


    virtual void CopyFrom(AnimationKey *_key) override
    {
        frameImageIndex = static_cast<AKFrameChange*>(_key)->frameImageIndex;
        animationFrame = static_cast<AKFrameChange*>(_key)->animationFrame;
    }

    virtual void CopyFrom(AnimationKey *_aniKeyDataStart, AnimationKey *_aniKeyDataEnd, float blend) override
    {
        AKFrameChange *kStart = static_cast<AKFrameChange*>(_aniKeyDataStart);

        frameImageIndex = kStart->frameImageIndex;
        animationFrame = kStart->animationFrame;
    }


    int frameImageIndex = 0;
    AnimationFrame animationFrame;
};


class AKFrameAnimation : public AnimationKey
{
public:
    constexpr static const float fSpeedMin = 0.25f;
    constexpr static const float fSpeedMax = 4.0f;

    AKFrameAnimation(AnimationTrack *_keyAnimation) : AnimationKey(_keyAnimation){}
    AKFrameAnimation(AnimationTrack *_keyAnimation, int _time) : AnimationKey(_keyAnimation, _time){}


    virtual void CopyFrom(AnimationKey *_key) override
    {
        animationName = static_cast<AKFrameAnimation*>(_key)->animationName;
        completeLoops = static_cast<AKFrameAnimation*>(_key)->completeLoops;
        discardAnimationsQueue = static_cast<AKFrameAnimation*>(_key)->discardAnimationsQueue;
        fSpeed = static_cast<AKFrameAnimation*>(_key)->fSpeed;
    }

    virtual void CopyFrom(AnimationKey *_aniKeyDataStart, AnimationKey *_aniKeyDataEnd, float blend) override
    {
        AKFrameAnimation *kStart = static_cast<AKFrameAnimation*>(_aniKeyDataStart);

        animationName = kStart->animationName;
        completeLoops = kStart->completeLoops;
        discardAnimationsQueue = kStart->discardAnimationsQueue;
        fSpeed = kStart->fSpeed;

    }


    std::string animationName;
    bool completeLoops = true;
    bool discardAnimationsQueue = false;
    float fSpeed = 1.0;
};


class AKTimelineAnimation : public AnimationKey
{
public:
    constexpr static const float fSpeedMin = 0.25f;
    constexpr static const float fSpeedMax = 4.0f;


    AKTimelineAnimation(AnimationTrack *_keyAnimation) : AnimationKey(_keyAnimation){}
    AKTimelineAnimation(AnimationTrack *_keyAnimation, int _time) : AnimationKey(_keyAnimation, _time){}


    virtual void CopyFrom(AnimationKey *_key) override
    {
        animationName = static_cast<AKTimelineAnimation*>(_key)->animationName;
        completeLoops = static_cast<AKTimelineAnimation*>(_key)->completeLoops;
        discardAnimationsQueue = static_cast<AKTimelineAnimation*>(_key)->discardAnimationsQueue;
        fSpeed = static_cast<AKTimelineAnimation*>(_key)->fSpeed;
    }

    virtual void CopyFrom(AnimationKey *_aniKeyDataStart, AnimationKey *_aniKeyDataEnd, float blend) override
    {
        AKTimelineAnimation *kStart = static_cast<AKTimelineAnimation*>(_aniKeyDataStart);

        animationName = kStart->animationName;
        completeLoops = kStart->completeLoops;
        discardAnimationsQueue = kStart->discardAnimationsQueue;
        fSpeed = kStart->fSpeed;

    }


    std::string animationName;
    bool completeLoops = true;
    bool discardAnimationsQueue = false;
    float fSpeed = 1.0;
};


class AKMeta : public AnimationKey
{
public:

    AKMeta(AnimationTrack *_keyAnimation) : AnimationKey(_keyAnimation){}
    AKMeta(AnimationTrack *_keyAnimation, int _time) : AnimationKey(_keyAnimation, _time){}



    virtual void CopyFrom(AnimationKey *_key) override
    {
        label = static_cast<AKMeta*>(_key)->label;
        parameters = static_cast<AKMeta*>(_key)->parameters;
        dataFlags = static_cast<AKMeta*>(_key)->dataFlags;
        paused = static_cast<AKMeta*>(_key)->paused;
    }

    virtual void CopyFrom(AnimationKey *_aniKeyDataStart, AnimationKey *_aniKeyDataEnd, float blend) override
    {
        AKMeta *kStart = static_cast<AKMeta*>(_aniKeyDataStart);

        label = kStart->label;
        parameters = kStart->parameters;
        dataFlags = kStart->dataFlags;
        paused = kStart->paused;
    }


    std::string label;
    std::string label2;
    CParameters parameters;
    int dataFlags = 0;
    bool paused = false;
};





//=====================================================================================================================


class TimelineAnimation;


class AnimationTrackParameters
{
public:

    ColorOverlayBlend colorBlend = ColorOverlayBlend::NORMAL;
    std::string pathNameID;
    bool reverseDirectionOnClosedShape = false;
    bool pathDirectionOrientation = false;
    float pathRelDistanceOffset = 0.0;
    float pathLength = 0.0;
};





class AnimationTrack
{
public:
    friend class TimelineAnimation;
    friend class MapLoader_bin;
    friend class SourceObjectsWriter_xml;
    friend class SourceObjectsLoader_xml;



    //--- constructors
    AnimationTrack(){}
    AnimationTrack(const AnimationTrack &ka);
    AnimationTrack& operator=(const AnimationTrack &ka);
    ~AnimationTrack();


    std::string GetName(){return name;}
    AnimationTrackKind GetKind(){return kind;}


    bool IsDisabled(){ return disabled; }
    void SetDisabled(bool _disabled){ disabled = _disabled; }
    TimelineAnimation* GetAnimation(){ return animation; }
    std::vector<AnimationKey*> &GetAnimationKeys(){return animationKeys;}
    AnimationTrackParameters &GetTrackParameters(){ return tp; }


#ifdef JUGI_EDITOR

    friend class jugi::gui::TimelineAnimationEditorGUI;
    friend class jugi::gui::TimelineAnimationEditor_AnimationWidgets;
    friend class jugi::gui::TimelineAnimationEditor_AniTrackWidgets;
    friend class jugi::gui::TimelineAnimationEditor_KeyWidgets;
    friend class jugi::gui::TimelineAnimationEditor;
    friend class jugi::gui::TAnimationKeysWidget;

    //--- constructors
    //EAnimationTrack(){}
    //EAnimationTrack(const EAnimationTrack &ka);
    //EAnimationTrack& operator=(const EAnimationTrack &ka);
    //~EAnimationTrack();


    void InitKeys();

    //--- editing
    AnimationKey *AddAnimationKey(int _time);
    void SortAnimationKeys();
    void RemoveAnimationKey(AnimationKey *_Key);

    void UpdatePathAnimationSpeeds();

    float GetAverageSpeedForKey(AnimationKey * _key);


private:
    //int countKindAni = 0;
    //int listOrder = 0;

#endif



private:
    AnimationTrackKind kind;
    TimelineAnimation * animation = nullptr;       // LINK
    //---
    bool disabled = false;
    std::string name;

    //---
    AnimationTrackParameters tp;
    std::vector<AnimationKey*>animationKeys;       // OWNED

    //--- editor
    int countKindAni = 0;
    int listOrder = 0;
    AnimationKey* prevKey = nullptr;               // LINK
    AnimationKey* nextKey = nullptr;               // LINK

};


//=====================================================================================================================


class AnimationMember
{
public:
    friend class TimelineAnimation;
    friend class MapLoader_bin;
    friend class SourceObjectsWriter_xml;
    friend class SourceObjectsLoader_xml;


    AnimationMember(){}
    AnimationMember(const std::string &_childNameID) : nameID(_childNameID){}
    ~AnimationMember();


    std::string GetNameID(){ return nameID; }
    std::vector<AnimationTrack*>& GetAnimationTracks(){ return animationTracks;}
    bool IsDisabled(){ return disabled; }
    void SetDisabled(bool _disabled){ disabled = _disabled; }

    AnimationTrack* FindAnimationTrack(AnimationTrackKind _kind);
    bool DeleteEmptyTracks();


private:
    std::string nameID;
    std::vector<AnimationTrack*>animationTracks;
    bool disabled = false;
};


//=====================================================================================================================



///\ingroup Animation
/// \brief The TimelineAnimation class represents the sprite timeline animation from JugiMap Editor.
class  TimelineAnimation : public Animation
{
public:
    friend class MapLoader_bin;
    friend class SourceSprite;
    friend class SourceObjectsLoader_xml;
    friend class SourceObjectsWriter_xml;


    //---
    std::vector<AnimationMember*>& GetAnimationMembers(){return animationMembers;}
    AnimationTrack* GetMetaAnimationTrack(){ return metaAnimationTrack; }

    AnimationMember* GetRootAnimationMember();
    AnimationMember* GetChildAnimationMember(const std::string &_childNameID);
    void ObtainDuration();


protected:

    //----
    TimelineAnimation(SourceSprite *_sourceObject);
    TimelineAnimation(const std::string &_nameID, SourceSprite *_sourceObject);
    TimelineAnimation(const TimelineAnimation &taSrc);
    TimelineAnimation& operator=(const TimelineAnimation &mkaSrc);
    ~TimelineAnimation() override;


private:
    std::vector<AnimationMember*>animationMembers;             // OWNED
    AnimationTrack *metaAnimationTrack = nullptr;              // OWNED


#ifdef JUGI_EDITOR

    friend class jugi::gui::TimelineAnimationEditorGUI;
    friend class jugi::gui::TimelineAnimationEditor_AnimationWidgets;
    friend class jugi::gui::TimelineAnimationEditor_AniTrackWidgets;
    friend class jugi::gui::TimelineAnimationEditor_KeyWidgets;
    friend class jugi::gui::TimelineAnimationEditor;
    friend class jugi::gui::TAnimationKeysWidget;
    friend class jugi::gui::TSourceObjectMultikeyAnimationsStored;
    friend class AnimationTrack;

    //----
    //ETimelineAnimation(ESourceObject *_sourceObject);
    //ETimelineAnimation(const std::string &_nameID, ESourceObject *_sourceObject);
    //ETimelineAnimation(const ETimelineAnimation &taSrc);
    //ETimelineAnimation& operator=(const ETimelineAnimation &mkaSrc);
    //~ETimelineAnimation() override;


    //--- EDITOR
    bool AddAnimationTrack(AnimationTrackKind _kind);
    bool DeleteAnimationTrack(const std::string &_aniName);
    bool DeleteEmptyChildAnimationMembers();
    bool HasNoAnimationTracks();

    float GetScaleDuration(){return scaleDuration;}
    void SetScaleDuration(float _scaleDuration);


    //EAnimationMember* GetRootAnimationMember();
    //EAnimationMember* GetChildAnimationMember(const std::string &_childNameID);
    void AddChildAnimationMember(const std::string &_childNameID);

    void SetActiveAnimationMember(AnimationMember* _animationMember);
    void ResetEditAnimationPointers();

    AnimationMember* GetActiveAnimationMember(){ return activeAnimationMember;}
    std::vector<AnimationTrack*>& GetEditedAnimationTracks(){return editedAnimationsTracksCopy; }



private:
    //std::vector<EAnimationMember*>animationMembers;             // OWNED
    //EAnimationTrack *metaAnimationTrack = nullptr;              // OWNED


    float scaleDuration = 1.0;

    // editor
    AnimationKey *editedKey = nullptr;                            // LINK
    AnimationMember* activeAnimationMember = nullptr;             // LINK
    std::vector<AnimationTrack*>editedAnimationsTracksCopy;       // LINKs


#endif


};


}




#endif
