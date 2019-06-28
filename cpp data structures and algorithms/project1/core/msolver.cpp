//msolver.cpp

#include <ics46/factory/DynamicFactory.hpp>
#include "msolver.hpp"

ICS46_DYNAMIC_FACTORY_REGISTER(MazeSolver, msolver, "msolver (Required)");

void msolver::solveMaze(const Maze& maze, MazeSolution& mazeSolution){

	find_one(maze, mazeSolution);


}


int msolver::find_one(const Maze& maze, MazeSolution& mazeSolution){
	add_visited(mazeSolution.getCurrentCell());

	if (mazeSolution.getCurrentCell() == mazeSolution.getEndingCell()){
		return 1;
	}


	std::vector<Direction> directlist = getValidDir(maze, mazeSolution.getCurrentCell());

	if (mazeSolution.getCurrentCell() != mazeSolution.getStartingCell()){
		Direction lookup = mazeSolution.getMovements().back();
		for (int i = 0; i < directlist.size() ; i++){
			if (going_back(directlist[i], lookup)){
				directlist.erase(directlist.begin() + i);
			}
		}
	}
	if (directlist.size() == 0){
		mazeSolution.backUp();
		return 0;
	}



	std::pair<int, int> future = get_future_cell(mazeSolution.getCurrentCell(), directlist.back());
	while (in_visited(future)){

		if (directlist.size() == 1){
			mazeSolution.backUp();
			return 0;
		}
		else{
			directlist.pop_back();
			future = get_future_cell(mazeSolution.getCurrentCell(), directlist.back());
		}

	}
	
	mazeSolution.extend(directlist.back());
	directlist.pop_back();

	int code = find_one(maze,mazeSolution);
	if (code == 1){
		return 1;
	}

	return find_one(maze,mazeSolution);

}


std::vector<Direction> msolver::getValidDir(const Maze& maze, std::pair<int, int> current){
	std::vector<Direction> result;
	if (not maze.wallExists(current.first, current.second, Direction::up)){
		result.push_back(Direction::up);
	}

	if (not maze.wallExists(current.first, current.second, Direction::down)){
		result.push_back(Direction::down);
	}

	if (not maze.wallExists(current.first, current.second, Direction::left)){
		result.push_back(Direction::left);
	}

	if (not maze.wallExists(current.first, current.second, Direction::right)){
		result.push_back(Direction::right);
	}
	return result;
}

void msolver::add_visited(std::pair<int, int> targ){
	visited.push_back(targ);
}

bool msolver::going_back(Direction direct, Direction last){
	switch (direct){
		case Direction::up:
		return last == Direction::down;
		case Direction::down:
		return last == Direction::up;
		case Direction::left:
		return last == Direction::right;
		case Direction::right:
		return last == Direction::left;
		default:
		return false;
	}
}


bool msolver::in_visited(std::pair<int, int> targ){
	return std::find(visited.begin(), visited.end(), targ) != visited.end();
}

std::pair<int, int> msolver::get_future_cell(std::pair<int, int> comp, Direction direction){
	switch (direction){
		case Direction::up:
		comp.second -= 1; break;
		case Direction::down:
		comp.second += 1; break;
		case Direction::left:
		comp.first -= 1; break;
		case Direction::right:
		comp.first += 1; break;

	}

	return comp;
}