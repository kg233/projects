
import goody
from goody import irange
import prompt
from random import choice
from collections import defaultdict


# For use in read_corpus: leave unchanged in this file
def word_at_a_time(file : open):
    for line in file:
        for item in line.strip().split():
                yield item


def read_corpus(os : int, file : open) -> {(str):[str]}:
    reader, result = word_at_a_time(file), defaultdict(list)
    scan = [next(reader) for i in range(os)]
    for next_word in reader:
        if next_word not in result[tuple(scan)]: 
            result[tuple(scan)].append(next_word)
        scan.pop(0)
        scan.append(next_word)
    return result
            

def corpus_as_str(corpus : {(str):[str]}) -> str:
    result = ''.join(('  {} can be followed by any of {}\n'.format(key, corpus[key]) for key in sorted(corpus)))
    min_v = min(corpus.values(), key = (lambda x: len(x)))
    max_v = max(corpus.values(), key = (lambda x: len(x)))
    return result + 'min/max value lengths = {}/{}\n'.format(len(min_v), len(max_v))
    
def produce_text(corpus : {(str):[str]}, start : [str], count : int) -> [str]:
    result = start[:]
    for num in range(count):
        looking_key = result[-len(start):]
        if tuple(looking_key) not in corpus.keys():
            result.append(None)
            return result
        result.append(choice(corpus[tuple(looking_key)]))
    return result


if __name__ == '__main__':
    # Write script here
    os = prompt.for_int('Specify the order statistic to use', is_legal = (lambda x: x > 0), error_message = 'Enter a positive number')
    corpus = read_corpus(os, goody.safe_open('Specify the file name for processing', 'r', 'unable to read this file'))
    print('Corpus\n{}'.format(corpus_as_str(corpus)))
    start_list = [input('Specify word {}: '.format(i)) for i in range(1, os + 1)]
    gen_num = prompt.for_int('Specify # of words to generate', is_legal = (lambda x: x >= 0), error_message = 'Enter a non-negative number')
    print('Random text = {}\n'.format(produce_text(corpus, start_list, gen_num)))
    # For running batch self-tests
    print()
    import driver
    driver.default_file_name = "bsc5.txt"
#     driver.default_show_traceback = True
#     driver.default_show_exception = True
#     driver.default_show_exception_message = True
    driver.driver()
