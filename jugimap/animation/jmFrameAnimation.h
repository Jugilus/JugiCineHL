#ifndef JUGIMAP_FRAME_ANIMATION_H
#define JUGIMAP_FRAME_ANIMATION_H

#include <vector>
#include "jmAnimationCommon.h"



namespace jugimap{


class SourceImage;
class SourceSprite;
class Sprite;
//class StandardSprite;
//class JugiMapBinaryLoader;





///\ingroup Animation
/// \brief The FrameAnimation class represents the sprite frame animation from JugiMap Editor.
class  FrameAnimation : public Animation
{
public:
    friend class MapLoader_bin;
    friend class SourceSprite;
    friend class SourceObjectsLoader_xml;
    friend class SourceObjectsWriter_xml;



    FrameAnimation(SourceSprite* _sourceObject);
    FrameAnimation(SourceSprite* _sourceObject, const std::string &_nameID);
    FrameAnimation(const FrameAnimation &fa);
    FrameAnimation& operator=(const FrameAnimation &fa);
    ~FrameAnimation();

    std::vector<AnimationFrame*>&GetFrames(){ return frames; }
    //void SetFramesMStime();
    int ObtainDuration();


private:
    std::vector<AnimationFrame*>frames;                         // OWNED

    float scaleDuration = 1.0;
    AnimationFrame *editedFrame = nullptr;                // LINK



#ifdef JUGI_EDITOR
public:

    friend class jugi::gui::FrameAnimationEditorGUI;
    friend class jugi::gui::FrameAnimationEditor_AnimationWidgets;
    friend class jugi::gui::TAnimationFramesWidget;
    friend struct jugi::gui::TSourceObjectFrameAnimationsStored;
    friend class jugi::gui::FrameAnimationEditor;
    friend void oldStuff::_MakeSets_StandardSprites_LegacyLSDformat(std::vector<oldStuff::TObjectData *> &, const std::string &);


    float GetScaleDuration(){ return scaleDuration; }
    void SetScaleDuration(float _scaleDuration);


    void FlipFramesX();
    void FlipFramesY();

    static void FindAndAssignMirrorAnimations(SourceSprite* _Sourceobject);
    static void AssignMirrorAnimations(SourceSprite* _Sourceobject);
    static void RebuildMirrorAnimationsIfAvailable(FrameAnimation *_FrameAnimation);

    void _SetHorMirroredAnimation(FrameAnimation *_horMirroredAnimation){ horMirroredAnimation = _horMirroredAnimation; }
    void _SetVerMirroredAnimation(FrameAnimation *_verMirroredAnimation){ verMirroredAnimation = _verMirroredAnimation; }


private:

    //std::vector<TAnimationFrame*>frames;                         // OWNED

    //----
    //float scaleDuration = 1.0;

    FrameAnimation *horMirroredAnimation = nullptr;       // LINK animacija je zrcalna verzija druge animacije in uporablja njene parametre
    FrameAnimation *verMirroredAnimation = nullptr;       // LINK
    std::string horMirror,verMirror;                            // pomožne variable pri loadanju

    //TAnimationFrame *editedFrame = nullptr;                // LINK

#endif

};









//==================================================================================



///\ingroup Animation
///\brief A class for using frame animations in animation players.
class FrameAnimationInstance : public AnimationInstance
{
public:
    friend class EFrameAnimationPlayer;


    ///\brief Constructor.
    ///
    /// Create an FrameAnimationInstance object for animating the given sprite *_sprite* with the given frame animation *_animation*.
    FrameAnimationInstance(FrameAnimation *_animation, Sprite *_sprite);


    ///\brief Constructor.
    ///
    /// Create an FrameAnimationInstance object for animating the given sprites *_sprites* with the given frame animation *_animation*.
    /// The sprites must have the same SourceSprite!
    FrameAnimationInstance(FrameAnimation *_animation, std::vector<Sprite*>&_sprites);


    ///\brief Destructor.
    ~FrameAnimationInstance() override;


    ///\brief Returns the assigned sprite; if a vector of sprites has been assigned it returns nullptr!
    Sprite* GetSprite(){ return sprite;}


    ///\brief Returns a pointer to the vector of assigned sprites; if one sprite has been assigned it returns nullptr.
    ///
    /// The vector of assigned sprites object is owned by FrameAnimationInstance object! Sprites themselves are link pointers.
    std::vector<Sprite*>* GetSprites(){ return sprites;}


    ///\brief Returns the starting index in the sequence of frames of this animation.
    int GetStartingFrameIndex(){ return startingFrameIndex;}


    ///\brief Set the starting index in the sequence of frames of this animation to the given *_startingFrameIndex*.
    void SetStartingFrameIndex(int _startingFrameIndex);


    ///\brief Get a reference to the AnimatedProperties object of this animation instance.
    AnimatedProperties &GetAnimatedProperties(){ return ap; }

    int currentFrameIntData();

    int Update(int msTimePoint, int _flags) override;
    void UpdateAnimatedSprites(bool _resetSpriteAnimatedProperties) override;
    void ResetAnimatedProperties() override;
    void OnPlayingStart() override;
    void OnPlayingStop() override;




    //EFrameAnimationInstance(EFrameAnimation *_animation, ESprite *_sprite);
    //EFrameAnimationInstance(EFrameAnimation *_animation, std::vector<ESprite*>&_sprites);
    //~EFrameAnimationInstance();


    //void SetStartingFrameIndex(int _startingFrameIndex);

    //EAnimatedProperties &GetAnimatedProperties(){ return ap; }

    //int Update(int msTimePoint, int _flags) override;
    //void UpdateAnimatedSprites(bool _resetSpriteAnimatedProperties) override;
    //void ResetAnimatedProperties() override;

    void Update(int _frameIndex);
    void Update(AnimationFrame *_animationFrame);




private:

    //ESprite* sprite = nullptr;                                       // LINK
    //std::vector<ESprite*>*sprites = nullptr;                         // vector OWNED, sprites LINKS

    //EAnimatedProperties ap;

    //int startingFrameIndex = 0;
    //int activeFrameIndex = 0;

    //EFrameAnimationInstance(const EFrameAnimationInstance& _src) = delete ;
    //EFrameAnimationInstance& operator=(const EFrameAnimationInstance& _src) = delete;







private:
    Sprite* sprite = nullptr;                                       // LINK
    std::vector<Sprite*>*sprites = nullptr;                         // vector OWNED, sprites LINKS

    AnimatedProperties ap;

    int startingFrameIndex = 0;
    int activeFrameIndex = 0;

    FrameAnimationInstance(const FrameAnimationInstance& _src) = delete ;
    FrameAnimationInstance& operator=(const FrameAnimationInstance& _src) = delete;

    //void Update(TAnimationFrame *_animationFrame);

};




}
#endif
