#include <string>
#include <sstream>


#ifdef JUGI_EDITOR

#include <QDebug>

#else

#ifdef _WIN32
    #define NOMINMAX
    #include "Windows.h"
    #undef ERROR
    #undef DELETE

#endif

#endif




#include "ncine/SceneNode.h"
#include "jmCommonFunctions.h"



namespace jugimap {



/*
void DbgOutput(const std::string &_text)
{

#ifdef _WIN32

    // this works only for ASCII text !

    std::wstring wsTmp(_text.begin(), _text.end());

    std::wstringstream ss;
    ss << wsTmp << std::endl;
    OutputDebugStringW(ss.str().c_str());

#endif

}
*/


void print(const std::string &text)
{

#ifdef JUGI_EDITOR

    qDebug() << QString::fromStdString(text);

#else

#ifdef _WIN32

    // this works only for ASCII text !

    std::wstring wsTmp(text.begin(), text.end());

    std::wstringstream ss;
    ss << wsTmp << std::endl;
    OutputDebugStringW(ss.str().c_str());

#endif


#endif

}



double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}


int iRand(int iMin, int iMax)
{
    return rand() % (iMax-iMin+1) + iMin;
}


/*
std::string ExtractNameFromPath(std::string path)
{

    size_t indexLastSlash = path.find_last_of("\\/");
    if (std::string::npos != indexLastSlash) path.erase(0, indexLastSlash+1);
    size_t indexPeriod = path.rfind('.');
    if (std::string::npos != indexPeriod) path.erase(indexPeriod);

    return path;
}


std::vector<std::string>SplitString(std::string text, const std::string &delimiter)
{
    std::vector<std::string> splitTexts;

    std::string::size_type pos = 0;
    std::string token;
    while ((pos = text.find(delimiter)) != std::string::npos) {
        splitTexts.push_back(text.substr(0, pos));
        text.erase(0, pos + delimiter.length());
    }
    if(text!="") splitTexts.push_back(text);    //last part

    return splitTexts;
}


void RemoveStringWhiteSpacesOnStartAndEnd(std::string &s)
{

    //s.erase(std::remove_if(s.begin(),s.end(), [](char c){ return (c==' ' || c== '\n' || c=='\r' || c=='\t' || c=='\v' || c== '\f');} ));

    if(s.empty()) return;


    unsigned int index1 = 0 ;
    unsigned index2 = s.length()-1;

    for(unsigned int i=0; i<s.length(); i++){
        char c = s[i];
        if(!(c==' ' || c== '\n' || c=='\r' || c=='\t' || c=='\v' || c== '\f')){
            index1 = i;
            break;
        }
    }

    for(unsigned int i=s.length()-1; i>=0; i--){
        char c = s[i];
        if(!(c==' ' || c== '\n' || c=='\r' || c=='\t' || c=='\v' || c== '\f')){
            index2 = i;
            break;
        }
    }

    if(index1>0 || index2<s.length()-1){
        s = s.substr(index1, index2-index1+1);
    }

}
*/


//================================================================================================================================


AffineMat3f MakeTransformationMatrix(Vec2f pos, Vec2f scale, Vec2i flip, float rotation, Vec2f handle)
{
    AffineMat3f m = MakeTransformationMatrix(pos, scale, flip, rotation);
    m = m.Translate(-handle.x, -handle.y);
    return m;
}


AffineMat3f MakeTransformationMatrix(Vec2f pos, Vec2f scale, Vec2i flip, float rotation)
{
    AffineMat3f m = AffineMat3f::Translation(pos.x, pos.y);
    m = m.Rotate(rotation*mathPI/180.0);
    //float scaleX = scale.x;
    //if(flip.x) scaleX = -scaleX;
    //float scaleY = scale.y;
    //if(flip.y) scaleY = -scaleY;
    //m = m.Scale((flip.x==1)? -scale.x : scale.x, (flip.y==1)? -scale.y : scale.y);
    m = m.Scale(scale.x, scale.y);

    return m;
}


