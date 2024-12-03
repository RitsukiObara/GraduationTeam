//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "player.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "iceManager.h"
#include "debugproc.h"
#include "particle.h"
#include "camera.h"
#include "manager.h"
#include "flowIce.h"
#include "effect3D.h"
#include "collision.h"
#include "texture.h"
#include "peckLine.h"
#include "sound.h"
#include "shadow.h"
#include "MyEffekseer.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// ボディのパス

const float RATE_DECREASE_MOVE = 0.5f;	// 移動減衰の割合
const float LINE_FACT_ROT = 0.3f;	// 向きを補正するまでの入力しきい値
const float FACT_ROTATION = 0.1f;	// 回転係数

const float LINE_INPUT_MOVE = 0.3f;	// 移動するまでのスティック入力のしきい値
const float RANGE_SELECT_ICE = D3DX_PI * 2 / 6;	// 氷を選択するときの角度の範囲
const float RATE_CHANGE_GRID = 0.56f;	// 次のグリッドに移る判定の割合

const float TIME_MAX_SPEED = 1.0f;	// 最大速度に達するまでにかかる時間
const float SPEED_MOVE_MAX = 2.5f;	// 最大移動速度

const float LINE_STOP_TURN = 0.2f;	// 振り向きを停止するしきい値
const float LINE_START_TURN = D3DX_PI * 0.6f;	// 振り向きを開始するしきい値
const float FACT_ROTATION_TURN = 0.2f;	// 振り向き回転係数

const float RANGE_ROT_FORWARD = D3DX_PI * 2 / CIceManager::E_Direction::DIRECTION_MAX;	// 前進するのに判断する角度

const float DEATH_VIBRATION_POWER = 0.7f;	// 死亡時の振動強さ
const int DEATH_VIBRATION_TIME = 30;			// 死亡時の振動時間

const float PECK_VIBRATION_POWER = 0.5f;	// 割る時の振動強さ
const int PECK_VIBRATION_TIME = 10;			// 割る時の振動時間

const float POW_VIB_BREAK = 0.8f;	// 壊した時の振動強さ
const int TIME_VIB_BREAK = 50;		// 壊した時の振動時間

const float POW_VIB_FLOW = 0.7f;	// 流されてる時の振動強さ
const int TIME_VIB_FLOW = 10;		// 流されてる時の振動時間

//-------------------------------
// ジャンプの定数
//-------------------------------
namespace jump
{
const float RANGE_JUMP = 0.3f;	// ジャンプ先の目標にする角度の範囲
const float FACT_MOVE = 0.07f;	// 移動係数
const float LINE_END = 10.0f;	// 終了と示すしきい値
const float HEIGHT = 100.0f;	// 高さ
}

//-------------------------------
// 方向UIの定数
//-------------------------------
namespace DirUI
{
const string PATH_TEX = "data\\TEXTURE\\UI\\dirPeck.png";	// テクスチャパス
const float WIDTH = 70.0f;		// 幅
const float HEIGHT = 130.0f;	// 高さ
const float FACT_ROT = 0.6f;	// 回転係数
}

const D3DXCOLOR COL_LINE[NUM_PLAYER] =									// 色
{
	D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),
	D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),
	D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),
	D3DXCOLOR(0.0f,1.0f,1.0f,1.0f),
};
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
vector<CPlayer*> CPlayer::s_apPlayer;	// 格納用の配列

//=====================================================
// コンストラクタ
//=====================================================
CPlayer::CPlayer(int nPriority) : m_nGridV(0), m_nGridH(0), m_state(STATE_NONE), m_pIceMoveDest(nullptr), m_bEnableInput(false), m_fTimerStartMove(0.0f),
m_fragMotion(), m_bTurn(false), m_fRotTurn(0.0f), m_pLandSystemFlow(nullptr), m_pLandFlow(nullptr), m_nTimePeck(0), m_nID(0), m_pPeckLine(nullptr),
m_bEnableJump(false), m_pIceDestJump(nullptr), m_posInitJump(), m_pShadow(nullptr)
{
	// デフォルトは入った順の番号
	m_nID = (int)s_apPlayer.size();

	s_apPlayer.push_back(this);
}

//=====================================================
// デストラクタ
//=====================================================
CPlayer::~CPlayer()
{

}

