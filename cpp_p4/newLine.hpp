//newLine.hpp

#ifndef newLine_HPP
#define newLine_HPP

#include "Command.hpp"


class newLine: public Command
{
public:
    virtual void execute(EditorModel& model) override;

    virtual void undo(EditorModel& model) override;

private:

    int line;
    int col;


};









#endif