BoundingBoxFloat TransformRectf(const BoundingBoxFloat &rect, const AffineMat3f &matrix)
{

    Vec2f p1 = matrix * Vec2f( rect.min.x, rect.min.y );
    Vec2f p2 = matrix * Vec2f( rect.max.x, rect.min.y );
    Vec2f p3 = matrix * Vec2f( rect.max.x, rect.max.y );
    Vec2f p4 = matrix * Vec2f( rect.min.x, rect.max.y );

    Vec2f pMin(std::min<float>({p1.x,p2.x,p3.x,p4.x}),  std::min<float>({p1.y,p2.y,p3.y,p4.y}));
    Vec2f pMax(std::max<float>({p1.x,p2.x,p3.x,p4.x}),  std::max<float>({p1.y,p2.y,p3.y,p4.y}));

    return BoundingBoxFloat( std::round( pMin.x ), std::round( pMin.y ), std::round( pMax.x ), std::round( pMax.y ));
}


ncine::Matrix4x4f MakeTransformationMatrix(Vec2f pos, Vec2f scale, float rotation, Vec2f handle, Vec2i flip)
{
    ncine::Matrix4x4f m = MakeTransformationMatrix(pos, scale, rotation, flip);
    m.translate(-handle.x, -handle.y, 0.0f);
    return m;
}


ncine::Matrix4x4f MakeTransformationMatrix(Vec2f pos, Vec2f scale, float rotation, Vec2i flip)
{

    ncine::Matrix4x4f m = ncine::Matrix4x4f::translation(pos.x, pos.y, 0.0f);
    m.rotateZ(rotation);
    m.scale((flip.x==1)? -scale.x : scale.x, (flip.y==1)? -scale.y : scale.y, 1.0f);

    return m;

}


BoundingBoxFloat TransformRectf(const BoundingBoxFloat &rect, const ncine::Matrix4x4f &matrix)
{

    Vec2f p1 = transform(Vec2f( rect.min.x, rect.min.y ), matrix);
    Vec2f p2 = transform(Vec2f( rect.max.x, rect.min.y ), matrix);
    Vec2f p3 = transform(Vec2f( rect.max.x, rect.max.y ), matrix);
    Vec2f p4 = transform(Vec2f( rect.min.x, rect.max.y ), matrix);

    Vec2f pMin(std::min<float>({p1.x,p2.x,p3.x,p4.x}),  std::min<float>({p1.y,p2.y,p3.y,p4.y}));
    Vec2f pMax(std::max<float>({p1.x,p2.x,p3.x,p4.x}),  std::max<float>({p1.y,p2.y,p3.y,p4.y}));

    return BoundingBoxFloat( std::round( pMin.x ), std::round( pMin.y ), std::round( pMax.x ), std::round( pMax.y ));
}


bool Point_in_polygon(double tx, double ty, const std::vector<Vec2f> &Poly)
{
    if(Poly.size() < 3) return false;

    unsigned j;
    int yflag0, yflag1, inside_flag;
    double  vtx0, vty0, vtx1, vty1;

    vtx0 = Poly[Poly.size()-1].x;
    vty0 = Poly[Poly.size()-1].y;

    // get test bit for above/below X axis
    yflag0 = (vty0 >= ty);

    vtx1 = Poly[0].x;
    vty1 = Poly[0].y;

    inside_flag = 0;
    for (j = 1; j <= Poly.size(); ++j)
    {
        yflag1 = (vty1 >= ty);

        if (yflag0 != yflag1)
        {

            if ( ((vty1-ty) * (vtx0-vtx1) >=
                  (vtx1-tx) * (vty0-vty1)) == yflag1 )
            {
                inside_flag ^= 1;
            }
        }

        // Move to the next pair of vertices, retaining info as possible.
        yflag0 = yflag1;
        vtx0 = vtx1;
        vty0 = vty1;

        unsigned k = (j >= Poly.size()) ? j - Poly.size() : j;
        vtx1 = Poly[k].x;
        vty1 = Poly[k].y;
    }
    return inside_flag != 0;
}


// STRING
//----------------------------------------------------------------------------------------------


std::string StdString::baseNameFromFilePath(const std::string &path)
{

    size_t indexLastSlash = path.find_last_of("\\/");
    if(indexLastSlash != std::string::npos){
        size_t indexPeriod = path.find(".", indexLastSlash+1);
        if(indexPeriod != std::string::npos){
            int len = indexPeriod - (indexLastSlash+1);
            return path.substr(indexLastSlash+1, len);
        }else{
            return path.substr(indexLastSlash+1);
        }
    }else{
        size_t indexPeriod = path.find(".", indexLastSlash+1);
        if(indexPeriod != std::string::npos){
            return path.substr(0, indexPeriod);
        }
    }
    return path;

}


