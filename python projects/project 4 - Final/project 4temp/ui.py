## Kuixi Ren 43838927
import mechanics as m

def run():
    blockF = m.BlockField(get_list())
    blockF.move_forward()
    GameF = m.GameField(blockF.output())
    while True:
        if not GameF.hasfaller:
            blockF = m.BlockField(m.check_all(blockF.output()))
            dlist = blockF.output()
            GameF = m.GameField(dlist) 
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
            GameF = m.perform_create_faller(GameF, user_inp)
        elif user_inp.startswith('R'):
            GameF = m.perform_rotation(GameF)
        elif user_inp == '<':
            GameF = m.perform_move_h(GameF, 'left')
        elif user_inp == '>':
            GameF = m.perform_move_h(GameF, 'right')
        elif user_inp == '':
            agelist = None
            agelist = GameF.age()    
            if agelist != None:
                GameF = m.GameField(agelist)
                blockF = m.BlockField(agelist)
                blockF.move_forward()

def get_list() -> list:
    '''get a list based on user input'''
    result = m.set_size(_ask_how_many(), _ask_how_many(), _ask_type())
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

def _ask_how_many() -> int:
    '''ask for a number'''
    return int(input())

def _ask_type() -> str:
    '''ask for either EMPTY or CONTENT'''
    return input()

if __name__ == '__main__':
    run()
    
