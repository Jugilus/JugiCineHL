#ifndef JUGIMAP_STORAGE_H
#define JUGIMAP_STORAGE_H


#include <vector>
#include <bitset>
#include <string>

#include "jmSystem.h"
#include "jmCommonFunctions.h"

namespace jugimap {



template<class T>
class SimpleStorage
{
public:


    SimpleStorage(const std::string &_name) : mName(_name) {}

    /*
    ~EnginesControllersCfgsGroup()
    {

        for(T * c : mEnginesControllersCfgs){
            delete c;
        }

    }
    */


    T &addObject(const T &_obj)
    {
        mObjects.push_back(_obj);
        return mObjects.back();
    }


    T *getObject(const std::string &_name, bool _setErrorMessage=true)
    {

        for(T &obj : mObjects){
            if(obj.name()==_name){
                return obj;
            }
        }

        if(_setErrorMessage){
            dbgSystem.addMessage("A 'EnginesControllerCfg' object with name '" + _name + "' not found!");
        }

        return nullptr;
    }


    std::vector<T> & objects(){ return mObjects; }



private:
    std::string mName;
    std::vector<T>mObjects;      // OWNED  (stored as pointers to avoid unvalidation problems when linking objects as references)

};



template<class T>
class SimpleStorage<T*>
{
public:


    SimpleStorage(const std::string &_name) : mName(_name) {}

    /*
    ~EnginesControllersCfgsGroup()
    {

        for(T * c : mEnginesControllersCfgs){
            delete c;
        }

    }
    */


    T*  addObject(T* _obj)
    {
        mObjects.push_back(_obj);
        return mObjects.back();
    }



    T* getObject(const std::string &_name, bool _setErrorMessage=true)
    {

        for(T* obj : mObjects){
            if(obj->name()==_name){
                return obj;
            }
        }

        if(_setErrorMessage){
            dbgSystem.addMessage("A 'EnginesControllerCfg' object with name '" + _name + "' not found!");
        }

        return nullptr;
    }


    std::vector<T*> & objects(){ return mObjects; }


private:
    std::string mName;
    std::vector<T*>mObjects;      // OWNED  (stored as pointers to avoid unvalidation problems when linking objects as references)

};


}

#endif


