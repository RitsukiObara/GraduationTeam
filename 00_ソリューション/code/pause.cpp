//*****************************************************
//
// ポーズの処理[pause.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "pause.h"
#include "manager.h"
#include "polygon2D.h"
#include "inputManager.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "sound.h"
#include "UI.h"
#include "gameManager.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MENU_WIDTH	(0.11f)	// 項目の幅
#define MENU_HEIGHT	(0.18f)	// 項目の高さ
#define MOVE_FACT	(0.08f)	// 移動速度
#define LINE_ARRIVAL	(0.05f)	// 到着したとされるしきい値
#define LINE_UNINIT	(0.2f)	// 終了するまでのしきい値
#define SPEED_FADE	(0.03f)	// 背景のフェード速度
#define ALPHA_BG	(0.5f)	// 背景の不透明度

namespace
{
	namespace buttonUI
	{
		const string PATH = "data\\TEXTURE\\UI\\SelectAndBack.png";
		const float WIDTH = 0.1957f;
		const float HEIGHT = 0.049f;
		const D3DXVECTOR3 POS = D3DXVECTOR3(0.78f, 0.92f, 0.0f);
	}
}

//====================================================
// コンストラクタ
//====================================================
CPause::CPause()
{
	m_pInputManager = nullptr;
	m_menu = MENU_RESUME;
	m_state = STATE_NONE;
	ZeroMemory(&m_apMenu[MENU_RESUME], sizeof(m_apMenu));
	ZeroMemory(&m_aPosDest[MENU_RESUME], sizeof(D3DXVECTOR3) * MENU_MAX);
	m_pButton = nullptr;
	m_posDestButton = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bSound = false;
	nCountMove = 0;
}

//====================================================
// デストラクタ
//====================================================
CPause::~CPause()
{

}

