#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <array>
#include <fstream>
#include <string>
#include "utf8cpp/utf8.h"
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmInput.h"
#include "jmStreams.h"
#include "jmUtilities.h"
#include "jmVectorShape.h"
#include "jmTextSprite.h"
#include "jmFont.h"
#include "jmColor.h"
#include "jmMap.h"
#include "jmGuiBar.h"
#include "jmGuiText.h"


namespace jugimap{





UnicodeString::UnicodeString(const std::string &_s)
{
    Set(_s);
}


void UnicodeString::Set(const std::string &_s)
{

    codes.clear();
    Append(_s);

}


void UnicodeString::Append(const std::string &_s)
{

    int count = 0;
    char *i = (char*)_s.data();
    char *iEnd = (char*)_s.data() + _s.size();
    while(i<iEnd){
        int unicode = utf8::next(i, iEnd);
        codes.push_back(unicode);
        //qDebug()<<hex<<count<<" : "<< unicode;
        count++;
    }
}


void UnicodeString::Append(const UnicodeString &_us)
{

    for(int c : _us.codes){
        codes.push_back(c);
    }
}


void UnicodeString::Append(int _code)
{
    codes.push_back(_code);
}


void UnicodeString::Clear()
{
    codes.clear();
}


std::string UnicodeString::ToStdString()
{

    std::string s;

    for(int c : codes){
        utf8::append(c, s);
    }

    return s;
}


std::string UnicodeString::ToStdString(int _startIndex, int _endIndex)
{

    std::string s;

    if(codes.empty()) return s;
    if(_startIndex>=codes.size()) return s;

    if(_startIndex < 0) _startIndex = 0;

    if(_endIndex==-1 || _endIndex>=codes.size()){
        _endIndex = codes.size()-1;
    }
    if(_endIndex<_startIndex) _endIndex = _startIndex;


    for(int i=_startIndex; i<=_endIndex; i++){
        utf8::append(codes[i], s);
    }

    return s;
}




void VerifyAndFixUtf8string(std::string& str)
{
    std::string temp;
    utf8::replace_invalid(str.begin(), str.end(), back_inserter(temp));
    str = temp;
}


std::string GetSubstring(const std::string &text, int indexStart, int indexEnd, bool &endReached)
{

    std::string s;
    endReached = true;

    int count = 0;
    char *i = (char*)text.data();
    char *iEnd = (char*)text.data() + text.size();


    while(i<iEnd){
        int unicode = utf8::next(i, iEnd);

        if(count<indexStart){
            // empty;

        }else if(count>indexEnd){
            endReached = false;
            break;

        }else{
            utf8::append(unicode, s);
        }

        //qDebug()<<hex<<count<<" : "<< unicode;
        count++;
    }

    return s;
}


void GetStringUnicodes(const std::string &text, std::vector<int>&unicodes)
{

    int count = 0;
    char *i = (char*)text.data();
    char *iEnd = (char*)text.data() + text.size();
    while(i<iEnd){
        int unicode = utf8::next(i, iEnd);
        unicodes.push_back(unicode);
        //qDebug()<<hex<<count<<" : "<< unicode;
        count++;
    }

}


int GetCharacterUnicode(const std::string &character)
{

    int unicode = 0;
    char *i = (char*)character.data();
    char *iEnd = (char*)character.data() + character.size();
    if(i<iEnd){
        unicode = utf8::next(i, iEnd);
    }

    return unicode;

}


std::string GetUnicodeCharacter(int unicode)
{
    std::string letter;
    utf8::append(unicode, letter);

    return letter;
}



int GetStringNumberOfCharacters(const std::string &text)
{

    int count = 0;
    char *i = (char*)text.data();
    char *iEnd = (char*)text.data() + text.size();
    while(i<iEnd){
        int unicode = utf8::next(i, iEnd);
        //qDebug()<<hex<<count<<" : "<< unicode;
        count++;
    }

    return count;

}


//========================================================================================


TextBook::~TextBook()
{
    for(TextSegment *ts : mTextSegments){
        if(ts){
            delete ts;
        }
    }
    for(TextSequence *ts : mTextSequences){
        if(ts){
            delete ts;
        }
    }
}


bool TextBook::loadExternalData()
{

    if(mExternalDataLoaded) return true;


    bool loaded = loadContent(textColorsLibrary, fontsLibrary);

    if(loaded){
        mExternalDataLoaded = true;
    }

    return loaded;
}


void TextBook::deleteExternalData()
{

    for(TextSegment *ts : mTextSegments){
        if(ts){
            delete ts;
        }
    }
    mTextSegments.clear();

    for(TextSequence *ts : mTextSequences){
        if(ts){
            delete ts;
        }
    }
    mTextSequences.clear();

    //----
    mExternalDataLoaded = false;

}


bool TextBook::addTextSegment(TextSegment *_textSegment)
{

    if(_textSegment->GetId() >= mTextSegments.size()){
        mTextSegments.resize(_textSegment->GetId()+1, nullptr);
    }

    if(mTextSegments[_textSegment->GetId()]==nullptr){
        mTextSegments[_textSegment->GetId()] = _textSegment;
        return true;
    }

    return false;
}


bool TextBook::addTextSequence(TextSequence *_textSequence)
{

    if(_textSequence->GetId() >= mTextSequences.size()){
        mTextSequences.resize(_textSequence->GetId()+1, nullptr);
    }

    if(mTextSequences[_textSequence->GetId()]==nullptr){
        mTextSequences[_textSequence->GetId()] = _textSequence;
        return true;
    }

    return false;
}


bool TextBook::loadContent(ColorsLibrary &_colors, FontsLibrary &_fonts)
{

    TextLibrary &dbgTextLibrary = textLibrary;

    std::string filePath;
    if(textLibrary.relativeTextsPath().empty()==false && textLibrary.activeLanguage().empty()==false){
        std::string fileName = StdString::fileNameFromFilePath(mRelativeFilePath);
        filePath = CurrentProject.gameRootDir + "/" + textLibrary.relativeTextsPath() + "/" + textLibrary.activeLanguage() + "/" + fileName;
    }else{
        filePath = CurrentProject.gameRootDir + "/" + mRelativeFilePath;
    }


    //assert(_colors.empty()==false);
    assert(_fonts.fonts().size()>0);


    TextSegment::defaultColor = _colors.getDefaultColor()->colorRGBA();
    TextSegment::defaultFont = _fonts.GetSystemFont();

    TextStreamReader *file = new StdTextFileStreamReader(filePath);
    TextStreamReader &File = *file;

    if(File.IsOpen()==false){
        return false;
    }


    //----
    std::string text;
    TextSegment *segment = nullptr;
    TextSequence *sequence = nullptr;
    CParameters parameters;


    while(File.Eof()==false){

        File.ReadLine(text);

        // remove REM parts
        std::size_t pos = text.find("//");
        if(pos!=std::string::npos){
            text = text.substr(0, pos);
        }

        if(text==""){
            continue;                                                                   // skip empty row
        }


        if(text.substr(0,14)==">buttonColors:"){
            std::string value = text.substr(text.find(":")+1);
            StdString::removeStringWhiteSpacesOnStartAndEnd(value);
            std::vector<std::string>vValues = StdString::splitString(value, ",");
            if(vValues.size()==2){

                if(vValues[0].empty()==false){
                    if(vValues[0].front()=='#'){
                        TextSegment::currentButtonColor = ColorRGBA::ParseFromHexString(vValues[0].substr(1));
                    }else{
                        TextSegment::currentButtonColor = _colors.getColorRGBA(vValues[0]);
                    }
                }

                if(vValues[1].empty()==false){
                    if(vValues[1].front()=='#'){
                        TextSegment::currentButtonColor_CO = ColorRGBA::ParseFromHexString(vValues[1].substr(1));
                    }else{
                        TextSegment::currentButtonColor = _colors.getColorRGBA(vValues[0]);
                    }
                }
            }


        //------
        }else if(text.substr(0,10)==">defColor:"){
            std::string value = text.substr(text.find(":")+1);
            StdString::removeStringWhiteSpacesOnStartAndEnd(value);

            if(value.empty()==false){
                if(value.front()=='#'){
                    value = value.substr(1);
                    TextSegment::defaultColor = ColorRGBA::ParseFromHexString(value);

                }else{
                    TextSegment::defaultColor = _colors.getColorRGBA(value);
                }
            }


        //------
        }else if(text.substr(0,9)==">defFont:"){
            std::string value = text.substr(text.find(":")+1);
            StdString::removeStringWhiteSpacesOnStartAndEnd(value);
            TextSegment::defaultFont = _fonts.getFont(value);

        //------
        }else if(text.substr(0,11)==">defIndent:"){
            std::string value = text.substr(text.find(":")+1);
            StdString::removeStringWhiteSpacesOnStartAndEnd(value);
            TextSegment::defaultIndent = StdString::stringToInt(value, 0);

        //------
        }else if(text.substr(0,15)==">defTabSpacing:"){
            std::string value = text.substr(text.find(":")+1);
            StdString::removeStringWhiteSpacesOnStartAndEnd(value);
            TextSegment::defaultTabSpacing = StdString::stringToInt(value, 0);

        //------
        }else if(text.substr(0,4)==">id:"){                                     // new segment

            //assert(segment==nullptr);

            //---- store possible current segment
            if(segment){
                if(segment->GetWords().empty()){
                    delete segment;

                }else{

                    if(sequence){
                        sequence->GetTextSegments().push_back(segment);

                    }else if(addTextSegment(segment)==false){                   // Store text segment in book (which also take over ownership)!
                        delete segment;
                    }
                }
                segment = nullptr;
            }

            //---- store current sequence if active
            if(sequence){
                if(addTextSequence(sequence)==false){                           // Store text segment in book (which also take over ownership)!
                    delete sequence;
                }
                sequence = nullptr;
            }

            //--- parameters
            parameters.clear();
            _parseParametersIfDefined(text, parameters);

            StdString::removeStringWhiteSpacesOnStartAndEnd(text);

            //---
            //int id = std::stoi(text.substr(text.find(":")+1));
            int id = StdString::stringToInt(text.substr(text.find(":")+1), 0);
            segment = new TextSegment(id);

            //---
            continue;       // goto next line


        //------
        }else if(text.substr(0,7)==">seqId:"){

            //---- store possible current segment
            if(segment){
                if(segment->GetWords().empty()){
                    delete segment;

                }else{

                    if(sequence){
                        sequence->GetTextSegments().push_back(segment);

                    }else if(addTextSegment(segment)==false){           // Store text segment in book (which also take over ownership)!
                        delete segment;
                    }
                }
                segment = nullptr;
            }

            //---- store current sequence if exists
            if(sequence){
                if(addTextSequence(sequence)==false){                   // Store text segment in book (which also take over ownership)!
                    delete sequence;
                }
                sequence = nullptr;
            }


            //--- parameters
            parameters.clear();
            _parseParametersIfDefined(text, parameters);

            StdString::removeStringWhiteSpacesOnStartAndEnd(text);

            //int id = std::stoi(text.substr(text.find(":")+1));
            int id = StdString::stringToInt(text.substr(text.find(":")+1), 0);
            sequence = new TextSequence(id);
            sequence->GetParameters() = parameters;

            //---
            continue;       // goto next line


        //------
        }else if(text.substr(0,4)==">add"){

            //assert(segment==nullptr);       // this should never happen as segment get stored at ">seqId:" line
            assert(sequence);               // this should never happen as sequence get created at ">seqId:" line

            //---- store possible current segment
            if(segment){
                sequence->GetTextSegments().push_back(segment);
                segment = nullptr;
            }


            // adding segments to sequence
            int id = sequence->GetTextSegments().size();        // next index
            segment = new TextSegment(id);

            //--- parameters
            _parseParametersIfDefined(text, segment->parameters());

            //---
            continue;       // goto next line


        }else{

            if(segment){
                segment->AddParagraph(text, _colors, _fonts);
            }
        }

    }



    //---- store last segment
    if(segment){
        if(segment->GetWords().empty()){
            delete segment;

        }else{

            if(sequence){
                sequence->GetTextSegments().push_back(segment);

            }else if(addTextSegment(segment)==false){                           // Store text segment in book (which also take over ownership)!
                delete segment;
            }
        }
        segment = nullptr;
    }

    // store last sequence
    if(sequence){
       if(addTextSequence(sequence)==false){
           delete sequence;
       }
       sequence = nullptr;
    }


    File.Close();
    delete file;


    return true;

}


void TextBook::_parseParametersIfDefined(std::string &text, CParameters &_parameters)
{


    std::size_t pos1 = text.find("[");
    std::size_t pos2 = text.find("]");

    if(pos1 != std::string::npos && pos2 != std::string::npos ){
        std::string parameters = text.substr(pos1+1, pos2-pos1-1);
        std::vector<std::string>vParameters = StdString::splitString(parameters, ",");
        for(std::string &s : vParameters){
            //_parameters.push_back(Parameter::Parse(s));
            _parameters.push_back(CParameter(s));
        }
    }

    //---- remove parameters part from text til the end!
    if(pos1 != std::string::npos){
        text = text.substr(0, pos1);
    }

}



//========================================================================================



void TextSegmentWord::UpdateSize(bool _forceUpdate)
{

    if(word=="<<NL") return;
    if(content) return;

    assert(font);

    if(width<0 || _forceUpdate){
        width = font->GetPixelWidth(word);
    }
    if(height<0 || _forceUpdate){
        height = font->GetCommonPixelHeight() * fFontHeight;
    }


}


//========================================================================================

ColorRGBA TextSegment::defaultColor;
Font* TextSegment::defaultFont = nullptr;
ColorRGBA TextSegment::currentColor;
Font* TextSegment::currentFont;
std::string TextSegment::currentButton;
ColorRGBA TextSegment::currentButtonColor;
ColorRGBA TextSegment::currentButtonColor_CO{255,0,0};
//int TextSegment::currentOffset = 0;
int TextSegment::defaultIndent = 0;
int TextSegment::defaultTabSpacing = 30;
int TextSegment::currentIndent = 0;
int TextSegment::currentTabSpacing = 30;


TextSegment::~TextSegment()
{
    for(TextSegmentWord *tw : mWords){
        delete tw;
    }
}



void TextSegment::AddParagraph(const std::string &_text, ColorsLibrary &_colors, FontsLibrary &_fonts)
{

    assert(_fonts.fonts().size()>0);


    std::string text = _text;


    VerifyAndFixUtf8string(text);
    std::vector<int>unicodes;
    GetStringUnicodes(text, unicodes);

    if(unicodes.empty()) return;


    //currentColor = defaultColor;                    // index 0
    //currentFont = defaultFont;                          // index 0
    //currentIndent = defaultIndent;
    //currentTabSpacing = defaultTabSpacing;




    TextSegmentWord * word = nullptr;


    //--- If text already exist, the new one will be added is added in new line
    if(mWords.empty()==false){
        word = new TextSegmentWord();
        word->word = "<<NL";            // nw line keyword !
        mWords.push_back(word);
        word = nullptr;
        //word = new TTextWord();
    }

    if(currentIndent > 0){

        word = new TextSegmentWord();
        word->width = currentIndent;
        word->height = 0;
        word->content = TextSegmentWord::INDENT;
        mWords.push_back(word);
        word = nullptr;
    }


    for(int i=0; i<unicodes.size(); i++){

        int code = unicodes[i];

        std::string letter;
        utf8::append(code, letter);

        bool whiteSpace = (letter.length()==1 && isWhiteSpaceCharacter(letter[0]));

        //if(letter != " "){
        if(whiteSpace==false){

            if(word==nullptr){
                word = new TextSegmentWord();
            }

            word->word += letter;
            word->nCharacters++;

        //--- space
        }else if(letter[0]==' '){

            if(word){
                ProcessAddingWord(word, _colors, _fonts);
                word = nullptr;
            }


        //--- tab
        }else if(letter[0]=='\t'){

            word = new TextSegmentWord();
            word->width = currentTabSpacing;
            word->height = 0;
            word->content = TextSegmentWord::TAB_SPACING;
            mWords.push_back(word);
            word = nullptr;

        }

       // last character -> add last word if it's not empty.
        if(i == unicodes.size()-1){

            /*
            if(word->word != ""){
                word->color = currentColor;     // !
                word->font = currentFont;       // !
                words.push_back(word);
            }else{
                delete word;
            }
            word = nullptr;
            */

            if(word){
                ProcessAddingWord(word, _colors, _fonts);
                word = nullptr;
            }

            //word = ProcessAddingWord(word, colors, fonts);

            // if word was not added to words, delete it (it was
            //if(words.empty()==false && word!= words.back()){
            //    delete word;
            //}
        }
    }

    assert(word==nullptr);
}



void TextSegment::ProcessAddingWord(TextSegmentWord *word, ColorsLibrary &colors, FontsLibrary &fonts)
{


    if(word->word ==""){                        // Extra spaces are not registered!



    //--- KEYS

    //--- COLOR
    }else if(word->word.substr(0,7) == "<color:" && word->word.substr(word->nCharacters-1)==">"){        // keyword for color

        std::string value = word->word.substr(7, word->nCharacters-1-7);
        if(value.empty()==false){
            if(value.front()=='#'){
                value = value.substr(1);
                currentColor = ColorRGBA::ParseFromHexString(value);

            }else{
                currentColor = colors.getColorRGBA(value);
            }
        }

    }else if(word->word == "<defColor>"){        // keyword for defaultFont
        currentColor = defaultColor;


    //--- FONT
    }else if(word->word.substr(0,6) == "<font:" && word->word.substr(word->nCharacters-1)==">"){        // keyword for font

        std::string fontName = word->word.substr(6, word->nCharacters-1-6);
        currentFont = fonts.getFont(fontName);


    }else if(word->word == "<defFont>"){        // keyword for defaultFont
        currentFont = defaultFont;


    //--- BUTTON
    }else if(word->word.substr(0,8) == "<button:" && word->word.substr(word->nCharacters-1)==">"){        // keyword for button

        currentButton = word->word.substr(8, word->nCharacters-1-8);

    }else if(word->word == "</button>"){        // keyword for button end

        currentButton = "";


    //--- OFFSET
    }else if(word->word.substr(0,8) == "<offset:" && word->word.substr(word->nCharacters-1)==">"){      // horizontal spacing

        std::string value = word->word.substr(8, word->nCharacters-1-8);
        //word->width = std::stof(value);
        word->width = StdString::stringToFloat(value, 0);
        word->height = 0;
        word->content = TextSegmentWord::OFFSET;
        mWords.push_back(word);


    //--- SPACING
    }else if(word->word.substr(0,12) == "<verSpacing:" && word->word.substr(word->nCharacters-1)==">"){      // vertical spacing

        std::string spacing = word->word.substr(12, word->nCharacters-1-12);
        //word->height = std::stof(spacing);
        word->height = StdString::stringToFloat(spacing, 0);
        word->width = 0;
        word->content = TextSegmentWord::VER_SPACING;
        mWords.push_back(word);


    //--- TAB
    }else if(word->word.substr(0,12) == "<tabSpacing:" && word->word.substr(word->nCharacters-1)==">"){      // horizontal spacing

        std::string value = word->word.substr(12, word->nCharacters-1-12);
        //currentTabSpacing = std::stof(value);
        currentTabSpacing = StdString::stringToFloat(value, 0);

    }else if(word->word == "<defTabSpacing>"){      // horizontal spacing
        currentTabSpacing = defaultTabSpacing;


    //--- PARAGRAPH INDENT
    }else if(word->word.substr(0,8) == "<indent:" && word->word.substr(word->nCharacters-1)==">"){
        std::string value = word->word.substr(8, word->nCharacters-1-8);
        //currentIndent = std::stof(value);
        currentIndent = StdString::stringToFloat(value, 0);

    }else if(word->word == "<defIndent>"){
        currentIndent = defaultIndent;




    //--- TEXT ------------------------------------------------------------
    }else{

        word->word += " ";                       // del vsake besede je razmak do naslednje
        word->nCharacters++;
        word->color = currentColor;
        word->font = currentFont;
        word->button = currentButton;
        if(word->button != ""){
            word->color = currentButtonColor;
            word->buttonColor_CO = currentButtonColor_CO;
        }
        //word->buttonColor = currentButtonColor;

        mWords.push_back(word);
    }


    //--- If the word was not stored, delete it !
    if(mWords.empty() || mWords.back()!=word){
        delete word;
    }

}



bool TextSegment::Contains(const std::string &_word, bool compareWithOriginalWord)
{

    std::string word = _word + " ";

    for(TextSegmentWord *tw : mWords){

        if(compareWithOriginalWord && tw->originalWord.length()>0){
            if(tw->originalWord==word){
                return true;
            }

        }else if(tw->word==word){
            return true;
        }
    }

    return false;
}


void TextSegment::SetWordSizesIfNeeded()
{
    for(TextSegmentWord *tw : mWords){
        tw->UpdateSize();
    }
}


std::string TextSegment::plainText()
{

    std::string text;
    for(TextSegmentWord *tw : mWords){
        text += tw->word;
    }

    return text;
}


bool TextSegment::Replace(const std::string &_searchedWord, const std::string &newWord)
{

    bool found = false;

    std::string searchedWord = _searchedWord + " ";

    for(TextSegmentWord* w : mWords){

        std::string s = (w->originalWord!="")? w->originalWord : w->word;
        if(s==searchedWord){
            if(w->originalWord==""){
                w->originalWord = w->word;
            }

            w->word = newWord + " ";
            w->nCharacters = GetStringNumberOfCharacters(w->word);
            w->UpdateSize(true);
            found = true;
        }
    }

    return found;

}



//========================================================================================



TextSequence::~TextSequence()
{

    for(TextSegment *ts : TextSegments){
        if(ts){
            delete ts;
        }
    }

}



//========================================================================================================================


//std::string TextLibrary::path;
//std::string TextLibrary::pathPrefix;


TextLibrary::~TextLibrary()
{

    for(TextBook* b : mBooks){
        delete b;
    }

}


TextBook* TextLibrary::CreateNewBook(const std::string  &_bookName)
{

    for(TextBook* b : mBooks){
        if(b->name()==_bookName){
            return b;
        }
    }

    TextBook *book = new TextBook(_bookName);
    mBooks.push_back(book);

    return book;

}


TextBook* TextLibrary::FindBook(const std::string &_bookName)
{

    for(TextBook* b : mBooks){
        if(b->name()==_bookName){
            return b;
        }
    }

    return nullptr;
}


TextSegment* TextLibrary::GetTextSegmentFromBook(const std::string &_bookName, int _segmentID)
{

    TextBook* book = FindBook(_bookName);

    if(book){
        if(book->isExternalDataLoaded()==false){
            book->loadExternalData();
        }
        if(_segmentID>=0 && _segmentID < book->textSegments().size()){
            return book->textSegments()[_segmentID];
        }
    }

    return nullptr;
}


bool TextLibrary::loadExternalData()
{

    dbgSystem.addMessage("Loading text library external data...");

    for(TextBook* b : mBooks){
        if(b->loadExternalData()==false){
            return false;
        }
    }

    dbgSystem.removeLastMessage();
    return true;

}



TextLibrary textLibrary;
	



}




