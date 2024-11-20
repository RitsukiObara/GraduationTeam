//*****************************************************
//
// シロクマの処理[bears.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "bears.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "iceManager.h"
#include "player.h"
#include "particle.h"
#include "debugproc.h"
#include "UI_enemy.h"
#include "effect3D.h"
#include "manager.h"
#include "MyEffekseer.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionWhitebear.txt";	// ボディのパス

const float HEIGHT_APPER = -400.0f;		// 出現時の高さ
const float WIDTH_APPER = -340.0f;		// 出現時の横のずれ
const float POW_APPER_JUMP = 45.0f;		// 出現時のジャンプ力
const float APPER_GRAVITY = -0.98f;		// 出現時の重力
const float FACT_MOVE_APPER = 0.04f;	// 出現時の移動係数

const float RANGE_FIND_PLAYER = 1000.0f;	// プレイヤー発見範囲

const float SPEED_ONESTEP = 1.1f;	// 一歩のスピード
const float FACT_DECMOVE = 0.96f;	// 移動減衰係数

const float RADIUS_HIT = 150.0f;	// ヒット判定の半径

//-----------------------------
// 突進の定数
//-----------------------------
namespace charge
{
const float RATE_START = 0.7f;		// 突進を開始するのに氷に近づいてる割合
const float RATE_RANGE = D3DX_PI / CIceManager::E_Direction::DIRECTION_MAX;	// 突撃の角度範囲
const float SPEED_ONESTEP = 1.0f;	// 一歩の速度
}
}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CBears::CBears(int nPriority) : CEnemy(nPriority), m_pPlayerTarget(nullptr), m_vecCharge(), m_fTimerAcceleCharge(0.0f), m_bCharge(false)
{

}

//=====================================================
// デストラクタ
//=====================================================
CBears::~CBears()
{

}

//=====================================================
// 生成処理
//=====================================================
CBears* CBears::Create(void)
{
	CBears* pBears = nullptr;

	pBears = new CBears;

	if (pBears != nullptr)
	{
		pBears->Init();
	}

	return pBears;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBears::Init(void)
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

	// 敵UIの敵数を加算
	CUIEnemy *pUIEnemy = CUIEnemy::GetInstance();

	if (pUIEnemy != nullptr)
		pUIEnemy->AddEnemy();

	// スピードを0に設定
	SetSpeedMove(0.0f);

	return S_OK;
}

