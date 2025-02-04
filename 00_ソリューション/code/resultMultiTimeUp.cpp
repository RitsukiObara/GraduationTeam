//*****************************************************
//
// マルチタイムアップリザルト[resultMultiTimeUp.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "resultMultiTimeUp.h"
#include "UI.h"
#include "camera.h"
#include "cameraState.h"
#include "number.h"
#include "texture.h"
#include "manager.h"
#include "inputManager.h"
#include "fade.h"
#include "sound.h"
#include "gameManager.h"
#include "player.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
//----------------------------
// メニュー定数
//----------------------------
namespace menu
{
const string PATH_TEX = "data\\TEXTURE\\UI\\tutorialskip.png";	// テクスチャパス
const float TIME_APPER = 2.0f;									// 出現にかかる時間
const float WIDTH = 0.16f;										// 幅
const float HEIGHT = 0.10f;										// 高さ
const float HEIGHT_INIT = -HEIGHT;
const D3DXVECTOR3 POS_INIT = { 0.5f,HEIGHT_INIT,0.0f };			// 初期位置
const float HEIGHT_DEST = 0.7f;									// 目標の高さ
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;			// 高さの差分
}
}

//*****************************************************
// 静的メンバ変数
//*****************************************************
CResultMultiTimeUp::FuncUpdateState CResultMultiTimeUp::s_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,								// 何もしない更新
	&CResultMultiTimeUp::UpdateApperMenu,	// メニュ出現状態更新
	&CResultMultiTimeUp::UpdateWait,		// 選択状態更新
	nullptr,								// 終了状態の更新
};

//====================================================
// コンストラクタ
//====================================================
CResultMultiTimeUp::CResultMultiTimeUp() : m_fTimer(0.0f), m_state(E_State::STATE_NONE)
{

}

//====================================================
// デストラクタ
//====================================================
CResultMultiTimeUp::~CResultMultiTimeUp()
{

}

//====================================================
// 初期化処理
//====================================================
HRESULT CResultMultiTimeUp::Init(void)
{
	// 親クラスの初期化
	CResultSingle::Init();

	// 2Dオブジェクトの生成
	Create2D();

	// 親の状態設定
	CResultSingle::SetState(CResultSingle::E_State::STATE_FADE);

	return S_OK;
}

//====================================================
// 2Dオブジェクトの生成
//====================================================
void CResultMultiTimeUp::Create2D(void)
{
	// メニューの生成
	CreateMenu();
}

//====================================================
// メニューの生成
//====================================================
void CResultMultiTimeUp::CreateMenu(void)
{
	// キャプションの生成
	m_pMenu = CUI::Create();
	if (m_pMenu == nullptr)
		return;

	m_pMenu->SetSize(menu::WIDTH, menu::HEIGHT);
	m_pMenu->SetPosition(menu::POS_INIT);
	int nIdxTexture = Texture::GetIdx(&menu::PATH_TEX[0]);
	m_pMenu->SetIdxTexture(nIdxTexture);
	m_pMenu->SetAlpha(0.0f);
	m_pMenu->SetVtx();
}

//====================================================
// 終了処理
//====================================================
void CResultMultiTimeUp::Uninit(void)
{
	CResultSingle::Uninit();
}

//====================================================
// 更新処理
//====================================================
void CResultMultiTimeUp::Update(void)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (s_aFuncUpdateState[m_state] != nullptr)
	{ // 更新関数が指定されている場合

		// 各状態ごとの更新
		(this->*(s_aFuncUpdateState[m_state]))();
	}

	// 親クラスの更新
	CResultSingle::Update();

	if (GetState() == CResultSingle::E_State::STATE_ENDAPPERCAPTION && m_state == E_State::STATE_NONE)
		m_state = E_State::STATE_APPER_MENU;	// フェードが終了したら演出始まる
}

//=====================================================
// メニュー出現状態更新
//=====================================================
void CResultMultiTimeUp::UpdateApperMenu(void)
{
	m_fTimer += CManager::GetDeltaTime();

	float fTime = m_fTimer / menu::TIME_APPER;
	float fRate = easing::EaseOutExpo(fTime);
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	//-----------------------------------------
	// キャプションの移動
	//-----------------------------------------
	if (m_pMenu == nullptr)
		return;

	D3DXVECTOR3 posCaption = m_pMenu->GetPosition();
	posCaption.y = menu::HEIGHT_INIT + menu::DIFF_HEIGHT * fRate;

	m_pMenu->SetPosition(posCaption);
	m_pMenu->SetVtx();
	m_pMenu->SetAlpha(fRate);

	if (m_fTimer > menu::TIME_APPER)
	{// 一定時間経過で次の状態に移る
		m_state = E_State::STATE_WAIT;
		m_fTimer = 0.0f;
	}
}

//=====================================================
// 選択状態更新
//=====================================================
void CResultMultiTimeUp::UpdateWait(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	CSound* pSound = CSound::GetInstance();
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	if (pInputManager == nullptr)
		return;

	if (pInputManager->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
	{
		// 引き分け設定にしてマルチリザルトへ移行
		gameManager::SaveWinner(CPlayer::GetNumPlayer(), -1);

		CFade *pFade = CFade::GetInstance();

		if (pFade != nullptr)
			pFade->SetFade(CScene::MODE_RESULTMULTI);
	}
}

//====================================================
// 描画処理
//====================================================
void CResultMultiTimeUp::Draw(void)
{
	// 親クラスの描画
	CResultSingle::Draw();
}