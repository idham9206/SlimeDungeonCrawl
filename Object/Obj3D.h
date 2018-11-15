#pragma once

#include "../DeviceResources.h"

// モデルを表示するクラス
class Obj3D
{
protected:

	// モデルハンドル
	std::unique_ptr<DirectX::Model> m_model;

	//仮のデバイスリソーシズ
	DX::DeviceResources* m_deviceResources;
	
	//仮のコモンステーツ
	DirectX::CommonStates* m_states;


	// 位置
	DirectX::SimpleMath::Vector3 m_position;

	// 回転
	DirectX::SimpleMath::Quaternion m_rotation;

	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;

public:
	// コンストラクタ
	Obj3D();

	//初期化
	void Initialize(const wchar_t* textureAddress, DX::DeviceResources* deviceResources, DirectX::CommonStates* states);

	// 更新
	void Update(float elapsedTime);

	// 描画
	void Render(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix& projection);

	//// モデルハンドルを設定する関数
	//void SetModel(DirectX::Model* model) { m_model = model; }

	// 位置を設定する関数
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

	// 回転を設定する関数
	void SetRotation(DirectX::SimpleMath::Quaternion rotation) { m_rotation = rotation; }

};
