#pragma once
#include "..\DeviceResources.h"

class Number
{
public:
	Number(DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2(1.f, 1.f));
	~Number();

public:
	// 開始処理
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	//作成
	void Create(DX::DeviceResources* deviceResources, const wchar_t* textureAddress);

	//セッター
	void SetNumber(int score);
	//ゲッター
	int GetNumber();


private:
	//仮のデバイスリソーシズ
	DX::DeviceResources* m_deviceResources;

	//定義変数
	static const int MAX_BOXCOUNT = 5; 	//数字桁の最大限
	static const int FRAME_WIDTH = 16; //１フレームの幅
	static const int FRAME_HEIGHT = 24;//１フレームの高さ

	//切り取り用の変数
	RECT m_rect[MAX_BOXCOUNT];
	//桁の番号
	int m_numberCountNum[MAX_BOXCOUNT];
	//スコア
	int m_numberCount;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;

	//表示の位値
	DirectX::SimpleMath::Vector2 m_position;
	//各数字の位値
	DirectX::SimpleMath::Vector2 m_positionNumber[MAX_BOXCOUNT];
	//拡大率
	DirectX::SimpleMath::Vector2 m_scale;
};

