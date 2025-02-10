#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>

#include "Framework.h"
#include <fstream>
#include <string>
#include <cmath>
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

int cellWidth = gScreenWidth / kGridColumnsX;
int cellHeight = gScreenHeight / kGridRowsY;

void drawMap()
{
	for (int y = 0; y < kGridRowsY; y++)
	{
		for (int x = 0; x < kGridColumnsX; x++)
		{
			cellWidth = gScreenWidth / kGridColumnsX;
			cellHeight = gScreenHeight / kGridRowsY;
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

int aPointX = 0;
int bPointX = 0;
int aPointY = 0;
int bPointY = 0;
void drawLine()
{
	void GetMousePosition(int& x, int& y);
	int mouseX, mouseY;
	GetMousePosition(mouseX, mouseY);
	
	if (IsButtonPressed(EButton::eLeft)) // Checks for point A - for the line
	{
		aPointX = mouseX / cellWidth;
		aPointY = mouseY / cellHeight;

		logicMap[aPointY][aPointX] = 'C';
	}
	else if (IsButtonPressed(EButton::eRight)) // Checks for point B - for the line
	{
		bPointX = mouseX / cellWidth;
		bPointY = mouseY / cellHeight;

		logicMap[bPointY][bPointX] = 'C';
	}
	int xDifference = aPointX - bPointX;
	int yDifference = aPointY - bPointY;

	int decision = 2 * yDifference - xDifference;

	for (int i = aPointX; i < bPointX; i++)
	{
		logicMap[aPointY][aPointX] = 'C';

		aPointX += 1;

		if (decision > 0)
		{
			aPointY += 1;
			decision += 2 * (yDifference - xDifference);
		}
		else
		{
			decision += 2 * yDifference;
		}
	}
}

int main()
{
	bool IsButtonPressed(EButton whichButton);

	loadMap(fileName);

	memcpy(displayMap, logicMap, sizeof(bool) * kGridRowsY * kGridColumnsX);

	while (UpdateFramework())
	{
		drawMap();

		drawLine();

		memcpy(displayMap, logicMap, sizeof(bool) * kGridRowsY * kGridColumnsX);
	}

	return 0;
}