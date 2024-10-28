//*****************************************************
//
// ランキングの処理[Ranking.cpp]
//Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "ranking.h"
#include "fade.h"
#include "object.h"
#include "manager.h"
#include "texture.h"
#include "inputManager.h"
#include "skybox.h"
#include "universal.h"
#include "sound.h"
#include "meshcylinder.h"
#include "texture.h"
#include "camera.h"
#include "polygon2D.h"
#include "timer.h"
#include "cameraState.h"
#include "UI.h"
#include "light.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_PLACE					(6)										// スコアの桁数
#define TIMER_TRANS					(360)									// 遷移時間
#define RANKING_WIDTH			(80)	// 横幅
#define RANKING_HEIGHT			(80)	// 高さ
#define FLASH_SPEED					(10)	// 点滅スピード
#define RANKING_BIN_FILE			"data\\BIN\\ranking"								// ランキングファイル名
#define RANKING_TEX_FILE			"data\\TEXTURE\\UI\\Number000.png"		// ランキングテクスチャ名
#define BG_TEX_FILE					"data\\TEXTURE\\BG\\ResultBg000.png"	// ランキング背景テクスチャ名
#define RANKING_PATH	"data\\TEXTURE\\UI\\ranking00.png"	// 項目テクスチャ名

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SPEED_ROTATION = 0.003f;	// 回るスピード
const float HEIGHT_CYLINDER = 800.0f;	// シリンダーの高さ
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CRanking::s_nTime = 0;

//=====================================================
// コンストラクタ
//=====================================================
CRanking::CRanking()
{
	ZeroMemory(&m_apTimer[0],sizeof(m_apTimer));
	m_nCntState = 0;
	m_nRankUpdate = -1;
	m_nScore = 0;
	m_nTimerTrans = 0;
	ZeroMemory(&m_aScore[0],sizeof(int) * NUM_RANK);
	m_state = STATE_NORMAL;
}

//=====================================================
// デストラクタ
//=====================================================
CRanking::~CRanking()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CRanking::Init(void)
{
	//s_fTime = 49.0f;

	// 項目の生成
	CUI *pObject2D = CUI::Create();
	pObject2D->SetSize(0.3f,0.15f);
	pObject2D->SetPosition(D3DXVECTOR3(0.5f, 0.18f, 0.0f));

	int nIdx = CTexture::GetInstance()->Regist(RANKING_PATH);
	pObject2D->SetIdxTexture(nIdx);
	pObject2D->SetVtx();

	// 数字の生成
	for (int nCnt = 0; nCnt < NUM_RANK; nCnt++)
	{
		m_apTimer[nCnt] = CTimer::Create();
	}

	// 初期設定
	Reset();
	Sort();

	// スコアの取得
	//int nScore = Manager::GetScore();

	// 取得したスコアでランキング設定
	Set(s_nTime);

	// 保存
	Save();

	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		//pSound->Play(CSound::LABEL_BGM000);
	}

	Camera::ChangeState(new CFollowPlayer);

	CScene::Init();

	// カメラインスタンス取得
	CCamera *pCamera = CManager::GetCamera();
	assert(pCamera != nullptr);

	// カメラ情報の設定
	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	pInfoCamera->fLength = { 500.0f };			// 距離
	pInfoCamera->rot = { 1.35f, 0.0f, 0.0f };	// 向き
	pInfoCamera->posR = { 0.0f, 600.0f, 0.0f };	// 注視点位置
	pCamera->SetPosV();	// 視点反映

	D3DXVECTOR3 aDir[3] =
	{
		{ -1.4f, 0.24f, -2.21f, },
		{ 1.42f, -0.8f, 0.08f },
		{ -0.29f, -0.8f, 0.55f }
	};

	for (int i = 0; i < 3; i++)
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
	return S_OK;

}

//=====================================================
// 終了処理
//=====================================================
void CRanking::Uninit(void)
{
	s_nTime = 0;

	CScene::Uninit();

	CObject::ReleaseAll();
}

//=====================================================
// 更新処理
//=====================================================
void CRanking::Update(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	CFade *pFade = CFade::GetInstance();

	// シーンの更新
	CScene::Update();

	// カウントアップ
	m_nCntState++;

	if (m_nRankUpdate != -1)
	{// ランキングが更新された
		// 該当のランクを点滅==================================
		
		if (m_nCntState >= FLASH_SPEED)
		{// カウンタが一定の値に達したら
			switch (m_state)
			{
			case STATE_NORMAL:
				m_state = STATE_DIFF;
				break;
			case STATE_DIFF:
				m_state = STATE_NORMAL;
				break;
			}

			m_nCntState = 0;
		}

		// 変数宣言
		D3DXCOLOR colRanking = {0.0f,0.0f,0.0f,0.0f};

		switch (m_state)
		{// 状態による分岐
		case STATE_NORMAL:
			colRanking = { 1.0f,1.0f,1.0f,1.0f};
			break;
		case STATE_DIFF:
			colRanking = { 0.5f,1.0f,0.5f,1.0f };
			break;
		}

		if (m_apTimer[m_nRankUpdate] != nullptr)
		{
			m_apTimer[m_nRankUpdate]->SetColor(CTimer::E_Number::NUMBER_MINUTE, colRanking);
			m_apTimer[m_nRankUpdate]->SetColor(CTimer::E_Number::NUMBER_SECOND, colRanking);
		}
	}

	// 画面遷移==========================================
	if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
	{//ENTERキーが押されたら
		//タイトルに移行
		if (pFade != nullptr)
		{
			pFade->SetFade(CScene::MODE_TITLE);
		}
	}

	// 遷移タイマー進行
	m_nTimerTrans++;
}

