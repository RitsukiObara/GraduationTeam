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
#include "UI_enemy.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
std::vector<CEnemy*> CEnemy::s_vector = {};	// 自身のポインタ

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CEnemy::CEnemy(int nPriority) : m_nGridV(0), m_nGridH(0),m_state(E_State::STATE_NONE)
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
CEnemy* CEnemy::Create(int nType)
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
		pEnemy->Init();

		CUIEnemy *pUIEnemy = CUIEnemy::GetInstance();

		if (pUIEnemy != nullptr)
			pUIEnemy->AddEnemy();
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
	m_state = E_State::STATE_STOP;

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

	pIceMgr->GetRightDownIdx(&m_nGridV, &m_nGridH);
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
	CUIEnemy *pUIEnemy = CUIEnemy::GetInstance();

	if (pUIEnemy != nullptr)
		pUIEnemy->DeleteEnemy();

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

	// 氷に追従
	FollowIce();

	// 状態に応じた更新
	UpdateState updateFuncs[CEnemy::E_State::STATE_MAX] =
	{
		nullptr,
		&CEnemy::UpdateStop,
		&CEnemy::UpdateMove,
		&CEnemy::UpdateAttack,
		&CEnemy::UpdateDrift
	};

	if (updateFuncs[m_state] != nullptr)
		(this->*updateFuncs[m_state])();

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
		pos.y = pIceStand->GetPosition().y;

	SetPosition(pos);
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
}

//=====================================================
// 描画処理
//=====================================================
void CEnemy::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}