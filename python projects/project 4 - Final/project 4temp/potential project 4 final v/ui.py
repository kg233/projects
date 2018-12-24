## Kuixi Ren 43838927
import mechanics as m

def run():
    blockF = m.BlockField(get_list())
    blockF.move_forward()
    GameF = m.GameField(blockF.output())
    while True:
        if not GameF.hasfaller:
            GameF.check_all()
            blockF = m.BlockField(GameF.output())
        try:
            blockF.check_over() ####
        except m.GameOverError:
            print_list(GameF.list)
            print('GAME OVER')
            return
        print_list(GameF.list)
        user_inp = input()
        if user_inp == 'Q':
            return
        elif user_inp.startswith('F'):
            try:
                GameF = perform_create_faller(GameF, user_inp)
            except m.GameOverError:
                print('GAME OVER')
                return
        elif user_inp == 'R':
            GameF = perform_rotation(GameF)
        elif user_inp == '<':
            GameF = perform_move_h(GameF, 'left')
        elif user_inp == '>':
            GameF = perform_move_h(GameF, 'right')
        elif user_inp == '':
            GameF.age()

def get_list() -> list:
    '''get a list based on user input'''
    result = _set_size(_ask_how_many(), _ask_how_many(), _ask_type())
    return result

def print_list(dlist: list):
    '''prints the list,'''
    norm_list = m.transform_list_perspective(dlist)
    truncate_list = []
    for ind in range(2, len(norm_list)):
        truncate_list.append(norm_list[ind])
    for row in truncate_list:
        print('|{}|'.format(''.join(row)))
    print(' ' + '---' * len(truncate_list[-1]) + ' ')
    
def perform_create_faller(gameF, s: str) -> m.GameField:
    '''taking a gameF, return a dlist with a faller created'''
    command = s.split()
    gameF.make_faller(int(command[1]) - 1, command[4], command[3], command[2])
    return gameF

def perform_move_h(gameF, s: str) -> m.GameField:
    '''Moves left on the GameField obj, returns new GameField'''
    if s == 'left':
        gameF.move_left()
    elif s == 'right':
        gameF.move_right()
    return gameF

def perform_rotation(gameF) -> m.GameField:
    '''gameF.rotate(), returns Gamefield'''
    gameF.rotate()
    return gameF

def _ask_how_many() -> int:
    '''ask for a number'''
    return int(input())

def _ask_type() -> str:
    '''ask for either EMPTY or CONTENT'''
    return input()

def _set_size(row, column, ask_type) -> list:
    '''ask user for row and column, return a list based on input'''
    temp = m.create_empty_list(row, column)
    if ask_type == 'EMPTY':
        return temp
    wordlist = []
    for row_ind in range(row):
        s = input()
        if len(s) != column:
            print('string length do not match size')
            return
        wordlist.append(_split_to_list(s))
    expanded_word = m.transform_list_perspective(wordlist)
    for col_ind in range(column):
        for row_ind in range(row):
            temp[col_ind][row_ind + 2] = expanded_word[col_ind][row_ind]
    return temp

def _split_to_list(s: str) -> list:
    '''change a string to a list of characters with space in front and back'''
    l = list(s)
    for item in range(len(l)):
        l[item] = ' ' + str(l[item]) + ' '
    return l

if __name__ == '__main__':
    run()
    
