#pragma once
#include "..\..\Object\Obj2D.h"

class Dungeon;

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

	//動き方
	void Move();

	DirectX::SimpleMath::Vector3 GetPosition() { return m_player->GetPosition(); }
	
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_player->SetPosition(position); }

	void SetDungeon(Dungeon* dungeon) { m_dungeon = dungeon; }

private:

	Dungeon* m_dungeon;

	//プレイヤーのハンドル
	std::unique_ptr<Obj2D> m_player;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerTexture;

	int m_CD;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;
};