//=====================================================
// 生成処理
//=====================================================
CPlayer* CPlayer::Create(void)
{
	CPlayer *pPlayer = nullptr;

	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->Init();
	}

	return pPlayer;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayer::Init(void)
{
	// 読込
	Load((char*)&PATH_BODY[0]);

	// 継承クラスの初期化
	CMotion::Init();

	InitPose(0);

	// グリッド番号の初期化
	InitGridIdx();
	
	// 方向UI生成
	CreateDirUI();

	// 入力可能フラグを設定
	m_bEnableInput = true;

	// 状態設定
#ifdef _DEBUG
	m_state = STATE_NORMAL;
#else
	m_state = STATE_NORMAL;
#endif // _DEBUG

	// 影の生成
	m_pShadow = CShadow::Create();

	return S_OK;
}

//=====================================================
// グリッド番号の初期化
//=====================================================
void CPlayer::InitGridIdx(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->GetLeftDownIdx(&m_nGridV, &m_nGridH);
	
	D3DXVECTOR3 pos = pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH);
	SetPosition(pos);
}

//=====================================================
// 方向UIの生成
//=====================================================
void CPlayer::CreateDirUI(void)
{
#if 0
	// 生成
	m_pDir = CPolygon3D::Create(GetPosition());
	assert(m_pDir != nullptr);

	// テクスチャ設定
	int nIdxTexture = Texture::GetIdx(&DirUI::PATH_TEX[0]);
	m_pDir->SetIdxTexture(nIdxTexture);

	// サイズ設定
	m_pDir->SetSize(DirUI::WIDTH, DirUI::HEIGHT);
	m_pDir->SetVtx();

	// ライティングの設定
	m_pDir->EnableLighting(false);
	m_pDir->EnableZtest(true);
#else
	m_pPeckLine = CPeckLine::Create(COL_LINE[GetID()], GetPosition());
#endif
}

