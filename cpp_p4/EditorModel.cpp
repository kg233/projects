// EditorModel.cpp
//
// ICS 45C Fall 2018
// Project #4: People Just Love to Play with Words
//
// Implementation of the EditorModel class

#include "EditorModel.hpp"
#include "EditorException.hpp"


EditorModel::EditorModel()
    :cursorLineNum{1}, cursorColNum{1}, text{""}
{

}


int EditorModel::cursorLine() const
{
    return cursorLineNum;
}


int EditorModel::cursorColumn() const
{
    return cursorColNum;
}


int EditorModel::lineCount() const
{
    return text.size();
}


const std::string& EditorModel::line(int lineNumber) const
{
    return text[lineNumber-1];


}


const std::string& EditorModel::currentErrorMessage() const
{
    return errorMsg;
}


void EditorModel::setErrorMessage(const std::string& errorMessage)
{
    errorMsg = errorMessage;
}


void EditorModel::clearErrorMessage()
{
    errorMsg = "";
}

//my functions

void EditorModel::setCursorLine(int value){
    cursorLineNum = value;
}

void EditorModel::setCursorCol(int value){
    cursorColNum = value;
}

void EditorModel::modify(bool opCode, char key){
    if (opCode){
        if (key != '\0'){
            text[cursorLineNum-1].insert(cursorColNum - 1, std::string(1,key));
        }
    }
    else{
        text[cursorLineNum-1].erase(cursorColNum - 1, 1);
    }
}

void EditorModel::setLineContent(int line, const std::string& str){
    text[line] = str;
}

void EditorModel::removeLine(int line){
    text.erase(text.begin() + line -1);
}

void EditorModel::insertLine(int line, const std::string& line_content){
    text.insert(text.begin()+ line - 1, line_content);
}