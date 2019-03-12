#include "..\pch.h"
#include "ScenePlay.h"
#include "SceneResult.h"
#include "SceneTitle.h"

#include "..\\Utility\\ADX2\ADX2Le.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace MyLibrary;

ScenePlay::ScenePlay()
	: /*m_effectManager(nullptr),*/ m_dungeon(nullptr), m_gameTimer(nullptr),
	m_clearState(false), m_startState(false), m_gameOverState(false),
	m_gameTimerCD(100.0f), m_startCD(4.0f),
	m_positionOver(Vector2(55.0f, -350.0f)),
	m_posMagma{ Vector2::Zero }
{
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::Initialize(DX::DeviceResources* deviceResources, CommonStates* states)
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

	//ゲームタイマーの制作
	m_gameTimer = new Number(Vector2(320.0f, 180.0f), Vector2(10.0f, 10.0f));
	m_gameTimer->Initialize();
	m_gameTimer->Create(m_deviceResources, L"Resources\\Textures\\Number.png");


	//ダンジョンを作成する
	m_dungeon = new Dungeon();
	m_dungeon->Initialize(m_deviceResources, m_states);
	
	//プレイヤー初期化
	m_player = std::make_unique<Player>();
	m_player->Initialize(m_deviceResources, m_states);
	m_player->SetDungeon(m_dungeon);


	// テクスチャのロード
	CreateWICTextureFromFile(device, L"Resources\\Textures\\shadowbg1.png", nullptr, m_textureShadow.GetAddressOf());
	CreateWICTextureFromFile(device, L"Resources\\Textures\\GameClear.png", nullptr, m_textureGoal.GetAddressOf());
	CreateWICTextureFromFile(device, L"Resources\\Textures\\GameOver.png", nullptr, m_textureGameOver.GetAddressOf());
	//CreateWICTextureFromFile(device, L"Resources\\Textures\\GameTNTUI.png", nullptr, m_textureTNTUI.GetAddressOf());
	CreateWICTextureFromFile(device, L"Resources\\Textures\\PlayBG.jpg", nullptr, m_textureMagma.GetAddressOf());

	//音楽のロード
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"CueSheet_0.acb", L"CueSheet_0.awb");
	adx2le->Play(0);


	//m_effectManager = new EffectManager();
	//m_effectManager->Create(m_deviceResources, 1);
	//m_effectManager->Initialize(5, Vector3(0, 0, 0), Vector3(0, 0, 0));

	//m_effectManager->SetRenderState(m_player->playerPositionToCamera(), m_view, m_projection);
	//背景の位置を設定する
	m_posMagma[0] = Vector2(0, 0);
	m_posMagma[1] = Vector2(0, 1920);

}

