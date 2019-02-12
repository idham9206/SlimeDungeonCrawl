#pragma once
#include "..\..\Object\Obj2D.h"

// ���� -----------------------
enum Direction
{
	DIR_NONE,      // �����Ȃ�
	DIR_RIGHT,     // �E
	DIR_UP,        // �O
	DIR_LEFT,      // ��
	DIR_DOWN,      // ���
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
// �����x�N�g�� -----------------------------------------------------
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

