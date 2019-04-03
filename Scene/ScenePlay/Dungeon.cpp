#include "..\..\pch.h"
#include "Dungeon.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

const float Dungeon::BLOCK_SPEED = 0.2f;

Dungeon::Dungeon():
	m_blockAlpha(nullptr), /*m_blockBeta{ nullptr } ,*/ m_spawnPosAlpha{ Vector3::Zero }, m_spawnPosBeta{ Vector3::Zero },
	m_playerPos ( Vector3::Zero ), m_shadowPos (Vector3::Zero),
	m_block{ nullptr }, m_data { TILE_NONE } , m_loader (nullptr),
	m_blockCD (40), m_CD(BLOCK_COOLDOWN), m_dataAlpha(0)
{
}


Dungeon::~Dungeon()
{

	// �u���b�N�̈�̔j��
	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_HEIGHT; j++)
		{
			for (int k = 0; k < MAZE_LENGTH; k++)
			{
				if (m_block[i][j][k] != nullptr)
				{
					m_block[i][j][k].reset();
					m_block[i][j][k] = nullptr;
				}


			}
		}
	}



	for (int i = 0; i < TILE_ID; i++)
	{
		if (m_model[i] != nullptr)
		{
			m_model[i].reset();
			m_model[i] = nullptr;

		}
	}

	//���������ŕ����u���b�N�u�������ꍇ�̏����ł��B
	//���͖������Ȃ̂ŁA�Ƃ肠�����u���Ƃ��܂��B
	//for (int i = 0; i < BLOCK_MAXCOUNT; i++)
	//{
	//	if (m_blockBeta[i] != nullptr)
	//	{
	//		delete m_blockBeta[i];
	//		m_blockBeta[i] = nullptr;
	//	}

	//}

	if (m_blockAlpha != nullptr)
	{
		delete m_blockAlpha;
		m_blockAlpha = nullptr;

	}

	if (m_loader != nullptr)
	{
		delete m_loader;
		m_loader = nullptr;
	}
}

void Dungeon::Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates * states)
{
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//��������ݒ肷��
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	m_states = states;

	EffectFactory fx(device);
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//�f�[�^����
	m_loader = new DataLoad();
	m_loader->LoadData(m_dungeonData);


	//==================================
	//�e�̊֘A
	//==================================
	// �e�N�X�`���̃��[�h
	CreateWICTextureFromFile(device, L"Resources\\Textures\\shadow.png", nullptr, m_textureShadow.GetAddressOf());

	m_shadow = std::make_unique<Obj2D>();
	m_shadow->Initialize(m_deviceResources, states, 1);
	m_shadow->SetTexture(m_textureShadow.Get());
	//===================================


	//���f������
	m_model[TILE_BLOCK1] = Model::CreateFromCMO(device, L"Resources\\Models\\box.cmo", fx);
	m_model[TILE_BLOCK2] = Model::CreateFromCMO(device, L"Resources\\Models\\box5.cmo", fx);
	m_model[TILE_GOAL] = Model::CreateFromCMO(device, L"Resources\\Models\\boxgoal1.cmo", fx);

	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_HEIGHT; j++)
		{
			for (int k = 0; k < MAZE_LENGTH; k++)
			{
				m_block[i][j][k] = nullptr;
				m_data[i][j][k] = TILE_NONE;

			}
		}
	}

	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_HEIGHT; j++)
		{
			for (int k = 0; k < MAZE_LENGTH; k++)
			{
				m_block[i][j][k] = nullptr;
				m_data[i][j][k] = TILE_NONE;

			}
		}
	}
	//�Ă񂾃f�[�^��ϊ�����
	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_HEIGHT; j++)
		{
			for (int k = 0; k < MAZE_LENGTH; k++)
			{

				switch (m_loader->GetData()[i][j][k])
				{
				case 0: m_data[i][j][k] = TILE_NONE;	break;
				case 1: m_data[i][j][k] = TILE_BLOCK1;	break;
				case 2: m_data[i][j][k] = TILE_BLOCK2;	break;
				case 3: m_data[i][j][k] = TILE_GOAL;	break;
				//======================================
				case 11: 
					m_data[i][j][k] = TILE_FALLINGBLOCK1; 
					m_dataAlpha++;
					break;

				//======================================
				case 99: 
					m_playerPos = Vector3((float)i, (float)j, (float)k);
					m_data[i][j][k] = TILE_NONE;
					break;
				default : m_data[i][j][k] = TILE_NONE;  break;
				}


			}
		}
	}
	//������u���b�N�̉��ʒu�̃T�C�Y�X�V
	m_spawnPosAlpha.resize(m_dataAlpha);

	//������u���b�N�̐��̉��f�[�^
	int dataBeta = 0;
	// �u���b�N���쐬
	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_HEIGHT; j++)
		{
			for (int k = 0; k < MAZE_LENGTH; k++)
			{


				switch (m_data[i][j][k])
				{
				case TILE_BLOCK1:
					m_block[i][j][k] = std::make_unique<Obj3D>();
					m_block[i][j][k]->Initialize(m_deviceResources, m_states);
					m_block[i][j][k]->SetModel(m_model[TILE_BLOCK1].get());
					m_block[i][j][k]->SetPosition(Vector3((float)i, -1.0f + (float)j, -0.5f + (float)k));
					break;

				case TILE_BLOCK2:
					m_block[i][j][k] = std::make_unique<Obj3D>();
					m_block[i][j][k]->Initialize(m_deviceResources, m_states);
					m_block[i][j][k]->SetModel(m_model[TILE_BLOCK2].get());
					m_block[i][j][k]->SetPosition(Vector3((float)i, -1.0f + (float)j, -0.5f + (float)k));
					break;

				case TILE_GOAL:
					m_block[i][j][k] = std::make_unique<Obj3D>();
					m_block[i][j][k]->Initialize(m_deviceResources, m_states);
					m_block[i][j][k]->SetModel(m_model[TILE_GOAL].get());
					m_block[i][j][k]->SetPosition(Vector3((float)i, -1.5f + (float)j, (float)k));
					break;

				case TILE_FALLINGBLOCK1:
					m_spawnPosAlpha[dataBeta] = Vector3((float)i, (float)j + 5, (float)k);
					dataBeta++;
					break;

				default:
					break;
				}

			}

		}
	}
	if (m_loader != nullptr)
	{
		delete m_loader;
		m_loader = nullptr;
	}
}

