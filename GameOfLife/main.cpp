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
constexpr int kGridColumnsX{ 40 };
constexpr int kGridRowsY{ 40 };

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

		// Iterates through the grid
		for (int y = 0; y < kGridRowsY; y++)
		{
			for (int x = 0; x < kGridColumnsX; x++)
			{
				// Checks the current cell's neighbours
				int liveNeighbours = 0;
				for (int yCheck = -1; yCheck <= 1; yCheck++)
				{
					for (int xCheck = -1; xCheck <= 1; xCheck++)
					{
						if (xCheck != 0 || yCheck != 0)
						{
							int neighbourY = y + yCheck;
							int neighbourX = x + xCheck;
							if (neighbourY >= 0 && neighbourY < kGridRowsY && neighbourX >= 0 && neighbourX < kGridColumnsX)
							{
								// If their neighbour is alive; increments the variable
								if (displayMap[neighbourY][neighbourX] == 'C')
								{
									liveNeighbours++;
								}
							}
						}
					}
				}

				if (displayMap[y][x] == 'C') // Underpopulation and Overpopulation rules
				{
					if (liveNeighbours < 2 || liveNeighbours > 3)
					{
						logicMap[y][x] = '.';
					}
				}
				else if (displayMap[y][x] == '.') // Reproduction
				{
					if (liveNeighbours == 3)
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