//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
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

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// ボディのパス
const int MOVE_FRAME = 25;	// 移動にかかるフレーム数

const float RATE_DECREASE_MOVE = 0.5f;	// 移動減衰の割合
const float LINE_FACT_ROT = 0.3f;	// 向きを補正するまでの入力しきい値
const float FACT_ROTATION = 0.1f;	// 回転係数
const float DEFAULT_WEIGHT = 5.0f;	// 仮の重さ
const float POSDEST_NEAREQUAL = 0.01f;	// 大体目標位置に着いたとする距離

const float LINE_JUDGE_MOVE = 50.0f;	// 移動に移ったと判断するライン

const float LINE_INPUT_MOVE = 0.3f;	// 移動するまでのスティック入力のしきい値
const float RANGE_SELECT_ICE = D3DX_PI * 2 / 6;	// 氷を選択するときの角度の範囲
const float RATE_CHANGE_GRID = 0.6f;	// 次のグリッドに移る判定の割合

const float TIME_MAX_SPEED = 1.0f;	// 最大速度に達するまでにかかる時間
const float SPEED_MOVE_MAX = 2.5f;	// 最大移動速度

const float LINE_STOP_TURN = 0.2f;	// 振り向きを停止するしきい値
const float LINE_START_TURN = D3DX_PI * 0.6f;	// 振り向きを開始するしきい値
const float FACT_ROTATION_TURN = 0.2f;	// 振り向き回転係数
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
vector<CPlayer*> CPlayer::s_apPlayer;	// 格納用の配列

//=====================================================
// コンストラクタ
//=====================================================
CPlayer::CPlayer(int nPriority) : m_nGridV(0), m_nGridH(0), m_state(STATE_NONE), m_pIceMoveDest(nullptr), m_bEnableInput(false), m_fTimerStartMove(0.0f),
m_fragMotion(), m_bTurn(false), m_fRotTurn(0.0f), m_pLandSystemFlow(nullptr), m_pLandFlow(nullptr)
{
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
	// 入力マネージャー生成
	m_pInputMgr = CInputManager::Create();

	// 読込
	Load((char*)&PATH_BODY[0]);

	// 継承クラスの初期化
	CMotion::Init();

	InitPose(0);

	// グリッド番号の初期化
	InitGridIdx();

	// 入力可能フラグを設定
	m_bEnableInput = true;

	// 状態設定
#ifdef _DEBUG
	m_state = STATE_INVINCIBLE;
#else
	m_state = STATE_NORMAL;
#endif // _DEBUG

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
// 終了処理
//=====================================================
void CPlayer::Uninit(void)
{
	for (auto itr = s_apPlayer.begin(); itr < s_apPlayer.end(); itr++)
	{
		//削除対象じゃない場合
		if (*itr != this)
		{
			continue;
		}

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

	// モーションの管理
	ManageMotion();

	// モーション更新
	CMotion::Update();

#ifdef _DEBUG
	Debug();
#endif
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

	if (false)
		StayJump();	// ジャンプ中の処理
}

//=====================================================
// アナログ移動
//=====================================================
void CPlayer::MoveAnalog(void)
{
	// アナログ移動入力
	InputMoveAnalog();

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

#ifdef _DEBUG
		CDebugProc::GetInstance()->Print("\n振り返ってるよ！！！！！！！！！！！！！！");
#endif
	}
	else
	{// 通常の前進状態
#ifdef _DEBUG
		CDebugProc::GetInstance()->Print("\n振り返ってない");
#endif
		// 前進処理
		Forward();

		// 振り返りの検出
		JudgeTurn();

		// 向きの補正
		FactingRot();
	}

	// 移動量の減衰
	DecreaseMove();

	// 今いるグリッド番号の取得
	CheckGridChange();
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
	AddPosition(move);
}

//=====================================================
// 移動量減衰
//=====================================================
void CPlayer::DecreaseMove(void)
{
	D3DXVECTOR3 move = GetMove();

	// 移動量の減衰
	move *= RATE_DECREASE_MOVE;

	SetMove(move);
}

//=====================================================
// 向きの補正
//=====================================================
void CPlayer::FactingRot(void)
{
	if (m_pInputMgr == nullptr)
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
#if 0
		m_fRotTurn = fAngleInput;
#else 
		m_fRotTurn = rot.y + D3DX_PI;
		universal::LimitRot(&m_fRotTurn);
#endif

		m_bTurn = true;	// しきい値を越えていたら振り返る判定
	}
}

