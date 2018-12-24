## Kuixi Ren 43838927

##game over
class GameOverError(Exception):
    pass

## class
class Faller:
    '''A class that holds falling objects, will be Lander when landed'''
    def __init__(self, dlist, column, lastind):
        self.column = column
        self.lastind = lastind
        self.block_a = dlist[column][lastind]   #X
        self.block_b = dlist[column][lastind-1] #Y
        self.block_c = dlist[column][lastind-2] #Z
        self.cannmove = True
        
    def rotate(self):
        self.block_a, self.block_b, self.block_c = self.block_b, self.block_c, self.block_a
        
    def where(self):
        return (self.column, self.lastind, (self.block_a, self.block_b, self.block_c))
        
class Lander:
    '''landing blocks that will freeze into Blocks'''
    def __init__(self, dlist, Faller):
        self.column = Faller.column
        self.lastind = Faller.lastind
        self.block_a = dlist[self.column][self.lastind]   #X
        self.block_b = dlist[self.column][self.lastind-1] #Y
        self.block_c = dlist[self.column][self.lastind-2]  #Z

    def rotate(self):
        self.block_a, self.block_b, self.block_c = self.block_b, self.block_c, self.block_a
        
    def where(self):
        return (self.column, self.lastind, (self.block_a, self.block_b, self.block_c))    

class Air:
    '''air block'''
    
    def __init__(self, dlist, ind1, ind2):
        self.ind1 = ind1
        self.ind2 = ind2
        self.identity = dlist[ind1][ind2]
        self.canmove = True

    def where(self):
        '''returns the two indexes of the block in a tuple'''
        return (self.ind1, self.ind2, 'AIR')

class Block:
    '''represents one block in the field, '''
    def __init__(self, dlist, ind1, ind2):
        self.ind1 = ind1
        self.ind2 = ind2
        self.identity = dlist[ind1][ind2]
        self.canmove = False
    def where(self):
        '''returns the two indexes of the block in a tuple'''
        return (self.ind1, self.ind2, self.identity)    

class BlockField:
    '''A class object that considers all object to be blocks, all empty to air, considers winning/losing state'''
    def __init__(self, dlist):
        '''sets the 2dlist parameter as the field'''
        self.list = dlist
        self.block_list = self._create_block_list()
###
    def locate_all(self):
        '''returns all the locations of each block, FOR DEBUGGING'''
        for column in range(len(self.block_list)):
            for block in range(len(self.block_list[column])):
                print(self.block_list[column][block].where())
        print(self.block_list)
###

    def move_forward(self):
        '''move blocks forward if possible'''
        temp_list = []
        for column in self.list:
            temp_col = column
            for i in range(len(temp_col)):
                temp_col = _empty_infront(temp_col)
            temp_list.append(temp_col)
        self.list = temp_list
        self.block_list = self._create_block_list()
        

    def check_over(self):
        '''checks current dlist for game over'''
        can_check = True
        for column in self.block_list:
            for ind in range(len(column)):
                if not column[ind].identity.startswith(' '):
                    can_check = False
            if can_check and (type(column[0]) == Block or type(column[1]) == Block) and (column[0].identity.startswith(' ') or column[1].identity.startswith(' ')):
                raise GameOverError()
        #print(can_check)

    def _create_block_list(self):
        '''creates block_list from self.list'''
        block_list = []
        for ind1 in range(len(self.list)):
            column_list = []
            for ind2 in range(len(self.list[ind1])):
                if self.list[ind1][ind2] != '   ':
                    column_list.append(Block(self.list, ind1, ind2))
                else:
                    column_list.append(Air(self.list, ind1, ind2))
            block_list.append(column_list)
        return block_list

    def output(self):
        '''returns self.list, the dlist'''
        return self.list

