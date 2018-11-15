#pragma once

#include "..\DeviceResources.h"

class Obj2D
{
private:
	// 位置
	DirectX::SimpleMath::Vector3 m_position;

	// 回転
	DirectX::SimpleMath::Quaternion m_rotation;

	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;

	DirectX::SimpleMath::Vector4 m_spriteFrame;

	float m_speed;

	int m_count;

	float m_frametime;

	const float FRAME_SIZE = 0.25f;

	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//仮のデバイスリソーシズ
	DX::DeviceResources* m_deviceResources;

	//仮のコモンステーツ
	DirectX::CommonStates* m_states;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// オブジェクト制作に必要なものまとめ
	void Create(DX::DeviceResources * deviceResources, DirectX::CommonStates* states, const wchar_t* textureAddress);

public:
	Obj2D();
	~Obj2D();

	//初期化
	void Initialize(const wchar_t* textureAddress, DX::DeviceResources * deviceResources, DirectX::CommonStates* states);

	// 更新
	void Update(float elapsedTime);

	// 描画
	void Render(DirectX::SimpleMath::Vector3 cameraEye, DirectX::SimpleMath::Matrix view,
		const DirectX::SimpleMath::Matrix& projection);



	void SetTexture(ID3D11ShaderResourceView* texture) { m_texture = texture; }
	// 位置を設定する関数
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

	// 回転を設定する関数
	void SetRotation(DirectX::SimpleMath::Quaternion rotation) { m_rotation = rotation; }

	void DrawSprite3D(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, 
		const DirectX::SimpleMath::Matrix& projection, DirectX::SimpleMath::Vector4& spriteFrame);
	//プレイヤーの位置を取得する関数
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
};

