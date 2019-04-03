#include "..\pch.h"
#include "SceneStageSelect.h"
#include "ScenePlay.h"

#include "..\\Utility\\ADX2\ADX2Le.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

SceneSelect::SceneSelect()
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates * states)
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
	CreateWICTextureFromFile(device, L"Resources\\Textures\\StageSelect.png", nullptr, m_textureSelect.GetAddressOf());
	CreateWICTextureFromFile(device, L"Resources\\Textures\\StageSelectSazai.png", nullptr, m_textureSazai.GetAddressOf());

	m_blink = new Blink();
	m_blink->Initialize(30);

}

SceneBase * SceneSelect::Update(float elapsedTime)
{
	float time = elapsedTime;

	auto kb = Keyboard::Get().GetState();

	KeyTriggerFunction();

	m_blink->Update(time);

	//シーン作る
	ScenePlay * scene = nullptr;
	if (scene == nullptr)
	{
		scene = new ScenePlay();
		if (keyCountNumber0 == 1)
		{
			scene->CreateDungeon(L"Stage00.csv");
			return scene;
		}
		if (keyCountNumber1 == 1)
		{
			scene->CreateDungeon(L"Stage01.csv");
			return scene;
		}

	}

	return nullptr;

}

void SceneSelect::Render()
{

	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();

	// スプライトの描画
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
	// スプライトバッチの解放
	m_sprites.reset();
	m_sprites = nullptr;

	//点滅フラグを解放
	delete m_blink;
	m_blink = nullptr;

}
