//*****************************************************
//
// シングル負けリザルトの処理[resultSingleLose.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "resultSingleLose.h"
#include "UI.h"
#include "camera.h"
#include "cameraState.h"
#include "number.h"
#include "texture.h"
#include "manager.h"
#include "inputManager.h"
#include "fade.h"
#include "sound.h"

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
const string PATH_TEX[CResultSingleLose::E_Select::SELECT_MAX] =
{// メニューテクスチャのパス
	"data\\TEXTURE\\UI\\Restart.png",
	"data\\TEXTURE\\UI\\Quit.png",
};
const float TIME_APPER = 2.0f;		// 出現にかかる時間
const float WIDTH = 0.16f;			// 幅
const float HEIGHT = 0.22f;			// 高さ
const float HEIGHT_INIT = -HEIGHT;
const D3DXVECTOR3 POS_INIT[CResultSingleLose::E_Select::SELECT_MAX] =
{// 初期位置
	{ 0.3f,HEIGHT_INIT,0.0f },
	{ 0.7f,HEIGHT_INIT,0.0f },
};
const float HEIGHT_DEST = 0.7f;							// 目標の高さ
const float DIFF_HEIGHT = HEIGHT_DEST - HEIGHT_INIT;	// 高さの差分
}
}

//*****************************************************
// 静的メンバ変数
//*****************************************************
CResultSingleLose::FuncUpdateState CResultSingleLose::s_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,								// 何もしない更新
	&CResultSingleLose::UpdateApperMenu,	// メニュ出現状態更新
	&CResultSingleLose::UpdateSelect,		// 選択状態更新
	nullptr,								// 終了状態の更新
};

//====================================================
// コンストラクタ
//====================================================
CResultSingleLose::CResultSingleLose() : m_fTimer(0.0f), m_apMenu(), m_state(E_State::STATE_NONE)
{

}

//====================================================
// デストラクタ
//====================================================
CResultSingleLose::~CResultSingleLose()
{

}

//====================================================
// 初期化処理
//====================================================
HRESULT CResultSingleLose::Init(void)
{
	// 親クラスの初期化
	CResultSingle::Init();

	// 2Dオブジェクトの生成
	Create2D();

	// 親の状態設定
	CResultSingle::SetState(CResultSingle::E_State::STATE_FADE);

	m_select = SELECT_CONTINUE;
	m_apMenu[SELECT_QUIT]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	return S_OK;
}

//====================================================
// 2Dオブジェクトの生成
//====================================================
void CResultSingleLose::Create2D(void)
{
	// メニューの生成
	CreateMenu();
}

//====================================================
// メニューの生成
//====================================================
void CResultSingleLose::CreateMenu(void)
{
	for (int i = 0; i < E_Select::SELECT_MAX; i++)
	{
		// キャプションの生成
		m_apMenu[i] = CUI::Create();
		if (m_apMenu[i] == nullptr)
			continue;

		m_apMenu[i]->SetSize(menu::WIDTH, menu::HEIGHT);
		m_apMenu[i]->SetPosition(menu::POS_INIT[i]);
		int nIdxTexture = Texture::GetIdx(&menu::PATH_TEX[i][0]);
		m_apMenu[i]->SetIdxTexture(nIdxTexture);
		m_apMenu[i]->SetAlpha(0.0f);
		m_apMenu[i]->SetVtx();
	}
}

//====================================================
// 終了処理
//====================================================
void CResultSingleLose::Uninit(void)
{
	CResultSingle::Uninit();
}

//====================================================
// 更新処理
//====================================================
void CResultSingleLose::Update(void)
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
void CResultSingleLose::UpdateApperMenu(void)
{
	m_fTimer += CManager::GetDeltaTime();

	float fTime = m_fTimer / menu::TIME_APPER;
	float fRate = easing::EaseOutExpo(fTime);
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	for (int i = 0; i < E_Select::SELECT_MAX; i++)
	{
		//-----------------------------------------
		// キャプションの移動
		//-----------------------------------------
		if (m_apMenu[i] == nullptr)
			continue;

		D3DXVECTOR3 posCaption = m_apMenu[i]->GetPosition();
		posCaption.y = menu::HEIGHT_INIT + menu::DIFF_HEIGHT * fRate;

		m_apMenu[i]->SetPosition(posCaption);
		m_apMenu[i]->SetVtx();
		m_apMenu[i]->SetAlpha(fRate);
	}

	if (m_fTimer > menu::TIME_APPER)
	{// 一定時間経過で次の状態に移る
		m_state = E_State::STATE_SELECT;
		m_fTimer = 0.0f;
	}
}

//=====================================================
// 選択状態更新
//=====================================================
void CResultSingleLose::UpdateSelect(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	CSound* pSound = CSound::GetInstance();
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
	{
		return;
	}
	else
	{
		if (pFade->GetState() != CFade::FADE_NONE)
			return;
	}

	if (pInputManager == nullptr)
	{
		return;
	}

	if (m_apMenu[m_select] != nullptr)
	{// 以前に選択してた項目を非選択色にする
		m_apMenu[m_select]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	}

	// 項目切り替え
	if (pInputManager->GetTrigger(CInputManager::BUTTON_AXIS_RIGHT))
	{
		m_select = (E_Select)((m_select + 1) % E_Select::SELECT_MAX);
		pSound->Play(CSound::LABEL_SE_SELECT);
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_AXIS_LEFT))
	{
		m_select = (E_Select)((m_select + E_Select::SELECT_MAX - 1) % E_Select::SELECT_MAX);
		pSound->Play(CSound::LABEL_SE_SELECT);
	}

	if (m_apMenu[m_select] != nullptr)
	{// 選択している項目の色変更
		m_apMenu[m_select]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
	{// 選択項目にフェードする
		if (pSound != nullptr)
		{
			pSound->Play(CSound::LABEL_SE_DECISION);
		}

		Fade(m_select);
	}
}

//====================================================
// 描画処理
//====================================================
void CResultSingleLose::Draw(void)
{
	// 親クラスの描画
	CResultSingle::Draw();
}

//====================================================
// フェードする処理
//====================================================
void CResultSingleLose::Fade(E_Select select)
{
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	switch (select)
	{
	case CResultSingleLose::E_Select::SELECT_CONTINUE:	// コンティニュー
		pFade->SetFade(CScene::MODE::MODE_GAME);
		break;
	case CResultSingleLose::E_Select::SELECT_QUIT:		// タイトルに戻る
		pFade->SetFade(CScene::MODE::MODE_TITLE);
		break;
	default:
		break;
	}
}