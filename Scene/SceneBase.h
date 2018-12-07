////====================================================////
////				ベースシーン　クラス				////
////													////
////				作者：ムハマドイダム				////
////====================================================////
#pragma once

#include "../DeviceResources.h"
#include <CommonStates.h>

class SceneBase
{
public:
	SceneBase();
	~SceneBase();

	virtual void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states) = 0;
	virtual SceneBase* Update(float elapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Reset() = 0;

	//===============================================================
	//セッターゲッターまとめ
	//ビュー
	void SetView(DirectX::SimpleMath::Matrix view) { m_view = view; }
	DirectX::SimpleMath::Matrix GetView() { return m_view; }

	//アイ
	void SetEye(DirectX::SimpleMath::Vector3 eye) { m_eye = eye; }
	DirectX::SimpleMath::Vector3 GetEye() { return m_eye; }

	//射影
	void SetProjection(DirectX::SimpleMath::Matrix projection) { m_projection = projection; }
	DirectX::SimpleMath::Matrix GetProjection() { return m_projection; }

	//ワールド
	void SetWorld(DirectX::SimpleMath::Matrix world) { m_world = world; }
	DirectX::SimpleMath::Matrix GetWorld() { return m_world; }


protected:
	//仮のデバイスリソーシズ
	DX::DeviceResources* m_deviceResources;
	//仮のコモンステーツ
	DirectX::CommonStates* m_states;

	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;

	//アイ行列
	DirectX::SimpleMath::Vector3 m_eye;

};

