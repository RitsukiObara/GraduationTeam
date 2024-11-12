//*****************************************************
//
// モード選択処理[selectmode.cpp]
// Author:石原颯馬
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "selectmode.h"
#include "object.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "manager.h"
#include "fade.h"
#include "texture.h"
#include "camera.h"
#include "CameraState.h"
#include "renderer.h"
#include "sound.h"
#include "polygon3D.h"
#include "objectX.h"
#include "skybox.h"
#include "meshcylinder.h"
#include "meshfield.h"
#include "particle.h"
#include "orbit.h"
#include "debugproc.h"
#include "UI.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	namespace selectUI
	{
		const char* PATH[CSelectMode::MODE_MAX] =
		{
			"data\\TEXTURE\\UI\\single_mode.png",
			"data\\TEXTURE\\UI\\party_mode.png",
			//"data\\TEXTURE\\UI\\tutorial00.jpg"
		};
		const float WIDTH[CSelectMode::MODE_MAX] =
		{
			0.14f,
			0.14f,
			//0.5f
		};
		const float HEIGHT[CSelectMode::MODE_MAX] =
		{
			0.25f,
			0.25f,
			//0.5f
		};
		const D3DXVECTOR3 POS[CSelectMode::MODE_MAX] =
		{
			D3DXVECTOR3(0.395f, 0.38f, 0.0f),
			D3DXVECTOR3(0.605f, 0.62f, 0.0f),
			//D3DXVECTOR3(0.5f, 0.5f, 0.0f)
		};
		const float NOSELECT_ALPHA = 0.35f;	// 選択されていないときの不透明度
	}

	namespace manual
	{
		const char* PATH = "data\\TEXTURE\\UI\\tutorial00.jpg";	// パス
		const float WIDTH		= 0.5f;	// 幅
		const float HEIGHT		= 0.5f;	// 高さ
		const D3DXVECTOR3 POS	= D3DXVECTOR3(0.5f, 0.5f, 0.0f);	// 位置
	}

	namespace control
	{
		const char* PATH = "data\\TEXTURE\\UI\\tutorial_control00.png";	// パス
		const float WIDTH		= 0.18f;	// 幅
		const float HEIGHT		= 0.08f;	// 高さ
		const D3DXVECTOR3 POS	= D3DXVECTOR3(0.82f, 0.91f, 0.0f);	// 位置
	}
}

//=====================================================
// コンストラクタ
//=====================================================
CSelectMode::CSelectMode()
{
	m_pManual = nullptr;
	m_fCurTime = 0.0f;
	m_selectMode = MODE_SINGLE;
	for (int cnt = 0; cnt < MODE_MAX; cnt++) { m_apModeUI[cnt] = nullptr; }
}

//=====================================================
// デストラクタ
//=====================================================
CSelectMode::~CSelectMode()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectMode::Init(void)
{
	// シーンの初期化
	if (FAILED(CScene::Init()))
	{ // 初期化に失敗した場合

		return E_FAIL;
	}

	// 説明の生成
	//m_pManual = CUI::Create();
	//if (m_pManual != nullptr)
	//{
	//	// 説明の設定
	//	m_pManual->SetIdxTexture(CTexture::GetInstance()->Regist(manual::PATH));	// テクスチャ割当
	//	m_pManual->SetPosition(manual::POS);				// 位置
	//	m_pManual->SetSize(manual::WIDTH, manual::HEIGHT);	// 大きさ
	//	m_pManual->SetVtx();	// 頂点反映
	//}

	for (int cnt = 0; cnt < MODE_MAX; cnt++)
	{
		m_apModeUI[cnt] = CUI::Create();
		if (m_apModeUI[cnt] != nullptr)
		{
			// 設定
			m_apModeUI[cnt]->SetIdxTexture(CTexture::GetInstance()->Regist(selectUI::PATH[cnt]));	// テクスチャ割当
			m_apModeUI[cnt]->SetPosition(selectUI::POS[cnt]);				// 位置
			m_apModeUI[cnt]->SetSize(selectUI::WIDTH[cnt], selectUI::HEIGHT[cnt]);	// 大きさ
			m_apModeUI[cnt]->SetVtx();	// 頂点反映
		}
	}
	ChangeSelectMode(0);	// 何も選択しない（透明度設定のみする）

	// BGMの再生
	CSound* pSound = CSound::GetInstance();
	assert(pSound != nullptr);
	pSound->Play(pSound->LABEL_BGM_TITLE);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSelectMode::Uninit(void)
{
	for (int cnt = 0; cnt < MODE_MAX; cnt++)
	{
		if (m_apModeUI[cnt] != nullptr)
		{
			Object::DeleteObject((CObject**)&m_apModeUI[cnt]);
		}
	}
	Object::DeleteObject((CObject**)&m_pManual);

	// シーンの終了
	CScene::Uninit();

	// オブジェクト全破棄
	CObject::ReleaseAll();
}

//=====================================================
// 更新処理
//=====================================================
void CSelectMode::Update(void)
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	assert(pKeyboard != nullptr);

	CSound* pSound = CSound::GetInstance();	// サウンド情報
	assert(pSound != nullptr);

	// モードの移動
	if (pKeyboard->GetTrigger(DIK_A))
	{
		ChangeSelectMode(-1);	// モード前にずらす

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_PAUSE_ENTER00);
	}
	else if (pKeyboard->GetTrigger(DIK_D))
	{
		ChangeSelectMode(1);	// モード先にずらす

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_PAUSE_ENTER00);
	}

	// モード選択完了
	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		// フェード中の場合抜ける
		CFade* pFade = CFade::GetInstance();
		if (pFade == nullptr)
			assert(false);
		if (pFade->GetState() != CFade::FADE_NONE)
			return;

		switch (m_selectMode)
		{
		case CSelectMode::MODE_SINGLE:
			pFade->SetFade(CScene::MODE_GAME);
			break;
		case CSelectMode::MODE_PARTY:
			pFade->SetFade(CScene::MODE_SELECTPLAYER);
			break;
		default:
			assert(false);
			break;
		}

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_PAUSE_ENTER00);
	}

	// シーンの更新
	CScene::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CSelectMode::Draw(void)
{
	// シーンの描画
	CScene::Draw();
}

//=====================================================
// 終了操作の点滅更新処理
//=====================================================
void CSelectMode::ChangeSelectMode(int move)
{
	// 値移動
	m_selectMode = (MODE)((m_selectMode + move + MODE_MAX) % MODE_MAX);

	// 選択しているモード不透明/それ以外半透明
	for (int cnt = 0; cnt < MODE_MAX; cnt++)
	{
		if (m_apModeUI[cnt] != nullptr)
		{
			// 設定
			if (cnt == m_selectMode)
			{// 選択しているモード
				m_apModeUI[cnt]->SetAlpha(1.0f);
			}
			else
			{// それ以外半透明
				m_apModeUI[cnt]->SetAlpha(selectUI::NOSELECT_ALPHA);
			}
			m_apModeUI[cnt]->SetVtx();	// 頂点反映
		}
	}
}
