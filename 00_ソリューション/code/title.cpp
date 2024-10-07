//*****************************************************
//
// タイトル処理[title.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "title.h"
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
#include "smoke.h"
#include "meshcylinder.h"
#include "fan3D.h"
#include "meshfield.h"
#include "particle.h"
#include "orbit.h"
#include "debugproc.h"
#include "UI.h"
#include "light.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const int NUM_LIGHT = 3;	// ライトの数

	namespace title
	{
		const char* PATH = "data\\TEXTURE\\UI\\title_logo00.png";	// パス
		const float WIDTH		= 1.09f * 0.5f * 0.88f;	// 幅
		const float HEIGHT		= 0.18f * 0.88f;		// 高さ
		const float	MOVE_TIME	= 1.5f;	// 移動時間
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 目標色
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		// 初期色
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;						// 差分色
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(0.5f, 0.32f, 0.0f);			// 目標位置
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 0.1f, 0.0f);	// 初期位置
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;						// 差分位置
	}

	namespace select
	{
		const char* PATH[] =
		{
			"data\\TEXTURE\\UI\\title_select00.png",	// ゲーム選択肢のパス
			"data\\TEXTURE\\UI\\title_select01.png",	// チュートリアル選択肢のパス
		};

		const float	MOVE_TIME	= 0.84f;	// 移動時間
		const float	PLUS_TIME	= 0.0f;	// 経過の延長時間
		const float	WAIT_TIME	= 0.15f;	// 待機時間
		const float WIDTH		= 0.8f  * 0.32f;	// 幅
		const float HEIGHT		= 0.25f * 0.32f;	// 高さ
		const float	DEST_ALPHA	= 1.0f;		// 目標透明度
		const float	INIT_ALPHA	= 0.0f;		// 初期透明度
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// 差分透明度

		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);		// 非選択時の色
		const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);		// 選択時の色
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, DEST_ALPHA);	// 目標色
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, INIT_ALPHA);	// 初期色
		const D3DXVECTOR3 SPACE		= D3DXVECTOR3(0.36f, 0.0f, 0.0f);			// 空白
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(0.5f - SPACE.x * 0.5f, 0.85f, 0.0f);	// 目標位置
		const D3DXVECTOR3 INIT_POS	= DEST_POS;				// 初期位置
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;	// 差分位置
	}
}

//*****************************************************
//	静的メンバ変数宣言
//*****************************************************
CTitle::AFuncUpdateState CTitle::m_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,					// 何もしない更新
	&CTitle::UpdateLogoMove,	// ロゴの移動状態の更新
	&CTitle::UpdateSelectMove,	// 選択項目の移動状態の更新
	&CTitle::UpdateSelect,		// 選択状態の更新
	nullptr,					// 終了状態の更新
};

//=====================================================
// コンストラクタ
//=====================================================
CTitle::CTitle()
{
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));
	m_state = STATE_LOGO_MOVE;
	m_pLogo = nullptr;
	m_fCurTime = 0.0f;
	m_nCurSelect = 0;
	m_nOldSelect = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CTitle::~CTitle()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CTitle::Init(void)
{
	// シーンの初期化
	if (FAILED(CScene::Init()))
	{ // 初期化に失敗した場合

		return E_FAIL;
	}

	// ロゴの生成
	m_pLogo = CUI::Create();
	if (m_pLogo != nullptr)
	{
		// ロゴの設定
		m_pLogo->SetIdxTexture(CTexture::GetInstance()->Regist(title::PATH));	// テクスチャ割当
		m_pLogo->SetPosition(title::INIT_POS);			// 位置
		m_pLogo->SetSize(title::WIDTH, title::HEIGHT);	// 大きさ
		m_pLogo->SetCol(title::INIT_COL);				// 色
		m_pLogo->SetVtx();								// 頂点反映
	}

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// 選択肢の生成
		m_apSelect[i] = CUI::Create();
		if (m_apSelect[i] != nullptr)
		{
			// 生成位置を計算
			D3DXVECTOR3 posSelect = select::INIT_POS + (select::SPACE * (float)i);	// 生成位置

			// 選択肢の設定
			m_apSelect[i]->SetIdxTexture(CTexture::GetInstance()->Regist(select::PATH[i]));	// テクスチャ割当
			m_apSelect[i]->SetPosition(posSelect);					// 位置
			m_apSelect[i]->SetSize(select::WIDTH, select::HEIGHT);	// 大きさ
			m_apSelect[i]->SetCol(select::INIT_COL);				// 色
			m_apSelect[i]->SetVtx();								// 頂点反映
		}
	}

	// カメラインスタンス取得
	CCamera *pCamera = CManager::GetCamera();
	assert(pCamera != nullptr);

	// カメラ情報の設定
	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	pInfoCamera->fLength = { 500.0f };			// 距離
	pInfoCamera->rot  = { 1.35f, 0.0f, 0.0f };	// 向き
	pInfoCamera->posR = { 0.0f, 600.0f, 0.0f };	// 注視点位置
	pCamera->SetPosV();	// 視点反映

	// ライトの生成
	CreateLight();

	// BGMの再生
	CSound* pSound = CSound::GetInstance();
	assert(pSound != nullptr);
	pSound->Play(pSound->LABEL_BGM_TITLE);

	return S_OK;
}

