#include "pch.h"
#include "EffectManager.h"
#include <WICTextureLoader.h>
#include "BinaryFile.h"
#include "d3d11.h"
#include <Effects.h>
#include <Model.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;

const std::vector<D3D11_INPUT_ELEMENT_DESC> EffectManager::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
void EffectManager::Create(DX::DeviceResources * deviceResources, int count)
{
	m_deviceResources = deviceResources;
	auto device = m_deviceResources->GetD3DDevice();

	const wchar_t* name1 = L"Resources\\Textures\\melon.png";
	const wchar_t* name2 = L"Resources\\Textures\\Galaxy.jpg";
	DirectX::CreateWICTextureFromFile(deviceResources->GetD3DDevice(), name1, nullptr, m_texture1.GetAddressOf());
	DirectX::CreateWICTextureFromFile(deviceResources->GetD3DDevice(), name2, nullptr, m_texture2.GetAddressOf());

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());
	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(m_deviceResources->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	m_fxFactory = std::make_unique<EffectFactory>(device);
	m_fxFactory->SetDirectory(L"Resources");
	m_model = Model::CreateFromCMO(device, L"Resources\\cup.cmo", *m_fxFactory);

	for (int i = 0; i < count; i++) {
		MyEffect* effect = new MyEffect();
		m_effectList.push_back(effect);
	}
}
void EffectManager::Lost() {
	for (std::list<MyEffect*>::iterator itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		delete (*itr);
	}
}


void EffectManager::Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 dir)
{
	for (std::list<MyEffect*>::iterator itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		Vector3 vel = dir;
		vel.Normalize();

		vel *= sin(rand()) + 1.0f;
		Vector3 raff = Vector3(vel.y, -vel.x, 0);
		raff *= sin(rand())*0.3f;
		vel = vel + raff;

		vel *= 0.1f;
		(*itr)->Initialize(life, pos, vel);
	}
}
void EffectManager::Update(DX::StepTimer timer)
{
	m_time = float(timer.GetTotalSeconds());
	for(std::list<MyEffect*>::iterator itr = m_effectList.begin(); itr != m_effectList.end();itr++)
	{
		(*itr)->Update(timer);
	}
}

void EffectManager::Render()
{



	m_vertex.clear();
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		Vector3 pos = (*itr)->GetPosition();
		Vector3 vel = (*itr)->GetVelocity();

		{
			VertexPositionColorTexture vertex;
			vertex = VertexPositionColorTexture(pos, Vector4(vel.x,vel.y,vel.z,1), Vector2(0.0f,3.0f));
			m_vertex.push_back(vertex);
		}
	}
	RenderModel();
	Draw(Matrix::Identity,m_view, m_proj);
}

void EffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_view = view;
	m_proj = proj;
}

void EffectManager::Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	m_model->Draw(m_deviceResources->GetD3DDeviceContext(), *m_states, Matrix::Identity, m_view, m_proj);

	ConstBuffer cbuff;

	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = world.Transpose();
	cbuff.Diffuse = Vector4(cosf(m_time), m_time, sinf(m_time), 1);
	//cbuff.Diffuse = Vector4(m_position.x, m_position.y, m_position.z, 1);

	//定数バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファに参照する
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// カリングは左周り
	context->RSSetState(m_states->CullNone());

	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, m_texture1.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_texture2.GetAddressOf());


	context->IASetInputLayout(m_inputLayout.Get());

	// 半透明部分を描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertex[0], m_vertex.size());
	m_batch->End();

}

void EffectManager::RenderModel() 
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	D3D11_TEXTURE2D_DESC texDesc;
	m_deviceResources->GetRenderTarget()->GetDesc(&texDesc);
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_deviceResources->GetD3DDevice()->CreateTexture2D(&texDesc, NULL, m_tex.GetAddressOf());

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// レンダーターゲットビューの生成
	m_deviceResources->GetD3DDevice()->CreateRenderTargetView(m_tex.Get(), &rtvDesc, &m_rtv);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	auto hr = m_deviceResources->GetD3DDevice()->CreateShaderResourceView(
		m_tex.Get(), &srvDesc, m_srv.GetAddressOf());

	ID3D11RenderTargetView* defaultRTV = m_deviceResources->GetRenderTargetView();

	ID3D11DepthStencilView* pDSV = m_deviceResources->GetDepthStencilView();

	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), pDSV);
	context->ClearRenderTargetView(m_rtv.Get(), clearColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	m_model->Draw(m_deviceResources->GetD3DDeviceContext(), *m_states, Matrix::Identity, m_view, m_proj);

	clearColor[0] =0.3f;
	clearColor[1] = 0.3f;

	context->OMSetRenderTargets(1, &defaultRTV, pDSV);
	context->ClearRenderTargetView(defaultRTV, clearColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

}