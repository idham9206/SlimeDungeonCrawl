////====================================================////
////				　プレイヤークラス					////
////													////
////				作者：ムハマドイダム				////
////====================================================////
#pragma once
#include "..\..\Object\Obj2D.h"

// 方向 -----------------------
enum Direction
{
	DIR_NONE,      // 方向なし
	DIR_RIGHT,     // 右
	DIR_UP,        // 前
	DIR_LEFT,      // 左
	DIR_DOWN,      // 後ろ
	DIR_JUMP,
	DIR_FALL
};

// 方向ベクトル -----------------------------------------------------
static const DirectX::SimpleMath::Vector3 DIRECTION_VECTOR[] =
{
	DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
	DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f),
	DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f),
	DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f),
	DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f),
	//DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f),
	//DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f),

};


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

	//動き方
	void Move();
	Direction PMove(Direction nextMovingDirection);
	void Climb();
	void Fall();

	//セッターゲッター関数まとめ
	DirectX::SimpleMath::Vector3 GetPosition() { return m_player->GetPosition(); }
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_player->SetPosition(position); }
	void SetDungeon(Dungeon* dungeon) { m_dungeon = dungeon; }

private:
	//デバイスリソーシズへのポインター
	DX::DeviceResources* m_deviceResources;

	//ダンジョンへのポインター
	Dungeon* m_dungeon;

	//プレイヤーのハンドル
	std::unique_ptr<Obj2D> m_player;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerTexture;

	Direction m_movingDirection;
	float m_speed;

};

