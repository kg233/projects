## Kuixi Ren 43838927

import connectfour
from collections import namedtuple
action = namedtuple('action', 'drop_pop num')

NONE = connectfour.NONE
RED = connectfour.RED
YELLOW = connectfour.YELLOW

    
def read_board(board: [[int]]):
    '''converts GameState.board into display string'''

    print()
    result = []
    
    row = len(board[-1])

    for i in range(row):
        result.append([])

    for column in range(len(board)):
        for item in range(len(board[column])):
            if board[column][item] == NONE:
                result[item].append('.')

            elif board[column][item] == RED:
                result[item].append('R')

            elif board[column][item] == YELLOW:
                result[item].append('Y')
                
    width = len(result[-1])
    number_line_list = []
    number_line = ''
    
    for i in range(1, int(width)+1):
        number_line += '{:2s}  '.format(str(i))
        
    print(number_line)
    
    for row in result:
        line = ''
        for item in row:
            
            line += item
        print(_string_with_spaces(line))
        print()
    print()

def print_whos_turn(state):
    '''prints a line indicating who's turn it is'''
    print(turn_color(state.turn), '\'S TURN\n', sep = '')

def turn_color(num: int) -> str:
    '''returns the color(RED, YELLOW) based on the number'''
    if num == RED:
        return 'RED'
    elif num == YELLOW:
        return 'YELLOW'
    
def user_drop(state) -> tuple:
    '''asks user where to drop, returns new GameState'''
    while True:
        play = _action_reader()
        try:
            return drop_to(state, play)
            
        except ValueError:
            print('\nERROR: PLEASE ENTER A CORRECT COLUMN NUMBER!!\n')
            
        except connectfour.GameOverError:
            print('\nERROR: GAME IS OVER\n')
            return state

        except connectfour.InvalidMoveError:
            print('\nERROR: THE MOVE IS INVALID, PLEAE TRY AGAIN\n')
            

def drop_to(state, play: action) -> tuple:
    '''Returns GameState after new move has been executed'''
    if play.drop_pop.lower() == 'drop':
        new_state = connectfour.drop(state, int(play.num)-1)
        return new_state
    elif play.drop_pop.lower() == 'pop':
        new_state = connectfour.pop(state, int(play.num)-1)
        return new_state

def decide_winner(state):
    '''Prints x player wins if x wins'''
    if connectfour.winner(state) == RED:
        print('RED PLAYER WINS!')
    elif connectfour.winner(state) == YELLOW:
        print('YELLOW PLAYER WINS!')

def display_begin_banner():
    '''prints the text for game start'''
    print('--------------------------')
    print('GAME START')
    
def display_end_banner():
    '''prints game over text'''
    print()
    print('Game Over')
    print('--------------------------')
    

def _string_with_spaces(line: str) -> str:
    '''returns a string with two spaces in between each character'''
    temp = []
    for item in line:
        temp.append(item)

    return '   '.join(temp)

        
    
def _action_reader() -> action:

    '''prompts user to enter a action, only allow drop or pop'''

    while True:
        entry = input('PLEASE ENTER AN ACTION (DROP/POP) FOLLOWED BY A COLUMN NUMBER: ')
        entry = entry.strip()

        if entry.lower().startswith('drop'):
            return action(entry[0:4], entry[4:].strip())
        
        elif entry.lower().startswith('pop'):
            return action(entry[0:3], entry[3:].strip())
        else:
            print('\nERROR: INVALID ENTRY, PLEASE ENTER AN ACTION (DROP/POP) FOLLOWED BY A COLUMN NUMBER\n')



        
            
        
