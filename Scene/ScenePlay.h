////====================================================////
////			　	プレイシーンのクラス	　			////
////													////
////				作者：ムハマドイダム				////
////====================================================////
#pragma once
#include "SceneBase.h"

#include "..\Utility\Number.h"
#include "..\Object\Obj2D.h"
#include "..\Object\Obj3D.h"

//#include "..\Effect\Myeffect.h"
//#include "..\Effect\EffectManager.h"

#include "ScenePlay\Dungeon.h"
#include "ScenePlay\Player.h"

class ScenePlay : public SceneBase
{
public:
	ScenePlay();
	~ScenePlay();

	//初期化
	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states) override;
	//更新
	SceneBase* Update(float elapsedTime) override;
	//描画
	void Render() override;
	//解放
	void Reset() override;

	DirectX::SimpleMath::Vector3 PlayerPositionToCamera();

private:
	//ゲーム内のタイマーハンドル
	float m_gameTimerCD;
	Number* m_gameTimer;

	//ダンジョンのデータ仮ハンドル
	const wchar_t * m_dungeonData;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;

	//ダンジョンのハンドル
	Dungeon* m_dungeon;

	//プレイヤーのハンドル
	std::unique_ptr<Player> m_player;

	// ブラックアウトエフェクトスプライトバッチ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureShadow;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureGoal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureGameOver;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureTNTUI;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureMagma;

	//キーボードの押したらの情報ハンドル
	DirectX::Keyboard::KeyboardStateTracker m_tracker;

	//マグマのポジション
	DirectX::SimpleMath::Vector2 m_posMagma[2];

	//
	DirectX::SimpleMath::Vector2 m_positionOver;

	//
	float m_startCD;

	//
	bool m_clearState;
	bool m_startState;
	bool m_gameOverState;

	//EffectManager*							m_effectManager;

public:
	//ダンジョンの作成
	void CreateDungeon(const wchar_t * data);


};