#include "..\pch.h"
#include "Number.h"



Number::Number(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale) 
	: m_position(position), m_scale(scale)
	,m_sprites(nullptr)
{
}


Number::~Number()
{
	m_sprites.reset();

	m_texture.Reset();

	m_states.reset();
}

void Number::Initialize()
{

	//�X�R�A������
	m_numberCount = 0;

	for (int i = 0; i < MAX_BOXCOUNT; i++)
	{
		m_positionNumber[i].x = (float)m_position.x - (m_scale.x) *((FRAME_WIDTH + 4) * i);
		m_positionNumber[i].y = (float)m_position.y - (m_scale.y);

		m_numberCountNum[i] = 0;

	}

}

void Number::Update()
{
	float imageL; //
	float imageT;
	float imageR; //
	float imageB;

	int checkColumn;				//	���[�v�񐔂𐧌�
	int loopCount = 0;				//	�ϐ�g_scoreCountNum�̓Y����


	for (int i = 0; i < MAX_BOXCOUNT; i++)
	{
		m_numberCountNum[i] = 0; //�g���ĂȂ��Ƃ����0�ɂ���
	}


	for (checkColumn = m_numberCount; checkColumn > 0; checkColumn /= 10)
	{
		//	���[�v�񐔂ɑΉ����錅�ɒl������
		m_numberCountNum[loopCount] = (checkColumn % 10);

		loopCount++;
	}

	//�l��0~9�ȊO�������烊�Z�b�g
	for (int i = 0; i < MAX_BOXCOUNT; i++)
	{

		if (m_numberCountNum[i] >= 10)
		{
			m_numberCountNum[i] = 0;
		}
		if (m_numberCountNum[i] <= 0)
		{
			m_numberCountNum[i] = 0;

		}

	}


	for (int i = 0; i < 5; i++)
	{
		imageT = 0.0f;
		imageL = m_numberCountNum[i] * (float)FRAME_WIDTH;
		imageR = (imageL + (float)FRAME_WIDTH);
		imageB = (float)FRAME_HEIGHT;

		m_rect[i].top = (long)imageT;
		m_rect[i].left = (long)imageL;
		m_rect[i].right = (long)imageR;
		m_rect[i].bottom = (long)imageB;

	}

}

void Number::Draw()
{
	m_sprites->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());
	//�i���o�[�摜�̌��`��
	for (int i = 0; i < MAX_BOXCOUNT; i++)
	{
		

		if (i > 0)
		{
			if (m_numberCountNum[i] < 1)
			{
				if (m_numberCountNum[i + 1] > 0)
				{
					if (i < 4)
					{
						m_sprites->Draw(m_texture.Get(), m_positionNumber[i], &m_rect[i], DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 0.0f), m_scale, DirectX::SpriteEffects_None, 0.0f);
						m_sprites->Draw(m_texture.Get(), m_positionNumber[i-1], &m_rect[i-1], DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 0.0f), m_scale, DirectX::SpriteEffects_None, 0.0f);
						
						if (i > 1)
						{
							m_sprites->Draw(m_texture.Get(), m_positionNumber[i - 2], &m_rect[i - 2], DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 0.0f), m_scale, DirectX::SpriteEffects_None, 0.0f);

						}
					}

				}
			}
			else
			{
				m_sprites->Draw(m_texture.Get(), m_positionNumber[i], &m_rect[i], DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 0.0f), m_scale, DirectX::SpriteEffects_None, 0.0f);

			}

		}
		else
		{
			m_sprites->Draw(m_texture.Get(), m_positionNumber[i], &m_rect[i], DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 0.0f), m_scale, DirectX::SpriteEffects_None, 0.0f);

		}
		

	}
	m_sprites->End();
}

void Number::Create(DX::DeviceResources * deviceResources, const wchar_t* textureAddress)
{
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();


	// �X�v���C�g�o�b�`�̍쐬
	m_sprites = std::make_unique<DirectX::SpriteBatch>(context);

	// �R�����X�e�[�g�̍쐬
	m_states = std::make_unique<DirectX::CommonStates>(device);

	// �e�N�X�`���̃��[�h
	DirectX::CreateWICTextureFromFile(device, textureAddress, nullptr, m_texture.GetAddressOf());


}

void Number::SetNumber(int score)
{
	m_numberCount = score;
}

int Number::GetNumber()
{
	return m_numberCount;
}
