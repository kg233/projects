## Kuixi Ren 43838927
import connectfour
import connectfour_function

NONE = connectfour.NONE
RED = connectfour.RED
YELLOW = connectfour.YELLOW

def begin_singleplayer():
    connectfour_function.display_begin_banner()

    current_state = None

    current_state = connectfour.new_game()

    connectfour_function.read_board(current_state.board)

    while True:

        connectfour_function.print_whos_turn(current_state)
        
        current_state = connectfour_function.user_drop(current_state)
        
        connectfour_function.read_board(current_state.board)
        
        if connectfour.winner(current_state) != NONE:           
            connectfour_function.decide_winner(current_state)
            break
    connectfour_function.display_end_banner()

if __name__ == '__main__':
    begin_singleplayer()

            
            
    

