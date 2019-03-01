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
	//DIR_JUMP,
	//DIR_FALL
};
enum State
{
	NONE,
	IDLE_FRONT,
	IDLE_BACK,
	MOVE_FRONT,
	MOVE_BACK,
	HIT_FRONT,
	HIT_BACK
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
	void AMove();
	Direction PMove(Direction nextMovingDirection);
	void Climb();
	void Fall();

	//セッターゲッター関数まとめ
	DirectX::SimpleMath::Vector3 GetPosition() { return m_player->GetPosition(); }
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_player->SetPosition(position); }
	void SetDungeon(Dungeon* dungeon) { m_dungeon = dungeon; }
	void SetCharaDeadState() { m_isDead = true; }
	void SetGameOverState() { m_isOver = true; }
	bool GetCharaState() { return m_isDead; }

private:
	//デバイスリソーシズへのポインター
	DX::DeviceResources* m_deviceResources;

	//ダンジョンへのポインター
	Dungeon* m_dungeon;

	//プレイヤーのハンドル
	std::unique_ptr<Obj2D> m_player; //プレイヤー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerTexture[6]; //テクスチャー
	DirectX::SimpleMath::Vector3 m_position; //
	Direction m_movingDirection; //
	State m_charaState; //

	DirectX::Keyboard::KeyboardStateTracker tracker;


	float m_speed;

	bool m_isDead;
	bool m_isOver;
	bool m_isMove;
	bool m_isFront;

};

