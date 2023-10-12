#include <string>
#include <sstream>
//#include "jmObjectFactory.h"
//#include "jmSourceSprite.h"
#include "jmFont.h"
#include "jmColor.h"
//#include "jmInput.h"
#include "jmSystem.h"

#ifdef JUGI_EDITOR
#include "../jugi/globals.h"
#endif



namespace jugimap {




TEProjectInfo CurrentProject;


int Time::GetPassedNetTimeMS()
{
#ifdef JUGI_EDITOR
    return jugi::glob::TimerGlobal.elapsed();
#else
    //return passedMicroseconds/1000.0 - suspendedTimeMS;
    //return passedMicroseconds;      // actualy this are miliseconds with timeStep method
    return passedTimeS * 1000.0;      // actualy this are miliseconds with timeStep method

#endif

}


//int Time::GetPassedNetTimeS()
//{
//    return GetPassedNetTimeMS()/1000;
//}



Settings settings;
Time time;


//-------------------------------------------------------------------------------------------------



void DbgSystem::addMessage(const std::string &_message)
{
    if(mActive==false) return;

    mErrorMessages.emplace_back(_message);
}



void DbgSystem::removeLastMessage()
{
    if(mActive==false) return;

    assert(mErrorMessages.empty()==false);

    mErrorMessages.pop_back();

}





//--------------------------------------------------------------------------------------------------

jugimap::BoundingBoxFloat Y_Up_to_Y_Down::convertRect(jugimap::BoundingBoxFloat r)
{
    return BoundingBoxFloat({r.min.x, settings.GetScreenSize().y -r.max.y},
                 {r.max.x, settings.GetScreenSize().y-r.min.y});
}


jugimap::BoundingBoxInt Y_Up_to_Y_Down::convertRect(jugimap::BoundingBoxInt r)
{
    return BoundingBoxInt({r.min.x, settings.GetScreenSize().y -r.max.y},
                 {r.max.x, settings.GetScreenSize().y-r.min.y});
}


jugimap::BoundingBoxFloat Y_Up_to_Y_Down::convertRect(ncine::Rectf r)
{
    return BoundingBoxFloat({r.x, settings.GetScreenSize().y - (r.y + r.h)},
                 {r.x+r.w, settings.GetScreenSize().y -r.y});
}


jugimap::BoundingBoxInt Y_Up_to_Y_Down::convertRect(ncine::Recti r)
{
    return BoundingBoxInt({r.x, settings.GetScreenSize().y - (r.y + r.h)},
                 {r.x+r.w, settings.GetScreenSize().y -r.y});
}



/*
void DeleteGlobalObjects()
{

    //SourceGraphics::deleteGlobalData();
    //delete dummyTextSourceSprite;
    //if(objectFactory) delete objectFactory;
    fontsLibrary.deleteContent();
    textColorsLibrary.deleteContent();



}
*/

DbgSystem dbgSystem;


}