class GameField:
    '''An object that treats Blocks as solid walls, used to manage fallers and lander'''
    def __init__(self, dlist):
        '''Uses Blockfield'''
        self.blockfield = BlockField(dlist)
        self.list = self.blockfield.list
        self.block_list = self.blockfield.block_list
        self.hasfaller = False
        self.faller = None
        self.matching = False

    def make_faller(self, column, X, Y, Z):
        '''make a faller, at the column index, COLUMN IS THE INDEXXXXX'''
        if (not self.hasfaller) and (not self.matching):
            if self.list[column][0] == '   ' and self.list[column][1] == '   ' and self.list[column][2] != '   ':
                #print('This column is full')
                raise GameOverError
    
            if _can_move_to(self.block_list, column, 2):
                _dlist_replace_to_fallers(self.list, column, 2, X, Y, Z)
                self.block_list[column][2] = Faller(self.list, column, 2)
                self.faller = self.block_list[column][2]
                self._check_ground()
                self.hasfaller = True

    def rotate(self):
        if self.hasfaller:
            self.faller.rotate()
            _replace_fallers(self.list, self.faller.column, self.faller.lastind, self.faller)
            self.block_list[self.faller.column][self.faller.lastind] = Faller(self.list, self.faller.column, self.faller.lastind)

    def move_left(self):
        self._move_horizontal('<')

    def move_right(self):
        self._move_horizontal('>')
       
    def age(self):
        '''move forward one grid if allowed, triggers only when Time is flowing
            if falling is not allowed, transform into a Lander, if already a Lander, transform
            to a block'''
        if type(self.faller) == Lander:
            self._freeze()
            #return self.list
        
        if self.hasfaller:
            if self.list[self.faller.column][self.faller.lastind + 1] == '   ':
                self._move_faller_one_down()
                self._check_ground()

        if not self.hasfaller:
            self.blockfield = BlockField(remove_star(self.list))
            self.move_to_end()
            self.matching = False
            #return self.list

    def move_to_end(self):
        '''a move_forward() method for GameField'''
        self.blockfield.move_forward()
        self.block_list = self.blockfield.block_list
        self.list = self.blockfield.output()

    def check_all(self):
        '''check for horizontal, vertical, and diagonal'''
        temp = self.list
        self.list = check_diagonal(check_horizontal(check_vertical(self.list)))
        self.blockfield = BlockField(self.list)
        if temp != self.list:
            self.matching = True
        
            
    def locate_all(self):
        '''returns all the locations of each block, FOR DEBUGGING'''
        for column in range(len(self.block_list)):
            for block in range(len(self.block_list[column])):
                print(self.block_list[column][block].where())
        print(self.list)
        #print(self.block_list)

    def _move_horizontal(self, direction):
        '''moves Faller/Lander in the direction, if possible, changes lander back to faller'''
        
        if self.hasfaller:
            if direction == '<': # left
                if self.faller.column != 0:
                    if _can_move_to(self.block_list, self.faller.column - 1, self.faller.lastind):
                        _dlist_replace_to_fallers(self.list, self.faller.column - 1, self.faller.lastind, self.faller.block_a[1], self.faller.block_b[1], self.faller.block_c[1])
                        _dlist_remove_three(self.list, self.faller.column, self.faller.lastind)
                        self.block_list[self.faller.column][self.faller.lastind] = Air(self.list, self.faller.column, self.faller.lastind)
                        self.block_list[self.faller.column - 1][self.faller.lastind] = Faller(self.list, self.faller.column - 1, self.faller.lastind)
                        self.faller = Faller(self.list, self.faller.column - 1, self.faller.lastind)
                        self._check_ground()
                        
            if direction == '>': # Right
                if self.faller.column != len(self.list) - 1:
                    if _can_move_to(self.block_list, self.faller.column + 1, self.faller.lastind):
                        # need to copy paste since the column is added instead of subtract
                        _dlist_replace_to_fallers(self.list, self.faller.column + 1, self.faller.lastind, self.faller.block_a[1], self.faller.block_b[1], self.faller.block_c[1])
                        _dlist_remove_three(self.list, self.faller.column, self.faller.lastind)
                        self.block_list[self.faller.column][self.faller.lastind] = Air(self.list, self.faller.column, self.faller.lastind)
                        self.block_list[self.faller.column + 1][self.faller.lastind] = Faller(self.list, self.faller.column + 1, self.faller.lastind)
                        self.faller = Faller(self.list, self.faller.column + 1, self.faller.lastind)
                        self._check_ground()

    def _move_faller_one_down(self):
        '''move faller one down, refreshes self.list and self.block_list'''
        _replace_fallers(self.list, self.faller.column, self.faller.lastind + 1, self.faller)     
        self.faller = Faller(self.list, self.faller.column, self.faller.lastind + 1)
        self.block_list[self.faller.column][self.faller.lastind - 1] = Air(self.list, self.faller.column, self.faller.lastind - 1)
        self.block_list[self.faller.column][self.faller.lastind] = Faller(self.list, self.faller.column, self.faller.lastind)
        
    def _check_ground(self):
        '''check to see if one unit below is air, if not, change faller to lander'''

        if type(self.faller) == Faller:
            if self.faller.lastind == len(self.list[-1])-1 or self.list[self.faller.column][self.faller.lastind + 1] != '   ':
                _dlist_replace_to_lander(self.list, self.faller.column, self.faller.lastind, self.faller)
                self.block_list[self.faller.column][self.faller.lastind] = Lander(self.list, self.faller)
                self.faller = Lander(self.list, self.faller)

    def _freeze(self) -> 'dlist':
        '''freezes the current landing block when age() is used,
        converts each block in Lander to a Block
        '''
        if type(self.faller) == Lander:
            obj_list = [self.faller.block_a[1], self.faller.block_b[1], self.faller.block_c[1]]
            for ind in range(len(obj_list)):
                self.list[self.faller.column][self.faller.lastind - ind] = ' {} '.format(obj_list[ind])
            self.blockfield = BlockField(self.list)
            self.block_list = self.blockfield.block_list
            self.hasfaller = False
            self.faller = None
    
    def _get_blockfield(self):
        self.blockfield = self.block_list

    def output(self):
        return self.list

