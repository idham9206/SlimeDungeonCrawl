#include "..\pch.h"
#include "SceneResult.h"
#include "SceneTitle.h"

#include "..\\Utility\\ADX2\ADX2Le.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

SceneResult::SceneResult():
	m_countdown(10.0f)
{
}

SceneResult::~SceneResult()
{
}

void SceneResult::Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates * states)
{
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//引数から設定する
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	// モデルを読み込む
	EffectFactory fx(device);

	device;

	// コモンステートの作成
	m_states = states;

	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

	// スプライトバッチの作成
	m_sprites = std::make_unique<SpriteBatch>(context);
	//
	CreateWICTextureFromFile(device, L"Resources\\Textures\\0.png", nullptr, m_texture.GetAddressOf());
}

SceneBase * SceneResult::Update(float elapsedTime)
{
	float time = elapsedTime;
	m_countdown -= time;


	if (m_countdown < 0)
	{
		return new SceneTitle();

	}
	else
	{
		return nullptr;
	}
}

void SceneResult::Render()
{

	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();
	// スプライトの描画
	m_sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
	m_sprites->Draw(m_texture.Get(), Vector2(35.0f, 30.0f));
	m_sprites->End();


}

void SceneResult::Reset()
{
	// スプライトバッチの解放
	m_sprites.reset();
	m_sprites = nullptr;

}
