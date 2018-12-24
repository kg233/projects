// KeypressInteractionReader.cpp
//
// ICS 45C Fall 2018
// Project #4: People Just Want to Play with Words
//
// Implementation of the KeypressInteractionReader
//
// YOU WILL NEED TO IMPLEMENT SOME THINGS HERE

#include "KeypressInteractionReader.hpp"

#include "cursorRight.hpp"
#include "cursorLeft.hpp"
#include "cursorUp.hpp"
#include "cursorDown.hpp"
#include "pressKey.hpp"
#include "backspace.hpp"
#include "deleteLine.hpp"
#include "newLine.hpp"
#include "cursorHome.hpp"
#include "cursorEnd.hpp"


// You will need to update this member function to watch for the right
// keypresses and build the right kinds of Interactions as a result.
//
// The reason you'll need to implement things here is that you'll need
// to create Command objects, which you'll be declaring and defining
// yourself; they haven't been provided already.
//
// I've added handling for "quit" here, but you'll need to add the others.

Interaction KeypressInteractionReader::nextInteraction()
{
    while (true)
    {
        Keypress keypress = keypressReader.nextKeypress();

        if (keypress.ctrl())
        {
            // The user pressed a Ctrl key (e.g., Ctrl+X); react accordingly

            switch (keypress.code())
            {
            case 'X':
                return Interaction::quit();

            case 'Z':
                return Interaction::undo();

            case 'A':
                return Interaction::redo();

            case 'O':
                return Interaction::command(new cursorRight);

            case 'U':
                return Interaction::command(new cursorLeft);

            case 'I':
                return Interaction::command(new cursorUp);

            case 'K':
                return Interaction::command(new cursorDown);

            case 'H':
                return Interaction::command(new backspace);

            case 'D':
                return Interaction::command(new deleteLine);

            case 'J':
                return Interaction::command(new newLine);
            case 'M':
                return Interaction::command(new newLine);

            case 'Y':
                return Interaction::command(new cursorHome);

            case 'P':
                return Interaction::command(new cursorEnd);

            }

        }
        else
        {

            return Interaction::command(new pressKey(keypress.code()));
            // The user pressed a normal key (e.g., 'h') without holding
            // down Ctrl; react accordingly
        }
    }
}

