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

			if (displayMap[y][x] == 'W')
			{
				ChangeColour(255, 255, 255);
			}
			else if (displayMap[y][x] == '.')
			{
				ChangeColour(0, 0, 0);
			}
			else if (displayMap[y][x] == 'L')
			{
				ChangeColour(162, 162, 162);
			}
			else if (displayMap[y][x] == 'R')
			{
				ChangeColour(255, 0, 0);
			}
			else if (displayMap[y][x] == 'G')
			{
				ChangeColour(0, 255, 0);
			}
			else if (displayMap[y][x] == 'B')
			{
				ChangeColour(0, 0, 255);
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

int aPointX = -1;
int bPointX = -1;
int aPointY = -1;
int bPointY = -1;

bool pointASet = false;
bool pointBSet = false;

char brushColour = 'W';
int brushSize = 1;

void drawLine()
{
	int mouseX, mouseY;
	GetMousePosition(mouseX, mouseY);
	
	if (mouseY / cellHeight < 34 && mouseX / cellWidth > -1 && mouseX / cellWidth < 40)
	{
		if (IsButtonPressed(EButton::eLeft)) // Checks for point A - for the line
		{
			aPointX = mouseX / cellWidth;
			aPointY = mouseY / cellHeight;

			pointASet = true;

			logicMap[aPointY][aPointX] = brushColour;
			if (brushSize == 2)
			{
				logicMap[aPointY - 1][aPointX] = brushColour;
				if (aPointX - 1 != -1)
				{
					logicMap[aPointY][aPointX - 1] = brushColour;
				}
				if (aPointX + 1 != 40)
				{
					logicMap[aPointY][aPointX + 1] = brushColour;
				}
				if (aPointY + 1 != 34)
				{
					logicMap[aPointY + 1][aPointX] = brushColour;
				}
			}
		}
		else if (pointASet && IsButtonPressed(EButton::eRight)) // Checks for point B - for the line
		{
			bPointX = mouseX / cellWidth;
			bPointY = mouseY / cellHeight;

			pointBSet = true;

			logicMap[bPointY][bPointX] = brushColour;
			if (brushSize == 2)
			{
				logicMap[bPointY - 1][bPointX] = brushColour;
				if (bPointX - 1 != -1)
				{
					logicMap[bPointY][bPointX - 1] = brushColour;
				}
				if (aPointX + 1 != 40)
				{
					logicMap[bPointY][bPointX + 1] = brushColour;
				}
				if (bPointY + 1 != 34)
				{
					logicMap[bPointY + 1][bPointX] = brushColour;
				}
			}
		}

		if (pointBSet)
		{
			int xDifference = abs(aPointX - bPointX);
			int yDifference = abs(aPointY - bPointY);
			int stepX = (aPointX < bPointX) ? 1 : -1;
			int stepY = (aPointY < bPointY) ? 1 : -1;
			int errorCheck = xDifference - yDifference;

			while (true)
			{
				logicMap[aPointY][aPointX] = brushColour;
				if (brushSize == 2)
				{
					logicMap[aPointY - 1][aPointX] = brushColour;
					if (aPointX - 1 != -1)
					{
						logicMap[aPointY][aPointX - 1] = brushColour;
					}
					if (aPointX + 1 != 40)
					{
						logicMap[aPointY][aPointX + 1] = brushColour;
					}
					if (aPointY + 1 != 34)
					{
						logicMap[aPointY + 1][aPointX] = brushColour;
					}
				}

				if (aPointX == bPointX && aPointY == bPointY)
				{
					break;
				}
				int errorDouble = 2 * errorCheck;
				if (errorDouble > -yDifference)
				{
					errorCheck -= yDifference;
					aPointX += stepX;
				}
				if (errorDouble < xDifference)
				{
					errorCheck += xDifference;
					aPointY += stepY;
				}
			}
			pointBSet = false;
		}
	}
	else
	{
		if (IsButtonPressed(EButton::eLeft) || IsButtonPressed(EButton::eRight))
		{
			if (mouseY / cellHeight >= 35 && mouseY / cellHeight <= 38)
			{
				if (mouseX / cellWidth >= 1 && mouseX / cellWidth <= 4)
				{
					brushColour = 'R';
					cout << "Red Paint" << endl;
				}
				else if (mouseX / cellWidth >= 6 && mouseX / cellWidth <= 9)
				{
					brushColour = 'G';
					cout << "Green Paint" << endl;
				}
				else if (mouseX / cellWidth >= 11 && mouseX / cellWidth <= 14)
				{
					brushColour = 'B';
					cout << "Blue Paint" << endl;
				}
				else if (mouseX / cellWidth >= 16 && mouseX / cellWidth <= 19)
				{
					brushColour = 'W';
					cout << "White Paint" << endl;
				}
				else if (mouseX / cellWidth >= 21 && mouseX / cellWidth <= 26)
				{
					brushSize = 1;
					cout << "Small Brush" << endl;
				}
				else if (mouseX / cellWidth >= 27 && mouseX / cellWidth <= 33)
				{
					brushSize = 2;
					cout << "Large Brush" << endl;
				}
				else if (mouseX / cellWidth >= 35 && mouseX / cellWidth <= 38)
				{
					brushColour = '.';
					pointASet = false;
					cout << "Eraser" << endl;
				}
			}
		}
	}
}

int main()
{
	bool IsButtonPressed(EButton whichButton);

	loadMap(fileName);

	memcpy(displayMap, logicMap, sizeof(bool) * kGridRowsY * kGridColumnsX);

	cout << "White Paint" << endl;
	cout << "Small Brush" << endl;

	while (UpdateFramework())
	{
		drawMap();

		drawLine();

		memcpy(displayMap, logicMap, sizeof(bool) * kGridRowsY * kGridColumnsX);
	}

	return 0;
}