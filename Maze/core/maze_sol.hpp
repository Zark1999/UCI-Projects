#ifndef MAZE_SOL_HPP
#define MAZE_SOL_HPP

#include "Maze.hpp"
#include "MazeSolution.hpp"
#include "MazeSolver.hpp"

class SimpleMazeSolver : public MazeSolver
{
public:
	~SimpleMazeSolver() = default;
	
	void solveMaze(const Maze& maze, MazeSolution& mazeSolution);
	void solve_routes(const Maze& maze, MazeSolution& mazeSolution);
	
	bool check_visited(int x, int y);
	std::vector<Direction> get_ways(int x, int y, const Maze& maze);
	
private:
	std::vector<std::pair<int,int>> cell_list;
};

#endif