void Dungeon::Update(float elapsedTime, bool startFlag)
{
	if (m_blockCD > 0 && startFlag)
	{
		m_blockCD--;
	}
	if (m_CD >= 0)
	{
		m_CD--;
	}

	//�e�̍X�V
	m_shadow->Update(elapsedTime);

	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_HEIGHT; j++)
		{
			for (int k = 0; k < MAZE_LENGTH; k++)
			{
				{
					if (m_block[i][j][k] != nullptr)
					{
						m_block[i][j][k]->Update(elapsedTime);

					}
				}
			}
		}
		//������u���b�N�̉��ϐ�
		static int dataBeta = 0;

		//���u���b�N�܂��Ȃ�������쐬����
		if (m_blockCD <= 0)
		{
			if (m_blockAlpha == nullptr)
			{

				m_blockAlpha = new Obj3D();
				m_blockAlpha->Initialize(m_deviceResources, m_states);
				m_blockAlpha->SetModel(m_model[TILE_BLOCK1].get());
				m_blockAlpha->SetPosition(Vector3((float)m_spawnPosAlpha[dataBeta].x, -1.0f + (float)m_spawnPosAlpha[dataBeta].y, -0.5f + (float)m_spawnPosAlpha[dataBeta].z));
				m_shadow->SetPosition(Vector3((float)m_spawnPosAlpha[dataBeta].x, -1.0f + (float)m_spawnPosAlpha[dataBeta].y, -3.5f + (float)m_spawnPosAlpha[dataBeta].z));
				//���������ŕ����u���b�N�u�������ꍇ�̏����ł��B
				//���͖������Ȃ̂ŁA�Ƃ肠�����u���Ƃ��܂��B
				//for (int i = 0; i < BLOCK_MAXCOUNT; i++)
				//{
				//	//�����̃V�[�h�����Z�b�g����B
				//	//srand((unsigned int)time(NULL));
				//	m_spawnPosBeta[i].x = m_spawnPosAlpha.x + (-1.0f + (float)(rand() % 2));
				//	m_spawnPosBeta[i].y = m_spawnPosAlpha.y + ((float)(rand() % 1));
				//	m_spawnPosBeta[i].z = m_spawnPosAlpha.z + (-1.0f + (float)(rand() % 2));

				//	if (m_spawnPosAlpha != m_spawnPosBeta[i])
				//	{
				//		m_blockBeta[i] = new Obj3D();
				//		m_blockBeta[i]->Initialize(m_deviceResources, m_states);
				//		m_blockBeta[i]->SetModel(m_model[TILE_BLOCK1].get());
				//		m_blockBeta[i]->SetPosition(Vector3((float)m_spawnPosBeta[i].x, -1.0f + (float)m_spawnPosBeta[i].y, -0.5f + (float)m_spawnPosBeta[i].z));
				//	}

				//}
			}

		}

		//���u���b�N�������痎���鏈�������܂�
		if (m_blockAlpha != nullptr)
		{
			m_blockAlpha->Update(elapsedTime);

			//���������ŕ����u���b�N�u�������ꍇ�̏����ł��B
			//���͖������Ȃ̂ŁA�Ƃ肠�����u���Ƃ��܂��B
			//for (int i = 0; i < BLOCK_MAXCOUNT; i++)
			//{
			//	if (m_blockBeta[i] != nullptr)
			//	{
			//		m_blockBeta[i]->Update(elapsedTime);
			//	}
			//}

			if (IsMovable(Vector3((m_spawnPosAlpha[dataBeta].x), (m_spawnPosAlpha[dataBeta].y - Dungeon::BLOCK_SPEED), (m_spawnPosAlpha[dataBeta].z))))
			{
				if (m_CD < 0)
				{
					m_spawnPosAlpha[dataBeta].y -= Dungeon::BLOCK_SPEED;
					m_blockAlpha->SetPosition(Vector3((float)m_spawnPosAlpha[dataBeta].x, -1.0f + (float)m_spawnPosAlpha[dataBeta].y, -0.5f + (float)m_spawnPosAlpha[dataBeta].z));
					
					//���������ŕ����u���b�N�u�������ꍇ�̏����ł��B
					//���͖������Ȃ̂ŁA�Ƃ肠�����u���Ƃ��܂��B
					//for (int i = 0; i < BLOCK_MAXCOUNT; i++)
					//{
					//	if (m_blockBeta[i] != nullptr)
					//	{
					//		m_spawnPosBeta[i].y -= Dungeon::BLOCK_SPEED;
					//		m_blockBeta[i]->SetPosition(Vector3((float)m_spawnPosBeta[i].x, -1.0f + (float)m_spawnPosBeta[i].y, -0.5f + (float)m_spawnPosBeta[i].z));
					//	}
					//}

					m_CD = BLOCK_COOLDOWN;
				}
			}
			else
			{
				{
					int i = (int)m_spawnPosAlpha[dataBeta].x;
					int j = (int)m_spawnPosAlpha[dataBeta].y;
					int k = (int)m_spawnPosAlpha[dataBeta].z;
					if (m_block[i][j][k] == nullptr)
					{
						m_block[i][j][k] = std::make_unique<Obj3D>();
						m_block[i][j][k]->Initialize(m_deviceResources, m_states);
						m_block[i][j][k]->SetModel(m_model[TILE_BLOCK1].get());
						m_block[i][j][k]->SetPosition(Vector3((float)i, -1.0f + (float)j, -0.5f + (float)k));
					}


					if (m_data[i][j][k] == TILE_NONE)
					{
						m_data[i][j][k] = TILE_BLOCK1;
					}
					else if (m_data[i][j][k] == TILE_FALLINGBLOCK1)
					{
						m_data[i][j][k] = TILE_BLOCK1;
					}


					if (m_blockAlpha != nullptr)
					{
						delete m_blockAlpha;
						m_blockAlpha = nullptr;
						if (dataBeta < m_dataAlpha - 1)
						{
							dataBeta++;

						}
					}

					//���������ŕ����u���b�N�u�������ꍇ�̏����ł��B
					//���͖������Ȃ̂ŁA�Ƃ肠�����u���Ƃ��܂��B
					//for (int l = 0; l < BLOCK_MAXCOUNT; l++)
					//{
					//	{
					//		int i = (int)m_spawnPosBeta[l].x;
					//		int j = (int)m_spawnPosBeta[l].y;
					//		int k = (int)m_spawnPosBeta[l].z;
					//		if (m_block[i][j][k] == nullptr)
					//		{
					//			m_block[i][j][k] = std::make_unique<Obj3D>();
					//			m_block[i][j][k]->Initialize(m_deviceResources, m_states);
					//			m_block[i][j][k]->SetModel(m_model[TILE_BLOCK1].get());
					//			m_block[i][j][k]->SetPosition(Vector3((float)i, -1.0f + (float)j, -0.5f + (float)k));
					//		}

					//		if (m_data[i][j][k] == TILE_NONE)
					//		{
					//			m_data[i][j][k] = TILE_BLOCK1;
					//		}

					//		//if (m_blockBeta[l] != nullptr)
					//		//{
					//		//	delete m_blockBeta[l];
					//		//	m_blockBeta[l] = nullptr;
					//		//}
					//	}
					//}

				}

			}


		}

	}
}


