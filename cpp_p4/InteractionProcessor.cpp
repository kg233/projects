// InteractionProcessor.cpp
//
// ICS 45C Fall 2018
// Project #4: People Just Love to Play with Words
//
// Implementation of the InteractionProcessor class

#include "InteractionProcessor.hpp"
#include "EditorException.hpp"
#include "Interaction.hpp"

// #include "BooEditLog.hpp"

// This function implements command execution, but does not handle "undo"
// and "redo"; you'll need to add that handling.

void InteractionProcessor::run()
{
    view.refresh();

    std::vector<Command*> undo_stack;
    std::vector<Command*> redo_stack;

    while (true)
    {
        Interaction interaction = interactionReader.nextInteraction();

        if (interaction.type() == InteractionType::quit)
        {
            std::for_each(undo_stack.begin(), undo_stack.end(), [](Command* x){delete x;});
            undo_stack.erase(undo_stack.begin(), undo_stack.end());
            std::for_each(redo_stack.begin(), redo_stack.end(), [](Command* x){delete x;});
            redo_stack.erase(redo_stack.begin(), redo_stack.end());

            break;
        }
        else if (interaction.type() == InteractionType::undo)
        {

            if (undo_stack.size() > 0){
                

                Command* command = undo_stack[undo_stack.size()-1];
                redo_stack.push_back(command);

                command->undo(model); //shoudn't have exceptions occur since command has been executed successfully before

                undo_stack.pop_back();
            }
            view.refresh();
        }
        else if (interaction.type() == InteractionType::redo)
        {
            if (redo_stack.size() > 0){
                Command* command = redo_stack[redo_stack.size()-1];
                undo_stack.push_back(command);
                command -> execute(model);

                redo_stack.pop_back();
            }
            view.refresh();
        }
        else if (interaction.type() == InteractionType::command)
        {
            Command* command = interaction.command();
            try
            {
                command->execute(model);

                //add command to the undo stack
                undo_stack.push_back(command);
                //empties redo stack
                std::for_each(redo_stack.begin(), redo_stack.end(), [](Command* x){delete x;});
                redo_stack.erase(redo_stack.begin(), redo_stack.end());

                model.clearErrorMessage();
            }

            catch (EditorException& e)
            {
                model.setErrorMessage(e.getReason());
                delete command;
            }

            view.refresh();

            // Note that you'll want to be more careful about when you delete
            // commands once you implement undo and redo.  For now, since
            // neither is implemented, I've just deleted it immediately
            // after executing it.  You'll need to wait to delete it until
            // you don't need it anymore.
            
            // delete command;
        }
    }
}

