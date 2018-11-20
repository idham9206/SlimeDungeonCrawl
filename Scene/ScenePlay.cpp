#include "..\pch.h"
#include "ScenePlay.h"

#include "..\\Utility\\ADX2\ADX2Le.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

ScenePlay::ScenePlay()
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

	// �X�v���C�g�o�b�`�̍쐬
	m_spritesShadow = std::make_unique<SpriteBatch>(context);

	// �e�N�X�`���̃��[�h
	CreateWICTextureFromFile(device, L"Resources\\Textures\\shadowbg1.png", nullptr, m_textureShadow.GetAddressOf());

	//���y�̃��[�h
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Initialize(L"BGMTest.acf");
	adx2le->LoadAcb(L"CueSheet_0.acb", L"CueSheet_0.awb");
	adx2le->Play(0);

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

	//���X�V
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Update();

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

	//�v���C���[�`��
	m_player->Render(eye, m_view, m_projection);

	//�_���W�����̕`��
	m_dungeon->Render(m_view, m_projection);

	// �X�v���C�g�̕`��
	m_spritesShadow->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
	m_spritesShadow->Draw(m_textureShadow.Get(), Vector2::Zero);
	m_spritesShadow->End();

	//�^�C�}�[�`��
	m_gameTimer->Draw();

}

void ScenePlay::Reset()
{
	// �X�v���C�g�o�b�`�̉��
	m_sprites.reset();
	m_sprites = nullptr;
	m_spritesShadow.reset();
	m_spritesShadow = nullptr;

	//�v���C���[�̉��
	m_player.reset();
	m_player = nullptr;

	//�^�C�}�[�̉��
	delete m_gameTimer;
	m_gameTimer = nullptr;

	//�_���W�����̉��
	delete m_dungeon;
	m_dungeon = nullptr;

	//���̉��
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Finalize();


}

bool ScenePlay::IsMovable(DirectX::SimpleMath::Vector3 position)
{
	return m_dungeon->IsMovable(position);
}

bool ScenePlay::IsGoal(DirectX::SimpleMath::Vector3 position)
{
	return m_dungeon->IsGoal(position);
}

