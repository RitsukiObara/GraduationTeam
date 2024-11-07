//*****************************************************
//
// 氷の処理[ice.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "ice.h"
#include "texture.h"
#include "gameObject.h"
#include "fan3D.h"
#include "meshcylinder.h"
#include "ocean.h"
#include "iceHard.h"
#include "objectX.h"
#include "iceManager.h"
#include "manager.h"
#include "particle.h"
#include "model.h"
#include "effect3D.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const string PATH_TEX = "data\\TEXTURE\\MATERIAL\\ice001.jpg";	// テクスチャパス
const float SIZE_INIT = 100.0f;	// 初期サイズ
const float HEIGHT_ICE = 50.0f;	// 氷の高さ
const int NUM_CORNER = 6;	// 角の数
const D3DXVECTOR3 ROT_UP_INIT = { D3DX_PI * 0.5f,0.0f,0.0f };	// 上側の初期向き

const float TIME_REPAIR_ICE = 10.0f;	// 氷の修復にかかる時間
const int MAX_SCALE = 50; // スケールの最大値
const int MIN_SCALE = 20; // スケールの最小値
const string PATH_ICE_DEBRIS = "data\\MODEL\\block\\Drift_ice_piece.x";	// 破片氷のモデルパス
const float SPEED_SINK = 5.0f;	// 沈む速度
const float HEIGHT_DELETE = -100.0f;	// 削除するまでの高さ

const float LINE_STOP_ICE = 1.0f;	// 氷が止まるしきい値
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CIce::s_nNumAll = 0;
std::vector<CIce*> CIce::m_Vector = {};	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CIce::CIce(int nPriority) : CGameObject(nPriority), m_state(E_State::STATE_NONE), m_bBreak(false), m_bCanFind(false), m_bPeck(false), m_bAliveStandBlock(false),
m_pSide(nullptr),m_pUp(nullptr), m_pState(nullptr), m_bSink(false), m_bStop(nullptr)
{
	s_nNumAll++;
	m_Vector.push_back(this);
}

//=====================================================
// デストラクタ
//=====================================================
CIce::~CIce()
{
	s_nNumAll--;
}

