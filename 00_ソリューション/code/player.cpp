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

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// ボディのパス
	const int MOVE_FRAME = 25;	// 移動にかかるフレーム数

	const float SPEED_MOVE_ANALOG = 1.0f;	// アナログ移動での移動距離
	const float RATE_DECREASE_MOVE = 0.5f;	// 移動減衰の割合
	const float LINE_FACT_ROT = 0.3f;	// 向きを補正するまでの入力しきい値
	const float FACT_ROTATION = 0.3f;	// 回転係数
	const float DEFAULT_WEIGHT = 5.0f;	// 仮の重さ
	const float POSDEST_NEAREQUAL = 0.01f;	// 大体目標位置に着いたとする距離
}
#ifdef _DEBUG
//#define DEBUG_WARP	// デバッグ時瞬間移動したい場合定義
#endif

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CPlayer* CPlayer::s_pPlayer = nullptr;	// 自身のポインタ

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CPlayer::CPlayer(int nPriority) : m_nGridV(0), m_nGridH(0), m_bMove(false), m_bAnalog(false)
{
	m_nNextGridV = m_nGridV;
	m_nNextGridH = m_nGridH;
	m_jumpTime = 0.0f;
	m_posDest = { 0.0f,0.0f,0.0f };
	m_move = { 0.0f,0.0f,0.0f };
	m_state = STATE_NORMAL;
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
	if (s_pPlayer == nullptr)
	{
		s_pPlayer = new CPlayer;

		if (s_pPlayer != nullptr)
		{
			s_pPlayer->Init();
		}
	}

	return s_pPlayer;
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

	// 状態設定
#ifdef _DEBUG
	m_state = STATE_INVINCIBLE;
#else
	m_state = STATE_NORMAL;
#endif // _DEBUG


	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CPlayer::Uninit(void)
{
	s_pPlayer = nullptr;

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayer::Update(void)
{
	// 入力処理
	Input();

	// モーション更新
	CMotion::Update();

	// 横軸移動
	MovePositionXZ();

	// 着地確認
	LandCheck();

	// モーション完了後の処理
	MotionFinishCheck();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// 入力
//=====================================================
void CPlayer::Input(void)
{
	if (m_state == STATE_NORMAL || m_state == STATE_INVINCIBLE)
	{
		if (m_bAnalog)
			MoveAnalog();
		else
			MoveGrid();

		// 突っつきの入力
		InputPeck();
	}

#ifdef _DEBUG
	if (CInputJoypad::GetInstance()->GetTrigger(CInputJoypad::PADBUTTONS_UP, 0) ||
		CInputKeyboard::GetInstance()->GetTrigger(DIK_2))
		m_bAnalog = m_bAnalog ? false : true;
#endif
}

//=====================================================
// アナログ移動
//=====================================================
void CPlayer::MoveAnalog(void)
{
	// アナログ移動入力
	InputMoveAnalog();

	// 氷との判定
	CollideIce();
}

//=====================================================
// アナログ移動入力
//=====================================================
void CPlayer::InputMoveAnalog(void)
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
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
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	// 軸を正規化
	float fLengthAxis = D3DXVec3Length(&axisMove);

	D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = GetRotation();

	fLengthAxis *= SPEED_MOVE_ANALOG;

	// 移動速度の設定
	D3DXVECTOR3 move = GetMove();

	// 向いている方向にベクトルを伸ばす
	vecMove -= {sinf(rot.y)* fLengthAxis, 0.0f, cosf(rot.y)* fLengthAxis};
	D3DXVec3Normalize(&vecMove, &vecMove);
	vecMove *= SPEED_MOVE_ANALOG;
	move += vecMove;

	SetMove(move);

	// 移動量の反映
	AddPosition(move);

	// 移動量の減衰
	move *= RATE_DECREASE_MOVE;

	SetMove(move);

	if (fLengthAxis >= LINE_FACT_ROT)
	{// 入力がしきい値を越えていたら補正
		// 目標の向きに補正する
		float fRotDest = atan2f(-axisMove.x, -axisMove.z);

		universal::FactingRot(&rot.y, fRotDest, FACT_ROTATION);
		SetRotation(rot);
	}
}

//=====================================================
// 氷との判定
//=====================================================
void CPlayer::CollideIce(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	pIceManager->Collide(&pos);
	pIceManager->GetIdxGridFromPosition(pos, &m_nGridV, &m_nGridH);

	SetPosition(pos);
}

//=====================================================
// グリッド移動
//=====================================================
void CPlayer::MoveGrid(void)
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// グリッド取得========================================
	D3DXVECTOR3 posGrid = pIceManager->GetGridPosition(&m_nGridV, &m_nGridH);

	// 移動していなければ入力受付========================================
	if (m_bMove == false)
	{
		// 移動の入力========================================
		if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_LEFT))
		{
			m_nNextGridH--;
			m_bMove = true;
		}
		else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_RIGHT))
		{
			m_nNextGridH++;
			m_bMove = true;
		}
		else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_UP))
		{
			m_nNextGridV++;
			m_bMove = true;
		}
		else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_DOWN))
		{
			m_nNextGridV--;
			m_bMove = true;
		}

		if (m_bMove == true && GetMotion() == 0)
		{
			// ジャンプモーション
			SetMotion(MOTION_JUMPSTART);
		}

		// つつきの入力========================================
		if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_PECK))
		{// 乗っている氷を割る
			pIceManager->PeckIce(m_nGridV, m_nGridH, CIceManager::E_Direction::DIRECTION_LEFT);	// 割る処理

			CParticle::Create(D3DXVECTOR3(posGrid.x, posGrid.y - 20.0f, posGrid.z), CParticle::TYPE_ICEBREAK, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			// 氷割りモーション
			SetMotion(MOTION_PECK);
		}
	}

	// 移動処理
	MoveToGrid();
}

