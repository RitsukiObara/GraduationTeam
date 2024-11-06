//*****************************************************
//
// 敵の処理[enemy.cpp]
// Author:早川友貴
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "enemy.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "iceManager.h"
#include "debugproc.h"
#include "seals.h"
#include "ocean.h"
#include "destroy_score.h"
#include "effect3D.h"
#include "manager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float HEIGHT_ICE = 100.0f;	// 氷の高さ
const float SPPED_MOVE_INIT = 1.6f;	// 初期移動速度
const float SPEED_ROTATION = 0.1f;	// 回転速度
const float TIME_DEATH_IN_DRIFT = 10.0f;	// 漂流して死ぬまでの時間
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
std::vector<CEnemy*> CEnemy::s_vector = {};	// 自身のポインタ

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CEnemy::CEnemy(int nPriority) : m_nGridV(0), m_nGridH(0),m_state(E_State::STATE_NONE), m_pIceLand(nullptr), m_bFollowIce(false),
m_move(),m_nGridVDest(0), m_nGridHDest(0), m_fSpeedMove(0.0f), m_fTimerDeath(0.0f)
{
	s_vector.push_back(this);
}

//=====================================================
// デストラクタ
//=====================================================
CEnemy::~CEnemy()
{

}

//=====================================================
// 生成処理
//=====================================================
CEnemy* CEnemy::Create(int nType, int nGridV, int nGridH)
{
	CEnemy* pEnemy = nullptr;

	switch (nType)
	{
	case CEnemy::TYPE_SEALS:

		pEnemy = new CSeals;

		break;
	
	default:

		assert(false);

		break;
	}

	if (pEnemy != nullptr)
	{// 敵生成
		// グリッド初期化
		pEnemy->SetGridV(nGridV);
		pEnemy->SetGridH(nGridH);

		// 初期化処理
		pEnemy->Init();
	}

	return pEnemy;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemy::Init(void)
{
	// 継承クラスの初期化
	CMotion::Init();

	// グリッド番号初期化
	InitGridIdx();

	// 状態初期化
	m_state = E_State::STATE_APPER;

	// 出現時のトランスフォーム設定
	SetApperTransform();

	// 氷追従フラグを設定
	m_bFollowIce = false;

	// 移動速度の初期設定
	m_fSpeedMove = SPPED_MOVE_INIT;

	return S_OK;
}

//=====================================================
// グリッド番号の初期化
//=====================================================
void CEnemy::InitGridIdx(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	if (pIceMgr->GetGridIce(&m_nGridV, &m_nGridH) != nullptr)
		return;

	pIceMgr->GetLeftDownIdx(&m_nGridV, &m_nGridH);
	m_nGridVNext = m_nGridV;
	m_nGridHNext = m_nGridH;

	D3DXVECTOR3 pos = pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH);
	SetPosition(pos);
}

//=====================================================
// 終了処理
//=====================================================
void CEnemy::Uninit(void)
{
	for (auto itr = s_vector.begin(); itr < s_vector.end(); itr++ )
	{
		//削除対象じゃない場合
		if (*itr != this)
		{
			continue;
		}
		//Vectorから削除
		s_vector.erase(itr);

		break;
	}

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemy::Update(void)
{
	CMotion::Update();

	if(m_bFollowIce)
		FollowIce();	// 氷に追従

	// 状態に応じた更新
	UpdateState updateFuncs[CEnemy::E_State::STATE_MAX] =
	{
		nullptr,
		&CEnemy::UpdateApper,
		&CEnemy::UpdateStop,
		&CEnemy::UpdateMove,
		&CEnemy::UpdateAttack,
		&CEnemy::UpdateDrift
	};

	if (updateFuncs[m_state] != nullptr)
		(this->*updateFuncs[m_state])();

	// 移動量を位置に加算
	AddPosition(m_move);

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 氷に追従
//=====================================================
void CEnemy::FollowIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	// 氷の高さに合わせる
	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);
	
	if (pIceStand != nullptr)
		pos.y = pIceStand->GetPosition().y;	// 高さを合わせる
	else
		SetState(E_State::STATE_DRIFT);	// 漂流状態にする

	pos.y += HEIGHT_ICE;

	SetPosition(pos);
}