void Dungeon::Render(DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{

	//�R���e�L�X�g�Ăяo��
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	//�u���b�N�̃����_�[�\��
	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_HEIGHT; j++)
		{
			for (int k = 0; k < MAZE_LENGTH; k++)
			{
				if (m_block[i][j][k] != nullptr)
				{
					if (m_data[i][j][k] != 0)
					{
						m_block[i][j][k]->Render(view, projection);
					}
				}
			}
		}
	}


	//���̃u���b�Nnull����Ȃ�������
	if (m_blockAlpha != nullptr)
	{
		//���̃u���b�N�̉e��`�悷��
		m_shadow->Render(eye, view, projection);
		//���̃u���b�N��`�悷��
		m_blockAlpha->Render(view, projection);
	}

	//���������ŕ����u���b�N�u�������ꍇ�̏����ł��B
	//���͖������Ȃ̂ŁA�Ƃ肠�����u���Ƃ��܂��B
	//for (int i = 0; i < BLOCK_MAXCOUNT; i++)
	//{
	//	if (m_blockBeta[i] != nullptr)
	//	{
	//		m_blockBeta[i]->Render(view, projection);

	//	}
	//}



}


////=====================================================================================================////
/*�}�b�v�`�b�v�ɂ�铖���蔻��*/
bool Dungeon::IDChecker(TileID tileID, DirectX::SimpleMath::Vector3 position)
{	
	if (m_data[(int)position.x][(int)position.y][(int)position.z] == tileID)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}
