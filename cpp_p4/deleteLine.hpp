//deleteLine.hpp

#ifndef deleteLine_HPP
#define deleteLine_HPP

#include "Command.hpp"


class deleteLine: public Command
{
public:
    virtual void execute(EditorModel& model) override;

    virtual void undo(EditorModel& model) override;

private:
    std::string deleted_line;
    int line;
    int col;
};









#endif