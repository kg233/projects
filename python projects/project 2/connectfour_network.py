## Kuixi Ren 43838927
import connectfour
import connectfour_function
import connectfour_socket_handling

class BoardSizeError(Exception):
    pass

def establish() -> connectfour_socket_handling.Connection or str:
    '''UI for user to enter connection details, return connection namedtuple,
    returns unsuccessful if connection unsuccessful'''
    current_connection = None
    server = input('Please enter a server IP: ')
    port = input('Please enter the port: ')
    print(f'Connecting to {server}:{port}...')
    current_connection = connectfour_socket_handling.connect_to(server, port)
    if current_connection == None:
        print('Connection error, closing program..')
        return 'unsuccessful'
    print('Connection Successful!')
    try:
        _check_to_close(_ai_hello(_ask_username(), current_connection))
        _check_to_close(_check_server_AIGAME(current_connection))
        return current_connection
    except connectfour_socket_handling.ServerProtocolError:
        print('The server does not match the protocol, connection has been severed.')
        return 'unsuccessful'
    

def begin_multiplayer(current_connection: connectfour_socket_handling.Connection):
    '''UI for multiplayer'''
    if current_connection == 'unsuccessful':
        return
    
    NONE = connectfour.NONE
    RED = connectfour.RED
    YELLOW = connectfour.YELLOW


    
    connectfour_function.display_begin_banner()
    
    current_state = connectfour.new_game()

    connectfour_function.read_board(current_state.board)

    
    
    while True:
        print('------------------------------------------------------------------')
        connectfour_function.print_whos_turn(current_state)
        
        
        if current_state.turn == RED:
            
            current_state = _send_perf_user_action(current_connection, current_state)

            connectfour_function.read_board(current_state.board)
            
            if connectfour.winner(current_state) != NONE:
            
                connectfour_function.decide_winner(current_state)

                break


        elif current_state.turn == YELLOW:
            
            try:
                current_state = _perf_server_action(current_connection, current_state)
            except connectfour_socket_handling.ServerProtocolError:
                print('The server does not match the protocol.')
                connectfour_socket_handling.close_connection(current_connection)
                print('connection has been severed')
                return
            except BoardSizeError:
                print('Board size between client and server do not match.')
                connectfour_socket_handling.close_connection(current_connection)
                print('connection has been severed')
                return
            except (connectfour.GameOverError, ValueError, connectfour.InvalidMoveError):
                print('Server made an invalid move, closing connection...')
                connectfour_socket_handling.close_connection(current_connection)
                print('connection closed.')
                return
            
            
            connectfour_function.read_board(current_state.board)

            if connectfour.winner(current_state) != NONE:
            
                connectfour_function.decide_winner(current_state)

                break

    
    connectfour_function.display_end_banner()

    print('closing connection...')
    connectfour_socket_handling.close_connection(current_connection)
    print('connection closed!')
    return



def _check_to_close(b:bool):
    
    '''raises an exception and returns if b is false'''
    if not b:
        raise connectfour_socket_handling.ServerProtocolError()
        connectfour_socket_handling.close_connection(current_connection)
        return
        


def _ask_username() -> str:
    '''ask for a valid username, returns the valid username'''

    while True:
        name = input('Please enter a one-word username: ')
        if not (len(name.split()) != 1 or name.strip() != name):
            break

        print('Wrong username format, try again')
    return name
        
def _ai_hello(name: str, c:connectfour_socket_handling.Connection) -> bool:
    '''use the name for server's hello to the server, return True if successful
        false otherwise'''
    connectfour_socket_handling.send_msg(c, 'I32CFSP_HELLO ' + name)
    back_msg = connectfour_socket_handling.read_from_server(c)

    if back_msg.startswith('WELCOME'):
        return True
    else:
        return False
    
def _check_server_AIGAME(c: connectfour_socket_handling.Connection) -> bool:
    '''Sends AI_GAME to server and return True if server returns Ready'''
    connectfour_socket_handling.send_msg(c, 'AI_GAME')

    return _check_READY_WINNER(c)

def _check_OKAY_INVALID(c: connectfour_socket_handling.Connection) -> tuple:
    '''returns True if readline from server returns OKAY'''
    msg = connectfour_socket_handling.read_from_server(c)
    return  msg == 'OKAY', msg

def _check_READY_WINNER(c: connectfour_socket_handling.Connection) -> bool:
    '''returns True if readline from server returns "READY" or WINNER_"YELLOW" or "RED"'''
    msg = connectfour_socket_handling.read_from_server(c)
    return msg == "READY" or (msg == 'WINNER_YELLOW' or msg == 'WINNER_RED')


def _check_protocol(s: str) -> bool:
    '''check if the string follows the protocol'''
    if (s[:4] == "DROP" and s[4:5] == ' ' and s[5:]) \
       or (s[:3] == "POP" and s[3:4] == ' '):
        try:
            int(s[5:])
            return True
        except ValueError:
            try:
                int(s[4:])
                return True
            except ValueError:
                return False
    else:
        return False

def _str_to_action(s: str) -> connectfour_function.action:
    '''converts a string in to a action namedtuple'''
    if s.startswith('DROP'):
        return connectfour_function.action(s[:4], s[5:])
    else:
        return connectfour_function.action(s[:3], s[4:])

def _rcv_server_action(c: connectfour_socket_handling.Connection) -> connectfour_function.action:
    '''Read from server three times, check each for protocol error, if all
    normal, return the second line from server'''
    OKAY_stat, OKAY_msg = _check_OKAY_INVALID(c)
 
    if OKAY_stat:
        msg = connectfour_socket_handling.read_from_server(c)
    elif OKAY_msg == 'INVALID':
        raise BoardSizeError()
    else:
        raise connectfour_socket_handling.ServerProtocolError()
    
    if _check_READY_WINNER(c):
        if _check_protocol(msg):
            print('AI: ' + msg)
            return _str_to_action(msg)
        else:
            raise connectfour_socket_handling.ServerProtocolError()
        
    else:
        raise connectfour_socket_handling.ServerProtocolError()

def _perf_server_action(c: connectfour_socket_handling.Connection, state) -> connectfour.GameState:
    '''performs action rcvd from server, returns new GameState'''

    return connectfour_function.drop_to(state, _rcv_server_action(c))

        
        
def _send_perf_user_action(c: connectfour_socket_handling.Connection, state) -> connectfour.GameState:
    '''Performs the action inputted by user, send action to server'''
    while True:
        play = connectfour_function._action_reader()
        msg = ' '.join([play.drop_pop.upper(), play.num])
        if _check_protocol(msg):
            try:
                new_state = connectfour_function.drop_to(state, play)

                connectfour_socket_handling.send_msg(c, msg)
                
                return new_state
            except ValueError:
                print('\nERROR: PLEASE ENTER A CORRECT COLUMN NUMBER!!\n')
                
            except connectfour.GameOverError:
                print('\nERROR: GAME IS OVER\n')
                return state

            except connectfour.InvalidMoveError:
                print('\nERROR: THE MOVE IS INVALID, PLEAE TRY AGAIN\n')
                
        else:
            print('Invalid format, try again ("DROP/POP" followed by a space and column number)')



        
if __name__ == '__main__':
    begin_multiplayer(establish())
