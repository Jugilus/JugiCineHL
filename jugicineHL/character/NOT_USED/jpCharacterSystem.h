#ifndef PLAYER__CHARACTER_SYSTEM_H
#define PLAYER__CHARACTER_SYSTEM_H

#include <assert.h>
#include <string>
#include <vector>

#include "jmSignal.h"
#include "jmStorage.h"

#include "components/jpComponentsCommon.h"

#include "jpQueries.h"


namespace pugi {

class xml_node;

}



namespace jugimap{



class ItemDataAction;
struct SourceItem;







//------------------------------------------------------------------------------------


/*

class CharacterSystemFactory : public CustomComponentFactory
{
public:

    CharacterSystemFactory(const std::string &_typeSignature) : CustomComponentFactory(_typeSignature){}

    Component * createComponent() override { return new CharacterSystem(mTypeSignature); }

};

*/


//------------------------------------------------------------------------------------


class CharacterSystemSignalParser : public SignalParser
{
public:

    CharacterSystemSignalParser(std::vector<std::string> _keyWords) : SignalParser(_keyWords){}

    void parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject* obj1=nullptr, BaseObject* obj2=nullptr) override;

};



}


#endif
