# Submitter: kuixir(Ren, Kuixi)
import goody

def read_fa(file : open) -> {str:{str:str}}:
    result = dict()
    for line in file:
        split_line = line.rstrip().split(';')
        result[split_line[0]] = {u_inp: state for u_inp, state in zip(split_line[1::2], split_line[2::2])}
    return result



def fa_as_str(fa : {str:{str:str}}) -> str:
    return ''.join((f'  {state} transitions: {sorted(fa[state].items())}\n' for state in sorted(fa) ))

    
def process(fa : {str:{str:str}}, state : str, inputs : [str]) -> [None]:
    result = [state]
    current_state = state
    for inp in inputs:
        if inp not in fa[current_state]:
            result.append((inp, None))
            break
        current_state = fa[current_state].get(inp)
        result.append((inp, current_state))
    return result


def interpret(fa_result : [None]) -> str:
    result = f'Start state = {fa_result.pop(0)}\n'
    for item_tuple in fa_result:
        if item_tuple[1] == None:
            result += f'  Input = {item_tuple[0]}; illegal input: simulation terminated\n'
            continue
        result += '  Input = {}; new state = {}\n'.format(item_tuple[0], item_tuple[1])
    result += 'Stop state = {}\n'.format(fa_result[-1][1])
    return result



if __name__ == '__main__':
    # Write script here
    fa_dict = read_fa(goody.safe_open('Specify the file name storing the finite automaton', 'r', 'unable to read this file'))
    print('\nThe Description of the specified Finite Automaton\n' + fa_as_str(fa_dict))
    inp_file = goody.safe_open('Specify the file name storing the start-state with its inputs', 'r', 'unable to read this file')
    print()
    for line in inp_file:
        stripped_line = line.rstrip().split(';')
        print('Begin to trace a new FA simulation\n' + interpret(process(fa_dict, stripped_line.pop(0), stripped_line)))
    # For running batch self-tests
    print()
    import driver
    driver.default_file_name = "bsc3.txt"
#     driver.default_show_traceback = True
#     driver.default_show_exception = True
#     driver.default_show_exception_message = True
    driver.driver()
