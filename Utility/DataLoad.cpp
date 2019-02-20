#include "..\pch.h"
#include <fstream>
#include <sstream>
#include <string>

#include <iostream>

#include "DataLoad.h"

using namespace std;


DataLoad::DataLoad()
{

}


DataLoad::~DataLoad()
{
	if (m_data != nullptr)
	{
		// �f�[�^�̈�̔j��
		for (int i = 0; i < m_mapWidth; i++)
		{
			for (int j = 0; j < m_mapHeight; j++)
			{
				delete m_data[i][j];
				m_data[i][j] = nullptr;
			}
			delete m_data[i];
			m_data[i] = nullptr;
		}
		delete m_data;
		m_data = nullptr;

	}
}


void DataLoad::Feed()
{
	for (int k = 0; k < m_mapLength; k++)
	{
		for (int j = 0; j < m_mapHeight; j++)
		{
			cout << endl;
			for (int i = 0; i < m_mapWidth; i++)
			{
				cout << m_data[i][j][k];
			}
		}
		cout << endl << endl;
	}
}

bool DataLoad::LoadData(const wchar_t * filename)
{
	// �t�@�C���̃I�[�v�� -------------------------------------------
	ifstream ifs(wstring(L"Resources/Data/") + filename);    // ���͗p�t�@�C���X�g���[��
	if (ifs.fail()) {
		wstring errorMessage = wstring(L"\"") + filename + wstring(L"\"���J���܂���");
		MessageBox(NULL, errorMessage.c_str(), NULL, MB_OK | MB_ICONWARNING);
		return false;    // �ǂݍ��ݎ��s
	}


	// �t�@�C���̓ǂݍ��� -------------------------------------------
	string line;      // 1�s�p�o�b�t�@

	// �w�b�_�̓ǂݍ���
	if (getline(ifs, line))
	{
		char delimiter;
		istringstream header(line);    // ���͗p������X�g���[��
		header >> m_mapWidth >> delimiter >> m_mapHeight >> delimiter >> m_mapLength;

	}
	else
	{
		return false;
	}


	// �f�[�^�̈�̐���
	m_data = new int**[m_mapWidth];
	for (int i = 0; i < m_mapWidth; i++)
	{
		m_data[i] = new int*[m_mapHeight];
		for (int j = 0; j < m_mapHeight; j++)
		{
			m_data[i][j] = new int[m_mapLength];
		}

	}


	// �f�[�^�̓ǂݍ���
	string field;     // �t�B�[���h�p�o�b�t�@  
	string length;
	int    gx, gy, gz;    // �}�b�v�̃O���b�h���W

	gy = 0;
	gz = 0;
	while (getline(ifs, line))
	{
		istringstream iss(line);    // ���͗p������X�g���[��

		gx = 0;
		if (gz > (m_mapHeight - 1))
		{
			gz = 0;
			gy++;
		}

		while (getline(iss, field, ','))
		{
			if (field == "#")
			{
				gz--;
				break;
			}
			else
			{
				m_data[gx][gy][gz] = stoi(field);
				gx++;

			}

		}
		gz++;

	}

	

	return true;    // �ǂݍ��ݐ���
}