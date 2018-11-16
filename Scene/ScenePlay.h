#pragma once
#include "SceneBase.h"
#include "..\Utility\Number.h"
#include "..\Object\Obj2D.h"
#include "..\Object\Obj3D.h"

class ScenePlay : public SceneBase
{
public:
	ScenePlay();
	~ScenePlay();

	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states) override;
	SceneBase* Update(float elapsedTime) override;
	void Render() override;
	void Reset() override;

private:
	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	
	//�Q�[�����̃^�C�}�[�n���h��
	float m_gameTimerCD;
	Number* m_gameTimer;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;


	//�v���C���[�̃n���h��
	std::unique_ptr<Obj2D> m_player;

	//�^�C���̃n���h��
	std::unique_ptr<Obj3D> m_obj3D;


	//
	DirectX::SimpleMath::Vector3 playerPositionToCamera();

};