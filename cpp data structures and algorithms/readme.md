**project 1: a maze solver & a maze generator**
         * a maze generator that uses a recursive, depth-first algorithm to randomly generate a perfect maze of arbitrary size
         * a maze solver that uses a recursive, depth-first algorithm to traverse and solve a maze of arbitrary size, with the solution extending from the maze's starting cell to its ending cell without crossing any walls.
         * key files: core/msolver.cpp & core/mmaker.cpp
         
**project 2: othello game AI**
        * an AI that is capable of choosing moves in an Othello game, by using the recursive, search-tree-based algorithm described
        * the game code is given
        * Minimax algorithm and Alpha-beta pruning; determine move based on a score given to each possible outcome
        * Ai 2 searches 4 level down the search tree, while Ai 3 searches 5 level down ( a bit slower)
        * also implemented many heuristics for maximum advantage
        * key files: core/Ai2.cpp & core/Ai3.cpp
        
**project 3: datastructure for wordchecker tool**
        * two different data structure: hash map, AVL tree
        * supports indexing, adding (will rebalance avl if needed), removing (will rebalance avl if needed)
        * these data structures are template classes, used for strings in this project
        * stores tens of thousands of dictionary words for checking
        * coping optimization with copy elision
        * key files: core/AVLset.hpp & HashSet.hpp & WordChecker.cpp
  
**project 4: digraph data structure for gps
        * an implementation of directed graph used to store nodes and edges for a map for GPS
        * supports adding, removing, counting vertices and edges
        * checks for strongly connectivity using depth first traversal
        * uses Dijkstra's Shortest Path Algorithm to determine the shortest paths from the start vertex to every other vertex in the graph
        
