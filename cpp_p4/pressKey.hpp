//pressKey.hpp

#ifndef PRESSKEY_HPP
#define PRESSKEY_HPP

#include "Command.hpp"


class pressKey: public Command
{
public:
    pressKey(char key);

    virtual void execute(EditorModel& model) override;

    virtual void undo(EditorModel& model) override;

private:
    char key;
    int line;
    int col;

};









#endif