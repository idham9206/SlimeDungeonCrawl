////// 使えそうな関数まとめ
#pragma once
#include "..\pch.h"

//Z軸が0のまま
DirectX::SimpleMath::Vector3 Vec2toVec3(DirectX::SimpleMath::Vector2 vector2)
{
	return DirectX::SimpleMath::Vector3(vector2.x, vector2.y, 0.0f);
}


