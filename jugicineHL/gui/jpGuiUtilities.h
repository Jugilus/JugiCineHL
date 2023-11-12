#ifndef PLAYER__GUI_UTILITIES__H
#define PLAYER__GUI_UTILITIES__H

#include <memory>
#include <string>
#include <vector>
#include "jpGlobal.h"
#include "jpQueries.h"

//#include "jpGuiCommon.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class GuiWidget;
struct GuiWidgetCfg;
class WidgetsManager;
class WidgetFactory;






class WidgetSignalParser : public SignalParser
{
public:

    WidgetSignalParser(std::vector<std::string> _keyWords) : SignalParser(_keyWords){}

    void parseSignalAccessor(PlayedScene *scene, const std::string &_path, SignalAccessor &_signalAccessor, BaseObject* obj1=nullptr, BaseObject* obj2=nullptr) override;

private:

};




}






#endif
