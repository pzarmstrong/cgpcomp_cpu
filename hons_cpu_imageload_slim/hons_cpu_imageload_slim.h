#pragma once

#include <string>

struct pixVals
{
	unsigned int R, G, B;
};

std::string header;
int width, height, colourRange;

pixVals *pixArray;

bool* pixBinaryMap;
int* pixHazardMap;

bool getPos(int, int);
int findHazard(int, int, int, int);
void algorithm();

bool loadImage(std::string);
void create_pixBinaryMap();
void saveHazardImage(std::string);