## Chenqian Cao 93485665 and Kuixi Ren 43838927 ICS 32


from pathlib import Path
import os
from shutil import copyfile

## searching a directory for files

def main_func(L: list) -> list:
    '''The main function that takes the first letter and the file path'''
    the_list = []

    try: 
        if L[0] == 'D':
            the_list = D_return_files(L[1])

            for x in the_list:
                print(x)

        elif L[0] == 'R':
            the_list = R_return_files(L[1])
                
            for x in the_list:
                print(x)

        else:
            print('ERROR')
            return main_func(executer())
            

        return the_list

    except NotADirectoryError:
            print('ERROR')
            return main_func(executer())
        
def D_return_files(p: 'path') -> list:
    '''return a list of the files in path'''
    path = Path(p)
    result=[]
    sortl = []
    
    file_list = list(path.iterdir())
    
    for file in file_list:
        if file.is_file():
            result.append(file)
            
    sortl = sort_winpath(result)
    
    return sortl


def R_return_files(p:'path') -> list:
    '''returns a list of all the files in the directory, including the ones in the sub-dir'''
    result = []
    sortl = []
    result.extend(D_return_files(p))
    path = Path(p)
    file_list = list(path.iterdir())
    sortl = sort_winpath(file_list)
    for file in sortl:
        if file.is_dir():
            result.extend(R_return_files(file))
    return result

def sort_winpath(l: list) -> list:
    '''sort a list of windows path in lexicographical order'''
    result = []
    for item in l:
        result.append(str(item))
        result.sort()
    for ind in range(len(result)):
        result[ind] = Path(result[ind])
    return result
        
def executer() -> list:
    '''A function for sorting the command for main_func()'''
    command = str(input())
    command_split = command.split(' ', 1)
    try:
        if len(command_split) == 2 and len(command_split[0]) == 1 and len(command_split[1]) > 0 and command_split[1][0] != ' ' and Path(command_split[1]).exists():
              return command_split
        else:
            print('ERROR')
            return executer()
    except OSError:
        print('ERROR')
        return executer()
        
## Finding interesting files in the directory by some guidelines

def search_func(L: list) -> list:
    '''returns the interesting list by each command'''
    interest = []
    command = str(input())
    if command == 'A':
        interest = L
        for item in interest:
            print(item)
        return interest
    command_split = command.split(' ', 1)
    if len(command_split) == 2 and len(command_split[0]) == 1:
        if command_split[0] == 'N':
            for item in L:
                if os.path.basename(item) == command_split[1]:
                    print(item)
                    interest.append(item)
            return interest

        elif command_split[0] == 'E':
            if command_split[1][0] != '.':
                command_split[1] = '.' + command_split[1]
            for item in L:
                if item.suffix == command_split[1]:
                    print(item)
                    interest.append(item)
            return interest

        elif command_split[0] == 'T':
            for item in L:
                if istxt(item):
                    file = open(item, 'r')
                    for lines in file:
                        if command_split[1] in lines:
                            print(item)
                            interest.append(item)
                            break
                    file.close()
            return interest

        elif command_split[0] == '<':
            try:
                int(command_split[1][0])
                int(command_split[1])
                for item in L:
                    if size_comp(item, command_split[1], '<'):
                        print(item)
                        interest.append(item)
                return interest
            
            except ValueError:
                print("ERROR")
                return search_func(L)

        elif command_split[0] == '>':
            try:
                int(command_split[1][0])
                int(command_split[1])
                for item in L:
                    if size_comp(item, command_split[1], '>'):
                        print(item)
                        interest.append(item)
                return interest
            
            except ValueError:
                print("ERROR")
                return search_func(L)
        else:
            print('ERROR')
            return search_func(L)
    else:
        print('ERROR')
        return search_func(L)
    

def istxt(path) -> bool:
    '''returns true if file is a test document'''
    file = None
    try:
        file = open(path)
        file.read()
        return True
    except:
        return False
    finally:
        if file != None:
            file.close()

def size_comp(path, num, op: str) -> bool:
    '''Returns true if the file is less than or greater than depending on the
    input operation than the num'''
    if op == '<':
        return os.path.getsize(path) < int(num)
    elif op == '>':
        return os.path.getsize(path) > int(num)

## Action to be done to the files of interest

def act_func(L: list):
    '''Action on int_list'''
    if L == []:
        return
    
    command = str(input())
    if command == 'F':
        for item in L:
            if istxt(item):
                file = open(item, 'r')
                first_line = file.readline()
                if '\n' in first_line:
                    first_line = first_line.replace('\n', '')
                print(first_line)
                
                file.close()
            else:
                print("NOT TEXT")

    elif command == "D":
        for item in L:
            copyfile(item, str(item) + '.dup')
    elif command == 'T':
        for item in L:
            os.utime(item, times = None)
    else:
        print('ERROR')
        act_func(L)
            

if __name__ == '__main__':
    act_func(search_func(main_func(executer())))



