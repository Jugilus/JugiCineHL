#ifndef CLASS_MYEVENTHANDLER
#define CLASS_MYEVENTHANDLER

#include <jugimap.h>
#include "jmSignal.h"
#include <ncine/IAppEventHandler.h>
#include <ncine/IInputEventHandler.h>
#include <ncine/AppConfiguration.h>


class MyEventHandler : public ncine::IAppEventHandler, public ncine::IInputEventHandler
{
  public:
    ~MyEventHandler() override {}

    void onPreInit(ncine::AppConfiguration &config) override;
    void onInit() override;
    void onFrameStart() override;
    void onPostUpdate() override;



//#ifdef __ANDROID__
    void onTouchDown(const ncine::TouchEvent &event) override;
    void onTouchUp(const ncine::TouchEvent &event) override;
    void onTouchMove(const ncine::TouchEvent &event) override;
//#endif

    void onMouseButtonPressed(const ncine::MouseEvent &event) override;
    void onMouseButtonReleased(const ncine::MouseEvent &event) override;
    void onMouseMoved(const ncine::MouseState &state) override;
    void onScrollInput(const ncine::ScrollEvent &event) override;

    void onKeyPressed(const ncine::KeyboardEvent &event) override;
    void onKeyReleased(const ncine::KeyboardEvent &event) override;

    void onJoyButtonPressed(const ncine::JoyButtonEvent &event) override;
    void onJoyButtonReleased(const ncine::JoyButtonEvent &event) override;
    void onJoyMappedButtonPressed(const ncine::JoyMappedButtonEvent &event) override;
    void onJoyMappedButtonReleased(const ncine::JoyMappedButtonEvent &event) override;
    void onJoyHatMoved(const ncine::JoyHatEvent &event) override;
    void onJoyAxisMoved(const ncine::JoyAxisEvent &event) override;
    //void onJoyMappedButtonPressed(const ncine::JoyMappedButtonEvent &event) override;
    //void onJoyMappedButtonReleased(const ncine::JoyMappedButtonEvent &event) override;
    //void onJoyMappedAxisMoved(const ncine::JoyMappedAxisEvent &event) override;
    void onJoyConnected(const ncine::JoyConnectionEvent &event) override;
    void onJoyDisconnected(const ncine::JoyConnectionEvent &event) override;

    void onTextInput(const ncine::TextInputEvent &event) override;


    void onShutdown() override;
    void onResizeWindow(int width, int height) override;


private:

    //std::vector<jugimap::KeyCode>keysConversionTable;

};






class SignalTest
{
public:



    //static const int none = 0;
    static const int value0 = 1 << 0;
    static const int value1 = 1 << 1;
    static const int value2 = 1 << 2;
    static const int value3 = 1 << 3;
    static const int value4 = 1 << 4;
    static const int value5 = 1 << 5;
    static const int value6 = 1 << 6;
    static const int value7 = 1 << 7;

    static const int intValue0 = 0;
    static const int intValue1 = 1;
    static const int intValue2 = 2;
    static const int intValue3 = 3;
    static const int intValue4 = 4;
    static const int intValue5 = 5;
    static const int intValue6 = 6;
    static const int intValue7 = 7;


    SignalTest(){ mRunning = true; }

    void test();

    void test_BitsetSignal_1();
    void test_BitsetSignal_2();
    void dbgPrint_BitsetSignal(jugimap::BitsetSignal sigBitField, unsigned int nBits = 8);

    void test_IntSignal_1();
    void test_IntSignal_2();
    void dbgPrint_IntSignal(jugimap::IntSignal sigInt, unsigned int nValues = 8);


    bool mRunning = false;




};

extern SignalTest gSignalTest;













#endif
