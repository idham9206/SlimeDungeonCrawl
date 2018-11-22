#include "..\pch.h"
#include "Obj3D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Obj3D::Obj3D()
	: m_model(nullptr)
{
}

void Obj3D::Initialize(DX::DeviceResources* deviceResources, CommonStates* states)
{
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//引数から設定する
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	m_states = states;

	EffectFactory fx(device);
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

}

void Obj3D::Update(float elapsedTime)
{
	// ワールド行列を作成
	m_world =	Matrix::CreateFromQuaternion(m_rotation)
			*	Matrix::CreateTranslation(m_position)
			*	Matrix::CreateScale(1.0f);

	
}

void Obj3D::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix& projection)
{
	// モデルの描画
	m_model->Draw(m_deviceResources->GetD3DDeviceContext(), *m_states,
		m_world, view, projection);
}
