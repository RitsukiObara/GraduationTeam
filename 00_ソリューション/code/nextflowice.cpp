////*****************************************************
////
//// 氷が流れるUIの処理[nextflowice.cpp]
//// Author:若木一真
////
////*****************************************************
//
////*****************************************************
//// インクルード
////*****************************************************
//#include "nextflowice.h"
//#include "manager.h"
//#include "polygon2D.h"
//#include "inputkeyboard.h"
//#include "inputjoypad.h"
//#include "inputManager.h"
//#include "texture.h"
//#include "fade.h"
//#include "game.h"
//#include "sound.h"
//#include "UI.h"
//#include "inputManager.h"
//
////*****************************************************
//// マクロ定義
////*****************************************************
//#define RESULT_WIDTH	(0.18f)	// 項目の幅
//#define RESULT_HEIGHT	(0.05f)	// 項目の高さ
//#define MOVE_FACT	(0.15f)	// 移動速度
//#define LINE_ARRIVAL	(0.05f)	// 到着したとされるしきい値
//#define LINE_UNINIT	(3.0f)	// 終了するまでのしきい値
//
////*****************************************************
//// 定数定義
////*****************************************************
//namespace
//{
//	const char* CREAR_LOGO_PATH = "data\\TEXTURE\\UI\\stage_clear.png";	// クリアロゴのパス
//	const char* FAIL_LOGO_PATH = "data\\TEXTURE\\UI\\gameover.png";	// 失敗ロゴのパス
//}
//
////*****************************************************
//// 静的メンバ変数宣言
////*****************************************************
//CNextFlowIce* CNextFlowIce::m_pStageResultUI = nullptr;	// 自身のポインタ
//
////====================================================
//// コンストラクタ
////====================================================
//CNextFlowIce::CNextFlowIce() :m_apResult(), m_aPosDest()
//{
//	m_Result = RESULT_CLEAR;
//	m_state = STATE_NONE;
//	m_bSound = false;
//	nCountMove = 0;
//}
//
////====================================================
//// デストラクタ
////====================================================
//CNextFlowIce::~CNextFlowIce()
//{
//
//}
//
////====================================================
//// 生成処理
////====================================================
//CNextFlowIce* CNextFlowIce::Create(void)
//{
//	if (m_pStageResultUI == nullptr)
//	{
//		m_pStageResultUI = new CNextFlowIce;
//
//		if (m_pStageResultUI != nullptr)
//		{
//			m_pStageResultUI->Init();
//		}
//	}
//
//	return m_pStageResultUI;
//}
//
////====================================================
//// 初期化処理
////====================================================
//HRESULT CNextFlowIce::Init(void)
//{
//	//クリアの時
//	if (m_Result == RESULT_CLEAR)
//	{
//		// クリアロゴの生成
//		m_apResult[RESULT_CLEAR] = CPolygon2D::Create(7);
//
//		if (m_apResult[RESULT_CLEAR] != nullptr)
//		{
//			m_apResult[RESULT_CLEAR]->SetSize(300.0f, 100.0f);
//			m_apResult[RESULT_CLEAR]->SetPosition(D3DXVECTOR3(640.0f, 200.0f, 0.0f));
//			int nIdx = CTexture::GetInstance()->Regist(CREAR_LOGO_PATH);
//			m_apResult[RESULT_CLEAR]->SetIdxTexture(nIdx);
//			m_apResult[RESULT_CLEAR]->SetVtx();
//			m_apResult[RESULT_CLEAR]->SetAlpha(0.2f);
//		}
//	}
//
//	//失敗の時
//	else if (m_Result == RESULT_FAIL)
//	{
//		// 失敗ロゴの生成
//		m_apResult[RESULT_FAIL] = CPolygon2D::Create(7);
//
//		if (m_apResult[RESULT_FAIL] != nullptr)
//		{
//			m_apResult[RESULT_FAIL]->SetSize(330.0f, 100.0f);
//			m_apResult[RESULT_FAIL]->SetPosition(D3DXVECTOR3(640.0f, 200.0f, 0.0f));
//			int nIdx = CTexture::GetInstance()->Regist(FAIL_LOGO_PATH);
//			m_apResult[RESULT_FAIL]->SetIdxTexture(nIdx);
//			m_apResult[RESULT_FAIL]->SetVtx();
//			m_apResult[RESULT_FAIL]->SetAlpha(0.2f);
//		}
//	}
//
//	m_state = STATE_IN;
//
//	m_aPosDest[0].x = RESULT_WIDTH;
//
//	EnableNotStop(true);
//
//	return S_OK;
//}
//
////====================================================
//// 終了処理
////====================================================
//void CNextFlowIce::Uninit(void)
//{
//	for (int nCnt = 0; nCnt < RESULT_MAX; nCnt++)
//	{// メニュー項目の破棄
//		if (m_apResult[nCnt] != nullptr)
//		{
//			m_apResult[nCnt]->Uninit();
//
//			m_apResult[nCnt] = nullptr;
//		}
//	}
//
//	m_pStageResultUI = nullptr;
//
//	Release();
//}
//
////====================================================
//// 更新処理
////====================================================
//void CNextFlowIce::Update(void)
//{
//	// 状態管理
//	ResultState();
//}
//
////====================================================
//// 状態管理
////====================================================
//void CNextFlowIce::ResultState(void)
//{
//	// リザルトがクリアの時の処理
//	ResultClear();
//
//	// リザルトが失敗の時の処理
//	ResultFail();
//}
//
////====================================================
//// リザルトがクリアの時
////====================================================
//void CNextFlowIce::ResultClear(void)
//{
//	CInputManager* pInputManager = CInputManager::GetInstance();
//
//	// クリアの時
//	if (m_Result == RESULT_CLEAR)
//	{
//		if (m_apResult[RESULT_CLEAR] != nullptr)
//		{
//			D3DXVECTOR2 size = m_apResult[RESULT_CLEAR]->GetSize();
//			float fAlpha = m_apResult[RESULT_CLEAR]->GetAlpha();
//			nCountMove++;
//
//			if (nCountMove > 15 && nCountMove < 25)
//			{
//				fAlpha += 0.08f;
//			}
//
//			// ポリゴンをカウントごとに動かす
//			if (nCountMove > 0 && nCountMove < 20 ||
//				nCountMove > 50 && nCountMove < 80 ||
//				nCountMove > 110 && nCountMove < 140)
//			{
//				size.x += 1.0f;
//				size.y += 0.6f;
//			}
//
//			else if (nCountMove > 20 && nCountMove < 50 ||
//				nCountMove > 80 && nCountMove < 110 ||
//				nCountMove > 140 && nCountMove < 170)
//			{
//				size.x -= 1.0f;
//				size.y -= 0.6f;
//			}
//
//			else if (nCountMove > 170)
//			{
//				// カウントと透明度をリセット========================================
//				if (pInputManager->GetTrigger(CInputManager::BUTTON_RESULT))
//				{
//					nCountMove = 0;
//					fAlpha = 0.2f;
//				}
//			}
//
//			m_apResult[RESULT_CLEAR]->SetSize(size.x, size.y);
//			m_apResult[RESULT_CLEAR]->SetAlpha(fAlpha);
//			m_apResult[RESULT_CLEAR]->SetVtx();
//		}
//	}
//}
//
////====================================================
//// リザルトが失敗の時
////====================================================
//void CNextFlowIce::ResultFail(void)
//{
//	CInputManager* pInputManager = CInputManager::GetInstance();
//
//	// 失敗の時
//	if (m_Result == RESULT_FAIL)
//	{
//		if (m_apResult[RESULT_FAIL] != nullptr)
//		{
//			D3DXVECTOR3 pos = m_apResult[RESULT_FAIL]->GetPosition();
//			D3DXVECTOR3 rot = m_apResult[RESULT_FAIL]->GetRotation();
//			float fAlpha = m_apResult[RESULT_FAIL]->GetAlpha();
//			nCountMove++;
//
//			if (nCountMove > 15 && nCountMove < 25)
//			{
//				fAlpha += 0.08f;
//			}
//
//			// ポリゴンをカウントごとに動かす
//			if (nCountMove > 0 && nCountMove < 40 ||
//				nCountMove > 75 && nCountMove < 105 ||
//				nCountMove > 130 && nCountMove < 150 ||
//				nCountMove > 165 && nCountMove < 175)
//			{
//				pos.y += 1.0f;
//			}
//
//			else if (nCountMove > 40 && nCountMove < 75 ||
//				nCountMove > 105 && nCountMove < 130 ||
//				nCountMove > 150 && nCountMove < 165 ||
//				nCountMove > 175 && nCountMove < 180)
//			{
//				pos.y -= 1.0f;
//			}
//
//			else if (nCountMove > 185 && nCountMove < 205)
//			{
//				rot.z -= 0.008f;
//			}
//
//			else if (nCountMove > 205)
//			{
//				// カウントと透明度をリセット========================================
//				if (pInputManager->GetTrigger(CInputManager::BUTTON_RESULT))
//				{
//					nCountMove = 0;
//					fAlpha = 0.2f;
//					rot.z = 0.0f;
//				}
//			}
//
//			m_apResult[RESULT_FAIL]->SetPosition(pos);
//			m_apResult[RESULT_FAIL]->SetRotation(rot);
//			m_apResult[RESULT_FAIL]->SetAlpha(fAlpha);
//			m_apResult[RESULT_FAIL]->SetVtx();
//		}
//	}
//}
//
////====================================================
//// フェードする処理
////====================================================
//void CNextFlowIce::Fade(RESULT RESULT)
//{
//	CFade* pFade = CFade::GetInstance();
//
//	if (pFade == nullptr)
//	{
//		return;
//	}
//
//	switch (RESULT)
//	{
//	case CNextFlowIce::RESULT_CLEAR:
//
//		CGame::SetState(CGame::STATE_END);
//		pFade->SetFade(CScene::MODE_GAME);
//
//		break;
//	case CNextFlowIce::RESULT_FAIL:
//
//		m_state = STATE_OUT;
//		m_aPosDest[0].x = -RESULT_WIDTH;
//
//		break;
//	default:
//		break;
//	}
//}
//
////====================================================
//// 描画処理
////====================================================
//void CNextFlowIce::Draw(void)
//{
//
//}