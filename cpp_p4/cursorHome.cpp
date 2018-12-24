//cursorHome.cpp

#include "cursorHome.hpp"

void cursorHome::execute(EditorModel& model){
    col = model.cursorColumn();
    model.setCursorCol(1);
}

void cursorHome::undo(EditorModel& model){
    model.setCursorCol(col);
}