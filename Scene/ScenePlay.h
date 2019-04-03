////====================================================////
////			�@	�v���C�V�[���̃N���X	�@			////
////													////
////				��ҁF���n�}�h�C�_��				////
////====================================================////
#pragma once
#include "SceneBase.h"

#include "..\Utility\Number.h"
#include "..\Object\Obj2D.h"
#include "..\Object\Obj3D.h"

//#include "..\Effect\Myeffect.h"
//#include "..\Effect\EffectManager.h"

#include "ScenePlay\Dungeon.h"
#include "ScenePlay\Player.h"

class ScenePlay : public SceneBase
{
public:
	ScenePlay();
	~ScenePlay();

	//������
	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states) override;
	//�X�V
	SceneBase* Update(float elapsedTime) override;
	//�`��
	void Render() override;
	//���
	void Reset() override;

	DirectX::SimpleMath::Vector3 PlayerPositionToCamera();

private:
	//�Q�[�����̃^�C�}�[�n���h��
	float m_gameTimerCD;
	Number* m_gameTimer;

	//�_���W�����̃f�[�^���n���h��
	const wchar_t * m_dungeonData;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;

	//�_���W�����̃n���h��
	Dungeon* m_dungeon;

	//�v���C���[�̃n���h��
	std::unique_ptr<Player> m_player;

	// �u���b�N�A�E�g�G�t�F�N�g�X�v���C�g�o�b�`
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureShadow;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureGoal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureGameOver;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureTNTUI;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureMagma;

	//�L�[�{�[�h�̉�������̏��n���h��
	DirectX::Keyboard::KeyboardStateTracker m_tracker;

	//�}�O�}�̃|�W�V����
	DirectX::SimpleMath::Vector2 m_posMagma[2];

	//
	DirectX::SimpleMath::Vector2 m_positionOver;

	//
	float m_startCD;

	//
	bool m_clearState;
	bool m_startState;
	bool m_gameOverState;

	//EffectManager*							m_effectManager;

public:
	//�_���W�����̍쐬
	void CreateDungeon(const wchar_t * data);


};