//cursorDown.hpp

#ifndef CURSORDown_HPP
#define CURSORDown_HPP

#include "Command.hpp"

class cursorDown: public Command
{
public:
    virtual void execute(EditorModel& model) override;

    virtual void undo(EditorModel& model) override;

private:
    int old_line;
    int old_col;

};
















#endif