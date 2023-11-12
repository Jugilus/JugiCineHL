#ifndef PLAYER__SYSTEM__USER_H
#define PLAYER__SYSTEM__USER_H


#include <memory>
#include <string>
#include <vector>

#include "jmSignal.h"
#include "jpSettings.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


struct InputProfiles;
class ItemsGroup;
class GuiTable;



class UserProfile : public BaseObject
{
public:

    UserProfile(const std::string &_name, int _id);

    const std::string &name(){ return mName; }
    int id(){ return mId; }

    InputProfiles* inputProfiles(){ return mInputProfiles.get(); }
    void _setInputProfiles(InputProfiles* _inputProfiles);
    void finalizeInputProfilesOnLoaded();


private:
    std::string mName;
    int mId = 0;                                        // unique number
    std::unique_ptr<InputProfiles>mInputProfiles;

    bool isInputProfilesStructureOK();

};



class UserManager : public BaseObject
{
public:

    UserManager(PlayedApp* _parentApp);
    ~UserManager();

    void deleteContent();


    void setDefaultUser();

    bool addUser(const std::string &_name);

    UserProfile* getUser(const std::string &_name, bool _setErrorMessage = true);
    bool deleteUser(const std::string &_name);

    std::vector<UserProfile*> &users(){ return mUsers; }
    UserProfile *activeUser(){ return mActiveUser; }
    UserProfile *setActiveUser(const std::string &_name);

    void saveUser(UserProfile *_user);
    bool loadUsers();
    bool loadUser(const std::string &filePath);
    bool loadActiveUserIdentification();
    void saveActiveUserIdentification();
    bool removeUserDataFile(UserProfile *_user);

    ItemsGroup* userNamesItems(){ return mUsersNamesItems.get(); }

    StringSignal* activeUserSignal(){ return &mSigActiveUser; }


private:
    PlayedApp* mParentPlayerApp = nullptr;         // LINK

    std::string mDataPath;
    std::vector<UserProfile*>mUsers;            // OWNED
    int mUserIdCounter = 1;

    UserProfile* mActiveUser = nullptr;         // LINK
    //StringVar* mActiveUserName = nullptr;       // LINK
    StringSignal mSigActiveUser;

    std::unique_ptr<UserProfile>mDefaultUser;


    //--- for gui table
    std::unique_ptr<ItemsGroup>mUsersNamesItems;


};



}


#endif // PLAYER__USER_PROFILE_H