//=====================================================
// 停止中の動き
//=====================================================
void CEnemy::UpdateStop(void)
{
	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=====================================================
// 移動中の動き
//=====================================================
void CEnemy::UpdateMove(void)
{
	// 目標に近い氷を探す
	SarchNearIceToDest();

	// 次のグリッドに向かう処理
	MoveToNextGrid();

	// グリッドを移ったかのチェック
	CheckChangeGrid();
}

//=====================================================
// 目標に近い氷を探す
//=====================================================
void CEnemy::SarchNearIceToDest(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;
	
	vector<CIce*> apIce = pIceMgr->GetAroundIce(m_nGridV, m_nGridH);

	// 目標位置の取得
	D3DXVECTOR3 posDest = pIceMgr->GetGridPosition(&m_nGridVDest, &m_nGridHDest);

	// 全方向の氷チェック
	float fLengthMin = FLT_MAX;

	int aV[CIceManager::DIRECTION_MAX] = {};
	int aH[CIceManager::DIRECTION_MAX] = {};

	// 周辺のグリッド番号の計算
	Grid::CalcAroundGrids(m_nGridV, m_nGridH, aV, aH);

	// 探索フラグ用の氷の配列
	vector<CIce*> apIceSave[CIceManager::DIRECTION_MAX];

	// 現在立っている氷の取得
	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	CIce *pIceNext = nullptr;

	// 一番コストの低い隣り合う氷をチェック
	size_t sizeMin = UINT_MAX;	// 最小コストの用意
	for (int i = 0; i < CIceManager::DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (enemy::IsEnemyOnIce(aV[i], aH[i]))
			continue;

		if (apIce[i]->IsPeck())
			continue;

		// 立っている氷を追加しておく
		apIceSave[i].push_back(pIceStand);

		// 経路の探索
		bool bFindPath = PathFind(aV[i], aH[i], apIceSave[i]);

		if (!bFindPath)
		{// 経路が行き詰りだったら配列をクリア
			apIceSave[i].clear();
		}
		else
		{// 到着したら、コストを比較する
			if (sizeMin <= apIceSave[i].size())
				continue;

			// コストが最も小さかったら保存
			sizeMin = apIceSave[i].size();

			pIceNext = apIceSave[i][1];
		}
	}

	if (pIceNext != nullptr)
	{// 次の氷が発見できたらその番号を次の番号にする
		pIceMgr->GetIceIndex(pIceNext, &m_nGridVNext, &m_nGridHNext);
	}
}

//=====================================================
// 探索の再帰関数
//=====================================================
bool CEnemy::PathFind(int nIdxV, int nIdxH, vector<CIce*>& rIceSave)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	// 周辺グリッドの計算
	vector<CIce*> apIce(CIceManager::DIRECTION_MAX);
	int aV[CIceManager::DIRECTION_MAX] = {};
	int aH[CIceManager::DIRECTION_MAX] = {};

	// 周辺のグリッド番号の計算
	Grid::CalcAroundGrids(nIdxV, nIdxH, aV, aH);

	// 探索済み配列に入れる
	CIce *pIceFind = pIceMgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIceFind == nullptr)
		return false;

	rIceSave.push_back(pIceFind);

	// 目標に到着していたら真を返す
	if (nIdxV == m_nGridVDest && nIdxH == m_nGridHDest)
		return true;

	// グリッド番号が範囲外にいってないかのチェック
	int nNumV = pIceMgr->GetNumGridV();
	int nNumH = pIceMgr->GetNumGridH();

	for (int i = 0; i < CIceManager::DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValueInt(&nV, nNumV - 1, 0) &&
			!universal::LimitValueInt(&nH, nNumH - 1, 0))
		{// 指定した番号がグリッドを越えていない場合のみ保存
			apIce[i] = pIceMgr->GetGridIce(&aV[i], &aH[i]);
		}
	}

	D3DXVECTOR3 posDest = pIceMgr->GetGridPosition(&m_nGridVDest, &m_nGridHDest);
	float fDistMin = FLT_MAX;

	int nIdxMin = 0;
	for (int i = 0; i < CIceManager::DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;	// 情報がないなら無視

		if (universal::FindFromVector(rIceSave, apIce[i]))
			continue;	// 探索済みなら無視

		if (enemy::IsEnemyOnIce(aV[i], aH[i]))
			continue;	// 他の敵が乗ってたら無視

		if (apIce[i]->IsPeck())
			continue;	// つっついたやつなら無視

		D3DXVECTOR3 posIce = apIce[i]->GetPosition();
		float fDiff = 0.0f;

		if (universal::DistCmpFlat(posIce, posDest, fDistMin, &fDiff))
		{
			nIdxMin = i;
			fDistMin = fDiff;
		}
	}

	return PathFind(aV[nIdxMin], aH[nIdxMin], rIceSave);	// 探索

	// ここまで通ったら行き詰まりのルート、偽を返す
	return false;
}

