#pragma once
#include "SceneBase.h"

#include "..\Utility\Number.h"
#include "..\Object\Obj2D.h"
#include "..\Object\Obj3D.h"

#include "ScenePlay\Dungeon.h"
#include "ScenePlay\Player.h"

class ScenePlay : public SceneBase
{
public:
	ScenePlay();
	~ScenePlay();

	void Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* states) override;
	SceneBase* Update(float elapsedTime) override;
	void Render() override;
	void Reset() override;

	bool IsMovable(DirectX::SimpleMath::Vector3 position);
	bool IsGoal(DirectX::SimpleMath::Vector3 position);


private:
	//�Q�[�����̃^�C�}�[�n���h��
	float m_gameTimerCD;
	Number* m_gameTimer;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;

	//�_���W�����̃n���h��
	Dungeon* m_dungeon;

	std::unique_ptr<Player> m_player;
};