#ifndef PLAYER__GFXOBJECTS__COMMON__H
#define PLAYER__GFXOBJECTS__COMMON__H


#include <memory>

#include "jmSignal.h"
#include "jpSettings.h"
#include "jpVariables.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class TextSprite;
class PlayedScene;






class GfxObject : public SignalCallback
{
public:
    virtual ~GfxObject();

    virtual bool initConnections(PlayedScene* _scene) = 0;

    const std::string &name(){ return mName; }
    PlayedScene* parentPlayerScene(){ return mParentPlayerScene; }

    virtual void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true);
    virtual void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage=true);

protected:
    PlayedScene *mParentPlayerScene = nullptr;
    std::string mName;



};



//--------------------------------------------------------------------------------





class GSpritesObject : public GfxObject
{
public:

    GSpritesObject(const pugi::xml_node &_node);
    GSpritesObject(std::vector<void*>_objects, GfxObjectType _objType);

    ~GSpritesObject() override;

    virtual bool initConnections(PlayedScene* _scene);

    void setVisible(bool _visible);

    GfxObjectType objectType(){ return mObjectType; }
    std::vector<void*> &objects(){ return mObjects; }

    BoolSignal & visibleSignal(){ return mSigVisible; }

    bool onSignalSet(Signal *_signal) override;

    virtual void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage=true);
    virtual void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage=true);



protected:
    std::vector<void*>mObjects;                         // LINKS
    GfxObjectType mObjectType = GfxObjectType::NOT_DEFINED;

    BoolSignal mSigVisible;

    //cfg
    std::string mObjectName;
    std::string mSrcMapName;

    void _setVisible(bool _visible);

};


//--------------------------------------------------------------------------------


class GTextSprite : public GfxObject
{
public:

    GTextSprite(const pugi::xml_node &_node);
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
    TextSprite *mTextSprite = nullptr;      // LINK
    StringSignal mSigText;
    BoolSignal mSigVisible;


    //--- cfg
    std::string mTextSpriteName;
    std::string mSourceMapName;

};



//--------------------------------------------------------------------------------


class GfxObjectsGroup
{
public:

    bool initConnections(PlayedScene* _scene);


    GfxObject* getGfxObject(const std::string &_name);
    std::vector<GfxObject*> &gfxObjects(){ return mGfxObjects; }


private:
    std::vector<GfxObject*>mGfxObjects;

};






}






#endif