//=====================================================
// 生成処理
//=====================================================
CIce *CIce::Create(E_Type type,E_State state)
{
	CIce *pIce = nullptr;

	if (pIce == nullptr)
	{
		switch (type)
		{
		case CIce::TYPE_NORMAL:
			pIce = new CIce;
			break;
		case CIce::TYPE_HARD:
			pIce = new CIceHard;
			break;
		default:
			assert(false);
			break;
		}

		if (pIce != nullptr)
		{
			pIce->m_state = state;
			pIce->Init();
		}
	}

	return pIce;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CIce::Init(void)
{
	EnableCanFind(true);

	// メッシュの生成
	CreateMesh();

	// トランスフォームの初期設定
	SetTransform(SIZE_INIT);

	ChangeState(new CIceStaeteNormal);

	return S_OK;
}

//=====================================================
// メッシュの生成
//=====================================================
void CIce::CreateMesh(void)
{
	if (m_pUp == nullptr)
	{
		m_pUp = CFan3D::Create(3, NUM_CORNER);

		if (m_pUp != nullptr)
		{
			m_pUp->SetRotation(ROT_UP_INIT);
			int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
			m_pUp->SetIdxTexture(nIdxTexture);
		}
	}

	if (m_pSide == nullptr)
	{
		m_pSide = CMeshCylinder::Create(NUM_CORNER);
		int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
		m_pSide->SetIdxTexture(nIdxTexture);
	}

	SetTransform(Grid::SIZE);
}

//=====================================================
// メッシュの破棄
//=====================================================
void CIce::DeleteMesh(void)
{
	Object::DeleteObject((CObject**)&m_pUp);
	Object::DeleteObject((CObject**)&m_pSide);
}

//=====================================================
// 終了処理
//=====================================================
void CIce::Uninit(void)
{
	// メッシュの削除
	DeleteMesh();

	for (auto itr = m_Vector.begin(); itr < m_Vector.end(); itr++)
	{
		//削除対象じゃない場合
		if (*itr != this)
		{
			continue;
		}
		//Vectorから削除
		m_Vector.erase(itr);

		break;
	}

	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	CGameObject::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CIce::Update(void)
{	
	if (!IsSink())	// 沈むフラグがたっていないときのみ行う
		FollowWave();	// 波に追従する処理

	if (m_pState != nullptr)
		m_pState->Update(this);
}

//=====================================================
// 波に追従する処理
//=====================================================
void CIce::FollowWave(void)
{
	COcean *pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
	{
		return;
	}

	// 海と一緒に氷を動かす処理
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos.y = pOcean->GetHeight(pos, &move) + HEIGHT_ICE;

	if (m_pUp != nullptr)
	{
		m_pUp->SetPosition(pos);
	}
	if (m_pSide != nullptr)
	{
		m_pSide->SetPosition(pos + D3DXVECTOR3(0.0f, -HEIGHT_ICE, 0.0f));
	}

	SetPosition(pos);
}

//=====================================================
// 描画処理
//=====================================================
void CIce::Draw(void)
{

}

//=====================================================
// トランスフォーム設定
//=====================================================
void CIce::SetTransform(float fRadius)
{
	if (m_pUp == nullptr || m_pSide == nullptr)
		return;

	D3DXVECTOR3 posIce = GetPosition();

	// 上の扇ポリゴンの設定
	m_pUp->SetRadius(fRadius * 0.5f);
	m_pUp->SetPosition(posIce);
	m_pUp->SetVtx();

	// サイドのシリンダーの設定
	m_pSide->SetRadius(fRadius);
	m_pSide->SetHeight(HEIGHT_ICE);
	D3DXVECTOR3 posSide = posIce;
	posSide.y -= HEIGHT_ICE;
	m_pSide->SetPosition(posSide);
}

//=====================================================
// ステイトの変更
//=====================================================
void CIce::ChangeState(CIceState *pState)
{
	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	m_pState = pState;

	if (m_pState != nullptr)
		m_pState->Init(this);
}

//*******************************************************************************
// 通常ステイト
//*******************************************************************************
//=====================================================
// 初期化処理
//=====================================================
void CIceStaeteNormal::Init(CIce *pIce)
{
	pIce->EnableStop(true);

	// 漂着している番号を取得
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->GetIceIndex(pIce, &m_nIdxDriftV, &m_nIdxDriftH);
}

//=====================================================
// 終了処理
//=====================================================
void CIceStaeteNormal::Uninit(CIce *pIce)
{

}

//=====================================================
// 更新処理
//=====================================================
void CIceStaeteNormal::Update(CIce *pIce)
{
	// 番号取得に失敗しているかの確認
	bool bMove = CheckFailGetIndex(pIce);

	if(bMove)
		MoveToGrid(pIce);	// グリッドに向かって移動する処理
}

//=====================================================
// 番号取得に失敗しているかの確認
//=====================================================
bool CIceStaeteNormal::CheckFailGetIndex(CIce *pIce)
{
	if (m_nIdxDriftV == -1 ||
		m_nIdxDriftH == -1)
	{// 番号取得に失敗している場合、再取得
		CIceManager *pIceMgr = CIceManager::GetInstance();

		if (pIceMgr == nullptr)
			return false;

		pIceMgr->GetIceIndex(pIce, &m_nIdxDriftV, &m_nIdxDriftH);

		if (m_nIdxDriftV == 0 && m_nIdxDriftH == 0)
		{
			int n = 0;
		}

		if (m_nIdxDriftV == -1 ||
			m_nIdxDriftH == -1)
			return false;	// それでも失敗した場合、偽を返す
	}

	return true;
}

//=====================================================
// グリッドに向かって移動する処理
//=====================================================
void CIceStaeteNormal::MoveToGrid(CIce *pIce)
{
	// グリッドの位置を取得
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 posGrid = pIceMgr->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);
	D3DXVECTOR3 posIce = pIce->GetPosition();

	// 差分ベクトルを漂流速度に正規化
	float fSpeedFlow = pIceMgr->GetOceanLevel();
	D3DXVECTOR3 vecDiff = posGrid - posIce;
	D3DXVec3Normalize(&vecDiff, &vecDiff);

	vecDiff *= fSpeedFlow;

	// 氷の位置に移動量を加算
	pIce->AddPosition(vecDiff);
}

//*******************************************************************************
// 崩壊ステイト
//*******************************************************************************
//=====================================================
// 初期化処理
//=====================================================
void CIceStaeteBreak::Init(CIce *pIce)
{
	// 氷のメッシュを削除
	pIce->DeleteMesh();
	
	// ここで氷破壊時のエフェクトを発生
	D3DXVECTOR3 posIce = pIce->GetPosition();
	CParticle::Create(posIce, CParticle::TYPE::TYPE_ICEBREAK);

	// 氷破片を生成
	for (int i = 0; i < CIceStaeteBreak::NUM_ICE_BREAK; i++)
	{
		CObjectX *pPeace = CObjectX::Create();

		// モデルの割り当て
		int nIdxModel = CModel::Load((char*)&PATH_ICE_DEBRIS[0]);
		pPeace->BindModel(nIdxModel);

		// 位置をランダム設定
		D3DXVECTOR3 posPeace = posIce;
		posIce.x += (float)universal::RandRange((int)(Grid::SIZE * 0.25f), -(int)(Grid::SIZE * 0.25f));
		posIce.z += (float)universal::RandRange((int)(Grid::SIZE * 0.25f), -(int)(Grid::SIZE * 0.25f));

		pPeace->SetPosition(posIce);

		// スケールをランダム設定
		float fRand = universal::RandRange(MAX_SCALE, MIN_SCALE) * 0.1f;
		pPeace->SetScale(fRand);

		// 回転
		float fRotY = universal::RandRange(314, 0) * 0.01f;
		pPeace->SetRotation(D3DXVECTOR3(0.0f, fRotY, 0.0f));

		// 配列に入れる
		m_aPeaceIce.push_back(pPeace);
	}

	// カウンターを初期化
	m_fTimerRepair = TIME_REPAIR_ICE;
}

//=====================================================
// 終了処理
//=====================================================
void CIceStaeteBreak::Uninit(CIce *pIce)
{
	for (auto it : m_aPeaceIce)
		it->Uninit();

	m_aPeaceIce.clear();
}

//=====================================================
// 更新処理
//=====================================================
void CIceStaeteBreak::Update(CIce *pIce)
{
	// 氷の修復の更新
	UpdateRepair(pIce);

	// 氷の追従
	FollowIce(pIce);
}

//=====================================================
// 修復の更新
//=====================================================
void CIceStaeteBreak::UpdateRepair(CIce *pIce)
{
	// 沈んでいる状態なら通らない
	if (pIce->IsSink())
		return;

	// タイマーを減らす
	m_fTimerRepair -= CManager::GetDeltaTime();

	if (m_fTimerRepair <= 0.0f)
	{// 一定時間経過で、氷を修復する
		pIce->CreateMesh();

		pIce->ChangeState(new CIceStaeteNormal);
		pIce->EnablePeck(false);
	}
}

//=====================================================
// 氷の追従
//=====================================================
void CIceStaeteBreak::FollowIce(CIce *pIce)
{
	// 氷の追従
	for (auto it : m_aPeaceIce)
	{
		D3DXVECTOR3 pos = { it->GetPosition().x, pIce->GetPosition().y, it->GetPosition().z };

		it->SetPosition(pos);
	}

	// 沈む状態の時、本体の位置を下げる
	if (pIce->IsSink())
	{
		D3DXVECTOR3 posIce = pIce->GetPosition();
		posIce.y -= SPEED_SINK;
		pIce->SetPosition(posIce);

		if (posIce.y < HEIGHT_DELETE)
		{// 一定まで沈んだら削除する
			pIce->Uninit();

			return;
		}
	}
}

//*******************************************************************************
// 流れるステイト
//*******************************************************************************
//=====================================================
// 初期化処理
//=====================================================
void CIceStateFlow::Init(CIce *pIce)
{
	pIce->EnableStop(false);

	// 自身のポインタをグリッドから外す
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->DeleteIce(pIce);
}

//=====================================================
// 終了処理
//=====================================================
void CIceStateFlow::Uninit(CIce *pIce)
{

}

//=====================================================
// 更新処理
//=====================================================
void CIceStateFlow::Update(CIce *pIce)
{
	if (!m_bDrift)
		UpdateSarchIce(pIce);	// 氷を探している時の更新
	else
		UpdateDriftIce(pIce);	// 漂着してるときの更新
}

//=====================================================
// 氷を探しているときの更新
//=====================================================
void CIceStateFlow::UpdateSarchIce(CIce *pIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// 海流のベクトル取得
	CIceManager::E_Stream dir = pIceManager->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// 流れる速度に正規化して位置を加算
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow;
	pIce->AddPosition(vecStream);

	// 氷との判定
	CollideIce(pIce);
}

//=====================================================
// 漂着するときの更新
//=====================================================
void CIceStateFlow::UpdateDriftIce(CIce *pIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// グリッドの位置取得
	D3DXVECTOR3 posDrift = pIceManager->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);

	// 海流のベクトル取得
	D3DXVECTOR3 posIce = pIce->GetPosition();
	if (pIceManager == nullptr)
		return;

	// 海流のベクトル取得
	D3DXVECTOR3 vecDiff = posDrift - posIce;

	// 流れる速度に正規化して位置を加算
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecDiff, &vecDiff);
	vecDiff *= fSpeedFlow;
	pIce->AddPosition(vecDiff);

	// グリッドの位置との距離がしきい値を下回ったら止める
	bool bStop = universal::DistCmpFlat(posIce, posDrift, LINE_STOP_ICE, nullptr);

