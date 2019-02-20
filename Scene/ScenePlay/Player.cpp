#include "..\..\pch.h"
#include "Player.h"
#include "Dungeon.h"
#include "..\..\Utility\Utility.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player():
	m_dungeon(nullptr), m_deviceResources(nullptr),
	m_movingDirection(DIR_NONE), m_charaState(NONE), m_speed(0.1f),
	m_isDead(false), m_isMove(false), m_isFront(true), m_isOver(false)
{
}


Player::~Player()
{
}

void Player::Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates * states)
{
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//��������ݒ肷��
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	// ���f����ǂݍ���
	EffectFactory fx(device);

	device;

	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

	//�I�u�W�F�N�g2D���擾����
	//============================================================================================================================
	//�A�C�h��
	CreateWICTextureFromFile(device, L"Resources\\Textures\\chara_idle(front).png", nullptr, m_playerTexture[0].GetAddressOf());
	CreateWICTextureFromFile(device, L"Resources\\Textures\\chara_idle(back).png", nullptr, m_playerTexture[1].GetAddressOf());
	//����
	CreateWICTextureFromFile(device, L"Resources\\Textures\\chara_move(front).png", nullptr, m_playerTexture[2].GetAddressOf());
	CreateWICTextureFromFile(device, L"Resources\\Textures\\chara_move(back).png", nullptr, m_playerTexture[3].GetAddressOf());
	//�U�����ꂽ
	CreateWICTextureFromFile(device, L"Resources\\Textures\\chara_hit(front).png", nullptr, m_playerTexture[4].GetAddressOf());
	CreateWICTextureFromFile(device, L"Resources\\Textures\\chara_hit(back).png", nullptr, m_playerTexture[5].GetAddressOf());
	//============================================================================================================================
	m_player = std::make_unique<Obj2D>();
	m_player->SetPosition(Vector3(5.0f, 1.0f, 5.0f));
	m_player->Initialize(m_deviceResources, states, 4);
	m_player->SetTexture(m_playerTexture[0].Get());

}

// �X�V����
void Player::Update(float elapsedTime)
{
	bool wasMove = m_isMove;
	bool wasDead = m_isDead;
	//�I�u�W�F�N�g2D�̍X�V
	m_player->Update(elapsedTime);
	//Direction wasDirection = m_movingDirection;
	if (!m_isDead)
	{
		Move();
	}

	if (m_isOver)
	{
		m_isDead = m_isOver;
	}

	if (m_isDead)
	{
		if (m_isFront)
		{
			m_charaState = HIT_FRONT;
		}
		else if (!m_isFront)
		{
			m_charaState = HIT_BACK;
		}
	}


	switch (m_charaState)
	{
	case IDLE_FRONT:
		if (wasMove != m_isMove)
		{
			m_player->SetTexture(m_playerTexture[0].Get());
			m_player->SetFrameCount(4);
			m_player->ResetFrame();
		}
		break;
	case IDLE_BACK:
		if (wasMove != m_isMove)
		{
			m_player->SetTexture(m_playerTexture[1].Get());
			m_player->SetFrameCount(4);
			m_player->ResetFrame();
		}
		break;
	case MOVE_FRONT:
		if (wasMove != m_isMove)
		{
			m_player->SetTexture(m_playerTexture[2].Get());
			m_player->SetFrameCount(8);
			m_player->ResetFrame();
		}
		break;
	case MOVE_BACK:
		if (wasMove != m_isMove)
		{
			m_player->SetTexture(m_playerTexture[3].Get());
			m_player->SetFrameCount(8);
			m_player->ResetFrame();
		}
		break;
	case HIT_FRONT:
		if (wasDead != m_isDead)
		{
			m_player->SetTexture(m_playerTexture[4].Get());
			m_player->SetFrameCount(4);
			m_player->ResetFrame();
		}
		break;
	case HIT_BACK:
		if (wasDead != m_isDead)
		{
			m_player->SetTexture(m_playerTexture[5].Get());
			m_player->SetFrameCount(4);
			m_player->ResetFrame();
		}
		break;
	default:
		if (wasMove != m_isMove)
		{
			m_player->SetTexture(m_playerTexture[0].Get());
			m_player->SetFrameCount(4);
			m_player->ResetFrame();
		}
		break;
	}
	if (m_isDead)
	{
		m_player->SetFrameLoop(false);
	}

}

