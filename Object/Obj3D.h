#pragma once

#include "../DeviceResources.h"

// ���f����\������N���X
class Obj3D
{
protected:

	// ���f���n���h��
	std::unique_ptr<DirectX::Model> m_model;

	//���̃f�o�C�X���\�[�V�Y
	DX::DeviceResources* m_deviceResources;
	
	//���̃R�����X�e�[�c
	DirectX::CommonStates* m_states;


	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;

	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;

	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;

public:
	// �R���X�g���N�^
	Obj3D();

	//������
	void Initialize(const wchar_t* textureAddress, DX::DeviceResources* deviceResources, DirectX::CommonStates* states);

	// �X�V
	void Update(float elapsedTime);

	// �`��
	void Render(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix& projection);

	//// ���f���n���h����ݒ肷��֐�
	//void SetModel(DirectX::Model* model) { m_model = model; }

	// �ʒu��ݒ肷��֐�
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

	// ��]��ݒ肷��֐�
	void SetRotation(DirectX::SimpleMath::Quaternion rotation) { m_rotation = rotation; }

};
