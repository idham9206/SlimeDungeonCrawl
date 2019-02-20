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
		// データ領域の破棄
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
	// ファイルのオープン -------------------------------------------
	ifstream ifs(wstring(L"Resources/Data/") + filename);    // 入力用ファイルストリーム
	if (ifs.fail()) {
		wstring errorMessage = wstring(L"\"") + filename + wstring(L"\"が開けません");
		MessageBox(NULL, errorMessage.c_str(), NULL, MB_OK | MB_ICONWARNING);
		return false;    // 読み込み失敗
	}


	// ファイルの読み込み -------------------------------------------
	string line;      // 1行用バッファ

	// ヘッダの読み込み
	if (getline(ifs, line))
	{
		char delimiter;
		istringstream header(line);    // 入力用文字列ストリーム
		header >> m_mapWidth >> delimiter >> m_mapHeight >> delimiter >> m_mapLength;

	}
	else
	{
		return false;
	}


	// データ領域の生成
	m_data = new int**[m_mapWidth];
	for (int i = 0; i < m_mapWidth; i++)
	{
		m_data[i] = new int*[m_mapHeight];
		for (int j = 0; j < m_mapHeight; j++)
		{
			m_data[i][j] = new int[m_mapLength];
		}

	}


	// データの読み込み
	string field;     // フィールド用バッファ  
	string length;
	int    gx, gy, gz;    // マップのグリッド座標

	gy = 0;
	gz = 0;
	while (getline(ifs, line))
	{
		istringstream iss(line);    // 入力用文字列ストリーム

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

	

	return true;    // 読み込み成功
}