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

	//�I�u�W�F�N�g2D���擾����
	CreateWICTextureFromFile(device, L"Resources\\Textures\\charatest(front).png", nullptr, m_playerTexture.GetAddressOf());
	m_player = std::make_unique<Obj2D>();
	m_player->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_player->Initialize(m_deviceResources, m_states);
	m_player->SetTexture(m_playerTexture.Get());

	////�I�u�W�F�N�g3D���擾����
	//m_model = std::make_unique<Model>();
	//m_model = Model::CreateFromCMO(device, L"Resources\\Models\\box.cmo",fx);
	//m_obj3D = std::make_unique<Obj3D>();
	//m_obj3D->Initialize(m_deviceResources, m_states);
	//m_obj3D->SetModel(m_model.get());

	//�_���W�������쐬����
	m_dungeon = new Dungeon();
	m_dungeon->Initialize(m_deviceResources, m_states);
	
}

SceneBase * ScenePlay::Update(float elapsedTime)
{
	//�Q�[���^�C�}�[
	float time = elapsedTime;
	m_gameTimerCD -= time;
	m_gameTimer->Update();
	m_gameTimer->SetNumber((int)m_gameTimerCD);

	//�v���C���[�̍X�V
	m_player->Update(time);

	////�I�u�W�F�N�g3D�̍X�V
	//m_obj3D->SetPosition(Vector3(2.0f, 0.0f, 0.0f));
	//m_obj3D->Update(time);

	m_dungeon->Update(time);
	return nullptr;
}

void ScenePlay::Render()
{
	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_gameTimer->Draw();
	m_player->Render(m_eye, m_view, m_projection);
	

	//m_obj3D->Render(m_view, m_projection);

	m_dungeon->Render(m_view, m_projection);
}

void ScenePlay::Reset()
{
	// �X�v���C�g�o�b�`�̉��
	m_sprites.reset();
	m_player.reset();
	//m_obj3D.reset();

	delete m_gameTimer;
	m_gameTimer = nullptr;

}

DirectX::SimpleMath::Vector3 ScenePlay::playerPositionToCamera()
{

	return DirectX::SimpleMath::Vector3(m_player->GetPosition().x + 0.0f, m_player->GetPosition().y + 5.0f, m_player->GetPosition().z + 20.0f);
}
