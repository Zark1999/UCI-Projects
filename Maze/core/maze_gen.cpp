#include <ics46/factory/DynamicFactory.hpp>
#include "maze_gen.hpp"

ICS46_DYNAMIC_FACTORY_REGISTER(MazeGenerator,SimpleMaze,"SimpleMaze (Required)");

void SimpleMaze::generateMaze(Maze& maze)
{
	row = maze.getWidth();
	col = maze.getHeight();
	maze.addAllWalls();
	cell_list.push_back(coords{0,0});
	int current_x = 0; int current_y = 0;
	generate_routes(maze, current_x, current_y);
}

void SimpleMaze::generate_routes(Maze& maze, int current_x, int current_y)
{
	std::vector<Direction> d = get_direct(current_x,current_y);
	if (d.size() != 0)
	{
		Direction direction = choose_direct(d);
		maze.removeWall(current_x,current_y,direction);
		if (direction == Direction::up) {current_y = current_y-1;}
		if (direction == Direction::down) {current_y = current_y+1;}
		if (direction == Direction::left) {current_x = current_x-1;}
		if (direction == Direction::right) {current_x = current_x+1;}
		cell_list.push_back(coords{current_x,current_y});
		generate_routes(maze, current_x, current_y);
		generate_routes(maze, current_x, current_y);
	}
}


bool SimpleMaze::check_visited(const int& x, const int& y)
{
	for(unsigned int i = 0; i < cell_list.size(); i++)
	{
		if (cell_list[i].x == x and cell_list[i].y == y)
		{
			return true;
		}
	}
	return false;
}

std::vector<Direction> SimpleMaze::get_direct(const int& x, const int& y)
{
	std::vector<Direction> possible_direction;
	if (y > 0 and not check_visited(x,y-1))
	{
		possible_direction.push_back(Direction::up);
	}
	if (y < col-1 and not check_visited(x,y+1))
	{
		possible_direction.push_back(Direction::down);
	}
	if (x > 0 and not check_visited(x-1,y))
	{
		possible_direction.push_back(Direction::left);
	}
	if (x < row-1 and not check_visited(x+1,y))
	{
		possible_direction.push_back(Direction::right);
	}
	return possible_direction;
}

Direction SimpleMaze::choose_direct(std::vector<Direction> d)
{
	std::random_device device;
	std::default_random_engine engine{device()};
	std::uniform_int_distribution<int> distribution{0,(int)d.size()-1};
	return d[distribution(engine)];
}
