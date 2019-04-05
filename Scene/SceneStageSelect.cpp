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
	//引数から設定する
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	// モデルを読み込む
	EffectFactory fx(device);

	//device;

	// コモンステートの作成
	m_states = states;

	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

	// スプライトバッチの作成
	m_sprites = std::make_unique<SpriteBatch>(context);
	CreateWICTextureFromFile(device, L"Resources\\Textures\\StageSelect.png", nullptr, m_textureSelect.GetAddressOf()); //ステージセレクトのスプライト
	CreateWICTextureFromFile(device, L"Resources\\Textures\\StageSelectSazai.png", nullptr, m_textureSazai.GetAddressOf()); //謝罪のスプライト

	//点滅フラグを初期化
	m_blink = new Blink();
	m_blink->Initialize(30);

	//シーン作る
	m_scene = new ScenePlay();

}

SceneBase * SceneSelect::Update(float elapsedTime)
{
	float time = elapsedTime;

	auto kb = Keyboard::Get().GetState();

	KeyTriggerFunction();

	//点滅フラグの更新
	m_blink->Update(time);

	//0が押されたら
	if (keyCountNumber0 == 1)
	{
		m_scene->CreateDungeon(L"Stage00.csv");
		return m_scene;
	}
	//1が押されたら
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
	if (m_blink != nullptr)
	{
		delete m_blink;
		m_blink = nullptr;

	}

}
