//msolver.hpp


#ifndef MSOLVER_HPP
#define MSOLVER_HPP

#include "MazeSolver.hpp"
#include "MazeSolution.hpp"
#include "Direction.hpp"
#include <iostream>
#include "Maze.hpp"
#include <algorithm>

class msolver: public MazeSolver{
public:

	virtual void solveMaze(const Maze& maze, MazeSolution& mazeSolution);

	int find_one(const Maze& maze, MazeSolution& mazeSolution);

	std::vector<Direction> getValidDir(const Maze& maze, std::pair<int, int> current);

	void add_visited(std::pair<int, int> targ);

	bool going_back(Direction direct, Direction last);

	bool in_visited(std::pair<int, int> targ);

	std::pair<int, int> get_future_cell(std::pair<int, int> comp, Direction direction);

private:
	std::vector<std::pair<int, int>> visited;

};



















#endif