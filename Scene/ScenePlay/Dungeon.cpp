#include "..\..\pch.h"
#include "Dungeon.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Dungeon::Dungeon()
{
}


Dungeon::~Dungeon()
{
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

	m_loader = new DataLoad();
	m_loader->LoadData(L"Stage00.csv");

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

				switch (m_loader->GetData()[i][j][k])
				{
				case 0: m_data[i][j][k] = TILE_PASSAGE;  break;
				case 1: m_data[i][j][k] = TILE_BLOCK1;	break;
				case 2: m_data[i][j][k] = TILE_BLOCK2;	break;
				case 3: m_data[i][j][k] = TILE_GOAL;	break;
				}


			}
		}
	}

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
					m_block[i][j][k]->SetPosition(Vector3((float)i, -1.0f + (float)j, (float)k));
					break;

				case TILE_BLOCK2:
					m_block[i][j][k] = std::make_unique<Obj3D>();
					m_block[i][j][k]->Initialize(m_deviceResources, m_states);
					m_block[i][j][k]->SetModel(m_model[TILE_BLOCK2].get());
					m_block[i][j][k]->SetPosition(Vector3((float)i, -1.0f + (float)j, (float)k));
					break;

				case TILE_GOAL:
					m_block[i][j][k] = std::make_unique<Obj3D>();
					m_block[i][j][k]->Initialize(m_deviceResources, m_states);
					m_block[i][j][k]->SetModel(m_model[TILE_GOAL].get());
					m_block[i][j][k]->SetPosition(Vector3((float)i, -1.5f + (float)j, (float)k));
					break;

				default:
					break;
				}

			}

		}
	}

}

void Dungeon::Update(float elapsedTime)
{
	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_HEIGHT; j++)
		{
			for (int k = 0; k < MAZE_LENGTH; k++)
			{
				if (m_block[i][j][k] != nullptr)
				{
					m_block[i][j][k]->Update(elapsedTime);

				}
			}
		}
	}
}

void Dungeon::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix & projection)
{
	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_HEIGHT; j++)
		{
			for (int k = 0; k < MAZE_LENGTH; k++)
			{
				if (m_block[i][j][k] != nullptr)
				{
					if (k < 9)
					{
						m_block[i][j][k]->Render(view, projection);

					}
				}
			}
		}
	}
}

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