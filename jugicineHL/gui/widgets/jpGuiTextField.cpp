#include <assert.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <array>
#include <fstream>
#include <string>
#include "pugixml/pugixml.hpp"
#include "jmSystem.h"
#include "jmCommonFunctions.h"
#include "jmInput.h"
#include "jmStreams.h"
#include "jmFont.h"
#include "jmUtilities.h"
#include "jmVectorShape.h"
#include "jmTextSprite.h"
#include "jmCompositeSprite.h"
#include "jmStandardSprite.h"
#include "jmSourceSprite.h"
#include "jmMap.h"
#include "jmSpriteLayer.h"
#include "jmVectorLayer.h"
#include "jmLayer.h"
#include "jmText.h"

#include "jpGuiBar.h"
#include "jpGuiButton.h"
#include "jpGuiSlider.h"
#include "jpGuiSystem.h"
#include "jpGuiTextField.h"


namespace jugimap{






bool TextFieldCfg::initCfg(const pugi::xml_node &_node)
{

    dbgSystem.addMessage("Loading widget of type '" + factory->typeSignature() + "' and name '" + name + "'...");


    for(pugi::xml_node n = _node.first_child(); n; n = n.next_sibling()){
        std::string nodeName = std::string(n.name());

        if(nodeName=="parameters"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="textAlignment"){
                    std::string sTextAlignment = a.as_string();

                    if(sTextAlignment=="LEFT"){
                        textAlignment = TextAlignment::LEFT;

                    }else if(sTextAlignment=="JUSTIFY"){
                        textAlignment = TextAlignment::JUSTIFY;

                    }else{
                        dbgSystem.addMessage("Unknown text alignement '" + sTextAlignment + "' !");
                        return false;
                    }

                }else if(attributeName=="textDrawingDelay"){
                    std::string sTextDrawingDelay = a.as_string();

                    if(sTextDrawingDelay=="NONE"){
                        textDrawingDelay = TextDrawingDelay::NONE;

                    }else if(sTextDrawingDelay=="BY_ROWS"){
                        textDrawingDelay = TextDrawingDelay::BY_ROWS;

                    }else if(sTextDrawingDelay=="BY_WORDS"){
                        textDrawingDelay = TextDrawingDelay::BY_WORDS;

                    }else if(sTextDrawingDelay=="BY_CHARACTERS"){
                        textDrawingDelay = TextDrawingDelay::BY_CHARACTERS;

                    }else{
                        dbgSystem.addMessage("Unknown text drawing delay '" + sTextDrawingDelay + "' !");
                        return false;
                    }

                }else if(attributeName=="textScrollingMode"){
                    std::string sTextScrollingMode = a.as_string();

                    if(sTextScrollingMode=="NORMAL"){
                        textScrollingMode = TextScrollingMode::NORMAL;

                    }else if(sTextScrollingMode=="FIXED_STEP"){
                        textScrollingMode = TextScrollingMode::FIXED_STEP;

                    }else{
                        dbgSystem.addMessage("Unknown text scrolling mode '" + sTextScrollingMode + "' !");
                        return false;
                    }

                }else if(attributeName=="linesSpacingFactor"){
                    linesSpacingFactor = a.as_float(0.0f);


                }else{

                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="textArea"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="textRectangle"){
                    textRectangle = a.as_string();

                }else if(attributeName=="visibleRectangle"){
                    visibleRectangle = a.as_string();

                }else if(attributeName=="clickableRectangle"){
                    clickableRectangle = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="text"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="layer"){
                    layer = a.as_string();

                }else if(attributeName=="book"){
                    textBook = a.as_string();

                }else if(attributeName=="segmentID"){
                    textSegment = a.as_int(-1);

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else if(nodeName=="normalScrollingSlider"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="sprite"){
                    sliderSprite = a.as_string();

                }else if(attributeName=="slider"){
                    sliderCfg = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }

        }else if(nodeName=="fixedScrollingButtons"){

            for(pugi::xml_attribute a = n.first_attribute(); a; a = a.next_attribute()){
                std::string attributeName =std::string(a.name());

                if(attributeName=="scrollDownSprite"){
                    scrollDownSprite = a.as_string();

                }else if(attributeName=="scrollDownButton"){
                    scrollDownButton = a.as_string();

                }else if(attributeName=="scrollUpSprite"){
                    scrollUpSprite = a.as_string();

                }else if(attributeName=="scrollUpButton"){
                    scrollUpButton = a.as_string();

                }else{
                    dbgSystem.addMessage("Unknown attribute '" + attributeName + "' in node '" + nodeName +"' !");
                    return false;
                }
            }


        }else{
            dbgSystem.addMessage("Unknown node '" + nodeName + "' !");
            return false;
        }
    }


    dbgSystem.removeLastMessage();
    return true;

}


// --------------------------------------------------------------------------------------------


float GuiTextField::globalLinesSpacingFactor = 0.0;


//GuiTextField::GuiTextField(SpriteLayer *_layer, Recti _box)
//{
//    kind = GuiWidgetKind::TEXT_FIELD;
//}

/*
GuiTextField::GuiTextField(VectorShape * _vectorShape, Map* _map)
{

    mType = WidgetType::TEXT_FIELD;
    mVectorShape = _vectorShape;
    mMap = _map;
}
*/

GuiTextField::GuiTextField(TextFieldCfg* _cfg, CompositeSprite *_rootSprite)
{

    mType = WidgetType::TEXT_FIELD;
    mCfg = _cfg;
    mRootSprite = _rootSprite;
}


GuiTextField::~GuiTextField()
{
    for(TextFieldLine *tl : Lines){
        delete tl;
    }

    if(slider){
        delete slider;
    }
}


bool GuiTextField::initConnections(GuiSystem *_guiSystem)
{


    dbgSystem.addMessage("Init connections for the widget '" + mCfg->name + "' ...");


    //mMap = mRootSprite->parentLayer()->map();
    mRootMap = mRootSprite->parentLayer()->rootMap();

    mName = mRootSprite->parameters().value("wName");
    if(mName.empty()){
        mName = mRootSprite->name();
    }

    if(mName=="" && mChildWidget==false){
        dbgSystem.addMessage("Missing parameter 'wName' (identification name of the widget) !");
    }


    VectorShape* vs = FindVectorShapeWithName(mRootSprite->map(), mCfg->textRectangle);
    if(vs==nullptr){
        dbgSystem.addMessage("Text rectangle '" + mCfg->textRectangle + "' not found!");
        return false;
    }
    if(vs->isRectangle()==false){
        dbgSystem.addMessage("Text rectangle '" + mCfg->textRectangle + "' is not a rectangle vector shape!");
        return false;
    }

    /*
    name = vs->parameters().value("wName");
    if(name.empty()){
        name = vs->name();
    }

    if(name=="" && mChildWidget==false){
        dbgSystem.addMessage("Missing name of the  'wName' (identification name of the widget) !");
        return false;
    }
    */

    box = vs->boundingBox();


    //----
    visibleBox = box;
    if(mCfg->visibleRectangle.empty()==false){

        vs = FindVectorShapeWithName(mRootSprite->map(), mCfg->visibleRectangle);
        if(vs==nullptr){
            dbgSystem.addMessage("Visible rectangle '" + mCfg->visibleRectangle + "' not found!");
            return false;
        }
        visibleBox = vs->boundingBox();
    }


    //----
    clickableBox = box;
    if(mCfg->clickableRectangle.empty()==false){

        vs = FindVectorShapeWithName(mRootSprite->map(), mCfg->clickableRectangle);
        if(vs==nullptr){
            dbgSystem.addMessage("Clickable rectangle '" + mCfg->clickableRectangle + "' not found!");
            return false;
        }
        clickableBox = vs->boundingBox();
    }

    //----
    layer = dynamic_cast<SpriteLayer*>(FindLayerWithName(mRootSprite->map(), mCfg->layer));
    if(layer==nullptr){
        dbgSystem.addMessage("Layer '"+ mCfg->layer + "' not found!");
        return false;
    }


    //----
    alignment = mCfg->textAlignment;
    drawingDelay = mCfg->textDrawingDelay;
    scrollingMode = mCfg->textScrollingMode;
    localLinesSpacingFactor = mCfg->linesSpacingFactor;


    //---- slider
    if(mCfg->sliderCfg.empty()==false && mCfg->sliderSprite.empty()==false){

        std::vector<Sprite*>sprites;
        CollectSpritesWithName(mRootSprite->map(), sprites, mCfg->sliderSprite);

        if(sprites.empty()){
            dbgSystem.addMessage("Slider sprite '" + mCfg->sliderSprite + "' not found!");
            return false;
        }
        if(sprites.front()->type()!=SpriteType::COMPOSITE){
            dbgSystem.addMessage("Slider sprite must be a composite sprite!");
            return false;
        }


        SliderCfg * sliderCfg = dynamic_cast<SliderCfg *>(_guiSystem->widgetsManager()->getWidgetCfg(mCfg->sliderCfg));
        if(sliderCfg==nullptr){
            dbgSystem.addMessage("Slider '" + mCfg->sliderCfg +"' not found!");
            return false;
        }

        sliderCfg->min = 0.0f;
        sliderCfg->max = 1.0f;
        sliderCfg->def = 0.0f;
        sliderCfg->deltaValue = 0.01f;
        sliderCfg->deltaInPercents = false;


        slider = new GuiSlider(sliderCfg, static_cast<CompositeSprite*>(sprites.front()));
        slider->setChildWidget(true);
        if(slider->initConnections(_guiSystem)==false){
            return false;
        }
    }


    //----
    if(mCfg->textBook.empty()==false && mCfg->textSegment != -1){

        TextBook *textBook = textLibrary.FindBook(mCfg->textBook);
        if(textBook==nullptr){
            dbgSystem.addMessage("Book '" + mCfg->textBook + "' not found!");
            return false;
        }

        TextSegment *ts = textLibrary.GetTextSegmentFromBook(mCfg->textBook, mCfg->textSegment);
        if(ts==nullptr){
            dbgSystem.addMessage("Text segment id '" + std::to_string(mCfg->textSegment) + "' not found!");
            return false;
        }

        Start(ts);
    }


    mInitialized = true;

    //---
    dbgSystem.removeLastMessage();
    return true;

}

/*
bool GuiTextField::init()
{

    mName = mVectorShape->parameters().value("wName");

    dbgSystem.addMessage("init text field '" + mName + "'");


    if(mVectorShape->isRectangle()==false){
        dbgSystem.addMessage("Vector shape is not rectangle!");
        return false;
    }

    //---
    mTag = mVectorShape->parameters().intValue("wTag");
    mDesignKind = mVectorShape->parameters().value("wDesign");


    std::string layerName = mVectorShape->parameters().value("wLayer");
    if(layerName.empty()){
        dbgSystem.addMessage("Missing parameter 'wLayer' !");
        return false;
    }

    layer = dynamic_cast<SpriteLayer*>(FindLayerWithName(mMap, layerName));
    if(layer==nullptr){
        dbgSystem.addMessage("Layer '"+ layerName + "' not found!");
        return false;
    }

    box = mVectorShape->boundingBox();

    //----
    if(mVectorShape->parameters().exists("wSlider")){
        std::string sliderName = mVectorShape->parameters().value("wSlider");
        //slider = dynamic_cast<GuiSlider*>(_map->FindWidget(Parameter::Value(_vectorShape->GetParameters(), "wSlider")));

        std::vector<Sprite*>sprites;
        CollectSpritesWithParameter(mMap, sprites, "wName", sliderName);
        if(sprites.empty()==false && sprites.front()->type()==SpriteType::COMPOSITE){
            slider = new GuiSlider(static_cast<CompositeSprite*>(sprites.front()));
            if(slider->init()==false){
                return false;
            }

            slider->SetValueMin(0.0);
            slider->SetValueMax(1.0);
            //slider->SetDeltaValue(0.25, false);
            slider->SetDeltaValue(0.01, false);
        }
    }


    //----
    visibleBox = box;
    if(mVectorShape->parameters().exists("wVisibleBox")){
        std::string visibleBoxName = mVectorShape->parameters().value("wVisibleBox");
        VectorShape* vs = FindVectorShapeWithName(mMap, visibleBoxName);
        if(vs){
            visibleBox = vs->boundingBox();
        }
    }

    //----
    clickableBox = box;
    if(mVectorShape->parameters().exists("wClickableBox")){
        std::string clickableBoxName = mVectorShape->parameters().value("wClickableBox");
        VectorShape* vs = FindVectorShapeWithName(mMap, clickableBoxName);
        if(vs){
            clickableBox = vs->boundingBox();
        }
    }

    //----
    if(mVectorShape->parameters().exists("wTextSegment")){
        const std::string textSegment = mVectorShape->parameters().value("wTextSegment");
        std::vector<std::string>parts = StdString::splitString(textSegment, ",");
        if(parts.size()==2){
            const std::string bookName = parts[0];
            int segmentId = StdString::stringToInt(parts[1], 0);
            TextSegment *ts = textLibrary.GetTextSegmentFromBook(bookName, segmentId);
            if(ts){
                Start(ts);
            }
        }
    }


    mInitialized = true;

    //---
    dbgSystem.removeLastMessage();

    return true;
}
*/


void GuiTextField::SetTextRect(BoundingBoxInt _box)
{
    Reset();
    box = _box;
}


void GuiTextField::SetVisibleRect(BoundingBoxInt _visibleBox)
{
    Reset();
    visibleBox = _visibleBox;
}


void GuiTextField::SetClickableRect(BoundingBoxInt _clickableBox)
{
    //Reset();
    clickableBox = _clickableBox;
}


int GuiTextField::GetTextSegmentHeight(TextSegment *_textSegment)
{

    assert(box.GetWidth()>0);

    Reset();

    textSegment=_textSegment;
    textSegment->SetWordSizesIfNeeded();

    RebuildLines(false);
    Reset();

    return segmentHeight;
}


void GuiTextField::Start(TextSegment *_textSegment)
{

    if(_textSegment != textSegment){                  // this is usually always the case but for demo test it look a bit nicer if we dont change
        relScrollPosition = 0.0;
    }

    Reset();

    textSegment = _textSegment;
    textSegment->SetWordSizesIfNeeded();


    if(alignment==TextAlignment::JUSTIFY){
        RebuildGaps();
    }

    RebuildLines();

    if(Lines.empty()){
        if(slider){
            slider->SetValue(0.0);
            slider->setDisabled(true);
        }
        return;
    }

    //visibleBox = box;

    //scrollEnabled = false;
    relScrollStep = 1.0;
    scrollingRange = 0;

    //int availableHeight = box.Height();
    //if(nFixedLines != -1){
    //    availableHeight = nFixedLines * fixedLineHeight;
    //}

    if(segmentHeight > box.GetHeight()){
        scrollingRange = segmentHeight - box.GetHeight();
        relScrollStep =  box.GetHeight() / (float) scrollingRange;
        if(relScrollStep>1.0){
            relScrollStep = 1.0;
        }
        //scrollEnabled = true;
    }

    indexFirstLineInBox = 0;



    if(slider){
        //slider->SetDeltaValue(relScrollStep, false);
        slider->SetDeltaValue(relScrollStep/100.0, false);
        slider->SetValue(relScrollPosition);
        slider->setDisabled(segmentHeight <= box.GetHeight());     // disable if all text is displayed at once
    }

    //float offset = relScrollPosition * (segmentHeight - box.Height());
    int offset = std::roundf(relScrollPosition * scrollingRange);
    UpdateVisibleRange(offset);
    //UpdateVisibleRange();

    //---
    if(drawingDelay==TextDrawingDelay::NONE){
        //Update_NoDelay();

        delayedDrawingFinished = true;


    }else if(drawingDelay==TextDrawingDelay::BY_ROWS){

        for(int i=1; i<Lines.size(); i++){
            for(int j=0; j<Lines[i]->subtexts.size(); j++){
                if(Lines[i]->subtexts[j].textSprite){
                    Lines[i]->subtexts[j].textSprite->setVisible(false);
                }
                Lines[i]->subtexts[j].visibleViaDrawingDelay = false;
            }
        }
        drawingDelayStartMS = time.GetPassedNetTimeMS();
        delayedDrawingFinished = false;


    }else if(drawingDelay==TextDrawingDelay::BY_WORDS){

        for(int i=0; i<Lines.size(); i++){
            for(int j=0; j<Lines[i]->subtexts.size(); j++){
                if(i==0 && j==0) continue;
                if(Lines[i]->subtexts[j].textSprite){
                    Lines[i]->subtexts[j].textSprite->setVisible(false);
                }
                Lines[i]->subtexts[j].visibleViaDrawingDelay = false;
            }
        }
        drawingDelayStartMS = time.GetPassedNetTimeMS();
        delayedDrawingFinished = false;


    }else if(drawingDelay==TextDrawingDelay::BY_CHARACTERS){

        for(int i=0; i<Lines.size(); i++){
            for(int j=0; j<Lines[i]->subtexts.size(); j++){
                if(Lines[i]->subtexts[j].textSprite){
                    Lines[i]->subtexts[j].textSprite->setVisible(false);
                }
                Lines[i]->subtexts[j].visibleViaDrawingDelay = false;
            }
        }
        drawingDelayStartMS = time.GetPassedNetTimeMS();
        delayedDrawingFinished = false;
    }
}


void GuiTextField::setDisabled(bool _disabled)
{

    if(mDisabled==_disabled) return;

    GuiWidget::setDisabled(_disabled);

    // reset status flags
    if(_disabled){
        clickedTextName = "";
    }

}


void GuiTextField::setVisible(bool _visible)
{
    // empty - gui text field can not change visibility
}


void GuiTextField::SetScrollingMode(TextScrollingMode _scrollingMode)
{
    Reset();
    scrollingMode = _scrollingMode;
}


void GuiTextField::SetScrollingModeFixedStepProperties(int _numberOfLines, int _lineHeight, float _scrollDurationMS)
{
    Reset();
    fsNumberOfLines = _numberOfLines;
    fsLineHeight = _lineHeight;
    fsScrollDurationMS = _scrollDurationMS;
}


void GuiTextField::SetDrawingDelay(TextDrawingDelay _drawingDelay, int _drawingdelayMS)
{
    Reset();
    drawingDelay = _drawingDelay;
    drawingDelayMS = _drawingdelayMS;
}


void GuiTextField::SetTextAlignment(TextAlignment _textAlignment)
{
    Reset();
    alignment = _textAlignment;
}


void GuiTextField::Reset()
{

    for(TextFieldLine *tl : Lines){
        delete tl;
    }
    Lines.clear();

    textSegment = nullptr;
    gapWords.clear();
    drawingDelayStartMS=0;
    delayedDrawingFinished = false;

    clickableSubtexts.clear();
}


void GuiTextField::RebuildGaps()
{

     assert(box.GetWidth()>0);

     int lineWidth = 0;
     int linesN = 0;        		    //stevilo vrstic
     int lettersN = 0;                  //stevilo znakov (crk)
     int wCount = 0;             	   //stevec besed v celotnem text segmentu
     int wordsPerLine = 0;
     int xOffset = 0;

     gapWords.clear();


    std::vector<TextSegmentWord*>& words = textSegment->GetWords();
    if(words.empty()) return;


     while(true){

         if(wCount >= words.size()-1){	   //konec texta
             gapWords.push_back(0.0);
            //Print " END OF TEXT SEGMENT linesN:"+linesN+"  wordsInLine[linesN]:"+wordsInLine[linesN]+"   "+TS.text[wCount].word+"  -  "+TS.text[wCount].word.length+"  -  "+TS.text[wCount].width

             if(words[wCount]->word=="<<NL"){   //ce je zadnja beseda zahteva po novi vrstici se morajo  array-i povecati se za to vrstico
                 linesN+=1;
                 gapWords.push_back(0.0);
                //Print "ADDED EMPTY LINE"
             }
             break;
         }

        if(words[wCount]->word=="<<NL"){       //force end line
            gapWords.push_back(0.0);
            linesN+=1;
            lettersN = 0;
             lineWidth = 0;
             wordsPerLine = 0;
             wCount+=1;

             continue;
         }


         lineWidth += words[wCount]->width;
         //lettersN += words[wCount]->uWord.GetLength();
         lettersN += words[wCount]->nCharacters;
         //Print "linewidth:"+linewidth
         wordsPerLine+=1;
         //Print "linesN:"+linesN+"   wordsPerLine:"+wordsPerLine+"   -  "+TS.text[wCount].word
         wCount+=1;

         if(words[wCount]->content==TextSegmentWord::TEXT){

             if(wCount < words.size()){

                 // check if the line width reach over the available width
                 if(lineWidth + words[wCount]->width > box.GetWidth()){
                     gapWords.push_back(0.0);
                     gapWords[linesN] = (box.GetWidth()-lineWidth)/float(wordsPerLine-1);	   //the last word is not shifted so it is not used for the gap
                     if(gapWords[linesN] > gapWordsMax){
                         gapWords[linesN] = gapWordsMax;
                     }

                     wordsPerLine=0;
                     lettersN=0;
                     lineWidth=xOffset;
                     linesN+=1;
                 }
             }

         }else if(words[wCount]->content==TextSegmentWord::OFFSET){
             xOffset = words[wCount]->width;
         }


     }

}


void GuiTextField::RebuildLines(bool _rebuildEngineTexts)
{

    if(textSegment->GetWords().empty()){
        return;
    }

    //----
    Lines.clear();
    Lines.push_back(new TextFieldLine());        // create first line
    int ln = 0;                               // index of line
    int lineWidth = 0;                      // width in pixels
    int nWordsInLine = -1;
    int xOffset = 0;


    std::vector<TextSegmentWord*>& words = textSegment->GetWords();


    //if(words.empty()==false && words.front()->keyword!=TextSegmentWord::OFFSET){
    //    lineWidth += textSegment->GetParagarphIndent();
    //}

    for(int wc=0; wc<words.size(); wc++){

        nWordsInLine ++;

        if(words[wc]->word != "<<NL"){

            //ColorRGBA c(255,255,255);
            //if(colors && TS->words[wc]->colorID>=0 && TS->words[wc]->colorID < colors->size()){
            //    c = colors->at(TS->words[wc]->colorID);
            //}
            //c = TS->words[wc]->color;
            float gap = 0.0f;

            if(ln<gapWords.size()){
                gap = gapWords[ln] * nWordsInLine;
            }

            if(words[wc]->word=="Mustafa "){
                DummyFunction();
            }


            //if(nWordsInLine==0){    // first word, check if is offset
            //    xOffset = 0;
           //     if(words[wc]->keyword==TextSegmentWord::OFFSET){
           //         xOffset = words[wc]->width;
           //     }
           // }




            if(words[wc]->content==TextSegmentWord::TEXT){

                //if(nWordsInLine==0){
                    //lineWidth += textSegment->GetInd
                //}


                if(drawingDelay==TextDrawingDelay::NONE){
                    //Line[ln]->Add(*TS->words[wc], lineWidth+gap);
                    Lines[ln]->AddAsSeparateWords(*words[wc], lineWidth+gap);

                }else{
                    Lines[ln]->AddAsSeparateWords(*words[wc], lineWidth+gap);
                }

            }else if(words[wc]->content==TextSegmentWord::OFFSET){
                xOffset = words[wc]->width;

            }

            Lines[ln]->height = std::max(Lines[ln]->height, words[wc]->height);
            lineWidth += words[wc]->width;
        }


        if(wc+1 < words.size()){

            if((lineWidth + words[wc+1]->width > box.GetWidth() && words[wc+1]->word != "<<NL") ||
                words[wc]->word=="<<NL")
            {
                ln += 1;
                Lines.push_back(new TextFieldLine());
                nWordsInLine = -1;
                lineWidth = xOffset;

                //if(words[wc]->word=="<<NL" && words[wc+1]->content!=TextSegmentWord::OFFSET){
                //    lineWidth += textSegment->GetParagarphIndent();
                //}
                //if(words[wc]->word != "<<NL"){
                    //lineWidth += xOffset;
                //}

            }
        }
    }


    //segmentHeight = Line.size() * fontYdistance;

    segmentHeight = 0;
    int lineHeight = 0;
    //float ySpacingFactor = globalLinesSpacingFactor;
    //if(localLinesSpacingFactor > -0.99) ySpacingFactor = localLinesSpacingFactor;


    if(_rebuildEngineTexts){

        int yPos = box.min.y;
        //if(settings.IsYCoordinateUp()){
        if(settings.yOrientation()==Yorientation::UP){
            yPos = box.max.y;
        }

        for(int i=0; i<Lines.size(); i++){

            for(int j=0; j<Lines[i]->subtexts.size(); j++){
                //DbgOutput("Line: " + std::to_string(i) + "  sub: " + std::to_string(j)+ "  :  " + Line[i]->subtexts[j].text);
                Lines[i]->UpdateEngineText(j, layer, Lines[i]->subtexts[j].font, box.min.x, yPos);

                if(Lines[i]->subtexts[j].button != ""){
                    clickableSubtexts.push_back(&Lines[i]->subtexts[j]);
                }
            }

            // In FIXED_STEP mode we adjust the size of boxes to match the specified number of lines !
            if(scrollingMode == TextScrollingMode::FIXED_STEP && fsNumberOfLines == i+1){
                //if(settings.IsYCoordinateUp()){
                if(settings.yOrientation()==Yorientation::UP){
                    int margin = box.min.y - visibleBox.min.y;
                    box.min.y = yPos;
                    visibleBox.min.y = yPos - margin;
                }else{
                    int margin = visibleBox.max.y - box.max.y;
                    box.max.y = yPos;
                    visibleBox.max.y = yPos + margin;
                }
            }


            // get Y position of next line
            if(i < int(Lines.size())-1){

                if(scrollingMode==TextScrollingMode::NORMAL){
                    lineHeight =  Lines[i]->height;
                }else if(scrollingMode==TextScrollingMode::FIXED_STEP){
                    lineHeight = fsLineHeight;
                }
                lineHeight += lineHeight * (globalLinesSpacingFactor + localLinesSpacingFactor);


                //if(settings.IsYCoordinateUp()){
                if(settings.yOrientation()==Yorientation::UP){
                    yPos -= lineHeight;
                }else{
                    yPos += lineHeight;
                }
                segmentHeight += lineHeight;
            }
        }



    }else{

        for(int i=0; i<Lines.size(); i++){

            //float lineHeight = (fixedLineHeight != -1)? fixedLineHeight : Line[i]->height;

            if(i < int(Lines.size())-1){

                if(scrollingMode==TextScrollingMode::NORMAL){
                    lineHeight =  Lines[i]->height;
                }else if(scrollingMode==TextScrollingMode::FIXED_STEP){
                    lineHeight = fsLineHeight;
                }
                lineHeight += lineHeight * (globalLinesSpacingFactor + localLinesSpacingFactor);
                segmentHeight += lineHeight;
            }


            /*
            int lineHeight = Line[i]->height;
            if(i < int(Line.size())-1){
                lineHeight += lineHeight*ySpacingFactor;
            }
            if(mode==Mode::FIXED_STEP){
                lineHeight = fsLineHeight;
            }

            segmentHeight += lineHeight;   // * ySpacingFactor;
            //if(i < int(Line.size())-1){
            //    segmentHeight += lineHeight * ySpacingFactor;
            //}
            */
        }

    }



}


void GuiTextField::update()
{

    assert(mInitialized);

    if(mDisabled) return;
    if(mVisible==false) return;


    mPressedStarted = false;
    clickedTextName = "";


    mHovered = clickableBox.Contains(Vec2fToVec2i(guiCursorDeviceInput.getCursorInMapPosition()));
    if(mHovered){
        mPressed = guiCursorDeviceInput.isPressed();
        mPressedStarted = guiCursorDeviceInput.isPressedStarted();
        GuiWidget::_SetInteractedWidget(this);
    }


    if(textSegment==nullptr) return;
    if(Lines.empty()) return;


    //bool changedRange = false;

    //--- SCROLL
    if(scrollingRange>0){
        if(scrollTween.IsIdle()){

            if(scrollingMode==TextScrollingMode::NORMAL){

                if(slider){

                    slider->update();

                    if(slider->isValueChanged()){

                        if(slider->IsValueChangedViaStepButtons()){
                            float duration = std::abs(slider->GetValue()-relScrollPosition)*0.25;
                            scrollTween.Init(relScrollPosition, slider->GetValue(), duration, EasingKind::LINEAR);
                            scrollTween.Play();

                        }else{
                            relScrollPosition = slider->GetValue();      // value is in range 0.0 - 1.0
                            //UpdateVisibleRange();

                            int offset = std::roundf(relScrollPosition * scrollingRange);
                            if(offset<0) offset = 0;
                            UpdateVisibleRange(offset);
                        }
                    }
                }
            }

        }else{


            if(scrollingMode==TextScrollingMode::NORMAL){
                relScrollPosition = scrollTween.Update();
                int offset = std::roundf(relScrollPosition * scrollingRange);
                UpdateVisibleRange(offset);

            }else if(scrollingMode==TextScrollingMode::FIXED_STEP){
                float offset = scrollTween.Update();
                UpdateVisibleRange(offset);
            }
        }
    }



    //--- DELAYED DRAWING
    if(delayedDrawingFinished==false){

         if(drawingDelay==TextDrawingDelay::BY_ROWS){
             Update_DelayByRow();

         }else if(drawingDelay==TextDrawingDelay::BY_WORDS){
             Update_DelayByWord();

         }else if(drawingDelay==TextDrawingDelay::BY_CHARACTERS){
             Update_DelayByCharacters();
         }
    }


    //--- BUTTONS
    if(clickableSubtexts.empty()==false){
        clickedTextName = GetClickedText();
    }

}


void GuiTextField::Update_DelayByRow()
{

    if(time.GetPassedNetTimeMS()- drawingDelayStartMS < drawingDelayMS){
        return;
    }

    for(int i=0; i<Lines.size(); i++){
        if(Lines[i]->subtexts.empty()) continue;     // should not be possible


        if(Lines[i]->subtexts[0].visibleViaDrawingDelay==false){

            for(int j=0; j<Lines[i]->subtexts.size(); j++){
                assert(Lines[i]->subtexts[j].textSprite);

                Lines[i]->subtexts[j].visibleViaDrawingDelay = true;

                bool visible = Lines[i]->subtexts[j].visibleViaDrawingDelay && Lines[i]->inVisibleBox;

                TextSprite * textSprite = Lines[i]->subtexts[j].textSprite;
                if(textSprite==nullptr) continue;

                if(textSprite->isVisible() != visible){
                    textSprite->setVisible(visible);
                }
            }

            if(i==Lines.size()-1){   // all words are shown
                delayedDrawingFinished = true;
            }
            break;
        }
    }


    drawingDelayStartMS = time.GetPassedNetTimeMS();

}


void GuiTextField::Update_DelayByWord()
{

    if(time.GetPassedNetTimeMS()- drawingDelayStartMS < drawingDelayMS){
        return;
    }

    for(int i=0; i<Lines.size(); i++){
        for(int j=0; j<Lines[i]->subtexts.size(); j++){

            if(Lines[i]->subtexts[j].visibleViaDrawingDelay==false){
                Lines[i]->subtexts[j].visibleViaDrawingDelay = true;

                bool visible = Lines[i]->subtexts[j].visibleViaDrawingDelay && Lines[i]->inVisibleBox;

                TextSprite * textSprite = Lines[i]->subtexts[j].textSprite;
                if(textSprite==nullptr) continue;

                if(textSprite->isVisible() != visible){
                    textSprite->setVisible(visible);
                }

                if(i==Lines.size()-1 && j==Lines[i]->subtexts.size()-1 && Lines[i]->subtexts[j].visibleViaDrawingDelay){   // all words are shown
                    delayedDrawingFinished = true;
                }

                goto exitloop;
            }
        }
    }
    exitloop:;

    drawingDelayStartMS = time.GetPassedNetTimeMS();

}


void GuiTextField::Update_DelayByCharacters()
{

    if(time.GetPassedNetTimeMS()- drawingDelayStartMS < drawingDelayMS){
        return;
    }


    for(int i=0; i<Lines.size(); i++){
        for(int j=0; j<Lines[i]->subtexts.size(); j++){

            TextSprite * textSprite = Lines[i]->subtexts[j].textSprite;
            if(textSprite == nullptr) continue;


            if(Lines[i]->subtexts[j].visibleViaDrawingDelay==false){
                Lines[i]->subtexts[j].visibleViaDrawingDelay = true;
                Lines[i]->subtexts[j].charCount = 0;           // set counter to starting index
            }

            if(Lines[i]->subtexts[j].charCount >= 0){        // character counter has been started

                bool endReached = true;
                std::string s = GetSubstring(Lines[i]->subtexts[j].text, 0, Lines[i]->subtexts[j].charCount, endReached);
                if(endReached==false){
                    textSprite->setText(s);
                    Lines[i]->subtexts[j].charCount++;

                    bool visible = Lines[i]->subtexts[j].visibleViaDrawingDelay && Lines[i]->inVisibleBox;
                    if(textSprite->isVisible() != visible){
                        textSprite->setVisible(visible);
                    }

                    goto exitloop;

                }else{

                    Lines[i]->subtexts[j].charCount = -1;    // full word has been drawn -> set index to -1 that
                }

            }

            if(i==Lines.size()-1 && j==Lines[i]->subtexts.size()-1){   // all words are shown
                delayedDrawingFinished = true;
            }


        }
    }
    exitloop:;

    drawingDelayStartMS = time.GetPassedNetTimeMS();

}


void GuiTextField::SkipDelay()
{


    if(delayedDrawingFinished==true) return;

    for(int i=0; i<Lines.size(); i++){
        for(int j=0; j<Lines[i]->subtexts.size(); j++){

            TextSprite * textSprite = Lines[i]->subtexts[j].textSprite;
            if(textSprite==nullptr) continue;


            Lines[i]->subtexts[j].visibleViaDrawingDelay = true;
            bool visible = Lines[i]->subtexts[j].visibleViaDrawingDelay && Lines[i]->inVisibleBox;
            if(textSprite->isVisible() != visible){
                textSprite->setVisible(visible);
            }

            if(Lines[i]->subtexts[j].charCount>=0){
                textSprite->setText(Lines[i]->subtexts[j].text);
            }

            Lines[i]->subtexts[j].charCount = -1;


            //if(engineText->IsVisible()==false){
            //    engineText->SetVisible(true);
            //}
            //Line[i]->subtexts[j].charCount = -1;
        }
    }

    delayedDrawingFinished = true;

}


bool GuiTextField::ScrollTextUp()
{

    if(scrollingMode==TextScrollingMode::NORMAL){

        if(scrollingRange==0) return false;
        if(AreEqual(relScrollPosition, 1.0)) return false;

        float relDest = relScrollPosition + relScrollStep;
        if(relDest>1.0) relDest = 1.0;

        float duration = std::abs(relDest-relScrollPosition)*0.25;
        scrollTween.Init(relScrollPosition, relDest, duration, EasingKind::LINEAR);
        scrollTween.Play();

        if(slider){
            slider->SetValue(relDest);
        }


    }else if(scrollingMode==TextScrollingMode::FIXED_STEP){

        if(indexFirstLineInBox + fsNumberOfLines >= Lines.size()) return false;

        int currentOffset = indexFirstLineInBox * fsLineHeight ;
        indexFirstLineInBox += fsNumberOfLines;

        print( " firstLineInBox :" + std::to_string(indexFirstLineInBox));

        int lineHeight = fsLineHeight;
        lineHeight += lineHeight * (globalLinesSpacingFactor + localLinesSpacingFactor);

        int newOffset = indexFirstLineInBox * lineHeight ;

        //UpdateVisibleRangeB(newOffset);

        scrollTween.Init(currentOffset, newOffset, fsScrollDurationMS/1000.0, EasingKind::LINEAR);
        scrollTween.Play();

    }


    return true;
}


bool GuiTextField::ScrollTextDown()
{

    if(scrollingMode==TextScrollingMode::NORMAL){

        if(scrollingRange==0) return false;
        if(AreEqual(relScrollPosition, 0.0)) return false;

        float relDest = relScrollPosition - relScrollStep;
        if(relDest<0.0) relDest = 0.0;

        float duration = std::abs(relDest-relScrollPosition)*0.25;
        scrollTween.Init(relScrollPosition, relDest, duration, EasingKind::LINEAR);
        scrollTween.Play();

        if(slider){
            slider->SetValue(relDest);
        }


    }else if(scrollingMode==TextScrollingMode::FIXED_STEP){

        if(indexFirstLineInBox==0) return false;

        int currentOffset = indexFirstLineInBox * fsLineHeight ;

        indexFirstLineInBox -= fsNumberOfLines;

        //DbgOutput( " firstLineInBox :" + std::to_string(indexFirstLineInBox));

        if(indexFirstLineInBox < 0){
            assert(false);              // should never happened!
            indexFirstLineInBox = 0;
        }

        int lineHeight = fsLineHeight;
        lineHeight += lineHeight * (globalLinesSpacingFactor + localLinesSpacingFactor);
        int newOffset = indexFirstLineInBox * lineHeight;

        //UpdateVisibleRangeB(newOffset);

        scrollTween.Init(currentOffset, newOffset, fsScrollDurationMS/1000.0, EasingKind::LINEAR);
        scrollTween.Play();

    }

    return true;

}


void GuiTextField::UpdateVisibleRange(float offset)
{

    //offset = std::round(offset);

     for(int i=0; i<Lines.size(); i++){
         if(Lines[i]->subtexts.empty()) continue;     // possible if there was only key on the line

         //float yPos = (settings.IsYCoordinateUp())? (Lines[i]->yPos + offset) : (Lines[i]->yPos - offset);
         float yPos = (settings.yOrientation()==Yorientation::UP)? (Lines[i]->yPos + offset) : (Lines[i]->yPos - offset);
         int yPosInt = std::round(yPos);

         Lines[i]->inVisibleBox = (yPosInt >= visibleBox.min.y && yPosInt <= visibleBox.max.y);
         Lines[i]->inBox = (yPosInt >= box.min.y && yPosInt <= box.max.y);

         float alpha = 1.0;

         if(alphaFadeAtMargin){

             if(visibleBox.min.y < box.min.y){
                 if(yPosInt >= visibleBox.min.y && yPosInt<=box.min.y){
                     alpha = (yPosInt-(visibleBox.min.y))/float(box.min.y - visibleBox.min.y);
                 }
             }

            if(visibleBox.max.y > box.max.y){
                 if(yPosInt >= box.max.y && yPosInt <= visibleBox.max.y){
                     alpha = 1.0f - (yPosInt - box.max.y)/float(visibleBox.max.y - box.max.y);
                 }
             }
         }


         for(int j=0; j<Lines[i]->subtexts.size(); j++){
             TextSprite *ts = Lines[i]->subtexts[j].textSprite;                  assert(ts);

             //Lines[i]->subtexts[j].inVisibleRange = Lines[i]->inVisibleBox;

             bool visible = Lines[i]->inVisibleBox && Lines[i]->subtexts[j].visibleViaDrawingDelay;
             if(visible != ts->isVisible()){
                 ts->setVisible(visible);
             }

             Vec2f pos(ts->position().x, yPos);
             //if(pos.Equals(ts->GetPosition())==false){
             if(IsEqual(pos, ts->position())==false){
                 ts->setPosition(pos);
             }

             if(AreEqual(alpha, ts->alpha())==false){
                 ts->setAlpha(alpha);
             }
         }
     }

}


std::string GuiTextField::GetClickedText()
{

    std::string nameCursorOver;
    std::string nameClicked;

    for(TextFieldLineSubtext *st : clickableSubtexts){
        assert(st->textSprite);
        if(st->textSprite->pointInside(guiCursorDeviceInput.getCursorInMapPosition())){
            nameCursorOver = st->button;
            if(guiCursorDeviceInput.isPressedStarted()){
                nameClicked =  st->button;
            }
            break;
        }
    }


    //--- set colors
    for(TextFieldLineSubtext *st : clickableSubtexts){
        if(st->button==nameCursorOver){
            if(st->textSprite->color() != st->color_CO){
                st->textSprite->setColor(st->color_CO);
            }
        }else{
            if(st->textSprite->color() != st->color){
                st->textSprite->setColor(st->color);
            }
        }
     }

    return nameClicked;

}


bool GuiTextField::IsTextSegmentFirstLineDisplayed()
{

    if(Lines.empty()) return false;

    for(int i=0; i<Lines.size(); i++){
        if(Lines[i]->subtexts.empty()) continue;        // possible if there was only key on the line
        return Lines[i]->inBox;
    }

    return false;
}


bool GuiTextField::IsTextSegmentLastLineDisplayed()
{

    if(Lines.empty()) return false;

    for(int i=Lines.size()-1; i>=0; i--){
        if(Lines[i]->subtexts.empty()) continue;        // possible if there was only key on the line
        return Lines[i]->inBox;
    }

    return false;
}


//========================================================================================


TextFieldLine::~TextFieldLine()
{

    for(TextFieldLineSubtext &s : subtexts){
        if(s.textSprite){
           s.textSprite->parentLayer()->deleteSprite(s.textSprite);
        }
    }
}


void TextFieldLine::Add(const TextSegmentWord &_word, float textPos)
{

    if(subtexts.back().color != _word.color){

        //if(subtexts.back().text != ""){                  //naredimo nov subtext, razen ce je trenutni prazen (na zacetku vrstice)
            subtexts.push_back(TextFieldLineSubtext());
        //}

        subtexts.back().text += _word.word;
        //subtexts.back().uText.Append(_word.uWord);
        subtexts.back().color = _word.color;
        subtexts.back().font = _word.font;
        subtexts.back().button = _word.button;
        subtexts.back().color_CO = _word.buttonColor_CO;
        subtexts.back().xPos = std::roundf(textPos);

    }else{

        subtexts.back().text += _word.word;
        //subtexts.back().uText.Append(_word.uWord);

    }
}


void TextFieldLine::AddAsSeparateWords(const TextSegmentWord &_word, float textPos)
{

    //if(subtexts.back().text != ""){                  //naredimo nov subtext, razen ce je trenutni prazen (na zacetku vrstice)
        subtexts.push_back(TextFieldLineSubtext());
    //}

    subtexts.back().text += _word.word;
    subtexts.back().color = _word.color;
    subtexts.back().font = _word.font;
    subtexts.back().button = _word.button;
    subtexts.back().color_CO = _word.buttonColor_CO;
    subtexts.back().xPos = std::roundf(textPos);

}


void TextFieldLine::UpdateEngineText(int _subtextIndex, SpriteLayer *_layer, Font *_font, int _xBox, int _yPos)
{

    assert(_subtextIndex>=0 && _subtextIndex<subtexts.size());

    yPos = _yPos;

    const std::string &text = subtexts[_subtextIndex].text;

    if(subtexts[_subtextIndex].textSprite==nullptr){
        Vec2f pos(_xBox + subtexts[_subtextIndex].xPos, _yPos);
        //subtexts[_subtextIndex].textSprite = objectFactory->NewTextSprite(text, _font, subtexts[_subtextIndex].color, pos, TextHandleVariant::LEFT, _layer);
        subtexts[_subtextIndex].textSprite = _layer->addTextSprite(text, _font, subtexts[_subtextIndex].color, pos, TextHandleVariant::LEFT);

        //if(subtexts[_subtextIndex].button !=""){
        //    subtexts[_subtextIndex].textSprite->SetClickable(true);
        //}

    }else{
        subtexts[_subtextIndex].textSprite->setText(text);
    }

}


//========================================================================================


GuiWidgetCfg* TextFieldFactory::createWidgetCfg(const std::string &_name)
{
    return new TextFieldCfg(this, _name);
}


GuiWidget* TextFieldFactory::createWidget(GuiWidgetCfg* _cfg, Sprite *_rootSprite)
{
    assert(_cfg->factory==this);

    if(_rootSprite->type()!=SpriteType::COMPOSITE){
        dbgSystem.addMessage("Creating widget '" + _cfg->name + " of type '" + _cfg->factory->typeSignature() +"' from sprite '"
                             + _rootSprite->sourceSprite()->name() +"' error !");
        dbgSystem.addMessage("The sprite must be a composite sprite!'");
        return nullptr;
    }

    return new GuiTextField(static_cast<TextFieldCfg*>(_cfg), static_cast<CompositeSprite*>(_rootSprite));
}







}




