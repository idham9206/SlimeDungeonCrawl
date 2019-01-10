////====================================================////
////			　	プレイシーンのクラス	　			////
////													////
////				作者：ムハマドイダム				////
////====================================================////
#pragma once
#include "SceneBase.h"

#include "..\Utility\Number.h"
#include "..\Utility\Blink.h"

#include "..\Object\Obj2D.h"
#include "..\Object\Obj3D.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	~SceneTitle();

	//初期化
	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states) override;
	//更新
	SceneBase* Update(float elapsedTime) override;
	//描画
	void Render() override;
	//解放
	void Reset() override;


private:
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;
	std::unique_ptr<DirectX::SpriteBatch> m_spritesStart;


	// ブラックアウトエフェクトスプライトバッチ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureTitle;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureStart;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureBoard;

	//キーボードの押したらの情報ハンドル
	DirectX::Keyboard::KeyboardStateTracker m_tracker;

	//点滅フラグ
	Blink* m_blink;
};