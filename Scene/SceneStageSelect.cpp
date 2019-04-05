#include "..\pch.h"
#include "SceneStageSelect.h"

#include "..\\Utility\\ADX2\ADX2Le.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

SceneSelect::SceneSelect() : m_scene(nullptr)
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates * states)
{
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//��������ݒ肷��
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	// ���f����ǂݍ���
	EffectFactory fx(device);

	//device;

	// �R�����X�e�[�g�̍쐬
	m_states = states;

	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

	// �X�v���C�g�o�b�`�̍쐬
	m_sprites = std::make_unique<SpriteBatch>(context);
	CreateWICTextureFromFile(device, L"Resources\\Textures\\StageSelect.png", nullptr, m_textureSelect.GetAddressOf()); //�X�e�[�W�Z���N�g�̃X�v���C�g
	CreateWICTextureFromFile(device, L"Resources\\Textures\\StageSelectSazai.png", nullptr, m_textureSazai.GetAddressOf()); //�Ӎ߂̃X�v���C�g

	//�_�Ńt���O��������
	m_blink = new Blink();
	m_blink->Initialize(30);

	//�V�[�����
	m_scene = new ScenePlay();

}

SceneBase * SceneSelect::Update(float elapsedTime)
{
	float time = elapsedTime;

	auto kb = Keyboard::Get().GetState();

	KeyTriggerFunction();

	//�_�Ńt���O�̍X�V
	m_blink->Update(time);

	//0�������ꂽ��
	if (keyCountNumber0 == 1)
	{
		m_scene->CreateDungeon(L"Stage00.csv");
		return m_scene;
	}
	//1�������ꂽ��
	if (keyCountNumber1 == 1)
	{
		m_scene->CreateDungeon(L"Stage01.csv");
		return m_scene;
	}

	return nullptr;
}

void SceneSelect::Render()
{
	
	m_deviceResources->PIXBeginEvent(L"Render");

	// �X�v���C�g�̕`��
	m_sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
	if (m_blink->GetState())
	{
		m_sprites->Draw(m_textureSelect.Get(), Vector2(30.0f, 216.0f));
	}
	m_sprites->Draw(m_textureSazai.Get(), Vector2(160.0f, 400.0f));

	m_sprites->End();
	

}

void SceneSelect::Reset()
{
	// �X�v���C�g�o�b�`�̉��
	m_sprites.reset();
	m_sprites = nullptr;


	//�_�Ńt���O�����
	if (m_blink != nullptr)
	{
		delete m_blink;
		m_blink = nullptr;

	}

}
