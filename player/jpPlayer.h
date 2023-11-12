#ifndef PLAYER_H
#define PLAYER_H


#include <memory>
#include <string>


namespace jugimap{


class PlayedApp;


class Player
{
public:

enum class State
{

    NORMAL,
    RELOAD_APP_PART_1,
    RELOAD_APP_PART_2,
    ERROR
};


    Player();

    void preInit();
    void init();

    void update(float _framePeriod);


private:
    std::unique_ptr<PlayedApp>mApp;

    State mState = State::NORMAL;
    bool mShowGui = true;
    bool mShowOverlay = false;
    bool mShowEntityShapes = false;
    bool mPaused = false;

    //bool updateActiveSceneLogic();
    void showErrorMessage();
    void showMessage(const std::string &_text);
    void drawDebugOverlay();

};


extern Player *player;


}


#endif