std::string StdString::fileNameFromFilePath(const std::string &path)
{

    size_t indexLastSlash = path.find_last_of("\\/");

    if (indexLastSlash != std::string::npos){
        return path.substr(indexLastSlash+1);
    }
    return path;
}


std::string StdString::pathFromFilePath(const std::string &path)
{

    size_t indexLastSlash = path.find_last_of("\\/");
    if (indexLastSlash != std::string::npos){
        return path.substr(0,indexLastSlash);
    }
    return path;
}


std::string StdString::suffixFromFilePath(const std::string &path)
{

    size_t indexPeriod = path.find_last_of(".");
    if(indexPeriod != std::string::npos){
        return path.substr(indexPeriod+1);
    }
    return path;

}


std::string StdString::removeSuffixFromFilePath(const std::string &path)
{

    size_t indexLastSlash = path.find_last_of(".");

    if (indexLastSlash != std::string::npos){
        return path.substr(0,indexLastSlash);
    }
    return path;
}


std::vector<std::string> StdString::splitString(std::string text, const std::string &delimiter, bool removeWhiteSpacesOnStartAndEnd)
{

    std::vector<std::string> splitTexts;

    std::string::size_type pos = 0;
    std::string token;
    while ((pos = text.find(delimiter)) != std::string::npos) {
        splitTexts.push_back(text.substr(0, pos));
        text.erase(0, pos + delimiter.length());
    }
    if(text!="") splitTexts.push_back(text);    //last part

    if(removeWhiteSpacesOnStartAndEnd){
        for(std::string &s : splitTexts){
            StdString::removeStringWhiteSpacesOnStartAndEnd(s);
        }
    }

    return splitTexts;
}


std::string StdString::joinStrings(std::vector<std::string> &texts, const std::string &delimiter)
{

    std::string text;

    for(int i=0; i<texts.size(); i++){
        text += texts[i];
        if(i<texts.size()-1){
            text += delimiter;
        }
    }

    return text;
}


void StdString::appendNonEmptyString(std::string &source, const std::string &added, const std::string &delimiter)
{
    if(added.empty()==false){

        if(source.empty()==false){
            source += delimiter + added;
        }else{
            source = added;
        }
    }
}



void StdString::removeStringWhiteSpacesOnStartAndEnd(std::string &s)
{

    //s.erase(std::remove_if(s.begin(),s.end(), [](char c){ return (c==' ' || c== '\n' || c=='\r' || c=='\t' || c=='\v' || c== '\f');} ));

    if(s.empty()) return;


    unsigned int index1 = 0 ;
    unsigned index2 = s.length()-1;
    bool hasWhiteSpace = false;


    for(int i=0; i<s.length(); i++){
        char c = s[i];
        if(!(c==' ' || c== '\n' || c=='\r' || c=='\t' || c=='\v' || c== '\f')){
            index1 = i;
            break;

        }else{
            hasWhiteSpace = true;
        }
    }

    if(index1==0 && hasWhiteSpace){     // the string contains only white spaces
        s = "";
        return;
    }


    for(int i=s.length()-1; i>=0; i--){
        char c = s[i];
        if(!(c==' ' || c== '\n' || c=='\r' || c=='\t' || c=='\v' || c== '\f')){
            index2 = i;
            break;
        }
    }

    if(index1>0 || index2<s.length()-1){
        s = s.substr(index1, index2-index1+1);
    }
}


void StdString::removeEndSlash(std::string &s)
{

    if(s.empty()) return;

    char last = s.back();
    if(last=='\\' || last=='/'){
        s.pop_back();
    }

}



float StdString::stringToFloat(const std::string &_text, float defNumber)
{

    float n = defNumber;

    try{
         n = std::stof(_text);

    } catch( ... )
    {
    }

    return n;
}



int StdString::stringToInt(const std::string &_text, int defNumber)
{

    int n = defNumber;

    try{
         n = std::stoi(_text);

    } catch( ... )
    {
    }

    return n;

}


bool StdString::stringToBool(const std::string &_text)
{

    if(_text=="true" || _text=="1"){
        return true;
    }

    return false;

}



}
