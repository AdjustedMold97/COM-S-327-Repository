#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int DIMX = 80;
int DIMY = 21;
int INFINITY = 9999;

enum terrain {
	path,
	//calling this door instead of exit keyword
	door,
	wall,
	empty
};

typedef struct point {
	int x;
	int y;
	enum terrain t;
} point;

void printMap(enum terrain map[DIMX][DIMY]);

//using Dijkstra's Algorithm to find a path between a and b
//and then drawing that path onto the map.
void drawPath(enum terrain map[DIMX][DIMY], point a, point b)
{

	int visited[DIMX][DIMY];

	int i;
	int j;

	for (j = 0; j < DIMY; j++)
	{
		for (i = 0; i < DIMX; i++)
		{
			visited[i][j] = 0;
		}
	}

	//we have already visited the start point
	visited[a.x][a.y] = 1;

	//calculating the cost of all points from the starting position
	//essentially the "walking distance"
	int cost[DIMX][DIMY];

	int xcost;
	int ycost;

	for (j = 0; j < DIMY; j++)
	{

		if (a.y < j)
			ycost = j - a.y;

		else
			ycost = a.y - j;

		for (i = 0; i < DIMX; i++)
		{
			if (a.x < i)
				xcost = i - a.x;

			else
				xcost = a.x - i;
			
			cost[i][j] = xcost + ycost;
		}
	}

		

}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	enum terrain map[DIMX][DIMY];

	//defining exit points
	point exit_top;
	//generating an index between 1 and 78
	//so that the exit doesn't appear on corners 
	exit_top.x = (rand() % DIMX - 3) + 1;
	exit_top.y = 0;
	exit_top.t = door;

	point exit_right;
	exit_right.x = DIMX - 1;
	//generating betweeen 1 and 19 for the same reason
	exit_right.y = (rand() % DIMY - 3) + 1;
	exit_right.t = door;

	point exit_bottom;
	exit_bottom.x = (rand() % DIMX - 3) + 1;
	exit_bottom.y = DIMY - 1;
	exit_bottom.t = door;

	point exit_left;
	exit_left.x = 0;
	exit_left.y = (rand() % DIMY - 3) + 1;
	exit_left.t = door;

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
	map[exit_top.x][exit_top.y] = exit_top.t;
	map[exit_right.x][exit_right.y] = exit_right.t;
	map[exit_bottom.x][exit_bottom.y] = exit_bottom.t;
	map[exit_left.x][exit_left.y] = exit_left.t;

	//Helps prevent compilation errors
	int j;

	for (j = 1; j < DIMY - 1; j++)
	{
		for (i = 1; i < DIMX - 1; i++)
		{
			map[i][j] = empty;
		}
	}

	printMap(map);

	return 0;
}

void printMap(enum terrain map[DIMX][DIMY])
{
	int i;
	int j;

	for (j = 0; j < DIMY; j++)
	{
		for (i = 0; i < DIMX; i++)
		{
			if (map[i][j] == wall)
				printf("%s", "█");
				
			else if (map[i][j] == door)
				printf("%s", "E");

			else if (map[i][j] == empty)
				printf("%s", " ");
		}
		
		printf("\n");
	}
}