## functions

def create_empty_list(row, column) -> list:
        '''creates a 2d list that has ROW sublist, each sublist has COLUMN item, each item is placeholder 0'''
        result = []
        for r in range(column):
            col = []
            for item in range(row + 2):
                col.append('   ')
            result.append(col)
        return result

def transform_list_perspective(l: [list]) -> [list]:
    '''transform from sublist being rows to sublist being columns'''
    result = []
    row = len(l[-1])
    for i in range(row):
        result.append([])
    for column in range(len(l)):
        for item in range(len(l[column])):
                result[item].append(l[column][item])
    return result     

def check_vertical(dlist) -> 'dlist':
    '''check for match in the vertical dimension (column), asterisk on both side if found'''
    result = transform_list_perspective(transform_list_perspective(dlist))
    for column in range(len(dlist)):
        for ind in range(len(dlist[0])):
            if dlist[column][ind] != '   ':
                if ind + 2 <= len(dlist[0]) - 1:
                    if dlist[column][ind][1] == dlist[column][ind + 1][1] and \
                    dlist[column][ind][1] == dlist[column][ind+2][1]:
                        result[column][ind] = '*{}*'.format(dlist[column][ind][1])
                        result[column][ind+1] = '*{}*'.format(dlist[column][ind+1][1])
                        result[column][ind+2] = '*{}*'.format(dlist[column][ind+2][1])
    return result

def check_horizontal(dlist) -> 'dlist':
    '''check for match in the horizontal dimension (row), asterisk on both side if found'''
    result = transform_list_perspective(dlist)
    result = check_vertical(result)
    return transform_list_perspective(result)