//=====================================================
// 描画処理
//=====================================================
void CRanking::Draw(void)
{
	CScene::Draw();
}

//=====================================================
// 設定処理
//=====================================================
void CRanking::Set(int nTime)
{
	// 変数宣言
	D3DXVECTOR3 pos;
	char *apPath[NUM_RANK] = 
	{
		"data\\TEXTURE\\UI\\1st.png",
		"data\\TEXTURE\\UI\\2nd.png",
		"data\\TEXTURE\\UI\\3rd.png",
		"data\\TEXTURE\\UI\\4th.png",
		"data\\TEXTURE\\UI\\5th.png",
	};

	// ソート
	Sort();

	if (nTime < m_aScore[NUM_RANK - 1])
	{// 最小値を越したら代入
		m_aScore[NUM_RANK - 1] = nTime;

		// 再ソート
		Sort();

		for (int nCnt = 0; nCnt < NUM_RANK; nCnt++)
		{// 足した値と合致する記録を探す
			if ((int)nTime == (int)m_aScore[nCnt])
			{// ニューレコード番号を記録
				m_nRankUpdate = nCnt;
			}
		}
	}

	if (m_aScore != nullptr)
	{
		// 数字の設定
		for (int nCnt = 0; nCnt < NUM_RANK; nCnt++)
		{
			if (m_apTimer[nCnt] != nullptr)
			{
				// 値の設定
				m_apTimer[nCnt]->SetSecond(m_aScore[nCnt]);

				// 位置の設定
				pos = D3DXVECTOR3{ 0.5f,0.4f + 0.1f * nCnt,0.0f };

				m_apTimer[nCnt]->SetPosition(pos);

				pos = m_apTimer[nCnt]->GetPosition();

				m_apTimer[nCnt]->SetScaleNumber(1.0f);

				// 順位の生成
				CUI *pObject2D = CUI::Create();
				pObject2D->SetSize(0.05f, 0.05f);
				pos.x -= 0.19f;

				pObject2D->SetPosition(pos);

				int nIdx = CTexture::GetInstance()->Regist(apPath[nCnt]);
				pObject2D->SetIdxTexture(nIdx);
				pObject2D->SetVtx();
			}
		}
	}

	// 保存処理
#ifndef _DEBUG
	Save();
#endif
}

//=====================================================
// ランキングソート
//=====================================================
void CRanking::Sort(void)
{
	for (int nCntRanking = 0; nCntRanking < NUM_RANK - 1; nCntRanking++)
	{//ランキングをソート
		//左端の値を最大値とする
		int nTop = nCntRanking;

		for (int nCount2 = nCntRanking + 1; nCount2 < NUM_RANK; nCount2++)
		{//左の値と対象の値を比較
			if (m_aScore[nTop] > m_aScore[nCount2])
			{//もし比較した数字が小さかったら
				nTop = nCount2;
			}
		}

		//要素の入れ替え
		int nTemp = m_aScore[nCntRanking];
		m_aScore[nCntRanking] = m_aScore[nTop];
		m_aScore[nTop] = nTemp;
	}
}

//=====================================================
// ランキング情報リセット
//=====================================================
void CRanking::Reset(void)
{
	//外部ファイル読み込み
	Load();

#if 1
	//ランキング初期設定
	m_aScore[0] = 45;
	m_aScore[1] = 49;
	m_aScore[2] = 50;
	m_aScore[3] = 55;
	m_aScore[4] = 59;
#endif
}

//=====================================================
// ランキング情報保存
//=====================================================
void CRanking::Save(void)
{
	//ポインタ宣言
	FILE *pFile;

	//ファイルを開く
	pFile = fopen(RANKING_BIN_FILE, "wb");

	if (pFile != NULL)
	{//ファイルが開けた場合

	    //バイナリファイルに書き込む
		fwrite(&m_aScore[0], sizeof(float), NUM_RANK, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合

	}
}

//=====================================================
//ランキング情報読み込み
//=====================================================
void CRanking::Load(void)
{
	//ポインタ宣言
	FILE *pFile;

	//ファイルを開く
	pFile = fopen(RANKING_BIN_FILE, "rb");

	if (pFile != NULL)
	{//ファイルが開けた場合

	    //バイナリファイルから読み込む
		fread(&m_aScore[0], sizeof(float), NUM_RANK, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
	}
}