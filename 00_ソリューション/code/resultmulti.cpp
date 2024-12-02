//*****************************************************
//
// マルチモードのリザルト処理[resultmulti.cpp]
// Author:石原颯馬
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "resultmulti.h"
#include "inputManager.h"
#include "fade.h"
#include "texture.h"
#include "camera.h"
#include "CameraState.h"
#include "sound.h"
#include "objectX.h"
#include "meshfield.h"
#include "UI.h"
#include "gameManager.h"
#include "npcpenguin.h"
#include "npcpenguinstate_resultmulti.h"
#include "resultseal.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	namespace Penguin
	{
		const D3DXVECTOR3 WINNER_POS = D3DXVECTOR3(800.0f, 0.0f, -1800.0f);	// 勝者の位置
		const vector<D3DXVECTOR3> LOSER_POS =	// 敗者の位置
		{
			D3DXVECTOR3(800.0f, 0.0f, -500.0f),
			D3DXVECTOR3(800.0f, 0.0f, -700.0f),
			D3DXVECTOR3(800.0f, 0.0f, -900.0f)
		};
		const vector<D3DXVECTOR3> LOSER_ROT =	// 敗者の向き
		{
			D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f),
			D3DXVECTOR3(0.0f, 0.5471f * D3DX_PI, 0.0f),
			D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f)
		};
		const int MAX_PLAYER = 4;	// 最大人数
	}

	namespace Map
	{
		const string FIELD_TEX_PATH = "data\\TEXTURE\\MATERIAL\\field.jpg";	// 地面テクスチャパス
		const string SNOWDOME_PATH = "data\\MODEL\\object\\Snowdome.x";		// かまくらモデルパス
	}

	namespace UI
	{
		const string NUMBER_PATH = "data\\TEXTURE\\UI\\player_Count.png";	// 何Pとかのテクスチャ
		const string TEXT_PATH = "data\\TEXTURE\\UI\\win.png";				// 「の勝利」のテクスチャ
		const float NUMBER_WIDTH = 0.1f;	// 幅（何P）
		const float NUMBER_HEIGHT = 0.15f;	// 高さ（何P）
		const float TEXT_WIDTH = 0.15f;		// 幅（「の勝利」）
		const float TEXT_HEIGHT = 0.15f;	// 高さ（「の勝利」）
		const D3DXVECTOR3 NUMBER_POS = D3DXVECTOR3(0.3125f, 0.85f, 0.0f);	// 何Pとかの位置
		const D3DXVECTOR3 TEXT_POS = D3DXVECTOR3(0.55f, 0.85f, 0.0f);		// 「の勝利」の位置
		const int MAX_PLAYER = 4;
	}
}

