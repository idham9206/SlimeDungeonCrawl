////====================================================////
////				�@�v���C���[�N���X					////
////													////
////				��ҁF���n�}�h�C�_��				////
////====================================================////
#pragma once
#include "..\..\Object\Obj2D.h"

// ���� -----------------------
enum Direction
{
	DIR_NONE,      // �����Ȃ�
	DIR_RIGHT,     // �E
	DIR_UP,        // �O
	DIR_LEFT,      // ��
	DIR_DOWN,      // ���
	DIR_JUMP,
	DIR_FALL
};

// �����x�N�g�� -----------------------------------------------------
static const DirectX::SimpleMath::Vector3 DIRECTION_VECTOR[] =
{
	DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
	DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f),
	DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f),
	DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f),
	DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f),
	//DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f),
	//DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f),

};


class Dungeon;

class Player
{
public:
	Player();
	~Player();

	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	void Reset();

	//������
	void Move();
	Direction PMove(Direction nextMovingDirection);
	void Climb();
	void Fall();

	//�Z�b�^�[�Q�b�^�[�֐��܂Ƃ�
	DirectX::SimpleMath::Vector3 GetPosition() { return m_player->GetPosition(); }
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_player->SetPosition(position); }
	void SetDungeon(Dungeon* dungeon) { m_dungeon = dungeon; }

private:
	//�f�o�C�X���\�[�V�Y�ւ̃|�C���^�[
	DX::DeviceResources* m_deviceResources;

	//�_���W�����ւ̃|�C���^�[
	Dungeon* m_dungeon;

	//�v���C���[�̃n���h��
	std::unique_ptr<Obj2D> m_player;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerTexture;

	Direction m_movingDirection;
	float m_speed;

};

