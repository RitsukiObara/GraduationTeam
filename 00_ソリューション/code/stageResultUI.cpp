//*****************************************************
//
// ステージリザルトUIの処理[stageResultUI.cpp]
// Author:若木一真
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "stageResultUI.h"
#include "manager.h"
#include "polygon2D.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputManager.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "sound.h"
#include "UI.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define RESULT_WIDTH	(0.18f)	// 項目の幅
#define RESULT_HEIGHT	(0.05f)	// 項目の高さ
#define MOVE_FACT	(0.15f)	// 移動速度
#define LINE_ARRIVAL	(0.05f)	// 到着したとされるしきい値
#define LINE_UNINIT	(3.0f)	// 終了するまでのしきい値

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const char* CREAR_LOGO_PATH = "data\\TEXTURE\\UI\\caption03.png";	// クリアロゴのパス
	const char* FAIL_LOGO_PATH = "data\\TEXTURE\\UI\\caption02.png";	// 失敗ロゴのパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CStageResultUI* CStageResultUI::m_pStageResultUI = nullptr;	// 自身のポインタ

//====================================================
// コンストラクタ
//====================================================
CStageResultUI::CStageResultUI():m_apResult(),m_aPosDest()
{
	m_RESULT = RESULT_CREAR;
	m_state = STATE_NONE;
	m_bSound = false;
}

//====================================================
// デストラクタ
//====================================================
CStageResultUI::~CStageResultUI()
{

}

//====================================================
// 生成処理
//====================================================
CStageResultUI* CStageResultUI::Create(void)
{
	if (m_pStageResultUI == nullptr)
	{
		m_pStageResultUI = new CStageResultUI;

		if (m_pStageResultUI != nullptr)
		{
			m_pStageResultUI->Init();
		}
	}

	return m_pStageResultUI;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CStageResultUI::Init(void)
{
	//クリアの時
	if (m_RESULT == RESULT_CREAR)
	{
		// クリアロゴの生成
		m_apResult[RESULT_CREAR] = CPolygon2D::Create(7);

		if (m_apResult[RESULT_CREAR] != nullptr)
		{
			m_apResult[RESULT_CREAR]->SetSize(300.0f, 100.0f);
			m_apResult[RESULT_CREAR]->SetPosition(D3DXVECTOR3(640.0f, 150.0f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(CREAR_LOGO_PATH);
			m_apResult[RESULT_CREAR]->SetIdxTexture(nIdx);
			m_apResult[RESULT_CREAR]->SetVtx();
		}
	}

	//失敗の時
	else if (m_RESULT == RESULT_FAIL)
	{
		// 失敗ロゴの生成
		m_apResult[RESULT_FAIL] = CPolygon2D::Create(7);

		if (m_apResult[RESULT_FAIL] != nullptr)
		{
			m_apResult[RESULT_FAIL]->SetSize(330.0f, 100.0f);
			m_apResult[RESULT_FAIL]->SetPosition(D3DXVECTOR3(640.0f, 150.0f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(FAIL_LOGO_PATH);
			m_apResult[RESULT_FAIL]->SetIdxTexture(nIdx);
			m_apResult[RESULT_FAIL]->SetVtx();
		}
	}

	m_state = STATE_IN;

	m_aPosDest[0].x = RESULT_WIDTH;

	EnableNotStop(true);

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CStageResultUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < RESULT_MAX; nCnt++)
	{// メニュー項目の破棄
		if (m_apResult[nCnt] != nullptr)
		{
			m_apResult[nCnt]->Uninit();

			m_apResult[nCnt] = nullptr;
		}
	}

	m_pStageResultUI = nullptr;

	Release();
}

//====================================================
// 更新処理
//====================================================
void CStageResultUI::Update(void)
{
	// 状態管理
	ManageState();
}

//====================================================
// 状態管理
//====================================================
void CStageResultUI::ManageState(void)
{
	// 終了フラグ用
	int nEnd = 0;

	// ポリゴンを目標位置に向かわせる
	//クリアの時
	if (m_RESULT == RESULT_CREAR)
	{
		if (m_apResult[RESULT_CREAR] != nullptr)
		{
			D3DXVECTOR3 pos = m_apResult[RESULT_CREAR]->GetPosition();
			D3DXVECTOR3 posOld = pos;
			////D3DXVECTOR3 vecDiff = m_apResult[RESULT_CREAR] - pos;
			//float fDiffOld = vecDiff.x;

			//vecDiff *= MOVE_FACT;

			//vecDiff += pos;

			//m_apResult[RESULT_CREAR]->SetPosition(vecDiff);

			//m_apResult[RESULT_CREAR]->SetVtx();

			//float fDiff = m_aPosDest[RESULT_CREAR].x - vecDiff.x;

			//if (fDiffOld * fDiffOld >= LINE_ARRIVAL * LINE_ARRIVAL &&
			//	fDiff * fDiff < LINE_ARRIVAL * LINE_ARRIVAL &&
			//	RESULT_CREAR < RESULT_MAX - 1)
			//{// 差分がしきい値より下になったら下のものを動かす
			//	if (m_state == STATE_IN)
			//	{
			//		m_aPosDest[RESULT_CREAR + 1].x = RESULT_WIDTH;
			//	}
			//	else if (m_state == STATE_OUT)
			//	{
			//		m_aPosDest[RESULT_CREAR + 1].x = -RESULT_WIDTH;
			//	}
			//}

			//if (fDiff * fDiff < LINE_UNINIT * LINE_UNINIT &&
			//	m_state == STATE_OUT)
			//{// 終了のライン
			//	nEnd++;
			//}
		}
	}

	if (nEnd == RESULT_MAX &&
		m_state == STATE_OUT)
	{
		Uninit();
	}
}

//====================================================
// フェードする処理
//====================================================
void CStageResultUI::Fade(RESULT RESULT)
{
	CFade* pFade = CFade::GetInstance();

	if (pFade == nullptr)
	{
		return;
	}

	switch (RESULT)
	{
	case CStageResultUI::RESULT_CREAR:

		CGame::SetState(CGame::STATE_END);
		pFade->SetFade(CScene::MODE_GAME);

		break;
	case CStageResultUI::RESULT_FAIL:

		m_state = STATE_OUT;
		m_aPosDest[0].x = -RESULT_WIDTH;

		break;
	default:
		break;
	}
}

//====================================================
// 描画処理
//====================================================
void CStageResultUI::Draw(void)
{

}