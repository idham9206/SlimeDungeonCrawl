#pragma once
#include "..\..\Utility\Dataload.h"
#include "..\..\Object\Obj3D.h"

enum TileID : unsigned char
{
	TILE_NONE,
	TILE_PASSAGE,
	TILE_BLOCK1,
	TILE_BLOCK2,
	TILE_GOAL,

	TILE_ID,

	//====================
	TILE_FALLINGBLOCKSTART = 10,
	TILE_FALLINGBLOCK1,

	TILE_FALLINGBLOCKCOUNT,


	TILE_START = 99,
};


class Dungeon
{
public:
	static const float TILE_SIZE;
	static const int MAZE_WIDTH = 10;
	static const int MAZE_HEIGHT = 10;
	static const int MAZE_LENGTH = 10;

	static const float BLOCK_SPEED;
	static const int BLOCK_MAXCOUNT = 3;
	static const int BLOCK_COOLDOWN = 10;

private:
	//仮のデバイスリソーシズ
	DX::DeviceResources* m_deviceResources;
	//落ちるブロックの仮変数
	int m_dataAlpha = 0;

	//仮のコモンステーツ
	DirectX::CommonStates* m_states;

	//外部からのデータ読み込みハンドル
	DataLoad* m_loader;
	TileID m_data[MAZE_WIDTH][MAZE_HEIGHT][MAZE_LENGTH];
	// ブロック
	std::unique_ptr<Obj3D> m_block[MAZE_WIDTH][MAZE_HEIGHT][MAZE_LENGTH];
	std::unique_ptr<DirectX::Model> m_model[TILE_ID];

	//仮プレイヤー位置
	DirectX::SimpleMath::Vector3 m_playerPos;

	//ブロックがすぐ出すでなければ、この変数でフラッグ立てる
	int m_blockCD;
	int m_CD;

	//影の関連ハンドル　============================
	bool m_shadowFlag;
	//影のテクスチャー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureShadow;
	//影の位置
	DirectX::SimpleMath::Vector3 m_shadowPos;
	// エフェクト
	std::unique_ptr<DirectX::AlphaTestEffect> m_shadowBatchEffect;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_shadowBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_shadowInputLayout;
	//=============================================


	//落ちるブロック用の変数
		//乱数で使ってる場合
		Obj3D* m_blockAlpha;
		std::vector<DirectX::SimpleMath::Vector3> m_spawnPosAlpha;
		Obj3D* m_blockBeta[BLOCK_MAXCOUNT];
		DirectX::SimpleMath::Vector3 m_spawnPosBeta[BLOCK_MAXCOUNT];
		//落ちるブロックパターン1
		DirectX::SimpleMath::Vector3 m_fallingBlockPos[5];
		

public:
	Dungeon();
	~Dungeon();

	//初期化
	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states);

	// 更新
	void Update(float elapsedTime, bool startFlag);

	// 描画
	void Dungeon::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix & projection);

	//セッターゲッター関数まとめ
	//チェッカー系
	bool IDChecker(TileID tileID, DirectX::SimpleMath::Vector3 position);

	bool IsMovable(DirectX::SimpleMath::Vector3 position);

	bool IsClimbing(DirectX::SimpleMath::Vector3 position);
	bool FallingDown(DirectX::SimpleMath::Vector3 position);

	bool IsGoal(DirectX::SimpleMath::Vector3 position);

	//プレイヤーのスタート位置を返す
	DirectX::SimpleMath::Vector3 GetPlayerStartPosition()
	{
		return m_playerPos;
	}

};


