#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>

#include "Framework.h"
#include <fstream>
#include <string>
using namespace std;

// Screen dimensions
int gScreenWidth{ 800 };
int gScreenHeight{ 600 };

// Delay to slow things down
int gTimeDelayMS{ 100 };

// Maze size as constants
constexpr int kGridColumnsX{ 20 };
constexpr int kGridRowsY{ 20 };

char logicMap[kGridRowsY][kGridColumnsX];
char displayMap[kGridRowsY][kGridColumnsX];

ofstream output;
ifstream input;
string fileName = "blankMap";

void drawMap()
{
	for (int y = 0; y < kGridRowsY; y++)
	{
		for (int x = 0; x < kGridColumnsX; x++)
		{
			int cellWidth = gScreenWidth / kGridColumnsX;
			int cellHeight = gScreenHeight / kGridRowsY;
			int cellPosX = x * cellWidth;
			int cellPosY = y * cellHeight;

			if (displayMap[y][x] == 'C')
			{
				ChangeColour(255, 255, 255);
			}
			else if (displayMap[y][x] == '.')
			{
				ChangeColour(0, 0, 0);
			}

			DrawRectangle(cellPosX, cellPosY, cellWidth, cellHeight);
		}
	}
}

void loadMap(string fileName)
{
	input.open(fileName + ".txt");

	// Iterates through the file outputting the grid
	for (int y = 0; y < kGridRowsY; y++)
	{
		for (int x = 0; x < kGridColumnsX; x++)
		{
			input >> logicMap[y][x];
		}
		output << endl;
	}

	input.close();

	if (!input)
	{
		cerr << "file could not be opened" << endl;
	}
}

int main()
{
	loadMap(fileName);

	memcpy(displayMap, logicMap, sizeof(bool) * kGridRowsY * kGridColumnsX);

	while (UpdateFramework())
	{
		drawMap();

		for (int y = 0; y < kGridRowsY; y++)
		{
			for (int x = 0; x < kGridColumnsX; x++)
			{
				int liveNeighbours = 0;
				if (logicMap[y][x] == 'C')
				{
					if (logicMap[y + 1][x - 1] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y + 1][x] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y + 1][x + 1] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y][x - 1] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y][x + 1] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y - 1][x - 1] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y - 1][x] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y - 1][x + 1] == 'C')
					{
						liveNeighbours++;
					}

					if (liveNeighbours < 2) // Underpopulation
					{
						logicMap[y][x] = '.';
					}
					else if (liveNeighbours > 3) // Overpopulation
					{
						logicMap[y][x] = '.';
					}
				}
				else if (logicMap[y][x] == '.')
				{
					if (logicMap[y + 1][x - 1] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y + 1][x] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y + 1][x + 1] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y][x - 1] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y][x + 1] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y - 1][x - 1] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y - 1][x] == 'C')
					{
						liveNeighbours++;
					}
					if (logicMap[y - 1][x + 1] == 'C')
					{
						liveNeighbours++;
					}

					if (liveNeighbours == 3) // Reproduction
					{
						logicMap[y][x] = 'C';
					}
				}
			}
		}

		memcpy(displayMap, logicMap, sizeof(bool) * kGridRowsY * kGridColumnsX);
	}

	return 0;
}