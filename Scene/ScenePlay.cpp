#include "..\pch.h"
#include "ScenePlay.h"
#include "SceneResult.h"

#include "..\\Utility\\ADX2\ADX2Le.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

ScenePlay::ScenePlay()
	: /*m_effectManager(nullptr),*/ m_dungeon(nullptr), m_gameTimer(nullptr),
	m_clearState(false),
	m_gameTimerCD(10.0f)

{
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::Initialize(DX::DeviceResources* deviceResources, CommonStates* states)
{
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//��������ݒ肷��
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

	//�Q�[���^�C�}�[�̐���
	m_gameTimer = new Number(Vector2(420.0f, 10.0f), Vector2(2.0f, 2.0f));
	m_gameTimer->Initialize();
	m_gameTimer->Create(m_deviceResources, L"Resources\\Textures\\Number.png");


	//�_���W�������쐬����
	m_dungeon = new Dungeon();
	m_dungeon->Initialize(m_deviceResources, m_states);
	
	//�v���C���[������
	m_player = std::make_unique<Player>();
	m_player->Initialize(m_deviceResources, m_states);
	m_player->SetDungeon(m_dungeon);

	// �X�v���C�g�o�b�`�̍쐬
	m_spritesShadow = std::make_unique<SpriteBatch>(context);
	m_spriteGoal = std::make_unique<SpriteBatch>(context);


	// �e�N�X�`���̃��[�h
	CreateWICTextureFromFile(device, L"Resources\\Textures\\shadowbg1.png", nullptr, m_textureShadow.GetAddressOf());
	//
	CreateWICTextureFromFile(device, L"Resources\\Textures\\GameClear.png", nullptr, m_textureGoal.GetAddressOf());

	//���y�̃��[�h
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"CueSheet_0.acb", L"CueSheet_0.awb");
	adx2le->Play(0);


	//m_effectManager = new EffectManager();
	//m_effectManager->Create(m_deviceResources, 1);
	//m_effectManager->Initialize(5, Vector3(0, 0, 0), Vector3(0, 0, 0));

	//m_effectManager->SetRenderState(m_player->playerPositionToCamera(), m_view, m_projection);

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
	m_player->Update(time);

	if (m_dungeon->IsGoal(m_player->GetPosition()))
	{
		m_clearState = true;
	}


	//���X�V
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Update();

	//m_effectManager->Update(time);

	//if (m_gameTimerCD < 0)
	//{
	//	return new SceneResult();

	//}
	//else
	//{
	//	return nullptr;
	//}

	return nullptr;
}

void ScenePlay::Render()
{
	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();

	//�J�����̈ʒu
	Vector3 eye = PlayerPositionToCamera();
	//�����_�̈ʒu
	Vector3 target = m_player->GetPosition();

	//�r���[�s��̍쐬
	m_view = Matrix::CreateLookAt(eye, target, Vector3::Up);

	//�v���C���[�`��
	m_player->Render(eye, m_view, m_projection);

	//�_���W�����̕`��
	m_dungeon->Render(m_view, m_projection);

	// �X�v���C�g�̕`��
	m_spritesShadow->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
	m_spritesShadow->Draw(m_textureShadow.Get(), Vector2::Zero);
	m_spritesShadow->End();


	// �X�v���C�g�̕`��
	m_spriteGoal->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
	if (m_clearState)
	{
		m_spriteGoal->Draw(m_textureGoal.Get(), Vector2(50.0f, 30.0f));
	}
	m_spriteGoal->End();

	//�^�C�}�[�`��
	m_gameTimer->Draw();

	//m_effectManager->Render();




}

void ScenePlay::Reset()
{
	// �X�v���C�g�o�b�`�̉��
	m_sprites.reset();
	m_sprites = nullptr;
	m_spritesShadow.reset();
	m_spritesShadow = nullptr;
	m_spriteGoal.reset();
	m_spriteGoal = nullptr;

	//�v���C���[�̉��
	m_player.reset();
	m_player = nullptr;

	//�^�C�}�[�̉��
	delete m_gameTimer;
	m_gameTimer = nullptr;

	//�_���W�����̉��
	delete m_dungeon;
	m_dungeon = nullptr;

	////
	//if (m_effectManager != nullptr) {
	//	m_effectManager->Lost();
	//	delete m_effectManager;
	//	m_effectManager = nullptr;
	//}

	//���̉��
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Stop();


}

DirectX::SimpleMath::Vector3 ScenePlay::PlayerPositionToCamera()
{

	return DirectX::SimpleMath::Vector3(m_player->GetPosition().x + 0.0f, m_player->GetPosition().y + 5.0f, m_player->GetPosition().z + 5.0f);
}



