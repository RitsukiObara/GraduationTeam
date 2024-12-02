//*****************************************************
//
// ステージ選択管理処理[SelectStageManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "selectStageManager.h"
#include "skybox.h"
#include "selectStagePenguin.h"
#include "camera.h"
#include "cameraState.h"
#include "inputManager.h"
#include "debugproc.h"
#include "inputkeyboard.h"
#include "fade.h"
#include "collision.h"
#include "manager.h"
#include "particle.h"
#include "ocean.h"
#include "gameManager.h"
#include "tutorial.h"
#include "number.h"

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
const string PATH_TEXT = "data\\TEXT\\selectStage.txt";	// テキストパス
const string PATH_BANNER = "data\\MODEL\\other\\stageselect_banner.x";

const float RADIUS_COLLISION_PUSHOUT_STAGE = 500.0f;	// 押し出し判定の半径（ステージ）
const float RADIUS_COLLISION_PUSHOUT_BANNER = 250.0f;	// 押し出し判定の半径（看板）
const float RATE_SELECT_COLLISION = 1.4f;				// 選択時の半径の割合

const float SCALE_STATE[CSelectStageManager::E_StateStage::STATE_MAX] = { 0.0f, 1.0f, 1.4f };	// 状態ごとのスケール

const float SPEED_SCALING_STAGE = 0.3f;	// スケーリングの速度
const float TIME_ENTER = 2.0f;			// エンターにかかる時間

const float SPEED_SCALING_PENGUIN = 0.05f;	// ペンギンのスケーリング速度
const float SPEED_MOVE_ENTER = 0.01f;		// エンター時の移動速度

const float SPEED_PARTICLE = 22.0f;								// パーティクルの速度
const D3DXVECTOR3 BANNER_POS = D3DXVECTOR3(0.0f, 0.0, 1300.0);	// 看板の位置

const float HEIGHT_NUMBER = 0.14f;				// 名前の高さ
const D3DXVECTOR2 SIZE_NAME = { 0.1f, 0.06f };	// 名前のサイズ
const string PATH_TEX_NAME[] =					// テクスチャのパス
{
	"",
	"",
	"",
	"",
};
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
vector<CSelectStageManager::S_InfoStage*> CSelectStageManager::s_aInfoStage;	// ステージ情報の配列

//=====================================================
// コンストラクタ
//=====================================================
CSelectStageManager::CSelectStageManager() : m_pPenguin(nullptr), m_bEnter(false), m_fTimerFade(0.0f), m_nIdxSelect(0)
{

}

//=====================================================
// 生成処理
//=====================================================
CSelectStageManager *CSelectStageManager::Create(void)
{
	CSelectStageManager *pManager = nullptr;

	pManager = new CSelectStageManager;

	if (pManager != nullptr)
	{
		// 初期化処理
		pManager->Init();
	}

	return pManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSelectStageManager::Init(void)
{
	// スカイボックスの生成
	CSkybox::Create();

	// カメラのステイト設定
	Camera::ChangeState(new CCameraStateSelectStage);

	// ステージ選択看板設置
	CObjectX* pBanner = CObjectX::Create(BANNER_POS);
	pBanner->BindModel(CModel::Load(&PATH_BANNER[0]));
	pBanner->SetScale(15.0f);
	CCollisionSphere *pCollision = CCollisionSphere::Create(CCollision::TAG::TAG_BLOCK, CCollision::TYPE::TYPE_SPHERE, this);
	if (pCollision != nullptr)
	{
		pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT_BANNER);
		pCollision->SetPosition(BANNER_POS);
	}

	// ステージの設置
	SetStage();

	// ペンギンの生成
	m_pPenguin = CSelectStagePenguin::Create();

	// 海の生成
	COcean::Create();

	return S_OK;
}

//=====================================================
// 読み込み処理
//=====================================================
void CSelectStageManager::Load(void)
{
	if (!s_aInfoStage.empty())
		return;

	std::ifstream file(PATH_TEXT);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// 読み込むものがなくなるまで読込
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "SETSTAGE")
			{// ステージ情報読込開始
				// ステージ情報の生成
				S_InfoStage *pInfoStage = new S_InfoStage;

				if (pInfoStage == nullptr)
					continue;

				LoadStage(file, temp, pInfoStage);

				s_aInfoStage.push_back(pInfoStage);
			}

			if (file.eof())
			{// 読み込み終了
				break;
			}
		}

		file.close();
	}
	else
	{
		assert(("ファイルが開けませんでした", false));
	}
}

