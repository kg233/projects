
import goody
from collections import defaultdict

def read_voter_preferences(file : open):
    result = dict()
    for line in file:
        pref_list = line.rstrip().split(';')
        result[pref_list.pop(0)] = pref_list
    return result

def dict_as_str(d : {None:None}, key : callable=None, reverse : bool=False) -> str:
    return '  ' + '  '.join((str(voter) + ' -> ' + str(d[voter]) + '\n' for voter in sorted(d.keys(), key = key, reverse = reverse)))


def evaluate_ballot(vp : {str:[str]}, cie : {str}) -> {str:int}:
    result = defaultdict(int)
    for voter in vp:
        for pref in vp[voter]:
            if pref in cie:
                result[pref] += 1
                break
    return dict(result)


def remaining_candidates(vd : {str:int}) -> {str}:
    min_val = min(vd.values())
    return {can for can in vd.keys() if vd[can] > min_val}


def run_election(vp_file : open) -> {str}:
    vote_dict = read_voter_preferences(vp_file)
    can_set, ballot = set(list(vote_dict.values())[0]), 1
    print('\nPreferences: name -> [candidates in order]\n' + dict_as_str(vote_dict))
    while len(can_set) > 1:
        vote_result = evaluate_ballot(vote_dict, can_set)
        print(f'Vote count on ballot #{ballot}: candidates (alphabetically sorted); using remaining candidate set = {can_set}\n{dict_as_str(vote_result)}')
        print(f'Vote count on ballot #{ballot}: candidates (numerically sorted); using remaining candidate set = {can_set}\n{dict_as_str(vote_result, key = (lambda x: vote_result[x]), reverse = True)}')
        can_set = remaining_candidates(vote_result)
        ballot += 1
    print(f'The winner is  {can_set}' if len(can_set) == 1 else f'Tie among remaining candidates: cannot choose a unique winner.')
    return can_set
    
  
  
    
if __name__ == '__main__':
    # Write script here
    file = goody.safe_open('Specify the file name storing the voter preferences', 'r', 'unable to read this file')
    run_election(file)          
    # For running batch self-tests
    print()
    import driver
    driver.default_file_name = "bsc2.txt"
#     driver.default_show_traceback = True
#     driver.default_show_exception = True
#     driver.default_show_exception_message = True
    driver.driver()