//=====================================================
// 終了処理
//=====================================================
void CPlayer::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pDir);
	Object::DeleteObject((CObject**)&m_pPeckLine);
	Object::DeleteObject((CObject**)&m_pShadow);

	for (auto itr = s_apPlayer.begin(); itr < s_apPlayer.end(); itr++)
	{
		//削除対象じゃない場合
		if (*itr != this)
			continue;

		//Vectorから削除
		s_apPlayer.erase(itr);

		break;
	}

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayer::Update(void)
{
	if (m_pIceMoveDest != nullptr)
	{// 目標の氷が死んでたらnullにする
		if (m_pIceMoveDest->IsDeath())
			m_pIceMoveDest = nullptr;
	}

	// 入力処理
	Input();

	if (m_state == STATE_FLOW)
		StayFlow();	// 漂流中の処理

	// モーション更新
	CMotion::Update();

	// 方向UIの追従
	FollowDirUI();

	if (m_pPeckLine != nullptr)
		m_pPeckLine->SetPosition(GetPosition());

	// 氷の追従
	FollowIce();

	// モーションの管理
	ManageMotion();

	// 影の追従
	if (m_pShadow != nullptr)
		m_pShadow->SetPosition(GetPosition());

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 氷の追従
//=====================================================
void CPlayer::FollowIce(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	if (pIceStand != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		pos.y = pIceStand->GetPosition().y;
		SetPosition(pos);
	}
}

//=====================================================
// 入力
//=====================================================
void CPlayer::Input(void)
{
	if (m_bEnableInput)
	{
		// アナログ移動
		MoveAnalog();

		// 突っつきの入力
		InputPeck();

		// ジャンプの処理
		InputJump();
	}
}

//=====================================================
// アナログ移動
//=====================================================
void CPlayer::MoveAnalog(void)
{
	// アナログ移動入力
	InputMoveAnalog();

	// 今いるグリッド番号の取得
	CheckGridChange();

	if(m_state != E_State::STATE_INVINCIBLE && m_state != E_State::STATE_FLOW)	// 無敵時は行わない
		CollideIce();	// 氷との判定
}

//=====================================================
// アナログ移動入力
//=====================================================
void CPlayer::InputMoveAnalog(void)
{
	if (m_bTurn)
	{// 振り返ってる判定の入力
		// 振り返りの無効化
		DisableTurn();
	}
	else
	{// 通常の前進状態
		// 前進処理
		Forward();

		// 振り返りの検出
		JudgeTurn();

		// 向きの補正
		FactingRot();
	}

	// 移動量の減衰
	DecreaseMove();
}

//=====================================================
// 振り返りの無効化
//=====================================================
void CPlayer::DisableTurn(void)
{
	// 目標の向きに補正する
	D3DXVECTOR3 rot = GetRotation();
	universal::FactingRot(&rot.y, m_fRotTurn, FACT_ROTATION_TURN);
	SetRotation(rot);

	// 差分角度が一定以下になったら振り返りを停止する
	float fRotDiff = m_fRotTurn - rot.y;

	universal::LimitRot(&fRotDiff);

	if (LINE_STOP_TURN * LINE_STOP_TURN > fRotDiff * fRotDiff)
		m_bTurn = false;
}

//=====================================================
// 前進処理
//=====================================================
void CPlayer::Forward(void)
{
	if (m_pInputMgr == nullptr)
	{
		return;
	}

	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	// 目標方向の設定
	CInputManager::S_Axis axis = m_pInputMgr->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	// 軸を正規化
	float fLengthAxis = D3DXVec3Length(&axisMove);

	D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = GetRotation();

	float fSpeed = SPEED_MOVE_MAX;

	// 突っつきモーション中は行わない
	int nMotion = GetMotion();
	if (nMotion == MOTION_PECK || nMotion == MOTION_CANNOTPECK)
		return;

	if (LINE_INPUT_MOVE < fLengthAxis)
	{// 移動軸操作がしきい値を越えていたら、移動速度の立ち上がりを開始
		m_fTimerStartMove += CManager::GetDeltaTime();

		m_fragMotion.bWalk = true;
	}
	else
	{// 減速
		m_fTimerStartMove = 0.0f;

		m_fragMotion.bWalk = false;
	}

	// 値の補正
	universal::LimitValuefloat(&m_fTimerStartMove, TIME_MAX_SPEED, 0.0f);

	// イージングで補正
	fSpeed *= easing::EaseOutQuart(m_fTimerStartMove / TIME_MAX_SPEED);

	// 移動速度の設定
	D3DXVECTOR3 move = GetMove();

	// 向いている方向にベクトルを伸ばす
	vecMove -= {sinf(rot.y) * fSpeed, 0.0f, cosf(rot.y) * fSpeed};
	move += vecMove;

	SetMove(move);

	// 移動量の反映
	Translate(move);
}

//=====================================================
// 移動量減衰
//=====================================================
void CPlayer::DecreaseMove(void)
{
	D3DXVECTOR3 move = GetMove();

	// 移動量の減衰
	move.x *= RATE_DECREASE_MOVE;
	move.z *= RATE_DECREASE_MOVE;

	SetMove(move);
}

//=====================================================
// 向きの補正
//=====================================================
void CPlayer::FactingRot(void)
{
	if (m_pInputMgr == nullptr)
		return;

	// 突っつきモーション中は行わない
	int nMotion = GetMotion();
	if (nMotion == MOTION_PECK || nMotion == MOTION_CANNOTPECK)
		return;

	// 目標方向の設定
	CInputManager::S_Axis axis = m_pInputMgr->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	// 軸を正規化
	float fLengthAxis = D3DXVec3Length(&axisMove);

	if (fLengthAxis >= LINE_FACT_ROT)
	{// 入力がしきい値を越えていたら補正
		// 目標の向きに補正する
		float fRotDest = atan2f(-axisMove.x, -axisMove.z);

		D3DXVECTOR3 rot = GetRotation();
		universal::FactingRot(&rot.y, fRotDest, FACT_ROTATION);
		SetRotation(rot);
	}
}

//=====================================================
// 振り向きの検出
//=====================================================
void CPlayer::JudgeTurn(void)
{
	if (m_pInputMgr == nullptr)
		return;

	// 突っつきモーション中は行わない
	int nMotion = GetMotion();
	if (nMotion == MOTION_PECK || nMotion == MOTION_CANNOTPECK)
		return;

	// 差分角度を作成
	float fAngleInput = m_pInputMgr->GetAngleMove();
	D3DXVECTOR3 rot = GetRotation();

	// 目標方向の設定
	CInputManager::S_Axis axis = m_pInputMgr->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	// 軸を正規化
	float fLengthAxis = D3DXVec3Length(&axisMove);

	// 一定以上入力していなければ通らない
	if (fLengthAxis <= LINE_FACT_ROT)
		return;

	// 向きの判定
	float fRotDiff = fAngleInput - rot.y;
	universal::LimitRot(&fRotDiff);

	if (LINE_START_TURN * LINE_START_TURN < fRotDiff * fRotDiff)
	{
		// 現在の向きと正反対を目標の向きに設定
		m_fRotTurn = fAngleInput;
		universal::LimitRot(&m_fRotTurn);

		m_bTurn = true;	// しきい値を越えていたら振り返る判定
	}
}

//=====================================================
// 氷との判定
//=====================================================
void CPlayer::CollideIce(void)
{
	if (m_state != E_State::STATE_NORMAL)
		return;

	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	int nIdxV, nIdxH;
	pIceMgr->GetNearestIce(pos, &nIdxV, &nIdxH);

	// グリッドの位置に合わせる
	pIceMgr->Collide(&pos, nIdxV, nIdxH,RATE_CHANGE_GRID);

	SetPosition(pos);
}

//=====================================================
// 前方に氷があるかの判定
//=====================================================
bool CPlayer::CheckAnyIceForward(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	// 周辺の氷の取得
	vector<CIce*> apIce = pIceMgr->GetAroundIce(m_nGridV, m_nGridH);

	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	if (m_pInputMgr == nullptr)
		return false;

	// 目標方向の設定
	CInputManager::S_Axis axis = m_pInputMgr->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	float fRotInput = atan2f(axisMove.x, axisMove.z);
	universal::LimitRot(&fRotInput);

	CDebugProc::GetInstance()->Print("\nスティック角度[%f]", fRotInput);

	for (int i = 0; i < (int)apIce.size(); i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (apIce[i]->IsPeck())
			continue;

		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 rotPlayer = GetRotation();
		D3DXVECTOR3 posIce = apIce[i]->GetPosition();

		rotPlayer.y += D3DX_PI;
		universal::LimitRot(&rotPlayer.y);

		// 扇内にあったらターゲットにする
		if (universal::IsInFanTargetYFlat(posPlayer, posIce, fRotInput, RANGE_ROT_FORWARD))
		{
			return true;
		}
	}

	return false;
}

//=====================================================
// 向きによる氷の選択
//=====================================================
CIce *CPlayer::SelectIceByRot(float fRot)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return nullptr;

	// 周辺の氷の取得
	vector<CIce*> apIce = pIceMgr->GetAroundIce(m_nGridV, m_nGridH);

	for (auto it : apIce)
	{
		if (it == nullptr)
			continue;

		D3DXVECTOR3 posCurrentGrid = pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH);
		D3DXVECTOR3 posIce = it->GetPosition();

		//fRot -= D3DX_PI * 0.5f;
		//universal::LimitRot(&fRot);

		// 氷とスティック角度の比較
		bool bSelect = universal::IsInFanTargetYFlat(posCurrentGrid, posIce, fRot, RANGE_SELECT_ICE);

		if (bSelect)
		{// 氷が選べたらfor文を終了
			m_pIceMoveDest = it;
			break;
		}
	}

	return m_pIceMoveDest;
}

