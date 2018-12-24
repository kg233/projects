//cursorEnd.hpp

#ifndef cursorEnd_HPP
#define cursorEnd_HPP

#include "Command.hpp"


class cursorEnd: public Command
{
public:
    virtual void execute(EditorModel& model) override;

    virtual void undo(EditorModel& model) override;

private:
    int col;

};









#endif