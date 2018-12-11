#include "..\..\pch.h"
#include "Player.h"
#include "Dungeon.h"
#include "..\..\Utility\Utility.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player():
	m_dungeon(nullptr), m_deviceResources(nullptr),
	m_movingDirection(DIR_NONE), m_speed(0.1f)
{
}


Player::~Player()
{
}

void Player::Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates * states)
{
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//引数から設定する
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	// モデルを読み込む
	EffectFactory fx(device);

	device;

	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

	//オブジェクト2D情報取得する
	CreateWICTextureFromFile(device, L"Resources\\Textures\\charatest(frontmove).png", nullptr, m_playerTexture.GetAddressOf());
	m_player = std::make_unique<Obj2D>();
	m_player->SetPosition(Vector3(5.0f, 1.0f, 5.0f));
	m_player->Initialize(m_deviceResources, states, 8);
	m_player->SetTexture(m_playerTexture.Get());

}

void Player::Update(float elapsedTime)
{

	//オブジェクト2Dの更新
	m_player->Update(elapsedTime);
	Move();
}

void Player::Render(Vector3 eye, Matrix view, Matrix projection)
{
	//オブジェクト2Dの描画
	m_player->Render(eye, view, projection);

}

void Player::Reset()
{
	//オブジェクト2Dの解放
	m_player.reset();

}

//オブジェクト2Dの動
void Player::Move()
{
	//キーボードのハンドル
	auto kb = Keyboard::Get().GetState();
	//m_tracker.Update(kb);

	//プレイヤーの位置取得
	Vector3 position = m_player->GetPosition();


	Direction nextMovingDirection = DIR_NONE;
	static int countDown = 10;

	countDown--;

	if (kb.Up)
	{
		nextMovingDirection = DIR_UP;
	}
	else if (kb.Down)
	{
		nextMovingDirection = DIR_DOWN;
	}
	else if (kb.Right)
	{
		nextMovingDirection = DIR_RIGHT;
	}
	else if (kb.Left)
	{
		nextMovingDirection = DIR_LEFT;
	}

	//Climb();

	if (nextMovingDirection != DIR_NONE)
	{
		m_movingDirection = PMove(nextMovingDirection);
		//m_player->SetPosition(Vector3(position.x, position.y + 0.01f, position.z));

	}
	//Fall();

}



Direction Player::PMove(Direction nextDirection)
{
	//プレイヤーの位置取得
	Vector3 position = m_player->GetPosition();
	int iPosBuffX = (int)position.x;
	int iPosBuffY = (int)position.y;
	int iPosBuffZ = (int)position.z;
	Vector3 fPosBuff = Vector3((float)iPosBuffX, (float)iPosBuffY, (float)iPosBuffZ);
	// ローカル変数の定義 -----------------------------------
	Vector3 velocity = Vector3::Zero;    // 速度
	float   speed = m_speed;          // 速さ
	Vector3 center = fPosBuff;       // 現在位置のグリッド座標の中心座標

	if (m_movingDirection == Direction::DIR_NONE)
	{
		m_movingDirection = nextDirection;
	}

	// 直角に方向転換する場合 --------------------------------
	if (m_movingDirection % 2 != nextDirection % 2)
	{
		Vector3 differenceVector = center - m_player->GetPosition();          // 中心座標と現在位置の差分ベクトル
		float distance = differenceVector.Length();    // 中心座標と現在位置との距離

		if (distance < m_speed)    // このフレームで中心にたどり着ける場合
		{
			Vector3 nextMovingDirectionVector = DIRECTION_VECTOR[nextDirection]; // 次回の移動方向ベクトルの算出
			Vector3 destination = m_player->GetPosition() + nextMovingDirectionVector;   // 目的地

			if (m_dungeon->IsMovable(destination)) // 移動可能の場合
			{
				velocity = differenceVector;
				speed -= distance;
				//velocity += Vector3(0.0f, 1.0f, 0.0f);
				velocity += nextMovingDirectionVector * speed;
			}
			else    // 移動不可能の場合
			{

				nextDirection = m_movingDirection;    // 指定の方向を現在の進行方向に変更
			}


		}
		else    // このフレームで中心にたどり着けない場合
		{
			//Vector3 nextMovingDirectionVector = DIRECTION_VECTOR[nextDirection]; // 次回の移動方向ベクトルの算出
			//Vector3 destination = m_player->GetPosition() + nextMovingDirectionVector;   // 目的地

			//if (!m_dungeon->IsMovable(destination)) // 移動不可能の場合
			//{
			//	if (m_dungeon->IsMovable(Vector3(destination.x, destination.y + 1.0f, destination.z)))
			//	{
			//		velocity = differenceVector;
			//		speed -= distance;
			//		velocity += Vector3(0.0f, 1.0f, 0.0f);
			//	}

			//}


			nextDirection = m_movingDirection;    // 指定の方向を現在の進行方向に変更
		}
	}


	// 直角に方向転換しない場合 or 直角に方向転換できなかった場合 --------
	if (m_movingDirection % 2 == nextDirection % 2)
	{
		Vector3 nextMovingDirectionVector = DIRECTION_VECTOR[nextDirection];    // 方向ベクトルの算出
		velocity = nextMovingDirectionVector * speed;

		Vector3 nextPosition = m_player->GetPosition() + velocity;    // 移動予定の位置

		Vector3 destination = fPosBuff + nextMovingDirectionVector;   // 目的地
		Vector3 destinationCenter = destination;                      // 目的地の中心座標

		Vector3 differenceVector = destinationCenter - nextPosition;   // 目的地の中心座標と移動予定の位置の差分ベクトル
		float   distance = differenceVector.Length();          // 目的地の中心座標と移動予定の位置との距離

		if (distance < 1.0f)    // 移動予定の位置が隣のセルにはみ出している場合
		{
			if (!m_dungeon->IsMovable(destination))
			{
				velocity = center - m_player->GetPosition();

				//if (m_dungeon->IsMovable(Vector3(destination.x, destination.y + 1.0f, destination.z)))
				//{
				//	velocity = differenceVector;
				//	speed -= distance;
				//	velocity += Vector3(0.0f, 1.0f, 0.0f);
				//}

			}
		}
	}

	{
		Vector3 nextMovingDirectionVector = DIRECTION_VECTOR[nextDirection]; // 次回の移動方向ベクトルの算出
		Vector3 destination = m_player->GetPosition() + nextMovingDirectionVector;   // 目的地

		Vector3 differenceVector = center - m_player->GetPosition();          // 中心座標と現在位置の差分ベクトル
		float distance = differenceVector.Length();    // 中心座標と現在位置との距離

		if (!m_dungeon->IsMovable(destination)) // 移動不可能の場合
		{
			if (m_dungeon->IsMovable(Vector3(destination.x, destination.y + 1.0f, destination.z)))
			{
				velocity = differenceVector;
				speed -= distance;
				velocity += Vector3(0.0f, 1.0f, 0.0f);
			}

		}


		if (m_dungeon->IsMovable(Vector3(destination.x, destination.y - 1.0f, destination.z)))
		{
			velocity = differenceVector;
			speed -= distance;
			velocity += Vector3(0.0f, -1.0f, 0.0f);
		}

	}
	
	// 移動処理 ------------------------------------
	m_player->SetPosition(m_player->GetPosition() + velocity); // 速度分の移動

	return nextDirection;
}

