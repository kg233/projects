# Submitter: kuixir(Ren, Kuixi)
import goody
from collections import defaultdict
file = open('ndfaendin01.txt')

def read_ndfa(file : open) -> {str:{str:{str}}}:
    result = defaultdict(dict)
    for line in file:
        line_list = line.rstrip().split(';')
        result[line_list[0]]
        for key, value in zip(line_list[1::2], line_list[2::2]):
            result[line_list[0]].setdefault(key, set()).add(value)
    return dict(result)


def ndfa_as_str(ndfa : {str:{str:{str}}}) -> str:
    return ''.join(('  {} transitions: {}\n'.format(key, ll) for key in sorted(ndfa) for ll in [[(val, sorted(li)) for val, li in sorted(ndfa[key].items())]]))


def process(ndfa : {str:{str:{str}}}, state : str, inputs : [str]) -> [None]:
    result, current_set, new_set = [state], set(), set()
    current_set.add(state)
    for inp in inputs:
        new_set = set() 
        for current_state in current_set:
            if inp in ndfa[current_state]:
                new_set = new_set.union(ndfa[current_state][inp])
        current_set = new_set    
        result.append((inp, current_set))
        if current_set == set():
            return result
    return result


def interpret(result : [None]) -> str:
    ans = 'Start state = {}\n'.format(str(result[0]))
    for input, new_set in result[1:]:
        ans += '  Input = {}; new possible states = {}\n'.format(input, sorted(new_set))
    ans += 'Stop state(s) = {}\n'.format(sorted(result[-1][-1]))
    return ans


if __name__ == '__main__':
    # Write script here
    ndfa_dict = read_ndfa(goody.safe_open('Specify the file name storing the non-deterministic finite automaton', 'r', 'unable to read this file'))
    print('\nThe Description of the specified Non-Deterministic Finite Automaton\n' + ndfa_as_str(ndfa_dict))
    input_file = goody.safe_open('Specify the file name storing the start-state with its inputs', 'r', 'unable to read this file')
    print()
    for line in input_file:
        strip = line.rstrip().split(';')
        print('Begin to trace a new NDFA simulation\n' + interpret(process(ndfa_dict, strip.pop(0), strip)))
    # For running batch self-tests
    print()
    import driver
    driver.default_file_name = "bsc4.txt"
#     driver.default_show_traceback = True
#     driver.default_show_exception = True
#     driver.default_show_exception_message = True
    driver.driver()
