#pragma once
#include "..\DeviceResources.h"

class Number
{
public:
	Number(DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2(1.f, 1.f));
	~Number();

public:
	// �J�n����
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Draw();
	//�쐬
	void Create(DX::DeviceResources* deviceResources, const wchar_t* textureAddress);

	//�Z�b�^�[
	void SetNumber(int score);
	//�Q�b�^�[
	int GetNumber();


private:
	//���̃f�o�C�X���\�[�V�Y
	DX::DeviceResources* m_deviceResources;

	//��`�ϐ�
	static const int MAX_BOXCOUNT = 5; 	//�������̍ő��
	static const int FRAME_WIDTH = 16; //�P�t���[���̕�
	static const int FRAME_HEIGHT = 24;//�P�t���[���̍���

	//�؂���p�̕ϐ�
	RECT m_rect[MAX_BOXCOUNT];
	//���̔ԍ�
	int m_numberCountNum[MAX_BOXCOUNT];
	//�X�R�A
	int m_numberCount;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;
	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;

	//�\���̈ʒl
	DirectX::SimpleMath::Vector2 m_position;
	//�e�����̈ʒl
	DirectX::SimpleMath::Vector2 m_positionNumber[MAX_BOXCOUNT];
	//�g�嗦
	DirectX::SimpleMath::Vector2 m_scale;
};