void Player::Render(Vector3 eye, Matrix view, Matrix projection)
{
	//�I�u�W�F�N�g2D�̕`��
	m_player->Render(eye, view, projection);

}

void Player::Reset()
{
	//�I�u�W�F�N�g2D�̉��
	m_player.reset();

}

//�I�u�W�F�N�g2D�̓�
void Player::Move()
{
	//�L�[�{�[�h�̃n���h��
	auto kb = Keyboard::Get().GetState();
	//m_tracker.Update(kb);

	//�v���C���[�̈ʒu�擾
	Vector3 position = m_player->GetPosition();
	int iPosBuffX = (int)position.x;
	int iPosBuffY = (int)position.y;
	int iPosBuffZ = (int)position.z;
	Vector3 fPosBuff = Vector3((float)iPosBuffX, (float)iPosBuffY, (float)iPosBuffZ);

	Direction nextMovingDirection = DIR_NONE;
	static int countdown = 10;
	static float wasPosition = position.y;
	countdown--;

	m_isMove = false;
	if (kb.Up)
	{
		nextMovingDirection = DIR_UP;
		m_isMove = true;
		m_isFront = false;
	}
	else if (kb.Down)
	{
		nextMovingDirection = DIR_DOWN;
		m_isMove = true;
		m_isFront = true;
	}
	else if (kb.Right)
	{
		nextMovingDirection = DIR_RIGHT;
		m_isMove = true;
		m_isFront = false;
	}
	else if (kb.Left)
	{
		nextMovingDirection = DIR_LEFT;
		m_isMove = true;
		m_isFront = true;
	}

	if (nextMovingDirection != DIR_NONE)
	{
		m_movingDirection = PMove(nextMovingDirection);


		//if (countdown < 0)
		//{
		//	//m_player->SetPosition(Vector3(position.x, position.y + 0.5f, position.z));
		//	//wasPosition = position.y;
		//	//countdown = 10;
		//}
	}

	//if (m_dungeon->IsMovable(Vector3((iPosBuffX), (iPosBuffY - 1.0f), (iPosBuffY))))
	//{
	//	if (m_dungeon->IsMovable(position))
	//	{
	//		//position.y--;
	//		//m_player->SetPosition(Vector3(position.x, position.y - 0.1f, position.z));
	//		//countdown = 10;

	//	}

	//}

	AMove();
	if (m_isFront)
	{
		m_charaState = IDLE_FRONT;
		if (m_isMove)
		{
			m_charaState = MOVE_FRONT;
		}
		else
		{
			m_charaState = IDLE_FRONT;
		}
	}
	else if (!m_isFront)
	{
		m_charaState = IDLE_BACK;
		if (m_isMove)
		{
			m_charaState = MOVE_BACK;
		}
		else
		{
			m_charaState = IDLE_BACK;
		}
	}

	if (kb.A)
	{
		m_isDead = true;
	}


}

