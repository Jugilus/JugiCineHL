#include "ncine/FileSystem.h"
#include "pugixml/pugixml.hpp"
#include "jmStreams.h"
#include "jmSprite.h"
#include "jmCompositeSprite.h"
#include "jmSystem.h"
#include "jpPlayedApp.h"
#include "jpItemsCommon.h"
#include "jpItemsTable.h"
#include "jmUtilities.h"
#include "jpInput.h"
#include "jpUser.h"


namespace jugimap{






UserProfile::UserProfile(const std::string &_name, int _id)

{
    mName = _name;
    mId = _id;

    mType = BaseObjectType::USER_PROFILE;

    //mInputProfiles = app->inputSystem()->buildInputProfiles();
    //mInputProfiles = new InputProfiles();

}


void UserProfile::_setInputProfiles(InputProfiles *_inputProfiles)
{
    mInputProfiles.reset(_inputProfiles);
}



void UserProfile::finalizeInputProfilesOnLoaded()
{

    InputSystem *is = app->inputSystem();

    if(isInputProfilesStructureOK()){

        for(KeyboardProfile *p : mInputProfiles->mKeyboardProfiles){
            for(int i=0; i<is->gameInputCommands().size(); i++){
                p->keyboardInputCommands().at(i).mGameInputCommand = is->gameInputCommands().at(i);
            }
        }

        for(int j=0 ; j<mInputProfiles->mJoystickProfiles.size(); j++){
            JoystickProfile *p = mInputProfiles->mJoystickProfiles[j];

            p->_setJoystick(&joysticks[j]);
            for(int i=0; i<is->gameInputCommands().size(); i++){
                p->joystickInputCommands().at(i).mGameInputCommand = is->gameInputCommands().at(i);
            }
        }

    }else{

        mInputProfiles.reset(app->inputSystem()->newDefaultInputProfiles());
    }

}



bool UserProfile::isInputProfilesStructureOK()
{

    InputSystem *is = app->inputSystem();

    if(mInputProfiles->mJoystickProfiles.size() != joysticks.size()){
        return false;
    }

    for(KeyboardProfile *p : mInputProfiles->mKeyboardProfiles){
        if(p->keyboardInputCommands().size()!=is->gameInputCommands().size()){
            return false;
        }
    }

    for(JoystickProfile *p : mInputProfiles->mJoystickProfiles){
        if(p->joystickInputCommands().size()!=is->gameInputCommands().size()){
            return false;
        }
    }

    return true;
}

//----------------------------------------------------------------------------


UserManager::UserManager(PlayedApp *_parentApp)
{

    mType = BaseObjectType::USERS_DATABASE;
    mParentPlayerApp = _parentApp;

    //---
    mActiveUserName = static_cast<StringVar*>(_parentApp->systemVariables()->addVariable(new StringVar("activeUserName", "")));
    mUsersNamesItems.reset(new ItemsGroup("userNames", this));

}


UserManager::~UserManager()
{
    deleteContent();
}


void UserManager::deleteContent()
{

    for(UserProfile* u : mUsers){
        delete u;
    }
    mUsers.clear();

}


void UserManager::setDefaultUser()
{

    mDefaultUser.reset(new UserProfile("defaultUser", 0));
    mDefaultUser->_setInputProfiles(app->inputSystem()->newDefaultInputProfiles());
    mActiveUser = mDefaultUser.get();
    app->inputSystem()->setActiveUserProfiles(mDefaultUser->inputProfiles());

}


bool UserManager::addUser(const std::string &_name)
{

    if(_name.empty()){
        return false;
    }
    if(getUser(_name, false)){
        return false;
    }

    UserProfile * user = new UserProfile(_name, mUserIdCounter);
    user->_setInputProfiles(app->inputSystem()->newDefaultInputProfiles());
    mUsers.push_back(user);
    mUserIdCounter++;

    //--- add new item to the users items list
    GameItem *item = mUsersNamesItems->addNameItem(_name, true);
    assert(mUsersNamesItems->table());                          // set during the initialization of the user selector !
    mUsersNamesItems->table()->setSelectedItem(item);

    saveUser(user);


    return true;
}


bool UserManager::deleteUser(const std::string &_name)
{

    bool deleted = false;


    for(int i = int(mUsers.size())-1; i>=0; i-- ){
        UserProfile *u = mUsers[i];
        if(u->name()==_name){

            // change active profile if deleted
            if(u==mActiveUser){
                mActiveUser = nullptr;
                if(i+1 < mUsers.size()){
                    mActiveUser = mUsers[i+1];
                }else if(i-1 >= 0){
                    mActiveUser = mUsers[i-1];
                }
            }
            //---
            mUsers.erase(mUsers.begin()+i);
            removeUserDataFile(u);
            mUsersNamesItems->deleteItem(_name);

            //---
            deleted = true;
            delete u;
            break;
        }
    }


    if(deleted){
        //mUsersNamesItems->deleteItem(_name);
        if(mActiveUser==nullptr && mUsers.empty()==false){
            mActiveUser = mUsers.back();
        }

        if(mActiveUser){
            saveActiveUserIdentification();
        }

        //saveUsers();
    }

    if(mActiveUser){
        mActiveUserName->setValue(mActiveUser->name());

        //---
        GameItem * item = mUsersNamesItems->getItem(mActiveUser->name());
        mUsersNamesItems->table()->setSelectedItem(item);

        //---
        app->inputSystem()->setActiveUserProfiles(mActiveUser->inputProfiles());
    }

    return deleted;
}


UserProfile* UserManager::getUser(const std::string &_name, bool _setErrorMessage)
{

    for(UserProfile* u : mUsers){
        if(u->name()==_name){
            return u;
        }
    }

    if(_setErrorMessage){
        dbgSystem.addMessage("An user with name '" + _name + "' not found!");
    }

    return nullptr;

}


UserProfile *UserManager::setActiveUser(const std::string &_name)
{

    for(UserProfile* u : mUsers){
        if(u->name()==_name){
            mActiveUser = u;
            mActiveUserName->setValue(mActiveUser->name());
            saveActiveUserIdentification();
            //---
            app->inputSystem()->setActiveUserProfiles(mActiveUser->inputProfiles());

            //--- select active user item (in general this is not needed as active user is obtained from an item, so it's already selected
            if(mUsersNamesItems->table()->selectedItem()){
                GameItem *item = mUsersNamesItems->table()->selectedItem();
                if(item->name() != _name){
                    item = mUsersNamesItems->getItem(_name);
                    mUsersNamesItems->table()->setSelectedItem(item);
                }
            }

            return u;
        }
    }

    assert(false);

    return nullptr;
}



void UserManager::saveUser(UserProfile *_user)
{

    if(_user==nullptr) return;

    std::string filePath = app->saveDataDir() + "/user" + std::to_string(_user->id())+".dat";

    BinaryStreamWriter stream = BinaryStreamWriter(filePath);

    stream.writeInt(jpSaveSignatures::USER_DATA);
    stream.writeInt(_user->id());
    stream.writeString(_user->name());

    //---
    _user->inputProfiles()->save(stream);

    stream.Close();

}



bool UserManager::loadUsers()
{

    for(UserProfile* u : mUsers){
        delete u;
    }
    mUsers.clear();
    mActiveUser = nullptr;
    mUserIdCounter = 0;


    //---
    std::string dirPath = app->saveDataDir();

    if(ncine::FileSystem::isDirectory(dirPath.c_str())==false){
        return false;
    }

    ncine::FileSystem::Directory dir(dirPath.c_str());

    while(true){
        const char *_fileName = dir.readNext();
        if(_fileName==nullptr){
            break;
        }
        std::string fileName(_fileName);
        if(fileName.empty()==false && fileName.substr(0, 1)=="."){
             continue;
        }
        std::string filePath = dirPath + "/" + fileName;
        if(ncine::FileSystem::isFile(filePath.c_str())){

            std::string suffix = StdString::suffixFromFilePath(filePath);

            if(suffix=="dat"){
                loadUser(filePath);
            }
        }
    }

    if(mUsers.empty()){
        return false;
    }

    //--- sort users to be in order of creation
    sort(mUsers.begin(), mUsers.end(),
          [](UserProfile* u1, UserProfile* u2) -> bool { return u1->id() < u2->id();} );

    //---
    for(UserProfile* p : mUsers){
        p->finalizeInputProfilesOnLoaded();
    }

    //--- active user
    loadActiveUserIdentification();

    if(mActiveUser){
        // place active user to the last position so that it will appear at the top of selector
        StdVector::removeElement(mUsers, mActiveUser);
        mUsers.push_back(mActiveUser);

    }else{
        mActiveUser = mUsers.back();
        mActiveUserName->setValue(mActiveUser->name());
        saveActiveUserIdentification();
    }

    app->inputSystem()->setActiveUserProfiles(mActiveUser->inputProfiles());


    //---
    for(UserProfile* p : mUsers){
        mUsersNamesItems->addNameItem(p->name(), true);
    }
    // at this point guiTable of mUsersNamesItems is not yet assigned so we don't select the active item here


    return true;

}



bool UserManager::loadUser(const std::string &filePath)
{

    StdBinaryFileStreamReader stream(filePath);

    if(stream.IsOpen() == false){
        return false;
    }

    int signature = stream.ReadInt();
    if(signature!=jpSaveSignatures::USER_DATA){
        stream.Close();
        return false;
    }


    int userId = stream.ReadInt();
    std::string userName = stream.ReadString();
    UserProfile* user = new UserProfile(userName, userId);
    user->_setInputProfiles(new InputProfiles());

    signature = stream.ReadInt();
    assert(signature==jpSaveSignatures::INPUT_PROFILES);
    user->inputProfiles()->load(stream);

    //---
    stream.Close();

    //---
    mUsers.push_back(user);

    //---
    mUserIdCounter = std::max(mUserIdCounter, userId);
    mUserIdCounter++;


    return true;
}



bool UserManager::loadActiveUserIdentification()
{

    std::string filePath = app->saveDataDir() + "/activeUser.cur";

    StdBinaryFileStreamReader s(filePath);
    if(s.IsOpen()){
        std::string currentUserName = s.ReadString();
        int currentUserId = s.ReadInt();
        s.Close();

        mActiveUser = getUser(currentUserName, false);
        if(mActiveUser){
            mActiveUserName->setValue(mActiveUser->name());
            return true;
        }
    }

    return false;
}



void UserManager::saveActiveUserIdentification()
{

    std::string filePath = app->saveDataDir() + "/activeUser.cur";

    BinaryStreamWriter s(filePath);
    if(s.IsOpen()){
        s.writeString(mActiveUser->name());
        s.writeInt(mActiveUser->id());
        s.Close();
    }

}


bool UserManager::removeUserDataFile(UserProfile *_user)
{

    std::string filePath = app->saveDataDir() + "/user" + std::to_string(_user->id())+".dat";

    bool deleted = ncine::FileSystem::deleteFile(filePath.c_str());
    return deleted;

}




}