SceneBase * ScenePlay::Update(float elapsedTime)
{
	float timer = elapsedTime;
	auto kb = Keyboard::Get().GetState();
	m_tracker.Update(kb);


	m_startCD -= timer;
	if (!m_startState)
	{
		m_gameTimer->SetNumber((int)m_startCD);
		m_gameTimer->Update();

	}

	if (m_startCD < 0)
	{
		m_startState = true;
	}

	//Vector3 playerPosBeta = m_player->GetPosition();
	//m_dungeon->PlayerPosition(Vector3 (playerPosBeta.x, playerPosBeta.y, playerPosBeta.z + 1));

	//ダンジョンの更新
	m_dungeon->Update(timer, m_startState);

	//ゲームの更新するかどうか
	if (m_startState)
	{
		//ゲームはまだクリアしてないか、ゲームオバーしてないかチェックします。
		if (!m_clearState &&
			!m_gameOverState)
		{
			//ゲームタイマー
			//float timer = elapsedTime;
			//m_gameTimerCD -= timer;


			//プレイヤーの更新
			m_player->Update(timer);

			if (m_player->GetCharaState())
			{
				float timer = elapsedTime;
				static float deadCD = 2;
				deadCD -= timer;

				if (deadCD < 0)
				{
					m_gameOverState = true;
				}
			}

			//潰れたら死にます
			if (m_dungeon->IDChecker(TileID::TILE_BLOCK1, m_player->GetPosition()) ||
				m_dungeon->IDChecker(TileID::TILE_BLOCK2, m_player->GetPosition()))
			{
				m_player->SetGameOverState();
			}

			//黄色のゴールたどり着いたらクリア
			if (m_dungeon->IsGoal(m_player->GetPosition()))
			{
				m_clearState = true;
			}

			//プレイヤーが落ちたら死にます
			if (m_player->GetPosition().y < -1.0f)
			{
				if (!m_player->GetCharaState())
				{
					m_player->SetGameOverState();
				}

				//float timer = elapsedTime;
				//static float deadCD = 2;
				//deadCD -= timer;

				//if (deadCD < 0)
				//{
				//	m_gameOverState = true;
				//}
			}

			//音更新
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Update();
		}
		else
		{
			if (m_positionOver.y < 50.0f)
			{
				m_positionOver.y += 3.5f;
			}

			if (kb.Space)
			{
				return new SceneTitle();
			}
			else
			{
				return nullptr;

			}


		}


	}

	//背景を動かせる処理=======
	m_posMagma[0].y--;
	m_posMagma[1].y--;

	if (m_posMagma[0].y < -1920)
	{
		m_posMagma[0].y = 0;
	}
	if (m_posMagma[1].y < 0)
	{
		m_posMagma[0].y = 1920;
	}
	//===========================


	return nullptr;
}

void ScenePlay::Render()
{
	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();

	//カメラの位置
	Vector3 eye = PlayerPositionToCamera();
	//注視点の位置
	Vector3 target = m_player->GetPosition();

	//ビュー行列の作成
	m_view = Matrix::CreateLookAt(eye, target, Vector3::Up);

	// スプライトの描画
	m_sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
	m_sprites->Draw(m_textureMagma.Get(), m_posMagma[0]);
	m_sprites->Draw(m_textureMagma.Get(), m_posMagma[1]);
	m_sprites->End();

	//プレイヤー描画
	m_player->Render(eye, m_view, m_projection);

	//ダンジョンの描画
	m_dungeon->Render(m_view, m_projection);

	// スプライトの描画


	// スプライトの描画
	m_sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
	//m_sprites->Draw(m_textureTNTUI.Get(), Vector2(40.0f, 10.0f));
	if (m_clearState)
	{
		m_sprites->Draw(m_textureShadow.Get(), Vector2::Zero);
		m_sprites->Draw(m_textureGoal.Get(), m_positionOver);
	}
	if (m_gameOverState)
	{
		m_sprites->Draw(m_textureShadow.Get(), Vector2::Zero);
		m_sprites->Draw(m_textureGameOver.Get(), m_positionOver);
	}
	m_sprites->End();

	if (!m_startState)
	{	
		m_sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
		m_sprites->Draw(m_textureShadow.Get(), Vector2::Zero);
		m_sprites->End();

		//タイマー描画
		m_gameTimer->Draw();
	}
}

void ScenePlay::Reset()
{
	// スプライトバッチの解放
	m_sprites.reset();
	m_sprites = nullptr;
	//m_spritesShadow.reset();
	//m_spritesShadow = nullptr;

	//プレイヤーの解放
	m_player.reset();
	m_player = nullptr;

	//タイマーの解放
	delete m_gameTimer;
	m_gameTimer = nullptr;

	//ダンジョンの解放
	delete m_dungeon;
	m_dungeon = nullptr;

	////
	//if (m_effectManager != nullptr) {
	//	m_effectManager->Lost();
	//	delete m_effectManager;
	//	m_effectManager = nullptr;
	//}

	//音の解放
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Stop();


}

DirectX::SimpleMath::Vector3 ScenePlay::PlayerPositionToCamera()
{

	return DirectX::SimpleMath::Vector3(m_player->GetPosition().x + 0.0f, m_player->GetPosition().y + 7.0f, m_player->GetPosition().z + 5.0f);
}



