#include "..\pch.h"
#include "ScenePlay.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
//using namespace MyLibrary;

ScenePlay::ScenePlay()
{
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::Initialize(DX::DeviceResources* deviceResources, CommonStates* states)
{
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	// ���f����ǂݍ���
	EffectFactory fx(device);

	device;

	// �R�����X�e�[�g�̍쐬
	m_states = states;

	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*


	// �X�v���C�g�o�b�`�̍쐬
	m_sprites = std::make_unique<SpriteBatch>(context);

	m_gameTimer = new Number(Vector2(420.0f, 10.0f), Vector2(2.0f, 2.0f));
	m_gameTimerCD = 100;
	m_gameTimer->Initialize();


	m_gameTimer->Create(m_deviceResources, L"Resources\\Textures\\Number.png");


	//�_���W�������쐬����
	m_dungeon = new Dungeon();
	m_dungeon->Initialize(m_deviceResources, m_states);
	
	//�v���C���[������
	m_player = std::make_unique<Player>();
	m_player->Initialize(m_deviceResources, m_states);
	m_player->SetDungeon(m_dungeon);

}

SceneBase * ScenePlay::Update(float elapsedTime)
{
	//�Q�[���^�C�}�[
	float time = elapsedTime;
	m_gameTimerCD -= time;
	m_gameTimer->Update();
	m_gameTimer->SetNumber((int)m_gameTimerCD);

	//�_���W�����̍X�V
	m_dungeon->Update(time);

	//�v���C���[�̍X�V
	m_player->Update(elapsedTime);

	return nullptr;
}

void ScenePlay::Render()
{
	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();

	//�J�����̈ʒu
	Vector3 eye = m_player->playerPositionToCamera();
	//�����_�̈ʒu
	Vector3 target = m_player->GetPosition();

	//�r���[�s��̍쐬
	m_view = Matrix::CreateLookAt(eye, target, Vector3::Up);

	
	m_dungeon->Render(m_view, m_projection);

	m_player->Render(eye, m_view, m_projection);

	m_gameTimer->Draw();

}

void ScenePlay::Reset()
{
	// �X�v���C�g�o�b�`�̉��
	m_sprites.reset();
	m_player.reset();
	

	delete m_gameTimer;
	m_gameTimer = nullptr;
	delete m_dungeon;
	m_dungeon = nullptr;

}

bool ScenePlay::IsMovable(DirectX::SimpleMath::Vector3 position)
{
	return m_dungeon->IsMovable(position);
}

bool ScenePlay::IsGoal(DirectX::SimpleMath::Vector3 position)
{
	return m_dungeon->IsGoal(position);
}

