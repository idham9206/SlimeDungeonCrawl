#include "..\pch.h"
#include "SceneTitle.h"
#include "ScenePlay.h"

#include "..\\Utility\\ADX2\ADX2Le.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

SceneTitle::SceneTitle()
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates * states)
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
	CreateWICTextureFromFile(device, L"Resources\\Textures\\TitleLogo.png", nullptr, m_textureTitle.GetAddressOf());
	CreateWICTextureFromFile(device, L"Resources\\Textures\\TitleStart.png", nullptr, m_textureStart.GetAddressOf());
	CreateWICTextureFromFile(device, L"Resources\\Textures\\TitleBoard.png", nullptr, m_textureBoard.GetAddressOf());

	m_blink = new Blink();
	m_blink->Initialize(70);

}

SceneBase * SceneTitle::Update(float elapsedTime)
{
	float time = elapsedTime;

	auto kb = Keyboard::Get().GetState();
	m_tracker.Update(kb);

	m_blink->Update(time);

	if (kb.Space)
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

	// スプライトの描画
	m_sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
	m_sprites->Draw(m_textureBoard.Get(), Vector2(35.0f, -5.0f));
	m_sprites->Draw(m_textureTitle.Get(), Vector2(35.0f, 30.0f));
	if (m_blink->GetState())
	{
		m_sprites->Draw(m_textureStart.Get(), Vector2(160.0f, 416.0f));
	}
	m_sprites->End();


}

void SceneTitle::Reset()
{
	// スプライトバッチの解放
	m_sprites.reset();
	m_sprites = nullptr;
}
