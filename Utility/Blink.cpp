//--------------------------------------------------------------------------------------
// File: Blink.cpp
//
// “_–ÅŠÇ—ƒNƒ‰ƒX
//
// Date: 2018.10.18
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "..\pch.h"
#include "Blink.h"

Blink::Blink()
	: m_blinkFlag(true), m_cnt(0), m_interval(0), m_stopFlag(false)
{
}

void Blink::Initialize(int interval)
{
	m_interval = interval;
}

bool Blink::Update(float elapsedTime)
{
	if (m_stopFlag) return true;

	m_cnt++;
	if (m_cnt >= m_interval)
	{
		m_cnt = 0;
		m_blinkFlag = !m_blinkFlag;
	}
	return true;
}

bool Blink::GetState()
{
	return m_blinkFlag;
}

void Blink::Reset(bool flag)
{
	m_blinkFlag = flag;
	m_cnt = 0;
}

void Blink::Start()
{
	m_stopFlag = false;
}

void Blink::Stop()
{
	m_stopFlag = true;
}
