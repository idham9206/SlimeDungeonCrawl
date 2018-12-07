////====================================================////
////				�x�[�X�V�[���@�N���X				////
////													////
////				��ҁF���n�}�h�C�_��				////
////====================================================////
#pragma once

#include "../DeviceResources.h"
#include <CommonStates.h>

class SceneBase
{
public:
	SceneBase();
	~SceneBase();

	virtual void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states) = 0;
	virtual SceneBase* Update(float elapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Reset() = 0;

	//===============================================================
	//�Z�b�^�[�Q�b�^�[�܂Ƃ�
	//�r���[
	void SetView(DirectX::SimpleMath::Matrix view) { m_view = view; }
	DirectX::SimpleMath::Matrix GetView() { return m_view; }

	//�A�C
	void SetEye(DirectX::SimpleMath::Vector3 eye) { m_eye = eye; }
	DirectX::SimpleMath::Vector3 GetEye() { return m_eye; }

	//�ˉe
	void SetProjection(DirectX::SimpleMath::Matrix projection) { m_projection = projection; }
	DirectX::SimpleMath::Matrix GetProjection() { return m_projection; }

	//���[���h
	void SetWorld(DirectX::SimpleMath::Matrix world) { m_world = world; }
	DirectX::SimpleMath::Matrix GetWorld() { return m_world; }


protected:
	//���̃f�o�C�X���\�[�V�Y
	DX::DeviceResources* m_deviceResources;
	//���̃R�����X�e�[�c
	DirectX::CommonStates* m_states;

	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;

	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;

	//�A�C�s��
	DirectX::SimpleMath::Vector3 m_eye;

};

