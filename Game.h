﻿//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

#include "DebugCamera.h"
#include "GridFloor.h"

#include "Scene\SceneBase.h"
#include "Scene\ScenePlay.h"


#include "Object\Obj2D.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();


private:
	//シーンベースポインターを持つ
	SceneBase* m_scene;

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

	// キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyboard;

	// マウス
	std::unique_ptr<DirectX::Mouse> m_mouse;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;

	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_font;

	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;

	// デバッグカメラ
	//std::unique_ptr<DebugCamera> m_debugCamera;

	// グリッドの床
	//std::unique_ptr<GridFloor> m_gridFloor;
};