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

	TILE_ID
};


class Dungeon
{
public:
	static const int MAZE_WIDTH = 10;
	static const int MAZE_HEIGHT = 10;
	static const int MAZE_LENGTH = 10;
	static const float TILE_SIZE;


private:
	//���̃f�o�C�X���\�[�V�Y
	DX::DeviceResources* m_deviceResources;

	//���̃R�����X�e�[�c
	DirectX::CommonStates* m_states;

	//�O������̃f�[�^�ǂݍ��݃n���h��
	DataLoad* m_loader;
	//
	TileID m_data[MAZE_WIDTH][MAZE_HEIGHT][MAZE_LENGTH];

	// �u���b�N
	std::unique_ptr<Obj3D> m_block[MAZE_WIDTH][MAZE_HEIGHT][MAZE_LENGTH];
	std::unique_ptr<DirectX::Model> m_model[TILE_ID];


public:
	Dungeon();
	~Dungeon();

	//������
	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states);

	// �X�V
	void Update(float elapsedTime);

	// �`��
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix& projection);

	bool IsMovable(DirectX::SimpleMath::Vector3 position);

	bool IsGoal(DirectX::SimpleMath::Vector3 position);
};

