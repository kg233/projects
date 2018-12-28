

import connectfour
import connectfour_function
import socket
from collections import namedtuple

Connection = namedtuple('Connection', 'socket s_in s_out')

_debug = False

class ServerProtocolError(Exception):
    pass

def connect_to(host: str, port: int) -> Connection:
    '''connect to a server and a port, return invalid ip or port if
    failed to connect, return the socket and the in and out file'''
    four_socket = socket.socket()
    try:
        four_socket.connect((host, int(port)))
    except socket.gaierror:
        print('INVALID IP')
        
        return
    except ConnectionRefusedError:
        print('INVALID PORT')
        
        return
    except OverflowError:
        print('PORT MUST BE 0-65535')

        return

    except TimeoutError:
        print('Connection timed out')

        return

    except ValueError:
        print('PORT CAN ONLY BE NUMBERS')

        return

    four_socket_in = four_socket.makefile('r')
    four_socket_out = four_socket.makefile('w')

    return Connection(
        four_socket,
        four_socket_in,
        four_socket_out)
    
def read_from_server(c: Connection) -> str:
    '''returns a line from the server, if any'''
    msg = c.s_in.readline()[:-1]
    if _debug == True:
        print('***RCVD***:', msg)
    return msg

def send_msg(c: Connection, s: str):
    '''sends a message  to server, addes ELS and flushes'''
    if _debug == True:
        print('***SENT***:', s)
    c.s_out.write(_addELS(s))
    c.s_out.flush()

def close_connection(Connection):
    '''closes connections and files to the server'''
    Connection.socket.close()
    Connection.s_in.close()
    Connection.s_out.close()


def _addELS(msg: str) -> str:
    '''adds end of the line sequence to a string'''
    return msg + '\r\n'




    
