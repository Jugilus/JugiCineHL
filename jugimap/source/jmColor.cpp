#include <sstream>
#include <iomanip>
#include "jmCommonFunctions.h"
#include "jmColor.h"



namespace jugimap {




/*
TColorRGBA TColorRGBA::ParseFromHexString(std::string _hexColor)
{

    removeStringWhiteSpacesOnStartAndEnd(_hexColor);

    std::string hexR = "FF";
    std::string hexG = "FF";
    std::string hexB = "FF";
    std::string hexA = "FF";

    if(_hexColor.length()>=6){
        hexR = _hexColor.substr(0,2);
        hexG = _hexColor.substr(2,2);
        hexB = _hexColor.substr(4,2);

    }
    if(_hexColor.length()==8){
        hexA = _hexColor.substr(6,2);

    }

    int r = std::stoi(hexR, nullptr, 16);
    int g = std::stoi(hexG, nullptr, 16);
    int b = std::stoi(hexB, nullptr, 16);
    int a = std::stoi(hexA, nullptr, 16);

    int rgba = r | (g << 8) | (b << 16) | (a << 24);

    return TColorRGBA(rgba);

}
*/




ColorRGBA ColorRGBA::ParseFromHexString(std::string _hexColor)
{

    StdString::removeStringWhiteSpacesOnStartAndEnd(_hexColor);

    if(_hexColor.length()>0){
        if(_hexColor.substr(0,1)=="#"){
            _hexColor = _hexColor.substr(1);
        }
    }

    std::string hexR = "FF";
    std::string hexG = "FF";
    std::string hexB = "FF";
    std::string hexA = "FF";

    if(_hexColor.length()>=6){
        hexR = _hexColor.substr(0,2);
        hexG = _hexColor.substr(2,2);
        hexB = _hexColor.substr(4,2);

    }
    if(_hexColor.length()==8){
        hexA = _hexColor.substr(6,2);

    }

    int r = std::stoi(hexR, nullptr, 16);
    int g = std::stoi(hexG, nullptr, 16);
    int b = std::stoi(hexB, nullptr, 16);
    int a = std::stoi(hexA, nullptr, 16);

    return ColorRGBA(r,g,b,a);

}


std::string ColorRGBA::toHexString(bool includeAlpha)
{

    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2) << int(r) << std::setw(2) << int(g) << std::setw(2) <<int(b);
    if(includeAlpha){
        ss << std::setw(2) << int(a);
    }
    std::string hex(ss.str());

    return hex;
}



bool ColorRGBAf::operator ==(ColorRGBAf _colorRGBA)
{
    return AreEqual(r,_colorRGBA.r) && AreEqual(g,_colorRGBA.g) && AreEqual(b,_colorRGBA.b) && AreEqual(a,_colorRGBA.a);
}


//====================================================================================



/*
Color ColorsLibrary::Find(const std::string &_name)
{

    for(Color *c : mColors){
        if(c->name == _name){
            return c;
        }
    }

    //return ColorRGBA();
    return mDefaultColor->colorRGBA;
}
*/

/*
ColorRGBA ColorsLibrary::At(int _index)
{

    //if(_index>=0 && _index<mColors.size()){
    //    return mColors[_index].second;
    //}

    for(JColor *c : mColors){
        if(c->id == _index){
            return c->colorRGBA;
        }
    }

    return ColorRGBA();
}
*/


void ColorsLibrary::deleteContent()
{
    for(Color *c : mColors){
        delete c;
    }

    mColors.clear();

}

/*
JColor* ColorsLibrary::FindColor(const ColorRGBA &_color)
{

    //if(mDefaultColor && mDefaultColor->colorRGBA==_color){
    //    return mDefaultColor;
    //}

    for(JColor *c : mColors){
        if(c->colorRGBA == _color){
            return c;
        }
    }

    return mDefaultColor;


    //return nullptr;
}
*/



ColorRGBA ColorsLibrary::getColorRGBA(const std::string &_name)
{

    return getColor(_name)->colorRGBA();

}



Color* ColorsLibrary::getColor(const std::string &_name)
{
    for(Color *c : mColors){
        if(c->name() == _name){
            return c;
        }
    }

    return getDefaultColor();
}


Color* ColorsLibrary::getDefaultColor()
{

    assert(mDefaultColor);
    return mDefaultColor;

}



int ColorsLibrary::indexOfColor(const jugimap::Color *_color)
{

    for(int i=0; i<mColors.size(); i++){
        if(mColors[i]==_color){
            return i;
        }
    }
    return 0;
}



ColorsLibrary textColorsLibrary;






}