//=====================================================
// 氷との判定
//=====================================================
void CPlayer::CollideIce(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	// グリッドの位置に合わせる
	pIceMgr->Collide(&pos, m_nGridV, m_nGridH);

	// 氷の高さに合わせる
	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);
		
	if (pIceStand != nullptr)
		pos.y = pIceStand->GetPosition().y;

	SetPosition(pos);
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
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	int nIdxV = -1;
	int nIdxH = -1;

	// グリッド番号の取得
	D3DXVECTOR3 pos = GetPosition();
	pIceMgr->GetIdxGridFromPosition(pos, &nIdxV, &nIdxH, RATE_CHANGE_GRID);

	if (m_state != E_State::STATE_INVINCIBLE &&
		pIceMgr->GetGridIce(&nIdxV, &nIdxH) == nullptr)
	{// 無敵状態でない場合、氷がないグリッドの上に行っても番号を変えない
		// 漂流を開始
		StartFlows();

		return false;
	}

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

#ifdef _DEBUG
		CEffect3D::Create(pIceMgr->GetGridPosition(&nIdxV, &nIdxH), 50.0f, 120, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
#endif
		return true;
	}
}

//=====================================================
// 漂流の開始
//=====================================================
void CPlayer::StartFlows(void)
{
	if (FindFlowIce())
	{// 漂流する氷が見つかれば、漂流状態へ移行
		m_state = E_State::STATE_FLOW;
	}
}

//=====================================================
// 漂流する氷の検出
//=====================================================
bool CPlayer::FindFlowIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();
	
	if (pIceMgr == nullptr)
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
	CIceManager::E_Stream dir = pIceMgr->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// 流れる速度に正規化して位置を加算
	float fSpeedFlow = pIceMgr->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow;
	AddPosition(vecStream);

	// 流氷内に位置を制限
	LimitInSideFlowIce();
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
	if (m_pInputMgr == nullptr)
		return;

	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	if (m_pInputMgr->GetTrigger(CInputManager::BUTTON_PECK))
	{
		// 突っつく処理
		D3DXVECTOR3 rot = GetRotation();
		D3DXVECTOR3 pos = GetPosition();

		rot.y += D3DX_PI;
		universal::LimitRot(&rot.y);

		pIceManager->PeckIce(m_nGridV, m_nGridH, rot.y, pos);
	}
}

//=====================================================
// ジャンプの入力
//=====================================================
void CPlayer::InputJump(void)
{
	//if (m_pInputMgr == nullptr)
	return;

	if (m_pInputMgr->GetTrigger(CInputManager::BUTTON_JUMP))
		SarchJumpIce();	// ジャンプ先の氷を探す
}

//=====================================================
// ジャンプ先の氷を探す
//=====================================================
void CPlayer::SarchJumpIce(void)
{
	// プレイヤーの先から扇で判定をとる

	if (true)
		StartJump();	// 流れてる氷があればジャンプを開始
}

//=====================================================
// ジャンプの開始
//=====================================================
void CPlayer::StartJump(void)
{
	// ジャンプモーションフラグを立てる


	// 入力を無効化
	EnableInput(false);
}

//=====================================================
// ジャンプ中の処理
//=====================================================
void CPlayer::StayJump(void)
{
	// 目標の氷に向かって移動


	// 氷に着地したらジャンプを終了
	EndJump();
}

//=====================================================
// ジャンプの終了
//=====================================================
void CPlayer::EndJump(void)
{
	// ジャンプモーションフラグを折る


	// 入力を有効化
	EnableInput(true);
}

//=====================================================
// モーションの管理
//=====================================================
void CPlayer::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinifh = IsFinish();

	if (m_fragMotion.bWalk)
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

	// 操作可能フラグを折る
	m_bEnableInput = false;
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

	if (pDebugProc == nullptr || pInputKeyboard == nullptr || pJoypad == nullptr)
		return;

	pDebugProc->Print("\n縦[%d]横[%d]", m_nGridV, m_nGridH);
	pDebugProc->Print("\n位置[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);

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

	CEffect3D::Create(pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH), 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}

//=====================================================
// 描画処理
//=====================================================
void CPlayer::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}