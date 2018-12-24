//cursorLeft.hpp

#ifndef CURSORLeft_HPP
#define CURSORLeft_HPP

#include "Command.hpp"

class cursorLeft: public Command
{
public:
    virtual void execute(EditorModel& model) override;

    virtual void undo(EditorModel& model) override;

private:
    int old_line;
    int old_col;

};
















#endif