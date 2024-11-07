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
	m_pManual = CUI::Create();
	if (m_pManual != nullptr)
	{
		// 説明の設定
		m_pManual->SetIdxTexture(CTexture::GetInstance()->Regist(manual::PATH));	// テクスチャ割当
		m_pManual->SetPosition(manual::POS);				// 位置
		m_pManual->SetSize(manual::WIDTH, manual::HEIGHT);	// 大きさ
		m_pManual->SetVtx();	// 頂点反映
	}

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
	CInputManager* pInput = CInputManager::GetInstance();	// 入力マネージャー情報
	assert(pInput != nullptr);

	CSound* pSound = CSound::GetInstance();	// サウンド情報
	assert(pSound != nullptr);

	if (pInput->GetTrigger(CInputManager::BUTTON_ENTER))
	{
		// フェード中の場合抜ける
		CFade* pFade = CFade::GetInstance();
		if (pFade == nullptr) { assert(false); return; }
		if (pFade->GetState() != CFade::FADE_NONE) { assert(false); return; }

		// タイトルに遷移する
		pFade->SetFade(CScene::MODE_TITLE);

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
void CSelectMode::UpdateBlinkUI(void)
{

}
