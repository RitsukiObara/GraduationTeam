//*****************************************************
//
// レンダラーの処理[renderer.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "object.h"
#include "fade.h"
#include "fade_fallice.h"
#include "blur.h"
#include "inputkeyboard.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CRenderer *CRenderer::m_pRenderer = nullptr;	// 自身のポインタ

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	float FOG_START = 5000;
	float FOG_END = 20000;
}

//=====================================================
// コンストラクタ
//=====================================================
CRenderer::CRenderer()
{
	// 変数のクリア
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	ZeroMemory(&m_fogInfo, sizeof(SInfoFog));
	m_bDispimGui = false;
}

//=====================================================
// デストラクタ
//=====================================================
CRenderer::~CRenderer()
{

}

//=====================================================
// 生成処理
//=====================================================
CRenderer *CRenderer::Create(HWND hWnd, BOOL bWindow)
{
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer;

		if (m_pRenderer != nullptr)
		{
			m_pRenderer->Init(hWnd, bWindow);
		}
	}

	return m_pRenderer;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;						// ディスプレイモードの設定
	D3DPRESENT_PARAMETERS d3dpp;						//ディスプレイモードの設定

	// Direct3Dオブジェクト生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメーターの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// パラメーターのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面の幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// 高さ
	d3dpp.BackBufferFormat = d3ddm.Format;							// 形式
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// 映像信号に同期
	d3dpp.EnableAutoDepthStencil = TRUE;							// デプスとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// デプスバッファを8bitで使う
	d3dpp.Windowed = bWindow;										// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	if (!bWindow)
	{
		// ウィンドウスタイルを取得
		LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);

		// ウィンドウスタイルをWS_POPUPに変更
		SetWindowLong(hWnd, GWL_STYLE, lStyle & ~WS_OVERLAPPEDWINDOW | WS_POPUP);

		// ウィンドウの位置とサイズを画面全体に設定
		SetWindowPos
		(
			hWnd,
			HWND_TOP,
			0, 0, // x, y 座標
			GetSystemMetrics(SCREEN_WIDTH), // 画面の幅
			GetSystemMetrics(SCREEN_HEIGHT), // 画面の高さ
			SWP_FRAMECHANGED | SWP_SHOWWINDOW
		);
	}

	// Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	
	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプステートの設定
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_MIPFILTER,
		D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_MINFILTER,
		D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_MAGFILTER,
		D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_ADDRESSU,
		D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_ADDRESSV,
		D3DTADDRESS_WRAP);

	m_pD3DDevice->SetSamplerState(1,
		D3DSAMP_ADDRESSU,
		D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(1,
		D3DSAMP_ADDRESSV,
		D3DTADDRESS_CLAMP);

	//テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0,
		D3DTSS_ALPHAOP,
		D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0,
		D3DTSS_ALPHAARG1,
		D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0,
		D3DTSS_ALPHAARG2,
		D3DTA_CURRENT);

	// オブジェクトのリリース処理
	CObject::ReleaseAll();

	m_fogInfo.fStart = FOG_START;
	m_fogInfo.fEnd = FOG_END;

#ifdef _DEBUG
	m_bDispimGui = false;
#else
	m_bDispimGui = false;
#endif

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CRenderer::Uninit(void)
{
	// ブラーの破棄
	CBlur *pBlur = CBlur::GetInstance();

	if (pBlur != nullptr)
	{
		pBlur->Uninit();
		delete pBlur;
	}

	// オブジェクトのリリース
	CObject::ReleaseAll();

	if (m_pD3DDevice != nullptr)
	{// Direct3Dデバイス破棄
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	if (m_pD3D != nullptr)
	{// Direct3Dオブジェクトの破棄
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	delete this;
}

//=====================================================
// 更新処理
//=====================================================
void CRenderer::Update(void)
{
	// オブジェクトの更新
	CObject::UpdateAll();

#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_F4))
		{
			m_bDispimGui = m_bDispimGui ? false : true;
		}
	}
#endif
}

//=====================================================
// 描画処理
//=====================================================
void CRenderer::Draw(void)
{
	// フェード取得
	CFade *pFade = CFade::GetInstance();
	CFade_FallIce* pFadeFallIce = CFade_FallIce::GetInstance();

	// 画面クリア
	m_pD3DDevice->Clear(0, nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		COLOR_CLEAR, 1.0f, 0);

	// フォグを有効化
	//m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, m_fogInfo.bEnable);

	// フォグの色を設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, m_fogInfo.col);

	// フォグの状態を設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	// フォグの発生範囲を設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fogInfo.fStart));
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fogInfo.fEnd));

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画処理
		// FPS表示
		DrawFPS();
		
		// オブジェクトの描画
		CObject::DrawAll();

		if (pFade != nullptr)
		{// フェード描画
			pFade->Draw();
		}
		if (pFadeFallIce != nullptr)
		{// フェード描画
			pFadeFallIce->Draw();
		}

		CDebugProc::GetInstance()->Draw();

#ifdef _DEBUG
		// imgui描画
		ImGui::Render();

		if(m_bDispimGui)
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#endif

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// バック・フロントバッファを入れ替える
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//==================================================================================================
//デバッグ表示処理
//==================================================================================================
void CRenderer::DrawFPS(void)
{
	CDebugProc::GetInstance()->Print("\n基礎情報==================");
	CDebugProc::GetInstance()->Print("\nFPS:%d", GetFPS());
	CDebugProc::GetInstance()->Print("\nオブジェクト総数:[%d]", CObject::GetNumAll());
}

namespace Renderer
{
LPDIRECT3DDEVICE9 GetDevice(void)
{
	CRenderer *pRenderer = CRenderer::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	if (pRenderer != nullptr)
	{
		pDevice = pRenderer->GetDevice();
	}
	else
	{
		assert(("デバイスの取得に失敗", false));
	}

	return pDevice;
}
}