//=====================================================
// ライトの生成
//=====================================================
void CTitle::CreateLight(void)
{
	D3DXVECTOR3 aDir[NUM_LIGHT] =
	{
		{ -1.4f, 0.24f, -2.21f, },
		{ 1.42f, -0.8f, 0.08f },
		{ -0.29f, -0.8f, 0.55f }
	};

	for (int i = 0; i < NUM_LIGHT; i++)
	{
		CLight *pLight = CLight::Create();

		if (pLight == nullptr)
			continue;

		D3DLIGHT9 infoLight = pLight->GetLightInfo();

		infoLight.Type = D3DLIGHT_DIRECTIONAL;
		infoLight.Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

		D3DXVECTOR3 vecDir = aDir[i];
		D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトル正規化
		infoLight.Direction = vecDir;

		pLight->SetLightInfo(infoLight);
	}
}

//=====================================================
// 終了処理
//=====================================================
void CTitle::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pLogo);
	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		Object::DeleteObject((CObject**)&m_apSelect[i]);
	}

	// シーンの終了
	CScene::Uninit();

	// オブジェクト全破棄
	CObject::ReleaseAll();
}

//=====================================================
// 更新処理
//=====================================================
void CTitle::Update(void)
{
	// シーンの更新
	CScene::Update();

	assert(m_state > -1 && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // 更新関数が指定されている場合

		// 各状態ごとの更新
		(this->*(m_aFuncUpdateState[m_state]))();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CTitle::Draw(void)
{
	// シーンの描画
	CScene::Draw();
}

//=====================================================
//	ロゴの移動状態の更新処理
//=====================================================
void CTitle::UpdateLogoMove(void)
{
	// タイマーを加算
	m_fCurTime += CManager::GetDeltaTime();

	// 経過時刻の割合を計算
	float fRate = easing::InOutQuad(m_fCurTime, 0.0f, title::MOVE_TIME);

	// タイトルの色を反映
	m_pLogo->SetCol(title::INIT_COL + (title::DIFF_COL * fRate));

	// タイトルの位置を反映
	m_pLogo->SetPosition(title::INIT_POS + (title::DIFF_POS * fRate));
	m_pLogo->SetVtx();	// 頂点反映

	if (m_fCurTime >= title::MOVE_TIME)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// タイトルの色を補正
		m_pLogo->SetCol(title::DEST_COL);

		// タイトルの位置を補正
		m_pLogo->SetPosition(title::DEST_POS);
		m_pLogo->SetVtx();	// 頂点反映

		// 選択項目の移動状態にする
		m_state = STATE_SELECT_MOVE;
	}
}

//=====================================================
//	選択項目の移動状態の更新処理
//=====================================================
void CTitle::UpdateSelectMove(void)
{
	// タイマーを加算
	m_fCurTime += CManager::GetDeltaTime();

	// 選択肢の移動
	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// アイコン背景それぞれの経過時間を計算
		float fRateTime = m_fCurTime - (select::PLUS_TIME * (float)i);
		universal::LimitNum(fRateTime, 0.0f, select::MOVE_TIME);	// 経過時間を補正

		// それぞれの経過時刻から割合を計算
		float fRate = easing::InOutQuad(fRateTime, 0.0f, select::MOVE_TIME);

		// 選択肢の位置を計算
		D3DXVECTOR3 posInit = select::INIT_POS + (select::SPACE * (float)i);

		// 選択肢の色を計算
		D3DXCOLOR colCur = select::INIT_COL;
		colCur.a = select::INIT_ALPHA + (select::DIFF_ALPHA * fRate);	// 現在の透明度を設定

		// 選択肢の位置を反映
		m_apSelect[i]->SetPosition(posInit + (select::DIFF_POS * fRate));
		m_apSelect[i]->SetVtx();	// 頂点反映

		// 選択肢の色を反映
		m_apSelect[i]->SetCol(colCur);
	}

	// 選択肢の移動補正
	if (m_fCurTime >= select::MOVE_TIME + select::PLUS_TIME * (SELECT_MAX - 1))
	{ // 全選択肢の待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		for (int i = 0; i < SELECT_MAX; i++)
		{ // 選択肢の総数分繰り返す

			// 選択肢の目標生成位置を計算
			D3DXVECTOR3 posDest = select::DEST_POS + (select::SPACE * (float)i);

			// 選択肢の位置を補正
			m_apSelect[i]->SetPosition(posDest);
			m_apSelect[i]->SetVtx();	// 頂点反映

			// 選択肢の色を補正
			m_apSelect[i]->SetCol(select::DEST_COL);
		}

		// 選択状態にする
		m_state = STATE_SELECT;

		// タイトルカメラにする
		Camera::ChangeState(new CCameraStateTitle);
	}
}

