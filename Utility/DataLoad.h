////====================================================////
////				データ読み込みクラス				////
////													////
////				作者：ムハマドイダム				////
////====================================================////
#pragma once
class DataLoad
{
/// <summary>
/// メンバ変数の宣言
/// </summary>
/// 
private:
	int***	m_data;
	int		m_mapWidth;			// マップの幅
	int		m_mapHeight;		// マップの高さ
	int		m_mapLength;		//　マップの奥行


public:
	DataLoad();
	~DataLoad();

public:
	bool LoadData(const wchar_t* filename);
	// データの読み込み関数
	void Feed();

	int*** GetData()
	{
		return m_data;
	}
	void SetData(int*** data)
	{
		m_data = data;
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

