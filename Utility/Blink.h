//--------------------------------------------------------------------------------------
// File: Blink.h
//
// 点滅管理クラス
//
// Date: 2018.10.18
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once


class Blink
{
	// 点滅フラグ
	bool m_blinkFlag;

	// カウント
	int m_cnt;

	// 間隔
	int m_interval;

	// 停止フラグ
	bool m_stopFlag;

public:
	// コンストラクタ
	Blink();

	// 初期化関数
	void Initialize(int interval);

	// 更新関数
	bool Update(float elapsedTime);

	// 点滅状態の取得関数
	bool GetState();

	// リセット関数
	// flagには初期状態を渡してください
	void Reset(bool flag);

	// 点滅をスタートする関数
	void Start();

	// 点滅を停止する関数
	void Stop();
};
