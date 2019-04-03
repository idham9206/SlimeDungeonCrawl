////====================================================////
////		　ステージセレクトシーンのクラス	　		////
////													////
////				作者：ムハマドイダム				////
////====================================================////
#pragma once
#include "SceneBase.h"

#include "..\Utility\Number.h"
#include "..\Utility\Blink.h"

#include "..\Object\Obj2D.h"
#include "..\Object\Obj3D.h"

class SceneSelect : public SceneBase
{
public:
	SceneSelect();
	~SceneSelect();

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


	// ステージセレクトのスプライト
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureSelect;
	// 謝罪のスプライト
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureSazai;

	//点滅フラグ
	Blink* m_blink;
};