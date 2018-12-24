//cursorRight.hpp

#ifndef CURSORRIGHT_HPP
#define CURSORRIGHT_HPP

#include "Command.hpp"

class cursorRight: public Command
{
public:
    virtual void execute(EditorModel& model) override;

    virtual void undo(EditorModel& model) override;

private:
    int old_line;
    int old_col;

};
















#endif