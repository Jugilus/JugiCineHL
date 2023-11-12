#ifndef PLAYER__COMPONENTS__MESSAGE_PANEL__H
#define PLAYER__COMPONENTS__MESSAGE_PANEL__H


#include <memory>

#include "jpQueries.h"
#include "jpComponentsCommon.h"
#include "jpVariables.h"


namespace pugi{
class xml_node;
}

namespace jugimap{

class GTextSprite;
class GuiButton;
class GuiTextField;
class PlayedScene;
class MessagePanelSignalParser;



class MessagePanel : public Component
{
public:

    enum class Mode
    {
        NOT_DEFINED,
        DELETE_USER
    };


    MessagePanel(const std::string &_name);
    MessagePanel(const pugi::xml_node &_node);

    bool initCfg(PlayedScene *_scene, const pugi::xml_node &_node) override;
    bool initConnections(PlayedScene *_scene) override;

    void start() override;
    void update(UpdateMode &_updateMode) override;
    void onStateEnded() override;

    void obtainSignal_signalQuery(SignalQuery &_signalQuery, ParsedSignalPath &_psp, bool _setErrorMessage = true) override;
    void obtainSignal_signalSetter(SignalSetter &_signalSetter, ParsedSignalPath &_psp, bool _setErrorMessage = true) override;


private:

    GuiTextField *mTextField = nullptr;                 // LINK
    GuiButton *mOkButton = nullptr;                     // LINK

    Mode mMode = Mode::NOT_DEFINED;


    //SignalQuery mSQclose;
    CompositeQuery mSQclose;
    BoolSignal mSigClose;

    MessagePanelSignalParser *mMessagePanelSignalParser = nullptr;      //LINK


    //--- cfg
    struct Cfg
    {
        std::string mTextField;
        std::string mOkButton;

        std::string mSigClose;
    };
    std::unique_ptr<Cfg>mCfg;

};


//------------------------------------------------------------------------------------



class MessagePanelFactory : public CustomComponentFactory
{
public:

    MessagePanelFactory(const std::string &_typeSignature) : CustomComponentFactory(_typeSignature){}

    Component * createComponent() override { return new MessagePanel(mTypeSignature); }

};



//------------------------------------------------------------------------------------


class MessagePanelSignalParser : public SignalParser
{
public:

    MessagePanelSignalParser(std::vector<std::string> _keyWords) : SignalParser(_keyWords){}

    void parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject* obj1=nullptr, BaseObject* obj2=nullptr) override;

};



}






#endif