//=====================================================
// 出現時のトランスフォーム設定
//=====================================================
void CBears::SetApperTransform(void)
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
void CBears::Uninit(void)
{
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CBears::Update(void)
{
	// 継承クラスの更新
	CEnemy::Update();

	// 移動量の減衰
	Decreasemove();

	// モーションの管理
	ManageMotion();
}

//=====================================================
// 出現状態の更新
//=====================================================
void CBears::UpdateApper(void)
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
bool CBears::CollideLand(void)
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
void CBears::MoveToIce(void)
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
// 移動量の減衰
//=====================================================
void CBears::Decreasemove(void)
{
	float fSpeed = GetSpeedMove();

	fSpeed *= FACT_DECMOVE;

	SetSpeedMove(fSpeed);
}

//=====================================================
// 停止状態の更新
//=====================================================
void CBears::UpdateStop(void)
{
	// プレイヤーとの判定
	CollidePlayer();

	// 散歩先の初期設定
	DecideNextStrollGrid();

	CEnemy::UpdateStop();

	SetState(CEnemy::E_State::STATE_MOVE);
}

//=====================================================
// ターゲットの探索
//=====================================================
void CBears::SarchTarget(void)
{
	//------------------------------
	// 突進するかの判定
	//------------------------------
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// 現在の氷を取得
	int nIdxV = GetGridV();
	int nIdxH = GetGridH();
	CIce *pIce = pIceMgr->GetGridIce(&nIdxV, &nIdxH);
	D3DXVECTOR3 pos = GetPosition();

	if (!pIceMgr->IsInIce(pos, pIce, charge::RATE_START))
		return;	// 一定以上氷に近くなければ、突進しない

	//------------------------------
	// プレイヤーを探す処理
	//------------------------------
	// プレイヤーインスタンスの取得
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	if (apPlayer.empty())
		return;	// 配列が空なら終了

	float fLengthMin = RANGE_FIND_PLAYER;

	CPlayer *pPlayer = nullptr;	// 発見したプレイヤー

	for (auto it : apPlayer)
	{
		D3DXVECTOR3 posPlayer = it->GetPosition();

		float fDiff = 0.0f;

		if (it->GetState() == CPlayer::E_State::STATE_DEATH)
			continue;

		int nIdxPlayerV = it->GetGridV();
		int nIdxPlayerH = it->GetGridH();

		if (CanCharge(posPlayer, nIdxPlayerV, nIdxPlayerH))
		{// 突撃できたらターゲットにして突撃開始
#ifdef _DEBUG
			CEffect3D::Create(GetPosition(), 200.0f, 4, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
#endif
			// 突進ベクトルの保存
			m_vecCharge = posPlayer - pos;
			D3DXVec3Normalize(&m_vecCharge, &m_vecCharge);

			// 振り向きフラグを立てる
			EnableTurn(true);

			// プレイヤーを保存してfor文を終了
			pPlayer = it;
			break;
		}
	}

	//------------------------------
	// 結果を保存
	//------------------------------
	m_pPlayerTarget = pPlayer;

	if (m_pPlayerTarget != nullptr)	// ターゲットが見つかったら移動状態に移行
	{
		SetState(CEnemy::E_State::STATE_MOVE);

		SetMotion(E_Motion::MOTION_WALK);
	}
}

//=====================================================
// グリッド基準じゃない移動を止める
//=====================================================
void CBears::StopMoveByNotGrid(CIce *pIce)
{
	// 突進の終了
	EndCharge();

	// 継承クラスの処理
	CEnemy::StopMoveByNotGrid(pIce);
}

//=====================================================
// 突撃できるかの判定
//=====================================================
bool CBears::CanCharge(D3DXVECTOR3 posTarget, int nIdxTargetV, int nIdxTargetH)
{
	// 差分の角度を取得
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 vecDiff = posTarget - pos;

	float fRot = atan2f(vecDiff.x, vecDiff.z);

	int nIdxV = GetGridV();
	int nIdxH = GetGridH();

	return IsAliveTarget(nIdxV, nIdxH, fRot, nIdxTargetV, nIdxTargetH);
}

//=====================================================
// ターゲットに到達したかの再帰関数
//=====================================================
bool CBears::IsAliveTarget(int nIdxV, int nIdxH, float fRot, int nIdxTargetV, int nIdxTargetH)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	// 周辺の氷の取得
	vector<CIce*> apIce = pIceMgr->GetAroundIce(nIdxV, nIdxH);

	D3DXVECTOR3 posCurrentGrid = pIceMgr->GetGridPosition(&nIdxV, &nIdxH);

	for (int i = 0; i < (int)apIce.size(); i++)
	{
		if (apIce[i] == nullptr)
			continue;	// 氷がなければスキップ

		if (apIce[i]->IsPeck())
			continue;	// つっついた氷ならスキップ

		// 氷の番号取得
		int nIdxIceV;
		int nIdxIceH;
		pIceMgr->GetIceIndex(apIce[i], &nIdxIceV, &nIdxIceH);

		D3DXVECTOR3 posIce = apIce[i]->GetPosition();

		// 角度の設定
		D3DXVECTOR3 posTarget = pIceMgr->GetGridPosition(&nIdxTargetV, &nIdxTargetH);
		D3DXVECTOR3 vecDiff = posTarget - posCurrentGrid;
		fRot = atan2f(vecDiff.x, vecDiff.z);

		// 突進方向に氷があるなら通る
		if (universal::IsInFanTargetYFlat(posCurrentGrid, posIce, fRot, charge::RATE_RANGE))
		{// 進める場合は、進む先の氷に再帰関数を進める
#ifdef _DEBUG
			CEffect3D::Create(posIce, 100.0f, 4, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
#endif
			if (nIdxIceV == nIdxTargetV && nIdxIceH == nIdxTargetH)
				return true;	// 到達したら真を返す

			if (IsAliveTarget(nIdxIceV, nIdxIceH, fRot, nIdxTargetV, nIdxTargetH))
				return true;
		}
	}

	return false;
}

//=====================================================
// 突撃の準備
//=====================================================
void CBears::ReadyCharge(void)
{
	if(DisableTurn())
		StartCharge(); // 一定の向きを向いたら突進開始
}

//=====================================================
// 突撃の開始
//=====================================================
void CBears::StartCharge(void)
{
	// 振り向きフラグを折る
	EnableTurn(false);

	// 突撃フラグを立てる
	m_bCharge = true;

	// プレイヤーグリッドの発見
	FindPlayerGrid();
}

//=====================================================
// 突撃の終了
//=====================================================
void CBears::EndCharge(void)
{
	// 仮で探索状態にする
	SetState(CEnemy::E_State::STATE_STOP);

	// ターゲットのプレイヤーをnullにする
	m_pPlayerTarget = nullptr;

	// 加速タイマーリセット
	m_fTimerAcceleCharge = 0.0f;

	// 次の散歩先を探す
	DecideNextStrollGrid();

	// 突撃フラグを折る
	m_bCharge = false;
}

//=====================================================
// 移動状態の更新
//=====================================================
void CBears::UpdateMove(void)
{
	if (m_pPlayerTarget == nullptr)
	{// プレイヤー未発見時の処理
		// ターゲットの探索
		SarchTarget();
	}
	else
	{// プレイヤー発見時はプレイヤーを追いかける
		// 突進処理
		Charge();
	}

	// プレイヤーとの判定
	CollidePlayer();

	// 継承クラスの更新
	CEnemy::UpdateMove();

	if(!IsTurn() || m_pPlayerTarget != nullptr)
		MoveToNextGrid(); // 次のグリッドに進む
}

//=====================================================
// 突進中の処理
//=====================================================
void CBears::Charge(void)
{
	DisableTurn();
}

//=====================================================
// プレイヤーグリッドの発見
//=====================================================
void CBears::FindPlayerGrid(void)
{
	if (m_pPlayerTarget == nullptr)
		return;
	
	int nGridV = m_pPlayerTarget->GetGridV();
	int nGridH = m_pPlayerTarget->GetGridH();

	SetGridVDest(nGridV);
	SetGridHDest(nGridH);
}

//=====================================================
// 目標グリッドに到着したときの処理
//=====================================================
void CBears::AliveDestGrid(void)
{
	if (m_pPlayerTarget == nullptr)
	{// プレイヤー未発見時は次の散歩先を探す
		DecideNextStrollGrid();
	}
	else
	{// 突進していた時はオーバーヒート状態にする
		EndCharge();
	}
}

//=====================================================
// 次の散歩先を探す処理
//=====================================================
void CBears::DecideNextStrollGrid(void)
{
	vector<CIce*> apIce = CIce::GetInstance();

	if (apIce.empty())
		return;

	// 止まってない氷を除外
	universal::RemoveIfFromVector(apIce, [](CIce* ice) { return ice != nullptr && !ice->IsStop(); });

	// サイズからランダムで氷を指定
	int nRand = universal::RandRange((int)apIce.size() - 1, 0);

	CIce *pIce = apIce[nRand];

#ifdef _DEBUG
	CEffect3D::Create(apIce[nRand]->GetPosition(), 200.0f, 120, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif

	if (pIce == nullptr)
		return;	// もし選んだ氷がなかったら処理を終了

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// 選んだ氷のグリッド番号取得
	int nIdxV = 0;
	int nIdxH = 0;
	pIceMgr->GetIceIndex(pIce, &nIdxV, &nIdxH);

	// 目標のグリッドに設定
	SetGridVDest(nIdxV);
	SetGridHDest(nIdxH);
}

//=====================================================
// 死亡時の処理
//=====================================================
void CBears::Death(void)
{
	// 敵UIの敵数を減算
	CUIEnemy *pUIEnemy = CUIEnemy::GetInstance();

	if (pUIEnemy != nullptr)
		pUIEnemy->DeleteEnemy();

	CEnemy::Death();
}

//=====================================================
// 攻撃状態の更新
//=====================================================
void CBears::UpdateAttack(void)
{

}

//=====================================================
// 漂流状態の更新
//=====================================================
void CBears::UpdateDrift(void)
{
	CEnemy::UpdateDrift();
}

//=====================================================
// モーションの管理
//=====================================================
void CBears::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	//---------------------------------
	// 出現状態のモーション
	//---------------------------------
	if (nMotion == E_Motion::MOTION_STARTJUMP)
	{// ジャンプ開始モーション
		if (bFinish)	// 終わり次第滞空モーションへ移行
			SetMotion(E_Motion::MOTION_STAYJUMP);
	}

	//---------------------------------
	// 移動状態のモーション
	//---------------------------------
	if (m_pPlayerTarget != nullptr)
	{
		if (m_bCharge)
		{// 突進中
			if (nMotion != E_Motion::MOTION_CHARGE || bFinish)
				SetMotion(E_Motion::MOTION_CHARGE);
		}
		else
		{// 振り向き中の時
			if (nMotion != E_Motion::MOTION_TURNCHARGE || bFinish)
				SetMotion(E_Motion::MOTION_TURNCHARGE);
		}
	}
	else if (IsTurn())
	{// 振り向きモーション
		if (m_pPlayerTarget == nullptr)
		{// 通常振り向き
			if (nMotion != E_Motion::MOTION_TURN || bFinish)
				SetMotion(E_Motion::MOTION_TURN);
		}
		else
		{// 突進振り向き
			if (nMotion != E_Motion::MOTION_TURN || bFinish)
				SetMotion(E_Motion::MOTION_TURN);
		}
	}
	else if (!IsEnableMove())
	{// 移動不可の時は待機モーション
		if (nMotion != E_Motion::MOTION_TURN)
			SetMotion(E_Motion::MOTION_TURN);
	}
	else if (GetState() == CEnemy::E_State::STATE_MOVE)
	{
		if (nMotion != E_Motion::MOTION_WALK)
			SetMotion(E_Motion::MOTION_WALK);
	}
}

//=====================================================
// プレイヤーとの判定
//=====================================================
void CBears::CollidePlayer(void)
{
	// プレイヤーインスタンス取得
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	if (apPlayer.empty())
		return;	// 配列が空なら終了

	for (auto it : apPlayer)
	{
		if (it == nullptr)
			continue;

		if (it->GetState() == CPlayer::E_State::STATE_DEATH)
			continue;

		D3DXVECTOR3 posPlayer = it->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		if (universal::DistCmpFlat(pos, posPlayer,RADIUS_HIT,nullptr))
		{// 自身のグリッド番号と縦横が一致する場合、相手のヒット処理を呼ぶ
			// 即死なのでダメージは0
			it->Hit(0.0f);

			// 停止して目標のリセット
			SetState(CEnemy::E_State::STATE_STOP);
			m_pPlayerTarget = nullptr;

			// 突進の終了
			EndCharge();
		}
	}
}

//=====================================================
// モーションイベント
//=====================================================
void CBears::Event(EVENT_INFO* pEventInfo)
{
	int nMotion = GetMotion();

	if (nMotion == CBears::E_Motion::MOTION_WALK)
	{// 歩きの時、イベント発生でスピードを出す
		float fSpeed = GetSpeedMove();

		fSpeed += SPEED_ONESTEP;

		SetSpeedMove(fSpeed);
	}

	if (nMotion == E_Motion::MOTION_TURN)
	{// 方向転換時、跳ねるタイミングのみ回転させる
		// 振り向きの無効化
		DisableTurn();
	}

	if (nMotion == E_Motion::MOTION_TURNCHARGE)
	{// 突進前の回転
		ReadyCharge();
	}

	if (nMotion == E_Motion::MOTION_CHARGE)
	{// 突進中はイベント発生でスピード発生
		float fSpeed = GetSpeedMove();

		fSpeed += charge::SPEED_ONESTEP;

		SetSpeedMove(fSpeed);

		// エフェクトの発生
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_BEARSTEP, GetPosition());
	}
}

//=====================================================
// 描画処理
//=====================================================
void CBears::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}