#include "..\..\pch.h"
#include "Dungeon.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

const float Dungeon::BLOCK_SPEED = 0.2f;

Dungeon::Dungeon():
	m_blockAlpha(nullptr), m_blockBeta{ nullptr }, m_spawnPosAlpha(Vector3::Zero), m_spawnPosBeta { Vector3::Zero },
	m_block{ nullptr }, m_data { TILE_NONE } , m_loader (nullptr)
{
}


Dungeon::~Dungeon()
{

	// ブロック領域の破棄
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

	for (int i = 0; i < BLOCK_MAXCOUNT; i++)
	{
		if (m_blockBeta[i] != nullptr)
		{
			delete m_blockBeta[i];
			m_blockBeta[i] = nullptr;
		}

	}

	if (m_blockAlpha != nullptr)
	{
		delete m_blockAlpha;
		m_blockAlpha = nullptr;

	}

}

void Dungeon::Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates * states)
{
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//引数から設定する
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	m_states = states;

	EffectFactory fx(device);
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//データ制作
	m_loader = new DataLoad();
	m_loader->LoadData(L"Stage00.csv");




	//モデル制作
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

	//呼んだデータを変換する
	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_HEIGHT; j++)
		{
			for (int k = 0; k < MAZE_LENGTH; k++)
			{

				switch (m_loader->GetData()[i][j][k])
				{
				case 0: m_data[i][j][k] = TILE_NONE;  break;
				case 1: m_data[i][j][k] = TILE_BLOCK1;	break;
				case 2: m_data[i][j][k] = TILE_BLOCK2;	break;
				case 3: m_data[i][j][k] = TILE_GOAL;	break;
				}


			}
		}
	}

	// ブロックを作成
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

void Dungeon::Update(float elapsedTime)
{
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

		//仮ブロックまだなかったら作成処理
		if (m_blockAlpha == nullptr)
		{
			//乱数のシードをリセットする。
			//rand((unsigned int)time(NULL));
			m_spawnPosAlpha.x = (float)1 + rand() % 9;
			m_spawnPosAlpha.y = (float)8;
			m_spawnPosAlpha.z = (float)1 + rand() % 9;

			m_blockAlpha = new Obj3D();
			m_blockAlpha->Initialize(m_deviceResources, m_states);
			m_blockAlpha->SetModel(m_model[TILE_BLOCK1].get());
			m_blockAlpha->SetPosition(Vector3((float)m_spawnPosAlpha.x, -1.0f + (float)m_spawnPosAlpha.y, -0.5f + (float)m_spawnPosAlpha.z));

			//for (int i = 0; i < BLOCK_MAXCOUNT; i++)
			//{
			//	//乱数のシードをリセットする。
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

		//仮ブロックあったら落ちる処理動きます
		if (m_blockAlpha != nullptr)
		{
			static int CD = 10;
			CD--;
			m_blockAlpha->Update(elapsedTime);
			for (int i = 0; i < BLOCK_MAXCOUNT; i++)
			{
				if (m_blockBeta[i] != nullptr)
				{
					m_blockBeta[i]->Update(elapsedTime);
				}
			}

			if (IsMovable(Vector3((m_spawnPosAlpha.x), (m_spawnPosAlpha.y - Dungeon::BLOCK_SPEED), (m_spawnPosAlpha.z))))
			{
				if (CD < 0)
				{
					m_spawnPosAlpha.y -= Dungeon::BLOCK_SPEED;
					m_blockAlpha->SetPosition(Vector3((float)m_spawnPosAlpha.x, -1.0f + (float)m_spawnPosAlpha.y, -0.5f + (float)m_spawnPosAlpha.z));
					for (int i = 0; i < BLOCK_MAXCOUNT; i++)
					{
						if (m_blockBeta[i] != nullptr)
						{
							m_spawnPosBeta[i].y -= Dungeon::BLOCK_SPEED;
							m_blockBeta[i]->SetPosition(Vector3((float)m_spawnPosBeta[i].x, -1.0f + (float)m_spawnPosBeta[i].y, -0.5f + (float)m_spawnPosBeta[i].z));
						}
					}

					CD = 10;
				}
			}
			else
			{
				{
					int i = (int)m_spawnPosAlpha.x;
					int j = (int)m_spawnPosAlpha.y;
					int k = (int)m_spawnPosAlpha.z;
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

					if (m_blockAlpha != nullptr)
					{
						delete m_blockAlpha;
						m_blockAlpha = nullptr;

					}
					for (int l = 0; l < BLOCK_MAXCOUNT; l++)
					{
						{
							int i = (int)m_spawnPosBeta[l].x;
							int j = (int)m_spawnPosBeta[l].y;
							int k = (int)m_spawnPosBeta[l].z;
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

							if (m_blockBeta[l] != nullptr)
							{
								delete m_blockBeta[l];
								m_blockBeta[l] = nullptr;
							}
						}
					}

				}

			}


		}

		//if (m_block[1][0][1] != nullptr)
		//{
		//	m_block[1][0][1].reset();
		//	m_block[1][0][1] = nullptr;
		//	m_data[1][0][1] = TILE_NONE;

		//}
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
					if (m_data[i][j][k] != 0)
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

	if (m_blockAlpha != nullptr)
	{
		m_blockAlpha->Render(view, projection);
	}

	for (int i = 0; i < BLOCK_MAXCOUNT; i++)
	{
		if (m_blockBeta[i] != nullptr)
		{
			m_blockBeta[i]->Render(view, projection);

		}
	}
}


////=====================================================================================================////
/*マップチップによる当たり判定*/
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

//bool Dungeon::IsMovable(DirectX::SimpleMath::Vector3 position)
//{
//	if (m_data[(int)position.x][(int)position.y][(int)position.z] != TILE_BLOCK1 &&
//		m_data[(int)position.x][(int)position.y][(int)position.z] != TILE_BLOCK2)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//
//}
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
	if (m_data[(int)position.x][(int)position.y][(int)position.z] == TILE_NONE)
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
