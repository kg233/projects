//cursorLeft.cpp
#include "cursorLeft.hpp"

void cursorLeft::execute(EditorModel& model){
    old_line = model.cursorLine();
    old_col = model.cursorColumn();

    if (old_col == 1){
        if (old_line == 1){
            throw EditorException("At beginning of the first line");
        }
        else{
            model.setCursorLine(old_line - 1);
            model.setCursorCol(model.line(old_line-1).size() + 1);
        }
    }
    else{
        model.setCursorCol(old_col - 1);
    }

}

void cursorLeft::undo(EditorModel& model){
    model.setCursorCol(old_col);
    model.setCursorLine(old_line);
}

