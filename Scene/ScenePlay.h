#pragma once
#include "SceneBase.h"

#include "..\Utility\Number.h"
#include "..\Object\Obj2D.h"
#include "..\Object\Obj3D.h"

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

	//
	bool IsMovable(DirectX::SimpleMath::Vector3 position);
	//
	bool IsGoal(DirectX::SimpleMath::Vector3 position);


private:
	//ゲーム内のタイマーハンドル
	float m_gameTimerCD;
	Number* m_gameTimer;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;

	//ダンジョンのハンドル
	Dungeon* m_dungeon;

	//プレイヤーのハンドル
	std::unique_ptr<Player> m_player;

	// ブラックアウトエフェクトスプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spritesShadow;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureShadow;

};