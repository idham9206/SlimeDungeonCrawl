#pragma once
#include "..\..\Utility\Dataload.h"
#include "..\..\Object\Obj3D.h"

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
public:
	static const int MAZE_WIDTH = 10;
	static const int MAZE_HEIGHT = 10;
	static const int MAZE_LENGTH = 10;
	static const float TILE_SIZE;


private:
	//外部からのデータ読み込みハンドル
	DataLoad* m_loader;
	//
	TileID m_data[MAZE_WIDTH][MAZE_HEIGHT][MAZE_LENGTH];

	// ブロック
	std::unique_ptr<Obj3D> m_block[MAZE_WIDTH][MAZE_HEIGHT][MAZE_LENGTH];

public:
	Dungeon();
	~Dungeon();
};

