//cursorUp.cpp
#include "cursorUp.hpp"

void cursorUp::execute(EditorModel& model){
    old_line = model.cursorLine();
    old_col = model.cursorColumn();

    if (old_line == 1){
        throw EditorException("At top");
    }

    else{
        if (model.line(old_line-1).size() < old_col){
            model.setCursorLine(old_line-1);
            model.setCursorCol(model.line(old_line-1).size()+1);
        }
        else{
            model.setCursorLine(old_line-1);
        }
    }

}

void cursorUp::undo(EditorModel& model){
    model.setCursorCol(old_col);
    model.setCursorLine(old_line);
}

