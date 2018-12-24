//cursorUp.hpp

#ifndef CURSORUp_HPP
#define CURSORUp_HPP

#include "Command.hpp"

class cursorUp: public Command
{
public:
    virtual void execute(EditorModel& model) override;

    virtual void undo(EditorModel& model) override;

private:
    int old_line;
    int old_col;

};
















#endif