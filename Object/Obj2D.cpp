#include "..\pch.h"
#include "Obj2D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


Obj2D::Obj2D()
{
}


Obj2D::~Obj2D()
{
}

void Obj2D::Initialize(const wchar_t* textureAddress, DX::DeviceResources * deviceResources, DirectX::CommonStates* states)
{

	Create(deviceResources, states, textureAddress);

	m_spriteFrame.x = 0.0f;
	m_spriteFrame.y = 0.0f;
	m_spriteFrame.w = 0.25f;
	m_spriteFrame.z = 1.0f;
	m_frametime = 10.0f;

	m_speed = 2.0f;
	m_count = 10;

}

void Obj2D::Update(float elapsedTime)
{
	m_frametime--;
	if (m_frametime < 0.0f)
	{
		m_spriteFrame.x += 0.25f;
		m_spriteFrame.w += 0.25f;
		m_frametime = 10.0f;
	}


	if (m_spriteFrame.x > 0.75f)
	{
		m_spriteFrame.x = 0.0f;
	}
	if (m_spriteFrame.w > 1.0f)
	{
		m_spriteFrame.w = 0.25f;
	}

}

void Obj2D::Render(Vector3 cameraEye, DirectX::SimpleMath::Matrix view,
	const DirectX::SimpleMath::Matrix& projection)
{
	Matrix world;
	world = Matrix::CreateBillboard(
		Vector3((float)this->m_position.x, (float) this->m_position.y, (float) this->m_position.z),
		cameraEye,
		Vector3::Up
	);
	DrawSprite3D(world, view, projection, m_spriteFrame);

}

void Obj2D::Create(DX::DeviceResources * deviceResources, DirectX::CommonStates* states, const wchar_t* textureAddress)
{
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//��������ݒ肷��
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	m_states = states;
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

	// �G�t�F�N�g�̍쐬
	m_batchEffect = std::make_unique<AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);


	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());
	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//�e�N�X�`���[����
	CreateWICTextureFromFile(device, textureAddress,
		nullptr, m_texture.GetAddressOf());

}

void Obj2D::DrawSprite3D(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, const DirectX::SimpleMath::Matrix& projection, DirectX::SimpleMath::Vector4 & spriteFrame)
{

	auto context = m_deviceResources->GetD3DDeviceContext();

	float X = spriteFrame.x;
	float Y = spriteFrame.y;
	float W = spriteFrame.w;
	float H = spriteFrame.z;
	// ���_���i�����͎����Őݒ肵�Ă��������B�j
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(Vector3(1.0f, 1.0f, 0.0f), Vector2(X, Y)),
		VertexPositionTexture(Vector3(-1.0f, 1.0f, 0.0f), Vector2(W, Y)),
		VertexPositionTexture(Vector3(-1.0f, -1.0f, 0.0f), Vector2(W, H)),
		VertexPositionTexture(Vector3(1.0f,-1.0f, 0.0f), Vector2(X, H)),
	};

	// �������ŕ`��
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������܂Ȃ����Q�Ƃ�������
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// �s�����̂ݕ`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	m_batchEffect->Apply(context);

	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(view);
	m_batchEffect->SetProjection(projection);
	//m_batchEffect->SetWorld(DirectX::SimpleMath::Matrix::Identity);
	//m_batchEffect->SetView(DirectX::SimpleMath::Matrix::Identity);
	//m_batchEffect->SetProjection(DirectX::SimpleMath::Matrix::Identity);
	m_batchEffect->SetTexture(m_texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	/// <summary>
	/// =======================================
	/// </summary>
	// �s����������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);

	m_batch->End();



	// �������ŕ`��
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������܂Ȃ����Q�Ƃ�������
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// �s�����̂ݕ`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	m_batchEffect->Apply(context);
	/// <summary>
	/// =======================================
	/// </summary>
	// �s����������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();


}