//=====================================================
//	選択状態の更新処理
//=====================================================
void CTitle::UpdateSelect(void)
{
	CInputManager* pInput = CInputManager::GetInstance();	// 入力マネージャー情報
	assert(pInput != nullptr);

	CSound* pSound = CSound::GetInstance();	// サウンド情報
	assert(pSound != nullptr);

	//--------------------------------------------------------
	//	選択肢操作
	//--------------------------------------------------------
	// 前回の選択肢を保存
	m_nOldSelect = m_nCurSelect;

	// 選択肢操作
	if (pInput->GetTrigger(CInputManager::BUTTON_TRIGGER_LEFT))
	{
		// 左に選択をずらす
		m_nCurSelect = (m_nCurSelect + 1) % SELECT_MAX;

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_PAUSE_ARROW);
	}
	if (pInput->GetTrigger(CInputManager::BUTTON_TRIGGER_RIGHT))
	{
		// 右に選択をずらす
		m_nCurSelect = (m_nCurSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_PAUSE_ARROW);
	}

	// 前回の選択要素の色を白色に設定
	m_apSelect[m_nOldSelect]->SetCol(select::DEFAULT_COL);

	// 現在の選択要素の色を黄色に設定
	m_apSelect[m_nCurSelect]->SetCol(select::CHOICE_COL);

	//--------------------------------------------------------
	//	選択肢決定
	//--------------------------------------------------------
	if (pInput->GetTrigger(CInputManager::BUTTON_ENTER))
	{
		// 終了状態にする
		m_state = STATE_END;

		// フェード中の場合抜ける
		CFade* pFade = CFade::GetInstance();
		if (pFade == nullptr) { assert(false); return; }
		if (pFade->GetState() != CFade::FADE_NONE) { assert(false); return; }
		switch (m_nCurSelect)
		{ // 選択肢ごとの処理
		case SELECT_GAME:

			// ゲームに遷移する
			pFade->SetFade(CScene::MODE_GAME);
			break;

		case SELECT_TUTORIAL:

			// チュートリアルに遷移する
			pFade->SetFade(CScene::MODE_TUTORIAL);
			break;

		default:
			assert(false);
			break;
		}

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_PAUSE_ENTER00);
	}
}
