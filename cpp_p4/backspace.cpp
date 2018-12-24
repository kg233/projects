//backspace.cpp
#include "backspace.hpp"


void backspace::execute(EditorModel& model){
    line = model.cursorLine();
    col = model.cursorColumn();


    if (col != 1){
        deleted_key = model.line(line)[col-2];
        model.setCursorCol(col - 1);
        model.modify(false);

    }

    else{
        if (line == 1){
            throw EditorException("At top");
        }
        else{
            const std::string line_top = model.line(line-1);
            const std::string line_bottom = model.line(line);

            concat_line = line -1;
            concat_col = line_top.size() + 1;

            model.setLineContent(line-2, line_top + line_bottom);
            model.removeLine(line);

            model.setCursorCol(concat_col);
            model.setCursorLine(concat_line);

        }
    }
}

void backspace::undo(EditorModel& model){
    if (deleted_key != '\0'){
        model.modify(true, deleted_key);

    }
    else{
        std::string substring = model.line(concat_line).substr(concat_col-1);
        std::string top_string = model.line(concat_line);
        std::string top_string_after_split = top_string.erase(concat_col-1);



        model.setLineContent(concat_line-1, top_string_after_split);
        model.insertLine(line,substring);

    }

    model.setCursorCol(col);
    model.setCursorLine(line);


}