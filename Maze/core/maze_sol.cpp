#include "maze_sol.hpp"
#include <ics46/factory/DynamicFactory.hpp>

ICS46_DYNAMIC_FACTORY_REGISTER(MazeSolver, SimpleMazeSolver, "SimpleMazeSolver (Required)");

void SimpleMazeSolver::solveMaze(const Maze& maze, MazeSolution& mazeSolution)
{
	cell_list.push_back(mazeSolution.getStartingCell());
	solve_routes(maze, mazeSolution);
}

void SimpleMazeSolver::solve_routes(const Maze& maze, MazeSolution& mazeSolution)
{
	if (not mazeSolution.isComplete())
	{
		int current_x = mazeSolution.getCurrentCell().first;
		int current_y = mazeSolution.getCurrentCell().second;
		std::vector<Direction> ways = get_ways(current_x,current_y,maze);
		if (ways.size() == 0)
		{
			mazeSolution.backUp();
			solve_routes(maze, mazeSolution);
		}
		else
		{
			mazeSolution.extend(ways[0]);
			cell_list.push_back(mazeSolution.getCurrentCell());
			solve_routes(maze, mazeSolution);
		}
	}
}

bool SimpleMazeSolver::check_visited(int x, int y)
{
	for (unsigned int i = 0; i < cell_list.size(); i++)
	{
		if (cell_list[i].first == x and cell_list[i].second == y)
		{
			return true;
		}
	}
	return false;
}

std::vector<Direction> SimpleMazeSolver::get_ways(int x, int y, const Maze& maze)
{
	std::vector<Direction> ways;
	if(y > 0 and not check_visited(x,y-1) and not maze.wallExists(x,y,Direction::up))
	{ways.push_back(Direction::up);}
	if(y < maze.getHeight()-1 and not check_visited(x,y+1) and not maze.wallExists(x,y,Direction::down))
	{ways.push_back(Direction::down);}
	if(x > 0 and not check_visited(x-1,y) and not maze.wallExists(x,y,Direction::left))
	{ways.push_back(Direction::left);}
	if(x < maze.getWidth()-1 and not check_visited(x+1,y) and not maze.wallExists(x,y,Direction::right))
	{ways.push_back(Direction::right);}
	return ways;
}
