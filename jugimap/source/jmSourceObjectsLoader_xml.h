#ifndef JMSOURCEOBJECTSLOADER_XML_H
#define JMSOURCEOBJECTSLOADER_XML_H


#include <string>
#include <vector>
#include "jmGlobal.h"


namespace pugi {
class xml_node;
}


namespace jugimap {


class SourceFile;
class SourceImage;
class SourceSprite;
class CompositeSprite;
class SourceSet;
class SourceGroup;
class SourceLibrary;
class TimelineAnimation;
class AnimationTrack;
class AnimationMember;
class FrameAnimation;
struct CParameter;




class SourceObjectsLoader_xml
{
public:

    void loadSourceGroups();

    bool readAndMakeSubimages(SourceFile *sourceFile);
    bool readSourceImagesExtraData(SourceFile *sourceFile);
    //void setYorientationOverrided(Yorientation _yOrientationOverrided){ yOrientationOverrided = _yOrientationOverrided; }
    void loadGlobalAndSystemSourceObjects();


private:
    void loadGlobalSourceObjects();
    void loadSourceGroup(SourceGroup *sourceGroup);
    bool loadSourceSet(SourceSet *sourceSet);
    bool loadSourceSet_images(SourceSet *sourceSet, pugi::xml_node &node);
    bool loadSourceSet_sprites(SourceSet *sourceSet, pugi::xml_node &node);
    bool loadSourceSet_texts(SourceSet *sourceSet, pugi::xml_node &node);
    bool loadSourceCompositeSprite(CompositeSprite *cs, const std::string &filePath);
    bool loadTimelineAnimation(TimelineAnimation *ta, pugi::xml_node &node);
    AnimationTrack* readKeyAnimation(TimelineAnimation *ta, AnimationMember *mas, pugi::xml_node &node);
    bool loadFrameAnimation(FrameAnimation *fa, pugi::xml_node &node);
    void readParameter(CParameter &pv, pugi::xml_node &node);

    void OrderSets(SourceGroup *sourceGroup);
    std::string rootNodeName(const std::string &filePath);



//#ifdef JUGI_EDITOR
    void finalizeSourceCompositeSprites(SourceGroup *sourceGroup, std::vector<CompositeSprite*> &_sourceCompositeSprites);
//#else
//    void finalizeComposedSprites_ReadFromCfgFiles(SourceGroup *sourceGroup, std::vector<CompositeSprite*> &ComposedSprites);
//#endif


    Yorientation yOrientation = Yorientation::DOWN;
    //Yorientation yOrientationOverrided = Yorientation::NOT_DEFINED;

    //bool systemSourceObjectsCreated = false;
    //bool globalSourceObjectsCreated = false;

};








//extern SourceObjectsLoader_xml SourceObjectsLoader;
extern SourceObjectsLoader_xml sourceObjectsLoader_xml;




}













#endif // JMSOURCEOBJECTSLOADER_XML_H
