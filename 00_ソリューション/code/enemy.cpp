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
#include "flowIce.h"
#include "debugproc.h"
#include "seals.h"
#include "bears.h"
#include "ocean.h"
#include "destroy_score.h"
#include "UI_combo.h"
#include "effect3D.h"
#include "manager.h"
#include "sound.h"
#include "shadow.h"
#include "player.h"
#include "game.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float HEIGHT_ICE = 100.0f;	// 氷の高さ
const float SPPED_MOVE_INIT = 1.6f;	// 初期移動速度
const float SPEED_ROTATION = 0.05f;	// 回転速度
const float TIME_DEATH_IN_DRIFT = 6.0f;	// 漂流して死ぬまでの時間

const float LINE_STOP_TURN = 0.2f;	// 振り向きを停止するしきい値
const float LINE_START_TURN = D3DX_PI * 0.6f;	// 振り向きを開始するしきい値
const float FACT_ROTATION_TURN = 0.07f;	// 振り向き回転係数

const float LINE_ENABLE_MOVE = 0.1f;	// 移動開始できる角度のしきい値

const float RATE_STOP_FLOW_ICE_RADIUS = 0.5f;	// 漂流停止する際に検出する氷の半径の割合

const float RATE_STOP_CHARGE = 0.6f;	// 突進を止めるときの氷のサイズの割合
const float RANGE_STOP_MOVE = D3DX_PI * 1 / CIceManager::E_Direction::DIRECTION_MAX;	// 移動を止める角度の範囲

const float RATE_COLLIDEICE = 1.0f;	// 氷との判定の割合

const int MAX_DEPTH_FIND = 50;	// これ以上探索しない深さ

