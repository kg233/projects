//deleteLine.cpp

#include "deleteLine.hpp"

void deleteLine::execute(EditorModel& model){
    if (model.lineCount() == 1 and model.line(1) == ""){
        throw EditorException("No Lines to delete");
    }

    line = model.cursorLine();
    col = model.cursorColumn();
    deleted_line = model.line(line);
    model.removeLine(line);
    if (model.lineCount() == 0){
        model.insertLine(1, "");
        model.setCursorLine(1);
        model.setCursorCol(1);
    }
    else{
        if (model.lineCount() < line){
            model.setCursorLine(model.lineCount());
            model.setCursorCol(model.line(model.lineCount()).size()+1);
        }
        else{
            if (model.line(line).size() < col){
                model.setCursorCol(model.line(line).size()+1);
            }
        }

    }
    
}

void deleteLine::undo(EditorModel& model){
    if (model.lineCount() == 1 and model.line(1) == ""){
        model.removeLine(1);
        model.insertLine(1, deleted_line);
    }

    else{
        model.insertLine(line, deleted_line);
    }

    model.setCursorLine(line);
    model.setCursorCol(col);
}