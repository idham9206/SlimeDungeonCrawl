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
	//��������ݒ肷��
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	// ���f����ǂݍ���
	EffectFactory fx(device);

	device;

	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

	//�I�u�W�F�N�g2D���擾����
	CreateWICTextureFromFile(device, L"Resources\\Textures\\charatest(frontmove).png", nullptr, m_playerTexture.GetAddressOf());
	m_player = std::make_unique<Obj2D>();
	m_player->SetPosition(Vector3(5.0f, 1.0f, 5.0f));
	m_player->Initialize(m_deviceResources, states, 8);
	m_player->SetTexture(m_playerTexture.Get());

}

void Player::Update(float elapsedTime)
{

	//�I�u�W�F�N�g2D�̍X�V
	m_player->Update(elapsedTime);
	Move();
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
			Vector3 destination = m_player->GetPosition() + nextMovingDirectionVector;   // �ړI�n

			if (m_dungeon->IsMovable(destination)) // �ړ��\�̏ꍇ
			{
				velocity = differenceVector;
				speed -= distance;
				//velocity += Vector3(0.0f, 1.0f, 0.0f);
				velocity += nextMovingDirectionVector * speed;
			}
			else    // �ړ��s�\�̏ꍇ
			{

				nextDirection = m_movingDirection;    // �w��̕��������݂̐i�s�����ɕύX
			}


		}
		else    // ���̃t���[���Œ��S�ɂ��ǂ蒅���Ȃ��ꍇ
		{
			//Vector3 nextMovingDirectionVector = DIRECTION_VECTOR[nextDirection]; // ����̈ړ������x�N�g���̎Z�o
			//Vector3 destination = m_player->GetPosition() + nextMovingDirectionVector;   // �ړI�n

			//if (!m_dungeon->IsMovable(destination)) // �ړ��s�\�̏ꍇ
			//{
			//	if (m_dungeon->IsMovable(Vector3(destination.x, destination.y + 1.0f, destination.z)))
			//	{
			//		velocity = differenceVector;
			//		speed -= distance;
			//		velocity += Vector3(0.0f, 1.0f, 0.0f);
			//	}

			//}


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
		Vector3 destinationCenter = destination;                      // �ړI�n�̒��S���W

		Vector3 differenceVector = destinationCenter - nextPosition;   // �ړI�n�̒��S���W�ƈړ��\��̈ʒu�̍����x�N�g��
		float   distance = differenceVector.Length();          // �ړI�n�̒��S���W�ƈړ��\��̈ʒu�Ƃ̋���

		if (distance < 1.0f)    // �ړ��\��̈ʒu���ׂ̃Z���ɂ͂ݏo���Ă���ꍇ
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
		Vector3 nextMovingDirectionVector = DIRECTION_VECTOR[nextDirection]; // ����̈ړ������x�N�g���̎Z�o
		Vector3 destination = m_player->GetPosition() + nextMovingDirectionVector;   // �ړI�n

		Vector3 differenceVector = center - m_player->GetPosition();          // ���S���W�ƌ��݈ʒu�̍����x�N�g��
		float distance = differenceVector.Length();    // ���S���W�ƌ��݈ʒu�Ƃ̋���

		if (!m_dungeon->IsMovable(destination)) // �ړ��s�\�̏ꍇ
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
