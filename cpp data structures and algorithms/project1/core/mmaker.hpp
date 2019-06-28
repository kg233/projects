//mmaker.hpp

#ifndef MMAKER_HPP
#define MMAKER_HPP

#include "MazeGenerator.hpp"
#include <random>
#include <vector>
#include "Direction.hpp"
#include "Maze.hpp"
#include <algorithm>


class mmaker: public MazeGenerator{
public:
	virtual void generateMaze(Maze& maze);

	void add_visited(int x, int y); 

	std::vector<Direction> getValidDir(int x, int y, Maze& maze);

	bool check_in_visited(int x, int y);

	void break_two(int x, int y, Maze& maze, std::default_random_engine engine);

private:
	struct cell{
		int x;
		int y;
	};
	std::vector<cell> visited;

};


#endif