//=====================================================
// グリッドが変わったかどうかの判定
//=====================================================
bool CPlayer::CheckGridChange(void)
{
	if (m_state == STATE_FLOW)
		return false;	// 漂流中は入らない

	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	int nIdxV = -1;
	int nIdxH = -1;

	if (pIceMgr->GetIdxGridFromPosition(GetPosition(), &nIdxV, &nIdxH))
	{
		if (pIceMgr->GetGridIce(&nIdxV, &nIdxH) == nullptr)
		{// 氷が無ければ漂流開始
			// 漂流を開始
			if (!StartFlows())
				Hit(0.0f);	// 開始できなければその場で死亡

			return false;
		}
	}

	// グリッド番号の取得
	D3DXVECTOR3 posNext = GetPosition() + GetMove();
	if (!pIceMgr->GetIdxGridFromPosition(posNext, &nIdxV, &nIdxH, RATE_CHANGE_GRID))
		return false;	// グリッド番号取得失敗で偽を返す

	CIce *pIceForward = pIceMgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIceForward == nullptr)
		return false;

	if (pIceForward->IsPeck())
		return false;

	if ((nIdxV == m_nGridV &&
		nIdxH == m_nGridH) ||
		nIdxV == -1 ||
		nIdxH == -1)
	{// グリッドが変わってない時は偽を返す
		return false;
	}
	else
	{// グリッドが変わってたら値を保存して真を返す
		m_nGridV = nIdxV;
		m_nGridH = nIdxH;

		return true;
	}
}