//=====================================================
// ステージの読込
//=====================================================
void CSelectStageManager::LoadStage(std::ifstream& file, string str, S_InfoStage *pInfoStage)
{
	if (pInfoStage == nullptr)
		return;

	while (std::getline(file, str))
	{// 読込開始
		std::istringstream iss(str);
		std::string key;
		iss >> key;

		if (key == "END_SETSTAGE")
		{// 終了
			break;
		}

		if (key == "MODEL")
		{// パス
			iss >> str >> pInfoStage->pathModel;
		}

		if (key == "POS")
		{// 位置
			iss >> str >> pInfoStage->pos.x >> pInfoStage->pos.y >> pInfoStage->pos.z;
		}

		if (key == "PATHMAP")
		{// マップのパス
			iss >> str >> pInfoStage->pathMap;
		}

		if (key == "PATHENEMY")
		{// 敵のパス
			iss >> str >> pInfoStage->pathEnemy;
		}

		if (key == "DIR_STREAM")
		{// 海流の向き
			iss >> str >> pInfoStage->nDirStream;
		}
	}
}

//=====================================================
// ステージの設置
//=====================================================
void CSelectStageManager::SetStage(void)
{
	if (s_aInfoStage.empty())
		return;

	int nIdx = 1;

	for (S_InfoStage *pInfoStage : s_aInfoStage)
	{
		// ステージのXモデルの設置
		pInfoStage->pModel = CObjectX::Create();

		if (pInfoStage->pModel == nullptr)
			return;

		int nIdxModel = CModel::Load(&pInfoStage->pathModel[0]);
		pInfoStage->pModel->BindModel(nIdxModel);

		pInfoStage->pModel->SetPosition(pInfoStage->pos);

		// 当たり判定の生成
		pInfoStage->pCollision = CCollisionSphere::Create(CCollision::TAG::TAG_BLOCK, CCollision::TYPE::TYPE_SPHERE, this);

		if (pInfoStage->pCollision == nullptr)
			return;

		pInfoStage->pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT_STAGE);
		pInfoStage->pCollision->SetPosition(GetPosition());

		// 状態の初期設定
		pInfoStage->state = E_StateStage::STATE_NORMAL;

		if (pInfoStage != *(s_aInfoStage.end() - 1))
		{// パーティクル位置の生成
			CGameObject *pObject = new CGameObject;

			m_aParticlePos.push_back(pObject);
			pObject->SetPosition(pInfoStage->pos);
		}

		// 名前の生成
		pInfoStage->pName = CUI::Create();

		if (pInfoStage->pName == nullptr)
			return;

		D3DXVECTOR3 posScreen;
		universal::IsInScreen(pInfoStage->pos,&posScreen);
		universal::ConvertScreenRate(posScreen);

		pInfoStage->pName->SetPosition(D3DXVECTOR3(posScreen.x, posScreen.y - HEIGHT_NUMBER, 0.0f));
		pInfoStage->pName->SetSize(SIZE_NAME.x, SIZE_NAME.y);
		pInfoStage->pName->SetVtx();

		nIdx++;
	}
}

//=====================================================
// 終了処理
//=====================================================
void CSelectStageManager::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CSelectStageManager::Update(void)
{
	// 選択処理
	Select();

	if (m_bEnter)
		StayEnter();	// エンター中の処理
	else
		// モードセレクトに戻る処理
		ModeSelectBack();
	

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 選択処理
//=====================================================
void CSelectStageManager::Select(void)
{
	for (int i = 0; i < (int)s_aInfoStage.size(); i++)
	{
		S_InfoStage *pInfoStage = s_aInfoStage[i];

		// スケーリング処理
		Scaling(pInfoStage);

		// パーティクルを出す
		SetParticle(i);

		if (pInfoStage->pCollision == nullptr)
			continue;

		// 判定の追従
		pInfoStage->pCollision->SetPosition(pInfoStage->pModel->GetPosition());

		// 判定の一時拡大
		pInfoStage->pCollision->SetRadius(RATE_SELECT_COLLISION * RADIUS_COLLISION_PUSHOUT_STAGE);

		// プレイヤーが入っていたら選択状態にする
		bool bEnter = pInfoStage->pCollision->OnEnter(CCollision::TAG_PLAYER);

		// 判定の大きさを戻す
		pInfoStage->pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT_STAGE);

		if (bEnter)
		{
			pInfoStage->state = E_StateStage::STATE_SELECT;

			// エンター操作の検出
			if (m_pPenguin != nullptr)
			{
				if (m_pPenguin->IsEnter())
				{// エンターの開始
					m_nIdxSelect = i;

					StartEnter();

					return;
				}
			}

			break;
		}
		else
			pInfoStage->state = E_StateStage::STATE_NORMAL;
	}
}

//=====================================================
// スケーリング処理
//=====================================================
void CSelectStageManager::Scaling(S_InfoStage *pInfoStage)
{
	CObjectX *pModel = pInfoStage->pModel;

	if (pModel == nullptr)
		return;

	float fScaleDest = SCALE_STATE[pInfoStage->state];
	float fScale = pModel->GetScale().x;

	fScale += (fScaleDest - fScale) * SPEED_SCALING_STAGE;

	pModel->SetScale(fScale);
}

