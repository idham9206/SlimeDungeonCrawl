#pragma once
#include "..\..\Utility\Dataload.h"

enum TileID : unsigned char
{
	TILE_NONE,
	TILE_PASSAGE,
	TILE_BLOCK1,
	TILE_BLOCK2,
	TILE_GOAL,
	TILE_BOMB,
};

class Dungeon
{
private:
	DataLoad m_loader;

public:
	Dungeon();
	~Dungeon();
};