//=====================================================
// 漂流の開始
//=====================================================
bool CPlayer::StartFlows(void)
{
	if (FindFlowIce())
	{// 漂流する氷が見つかれば、漂流状態へ移行
		m_state = E_State::STATE_FLOW;
		return true;
	}

	return false;
}

//=====================================================
// 漂流する氷の検出
//=====================================================
bool CPlayer::FindFlowIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();
	
	if (pIceMgr == nullptr)
		return false;

	int nTemp;	// その場に氷が無かったら時のみ漂流
	if (!pIceMgr->GetIdxGridFromPosition(GetPosition(), &nTemp, &nTemp, 1.0f))
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

			if (pIceMgr->IsInIce(posPlayer, itIce, 0.7f))
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
void CPlayer::StayFlow(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	if (m_pLandSystemFlow == nullptr)
		return;

	if (m_pLandSystemFlow->IsDeath())
	{
		// 漂流の終了
		EndFlows();
	}

	// 海流のベクトル取得
	COcean::E_Stream dir = pIceMgr->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// 流れる速度に正規化して位置を加算
	float fSpeedFlow = pIceMgr->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow;
	Translate(vecStream);

	// 流氷内に位置を制限
	LimitInSideFlowIce();

	// コントローラーを振動させる
	VibJoypad(POW_VIB_FLOW, TIME_VIB_FLOW);

	// 漂流中の死
	FlowDeath();
}

//=====================================================
// 流氷の内側に制限
//=====================================================
void CPlayer::LimitInSideFlowIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	if (m_pLandSystemFlow == nullptr)
		return;

	// 流氷システムが所持する氷の取得
	vector<CIce*> apIce = m_pLandSystemFlow->GetIce();

	for (auto itIce : apIce)
	{
		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 posIce = itIce->GetPosition();

		if (pIceMgr->IsInIce(posPlayer, itIce,0.7f))
		{// 上に乗ってたら位置を制限
			pIceMgr->Collide(&posPlayer, itIce);
			posPlayer.y = posIce.y;
			SetPosition(posPlayer);
			m_pLandFlow = itIce;
			return;
		}
	}

	if (m_pLandFlow != nullptr)
	{// もしどの氷にも引っかからなければ、前回の氷との判定を行う
		D3DXVECTOR3 posPlayer = GetPosition();
		pIceMgr->Collide(&posPlayer, m_pLandFlow);
		posPlayer.y = m_pLandFlow->GetPosition().y;
		SetPosition(posPlayer);
	}
}

//=====================================================
// 漂流中の死
//=====================================================
void CPlayer::FlowDeath(void)
{
	D3DXVECTOR3 pos = GetPosition();

	if (!universal::IsInScreen(pos))
		Hit(0.0f);
}

//=====================================================
// 漂流の終了
//=====================================================
void CPlayer::EndFlows(void)
{
	m_state = E_State::STATE_NORMAL;
	m_pLandSystemFlow = nullptr;
}

//=====================================================
// 突っつきの入力
//=====================================================
void CPlayer::InputPeck(void)
{
	int nMotion = GetMotion();

	if (nMotion == MOTION_CANNOTPECK || nMotion == MOTION_PECK)
		return;

	if (m_pInputMgr == nullptr)
		return;

	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// 突っつけるかチェック
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 pos = GetPosition();

	rot.y += D3DX_PI;
	universal::LimitRot(&rot.y);

	CIceManager::E_Direction dir;

	CIce *pIcePeck = nullptr;

	if (pIceManager->CheckPeck(m_nGridV, m_nGridH, rot.y, pos, &dir,&pIcePeck))
	{// 突っつけるとき
		// 方向UIの回転
		RotationDirUI(dir);

		if (m_pInputMgr->GetTrigger(CInputManager::BUTTON_PECK))
			SetMotion(MOTION::MOTION_PECK);

		if (m_pPeckLine != nullptr)
		{
			if (pIcePeck != nullptr)
			{
				D3DXVECTOR3 posIce = pIcePeck->GetPosition();

				m_pPeckLine->StartMove(posIce);
			}
		}
	}
	else
	{// 突っつけないとき
		if (m_pInputMgr->GetTrigger(CInputManager::BUTTON_PECK))
			SetMotion(MOTION::MOTION_CANNOTPECK);

		if (m_pPeckLine != nullptr)
			m_pPeckLine->EndMove();
	}

	CDebugProc::GetInstance()->Print("\nつっつく方向[%d]", dir);
}