#ifdef _DEBUG
	//CEffect3D::Create(posIce, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
#endif

	if (bStop)
	{
		return;
	}
}

//=====================================================
// 氷との判定
//=====================================================
void CIceStateFlow::CollideIce(CIce *pIce)
{
	// 今いるグリッドの取得
	int nIdxV = 0;
	int nIdxH = 0;

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	D3DXVECTOR3 posIce = pIce->GetPosition();
	bool bOk = pIceManager->GetIdxGridFromPosition(posIce, &nIdxV, &nIdxH);

	if (bOk)
	{
		// 番号を保存
		m_nIdxDriftV = nIdxV;
		m_nIdxDriftH = nIdxH;
	}

	// 海流の方向に合わせた判定関数の呼び出し
	DirectionFunc directionFuncs[CIceManager::E_Stream::STREAM_MAX] = 
	{
		&CIceStateFlow::CheckUp,
		&CIceStateFlow::CheckRight,
		&CIceStateFlow::CheckDown,
		&CIceStateFlow::CheckLeft
	};

	CIceManager::E_Stream stream = pIceManager->GetDirStream();
	
	// 漂着する氷があったら、フラグを立てて漂着グリッド番号を保存
	m_bDrift = (this->*directionFuncs[stream])(pIce, m_nIdxDriftV, m_nIdxDriftH);

#ifdef _DEBUG
	D3DXVECTOR3 posGrid = pIceManager->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);
	CEffect3D::Create(posGrid, 100.0f, 5, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
#endif

	if (m_bDrift)
	{
		if (nIdxH == 0 && nIdxV == 0)
		{
			int n = 0;
		}

		// グリッドに氷情報を保存
		pIceManager->SetIceInGrid(nIdxV, nIdxH, pIce);
		pIce->ChangeState(new CIceStaeteNormal);
	}

#ifdef _DEBUG
	CEffect3D::Create(posIce, 100.0f, 5, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
#endif
}

//=====================================================
// 上方向の確認
//=====================================================
bool CIceStateFlow::CheckUp(CIce *pIce, int nIdxV, int nIdxH)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// 周辺の氷の取得
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// 左側のグリッドどれかに氷があれば漂着
	if (apIce[CIceManager::DIRECTION_LEFTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHTUP] != nullptr)
	{
		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// 右方向の確認
//=====================================================
bool CIceStateFlow::CheckRight(CIce *pIce, int nIdxV, int nIdxH)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// 周辺の氷の取得
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// 左側のグリッドどれかに氷があれば漂着
	if (apIce[CIceManager::DIRECTION_RIGHTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHT] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHTDOWN] != nullptr)
	{
		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// 下方向の確認
//=====================================================
bool CIceStateFlow::CheckDown(CIce *pIce, int nIdxV, int nIdxH)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// 周辺の氷の取得
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// 左側のグリッドどれかに氷があれば漂着
	if (apIce[CIceManager::DIRECTION_RIGHTDOWN] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFTDOWN] != nullptr)
	{
		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// 左方向の確認
//=====================================================
bool CIceStateFlow::CheckLeft(CIce *pIce, int nIdxV, int nIdxH)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// 周辺の氷の取得
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// 左側のグリッドどれかに氷があれば漂着
	if (apIce[CIceManager::DIRECTION_LEFTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFT] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFTDOWN] != nullptr)
	{
		bDrift = true;
	}

	return bDrift;
}

//*******************************************************************************
// 沈むステイト
//*******************************************************************************
//=====================================================
// 初期化処理
//=====================================================
void CIceStaeteSink::Init(CIce *pIce)
{

}

//=====================================================
// 終了処理
//=====================================================
void CIceStaeteSink::Uninit(CIce *pIce)
{

}

//=====================================================
// 更新処理
//=====================================================
void CIceStaeteSink::Update(CIce *pIce)
{

}