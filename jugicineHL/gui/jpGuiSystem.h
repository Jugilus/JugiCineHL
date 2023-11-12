#ifndef PLAYER__GUI_SYSTEM__H
#define PLAYER__GUI_SYSTEM__H

#include <memory>
#include <string>
#include <vector>
#include "jpGlobal.h"
#include "components/jpComponentsCommon.h"
#include "jpGuiCommon.h"


namespace pugi{
class xml_node;
}

namespace jugimap{


class GuiWidget;
struct GuiWidgetCfg;
class WidgetsManager;
class WidgetFactory;
class WidgetSignalParser;




class GuiSystem : public Component
{
public:
    GuiSystem();
    ~GuiSystem();
    void deleteWidgets();

    bool initCfg(PlayedScene *_scene, const pugi::xml_node &_node) override;
    bool buildObjects(PlayedScene *_scene) override;
    bool initConnections(PlayedScene *_scene) override;


    void start() override;
    void preUpdate(UpdateMode &_updateMode) override;
    void update(UpdateMode &_updateMode) override;
    void postUpdate(UpdateMode &_updateMode) override;
    //void drawShapes(ShapeDrawer &_drawer, int _flags=0) override;
    void onStateEnded() override;


    void setEnabled(bool _enabled){ mEnabled = _enabled; }

    bool isEnabled(){ return mEnabled; }

    void setWidgetsToInitialState();

    void updateWidgets();

    void addWidget(GuiWidget* _widget){ mWidgets.push_back(_widget); }

    GuiWidget* findWidget(const std::string &_name, WidgetType guiWidgetKind, bool _setErrorMessage=true);
    GuiWidget* findWidget(int _tag, WidgetType guiWidgetKind, bool _setErrorMessage=true);

    void appendToUsedWidgets(const std::vector<GuiWidget*> &_widgets);
    void removeFromUsedWidgets(const std::vector<GuiWidget*> &_widgets);

    std::vector<GuiWidget*>& widgets(){ return mWidgets; }
    std::vector<GuiWidget*> &usedWidgets(){ return mUsedWidgets; }

    //---
    void addModalBlockLevelToUsedWidgets();
    void removeModalBlockLevelToUsedWidgets();

    //---
    void resetInput();

    WidgetsManager* widgetsManager(){ return mWidgetsManager.get(); }


private:
    bool mEnabled = true;
    //PlayedScene *mParentPlayerScene= nullptr;               // LINK
    std::vector<GuiWidget*>mWidgets;
    std::vector<GuiWidget*>mUsedWidgets;
    std::unique_ptr<WidgetsManager>mWidgetsManager;

    WidgetSignalParser *mWidgetSignalParser = nullptr;      // LINK (stored in main parser which takes ownership)

};



//------------------------------------------------------------------------------------



class GuiSystemFactory : public CustomComponentFactory
{
public:

    GuiSystemFactory(const std::string &_typeSignature) : CustomComponentFactory(_typeSignature){}

    Component * createComponent() override { return new GuiSystem(); }


};


//------------------------------------------------------------------------------------


class WidgetsManager
{
public:

    ~WidgetsManager();


    void addWidgetFactory(WidgetFactory *_widgetFactory);

    GuiWidgetCfg* addWidgetCfg(const std::string &_typeSignature, const std::string &_name, bool _setErrorMessage=true);
    GuiWidgetCfg* getWidgetCfg(const std::string &_name, bool _setErrorMessage=true);
    //GuiWidget* createWidget(const std::string &_typeSignature, bool _setErrorMessage=true);

    std::vector<GuiWidgetCfg*> & widgetCfgs(){ return mWidgetCfgs; }

private:
    std::vector<WidgetFactory*>mWidgetFactories;
    std::vector<GuiWidgetCfg*>mWidgetCfgs;
};










}






#endif