////=====================================================================================================////

bool Dungeon::IsMovable(DirectX::SimpleMath::Vector3 position)
{
	if (m_data[(int)position.x][(int)position.y][(int)position.z] != TILE_BLOCK1 &&
		m_data[(int)position.x][(int)position.y][(int)position.z] != TILE_BLOCK2)
	{
		return true;
	}
	else
	{
		return false;
	}

}



bool Dungeon::IsClimbing(DirectX::SimpleMath::Vector3 position)
{
	if (((m_data[(int)position.x + 1][(int)position.y][(int)position.z] != TILE_BLOCK1 &&
		m_data[(int)position.x + 1][(int)position.y][(int)position.z] != TILE_BLOCK2)) ||
		((m_data[(int)position.x - 1][(int)position.y][(int)position.z] != TILE_BLOCK1 &&
			m_data[(int)position.x - 1][(int)position.y][(int)position.z] != TILE_BLOCK2)) ||
			((m_data[(int)position.x][(int)position.y][(int)position.z + 1] != TILE_BLOCK1 &&
				m_data[(int)position.x][(int)position.y][(int)position.z + 1] != TILE_BLOCK2)) ||
				((m_data[(int)position.x][(int)position.y][(int)position.z - 1] != TILE_BLOCK1 &&
					m_data[(int)position.x][(int)position.y][(int)position.z - 1] != TILE_BLOCK2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Dungeon::FallingDown(DirectX::SimpleMath::Vector3 position)
{
	if (m_data[(int)position.x][(int)position.y - 1][(int)position.z] == TILE_NONE)
	{
		return true;
	}
	else
	{
		return false;

	}
}

bool Dungeon::IsGoal(DirectX::SimpleMath::Vector3 position)
{
	{
		if (m_data[(int)position.x][(int)position.y][(int)position.z] == TILE_GOAL)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
}

void Dungeon::SetDungeonData(const wchar_t * data)
{
	m_dungeonData = data;
}
