#pragma once
#include "..\..\Object\Obj2D.h"


class Player
{
public:
	Player();
	~Player();

	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	void Reset();

	//
	DirectX::SimpleMath::Vector3 playerPositionToCamera();

	DirectX::SimpleMath::Vector3 GetPosition() { return m_player->GetPosition(); }
private:
	//プレイヤーのハンドル
	std::unique_ptr<Obj2D> m_player;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerTexture;


};

