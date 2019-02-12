////// g‚¦‚»‚¤‚ÈŠÖ”‚Ü‚Æ‚ß
#pragma once
#include "..\pch.h"

//Z²‚ª0‚Ì‚Ü‚Ü
DirectX::SimpleMath::Vector3 Vec2toVec3(DirectX::SimpleMath::Vector2 vector2)
{
	return DirectX::SimpleMath::Vector3(vector2.x, vector2.y, 0.0f);
}


