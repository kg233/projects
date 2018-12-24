//cursorRight.cpp
#include "cursorRight.hpp"

void cursorRight::execute(EditorModel& model){
    old_line = model.cursorLine();
    old_col = model.cursorColumn();

    if (old_col > model.line(old_line).size()){
        if (old_line == model.lineCount()){
            throw EditorException("At end of last line");
        }
        else{
        model.setCursorLine(old_line + 1);
        model.setCursorCol(1);
        }
    }
    else{
        model.setCursorCol(old_col + 1);
    }

}

void cursorRight::undo(EditorModel& model){
    model.setCursorCol(old_col);
    model.setCursorLine(old_line);
}