void Player::AMove()
{
	auto kb = Keyboard::Get().GetState();
	Vector3 position = m_player->GetPosition();

	static int countdown = 10;

	countdown--;

	if (countdown < 0)
	{
		if (kb.Up)
		{
			if (m_dungeon->IsMovable(Vector3(position.x, position.y, (position.z - 1.0f))))
			{
				//position.z--;
				//m_player->SetPosition(position);

			}
			else if (!m_dungeon->IsMovable(Vector3(position.x, position.y, (position.z - 1.0f))) &&
				m_dungeon->IsMovable(Vector3((position.x), (position.y + 1.0f), (position.z - 1.0f))))
			{
				{
					//position.z--;
					//position.y++;
					m_player->SetPosition(Vector3(position.x, position.y + 1.0f, position.z - 1.0f));
					countdown = 10;
				}
			}
		}
		else if (kb.Down)
		{
			if (m_dungeon->IsMovable(Vector3(position.x, position.y, (position.z + 1.0f))))
			{
				//position.z++;
				//m_player->SetPosition(position);


			}
			else if (!m_dungeon->IsMovable(Vector3(position.x, position.y, (position.z + 1.0f))) &&
				m_dungeon->IsMovable(Vector3((position.x), (position.y + 1.0f), (position.z + 1.0f))))
			{
				{
					//position.z++;
					//position.y++;
					m_player->SetPosition(Vector3(position.x, position.y + 1.0f, position.z + 1.0f));
					countdown = 10;

				}
			}


		}
		else if (kb.Right)
		{
			if (m_dungeon->IsMovable(Vector3((position.x + 1.0f), position.y, (position.z))))
			{
				//position.x++;
				//m_player->SetPosition(position);

			}
			else if (!m_dungeon->IsMovable(Vector3((position.x + 1.0f), position.y, (position.z))) &&
				m_dungeon->IsMovable(Vector3((position.x + 1.0f), (position.y + 1.0f), (position.z))))
			{
				{
					//position.x++;
					//position.y++;
					m_player->SetPosition(Vector3(position.x + 1.0f, position.y + 1.0f, position.z));
					countdown = 10;

				}
			}

		}
		else if (kb.Left)
		{

			if (m_dungeon->IsMovable(Vector3((position.x - 1.0f), position.y, (position.z))))
			{
				//position.x--;
				//m_player->SetPosition(position);


			}
			else if (!m_dungeon->IsMovable(Vector3((position.x - 1.0f), position.y, (position.z))) &&
				m_dungeon->IsMovable(Vector3((position.x - 1.0f), (position.y + 1.0f), (position.z))))
			{
				{
					//position.x--;
					//position.y++;
					m_player->SetPosition(Vector3(position.x - 1.0f, position.y + 1.0f, position.z));
					countdown = 10;

				}
			}


		}


		if (m_dungeon->IsMovable(Vector3((position.x), (position.y - 1.0f), (position.z))))
		{
<<<<<<< HEAD
			if (m_dungeon->FallingDown(Vector3((m_position.x), (m_position.y), (m_position.z))))
=======
			if (m_dungeon->IsMovable(Vector3((position.x), (position.y), (position.z))))
>>>>>>> parent of acc0c5c... daily commit
			{
				//position.y--;
				m_player->SetPosition(Vector3(position.x, position.y - 1.0f, position.z));
				countdown = 10;

			}

		}

	}

}