//=====================================================
// 突っつきの処理
//=====================================================
bool CPlayer::Peck(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	CInputJoypad* pInputJoypad = CInputJoypad::GetInstance();

	if (pInputJoypad == nullptr)
		return false;

	// 突っつく処理
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 pos = GetPosition();

	rot.y += D3DX_PI;
	universal::LimitRot(&rot.y);

	bool bResultBreak;

	if (pIceManager->PeckIce(m_nGridV, m_nGridH, rot.y, pos,&bResultBreak))
		m_nTimePeck++;

	// つっつきのコントローラー振動
	VibJoypad(PECK_VIBRATION_POWER, PECK_VIBRATION_TIME);

	if(bResultBreak)	// 破壊時のコントローラー振動
		VibJoypad(POW_VIB_BREAK, TIME_VIB_BREAK);

	return bResultBreak;
}

//=====================================================
// ジョイパッドを振動させる
//=====================================================
void CPlayer::VibJoypad(float fPow, int nFrame)
{
	CInputJoypad* pInputJoypad = CInputJoypad::GetInstance();

	if (pInputJoypad == nullptr)
		return;

	// joypad振動させる
	pInputJoypad->Vibration(m_nID, fPow, nFrame);
}

//=====================================================
// 方向UIの回転
//=====================================================
void CPlayer::RotationDirUI(int nDir)
{
#if 0
	if (m_pDir == nullptr)
		return;

	D3DXVECTOR3 rot = m_pDir->GetRotation();

	float fRate = D3DX_PI * 2 / CIceManager::E_Direction::DIRECTION_MAX;

	float fRotDest = D3DX_PI * 1.5f;
	fRotDest += fRate * (nDir - 1);

	universal::FactingRot(&rot.y, fRotDest, DirUI::FACT_ROT);

	m_pDir->SetRotation(rot);
#else

#endif
}

//=====================================================
// ジャンプの入力
//=====================================================
void CPlayer::InputJump(void)
{
	if (m_pInputMgr == nullptr)
		return;

	SarchJumpIce();	// ジャンプ先の氷を探す
}

//=====================================================
// ジャンプ先の氷を探す
//=====================================================
void CPlayer::SarchJumpIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	CIce *pIceTarget = nullptr;

	//-------------------------------
	// 向いている方向の氷を取得
	//-------------------------------
	D3DXVECTOR3 rotPlayer = GetRotation();

	rotPlayer.y += D3DX_PI;
	universal::LimitRot(&rotPlayer.y);

	CIce *pIceForward = SelectIceByRot(rotPlayer.y);

	if (pIceForward == nullptr)
		return;

	if (!pIceForward->IsPeck())
		return;

	// 氷の番号取得
	int nIdxV;
	int nIdxH;
	pIceMgr->GetIceIndex(pIceForward, &nIdxV, &nIdxH);

	vector<CIce*> apIce = pIceMgr->GetAroundIce(nIdxV, nIdxH);

	//-------------------------------
	// 向いている氷の周辺をチェック
	//-------------------------------
	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);
	for (int i = 0; i < (int)apIce.size(); i++)
	{
		if (pIceStand == nullptr)
			return;

		if (apIce[i] == nullptr)
			continue;

		if (apIce[i]->IsPeck())
			continue;

		if (pIceStand == apIce[i])
			continue;

		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 posIce = apIce[i]->GetPosition();

		universal::LimitRot(&rotPlayer.y);

		// 扇内にあったらターゲットにする
		if (universal::IsInFanTargetYFlat(pIceStand->GetPosition(), posIce, rotPlayer.y, jump::RANGE_JUMP))
		{
			pIceTarget = apIce[i];
			break;
		}
	}

	//-------------------------------
	// ジャンプの操作
	//-------------------------------
	m_pIceDestJump = pIceTarget;

	if (pIceTarget == nullptr)
		return;	// 何も見つからなかったら処理を通らない

	if (m_pInputMgr->GetTrigger(CInputManager::BUTTON_JUMP))
		StartJump();	// 流れてる氷があればジャンプを開始
}