//=====================================================
// パーティクルの発生
//=====================================================
void CSelectStageManager::SetParticle(int nIdx)
{
	if (nIdx >= (int)s_aInfoStage.size() - 1)
		return;

	// 差分ベクトルを移動速度に正規化
	D3DXVECTOR3 pos = s_aInfoStage[nIdx]->pos;
	D3DXVECTOR3 posNext = s_aInfoStage[nIdx + 1]->pos;

	D3DXVECTOR3 vecDiff = posNext - pos;

	D3DXVECTOR3 move = vecDiff;
	universal::VecConvertLength(&move, SPEED_PARTICLE);

	// 移動量を反映
	m_aParticlePos[nIdx]->Translate(move);

	// パーティクルを発生
	D3DXVECTOR3 posParticle = m_aParticlePos[nIdx]->GetPosition();
	CParticle::Create(posParticle, CParticle::TYPE::TYPE_STAGESELECT_SNOW);

	// 次の位置に一定以上近づいたら元の位置に戻す
	if (universal::DistCmpFlat(posParticle, posNext, RADIUS_COLLISION_PUSHOUT_STAGE * RATE_SELECT_COLLISION, nullptr))
	{
		D3DXVECTOR3 posBack = pos;
		D3DXVECTOR3 vecLength = vecDiff;

		universal::VecConvertLength(&vecLength, RADIUS_COLLISION_PUSHOUT_STAGE * RATE_SELECT_COLLISION);

		posBack += vecLength;

		m_aParticlePos[nIdx]->SetPosition(posBack);
	}
}

//=====================================================
// エンター開始
//=====================================================
void CSelectStageManager::StartEnter(void)
{
	// タイマーのリセット
	m_fTimerFade = 0.0f;

	// エンターフラグを立てる
	m_bEnter = true;

	// ペンギンを操作不能にする
	if (m_pPenguin != nullptr)
		m_pPenguin->EnableInput(false);

	// 当たり判定の削除
	for (S_InfoStage *pInfoStage : s_aInfoStage)
	{
		if (pInfoStage->pCollision != nullptr)
		{
			pInfoStage->pCollision->Uninit();
			pInfoStage->pCollision = nullptr;
		}
	}

	// ステージ番号保存
	gameManager::SaveIdxMap(m_nIdxSelect);
}

//=====================================================
// エンター中の処理
//=====================================================
void CSelectStageManager::StayEnter(void)
{
	m_fTimerFade += CManager::GetDeltaTime();

	if (TIME_ENTER < m_fTimerFade)
		EndEnter();	// エンターの終了

	if (m_pPenguin == nullptr)
		return;	// 以下、ペンギンの処理

	// ステージに引っ張られる処理
	D3DXVECTOR3 pos = m_pPenguin->GetPosition();
	D3DXVECTOR3 posStage = s_aInfoStage[m_nIdxSelect]->pos;

	universal::MoveToDest(&pos, posStage, SPEED_MOVE_ENTER);

	m_pPenguin->SetPosition(pos);

	// スケーリングの処理
	float fScale = m_pPenguin->GetParts(0)->pParts->GetScale().x;

	fScale += (0 - fScale) * SPEED_SCALING_PENGUIN;

	m_pPenguin->GetParts(0)->pParts->SetScale(fScale);
}

//=====================================================
// エンター終了
//=====================================================
void CSelectStageManager::EndEnter(void)
{
	// フェードの開始
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	bool bEnterTutorial = tutorial::LoadFrag();

	if(bEnterTutorial)
		pFade->SetFade(CScene::MODE::MODE_TUTORIAL);
	else
		pFade->SetFade(CScene::MODE::MODE_GAME);
}

//=====================================================
// デバッグ処理
//=====================================================
void CSelectStageManager::Debug(void)
{
	CDebugProc* pDebugProc = CDebugProc::GetInstance();
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();

	if (pDebugProc == nullptr || pKeyboard == nullptr)
		return;

	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	if (pKeyboard->GetTrigger(DIK_1))
		pFade->SetFade(CScene::MODE::MODE_GAME);
}

//=====================================================
// 描画処理
//=====================================================
void CSelectStageManager::Draw(void)
{

}

//=====================================================
// モードセレクトに戻る処理
//=====================================================
void CSelectStageManager::ModeSelectBack(void)
{
	// フェードの開始
	CFade* pFade = CFade::GetInstance();

	CInputManager* pInput = CInputManager::GetInstance();

	if (pInput == nullptr)
		return;

	if (pInput->GetTrigger(CInputManager::BUTTON_BACK))	// BACK押したとき
	{
		pFade->SetFade(CScene::MODE::MODE_SELECTMODE);
	}
}