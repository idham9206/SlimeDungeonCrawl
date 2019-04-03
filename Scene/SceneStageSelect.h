////====================================================////
////		�@�X�e�[�W�Z���N�g�V�[���̃N���X	�@		////
////													////
////				��ҁF���n�}�h�C�_��				////
////====================================================////
#pragma once
#include "SceneBase.h"

#include "..\Utility\Number.h"
#include "..\Utility\Blink.h"

#include "..\Object\Obj2D.h"
#include "..\Object\Obj3D.h"

class SceneSelect : public SceneBase
{
public:
	SceneSelect();
	~SceneSelect();

	//������
	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states) override;
	//�X�V
	SceneBase* Update(float elapsedTime) override;
	//�`��
	void Render() override;
	//���
	void Reset() override;


private:
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;


	// �X�e�[�W�Z���N�g�̃X�v���C�g
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureSelect;
	// �Ӎ߂̃X�v���C�g
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureSazai;

	//�_�Ńt���O
	Blink* m_blink;
};