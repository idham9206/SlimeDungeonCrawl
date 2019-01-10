////====================================================////
////			�@	�v���C�V�[���̃N���X	�@			////
////													////
////				��ҁF���n�}�h�C�_��				////
////====================================================////
#pragma once
#include "SceneBase.h"

#include "..\Utility\Number.h"
#include "..\Utility\Blink.h"

#include "..\Object\Obj2D.h"
#include "..\Object\Obj3D.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	~SceneTitle();

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
	std::unique_ptr<DirectX::SpriteBatch> m_spritesStart;


	// �u���b�N�A�E�g�G�t�F�N�g�X�v���C�g�o�b�`
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureTitle;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureStart;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureBoard;

	//�L�[�{�[�h�̉�������̏��n���h��
	DirectX::Keyboard::KeyboardStateTracker m_tracker;

	//�_�Ńt���O
	Blink* m_blink;
};