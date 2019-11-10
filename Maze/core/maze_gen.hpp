#ifndef MAZE_GEN_HPP
#define MAZE_GEN_HPP

#include <random>
#include <vector>
#include "MazeGenerator.hpp"
#include "Maze.hpp"

struct coords
{
	int x;
	int y;
};

class SimpleMaze: public MazeGenerator
{
public:
	~SimpleMaze() = default;
	void generateMaze(Maze& maze);
	void generate_routes(Maze& maze, int current_x, int current_y);
	bool check_visited(const int& x, const int& y);
	
	std::vector<Direction> get_direct(const int& x, const int& y);
	Direction choose_direct(std::vector<Direction> d);
	
	
private:
	unsigned int row;
	unsigned int col;
	std::vector<coords> cell_list;
};

#endif
