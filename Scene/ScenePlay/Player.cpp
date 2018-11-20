#include "..\..\pch.h"
#include "Player.h"
#include "Dungeon.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player()
{
}


Player::~Player()
{
}

void Player::Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates * states)
{
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//引数から設定する
	ID3D11Device* device = deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = deviceResources->GetD3DDeviceContext();

	// モデルを読み込む
	EffectFactory fx(device);

	device;

	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

	//オブジェクト2D情報取得する
	CreateWICTextureFromFile(device, L"Resources\\Textures\\charatest(front).png", nullptr, m_playerTexture.GetAddressOf());
	m_player = std::make_unique<Obj2D>();
	m_player->SetPosition(Vector3(5.0f, 1.0f, 5.0f));
	m_player->Initialize(deviceResources, states);
	m_player->SetTexture(m_playerTexture.Get());

	m_CD = 10;
}

void Player::Update(float elapsedTime)
{
	auto kb = Keyboard::Get().GetState();
	m_tracker.Update(kb);
	Vector3 position = m_player->GetPosition();
	m_CD--;
	if (m_CD < 0)
	{
		if (m_tracker.IsKeyPressed(DirectX::Keyboard::Up))
		{
			if (m_dungeon->IsMovable(Vector3(position.x, position.y, (position.z - 1.0f))))
			{
				m_player->SetPosition(Vector3((int)position.x, (int)position.y, (int)position.z - 1.0f));
				m_CD = 10;
			}
			else if (!m_dungeon->IsMovable(Vector3(position.x, position.y, (position.z - 1.0f))) &&
				m_dungeon->IsMovable(Vector3((position.x), (position.y + 1.0f), (position.z - 1.0f))))
			{
				if (m_CD < 0)
				{
					m_player->SetPosition(Vector3(position.x, position.y + 1.0f, position.z - 1.0f));
					m_CD = 10;
				}
			}
		}
		else if (m_tracker.IsKeyPressed(DirectX::Keyboard::Down))
		{

			if (m_dungeon->IsMovable(Vector3(position.x, position.y, (position.z + 1.0f))))
			{
				m_player->SetPosition(Vector3(position.x, position.y, position.z + 1.0f));
				m_CD = 10;

			}
			else if (!m_dungeon->IsMovable(Vector3(position.x, position.y, (position.z + 1.0f))) &&
				m_dungeon->IsMovable(Vector3((position.x), (position.y + 1.0f), (position.z + 1.0f))))
			{
				if (m_CD < 0)
				{
					m_player->SetPosition(Vector3(position.x, position.y + 1.0f, position.z + 1.0f));
					m_CD = 10;
				}
			}


		}
		else if (m_tracker.IsKeyPressed(DirectX::Keyboard::Right))
		{

			if (m_dungeon->IsMovable(Vector3((position.x + 1.0f), position.y, (position.z))))
			{
				m_player->SetPosition(Vector3((int)position.x + 1, (int)position.y, (int)position.z));
				m_CD = 10;

			}
			else if (!m_dungeon->IsMovable(Vector3((position.x + 1.0f), position.y, (position.z))) &&
				m_dungeon->IsMovable(Vector3((position.x + 1.0f), (position.y + 1.0f), (position.z))))
			{
				if (m_CD < 0)
				{
					m_player->SetPosition(Vector3(position.x + 1.0f, position.y + 1.0f, position.z));
					m_CD = 10;
				}
			}

		}
		else if (m_tracker.IsKeyPressed(DirectX::Keyboard::Left))
		{

			if (m_dungeon->IsMovable(Vector3((position.x - 1.0f), position.y, (position.z))))
			{
				m_player->SetPosition(Vector3(position.x - 1.0f, position.y, position.z));
				m_CD = 10;

			}
			else if (!m_dungeon->IsMovable(Vector3((position.x - 1.0f), position.y, (position.z))) &&
				m_dungeon->IsMovable(Vector3((position.x - 1.0f), (position.y + 1.0f), (position.z))))
			{
				if (m_CD < 0)
				{
					m_player->SetPosition(Vector3(position.x - 1.0f, position.y + 1.0f, position.z));
					m_CD = 10;
				}
			}


		}

		if (m_dungeon->IsMovable(Vector3((position.x), (position.y - 1.0f), (position.z))))
		{
			if (m_dungeon->IsMovable(Vector3((position.x), (position.y), (position.z))))
			{
				m_player->SetPosition(Vector3(position.x, position.y - 1.0f , position.z));

			}
			m_CD = 10;

		}



	}

	//プレイヤーの更新
	m_player->Update(elapsedTime);

}

void Player::Render(Vector3 eye, Matrix view, Matrix projection)
{

	m_player->Render(eye, view, projection);

}

void Player::Reset()
{
	m_player.reset();

}

DirectX::SimpleMath::Vector3 Player::playerPositionToCamera()
{
	//カメラの位置
	return DirectX::SimpleMath::Vector3(m_player->GetPosition().x + 0.0f, m_player->GetPosition().y + 5.0f, m_player->GetPosition().z + 5.0f);
}

