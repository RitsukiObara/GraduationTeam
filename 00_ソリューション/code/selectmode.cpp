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
#include "gameManager.h"
#include "npcpenguin.h"
#include "npcpenguinstate_selectmode.h"
#include "snow.h"
#include "inputkeyboard.h"
#include "MyEffekseer.h"
#include "tutorial.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	namespace selectUI
	{
		const string PATH[CSelectMode::MODE_MAX] =
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
			D3DXVECTOR3(0.195f, 0.48f, 0.0f),
			D3DXVECTOR3(0.405f, 0.72f, 0.0f),
			//D3DXVECTOR3(0.5f, 0.5f, 0.0f)
		};
		const D3DXCOLOR NOSELECT_COLOR = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.8f);
		const float NOSELECT_ALPHA = 0.5f;	// 選択されていないときの不透明度
		const float SELECTUI_COLORCHANGE_COEF = 0.4f;	// モードUIの変化の慣性
	}

	namespace buttonUI
	{
		const string PATH = "data\\TEXTURE\\UI\\SelectAndBack.png";
		const float WIDTH = 0.1957f;
		const float HEIGHT = 0.049f;
		const D3DXVECTOR3 POS = D3DXVECTOR3(0.78f, 0.92f, 0.0f);
	}

	namespace MeshField
	{
		const string TEX_PATH = "data\\TEXTURE\\MATERIAL\\field.jpg";
		const D3DXVECTOR3 POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		const int DIV_TEX = 128;
	}

	namespace Igloo
	{
		const string MODEL_PATH = "data\\MODEL\\object\\Snowdome_SelectMode.x";
		const D3DXVECTOR3 POS = D3DXVECTOR3(800.0f, -10.0f, 600.0f);
		const D3DXVECTOR3 ROT = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	namespace Penguin
	{
		const vector<D3DXVECTOR3> POS =
		{
			D3DXVECTOR3(400.0f, 10.0f, -1000.0f),
			D3DXVECTOR3(-500.0f, 10.0f, 800.0f),
			D3DXVECTOR3(2200.0f, 10.0f, 800.0f),
			D3DXVECTOR3(950.0f, 10.0f, 700.0f)
		};
	}

	const int CNT_SNOW = 10;						// 雪が降るタイミング
	const float MAX_HEIGHT = 1800.0f;				// 雪が降ってくる高さ
	const int MAX_SNOW_RADIUS = 100;				// 最大雪の半径
	const int MIN_SNOW_RADIUS = 20;					// 最低雪の半径
	const int SNOW_LIFE = 200;						// 雪のライフ
	const float SNOW_MOVE_SPED = -10.0f;			// 雪の移動速度
	const int MAX_SNOW_POS = 2200;					// 雪の最大生成場所
	const int MIN_SNOW_POS = -2200;					// 雪の最低生成場所
	const int BLIZZARD_CNT = 800;					// 吹雪が来るまでのカウント
}

