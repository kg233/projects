//backspace.hpp

#ifndef BACKSPACE_HPP
#define BACKSPACE_HPP

#include "Command.hpp"


class backspace: public Command
{
public:
    virtual void execute(EditorModel& model) override;

    virtual void undo(EditorModel& model) override;

private:
    char deleted_key = '\0';
    int line;
    int col;

    int concat_line;
    int concat_col;

};









#endif