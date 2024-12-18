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
#include "snow.h"

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

//--------------------------------
// 引き分けの定数
//--------------------------------
namespace draw
{
const string PATH = "data\\TEXTURE\\UI\\draw.png";			// 引き分け
const float WIDTH = 0.15f;									// 幅
const float HEIGHT = 0.10f;									// 高さ
const D3DXVECTOR3 POS = D3DXVECTOR3(0.5f, 0.85f, 0.0f);		// 位置
}
//----------------------------
// ボタンUI定数
//----------------------------
namespace buttonUI
{
	const string PATH = "data\\TEXTURE\\UI\\A_Move.png";
	const float WIDTH = 0.09f;
	const float HEIGHT = 0.049f;
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.88f, 0.92f, 0.0f);
}

namespace Snow
{
	const int CNT_SNOW = 5;						// 雪が降るタイミング
	const float MAX_HEIGHT = 1800.0f;				// 雪が降ってくる高さ
	const int MAX_SNOW_RADIUS = 100;				// 最大雪の半径
	const int MIN_SNOW_RADIUS = 20;					// 最低雪の半径
	const int SNOW_LIFE = 200;						// 雪のライフ
	const float SNOW_MOVE_SPED = -10.0f;			// 雪の移動速度
	const int MAX_SNOW_POS = 2200;					// 雪の最大生成場所
	const int MIN_SNOW_POS = -2200;					// 雪の最低生成場所
	const int BLIZZARD_CNT = 800;					// 吹雪が来るまでのカウント
}

}

//=====================================================
// コンストラクタ
//=====================================================
CResultMulti::CResultMulti()
{
	m_pWinnerNum = nullptr;
	m_pWinnerText = nullptr;
	m_pButtonUI = nullptr;
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

	// ボタンUI
	m_pButtonUI = CUI::Create();
	if (m_pButtonUI != nullptr)
	{
		// 設定
		m_pButtonUI->SetIdxTexture(CTexture::GetInstance()->Regist(&buttonUI::PATH[0]));	// テクスチャ割当
		m_pButtonUI->SetPosition(buttonUI::POS);					// 位置
		m_pButtonUI->SetSize(buttonUI::WIDTH, buttonUI::HEIGHT);	// 大きさ
		m_pButtonUI->SetVtx();	// 頂点反映
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

	// 引き分けフラグの設定
	m_bDraw = winner == -1;

	if (!m_bDraw)	// 通常の初期化
		InitNormal(winner, playerNum);
	else			// 引き分けの初期化
		InitDraw();

	// 入力マネージャー生成
	CInputManager::Create();

	return S_OK;
}

//=====================================================
// 通常の初期化
//=====================================================
void CResultMulti::InitNormal(int nIdxWinner, int nPlayerNum)
{
	//-----------------------------------
	// 勝者番号UI生成
	//-----------------------------------
	SetUI(&m_pWinnerNum, UI::NUMBER_PATH, UI::NUMBER_POS, UI::NUMBER_WIDTH, UI::NUMBER_HEIGHT);
	SetUI(&m_pWinnerText, UI::TEXT_PATH, UI::TEXT_POS, UI::TEXT_WIDTH, UI::TEXT_HEIGHT);

	//-----------------------------------
	// 情報設定
	//-----------------------------------
	vector<bool> enablePlayer;

	for (int cnt = 0; cnt < Penguin::MAX_PLAYER; cnt++)
	{
		if (cnt < nPlayerNum)
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
		m_pWinnerNum->SetTex(D3DXVECTOR2((float)(nIdxWinner) / UI::MAX_PLAYER, 0.0f), D3DXVECTOR2((float)(nIdxWinner + 1) / UI::MAX_PLAYER, 1.0f));
		m_pWinnerNum->SetVtx();
	}

	//-----------------------------------
	// キャラクター生成
	//-----------------------------------
	// 各ペンギン召喚
	CNPCPenguin* pPenguin = nullptr;
	// 勝者ペンギン
	pPenguin = CNPCPenguin::Create(new CNPCPenguinState_BANZAI, (CNPCPenguin::SKIN)nIdxWinner);
	pPenguin->SetPosition(Penguin::WINNER_POS);
	enablePlayer[nIdxWinner] = false;	// 勝者を表示済みとしてfalseにしておく
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
}

//=====================================================
// 引き分けの初期化
//=====================================================
void CResultMulti::InitDraw(void)
{
	SetUI(&m_pWinnerText, draw::PATH, draw::POS, draw::WIDTH, draw::HEIGHT);
}

//=====================================================
// 終了処理
//=====================================================
void CResultMulti::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pWinnerNum);
	Object::DeleteObject((CObject**)&m_pWinnerText);
	Object::DeleteObject((CObject**)&m_pButtonUI);
	
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

		// ステージプレイヤーに遷移
		pFade->SetFade(CScene::MODE_SELECTPLAYER);

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_DECISION);
	}

	// 雪降らす処理
	if (m_bDraw)
	{
		CSnow::SetSnow(Snow::MAX_SNOW_POS, Snow::MIN_SNOW_POS, Snow::MAX_HEIGHT, Snow::MAX_SNOW_RADIUS, Snow::MIN_SNOW_RADIUS,
			Snow::SNOW_LIFE, D3DXVECTOR3(0.0f, Snow::SNOW_MOVE_SPED, 0.0f), Snow::CNT_SNOW);
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