def check_diagonal(dlist) -> 'dlist':
    '''check for match in the diagonal dimension ([ind][ind]), asterisk on both side if found'''
    result = transform_list_perspective(transform_list_perspective(dlist))
    for ind1 in range(len(dlist)):
        for ind2 in range(len(dlist[-1])):
            if dlist[ind1][ind2] != '   ':
                if ind1 + 2 <= len(dlist) - 1:
                    if ind2 + 2 <= len(dlist[0]) - 1:
                        if dlist[ind1][ind2][1] == dlist[ind1 + 1][ind2 + 1][1] and \
                           dlist[ind1][ind2][1] == dlist[ind1 + 2][ind2 + 2][1]:
                            result[ind1][ind2] = '*{}*'.format(dlist[ind1][ind2][1])
                            result[ind1+1][ind2+1] = '*{}*'.format(dlist[ind1+1][ind2+1][1])
                            result[ind1+2][ind2+2] = '*{}*'.format(dlist[ind1+2][ind2+2][1])
                    if ind2 - 2 >= 0:
                        if dlist[ind1][ind2][1] == dlist[ind1 + 1][ind2 - 1][1] and \
                           dlist[ind1][ind2][1] == dlist[ind1 + 2][ind2 - 2][1]:
                            result[ind1][ind2] = '*{}*'.format(dlist[ind1][ind2][1])
                            result[ind1+1][ind2-1] = '*{}*'.format(dlist[ind1+1][ind2-1][1])
                            result[ind1+2][ind2-2] = '*{}*'.format(dlist[ind1+2][ind2-2][1])   
    return result

def remove_star(dlist) -> 'dlist':
    '''given a dlist, remove the asterisks and fill with air'''
    for ind1 in range(len(dlist)):
        for ind2 in range(len(dlist[ind1])):
            if dlist[ind1][ind2].startswith('*'):
                dlist[ind1][ind2] = '   '
    return dlist

def _empty_infront(column: 'column', ) -> 'column':
    '''move the block forward in a column if the index after it is three white space'''
    index = len(column) - 1
    if column[index] == '   ':
        column = [column[index]] + column[0:index]
        index -= 1
    while index >= 1:
        if column[index] == '   ':
            column = [column[index]] + column[0:index] + column[index + 1:]
        index -= 1
    return column

def _can_move_to(BlockField, column, ind) -> bool:
    '''returns True if there are no Walls at the index and the two units above the index'''
    if 0 <= ind < 2:
        return False
    return BlockField[column][ind].canmove and BlockField[column][ind-1].canmove and BlockField[column][ind-2].canmove

def _dlist_replace_to_fallers(dlist, column, ind, X, Y, Z):
    '''replace X at index ind, Y at index ind-1, Z at index ind -2'''
    dlist[column][ind] = '[{}]'.format(X)
    dlist[column][ind-1] = '[{}]'.format(Y)
    dlist[column][ind-2] = '[{}]'.format(Z)
    if ind >= 3:
        dlist[column][ind-3] = '   '
        
def _dlist_remove_three(dlist, column, ind):
    '''remove the three above and including ind, change to empty, used for dlist'''
    lock = ind - 3
    while ind > lock:
        dlist[column][ind] = '   '
        ind -= 1
    
def _replace_fallers(dlist, column, ind, faller):
    '''replace X at index ind, Y at index ind-1, Z at index ind -2'''
    dlist[column][ind] = '{}'.format(faller.block_a)
    dlist[column][ind-1] = '{}'.format(faller.block_b)
    dlist[column][ind-2] = '{}'.format(faller.block_c)
    if ind >= 3:
        dlist[column][ind-3] = '   '
        
def _dlist_replace_to_lander(dlist, column, ind, Faller):
    '''replace X at index ind, Y at index ind-1, Z at index ind -2'''
    dlist[column][ind] = '|{}|'.format(Faller.block_a[1])
    dlist[column][ind-1] = '|{}|'.format(Faller.block_b[1])
    dlist[column][ind-2] = '|{}|'.format(Faller.block_c[1])
    if ind >= 3:
        dlist[column][ind-3] = '   '

