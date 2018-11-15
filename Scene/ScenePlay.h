#pragma once
#include "SceneBase.h"
#include "..\Utility\Number.h"
#include "..\Object\Obj2D.h"
#include "..\Object\Obj3D.h"

class ScenePlay : public SceneBase
{
public:
	ScenePlay();
	~ScenePlay();

	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states) override;
	SceneBase* Update(float elapsedTime) override;
	void Render() override;
	void Reset() override;

private:
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	
	//ゲーム内のタイマーハンドル
	float m_gameTimerCount;
	Number* m_gameTimer;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;

	//// モデルハンドル
	//std::unique_ptr<DirectX::Model> m_model;

	//プレイヤーのハンドル
	std::unique_ptr<Obj2D> m_player;

	//
	std::unique_ptr<Obj3D> m_obj3D;


	//
	DirectX::SimpleMath::Vector3 playerPositionToCamera();

};