//=====================================================
// 次のグリッドに向かって移動する
//=====================================================
void CEnemy::MoveToNextGrid(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// 現在と次のグリッドの座標を取得
	D3DXVECTOR3 posNext = pIceMgr->GetGridPosition(&m_nGridVNext, &m_nGridHNext);
	D3DXVECTOR3 pos = GetPosition();

	// 差分ベクトルをスピード分に正規化
	D3DXVECTOR3 vecDiff = posNext - pos;
	universal::VecConvertLength(&vecDiff, m_fSpeedMove);
	SetMove(vecDiff);

	// 向きを補正する
	D3DXVECTOR3 rot = GetRotation();
	float fRotDest = atan2f(-vecDiff.x, -vecDiff.z);
	universal::FactingRot(&rot.y, fRotDest, SPEED_ROTATION);

	SetRotation(rot);
	
#ifdef _DEBUG
	//CEffect3D::Create(posNext, 100.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//CEffect3D::Create(pos, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
#endif
}

//=====================================================
// グリッドが変わったかのチェック
//=====================================================
void CEnemy::CheckChangeGrid(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// グリッド番号の取得
	D3DXVECTOR3 pos = GetPosition();
	CIce *pIce = pIceMgr->GetGridIce(&m_nGridVNext, &m_nGridHNext);
	
	if (!pIceMgr->IsInIce(pos, pIce))
	{// グリッドが変わってない時は偽を返す
		return;
	}
	else
	{// グリッドが変わってたら値を保存して真を返す
		m_nGridV = m_nGridVNext;
		m_nGridH = m_nGridHNext;

		if (m_nGridV == m_nGridVDest && m_nGridH == m_nGridHDest)
			AliveDestGrid();	// 目的地についた処理

		return;
	}
}

//=====================================================
// 漂流中の動き
//=====================================================
void CEnemy::UpdateDrift(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// 海流のベクトル取得
	CIceManager::E_Stream dir = pIceMgr->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// 流れる速度に正規化して位置を加算
	float fSpeedFlow = pIceMgr->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow;
	AddPosition(vecStream);

	COcean *pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
		return;

	// 海と一緒に氷を動かす処理
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SetMove(move);

	pos.y = pOcean->GetHeight(pos, &move) + HEIGHT_ICE;

	SetPosition(pos);

	// グリッドが合ったら止まる
	pIceMgr->GetIdxGridFromPosition(pos, &m_nGridV, &m_nGridH);

	CIce *pIce = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	if (pIce != nullptr)
		SetState(CEnemy::E_State::STATE_MOVE);

	// 漂流中の死
	DriftDeath();
}

//=====================================================
// 漂流中の死
//=====================================================
void CEnemy::DriftDeath(void)
{
	m_fTimerDeath += CManager::GetDeltaTime();

	if (m_fTimerDeath > TIME_DEATH_IN_DRIFT)
		Death();	// 一定時間経過したら死亡時の処理に入る
}

//=====================================================
// 死亡時の処理
//=====================================================
void CEnemy::Death(void)
{
	Uninit();
}

//=====================================================
// グリッドによる移動
//=====================================================
void CEnemy::TranslateByGrid(int nIdxV, int nIdxH)
{
	CIceManager *pIcemgr = CIceManager::GetInstance();

	if (pIcemgr == nullptr)
		return;

	D3DXVECTOR3 pos = pIcemgr->GetGridPosition(&nIdxV, &nIdxH);

	CIce *pIce = pIcemgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIce == nullptr)
		return;

	pos.y = pIce->GetPosition().y;

	SetPosition(pos);
}

//=====================================================
// デバッグ処理
//=====================================================
void CEnemy::Debug(void)
{
	CDebugProc* pDebugProc = CDebugProc::GetInstance();
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	if (pDebugProc == nullptr || pInputKeyboard == nullptr)
		return;

	pDebugProc->Print("\n現在グリッド[%d,%d]", m_nGridV, m_nGridH);
	pDebugProc->Print("\n次のグリッド[%d,%d]", m_nGridVNext, m_nGridHNext);
	pDebugProc->Print("\n目標グリッド[%d,%d]", m_nGridVDest, m_nGridHDest);

	pDebugProc->Print("\n現在の状態[%d]", m_state);
}

//=====================================================
// 描画処理
//=====================================================
void CEnemy::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}