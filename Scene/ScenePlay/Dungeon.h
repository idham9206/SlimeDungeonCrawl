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
	TILE_TRAP1,

	TILE_ID,

	TILE_START = 99,
};


class Dungeon
{
public:
	static const float TILE_SIZE;


private:
	//仮のデバイスリソーシズ
	DX::DeviceResources* m_deviceResources;

	//仮のコモンステーツ
	DirectX::CommonStates* m_states;

	//外部からのデータ読み込みハンドル
	DataLoad* m_loader;
<<<<<<< HEAD

	int m_mapWidth;
	int m_mapHeight;
	int m_mapLength;

	TileID*** m_data;
=======
	//
	TileID m_data[MAZE_WIDTH][MAZE_HEIGHT][MAZE_LENGTH];
>>>>>>> parent of acc0c5c... daily commit

	// ブロック
	Obj3D*** m_block;
	std::unique_ptr<DirectX::Model> m_model[TILE_ID];




public:
	Dungeon();
	~Dungeon();

	//初期化
	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states);

	// 更新
	void Update(float elapsedTime);

	// 描画
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix& projection);

	bool IDChecker(TileID tileID, DirectX::SimpleMath::Vector3 position);

	bool IsMovable(DirectX::SimpleMath::Vector3 position);

	bool IsClimbing(DirectX::SimpleMath::Vector3 position);
	bool FallingDown(DirectX::SimpleMath::Vector3 position);

	bool IsGoal(DirectX::SimpleMath::Vector3 position);

};


