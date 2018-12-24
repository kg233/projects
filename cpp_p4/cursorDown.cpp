//cursorDown.cpp
#include "cursorDown.hpp"

void cursorDown::execute(EditorModel& model){
    old_line = model.cursorLine();
    old_col = model.cursorColumn();

    if (old_line == model.lineCount()){
        throw EditorException("At bottom");
    }

    else{
        if (model.line(old_line+1).size() < old_col){
            model.setCursorLine(old_line+1);
            model.setCursorCol(model.line(old_line+1).size()+1);
        }
        else{
            model.setCursorLine(old_line+1);
        }
    }

}

void cursorDown::undo(EditorModel& model){
    model.setCursorCol(old_col);
    model.setCursorLine(old_line);
}

