//*****************************************************
//
// ステージ選択管理処理[SelectStageManager.cpp]
// Author:��山桃也
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

//*****************************************************
// マクロ定義
//*****************************************************
namespace
{
const string PATH_TEXT = "data\\TEXT\\selectStage.txt";	// テキストパス

const float RADIUS_COLLISION_PUSHOUT = 500.0f;	// 押し出し判定の半径
const float RATE_SELECT_COLLISION = 1.4f;	// 選択時の半径の割合

const float SCALE_STATE[CSelectStageManager::E_StateStage::STATE_MAX] = { 0.0f, 1.0f, 1.4f };	// 状態ごとのスケール

const float SPEED_SCALING_STAGE = 0.3f;	// スケーリングの速度
const float TIME_ENTER = 2.0f;	// エンターにかかる時間

const float SPEED_SCALING_PENGUIN = 0.05f;	// ペンギンのスケーリング速度
const float SPEED_MOVE_ENTER = 0.01f;	// エンター時の移動速度

const float SPEED_PARTICLE = 22.0f;	// パーティクルの速度
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************

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

	// 読込
	Load();

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

				m_aInfoStage.push_back(pInfoStage);
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
	}
}

//=====================================================
// ステージの設置
//=====================================================
void CSelectStageManager::SetStage(void)
{
	if (m_aInfoStage.empty())
		return;

	for (S_InfoStage *pInfoStage : m_aInfoStage)
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

		pInfoStage->pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT);
		pInfoStage->pCollision->SetPosition(GetPosition());

		// 状態の初期設定
		pInfoStage->state = E_StateStage::STATE_NORMAL;

		if (pInfoStage != *(m_aInfoStage.end() - 1))
		{// パーティクル位置の生成
			CGameObject *pObject = new CGameObject;

			m_aParticlePos.push_back(pObject);
			pObject->SetPosition(pInfoStage->pos);
		}
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

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 選択処理
//=====================================================
void CSelectStageManager::Select(void)
{
	for (int i = 0; i < (int)m_aInfoStage.size(); i++)
	{
		S_InfoStage *pInfoStage = m_aInfoStage[i];

		// スケーリング処理
		Scaling(pInfoStage);

		// パーティクルを出す
		SetParticle(i);

		if (pInfoStage->pCollision == nullptr)
			continue;

		// 判定の追従
		pInfoStage->pCollision->SetPosition(pInfoStage->pModel->GetPosition());

		// 判定の一時拡大
		pInfoStage->pCollision->SetRadius(RATE_SELECT_COLLISION * RADIUS_COLLISION_PUSHOUT);

		// プレイヤーが入っていたら選択状態にする
		bool bEnter = pInfoStage->pCollision->OnEnter(CCollision::TAG_PLAYER);

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
		}
		else
			pInfoStage->state = E_StateStage::STATE_NORMAL;

		// 判定の大きさを戻す
		pInfoStage->pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT);
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
	if (nIdx >= (int)m_aInfoStage.size() - 1)
		return;

	// 差分ベクトルを移動速度に正規化
	D3DXVECTOR3 pos = m_aInfoStage[nIdx]->pos;
	D3DXVECTOR3 posNext = m_aInfoStage[nIdx + 1]->pos;

	D3DXVECTOR3 vecDiff = posNext - pos;

	D3DXVECTOR3 move = vecDiff;
	universal::VecConvertLength(&move, SPEED_PARTICLE);

	// 移動量を反映
	m_aParticlePos[nIdx]->AddPosition(move);

	// パーティクルを発生
	D3DXVECTOR3 posParticle = m_aParticlePos[nIdx]->GetPosition();
	CParticle::Create(posParticle, CParticle::TYPE::TYPE_STAR_HIT);

	// 次の位置に一定以上近づいたら元の位置に戻す
	if (universal::DistCmpFlat(posParticle, posNext, RADIUS_COLLISION_PUSHOUT * RATE_SELECT_COLLISION, nullptr))
	{
		D3DXVECTOR3 posBack = pos;
		D3DXVECTOR3 vecLength = vecDiff;

		universal::VecConvertLength(&vecLength, RADIUS_COLLISION_PUSHOUT * RATE_SELECT_COLLISION);

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
	for (S_InfoStage *pInfoStage : m_aInfoStage)
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
	D3DXVECTOR3 posStage = m_aInfoStage[m_nIdxSelect]->pos;

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