Direction Player::PMove(Direction nextDirection)
{
	//�v���C���[�̈ʒu�擾
	Vector3 position = m_player->GetPosition();
	int iPosBuffX = (int)position.x;
	int iPosBuffY = (int)position.y;
	int iPosBuffZ = (int)position.z;
	Vector3 fPosBuff = Vector3((float)iPosBuffX, (float)iPosBuffY, (float)iPosBuffZ);
	// ���[�J���ϐ��̒�` -----------------------------------
	Vector3 velocity = Vector3::Zero;    // ���x
	float   speed = m_speed;          // ����
	Vector3 center = fPosBuff;       // ���݈ʒu�̃O���b�h���W�̒��S���W

	if (m_movingDirection == Direction::DIR_NONE)
	{
		m_movingDirection = nextDirection;
	}

	// ���p�ɕ����]������ꍇ --------------------------------
	if (m_movingDirection % 2 != nextDirection % 2)
	{
		Vector3 differenceVector = center - m_player->GetPosition();          // ���S���W�ƌ��݈ʒu�̍����x�N�g��
		float distance = differenceVector.Length();    // ���S���W�ƌ��݈ʒu�Ƃ̋���

		if (distance < m_speed)    // ���̃t���[���Œ��S�ɂ��ǂ蒅����ꍇ
		{
			Vector3 nextMovingDirectionVector = DIRECTION_VECTOR[nextDirection]; // ����̈ړ������x�N�g���̎Z�o
			Vector3 destination = fPosBuff + nextMovingDirectionVector;   // �ړI�n

			if (m_dungeon->IsMovable(destination)) // �ړ��\�̏ꍇ
			{
				velocity = differenceVector;
				speed -= distance;
				velocity += nextMovingDirectionVector * speed;
			}
			else    // �ړ��s�\�̏ꍇ
			{

				nextDirection = m_movingDirection;    // �w��̕��������݂̐i�s�����ɕύX

			}



		}
		else    // ���̃t���[���Œ��S�ɂ��ǂ蒅���Ȃ��ꍇ
		{

			nextDirection = m_movingDirection;    // �w��̕��������݂̐i�s�����ɕύX
		}
	}


	// ���p�ɕ����]�����Ȃ��ꍇ or ���p�ɕ����]���ł��Ȃ������ꍇ --------
	if (m_movingDirection % 2 == nextDirection % 2)
	{
		Vector3 nextMovingDirectionVector = DIRECTION_VECTOR[nextDirection];    // �����x�N�g���̎Z�o
		velocity = nextMovingDirectionVector * speed;

		Vector3 nextPosition = m_player->GetPosition() + velocity;    // �ړ��\��̈ʒu

		Vector3 destination = fPosBuff + nextMovingDirectionVector;   // �ړI�n
		int iDesBuffX = (int)destination.x;
		int iDesBuffY = (int)destination.y;
		int iDesBuffZ = (int)destination.z;
		Vector3 destinationCenter = Vector3((float)iDesBuffX, (float)iDesBuffY, (float)iDesBuffZ);                      // �ړI�n�̒��S���W

		Vector3 differenceVector = destinationCenter - nextPosition;   // �ړI�n�̒��S���W�ƈړ��\��̈ʒu�̍����x�N�g��
		float   distance = differenceVector.Length();          // �ړI�n�̒��S���W�ƈړ��\��̈ʒu�Ƃ̋���

		if (distance < 1.0f)    // �ړ��\��̈ʒu���ׂ̃Z���ɂ͂ݏo���Ă���ꍇ
		{
			if (!m_dungeon->IsMovable(destination))
			{
				velocity = center - m_player->GetPosition();
			}
		}
	}

	//{
	//	Vector3 nextMovingDirectionVector = DIRECTION_VECTOR[nextDirection];    // �����x�N�g���̎Z�o
	//	velocity = nextMovingDirectionVector * speed;
	//	Vector3 destination = fPosBuff + nextMovingDirectionVector;   // �ړI�n
	//	if (!m_dungeon->IsMovable(destination)) // �ړ��s�\�̏ꍇ
	//	{
	//		if (m_dungeon->IsMovable(Vector3(destination.x, destination.y + 1.0f, destination.z)))
	//		{
	//			velocity = (Vector3(0.0f, 1.0f, 0.0f) + (center - m_player->GetPosition()));
	//		}

	//	}

	//	if (m_dungeon->IsMovable(Vector3(destination.x, destination.y - 1.0f, destination.z)))
	//	{
	//		velocity = Vector3(0.0f, -1.0f, 0.0f);
	//	}


	//}
	
	// �ړ����� ------------------------------------
	m_player->SetPosition(m_player->GetPosition() + velocity); // ���x���̈ړ�

	return nextDirection;
}

void Player::Climb()
{
	//�L�[�{�[�h�̃n���h��
	auto kb = Keyboard::Get().GetState();

	//�v���C���[�̈ʒu�擾
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
	//�v���C���[�̈ʒu�擾
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
