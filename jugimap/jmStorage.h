#ifndef JUGIMAP_STORAGE_H
#define JUGIMAP_STORAGE_H

#include <array>
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


    T*  addObject(const std::string &_name, bool _setErrorMessage=true)
    {

        for(T &obj : mObjects){
            if(obj.name()==_name){

                if(_setErrorMessage){
                    dbgSystem.addMessage("An object with name '" + _name + "' already exists in storage '" + mName +"'!");
                }
                return nullptr;
            }
        }


        mObjects.push_back(T(_name));
        return &mObjects.back();
    }


    T*  addObject()
    {
        mObjects.push_back(T());
        return &mObjects.back();
    }


    T *getObject(const std::string &_name, bool _setErrorMessage=true)
    {

        for(T &obj : mObjects){
            if(obj.name()==_name){
                return &obj;
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


    ~SimpleStorage()
    {
        for(T* obj : mObjects){
            delete obj;
        }
    }


    T*  addObject(T* _obj, bool _setErrorMessage=true)
    {

        for(T* obj : mObjects){
            if(obj->name()==_obj->name()){

                if(_setErrorMessage){
                    dbgSystem.addMessage("An object with name '" + obj->name() + "' already exists in storage '" + mName +"'!");
                }
                return nullptr;
            }
        }

        mObjects.push_back(_obj);
        return mObjects.back();
    }


    T*  addObject(const std::string &_name, bool _setErrorMessage=true)
    {

        for(T* obj : mObjects){
            if(obj->name()==_name){

                if(_setErrorMessage){
                    dbgSystem.addMessage("An object with name '" + _name + "' already exists in storage '" + mName +"'!");
                }
                return nullptr;
            }
        }


        mObjects.push_back(new T(_name));
        return mObjects.back();
    }


    T*  addObject()
    {
        mObjects.push_back(new T());
        return &mObjects.back();
    }


    T* getObject(const std::string &_name, bool _setErrorMessage=true)
    {

        for(T* obj : mObjects){
            if(obj->name()==_name){
                return obj;
            }
        }

        if(_setErrorMessage){
            dbgSystem.addMessage("An object with name '" + _name + "' not found in storage '" + mName +"'!");
        }

        return nullptr;
    }


    std::vector<T*> & objects(){ return mObjects; }


private:
    std::string mName;
    std::vector<T*>mObjects;      // OWNED  (stored as pointers to avoid unvalidation problems when linking objects as references)

};



template<class T>
class SimpleNoNameStorage
{
public:


    SimpleNoNameStorage(){}
    SimpleNoNameStorage(const std::string &_dbgObjectName) : mDbgObjectNamePrefix(_dbgObjectName){}



    T *addObject(const T &_obj, bool _setErrorMessage=true)
    {

        for(T &obj : mObjects){
            if(obj.name()==_obj.name()){

                if(_setErrorMessage){
                    dbgSystem.addMessage(mDbgObjectNamePrefix +" with name '" + _obj.name() + "' already exists!");
                }
                return nullptr;
            }
        }

        mObjects.push_back(_obj);
        return &mObjects.back();
    }


    T*  addObject(const std::string &_name, bool _setErrorMessage=true)
    {

        for(T &obj : mObjects){
            if(obj.name()==_name){

                if(_setErrorMessage){
                    dbgSystem.addMessage(mDbgObjectNamePrefix + " with name '" + _name + "' already exists!");
                }
                return nullptr;
            }
        }


        mObjects.push_back(T(_name));
        return &mObjects.back();
    }


    T *getObject(const std::string &_name, bool _setErrorMessage=true)
    {

        for(T &obj : mObjects){
            if(obj.name()==_name){
                return &obj;
            }
        }

        if(_setErrorMessage){
            dbgSystem.addMessage(mDbgObjectNamePrefix + " '" + _name + "' not found!");
        }

        return nullptr;
    }


    std::vector<T> & objects(){ return mObjects; }



private:
    std::vector<T>mObjects;      // OWNED  (stored as pointers to avoid unvalidation problems when linking objects as references)
    std::string mDbgObjectNamePrefix{"An object"};

};



template<class T>
class SimpleNoNameStorage<T*>
{
public:


    SimpleNoNameStorage(){}
    SimpleNoNameStorage(const std::string &_dbgObjectName) : mDbgObjectNamePrefix(_dbgObjectName){}


    ~SimpleNoNameStorage()
    {
        for(T* obj : mObjects){
            delete obj;
        }
    }



    T* addObject(T* _obj, bool _setErrorMessage=true)
    {

        for(T* obj : mObjects){
            if(obj->name()==_obj->name()){

                if(_setErrorMessage){
                    dbgSystem.addMessage(mDbgObjectNamePrefix +" with name '" + _obj->name() + "' already exists!");
                }
                return nullptr;
            }
        }

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
            dbgSystem.addMessage(mDbgObjectNamePrefix +" '" + _name + "' not found'!");
        }

        return nullptr;
    }


    std::vector<T*> & objects(){ return mObjects; }


private:
    std::vector<T*>mObjects;      // OWNED  (stored as pointers to avoid unvalidation problems when linking objects as references)
    std::string mDbgObjectNamePrefix{"An object"};

};




template<class T>
class PrebuiltStorage
{
public:

    void preBuild(int _size)
    {
        mObjects.resize(_size);
    }


    T *getObject(const std::string &_name, bool _setErrorMessage=true)
    {

        for(T &obj : mObjects){
            if(obj.name()==_name){
                return &obj;
            }
        }

        if(_setErrorMessage){
            dbgSystem.addMessage("An object with name '" + _name + "' not found!");
        }

        return nullptr;
    }


    std::vector<T> & objects(){ return mObjects; }


private:
    std::vector<T>mObjects;      // OWNED  (stored as pointers to avoid unvalidation problems when linking objects as references)

};



struct PoolObjectPos
{
    int blockIndex = -1;
    int objectIndex = -1;
};


template<class T, int _blockSize>
class PoolBlock
{
public:

    PoolBlock() : mAcquiredCounter(0)
    {

    }


    bool isFree(){ return mAcquiredCounter==0; }
    bool isFull(){ return mAcquiredCounter==mObjects.size()-1; }

    int size(){ return mObjects.size(); }
    int acquiredCounter(){ return mAcquiredCounter; }


    T* acquireObject()
    {
        for(unsigned int i=0; i<mObjects.size(); i++){
            if(mObjects[i].poolPos().objectIndex==-1){
                mObjects[i].poolPos().objectIndex = i;
                mAcquiredCounter++;
                assert(mAcquiredCounter<mObjects.size());
                return &mObjects[i];
            }
        }

        return nullptr;
    }


    void releaseObject(T* _object)
    {

        int objectIndex = _object->poolPos().objectIndex;
        assert(objectIndex>=0 && objectIndex<mObjects.size());
        assert(_object == &mObjects[objectIndex]);

        _object->poolPos().objectIndex = -1;
        mAcquiredCounter--;
    }


private:
    int mAcquiredCounter = 0;
    std::array<T, _blockSize>mObjects;      // OWNED  (stored as pointers to avoid unvalidation problems when linking objects as references)

};




template<class T, int _blockSize>
class PoolStorage
{
public:


    PoolStorage()
    {
        mBlocks.push_back(new PoolBlock<T,_blockSize>());
    }



    ~PoolStorage()
    {
        for(PoolBlock<T,_blockSize>* b : mBlocks){
            delete b;
        }
    }


    T* acquireObject()
    {

        int blockIndex = -1;

        for(unsigned int i=0; i<mBlocks.size(); i++){
            PoolBlock<T,_blockSize>* block = mBlocks[i];
            if(block->isFull()==false){
                blockIndex = i;
                break;
            }
        }

        if(blockIndex==-1){
            mBlocks.push_back(new PoolBlock<T,_blockSize>());
            blockIndex = mBlocks.size()-1;
        }

        T* object = mBlocks[blockIndex]->acquireObject();
        object->poolPos().blockIndex = blockIndex;

        assert(object);

        return object;
    }


    void releaseObject(T* _object)
    {

        int blockIndex = _object->poolPos().blockIndex;
        assert(blockIndex>=0 && blockIndex<mBlocks.size());

        mBlocks[blockIndex]->releaseObject(_object);
        _object->poolPos().blockIndex = -1;
    }



private:
    std::vector<PoolBlock<T,_blockSize>*>mBlocks;

};


//======================================================================================================



template<class T, int _blockSize>
class PoolBlock<T*, _blockSize>
{
public:


    PoolBlock() : mAcquiredCounter(0)
    {
        for(unsigned int i=0; i<mObjects.size(); i++){
            mObjects[i] = nullptr;
        }

    }


    ~PoolBlock()
    {
        for(T* obj : mObjects){
            if(obj){
                delete obj;
            }
        }
    }


    bool isFree(){ return mAcquiredCounter==0; }
    bool isFull(){ return mAcquiredCounter==mObjects.size()-1; }

    int size(){ return mObjects.size(); }
    int acquiredCounter(){ return mAcquiredCounter; }


    T* acquireObject()
    {
        for(unsigned int i=0; i<mObjects.size(); i++){
            if(mObjects[i]==nullptr){
                mObjects[i] = new T();
            }
            if(mObjects[i]->poolPos().objectIndex==-1){
                mObjects[i]->poolPos().objectIndex = i;
                mAcquiredCounter++;
                assert(mAcquiredCounter<mObjects.size());
                return mObjects[i];
            }
        }

        return nullptr;
    }


    void releaseObject(T* _object)
    {

        int objectIndex = _object->poolPos().objectIndex;
        assert(objectIndex>=0 && objectIndex<mObjects.size());
        assert(_object == mObjects[objectIndex]);

        _object->poolPos().objectIndex = -1;
        mAcquiredCounter--;
    }


private:
    int mAcquiredCounter = 0;
    std::array<T*, _blockSize>mObjects{nullptr};      // OWNED  (stored as pointers to avoid unvalidation problems when linking objects as references)

};


template<class T, int _blockSize>
class PoolStorage<T*, _blockSize>
{
public:


    PoolStorage()
    {
        mBlocks.push_back(new PoolBlock<T*,_blockSize>());
    }


    PoolStorage(const PoolStorage<T*, _blockSize> & _src)
    {
        mBlocks.push_back(new PoolBlock<T*,_blockSize>());
    }


    ~PoolStorage()
    {
        for(PoolBlock<T*,_blockSize>* b : mBlocks){
            delete b;
        }

    }


    T* acquireObject()
    {

        int blockIndex = -1;

        for(unsigned int i=0; i<mBlocks.size(); i++){
            PoolBlock<T*,_blockSize>* block = mBlocks[i];
            if(block->isFull()==false){
                blockIndex = i;
                break;
            }
        }

        if(blockIndex==-1){
            mBlocks.push_back(new PoolBlock<T*,_blockSize>());
            blockIndex = mBlocks.size()-1;
        }

        T* object = mBlocks[blockIndex]->acquireObject();
        object->poolPos().blockIndex = blockIndex;

        assert(object);

        return object;
    }

    void releaseObject(T* _object)
    {

        int blockIndex = _object->poolPos().blockIndex;
        assert(blockIndex>=0 && blockIndex<mBlocks.size());

        mBlocks[blockIndex]->releaseObject(_object);
        _object->poolPos().blockIndex = -1;
    }


private:
    std::vector<PoolBlock<T*,_blockSize>*>mBlocks;

};





}

#endif


