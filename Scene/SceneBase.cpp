#include "..\pch.h"
#include "SceneBase.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SceneBase::SceneBase()
	: keyCountSpace(0), keyCountUp(0), keyCountDown(0), keyCountLeft(0), keyCountRight(0)
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::KeyTriggerFunction()
{
	auto kb = Keyboard::Get().GetState();

	if (kb.Space)
	{
		keyCountSpace++;
	}
	else
	{
		keyCountSpace = 0;
	}

	if (kb.Up)
	{
		keyCountUp++;
	}
	else
	{
		keyCountUp = 0;
	}
	if (kb.Down)
	{
		keyCountDown++;
	}
	else
	{
		keyCountDown = 0;
	}
	if (kb.Left)
	{
		keyCountLeft++;
	}
	else
	{
		keyCountLeft = 0;
	}
	if (kb.Right)
	{
		keyCountRight++;
	}
	else
	{
		keyCountRight = 0;
	}
	if (kb.NumPad0 || kb.D0)
	{
		keyCountNumber0++;
	}
	else
	{
		keyCountNumber0 = 0;
	}
	if (kb.NumPad1 || kb.D1)
	{
		keyCountNumber1++;
	}
	else
	{
		keyCountNumber1 = 0;
	}

}
