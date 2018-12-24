//cursorEnd.cpp

#include "cursorEnd.hpp"

void cursorEnd::execute(EditorModel& model){
    col = model.cursorColumn();
    model.setCursorCol(model.line(model.cursorLine()).size()+1);
}

void cursorEnd::undo(EditorModel& model){
    model.setCursorCol(col);
}