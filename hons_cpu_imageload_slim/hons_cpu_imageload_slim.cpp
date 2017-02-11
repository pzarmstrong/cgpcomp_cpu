// hons_cpu_imageload_slim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "hons_cpu_imageload_slim.h"
#include "RunTimer.h"

using namespace std;

RunTimer rt;
RunTimer execute;

bool getPos(int x, int y)
{
	return pixBinaryMap[x + y*width];
}

int findHazard(int x, int y, int x_delta, int y_delta)
{
	int count = 0;
	do
	{
		x += x_delta;
		y += y_delta;
		count++;
	} while (0 <= x && x <= width && 0 <= y && y <= height && !getPos(x, y));
	return count - 1;
}

void algorithm()
{
	int x, y, sum = 0;
	int min_element;

	int directions[16];		// NW, N, NE, E, SE, S, SW, W
							// WNW, NNW, NNE, ENE, ESE, SSE, SSW, WSW

	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			directions[0] = findHazard(x, y, 0, -1);	// N

			directions[1] = findHazard(x, y, 1, -2);	// NNE
			directions[2] = findHazard(x, y, 1, -1);	// NE
			directions[3] = findHazard(x, y, 2, -1);	// ENE

			directions[4] = findHazard(x, y, 1, 0);		// E

			directions[5] = findHazard(x, y, 2, 1);		// ESE
			directions[6] = findHazard(x, y, 1, 1);		// SE
			directions[7] = findHazard(x, y, 1, 2);		// SSE

			directions[8] = findHazard(x, y, 0, 1);		// S

			directions[9] = findHazard(x, y, -1, 2);	// SSW
			directions[10] = findHazard(x, y, -1, 1);	// SW
			directions[11] = findHazard(x, y, -2, 1);	// WSW

			directions[12] = findHazard(x, y, -1, 0);	// W

			directions[13] = findHazard(x, y, -2, -1);	// WNW
			directions[14] = findHazard(x, y, -1, -1);	// NW
			directions[15] = findHazard(x, y, -1, -2);	// NNW

			min_element = directions[0];
			for (int a = 1; a < 16; a++)
			{
				if (directions[a] < min_element)
				{
					min_element = directions[a];
				}
				else
				{
					continue;
				}
			}

			pixHazardMap[x + y*width] = min_element * 5;
		}
	}
	cout << " ## Algorithm Completed Successfully!" << endl;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " <filename>\n" << endl;

		cout << " > Press [ENTER] to end the program..." << endl;
		getchar();

		return 1;
	}
	else
	{
		//
		execute.startTimer("EXECUTION TIMER");

		cout << "\n # HONS #CPU COMPONENT \n" << endl;

		//
		//rt.startTimer("Image Input");

		string filename = "images/";
		filename += argv[1];

		if (loadImage(filename))
		{

			//rt.endTimer("Image Input");

			//
			//rt.startTimer("Create Binary and Number of Hazard Maps");

			create_pixBinaryMap();
			pixHazardMap = new int[width*height];

			//rt.endTimer("Create Binary and Number of Hazard Maps");

			//
			rt.startTimer("Algorithm");

			cout << " # RUNNING ALGORITHM \n" << endl;
			algorithm();

			rt.endTimer("Algorithm");

			//
			//rt.startTimer("Image Output");

			cout << " # SAVING HAZARD IMAGE \n" << endl;
			saveHazardImage(filename);

			//rt.endTimer("Image Output");

			execute.endTimer("EXECUTION TIMER");

			cout << "\n > Press [ENTER] to end the program..." << endl;
			getchar();

			return 0;
		}
		else
		{
			cout << "\n > Press [ENTER] to end the program..." << endl;
			getchar();

			return 1;
		}
	}
}


bool loadImage(string filename)
{
	filename += ".ppm";
	string head, wid, ht, colrs;
	string r, g, b;
	ifstream in;

	in.open(filename);

	if (in.is_open())
	{
		cout << " - " << filename << " is open" << endl;

		getline(in, head);
		header = head;
		cout << " - magic header: " << head << '\n';

		getline(in, wid);
		width = atoi(wid.c_str());
		cout << " - width px: " << width;

		getline(in, ht);
		height = atoi(ht.c_str());
		cout << " height px: " << height << '\n';

		getline(in, colrs);
		colourRange = atoi(colrs.c_str());
		cout << " - colour range: " << colrs << '\n';

		pixArray = new pixVals[width*height];

		for (int j = 0; !in.eof() && j < width*height; j++)
		{
			getline(in, r);
			pixArray[j].R = atoi(r.c_str());
			getline(in, g);
			pixArray[j].G = atoi(g.c_str());
			getline(in, b);
			pixArray[j].B = atoi(b.c_str());
		}

		cout << endl;
		in.close();
		return true;
	}
	else
	{
		cerr << " ## ERROR ## Cannot open file: " << filename << "\n\n" << endl;
		return false;
	}
}

void create_pixBinaryMap()
{
	pixBinaryMap = new bool[width*height];

	for (int i = 0; i < width*height; i++)
	{
		if (pixArray[i].R == colourRange)
		{
			pixBinaryMap[i] = true;
		}
		else if (pixArray[i].G == colourRange)
		{
			pixBinaryMap[i] = false;
		}
	}
	cout << " ## Binary Map created!" << endl;
}

void saveHazardImage(string filename)
{
	filename += "_OUT.ppm";
	ofstream fout(filename);
	cout << " ## File output: " << filename << endl;

	fout << "P3\n";
	fout << width << "\n";
	fout << height << "\n";
	fout << "255\n";

	for (unsigned int i = 0; i < width*height; i++)
	{
		if (pixBinaryMap[i])
		{
			fout << "100" << "\n";
			fout << "0" << "\n";
			fout << "0" << "\n";
		}
		else if (pixHazardMap[i] >= 255)
		{
			fout << "255" << "\n";
			fout << "255" << "\n";
			fout << "255" << "\n";
		}
		else
		{
			fout << pixHazardMap[i] << "\n";
			fout << pixHazardMap[i] << "\n";
			fout << pixHazardMap[i] << "\n";
		}
	}

	fout << flush;
	fout.close();

	cout << "\n ## Output file saved!" << endl;
}


