////====================================================////
////			 �I�u�W�F�N�g2D�֘A�N���X				////
////		3D�̋�Ԃ�2D�I�u�W�F�N�g��`��ł��B		////
////	�@	�@��{�r���{�[�h�֐��g���Ă�̂� �A�@		////
////			��ɃJ�����̕����Ɍ����Ă܂��B			////
////													////
////				��ҁF���n�}�h�C�_��				////
////====================================================////
#pragma once
#include "..\DeviceResources.h"

class Obj2D
{
private:
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;

	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;

	DirectX::SimpleMath::Vector2 m_scale;

	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;

	DirectX::SimpleMath::Vector4 m_spriteFrame;

	float m_speed;

	int m_count;

	float m_frametime;

	int m_frameCountX;
	int m_frameCountY;

	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	
	// �e�N�X�`���n���h��
	ID3D11ShaderResourceView* m_texture;

	//���̃f�o�C�X���\�[�V�Y
	DX::DeviceResources* m_deviceResources;

	//���̃R�����X�e�[�c
	DirectX::CommonStates* m_states;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �I�u�W�F�N�g����ɕK�v�Ȃ��̂܂Ƃ�
	void Create(DX::DeviceResources * deviceResources, DirectX::CommonStates* states);

public:
	Obj2D();
	~Obj2D();

	//������
	void Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates* states, int frameCountX, int frameCountY = 1);

	// �X�V
	void Update(float elapsedTime);

	// �`��
	void Render(DirectX::SimpleMath::Vector3 cameraEye, DirectX::SimpleMath::Matrix view,
		const DirectX::SimpleMath::Matrix& projection);



	void SetTexture(ID3D11ShaderResourceView* texture) { m_texture = texture; }
	// �ʒu��ݒ肷��֐�
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

	// ��]��ݒ肷��֐�
	void SetRotation(DirectX::SimpleMath::Quaternion rotation) { m_rotation = rotation; }

	void DrawSprite3D(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, 
		const DirectX::SimpleMath::Matrix& projection, DirectX::SimpleMath::Vector4& spriteFrame);
	//�v���C���[�̈ʒu���擾����֐�
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	DirectX::SimpleMath::Matrix Scale(float x, float y);
};

