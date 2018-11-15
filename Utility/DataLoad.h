#pragma once
class DataLoad
{
/// <summary>
/// �����o�ϐ��̐錾
/// </summary>
/// 
private:
	int***	m_data;
	int		m_mapWidth;			// �}�b�v�̕�
	int		m_mapHeight;		// �}�b�v�̍���
	int		m_mapLength;		//�@�}�b�v�̉��s


public:
	DataLoad();
	~DataLoad();

public:
	bool LoadData(const wchar_t* filename);
	void Feed();

	int*** GetData()
	{
		return m_data;
	}
	int GetWidth()
	{
		return m_mapWidth;
	}
	int GetHeight()
	{
		return m_mapHeight;
	}
	int GetLength()
	{
		return m_mapLength;
	}
};

