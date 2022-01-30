#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int DIMX = 80;
int DIMY = 21;
//essentially arbitrary, just needs to be bigger than DIMX * DIMY
////essential for Dijkstra's
int INFINITY = 9999;

//useful for storing and printing the map
enum terrain {
	path,
	//calling this door instead of exit keyword
	door,
	wall,
	empty,
	forest,
	rocky,
	plains,
	desert,
	center,
	mart,
	tall_grass,
	clearing
};

//useful in implementing Dijkstra's
typedef struct point {
	int x;
	int y;
} point;

//used for testing
void print_point(point p)
{
	printf("%s", "Point: ");
	printf("%d", p.x);
	printf("%s", ", ");
	printf("%d", p.y);
	printf("\n");
}

void printMap(enum terrain map[DIMX][DIMY]);

//using Dijkstra's Algorithm to find a path between a and b
//and then drawing that path onto the map.
void drawPath(enum terrain map[DIMX][DIMY], point a, point b)
{
	int visited[DIMX][DIMY];
	int distance[DIMX][DIMY];

	int i;
	int j;
	
	//by default, set visited for all nodes to 0
	//default distance is INFINITY
	for (j = 0; j < DIMY; j++)
	{
		for (i = 0; i < DIMX; i++)
		{
			visited[i][j] = 0;
			distance[i][j] = INFINITY;
		}
	}

	//distance from a is 0
	distance[a.x][a.y] = 0;

	//returns the parent node of parent[i][j]
	point parent[DIMX][DIMY];

	//used for escape later on
	parent[a.x][a.y].x = -1;

	point current;
	int mindist;

	//the algorithm itself
	while (visited[b.x][b.y] == 0)
	{

		mindist = INFINITY;

		//first find the node with the smallest distance
		for (j = 0; j < DIMY; j++)
		{
			for (i = 0; i < DIMX; i++)
			{
				//ignore nodes we've already visited
				if (visited[i][j] == 1)
					continue;

				if (distance[i][j] < mindist)
				{
					current.x = i;
					current.y = j;
					mindist = distance[i][j];
				}				
			}
		}

		//this is the current node, so we have "visited" it
		visited[current.x][current.y] = 1;

		//checking each neighbor surrounding current
		for (j = current.y - 1; j <= current.y + 1; j++)
		{
			for (i = current.x - 1; i <= current.x + 1; i++)
			{
				//we can't go diagonal, so i and j can't be nonzero at the same time
				if (i != current.x && j != current.y)
					continue;

				//if the neighbor is a wall, continue.
				if (map[i][j] == wall)
					continue;
					
				//if we have already visited this node, continue.
				if (visited[i][j] == 1)
					continue;
				
				//if this node doesn't have a defined distance, set it.
				//also set the parent
				if (mindist + 1 < distance[i][j])
				{
					distance[i][j] = mindist + 1;
			       		parent[i][j] = current;
				}
			}
		}
	}

	i = parent[b.x][b.y].x;
	j = parent[b.x][b.y].y;

	//now that we've found point b, we draw a line from b to a
	//following the parent nodes
	//"while [i][j] has a parent" ends when we get to point a
	while (parent[i][j].x != -1)
	{
		//make [i][j] a path
		map[i][j] = path;
		//go to parent
		i = parent[i][j].x;
		j = parent[i][j].y;
	}
}

