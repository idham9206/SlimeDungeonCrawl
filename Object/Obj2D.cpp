#include "..\pch.h"
#include "Obj2D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


Obj2D::Obj2D():
	m_scale(Vector2(1.0f, 1.0f)),
	m_position(Vector2(1.0f, 1.0f)),
	m_frameTimeResetter(30.0f),
	m_degreesY(0)
{
}


Obj2D::~Obj2D()
{
}

void Obj2D::Initialize(DX::DeviceResources * deviceResources, DirectX::CommonStates* states, int frameCountX, int frameCountY)
{
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//引数から設定する

	Create(deviceResources, states);

	m_frameCountX = frameCountX;
	m_frameCountY = frameCountY;
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

	m_spriteFrame.x = 0.0f;
	m_spriteFrame.y = 0.0f;
	m_spriteFrame.w = (float)1 / m_frameCountX;
	m_spriteFrame.z = (float)1 / m_frameCountY;
	m_frametime = m_frameTimeResetter;

	m_speed = 2.0f;
	m_count = 10;

}

void Obj2D::Update(float elapsedTime)
{
	m_frametime--;
	if (m_frametime < 0.0f)
	{
		m_spriteFrame.x += (float)1 / m_frameCountX;
		m_spriteFrame.w += (float)1 / m_frameCountX;
		m_frametime = m_frameTimeResetter;
	}

	if (m_loop)
	{
		if (m_spriteFrame.x > 1.0f - ((float)1 / m_frameCountX))
		{
			m_spriteFrame.x = 0.0f;
		}
		if (m_spriteFrame.w > 1.0f)
		{
			m_spriteFrame.w = (float)1 / m_frameCountX;
		}

	}
	else
	{
		if (m_spriteFrame.x > 1.0f - ((float)1 / m_frameCountX))
		{
			m_spriteFrame.x = 1.0f - ((float)1 / m_frameCountX);
		}
		if (m_spriteFrame.w > 1.0f)
		{
			m_spriteFrame.w = 1.0f;
		}

	}

	if (m_spriteFrame.x > 1.0f - ((float)1 / m_frameCountX))
	{
		m_spriteFrame.x = 0.0f;
	}
	if (m_spriteFrame.w > 1.0f)
	{
		m_spriteFrame.w = (float)1 / m_frameCountX;
	}
	//m_scale.x = cosf(x/30);
}

void Obj2D::Render(Vector3 cameraEye, DirectX::SimpleMath::Matrix view,
	const DirectX::SimpleMath::Matrix& projection)
{
	Matrix world;
	world = Scale(m_scale.x, m_scale.y)*
		Matrix::CreateBillboard(Vector3((float)this->m_position.x, (float) this->m_position.y, (float) this->m_position.z),
		cameraEye,
		Vector3::Up
	);

	DrawSprite3D(world, view, projection, m_spriteFrame);

}

void Obj2D::SetFrameCount(int x, int y)
{
	m_frameCountX = x;
	m_frameCountY = y;
}

void Obj2D::ResetFrame()
{
	m_spriteFrame.x = 0.0f;
	m_spriteFrame.y = 0.0f;
	m_spriteFrame.w = (float)1 / m_frameCountX;
	m_spriteFrame.z = (float)1 / m_frameCountY;
}

void Obj2D::Create(DX::DeviceResources * deviceResources, DirectX::CommonStates* states)
{
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
	//引数から設定する
	m_deviceResources = deviceResources;
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	m_states = states;
	//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

	// エフェクトの作成
	m_batchEffect = std::make_unique<AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);


	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());
	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

}

void Obj2D::DrawSprite3D(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, const DirectX::SimpleMath::Matrix& projection, DirectX::SimpleMath::Vector4 & spriteFrame)
{

	auto context = m_deviceResources->GetD3DDeviceContext();

	float X = spriteFrame.x;
	float Y = spriteFrame.y;
	float W = spriteFrame.w;
	float H = spriteFrame.z;
	// 頂点情報（ここは自分で設定してください。）
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(Vector3(1.0f, 1.0f, 0.0f), Vector2(X, Y)),
		VertexPositionTexture(Vector3(-1.0f, 1.0f, 0.0f), Vector2(W, Y)),
		VertexPositionTexture(Vector3(-1.0f, -1.0f, 0.0f), Vector2(W, H)),
		VertexPositionTexture(Vector3(1.0f,-1.0f, 0.0f), Vector2(X, H)),
	};

	// 半透明で描画
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	// 深度バッファに書き込まないが参照だけする
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// 不透明のみ描画する設定
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	m_batchEffect->Apply(context);

	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(view);
	m_batchEffect->SetProjection(projection);
	m_batchEffect->SetTexture(m_texture);
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	/// <summary>
	/// =======================================
	/// </summary>
	// 不透明部分を描画
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);

	m_batch->End();



	// 半透明で描画
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	// 深度バッファに書き込まないが参照だけする
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// 不透明のみ描画する設定
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	m_batchEffect->Apply(context);
	/// <summary>
	/// =======================================
	/// </summary>
	// 不透明部分を描画
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();


}

DirectX::SimpleMath::Matrix Obj2D::Scale(float x, float y)
{
	Matrix scale;
	
	scale = Matrix{
		Vector3(x, 0, 0),
		Vector3(0, y, 0),
		Vector3(0, 0, 1),
	};
	return scale;
}
