//pressKey.cpp
#include "pressKey.hpp"

pressKey::pressKey(char key)
    :key{key}
{
}

void pressKey::execute(EditorModel& model){
    line = model.cursorLine();
    col = model.cursorColumn();

    model.modify(true, key);

    model.setCursorCol(col + 1);
}

void pressKey::undo(EditorModel& model){
    model.setCursorCol(col);
    model.setCursorLine(line);

    model.modify(false);
}