void Player::Climb()
{
	//キーボードのハンドル
	auto kb = Keyboard::Get().GetState();

	//プレイヤーの位置取得
	Vector3 position = m_player->GetPosition();
	int iPosBuffX = (int)position.x;
	int iPosBuffY = (int)position.y;
	int iPosBuffZ = (int)position.z;
	Vector3 fPosBuff = Vector3((float)iPosBuffX, (float)iPosBuffY, (float)iPosBuffZ);

	static int countDown = 10;

	countDown--;

	//if (countDown < 0)
	{


		if (kb.Up)
		{
			if (!m_dungeon->IsMovable(Vector3(fPosBuff.x, fPosBuff.y, (fPosBuff.z - 1.0f))) &&
				m_dungeon->IsMovable(Vector3((fPosBuff.x), (fPosBuff.y + 1.0f), (fPosBuff.z - 1.0f))))
			{
				//if (countDown < 0)
				{
					fPosBuff.y++;
					countDown = 10;
				}
			}
		}
		else if (kb.Down)
		{

			if (!m_dungeon->IsMovable(Vector3(fPosBuff.x, fPosBuff.y, (fPosBuff.z + 1.0f))) &&
				m_dungeon->IsMovable(Vector3((fPosBuff.x), (fPosBuff.y + 1.0f), (fPosBuff.z + 1.0f))))
			{
				//if (countDown < 0)
				{
					fPosBuff.y++;
					countDown = 10;
				}
			}


		}
		else if (kb.Right)
		{

			 if (!m_dungeon->IsMovable(Vector3((fPosBuff.x + 1.0f), fPosBuff.y, (fPosBuff.z))) &&
				m_dungeon->IsMovable(Vector3((fPosBuff.x + 1.0f), (fPosBuff.y + 1.0f), (fPosBuff.z))))
			{
				//if (countDown < 0)
				{
					fPosBuff.y++;
					countDown = 10;
				}
			}

		}
		else if (kb.Left)
		{
			if (!m_dungeon->IsMovable(Vector3((fPosBuff.x - 1.0f), fPosBuff.y, (fPosBuff.z))) &&
				m_dungeon->IsMovable(Vector3((fPosBuff.x - 1.0f), (fPosBuff.y + 1.0f), (fPosBuff.z))))
			{
				//if (countDown < 0)
				{
					fPosBuff.y++;
					countDown = 10;
				}
			}


		}
	}

	m_player->SetPosition(fPosBuff);
}

void Player::Fall()
{
	//プレイヤーの位置取得
	Vector3 position = m_player->GetPosition();
	int iPosBuffX = (int)position.x;
	int iPosBuffY = (int)position.y;
	int iPosBuffZ = (int)position.z;
	Vector3 fPosBuff = Vector3((float)iPosBuffX, (float)iPosBuffY, (float)iPosBuffZ);


	if (m_dungeon->IsMovable(Vector3((fPosBuff.x), (fPosBuff.y - 1.0f), (fPosBuff.z))))
	{
		if (m_dungeon->IsMovable(Vector3((fPosBuff.x), (fPosBuff.y), (fPosBuff.z))))
		{
			position.y--;
		}

	}
	m_player->SetPosition(position);

}
