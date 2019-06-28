#include <ics46/factory/DynamicFactory.hpp>
#include "mmaker.hpp"


ICS46_DYNAMIC_FACTORY_REGISTER(MazeGenerator, mmaker, "maze maker (Required)");

void mmaker::generateMaze(Maze& maze){
	std::random_device device;
	std::default_random_engine engine{device()};
	std::uniform_int_distribution<int> dist{0, 3};
	maze.addAllWalls();

	add_visited(0, 0);
	break_two(0,0, maze, engine);


}

void mmaker::add_visited(int x, int y){
	cell temp{x, y};
	if (not check_in_visited(x,y)){
		visited.push_back(temp);
	}
	

}


void mmaker::break_two(int x, int y, Maze& maze, std::default_random_engine engine){
	std::vector<Direction> validDir = getValidDir(x, y, maze);
	int new_x = x;
	int new_y = y;

	if (validDir.size() == 0){
		return;
	}

	Direction direct;

	if (validDir.size() == 1){
		direct = validDir[0];
	}

	else{
		int range = validDir.size();
		std::uniform_int_distribution<int> dist{0, range-1};
		direct = validDir[dist(engine)];
	}

	maze.removeWall(x, y, direct);

	switch (direct){
		case Direction::up: new_y -= 1; break;
		case Direction::down: new_y += 1; break;
		case Direction::left: new_x -= 1; break;
		case Direction::right: new_x += 1; break;
	}

	add_visited(new_x, new_y);
	break_two(new_x, new_y, maze, engine);

	break_two(x,y, maze, engine);

}


std::vector<Direction> mmaker::getValidDir(int x, int y, Maze& maze){
	std::vector<Direction> result;

	if (y != 0 and maze.wallExists(x ,y, Direction::up) and not check_in_visited(x, y-1)){
		result.push_back(Direction::up);
	}

	if (y != maze.getHeight()-1 and maze.wallExists(x ,y, Direction::down) and not check_in_visited(x, y+1)){
		result.push_back(Direction::down);
	}
	if (x != 0 and maze.wallExists(x ,y, Direction::left) and not check_in_visited(x-1, y)){
		result.push_back(Direction::left);
	}
	if (x != maze.getWidth()-1 and maze.wallExists(x ,y, Direction::right) and not check_in_visited(x+1, y)){
		result.push_back(Direction::right);
	}
	return result;
}

bool mmaker::check_in_visited(int x, int y){
	for (int i = 0; i < visited.size(); i++){
		if (visited[i].x == x and visited[i].y == y){
			return true;
		}
	}
	return false;

}

