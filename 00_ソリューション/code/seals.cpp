//*****************************************************
//
// 敵の処理[enemy.cpp]
// Author:早川友貴
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "seals.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "iceManager.h"
#include "player.h"
#include "particle.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionSeals.txt";	// ボディのパス

const float HEIGHT_APPER = -400.0f;	// 出現時の高さ
const float WIDTH_APPER = -340.0f;	// 出現時の横のずれ
const float POW_APPER_JUMP = 45.0f;	// 出現時のジャンプ力
const float APPER_GRAVITY = -0.98f;	// 出現時の重力
const float FACT_MOVE_APPER = 0.04f;	// 出現時の移動係数
}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CSeals::CSeals(int nPriority) : CEnemy(nPriority), m_pPlayerTarget(nullptr)
{

}

//=====================================================
// デストラクタ
//=====================================================
CSeals::~CSeals()
{

}

//=====================================================
// 生成処理
//=====================================================
CSeals* CSeals::Create(void)
{
	CSeals* pSeals = nullptr;

	pSeals = new CSeals;

	if (pSeals != nullptr)
	{
		pSeals->Init();
	}

	return pSeals;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSeals::Init(void)
{
	// 読込
	Load((char*)&PATH_BODY[0]);

	if (FAILED(CEnemy::Init()))
	{ 
		return E_FAIL;
	}

	// モーションを初期設定
	SetMotion(E_Motion::MOTION_STARTJUMP);

	// ポーズ初期化
	InitPose(0);

	return S_OK;
}

//=====================================================
// 出現時のトランスフォーム設定
//=====================================================
void CSeals::SetApperTransform(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// グリッド番号の位置を取得
	int nIdxV = GetGridV();
	int nIdxH = GetGridH();

	D3DXVECTOR3 posGrid = pIceMgr->GetGridPosition(&nIdxV, &nIdxH);

	// 最初はグリッド位置をコピー
	D3DXVECTOR3 posApper = posGrid;

	// 左下にずらす
	posApper.x += WIDTH_APPER;
	posApper.y += HEIGHT_APPER;

	SetPosition(posApper);

	// 出現時のジャンプをさせる
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };
	move.y = POW_APPER_JUMP;
	SetMove(move);

	// 向きを氷の方に向ける
	D3DXVECTOR3 vecDiff = posGrid - posApper;

	float fRot = atan2f(-vecDiff.x, -vecDiff.z);

	D3DXVECTOR3 rot = { 0.0f,fRot,0.0f };

	SetRotation(rot);

	// 出現する場所に水しぶきを発生
	CParticle::Create(posApper, CParticle::TYPE::TYPE_DROP);
}

//=====================================================
// 終了処理
//=====================================================
void CSeals::Uninit(void)
{
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSeals::Update(void)
{
	// 継承クラスの更新
	CEnemy::Update();

	// モーションの管理
	ManageMotion();
}

//=====================================================
// 出現状態の更新
//=====================================================
void CSeals::UpdateApper(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	// 着地モーションが終了したら停止状態に移行
	if (bFinish && nMotion == E_Motion::MOTION_LANDING)
		SetState(CEnemy::E_State::STATE_STOP);

	if (E_Motion::MOTION_LANDING != nMotion)
	{// 着地モーションが被らないようにする
		if (CollideLand())
		{
			SetMotion(E_Motion::MOTION_LANDING); // 着地した判定に入ったら着地モーションへ移行
			EnableFollowIce(true);	// 氷に追従するようにする
		}
	}

	// 氷に引き寄せる
	MoveToIce();

	// 重力をかける
	D3DXVECTOR3 move = GetMove();
	move.y += APPER_GRAVITY;
	SetMove(move);
}

//=====================================================
// 着地判定
//=====================================================
bool CSeals::CollideLand(void)
{
	if (GetMotion() != MOTION_STAYJUMP)
		return false;	// ジャンプ中モーションになるまで入らない

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	// 着地するグリッドの氷を取得
	int nIdxV = GetGridV();
	int nIdxH = GetGridH();

	CIce *pIce = pIceMgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIce == nullptr)
		return false;

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posIce = pIce->GetPosition();

	if (pos.y < posIce.y)
	{// 氷よりも位置が低くなったら着地判定
		pos.y = posIce.y;
		SetPosition(pos);
		return true;
	}

	return false;
}

//=====================================================
// 氷に向かって移動する
//=====================================================
void CSeals::MoveToIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// 着地するグリッドの氷を取得
	int nIdxV = GetGridV();
	int nIdxH = GetGridH();

	CIce *pIce = pIceMgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIce == nullptr)
		return;

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posIce = pIce->GetPosition();

	D3DXVECTOR3 posMove = pos;
	
	universal::MoveToDest(&posMove, posIce, FACT_MOVE_APPER);

	posMove.y = pos.y;

	SetPosition(posMove);
}

//=====================================================
// 停止状態の更新
//=====================================================
void CSeals::UpdateStop(void)
{
	// プレイヤーとの判定
	CollidePlayer();

	// 一番近いプレイヤーをターゲットにする
	SarchTarget();
}

//=====================================================
// ターゲットの探索
//=====================================================
void CSeals::SarchTarget(void)
{
	// プレイヤーインスタンスの取得
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	if (apPlayer.empty())
		return;	// 配列が空なら終了

	float fLengthMin = FLT_MAX;

	D3DXVECTOR3 pos = GetPosition();

	for (auto it : apPlayer)
	{
		D3DXVECTOR3 posPlayer = it->GetPosition();

		float fDiff = 0.0f;

		if (universal::DistCmpFlat(pos, posPlayer, fLengthMin, &fDiff))
		{// 最小距離より近かったら保存
			m_pPlayerTarget = it;
			fLengthMin = fDiff;
		}
	}

	if (m_pPlayerTarget != nullptr)	// ターゲットが見つかったら移動状態に移行
		SetState(CEnemy::E_State::STATE_MOVE);
}

//=====================================================
// 移動状態の更新
//=====================================================
void CSeals::UpdateMove(void)
{
	// 継承クラスの更新
	CEnemy::UpdateMove();

	// プレイヤーとの判定
	CollidePlayer();
}

//=====================================================
// 攻撃状態の更新
//=====================================================
void CSeals::UpdateAttack(void)
{

}

//=====================================================
// 漂流状態の更新
//=====================================================
void CSeals::UpdateDrift(void)
{
	CEnemy::UpdateDrift();
}

//=====================================================
// モーションの管理
//=====================================================
void CSeals::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (nMotion == E_Motion::MOTION_STARTJUMP)
	{// ジャンプ開始モーション
		if (bFinish)	// 終わり次第滞空モーションへ移行
			SetMotion(E_Motion::MOTION_STAYJUMP);
	}
}

//=====================================================
// プレイヤーとの判定
//=====================================================
void CSeals::CollidePlayer(void)
{
	// プレイヤーインスタンス取得
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	if (apPlayer.empty())
		return;	// 配列が空なら終了

	for (auto it : apPlayer)
	{
		if (it == nullptr)
			continue;

		// プレイヤーのグリッド番号取得
		int nIdxVPlayer = it->GetGridV();
		int nIdxHPlayer = it->GetGridH();

		// 自身のグリッド番号
		int nIdxV = GetGridV();
		int nIdxH = GetGridH();

		if (nIdxV == nIdxVPlayer && nIdxH == nIdxHPlayer)
		{// 自身のグリッド番号と縦横が一致する場合、相手のヒット処理を呼ぶ
			it->Hit(0.0f);	// 即死なのでダメージは0
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CSeals::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}