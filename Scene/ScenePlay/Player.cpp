#include "..\..\pch.h"
#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player()
{
}


Player::~Player()
{
}

void Player::Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates * states)
{
	ID3D11Device* device = deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = deviceResources->GetD3DDeviceContext();

	// ���f����ǂݍ���
	EffectFactory fx(device);

	device;

	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

	//�I�u�W�F�N�g2D���擾����
	CreateWICTextureFromFile(device, L"Resources\\Textures\\charatest(front).png", nullptr, m_playerTexture.GetAddressOf());
	m_player = std::make_unique<Obj2D>();
	m_player->SetPosition(Vector3(5.0f, 3.0f, 5.0f));
	m_player->Initialize(deviceResources, states);
	m_player->SetTexture(m_playerTexture.Get());


}

void Player::Update(float elapsedTime)
{
	//�v���C���[�̍X�V
	m_player->Update(elapsedTime);

}

void Player::Render(Vector3 eye, Matrix view, Matrix projection)
{

	m_player->Render(eye, view, projection);

}

void Player::Reset()
{
	m_player.reset();

}

DirectX::SimpleMath::Vector3 Player::playerPositionToCamera()
{
	//�J�����̈ʒu
	return DirectX::SimpleMath::Vector3(m_player->GetPosition().x + 0.0f, m_player->GetPosition().y + 5.0f, m_player->GetPosition().z + 5.0f);
}
