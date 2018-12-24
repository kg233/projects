//newLine.cpp

#include "newLine.hpp"

void newLine::execute(EditorModel& model){
    line = model.cursorLine();
    col = model.cursorColumn();
    model.insertLine(line + 1, "");

    std::string substring = model.line(line).substr(col-1);
    std::string top_string = model.line(line);
    model.setLineContent(line-1, top_string.erase(col-1));

    model.setLineContent(line, substring);

    model.setCursorCol(1);
    model.setCursorLine(line + 1);
}

void newLine::undo(EditorModel& model){
    const std::string line_top = model.line(line);
    const std::string line_bottom = model.line(model.cursorLine());

    model.setLineContent(line-1, line_top + line_bottom);
    model.removeLine(line+1);

    model.setCursorCol(col);
    model.setCursorLine(line);
}