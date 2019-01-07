#include "..\pch.h"
#include "SceneTitle.h"
#include "ScenePlay.h"

#include "..\\Utility\\ADX2\ADX2Le.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

SceneTitle::SceneTitle():
	m_countdown(10.0f)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates * states)
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
	//
	CreateWICTextureFromFile(device, L"Resources\\Textures\\Title01.png", nullptr, m_texture.GetAddressOf());
}

SceneBase * SceneTitle::Update(float elapsedTime)
{
	float time = elapsedTime;
	m_countdown -= time;


	if (m_countdown < 0)
	{
		return new ScenePlay();

	}
	else
	{
		return nullptr;
	}
}

void SceneTitle::Render()
{

	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();
	// �X�v���C�g�̕`��
	m_sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
	m_sprites->Draw(m_texture.Get(), Vector2(35.0f, 30.0f));
	m_sprites->End();


}

void SceneTitle::Reset()
{
	// �X�v���C�g�o�b�`�̉��
	m_sprites.reset();
	m_sprites = nullptr;
}