//====================================================
// 生成処理
//====================================================
CPause *CPause::Create(CInputManager* pInput)
{
	CPause *pPause = nullptr;

	pPause = new CPause;

	if (pPause != nullptr)
	{
		pPause->Init();
		pPause->m_pInputManager = pInput;
	}

	return pPause;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CPause::Init(void)
{
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		// ゲームを停止する
		pGame->EnableStop(true);

		CSound* pSound = CSound::GetInstance();

		if (pSound != nullptr)
		{
			pSound->Play(pSound->LABEL_SE_PAUSE_MENU);
		}
	}

	// 背景の生成
	m_pBg = CUI::Create();

	if (m_pBg != nullptr)
	{
		m_pBg->SetPosition(D3DXVECTOR3(0.5f, 0.5f, 0.0f));

		m_pBg->SetSize(0.5f, 0.5f);

		m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		m_pBg->SetVtx();
	}

	// ボタンUI
	m_pButton = CUI::Create();
	if (m_pButton != nullptr)
	{
		// 設定
		m_pButton->SetIdxTexture(CTexture::GetInstance()->Regist(&buttonUI::PATH[0]));	// テクスチャ割当
		m_pButton->SetPosition(D3DXVECTOR3(2.5f, buttonUI::POS.y,0.0f));				// 位置
		m_pButton->SetSize(buttonUI::WIDTH, buttonUI::HEIGHT);	// 大きさ
		m_pButton->SetVtx();	// 頂点反映

		// 目標位置設定
		m_posDestButton = m_pButton->GetPosition();
		m_posDestButton.x = buttonUI::POS.x;
	}

	char *pPath[MENU_MAX] = 
	{// メニュー項目のパス
		"data\\TEXTURE\\UI\\Resume.png",
		"data\\TEXTURE\\UI\\Restart.png",
		"data\\TEXTURE\\UI\\Quit.png",
		"data\\TEXTURE\\UI\\pause.png",
	};

	int nIdxTexture;

	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{// メニュー項目のポリゴンを生成
		if (m_apMenu[nCntMenu] == nullptr)
		{
			m_apMenu[nCntMenu] = CUI::Create();

			if (m_apMenu[nCntMenu] != nullptr)
			{
				if (nCntMenu == MENU_RESUME)
				{//	ゲームに戻る
					// ポリゴンの設定
					m_apMenu[nCntMenu]->SetPosition(D3DXVECTOR3(-2.5f, 0.45f + MENU_HEIGHT * nCntMenu, 0.0f));
					m_apMenu[nCntMenu]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apMenu[nCntMenu]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
					m_aPosDest[nCntMenu] = m_apMenu[nCntMenu]->GetPosition();
					m_aPosDest[nCntMenu].x = 0.35f;
				}

				else if (nCntMenu == MENU_RESTART)
				{//	ゲームのリスタート
					// ポリゴンの設定
					m_apMenu[nCntMenu]->SetPosition(D3DXVECTOR3(2.5f, 0.45f + MENU_HEIGHT * nCntMenu, 0.0f));
					m_apMenu[nCntMenu]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apMenu[nCntMenu]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
					m_aPosDest[nCntMenu] = m_apMenu[nCntMenu]->GetPosition();
					m_aPosDest[nCntMenu].x = 0.65f;
				}

				else if (nCntMenu == MENU_QUIT)
				{//	タイトルに戻る
					// ポリゴンの設定
					m_apMenu[nCntMenu]->SetPosition(D3DXVECTOR3(-2.5f, 0.45f + MENU_HEIGHT * nCntMenu, 0.0f));
					m_apMenu[nCntMenu]->SetSize(MENU_WIDTH, MENU_HEIGHT);
					m_apMenu[nCntMenu]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
					m_aPosDest[nCntMenu] = m_apMenu[nCntMenu]->GetPosition();
					m_aPosDest[nCntMenu].x = 0.35f;
				}

				else if (nCntMenu == MENU_PAUSE)
				{//	ポーズ
					// ポリゴンの設定
					m_apMenu[nCntMenu]->SetPosition(D3DXVECTOR3(2.5f, 0.12f, 0.0f));
					m_apMenu[nCntMenu]->SetSize(MENU_WIDTH + 0.1f, MENU_HEIGHT - 0.05f);
					m_apMenu[nCntMenu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_aPosDest[nCntMenu] = m_apMenu[nCntMenu]->GetPosition();
					m_aPosDest[nCntMenu].x = 0.5f;
				}

				// テクスチャの設定
				nIdxTexture = CTexture::GetInstance()->Regist(pPath[nCntMenu]);

				m_apMenu[nCntMenu]->SetIdxTexture(nIdxTexture);
				m_apMenu[nCntMenu]->SetVtx();
			}
		}
	}

	m_state = STATE_IN;

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CPause::Uninit(void)
{
	if (m_pBg != nullptr)
	{// 背景ポリゴンの破棄
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{// メニュー項目の破棄
		if (m_apMenu[nCnt] != nullptr)
		{
			m_apMenu[nCnt]->Uninit();

			m_apMenu[nCnt] = nullptr;
		}
	}

	if (m_pButton != nullptr)
	{
		m_pButton->Uninit();
		m_pButton = nullptr;
	}

	// ゲームを再開する
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		pGame->EnableStop(false);
		pGame->ReleasePause();
	}

	// 自身の破棄
	Release();
}

//====================================================
// 更新処理
//====================================================
void CPause::Update(void)
{
	// 状態管理
	ManageState();
}

//====================================================
// 状態管理
//====================================================
void CPause::ManageState(void)
{
	if (m_state != STATE_OUT)
	{
		Input();
	}
	
	// 背景の管理
	ManageBg();

	// 終了フラグ用
	int nEnd = 0;

	// ポリゴンを目標位置に向かわせる
	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{
		if (m_apMenu[nCntMenu] != nullptr)
		{
			D3DXVECTOR3 pos = m_apMenu[nCntMenu]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			D3DXVECTOR3 vecDiff = m_aPosDest[nCntMenu] - pos;
			float fDiffOld = vecDiff.x;

			vecDiff *= MOVE_FACT;
			vecDiff += pos;

			m_apMenu[nCntMenu]->SetPosition(vecDiff);
			m_apMenu[nCntMenu]->SetVtx();

			float fDiff = m_aPosDest[nCntMenu].x - vecDiff.x;

			if (fDiff * fDiff < LINE_UNINIT * LINE_UNINIT &&
				m_state == STATE_OUT)
			{// 終了のライン
				nEnd++;
			}
		}
	}
	if (m_pButton != nullptr)
	{
		D3DXVECTOR3 pos = m_pButton->GetPosition();
		D3DXVECTOR3 posOld = pos;
		D3DXVECTOR3 vecDiff = m_posDestButton - pos;
		float fDiffOld = vecDiff.x;

		vecDiff *= MOVE_FACT;
		vecDiff += pos;

		m_pButton->SetPosition(vecDiff);
		m_pButton->SetVtx();
	}

	if (nEnd == MENU_MAX &&
		m_state == STATE_OUT)
	{
		Uninit();
		return;
	}
}

//====================================================
// 背景の管理
//====================================================
void CPause::ManageBg(void)
{
	if (m_pBg == nullptr)
		return;

	D3DXCOLOR col = m_pBg->GetCol();

	switch (m_state)
	{
	case CPause::STATE_IN:

		col.a += SPEED_FADE;

		if (col.a >= ALPHA_BG)
		{
			col.a = ALPHA_BG;

			m_state = STATE_NONE;
		}

		break;
	case CPause::STATE_OUT:

		col.a -= SPEED_FADE;

		if (col.a <= 0.0f)
		{
			col.a = 0.0f;
		}

		break;
	default:
		break;
	}

	m_pBg->SetCol(col);
}

//====================================================
// 操作処理
//====================================================
void CPause::Input(void)
{
	CSound* pSound = CSound::GetInstance();
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
	{
		return;
	}
	else
	{
		if (pFade->GetState() != CFade::FADE_NONE)
		{
			return;
		}
	}

	if (m_pInputManager == nullptr)
	{
		return;
	}
	
	if ((m_pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE) || 
		m_pInputManager->GetTrigger(CInputManager::BUTTON_BACK)) && 
		m_state == STATE::STATE_NONE)
	{
		m_state = STATE_OUT;

		//	画面外目標を設定
		OffPosition();
		return;
	}

	if (m_apMenu[m_menu] != nullptr)
	{// 以前に選択してた項目を非選択色にする
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(0.5f,0.5f,0.5f,1.0f));
	}

	// 項目切り替え
	if (m_pInputManager->GetTrigger(CInputManager::BUTTON_AXIS_DOWN))
	{
		nCountMove = 0;
		m_menu = (MENU)((m_menu + 1) % 3);

		if (pSound != nullptr && m_bSound == false)
		{
			pSound->Play(CSound::LABEL_SE_SELECT);

			m_bSound = true;
		}
	}

	if (m_pInputManager->GetTrigger(CInputManager::BUTTON_AXIS_UP))
	{
		nCountMove = 0;
		m_menu = (MENU)((m_menu + 3 - 1) % 3);
		
		if (pSound != nullptr && m_bSound == false)
		{
			pSound->Play(CSound::LABEL_SE_SELECT);

			m_bSound = true;
		}
	}

	if (m_apMenu[m_menu] != nullptr)
	{// 選択している項目の色変更
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (m_pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
	{// 選択項目にフェードする
		if (pSound != nullptr)
		{
			pSound->Play(CSound::LABEL_SE_DECISION);
		}

		Fade(m_menu);
	}

	//	選択した時にUIが動く処理
	SelectMove();
}

//====================================================
// フェードする処理
//====================================================
void CPause::Fade(MENU menu)
{
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
	{
		return;
	}

	switch (menu)
	{
	case CPause::MENU_RESUME:

		//	画面外目標を設定
		OffPosition();

		m_state = STATE_OUT;

		break;
	case CPause::MENU_RESTART:

		CGame::SetState(CGame::STATE_END);
		pFade->SetFade(CScene::MODE_GAME);

		break;
	case CPause::MENU_QUIT:
	{
		if(gameManager::IsMulti())
			pFade->SetFade(CScene::MODE_SELECTPLAYER);
		else
			pFade->SetFade(CScene::MODE_SELECTSTAGE);
	}
		break;
	default:
		break;
	}
}

//====================================================
// 画面外目標設定処理
//====================================================
void CPause::OffPosition(void)
{
	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{//	画面外の目標を設定	
		if (m_apMenu[nCntMenu] != nullptr)
		{
			if (nCntMenu == MENU_RESUME)
			{
				m_aPosDest[nCntMenu].x = -0.25f;
			}

			else if (nCntMenu == MENU_RESTART)
			{
				m_aPosDest[nCntMenu].x = 1.25f;
			}

			else if (nCntMenu == MENU_QUIT)
			{
				m_aPosDest[nCntMenu].x = -0.25f;
			}

			else if (nCntMenu == MENU_PAUSE)
			{
				m_aPosDest[nCntMenu].y = -0.25f;
			}
		}
	}
	m_posDestButton.x = 1.25f;
}

//====================================================
// 選択した時にUIが動く処理
//====================================================
void CPause::SelectMove(void)
{
	if (m_apMenu[m_menu] == nullptr)
		return;

	D3DXVECTOR3 pos = m_apMenu[m_menu]->GetPosition();
	nCountMove++;

	//	ゲームに戻る
	if (m_menu == MENU_RESUME)
	{
		//	ほかの項目を初期値に戻す
		if (m_aPosDest[MENU_RESTART].x < 0.65f)
		{
			m_aPosDest[MENU_RESTART].x += 0.001f;
		}

		else if (m_aPosDest[MENU_RESTART].x > 0.65f)
		{
			m_aPosDest[MENU_RESTART].x -= 0.001f;
		}

		if (m_aPosDest[MENU_QUIT].x < 0.35f)
		{
			m_aPosDest[MENU_QUIT].x += 0.001f;
		}

		else if (m_aPosDest[MENU_QUIT].x > 0.35f)
		{
			m_aPosDest[MENU_QUIT].x -= 0.001f;
		}
	}

	// リスタート
	else if (m_menu == MENU_RESTART)
	{
		//	ほかの項目を初期値に戻す
		if (m_aPosDest[MENU_RESUME].x < 0.35f)
		{
			m_aPosDest[MENU_RESUME].x += 0.001f;
		}

		else if (m_aPosDest[MENU_RESUME].x > 0.35f)
		{
			m_aPosDest[MENU_RESUME].x -= 0.001f;
		}

		if (m_aPosDest[MENU_QUIT].x < 0.35f)
		{
			m_aPosDest[MENU_QUIT].x += 0.001f;
		}

		else if (m_aPosDest[MENU_QUIT].x > 0.35f)
		{
			m_aPosDest[MENU_QUIT].x -= 0.001f;
		}
	}

	// タイトルに戻る
	else if (m_menu == MENU_QUIT)
	{
		//	ほかの項目を初期値に戻す
		if (m_aPosDest[MENU_RESUME].x < 0.35f)
		{
			m_aPosDest[MENU_RESUME].x += 0.001f;
		}

		else if (m_aPosDest[MENU_RESUME].x > 0.35f)
		{
			m_aPosDest[MENU_RESUME].x -= 0.001f;
		}

		if (m_aPosDest[MENU_RESTART].x < 0.65f)
		{
			m_aPosDest[MENU_RESTART].x += 0.001f;
		}

		else if (m_aPosDest[MENU_RESTART].x > 0.65f)
		{
			m_aPosDest[MENU_RESTART].x -= 0.001f;
		}
	}

	// ポリゴンをカウントごとに動かす
	if (nCountMove > 0 && nCountMove < 100)
	{
		m_aPosDest[m_menu].x += 0.001f;
	}

	else if (nCountMove > 100 && nCountMove < 300)
	{
		m_aPosDest[m_menu].x -= 0.001f;
	}

	else if (nCountMove > 300 && nCountMove < 500)
	{
		m_aPosDest[m_menu].x += 0.001f;
	}

	else if (nCountMove > 500)
	{
		nCountMove = 100;
	}

	m_apMenu[m_menu]->SetPosition(pos);
	m_apMenu[m_menu]->SetVtx();
}

//====================================================
// 描画処理
//====================================================
void CPause::Draw(void)
{

}
