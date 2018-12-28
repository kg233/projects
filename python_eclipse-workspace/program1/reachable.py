
import goody
import prompt
from collections import defaultdict

def read_graph(file : open) -> {str:{str}}:
    result = defaultdict(set)
    for line in file:
        key, value = tuple(line.rstrip().split(';'))
        result[key].add(value)
    return dict(result)
    
 
def graph_as_str(graph : {str:{str}}) -> str:
    return '  ' + '\n  '.join((src + ' -> ' + str(sorted(list(dest))) for src, dest in sorted(graph.items()))) + '\n'

         
def reachable(graph : {str:{str}}, start : str, trace : bool = False) -> {str}:
    reached_set = set()
    exploring_list = [start]
    while len(exploring_list) != 0:
        if trace:
            print('\nreached set    = ' + str(reached_set) + "\nexploring list = " + str(exploring_list) + '\n', end = '')
        target = exploring_list.pop()
        if target not in reached_set:
            reached_set.add(target)
        exploring_list += [item for item in list(graph.get(target, [])) if item not in reached_set]
        if trace:
            print('removing node from exploring and adding to reached: node = ' + target + \
                  '\nafter adding all nodes reachable directly from g but not already in reached, exploring = ' + str(exploring_list) + '\n')
    return reached_set

 
if __name__ == '__main__':
    # Write script here
    file = goody.safe_open('Specify the file name storing the graph', 'r', 'unable to read this file')
    graph = read_graph(file)
    file.close()
    print('\nGraph: node -> [all its destination nodes]\n' + graph_as_str(graph))
    while True:
        node = prompt.for_string('Specify the start node (or just enter quit)', is_legal = (lambda x: x in graph or x == 'quit'), error_message = 'Illegal: not a source node')
        if node == 'quit':
            break
        do_trace = prompt.for_bool('Trace this Algorithm', default = True, error_message = 'please enter a boolean value')
        print('From ' + node + ' the reachable nodes are ' + str(reachable(graph, node, trace = do_trace)) + '\n')
    # For running batch self-tests
    print()
    import driver
    driver.default_file_name = "bsc1.txt"
#     driver.default_show_traceback = True
#     driver.default_show_exception = True
#     driver.default_show_exception_message = True
    driver.driver()
