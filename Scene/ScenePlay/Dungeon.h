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

	TILE_ID,

	//====================
	TILE_FALLINGBLOCKSTART = 10,
	TILE_FALLINGBLOCK1,

	TILE_FALLINGBLOCKCOUNT,


	TILE_START = 99,
};


class Dungeon
{
public:
	static const float TILE_SIZE;
	static const int MAZE_WIDTH = 10;
	static const int MAZE_HEIGHT = 10;
	static const int MAZE_LENGTH = 10;

	static const float BLOCK_SPEED;
	static const int BLOCK_MAXCOUNT = 3;
	static const int BLOCK_COOLDOWN = 10;

private:
	//���̃f�o�C�X���\�[�V�Y
	DX::DeviceResources* m_deviceResources;
	//������u���b�N�̉��ϐ�
	int m_dataAlpha = 0;

	//���̃R�����X�e�[�c
	DirectX::CommonStates* m_states;

	//�O������̃f�[�^�ǂݍ��݃n���h��
	DataLoad* m_loader;
	TileID m_data[MAZE_WIDTH][MAZE_HEIGHT][MAZE_LENGTH];
	// �u���b�N
	std::unique_ptr<Obj3D> m_block[MAZE_WIDTH][MAZE_HEIGHT][MAZE_LENGTH];
	std::unique_ptr<DirectX::Model> m_model[TILE_ID];

	//���v���C���[�ʒu
	DirectX::SimpleMath::Vector3 m_playerPos;

	//�u���b�N�������o���łȂ���΁A���̕ϐ��Ńt���b�O���Ă�
	int m_blockCD;
	int m_CD;

	//�e�̊֘A�n���h���@============================
	bool m_shadowFlag;
	//�e�̃e�N�X�`���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureShadow;
	//�e�̈ʒu
	DirectX::SimpleMath::Vector3 m_shadowPos;
	// �G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect> m_shadowBatchEffect;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_shadowBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_shadowInputLayout;
	//=============================================


	//������u���b�N�p�̕ϐ�
		//�����Ŏg���Ă�ꍇ
		Obj3D* m_blockAlpha;
		std::vector<DirectX::SimpleMath::Vector3> m_spawnPosAlpha;
		Obj3D* m_blockBeta[BLOCK_MAXCOUNT];
		DirectX::SimpleMath::Vector3 m_spawnPosBeta[BLOCK_MAXCOUNT];
		//������u���b�N�p�^�[��1
		DirectX::SimpleMath::Vector3 m_fallingBlockPos[5];
		

public:
	Dungeon();
	~Dungeon();

	//������
	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states);

	// �X�V
	void Update(float elapsedTime, bool startFlag);

	// �`��
	void Dungeon::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix & projection);

	//�Z�b�^�[�Q�b�^�[�֐��܂Ƃ�
	//�`�F�b�J�[�n
	bool IDChecker(TileID tileID, DirectX::SimpleMath::Vector3 position);

	bool IsMovable(DirectX::SimpleMath::Vector3 position);

	bool IsClimbing(DirectX::SimpleMath::Vector3 position);
	bool FallingDown(DirectX::SimpleMath::Vector3 position);

	bool IsGoal(DirectX::SimpleMath::Vector3 position);

	//�v���C���[�̃X�^�[�g�ʒu��Ԃ�
	DirectX::SimpleMath::Vector3 GetPlayerStartPosition()
	{
		return m_playerPos;
	}

};