//=====================================================
// グリッドまでの移動
//=====================================================
void CPlayer::MoveToGrid(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

#ifdef DEBUG_WARP
	if (m_bMove == true)
	{// デバッグ時瞬間移動
		D3DXVECTOR3 pos = pIceManager->GetGridPosition(&m_nGridV, &m_nGridH);
		SetPosition(pos);
		SetMotion(MOTION_NEUTRAL);
		m_bMove = false;
	}
#else
	if (m_bMove == true && GetMotion() == MOTION_JUMPSTART && IsFinish() == true)
	{// ジャンプモーション終了
		m_nGridV = m_nNextGridV;
		m_nGridH = m_nNextGridH;

		// 新しいグリッドで位置取得・目標位置設定================
		m_posDest = pIceManager->GetGridPosition(&m_nGridV, &m_nGridH);

		// 移動量設定========================================
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 move = (m_posDest - pos) / MOVE_FRAME;
		move.y = 10.0f;
		SetMove(move);

		// ジャンプモーション
		SetMotion(MOTION_JUMPFLY);
	}
#endif
	if (m_bMove == false)
	{
		CIce* pIce = pIceManager->GetGridObject(&m_nGridV, &m_nGridH);
		if (pIce != nullptr)
		{
			D3DXVECTOR3 posObject = pIce->GetPosition();
			SetPosition(posObject);
		}
	}
}

//=====================================================
// 目標位置に移動
//=====================================================
void CPlayer::MovePositionXZ(void)
{
	// XZ移動
	D3DXVECTOR3 pos = GetPosition();
	pos.x += m_move.x;
	pos.z += m_move.z;

	D3DXVECTOR3 vecLength = D3DXVECTOR3(m_posDest.x, 0.0f, m_posDest.z) - D3DXVECTOR3(pos.x, 0.0f, pos.z);
	if (D3DXVec3Length(&vecLength) <= POSDEST_NEAREQUAL)
	{
		// XZ位置移動完了
		pos.x = m_posDest.x;
		pos.z = m_posDest.z;
		m_move = D3DXVECTOR3(0.0f, m_move.y, 0.0f);
	}

	CGameObject::SetPosition(pos);
}

//=====================================================
// 着地確認
//=====================================================
void CPlayer::LandCheck(void)
{
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posDest = m_posDest;

	CIceManager* pIceManager = CIceManager::GetInstance();
	if (pIceManager == nullptr)
		return;

	CIce* pIce = pIceManager->GetGridObject(&m_nGridV, &m_nGridH);
	if (pIce != nullptr)
	{// 着地地点あり
		D3DXVECTOR3 posObject = pIce->GetPosition();
		posDest = posObject;
	}

	m_jumpTime += CManager::GetInstance()->GetDeltaTime();
	pos.y += m_move.y - 9.8f * DEFAULT_WEIGHT * m_jumpTime;
	universal::LimitValuefloat(&pos.y, 1000.0f, posDest.y);

	if (pos.y <= posDest.y)
	{
		// Y位置移動完了
		pos.y = posDest.y;
		m_move.y = 0.0f;
		m_jumpTime = 0.0f;

		// 移動完了後氷に乗っているか判定
		if (pIce != nullptr || m_state == STATE_INVINCIBLE)
		{// 乗っている（普通に着地モーション）
			if (GetMotion() == MOTION_JUMPFLY)
			{
				SetMotion(MOTION_LANDING);
			}
		}
		else
		{// 乗っていないかつ無敵ではない
			m_state = STATE_DEATH;
		}
	}

	SetPosition(pos);
}

//=====================================================
// 突っつきの入力
//=====================================================
void CPlayer::InputPeck(void)
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	if (pInputManager->GetTrigger(CInputManager::BUTTON_PECK))
		pIceManager->PeckIce(m_nGridV, m_nGridH, CIceManager::E_Direction::DIRECTION_LEFT);
}

//=====================================================
// デバッグ処理
//=====================================================
void CPlayer::Debug(void)
{
	CDebugProc* pDebugProc = CDebugProc::GetInstance();
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	if (pDebugProc == nullptr || pInputKeyboard == nullptr || pJoypad == nullptr)
		return;

	pDebugProc->Print("\n縦[%d]横[%d]", m_nGridV, m_nGridH);
	pDebugProc->Print("\n位置[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);

	if (pInputKeyboard->GetTrigger(DIK_RSHIFT) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RB, 0))
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
			CIce *pIce = pIceManager->CreateIce(m_nGridV, m_nGridH, CIce::E_Type::TYPE_NORMAL);
			pIce->ChangeState(new CIceStateFlow);
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
}

//=====================================================
// モーション完了後の処理
//=====================================================
void CPlayer::MotionFinishCheck(void)
{
	if (CMotion::IsFinish() == true)
	{// 何かしらのモーション終了
		if (CMotion::GetMotion() == MOTION_LANDING)
		{// 着地終了（通常状態遷移）
			SetMotion(MOTION_NEUTRAL);
			m_bMove = false;
		}
		if (CMotion::GetMotion() == MOTION_PECK)
		{// 氷割り終了（通常状態遷移）
			SetMotion(MOTION_NEUTRAL);
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CPlayer::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}