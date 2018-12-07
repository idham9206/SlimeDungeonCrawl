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
	
	if (kb.Up)  nextMovingDirection = DIR_UP;
	else if (kb.Down)  nextMovingDirection = DIR_DOWN;
	else if (kb.Right)  nextMovingDirection = DIR_RIGHT;
	else if (kb.Left)  nextMovingDirection = DIR_LEFT;


	if (nextMovingDirection != DIR_NONE)
	{
		m_movingDirection = PMove(nextMovingDirection);
	}


}



Direction Player::PMove(Direction nextDirection)
{

	// ���[�J���ϐ��̒�` -----------------------------------
	Vector3 velocity = Vector3::Zero;    // ���x
	float   speed = m_speed;          // ����
	Vector3 position = m_player->GetPosition(); //�v���C���[�̈ʒu
	Vector3 center = m_player->GetPosition();       // ���݈ʒu�̃O���b�h���W�̒��S���W

	if (m_movingDirection == Direction::DIR_NONE)
	{
		m_movingDirection = nextDirection;
	}


	// ���p�ɕ����]������ꍇ --------------------------------
	if (m_movingDirection % 2 != nextDirection % 2)
	{
		Vector3 differenceVector = center - position;          // ���S���W�ƌ��݈ʒu�̍����x�N�g��
		float distance = position.Length();    // ���S���W�ƌ��݈ʒu�Ƃ̋���

		if (distance < m_speed)    // ���̃t���[���Œ��S�ɂ��ǂ蒅����ꍇ
		{
			Vector3 nextMovingDirectionVector = DIRECTION_VECTOR[nextDirection]; // ����̈ړ������x�N�g���̎Z�o
			Vector3 destination = nextMovingDirectionVector;   // �ړI�n

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

		Vector3 nextPosition = position + velocity;    // �ړ��\��̈ʒu

		Vector3 destination = nextMovingDirectionVector;   // �ړI�n
		Vector3 destinationCenter = destination;                      // �ړI�n�̒��S���W

		Vector3 differenceVector = destinationCenter - nextPosition;   // �ړI�n�̒��S���W�ƈړ��\��̈ʒu�̍����x�N�g��
		float   distance = differenceVector.Length();          // �ړI�n�̒��S���W�ƈړ��\��̈ʒu�Ƃ̋���

		if (distance < 1.0f)    // �ړ��\��̈ʒu���ׂ̃Z���ɂ͂ݏo���Ă���ꍇ
		{
			if (!m_dungeon->IsMovable(destination))
			{
				velocity = center - position;
			}
		}
	}


	// �ړ����� ------------------------------------
	//position += velocity;    
	m_player->SetPosition(position + velocity); // ���x���̈ړ�

	return nextDirection;
}