//used to generate Centers and Marts
int check_used(enum terrain map[DIMX][DIMY], int x, int y)
{
	enum terrain t = map[x][y];

	if (t == path || t == wall || t == mart || t == center || t == door)
		return 1;

	return 0;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	enum terrain map[DIMX][DIMY];

	//defining exit points
	point exit_top;
	//generating an index between 10 and 70 to make it feel more natural 
	exit_top.x = (rand() % 60) + 10;
	exit_top.y = 0;

	point exit_right;
	exit_right.x = DIMX - 1;
	//generating betweeen 3 and 18 for the same reason
	exit_right.y = (rand() % 15) + 3;

	point exit_bottom;
	exit_bottom.x = (rand() % 60) + 10;
	exit_bottom.y = DIMY - 1;

	point exit_left;
	exit_left.x = 0;
	exit_left.y = (rand() % 15) + 3;

	int i;

	//populating map with edges
	for (i = 0; i < DIMX; i++)
	{
		map[i][0] = wall;
		map[i][DIMY - 1] = wall;

		if (i < DIMY)
		{
			map[0][i] = wall;
			map[DIMX - 1][i] = wall;
		}
	}

	//inserting exit points into map
	map[exit_top.x][exit_top.y] = door;
	map[exit_right.x][exit_right.y] = door;
	map[exit_bottom.x][exit_bottom.y] = door;
	map[exit_left.x][exit_left.y] = door;

	int j;

	//necessary for populating the map later on
	for (j = 1; j < DIMY - 1; j++)
	{
		for (i = 1; i < DIMX - 1; i++)
		{
			map[i][j] = empty;
		}
	}

	//Generating a random point for paths to cross through
	point intersection;
	intersection.x = (rand() % 40) + 20;
	intersection.y = (rand() % 10) + 5;

	//drawing paths
	drawPath(map, exit_top, intersection);
	drawPath(map, intersection, exit_bottom);
	drawPath(map, intersection, exit_left);
	drawPath(map, intersection, exit_right);

	//correcting some specific cases
	map[intersection.x][intersection.y] = path;
	map[exit_top.x][1] = path;

	//now to generate different biomes for the distinct quadrants
	enum terrain quadrants[4];

	//stores a list of all biome terrains
	enum terrain biomes[4];
	biomes[0] = forest;
	biomes[1] = rocky;
	biomes[2] = plains;
	biomes[3] = desert;

	int temp;

	//two random quadrants get tall grass, the others get clearings
	temp = rand() % 4;
	quadrants[temp] = tall_grass;

	do
	{
		temp = rand() % 4;
	} while (quadrants[temp] == tall_grass);

	quadrants[temp] = tall_grass;

	for (i = 0; i < 4; i++)
	{
		if (quadrants[i] != tall_grass)
			quadrants[i] = clearing;
	}

	//doing quadrants left to right top to bottom
	//top left
	for (j = 1; j < DIMY - 2; j++)
	{
		for (i = 1; i < DIMX - 2; i++)
		{
			if (map[i][j] == path)
				break;

			if (map[i][j] == empty)
				map[i][j] = quadrants[0]; 
		}

		if (map[1][j] == path)
			break;
	}

	//top right
	for (j = 1; j < DIMY - 2; j++)
	{
		for (i = DIMX - 2; i > 0; i--)
		{
			if (map[i][j] == path)
				break;

			if (map[i][j] == empty)
				map[i][j] = quadrants[1];
		}
		
		if (map[DIMX - 2][j] == path)
			break;
	}

	//bottom left
	for (j = DIMY - 2; j > 0; j--)
	{
		for (i = 1; i < DIMX - 2; i++)
		{
			if (map[i][j] == path)
				break;

			if (map[i][j] == empty)
				map[i][j] = quadrants[2];
		}

		if (map[1][j] == path)
			break;
	}

	//bottom right
	for (j = DIMY - 2; j > 0; j--)
	{
		for (i = DIMX - 2; i > 0; i--)
		{
			if (map[i][j] == path)
				break;

			if (map[i][j] == empty)
				map[i][j] = quadrants[3];
		}

		if (map[DIMX - 2][j] == path)
			break;
	}

	//filling in remaining empties with a random biome
	temp = rand() % 4;

	for (j = 1; j < DIMY - 2; j++)
	{
		for (i = 1; i < DIMX - 2; i++)
		{
			if (map[i][j] == empty)
				map[i][j] = biomes[temp];
		}
	}

	//generating a random x for the mart
	temp = (rand() % 40) + 20;

	for (j = 1; j < DIMY - 2; j++)
	{
		if (map[temp][j] == path)
		{
			if (check_used(map, temp, j - 1) == 1 ||
			    check_used(map, temp, j - 2) == 1 ||
			    check_used(map, temp + 1, j - 1) == 1 ||
			    check_used(map, temp + 1, j - 2) == 1)
			{
				temp = (rand() % 76) + 2;
				j = 0;
				continue;
			}

			map[temp][j - 1] = mart;
			map[temp][j - 2] = mart;
			map[temp + 1][j - 1] = mart;
			map[temp + 1][j - 2] = mart;
			break;
		}
	}

	//generating a random y for the center
	temp = (rand() % 10) + 5;

	for (i = 1; i < DIMX - 2; i++)
	{
		if (map[i][temp] == path)
		{
			if (check_used(map, i - 1, temp) == 1 ||
			    check_used(map, i - 2, temp) == 1 ||
			    check_used(map, i - 1, temp + 1) == 1 ||
			    check_used(map, i - 2, temp + 1) == 1)
			{
				temp = (rand() % 17) + 2;
				i = 0;
				continue;
			}

			map[i - 1][temp] = center;
			map[i - 2][temp] = center;
			map[i - 1][temp + 1] = center;
			map[i - 2][temp + 1] = center;
			break;
		}
	}

	//finally, printing the map
	printMap(map);
	return 0;
}

//reads the map and prints corresponding symbols
void printMap(enum terrain map[DIMX][DIMY])
{
	int i;
	int j;

	//setting color to white by default
	printf("\033[0;37m");

	for (j = 0; j < DIMY; j++)
	{
		for (i = 0; i < DIMX; i++)
		{
			if (map[i][j] == wall)
				printf("%%");
	
			//prints in green
			else if (map[i][j] == tall_grass)
			{
				printf("\033[0;32m");
				printf("%s", ",");
			}		

			else if (map[i][j] == clearing)
				printf("%s", ".");
				
			else if (map[i][j] == door)
			{
				printf("\033[0;34m");
				printf("%s", "O");
			}
			
			//prints in cyan
			else if (map[i][j] == path)
			{
				printf("\033[0;34m");
				printf("%s", "#");
			}

			//prints in green
			else if (map[i][j] == forest)
			{
				printf("\033[0;32m");
				printf("%s", "^");
			}

			else if (map[i][j] == rocky)
				printf("%%");

			//prints in yellow
			else if (map[i][j] == plains)
			{
				printf("\033[1;33m");
				printf("%s", "/");
			}

			//prints in yellow
			else if (map[i][j] == desert)
			{
				printf("\033[1;33m");
				printf("%s", "~");
			}

			//prints in red
			else if (map[i][j] == center)
			{
				printf("\033[0;31m");
				printf("%s", "C");
			}

			//prints in red
			else if (map[i][j] == mart)
			{
				printf("\033[0;31m");
				printf("%s", "M");
			}

			else
				printf("%s", " ");

			//setting color back to white default
			printf("\033[0;37m");
		}
		
		printf("\n");
	}
}
