#ifndef JUGIMAP_GUI_TEXT_INPUT_H
#define JUGIMAP_GUI_TEXT_INPUT_H

#include <string>

#include "jmColor.h"
#include "jmTimer.h"

#include "jpGuiCommon.h"


namespace jugimap{
	

class TextSprite;
class CompositeSprite;
class StandardSprite;


/// \addtogroup Gui
/// @{





struct TextInputCfg : public GuiWidgetCfg
{

    TextInputCfg(WidgetFactory *_factory, const std::string &_name) : GuiWidgetCfg(_factory,_name){}

    bool initCfg(const pugi::xml_node &_node) override;


    std::string backgroundBoxSprite;
    int idleFrame = 0;
    int cursorOverFrame = 0;
    int activeFrame = 0;
    int disabledFrame = 0;

    std::string textSprite;
    //std::string cursorSprite;

    std::string idleTextColor;
    std::string activeTextColor;


};




///\brief The GuiTextField class provides a widget for text input.
///
/// The text editing is started with GuiTextInput::Start and completed by pressing the Enter key.
/// This widget currently works only with the ASCII character set.
class GuiTextInput : public GuiWidget
{
public:


    ///\brief Construct a new gui text input from the given *_rootSprite*.
    //GuiTextInput(CompositeSprite* _rootSprite);

    GuiTextInput(TextInputCfg *_cfg, CompositeSprite* _rootSprite);

    //~GuiTextInput();

    bool initConnections(GuiSystem *_guiSystem) override;
    //bool init() override;

    void setDisabled(bool _disabled) override;
    void setVisible(bool _visible) override;
    void update() override;


    TextInputCfg* cfg(){ return mCfg; }


    ///\brief Start the text editing.
    void start();

    ///\brief Set the text and start editing.
    void start(const std::string &_text);


    ///\brief Set the text.
    void setText(const std::string &_text);

    ///\brief Returns the text.
    const std::string &text(){ return mText; }


    ///\brief Set the maximum number of characters of the text.
    void setTextMaximumLength(int _maxLength){ mMaxLength = _maxLength; }


    ///\brief Set if the escape key aborts editing.
    void setAllowEscToAbort(bool _allowEscToAbort){ mAllowEscToAbort = _allowEscToAbort; }


    ///\brief Set if empty text is allowed.
    void setAllowEmptyText(bool _allowEmptyText){ mAllowEmptyText = _allowEmptyText; }


    ///\brief Set a string of characters which can not appear in the text.
    void setBannedChars(const std::vector<std::string> &_bannedChars){ mBannedChars = _bannedChars; }


    ///\brief Set a string of banned characters in file name.
    void makeCharsFileNameSafe();


    ///\brief Set the flashing interval of the text cursor in milliseconds.
    //void setCursorFlashingSpeed(int _cursorFlashingMS);
    void setCursorFlashingDuration(float _durationS);

    void finish();


private:
    TextInputCfg *mCfg = nullptr;                   //LINK
    CompositeSprite* mRootSprite = nullptr;           // LINK
    StandardSprite* mInputBoxSprite = nullptr;      // LINK
    TextSprite *mTextSprite = nullptr;           // LINK
    TextSprite *mCursorSprite = nullptr;         // LINK


    std::string mText;
    std::string mDefaultText;

    std::vector<int>mTextUnicodes;



    int mMaxLength = 20;
    char mCurrentChar = 0;
    bool mActive = false;
    ColorRGBA mCursorColor{0,255,255};



    //int mCursorFlashingMS = 250;
    //int mCursorFlashingStartMS = 0;
    float mCursorFlashingDurationS = 0.25f;
    Timer mCursorFlashingTimer;

    bool mAllowEscToAbort = false;
    bool mAllowEmptyText = true;
   // std::string mBannedChars;
    std::vector<std::string>mBannedChars;

    std::vector<char>mAllowedAsciCodes;


    // visual appearance for widget states
    int mIndexInputBoxSprite_idle = 0;
    int mIndexInputBoxSprite_CO = 0;
    int mIndexInputBoxSprite_active = 0;
    int mIndexInputBoxSprite_disabled = 0;

    ColorRGBA mTextColor_idle{255,255,255};
    //ColorRGBA textColor_CO{255,255,255};
    ColorRGBA mTextColor_active{255,255,255};
    //ColorRGBA textColor_disabled{255,255,255};


    void updateTextOnChange();


};



//------------------------------------------------------------------------------------------


class TextInputFactory : public WidgetFactory
{
public:

    TextInputFactory(const std::string &_typeSignature, WidgetType _type) : WidgetFactory(_typeSignature, _type){}

    GuiWidgetCfg* createWidgetCfg(const std::string &_name) override;
    GuiWidget* createWidget(GuiWidgetCfg* _cfg, Sprite *_rootSprite) override;

};


/// @}


	
}




#endif