//=====================================================
// ジャンプの開始
//=====================================================
void CPlayer::StartJump(void)
{
	// ジャンプモーションフラグを立てる
	m_fragMotion.bJump = true;

	// 入力を無効化
	EnableInput(false);

	// ジャンプ初期位置の保存
	m_posInitJump = GetPosition();
}

//=====================================================
// ジャンプ中の処理
//=====================================================
void CPlayer::StayJump(void)
{
	if (m_pIceDestJump == nullptr)
		return;

	//--------------------------------
	// 目標の氷に向かって移動
	//--------------------------------
	D3DXVECTOR3 posPlayer = GetPosition();
	D3DXVECTOR3 posIce = m_pIceDestJump->GetPosition();
	universal::MoveToDest(&posPlayer, posIce, jump::FACT_MOVE);

	SetPosition(posPlayer);

	//--------------------------------
	// 高さの管理
	//--------------------------------
	// 初期位置から目標位置までの距離で現在の割合を計算
	D3DXVECTOR3 vecDiffMax = posIce - m_posInitJump;
	float fLengthMax = sqrtf(vecDiffMax.x * vecDiffMax.x + vecDiffMax.z * vecDiffMax.z);

	D3DXVECTOR3 vecDiff = posPlayer - m_posInitJump;
	float fLength = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

	float fRate = fLength / fLengthMax;

	// 放物線の計算
	float fHeight = universal::ParabolaY(fRate - 0.5f, 10.0f);

	// 放物線の補正
	fHeight *= -jump::HEIGHT;
	fHeight += jump::HEIGHT * 2;

	// 高さを反映
	D3DXVECTOR3 pos = posPlayer;
	pos.y = m_posInitJump.y + fHeight;
	SetPosition(pos);

	//--------------------------------
	// 終了の判定
	//--------------------------------
	if (universal::DistCmpFlat(posPlayer, posIce, jump::LINE_END, nullptr))
	{// 氷に着地したらジャンプを終了
		EndJump();
	}
}

//=====================================================
// ジャンプの終了
//=====================================================
void CPlayer::EndJump(void)
{
	// ジャンプモーションフラグを折る
	m_fragMotion.bJump = false;

	// 入力を有効化
	EnableInput(true);

	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->GetIceIndex(m_pIceDestJump, &m_nGridV, &m_nGridH);
}

//=====================================================
// 方向UIの追従
//=====================================================
void CPlayer::FollowDirUI(void)
{
	if (m_pDir == nullptr)
		return;

	D3DXVECTOR3 posPlayer = GetPosition();

	m_pDir->SetPosition(posPlayer);
}

//=====================================================
// モーションの管理
//=====================================================
void CPlayer::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (nMotion == MOTION::MOTION_MULTIAPPEAR && !bFinish)
	{

	}
	else if (m_fragMotion.bJump)
	{// ジャンプ中
		if (nMotion == MOTION::MOTION_LANDING)
		{// 着地モーション終了でジャンプ終了
			if(bFinish)
				EndJump();
		}
		else if (nMotion == MOTION::MOTION_STAYJUMP)
		{// 滞空モーション中
			// 滞空中の処理
			StayJump();
		}
		else if (nMotion == MOTION::MOTION_STARTJUMP)
		{// ジャンプ開始モーション終了からの遷移
			if (bFinish)
			{
				SetMotion(MOTION::MOTION_STAYJUMP);
				CSound::GetInstance()->Play(CSound::LABEL_SE_PENGUIN_JUMP);
				MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_JUMP, GetPosition());
			}
		}
		else if (nMotion != MOTION::MOTION_STARTJUMP)	// ジャンプ開始モーションの開始
			SetMotion(MOTION::MOTION_STARTJUMP);
	}
	else if ((nMotion == MOTION_PECK || nMotion == MOTION_CANNOTPECK) && !bFinish)
	{

	}
	else if (m_fragMotion.bWalk)
	{// 歩きモーションフラグ有効
		if (nMotion != MOTION::MOTION_WALK)
			SetMotion(MOTION::MOTION_WALK);
	}
	else
	{// 何もフラグが立っていない状態
		if(nMotion != MOTION::MOTION_NEUTRAL)
			SetMotion(MOTION::MOTION_NEUTRAL);
	}
}

