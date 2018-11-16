#include "..\pch.h"
#include "../Game.h"
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
	
	CreateWICTextureFromFile(device, L"Textures\\charatest(front).png",
		nullptr, m_texture.ReleaseAndGetAddressOf());


	m_player = std::make_unique<Obj2D>();
	m_player->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_player->Initialize(L"Resources\\Textures\\charatest(front).png", m_deviceResources, m_states);

	//// ���f�������[�h���ă��f���n���h�����擾����
	//m_model = Model::CreateFromCMO(device, L"Resources\\Models\\box.cmo", fx);

	m_obj3D->Initialize(L"Resources\\Models\\box.cmo", m_deviceResources, m_states);

}

SceneBase * ScenePlay::Update(float elapsedTime)
{
<<<<<<< HEAD
	//float time = elapsedTime;
	float gameTimer = m_gameTimerCD - elapsedTime;
	m_gameTimer->Update();
	m_gameTimer->SetNumber((int)gameTimer);
	m_player->Update(elapsedTime);

	m_obj3D->SetPosition(Vector3(5.0f, 0.0f, 0.0f));
	m_obj3D->Update(elapsedTime);
=======
	m_gameTimerCount -= (1.0f / 60.0f);
	m_gameTimer->Update();
	m_gameTimer->SetNumber((int)m_gameTimerCount);
	m_player->Update(elapsedTime);
>>>>>>> parent of 01b6ebb... 3DObject class fixing

	return nullptr;
}

void ScenePlay::Render()
{
	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();

	//Vector3 target = m_player->GetPosition();
	m_gameTimer->Draw();
	m_player->Render(m_eye, m_view, m_projection);
	
	// ���f���̕`��
	//m_model->Draw(context, *m_states, m_world, m_view, m_projection);

}

void ScenePlay::Reset()
{
	// �X�v���C�g�o�b�`�̉��
	m_sprites.reset();

	delete m_gameTimer;

}

DirectX::SimpleMath::Vector3 ScenePlay::playerPositionToCamera()
{

	return DirectX::SimpleMath::Vector3(m_player->GetPosition().x + 0.0f, m_player->GetPosition().y + 5.0f, m_player->GetPosition().z + 20.0f);
}