const float LINE_FACT_ROT = 10.0f;	// これ以上の長さがないと向きを補正しない
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
std::vector<CEnemy*> CEnemy::s_vector = {};	// 自身のポインタ

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CEnemy::CEnemy(int nPriority) : m_nGridV(0), m_nGridH(0), m_state(E_State::STATE_NONE), m_pIceLand(nullptr), m_bFollowIce(false),
m_move(), m_nGridVDest(0), m_nGridHDest(0), m_fSpeedMove(0.0f), m_fFactRot(0.0f), m_fTimerDeath(0.0f), m_bTurn(false), m_bEnableMove(false), m_pLandSystemFlow(nullptr),
m_bMoveByGrid(false), m_pShadow(nullptr), m_spawn(E_Spawn::SPAWN_RU), m_type(TYPE_SEALS)
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
CEnemy* CEnemy::Create(int nType, E_Spawn spawn)
{
	CEnemy* pEnemy = nullptr;

	switch (nType)
	{
	case CEnemy::TYPE_SEALS:
		pEnemy = new CSeals;
		break;
	case CEnemy::TYPE_BEARS:
		pEnemy = new CBears;
		break;
	default:
		assert(false);
		break;
	}

	if (pEnemy != nullptr)
	{// 敵生成
		// グリッド番号初期化
		pEnemy->InitGridIdx(spawn);
		pEnemy->m_spawn = spawn;

		pEnemy->m_type = (TYPE)nType;

		// 初期化処理
		pEnemy->Init();

		// 海から飛び出しサウンド
		CSound* pSound = CSound::GetInstance();
		if (pSound != nullptr)
		{
			pSound->Play(CSound::LABEL_SE_SEA_SPLASH_01);
			pSound->Play(CSound::LABEL_SE_SEA_SPLASH_02);
		}
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

	// 状態初期化
	m_state = E_State::STATE_APPER;

	// 出現時のトランスフォーム設定
	SetApperTransform();

	// フラグを設定
	m_bFollowIce = false;
	m_bEnableMove = true;
	m_bMoveByGrid = true;

	// パラメーターの初期設定
	m_fSpeedMove = SPPED_MOVE_INIT;
	m_fFactRot = SPEED_ROTATION;

	// 影の生成
	m_pShadow = CShadow::Create();
	if (m_pShadow != nullptr)
		m_pShadow->SetPosition(GetPosition());

	return S_OK;
}

//=====================================================
// グリッド番号の初期化
//=====================================================
void CEnemy::InitGridIdx(E_Spawn spawn)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	if (pIceMgr->GetGridIce(&m_nGridV, &m_nGridH) != nullptr)
		return;

	switch (spawn)
	{
	case CEnemy::SPAWN_RU:
		pIceMgr->GetRightUpIdx(&m_nGridV, &m_nGridH);
		break;
	case CEnemy::SPAWN_LU:
		pIceMgr->GetLeftUpIdx(&m_nGridV, &m_nGridH);
		break;
	case CEnemy::SPAWN_RD:
		pIceMgr->GetRightDownIdx(&m_nGridV, &m_nGridH);
		break;
	case CEnemy::SPAWN_LD:
		pIceMgr->GetLeftDownIdx(&m_nGridV, &m_nGridH);
		break;
	default:
		assert(false);
		break;
	}

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
	Object::DeleteObject((CObject**)&m_pShadow);

	for (auto itr = s_vector.begin(); itr < s_vector.end(); itr++ )
	{
		//削除対象じゃない場合
		if (*itr != this)
			continue;

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

	// 安全な氷を探す処理
	SarchSafeIce();

	if (m_bFollowIce)
		FollowIce();	// 氷に追従

	// 移動量を位置に加算
	Translate(m_move);

	// 漂流開始の判定
	StartFlows();

	// 氷との判定
	CollideIce();

	// 影の追従
	if (m_pShadow != nullptr)
		m_pShadow->SetPosition(GetPosition());

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 安全な氷を探す
//=====================================================
void CEnemy::SarchSafeIce(void)
{
	if (m_state == E_State::STATE_APPER)
		return;

	CIceManager *pIceMgr = CIceManager::GetInstance();
	if (pIceMgr == nullptr)
		return;

	CIce *pIce = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	D3DXVECTOR3 pos = GetPosition();
	if (pIce == nullptr)
	{
		pIceMgr->GetNearestIce(pos, &m_nGridV, &m_nGridH);
		return;
	}

	if(pIce->IsPeck())
		pIceMgr->GetNearestIce(pos, &m_nGridV, &m_nGridH);
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
	CIce *pIceStand = CIce::GetNearestIce(pos);
	
	if (pIceStand != nullptr)
		pos.y = pIceStand->GetPosition().y;	// 高さを合わせる
	else
		return;

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
	MoveByGrid();		// グリッド基準の移動
}

//=====================================================
// グリッド基準の移動
//=====================================================
void CEnemy::MoveByGrid(void)
{
	// 目標に近い氷を探す
	SarchNearIceToDest();

	// 振り向きの検出
	JudgeTurn();

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
	else
	{// 次の氷が発見できなかったら

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

	if (rIceSave.size() > MAX_DEPTH_FIND)
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

		if (!universal::LimitValue(&nV, nNumV - 1, 0) &&
			!universal::LimitValue(&nH, nNumH - 1, 0))
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
}

//=====================================================
// 振り向きの検出
//=====================================================
void CEnemy::JudgeTurn(void)
{
	// 次に向かう氷の位置の取得
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	CIce *pIceNext = pIceMgr->GetGridIce(&m_nGridVNext, &m_nGridHNext);

	if (pIceNext == nullptr)
		return;

	// 位置取得
	D3DXVECTOR3 posNext = pIceNext->GetPosition();
	D3DXVECTOR3 vecDiff = posNext - GetPosition();

	// 差分角度を作成
	float fAngleDest = atan2f(-vecDiff.x, -vecDiff.z);
	D3DXVECTOR3 rot = GetRotation();

	// 向きの判定
	float fRotDiff = fAngleDest - rot.y;
	universal::LimitRot(&fRotDiff);

	if (LINE_START_TURN * LINE_START_TURN < fRotDiff * fRotDiff)
	{
		// 現在の向きと正反対を目標の向きに設定
		m_fRotTurn = fAngleDest;
		universal::LimitRot(&m_fRotTurn);

		m_bTurn = true;	// しきい値を越えていたら振り返る判定

		// 移動量をリセット
		m_fSpeedMove = 0.0f;
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
}

//=====================================================
// 振り返りの無効化
//=====================================================
bool CEnemy::DisableTurn(void)
{
	if (!m_bTurn)
		return true;

	// 目標の向きに補正する
	D3DXVECTOR3 rot = GetRotation();
	universal::FactingRot(&rot.y, m_fRotTurn, FACT_ROTATION_TURN);
	SetRotation(rot);

	// 差分角度が一定以下になったら振り返りを停止する
	float fRotDiff = m_fRotTurn - rot.y;

	universal::LimitRot(&fRotDiff);

	bool bFinishMotion = IsFinish();

	if (LINE_STOP_TURN * LINE_STOP_TURN > fRotDiff * fRotDiff)
		m_bTurn = false;

	return !m_bTurn;
}

//=====================================================
// 次のグリッドに向かって移動する
//=====================================================
void CEnemy::MoveToNextGrid(void)
{
	if (m_bTurn)	// 振り向き時は移動しない
		return;

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// 現在と次のグリッドの座標を取得
	D3DXVECTOR3 posNext = pIceMgr->GetGridPosition(&m_nGridVNext, &m_nGridHNext);
	D3DXVECTOR3 pos = GetPosition();

	// 差分ベクトルをスピード分に正規化
	D3DXVECTOR3 vecDiff = posNext - pos;

	// 向きを補正する
	float fLengthDiff = D3DXVec3Length(&vecDiff);
	if (fLengthDiff > LINE_FACT_ROT)
	{// 差分ベクトルが一定以上の長さがあるときのみ
		D3DXVECTOR3 rot = GetRotation();
		float fRotDest = atan2f(-vecDiff.x, -vecDiff.z);
		universal::FactingRot(&rot.y, fRotDest, m_fFactRot);

		SetRotation(rot);
	}

	// 移動可能判定
	if (!JudgeCanMove())
		return;

	// つっついた氷だったら停止する
	CIce *pIce = pIceMgr->GetGridIce(&m_nGridVNext, &m_nGridHNext);

	if (pIce != nullptr)
	{
		if (pIce->IsPeck())
		{
			SetState(CEnemy::E_State::STATE_STOP);
			SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			AliveDestGrid();
			return;
		}
	}
	else
	{
		SetState(CEnemy::E_State::STATE_STOP);
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		AliveDestGrid();
		return;
	}

	if (!m_bEnableMove)
		return;

	// 差分ベクトルをスピード分に正規化
	universal::VecConvertLength(&vecDiff, m_fSpeedMove);
	SetMove(vecDiff);
}

//=====================================================
// 移動できるかの判定
//=====================================================
bool CEnemy::JudgeCanMove(void)
{
	// 次に向かう氷の位置の取得
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	CIce *pIceNext = pIceMgr->GetGridIce(&m_nGridVNext, &m_nGridHNext);

	if (pIceNext == nullptr)
		return false;

	// 位置取得
	D3DXVECTOR3 posNext = pIceNext->GetPosition();
	D3DXVECTOR3 vecDiff = posNext - GetPosition();

	// 差分角度を作成
	float fAngleDest = atan2f(-vecDiff.x, -vecDiff.z);

	// 差分角度が一定以下になったら移動できる
	D3DXVECTOR3 rot = GetRotation();
	float fRotDiff = fAngleDest - rot.y;

	universal::LimitRot(&fRotDiff);

	if (LINE_ENABLE_MOVE * LINE_ENABLE_MOVE > fRotDiff * fRotDiff)
	{
		m_bEnableMove = true;	// 目的の向きを向いたので移動可能
		return true;
	}

	// ここまで通ったら移動しない
	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_bEnableMove = false;

	return false;
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
	
	if (!pIceMgr->IsInIce(pos, pIce,0.1f))
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
// 氷との判定
//=====================================================
void CEnemy::CollideIce(void)
{
	if (m_state == E_State::STATE_DRIFT)
		return;

	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	CIce *pIce = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	if (pIce == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();
	pIceMgr->Collide(&pos, pIce, RATE_COLLIDEICE);

	SetPosition(pos);
}

//=====================================================
// 漂流中の動き
//=====================================================
void CEnemy::UpdateDrift(void)
{
	// 漂流中の処理
	StayFlow();

	// 漂流中の死
	DriftDeath();
}

//=====================================================
// 漂流中の死
//=====================================================
void CEnemy::DriftDeath(void)
{
	D3DXVECTOR3 pos = GetPosition();

	if (!universal::IsInScreen(pos, nullptr))
		Death();	// 画面外に出たら死亡時の処理に入る
}

//=====================================================
// 漂流の開始
//=====================================================
void CEnemy::StartFlows(void)
{
	if (FindFlowIce())
	{// 漂流する氷が見つかれば、漂流状態へ移行
		m_state = E_State::STATE_DRIFT;
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_bTurn = false;
	}
}

//=====================================================
// 漂流する氷の検出
//=====================================================
bool CEnemy::FindFlowIce(void)
{
	if (GetState() == E_State::STATE_APPER)
		return false;	// 出現状態なら通らない

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	int nTemp;	// その場に氷が無かったら時のみ漂流
	if (!pIceMgr->GetIdxGridFromPosition(GetPosition(), &nTemp, &nTemp,1.0f))
		return false;

	vector<CFlowIce*> apSystemFlow = CFlowIce::GetInstance();

	for (auto itSystem : apSystemFlow)
	{
		if (itSystem == nullptr)
			continue;

		// 流氷システムが所持する氷の取得
		vector<CIce*> apIce = itSystem->GetIce();

		for (auto itIce : apIce)
		{
			D3DXVECTOR3 posPlayer = GetPosition();
			D3DXVECTOR3 posIce = itIce->GetPosition();

			if (pIceMgr->IsInIce(posPlayer, itIce, RATE_STOP_FLOW_ICE_RADIUS))
			{// どれかに乗っていたら現在のシステムを保存して関数を終了
				m_pLandSystemFlow = itSystem;

				return true;
			}
		}
	}

	return false;
}

//=====================================================
// 漂流中の処理
//=====================================================
void CEnemy::StayFlow(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	if (m_pLandSystemFlow != nullptr)
	{// 流氷システムを検出出来た場合の処理
		if (m_pLandSystemFlow->IsDeath())
		{
			// 漂流の終了
			EndFlows();
		}
	}
	else
	{// 流氷システムが検出できない場合の処理
		JudgeEndFlow();
	}
}

//=====================================================
// 漂流終了の判定
//=====================================================
void CEnemy::JudgeEndFlow(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// グリッドが合ったら止まる
	D3DXVECTOR3 pos = GetPosition();
	pIceMgr->GetIdxGridFromPosition(pos, &m_nGridV, &m_nGridH, RATE_STOP_FLOW_ICE_RADIUS);

	CIce *pIce = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	if (pIce != nullptr)
		EndFlows();	// 漂流の終了
}

//=====================================================
// 漂流の終了
//=====================================================
void CEnemy::EndFlows(void)
{
	m_state = E_State::STATE_STOP;
	m_pLandSystemFlow = nullptr;

	// 止まった先でのグリッド検出
	CIceManager *pIceMgr = CIceManager::GetInstance();
	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	// グリッド内で無かったら即死
	bool bResult = pIceMgr->IsInGrid(pos, 0.6f);

	if (!bResult)
	{
		Death();
		return;
	}

	pIceMgr->GetNearestIce(pos, &m_nGridV, &m_nGridH);

	m_nGridVNext = m_nGridV;
	m_nGridHNext = m_nGridH;

	m_nGridVDest = m_nGridV;
	m_nGridHDest = m_nGridH;
}

//=====================================================
// 死亡時の処理
//=====================================================
void CEnemy::Death(void)
{
	if (CGame::GetState() == CGame::E_State::STATE_NORMAL)
	{
		// コンボ、撃破時のスコア加算
		CUI_Combo* pUICombo = CUI_Combo::GetInstance();

		if (pUICombo != nullptr)
		{
			pUICombo->AddCombo(m_type);
		}
	}

	// 終了処理を呼ぶ
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

	if (pInputKeyboard->GetTrigger(DIK_7))
		Death();

#if 0
	pDebugProc->Print("\n敵情報==========================");
	pDebugProc->Print("\n位置[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	pDebugProc->Print("\n現在グリッド[%d,%d]", m_nGridV, m_nGridH);
	pDebugProc->Print("\n次のグリッド[%d,%d]", m_nGridVNext, m_nGridHNext);
	pDebugProc->Print("\n目標グリッド[%d,%d]", m_nGridVDest, m_nGridHDest);
	pDebugProc->Print("\nモーション[%d]", GetMotion());

	//pDebugProc->Print("\n現在の状態[%d]", m_state);
#endif
}

//=====================================================
// 描画処理
//=====================================================
void CEnemy::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}