//=====================================================
// モーションイベント
//=====================================================
void CPlayer::Event(EVENT_INFO* pEventInfo)
{
	int nMotion = GetMotion();

	if (nMotion == MOTION::MOTION_PECK)
	{
		// 突っつく処理
		Peck();
	}
}

//=====================================================
// ヒット処理
//=====================================================
void CPlayer::Hit(float fDamage)
{
	if (m_state == E_State::STATE_DEATH ||
		m_state == E_State::STATE_INVINCIBLE)
		return;	// 条件によってHit処理を無効化

	// 死亡状態にする
	m_state = E_State::STATE_DEATH;

	// パーティクルの発生
	D3DXVECTOR3 pos = GetPosition();
	CParticle::Create(pos, CParticle::TYPE::TYPE_HITENEMY);
	CParticle::Create(pos, CParticle::TYPE::TYPE_STAR_HIT);

	// 操作可能フラグを折る
	m_bEnableInput = false;

	// joypad振動させる
	VibJoypad(DEATH_VIBRATION_POWER, DEATH_VIBRATION_TIME);

	// ペンギンの鳴き声
	CSound::GetInstance()->Play(CSound::LABEL_SE_PENGUIN_VOICE00);
}

//=====================================================
// デバッグ処理
//=====================================================
void CPlayer::Debug(void)
{
	CDebugProc *pDebugProc = CDebugProc::GetInstance();
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();
	CInputManager *pInputMgr = CInputManager::GetInstance();

	if (pDebugProc == nullptr || pInputKeyboard == nullptr || pJoypad == nullptr || pInputMgr == nullptr || m_pShadow == nullptr)
		return;

#if 0
	pDebugProc->Print("\nプレイヤー情報==========================");
	pDebugProc->Print("\n番号[%d]", m_nID);
	pDebugProc->Print("\n縦[%d]横[%d]", m_nGridV, m_nGridH);
	pDebugProc->Print("\n位置[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	pDebugProc->Print("\n影の位置[%f,%f,%f]", m_pShadow->GetPosition().x, m_pShadow->GetPosition().y, m_pShadow->GetPosition().z);
	pDebugProc->Print("\n移動量[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);
#endif

	if (pInputMgr->GetTrigger(CInputManager::BUTTON_SETICE))
	{
		CIceManager* pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
		{
			pIceManager->CreateIce(m_nGridV, m_nGridH);
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RCONTROL) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_LB, 0))
	{
		CIceManager* pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
		{
			pIceManager->CreateIce(m_nGridV, m_nGridH, CIce::E_Type::TYPE_HARD);
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_LSHIFT))
	{// 流れる氷の設置
		CIceManager* pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
		{
			CFlowIce::Create();
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_F8))
	{// 無敵切り替え
		m_state = (m_state == STATE_INVINCIBLE) ? STATE_NORMAL : STATE_INVINCIBLE;
	}
	if (m_state == STATE_INVINCIBLE)
	{
		pDebugProc->Print("\n<<無敵中(*‘ω‘ *)（F8で通常）>>");
	}
	else
	{
		pDebugProc->Print("\n<<通常(-_-)zzz（F8で無敵）>>");
	}

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	CEffect3D::Create(pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH), 50.0f, 5, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
}

//=====================================================
// 描画処理
//=====================================================
void CPlayer::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}

//=====================================================
// 影の位置設定
//=====================================================
void CPlayer::SetShadowPos(D3DXVECTOR3 pos)
{
	if (m_pShadow != nullptr)
		m_pShadow->SetJustPosition(pos);
}

//=====================================================
// 全プレイヤーの入力フラグ設定
//=====================================================
void CPlayer::EnableInputAll(bool bValue)
{
	for (CPlayer *pPlayer : s_apPlayer)
		pPlayer->EnableInput(bValue);
}