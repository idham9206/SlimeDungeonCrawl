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

};

class Character : public Obj2D
{

};