//=====================================================
// コンストラクタ
//=====================================================
CResultMulti::CResultMulti()
{
	m_pWinnerNum = nullptr;
	m_pWinnerText = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CResultMulti::~CResultMulti()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CResultMulti::Init(void)
{
	// シーンの初期化
	if (FAILED(CScene::Init()))
		return E_FAIL;

	// カメラ
	Camera::ChangeState(new CCameraStateResultMulti);

	// 勝者番号UI生成
	SetUI(&m_pWinnerNum, UI::NUMBER_PATH, UI::NUMBER_POS, UI::NUMBER_WIDTH, UI::NUMBER_HEIGHT);
	SetUI(&m_pWinnerText, UI::TEXT_PATH, UI::TEXT_POS, UI::TEXT_WIDTH, UI::TEXT_HEIGHT);

	// メッシュフィールド
	CMeshField* pMeshField = CMeshField::Create();
	if (pMeshField != nullptr)
	{
		pMeshField->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pMeshField->SetIdxTexture(CTexture::GetInstance()->Regist(&Map::FIELD_TEX_PATH[0]));
		pMeshField->SetDivTex(128);
	}

	// かまくら
	CObjectX *pIgloo = CObjectX::Create();
	if (pIgloo != nullptr)
	{
		pIgloo->BindModel(CModel::Load(&Map::SNOWDOME_PATH[0]));
		pIgloo->SetPosition(D3DXVECTOR3(800.0f, -10.0f, 600.0f));
	}

	// BGMの再生
	CSound* pSound = CSound::GetInstance();
	assert(pSound != nullptr);
	pSound->Play(pSound->LABEL_BGM_TITLE);

	// 対戦結果取得
	int playerNum = Penguin::MAX_PLAYER;
	int winner = 0;
	vector<bool> enablePlayer;
	gameManager::LoadWinner(&playerNum, &winner);
	for (int cnt = 0; cnt < Penguin::MAX_PLAYER; cnt++)
	{
		if (cnt < playerNum)
		{
			enablePlayer.push_back(true);
		}
		else
		{
			enablePlayer.push_back(false);
		}
	}

	// 勝者番号設定
	if (m_pWinnerNum != nullptr)
	{
		m_pWinnerNum->SetTex(D3DXVECTOR2((float)(winner) / UI::MAX_PLAYER, 0.0f), D3DXVECTOR2((float)(winner + 1) / UI::MAX_PLAYER, 1.0f));
		m_pWinnerNum->SetVtx();
	}

	// 各ペンギン召喚
	CNPCPenguin* pPenguin = nullptr;
	// 勝者ペンギン
	pPenguin = CNPCPenguin::Create(new CNPCPenguinState_BANZAI,(CNPCPenguin::SKIN)winner);
	pPenguin->SetPosition(Penguin::WINNER_POS);
	enablePlayer[winner] = false;	// 勝者を表示済みとしてfalseにしておく
	// 敗者ペンギン
	int loser = 0;
	for (int cnt = 0; cnt < Penguin::MAX_PLAYER; cnt++)
	{
		if (enablePlayer[cnt] == true)
		{// まだ表示していない
			pPenguin = CNPCPenguin::Create(new CNPCPenguinState_Flee(D3DXVECTOR3(800.0f, -10.0f, 600.0f)), (CNPCPenguin::SKIN)cnt);
			pPenguin->SetPosition(Penguin::LOSER_POS[loser]);
			pPenguin->SetRotation(Penguin::LOSER_ROT[loser]);
			loser++;
		}
	}

	// アザラシ召喚
	CResultSeal* pSeal = CResultSeal::Create(D3DXVECTOR3(800.0f, -10.0f, 600.0f));
	pSeal->SetPosition(D3DXVECTOR3(1200.0f, 0.0f, -900.0f));
	pSeal->SetRotation(D3DXVECTOR3(0.0f, 0.4f * D3DX_PI, 0.0f));

	// 入力マネージャー生成
	CInputManager::Create();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CResultMulti::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pWinnerNum);
	Object::DeleteObject((CObject**)&m_pWinnerText);

	// オブジェクト全破棄
	CObject::ReleaseAll();

	// シーンの終了
	CScene::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CResultMulti::Update(void)
{
	CInputManager *pInputMgr = CInputManager::GetInstance();
	assert(pInputMgr != nullptr);

	CSound* pSound = CSound::GetInstance();	// サウンド情報
	assert(pSound != nullptr);

	// 完了
	if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
	{
		// フェード中の場合抜ける
		CFade* pFade = CFade::GetInstance();
		if (pFade == nullptr)
			assert(false);
		if (pFade->GetState() != CFade::FADE_NONE)
			return;

		// ステージセレクト遷移
		pFade->SetFade(CScene::MODE_SELECTSTAGE);

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_DECISION);
	}

	// シーンの更新
	CScene::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CResultMulti::Draw(void)
{
	// シーンの描画
	CScene::Draw();
}

//=====================================================
// UI配置処理
//=====================================================
void CResultMulti::SetUI(CUI** pUI, string path, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	*pUI = CUI::Create();
	if (*pUI != nullptr)
	{// 説明の設定
		(*pUI)->SetIdxTexture(CTexture::GetInstance()->Regist(&path[0]));	// テクスチャ割当
		(*pUI)->SetPosition(pos);		// 位置
		(*pUI)->SetSize(fWidth, fHeight);	// 大きさ
		(*pUI)->SetVtx();	// 頂点反映
	}
	else
	{
		assert(false);
	}
}

