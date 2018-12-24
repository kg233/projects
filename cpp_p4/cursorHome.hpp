//cursorHome.hpp

#ifndef cursorHome_HPP
#define cursorHome_HPP

#include "Command.hpp"


class cursorHome: public Command
{
public:
    virtual void execute(EditorModel& model) override;

    virtual void undo(EditorModel& model) override;

private:
    int col;

};









#endif