//=====================================================
// コンストラクタ
//=====================================================
CSelectMode::CSelectMode()
{
	m_fCurTime = 0.0f;
	m_selectMode = MODE_SINGLE;
	for (int cnt = 0; cnt < MODE_MAX; cnt++) { m_apModeUI[cnt] = nullptr; }
	m_pButtonUI = nullptr;
	m_nSnowStormCnt = 0;
	m_bStorm = false;
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
		return E_FAIL;

	// カメラ
	Camera::ChangeState(new CCameraStateSelectMode);

	m_nSnowStormCnt = 0;
	m_bStorm = false;

	// モードUI
	for (int cnt = 0; cnt < MODE_MAX; cnt++)
	{
		m_apModeUI[cnt] = CUI::Create();
		if (m_apModeUI[cnt] != nullptr)
		{
			// 設定
			m_apModeUI[cnt]->SetIdxTexture(CTexture::GetInstance()->Regist(&selectUI::PATH[cnt][0]));	// テクスチャ割当
			m_apModeUI[cnt]->SetPosition(selectUI::POS[cnt]);				// 位置
			m_apModeUI[cnt]->SetSize(selectUI::WIDTH[cnt], selectUI::HEIGHT[cnt]);	// 大きさ
			m_apModeUI[cnt]->SetVtx();	// 頂点反映
		}
	}
	ChangeSelectMode(0);	// 何も選択しない（透明度設定のみする）

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

	// メッシュフィールド
	CMeshField* pMeshField = CMeshField::Create();
	pMeshField->SetPosition(MeshField::POS);
	pMeshField->SetIdxTexture(CTexture::GetInstance()->Regist(&MeshField::TEX_PATH[0]));
	pMeshField->SetDivTex(MeshField::DIV_TEX);

	// かまくら
	CObjectX *pIgloo = CObjectX::Create(Igloo::POS, Igloo::ROT,4);
	pIgloo->BindModel(CModel::Load(&Igloo::MODEL_PATH[0]));

	// BGMの再生
	CSound* pSound = CSound::GetInstance();
	assert(pSound != nullptr);
	pSound->Play(pSound->LABEL_BGM_TITLE);

	// 遊ぶペンギン
	CNPCPenguin* pPenguin = nullptr;
	for (auto itr = Penguin::POS.begin(); itr != Penguin::POS.end(); itr++)
	{
		pPenguin = CNPCPenguin::Create(new CNPCPenguinState_Stand);
		pPenguin->SetPosition((*itr));
	}

	// 入力マネージャー生成
	CInputManager::Create();

	// チュートリアルフラグをリセット
	tutorial::SaveFrag(true);

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

	Object::DeleteObject((CObject**)&m_pButtonUI);
	
	// オブジェクト全破棄
	CObject::ReleaseAll();

	// シーンの終了
	CScene::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSelectMode::Update(void)
{
	CInputManager *pInputMgr = CInputManager::GetInstance();
	assert(pInputMgr != nullptr);

	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	assert(pKeyboard != nullptr);

	CSound* pSound = CSound::GetInstance();	// サウンド情報
	assert(pSound != nullptr);

	CSnow::SetSnow(MAX_SNOW_POS, MIN_SNOW_POS, MAX_HEIGHT, MAX_SNOW_RADIUS, MIN_SNOW_RADIUS,
		SNOW_LIFE, D3DXVECTOR3(0.0f, SNOW_MOVE_SPED, 0.0f), CNT_SNOW);

	// モードの移動
	if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_AXIS_LEFT))
	{
		ChangeSelectMode(-1);	// モード前にずらす

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_SELECT);
	}
	else if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_AXIS_RIGHT))
	{
		ChangeSelectMode(1);	// モード先にずらす

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_SELECT);
	}

	// フェード中の場合抜ける
	CFade* pFade = CFade::GetInstance();
	if (pFade == nullptr)
		assert(false);

	// モード選択完了
	if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
	{
		vector<bool> abEnter = { true };

		switch (m_selectMode)
		{
		case CSelectMode::MODE_SINGLE:
			gameManager::SaveMode(CGame::E_GameMode::MODE_SINGLE, abEnter);
			pFade->SetFade(CScene::MODE_SELECTSTAGE);
			break;
		case CSelectMode::MODE_PARTY:
			pFade->SetFade(CScene::MODE_SELECTPLAYER);
			break;
		default:
			assert(false);
			break;
		}

		// サウンドの再生
		pSound->Play(CSound::LABEL_SE_DECISION);
	}

	//タイトルに戻る
	if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_BACK))
	{
		pFade->SetFade(CScene::MODE_TITLE);
	}

	m_nSnowStormCnt++;

	// 吹雪を生成
	if (m_nSnowStormCnt == BLIZZARD_CNT)
	{
		D3DXVECTOR3 posEffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 rotEffect = { 0.0f,0.0f,0.0f };

		// エフェクトの生成
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_BLIZZARD, posEffect, rotEffect);

		m_nSnowStormCnt = 0;
	}

	// モードUI見た目更新処理
	UpdateSelectModeUI();

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
	m_selectMode = (MODE)((m_selectMode + move + MODE_MAX) % MODE_MAX);	// 値移動
}

//=====================================================
// モードUI見た目更新処理
//=====================================================
void CSelectMode::UpdateSelectModeUI(void)
{
	// 選択しているモード不透明/それ以外半透明
	for (int cnt = 0; cnt < MODE_MAX; cnt++)
	{
		if (m_apModeUI[cnt] != nullptr)
		{
			// 設定
			if (cnt == m_selectMode)
			{// 選択しているモード
				D3DXCOLOR nowCol = m_apModeUI[cnt]->GetCol();
				D3DXCOLOR diffCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) - nowCol;
				diffCol *= selectUI::SELECTUI_COLORCHANGE_COEF;
				nowCol += diffCol;
				m_apModeUI[cnt]->SetCol(nowCol);
			}
			else
			{// それ以外半透明
				D3DXCOLOR nowCol = m_apModeUI[cnt]->GetCol();
				D3DXCOLOR diffCol = selectUI::NOSELECT_COLOR - nowCol;
				diffCol *= selectUI::SELECTUI_COLORCHANGE_COEF;
				nowCol += diffCol;
				m_apModeUI[cnt]->SetCol(nowCol);
			}
			m_apModeUI[cnt]->SetVtx();	// 頂点反映
		}
	}
}
