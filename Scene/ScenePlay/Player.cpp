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
	//��������ݒ肷��
	ID3D11Device* device = deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = deviceResources->GetD3DDeviceContext();

	// ���f����ǂݍ���
	EffectFactory fx(device);

	device;

	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

	//�I�u�W�F�N�g2D���擾����
	CreateWICTextureFromFile(device, L"Resources\\Textures\\charatest(front).png", nullptr, m_playerTexture.GetAddressOf());
	m_player = std::make_unique<Obj2D>();
	m_player->SetPosition(Vector3(5.0f, 1.0f, 5.0f));
	m_player->Initialize(deviceResources, states, 4);
	m_player->SetTexture(m_playerTexture.Get());

}

void Player::Update(float elapsedTime)
{
	//�v���C���[�̓�
	Move();

	//�v���C���[�̍X�V
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
	//�J�����̈ʒu
	return DirectX::SimpleMath::Vector3(m_player->GetPosition().x + 0.0f, m_player->GetPosition().y + 5.0f, m_player->GetPosition().z + 5.0f);
}

void Player::Move()
{
	auto kb = Keyboard::Get().GetState();
	m_tracker.Update(kb);

	//�v���C���[�̈ʒu�擾
	Vector3 position = m_player->GetPosition();

	//�����̃J�E���g�_�E��
	static int move_CD = 10;
	move_CD--;

	//�J�E���g�_�E���[���ɂȂ����瓮����悤�ɂ���B
	if (move_CD < 0)
	{
		if (m_tracker.IsKeyPressed(DirectX::Keyboard::Up))
		{
			if (m_dungeon->IsMovable(Vector3(position.x, position.y, (position.z - 1.0f))))
			{
				m_player->SetPosition(Vector3(position.x, position.y, position.z - 1.0f));
				move_CD = 10;
			}
			else if (!m_dungeon->IsMovable(Vector3(position.x, position.y, (position.z - 1.0f))) &&
				m_dungeon->IsMovable(Vector3((position.x), (position.y + 1.0f), (position.z - 1.0f))))
			{
				if (move_CD < 0)
				{
					m_player->SetPosition(Vector3(position.x, position.y + 1.0f, position.z - 1.0f));
					move_CD = 10;
				}
			}
		}
		else if (m_tracker.IsKeyPressed(DirectX::Keyboard::Down))
		{

			if (m_dungeon->IsMovable(Vector3(position.x, position.y, (position.z + 1.0f))))
			{
				m_player->SetPosition(Vector3(position.x, position.y, position.z + 1.0f));
				move_CD = 10;

			}
			else if (!m_dungeon->IsMovable(Vector3(position.x, position.y, (position.z + 1.0f))) &&
				m_dungeon->IsMovable(Vector3((position.x), (position.y + 1.0f), (position.z + 1.0f))))
			{
				if (move_CD < 0)
				{
					m_player->SetPosition(Vector3(position.x, position.y + 1.0f, position.z + 1.0f));
					move_CD = 10;
				}
			}


		}
		else if (m_tracker.IsKeyPressed(DirectX::Keyboard::Right))
		{

			if (m_dungeon->IsMovable(Vector3((position.x + 1.0f), position.y, (position.z))))
			{
				m_player->SetPosition(Vector3(position.x + 1, position.y, position.z));
				move_CD = 10;

			}
			else if (!m_dungeon->IsMovable(Vector3((position.x + 1.0f), position.y, (position.z))) &&
				m_dungeon->IsMovable(Vector3((position.x + 1.0f), (position.y + 1.0f), (position.z))))
			{
				if (move_CD < 0)
				{
					m_player->SetPosition(Vector3(position.x + 1.0f, position.y + 1.0f, position.z));
					move_CD = 10;
				}
			}

		}
		else if (m_tracker.IsKeyPressed(DirectX::Keyboard::Left))
		{

			if (m_dungeon->IsMovable(Vector3((position.x - 1.0f), position.y, (position.z))))
			{
				m_player->SetPosition(Vector3(position.x - 1.0f, position.y, position.z));
				move_CD = 10;

			}
			else if (!m_dungeon->IsMovable(Vector3((position.x - 1.0f), position.y, (position.z))) &&
				m_dungeon->IsMovable(Vector3((position.x - 1.0f), (position.y + 1.0f), (position.z))))
			{
				if (move_CD < 0)
				{
					m_player->SetPosition(Vector3(position.x - 1.0f, position.y + 1.0f, position.z));
					move_CD = 10;
				}
			}


		}

		if (m_dungeon->IsMovable(Vector3((position.x), (position.y - 1.0f), (position.z))))
		{
			if (m_dungeon->IsMovable(Vector3((position.x), (position.y), (position.z))))
			{
				m_player->SetPosition(Vector3(position.x, position.y - 1.0f, position.z));

			}
			move_CD = 10;

		}
	}


}

