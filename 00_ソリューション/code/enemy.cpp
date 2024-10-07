//*****************************************************
//
// 敵の処理[enemy.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "enemy.h"
#include "game.h"
#include "player.h"
#include "manager.h"
#include "random.h"
#include "inputkeyboard.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionPlayer.txt";	// ボディのパス
	const D3DXVECTOR3 ROT_INITIAL = { 0.0f, -D3DX_PI * 0.5f, 0.0f };	// 初期の向き

	const int ATK_NUM[] =	// 初動の各攻撃確率
	{
		5,	// 左攻撃の初動確率
		5,	// 右攻撃の初動確率
		2,	// 強攻撃の初動確率
	};
	const int ATK_START = 200;	// 初動の攻撃開始値

	const int MOVE_ATK_START = 5;	// 攻撃開始値の変動値
	const int MIN_ATK_START = 450;	// 攻撃開始値の最小値
	const int MAX_ATK_START = 880;	// 攻撃開始値の最大値
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CEnemy *CEnemy::m_pEnemy = nullptr;	// 自身のポインタ

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CEnemy::CEnemy(int nPriority)
{
	// 生成した自身のアドレスを保存
	m_pEnemy = this;

	// メンバ変数をクリア
	m_aAtkNum[ATTACK_LEFT] = ATK_NUM[ATTACK_LEFT];			// 左攻撃の確率
	m_aAtkNum[ATTACK_RIGHT] = ATK_NUM[ATTACK_RIGHT];		// 右攻撃の確率
	m_aAtkNum[ATTACK_BOTHHAND] = ATK_NUM[ATTACK_BOTHHAND];	// 強攻撃の確率
	m_nMaxAtkStart = ATK_START;	// 攻撃開始値の最大値
	m_fLastAtkTime = 0.0f;		// 前回攻撃からの経過時間
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
CEnemy *CEnemy::Create(void)
{
	if (m_pEnemy == nullptr)
	{
		m_pEnemy = new CEnemy;
		if (m_pEnemy != nullptr)
		{
			m_pEnemy->Init();
		}
	}

	return m_pEnemy;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemy::Init(void)
{
	// 継承クラスの初期化
	if (FAILED(CRikishi::Init()))
	{ // 初期化に失敗した場合

		return E_FAIL;
	}

	// ポーズの初期化
	InitPose(MOTION_GUARD);

	// 向きの初期化
	SetRotation(ROT_INITIAL);

	// 位置の更新
	UpdatePosition();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemy::Uninit(void)
{
	// 自身の保持ポインタを初期化
	m_pEnemy = nullptr;

	// 継承クラスの終了
	CRikishi::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemy::Update(void)
{
	// 継承クラスの更新
	CRikishi::Update();

	// 位置の更新
	UpdatePosition();

	// ゲームが通常状態以外の場合抜ける
	if (CGame::GetState() != CGame::STATE_NORMAL) { return; }

	// 攻撃の更新
	UpdateAttack();

#ifdef _DEBUG
	// デバッグのみ更新
	Debug();
#endif
}

//=====================================================
// 描画処理
//=====================================================
void CEnemy::Draw(void)
{
	// 継承クラスの描画
	CRikishi::Draw();
}

//=====================================================
// 攻撃
//=====================================================
void CEnemy::Attack(E_Attack typeAttack)
{
	int nMotion = GetMotion();

	if (nMotion != MOTION_GUARD)
		return;

	// 親クラスの攻撃を呼び出し
	CRikishi::Attack(typeAttack);

	// TODO：この関数が呼ばれても攻撃に失敗する可能性がある場合はこれ以降の処理位置を変更

	// 連続攻撃頻度を上げる
	m_nMaxAtkStart -= MOVE_ATK_START;
	universal::LimitMinNum(m_nMaxAtkStart, MIN_ATK_START);
}

//=====================================================
// 攻撃命中時の処理
//=====================================================
void CEnemy::HitAttack(E_Attack typeAttack)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	float fKnockBack = pPlayer->Hit(typeAttack, Rikishi::DAMAGE_ATTACK);
}

//=====================================================
// 位置の更新処理
//=====================================================
void CEnemy::UpdatePosition(void)
{
	// ゲームシーン以外なら抜ける
	CGame *pGame = CGame::GetInstance();
	if (pGame == nullptr) { return; }

	// 世界の中心から自身の位置を計算
	D3DXVECTOR3 posMid = pGame->GetPosMid();
	D3DXVECTOR3 posEnemy = posMid;
	posEnemy.x -= Rikishi::DISTANCE_RIKISHI;

	// 位置を反映
	SetPosition(posEnemy);
}

//=====================================================
// 攻撃の更新処理
//=====================================================
void CEnemy::UpdateAttack(void)
{
	// 経過時間を更新
	m_fLastAtkTime += CManager::GetDeltaTime();

	int nAtk = (int)(m_fLastAtkTime * 10.0f);	// 攻撃係数
	int nRand = rand() % m_nMaxAtkStart;		// 攻撃開始値
	if (nRand <= nAtk)
	{ // 攻撃係数が攻撃開始値以下の場合

		// 経過時間を初期化
		m_fLastAtkTime = 0.0f;

		// 攻撃のランダム生成
		RandomAttack();
	}

	// それぞれの攻撃確率
	CDebugProc::GetInstance()->Print("\n左攻撃確率：[%d]", m_aAtkNum[ATTACK_LEFT]);
	CDebugProc::GetInstance()->Print("\n右攻撃確率：[%d]", m_aAtkNum[ATTACK_RIGHT]);
	CDebugProc::GetInstance()->Print("\n強攻撃確率：[%d]", m_aAtkNum[ATTACK_BOTHHAND]);

	// 攻撃確立
	CDebugProc::GetInstance()->Print("\n攻撃係数：[%d]", nAtk);
	CDebugProc::GetInstance()->Print("\n攻撃開始値：[%d]", nRand);
	CDebugProc::GetInstance()->Print("\n攻撃開始値の最大：[%d]", m_nMaxAtkStart);
}

//=====================================================
// 攻撃のランダム生成処理
//=====================================================
void CEnemy::RandomAttack(void)
{
	// ランダム生成アルゴリズムを生成
	CRandom<E_Attack> *pRandom = CRandom<E_Attack>::Create();

	// 生成可能な攻撃を設定
	pRandom->AddList(ATTACK_LEFT, m_aAtkNum[ATTACK_LEFT]);
	pRandom->AddList(ATTACK_RIGHT, m_aAtkNum[ATTACK_RIGHT]);
	pRandom->AddList(ATTACK_BOTHHAND, m_aAtkNum[ATTACK_BOTHHAND]);

	// ランダムで選択された攻撃をする
	E_Attack select = pRandom->GetRandomNum();
	Attack(select);

	// 今回選択された攻撃の次回生成確立を下げる
	m_aAtkNum[select]--;

	// 今回選択された攻撃以外の確率を初期化
	for (int i = 0; i < 3; i++)
	{
		if (i != select)
		m_aAtkNum[i] = ATK_NUM[i];
	}

	// ランダム生成アルゴリズムを破棄
	CRandom<E_Attack>::Release(pRandom);
}

//=====================================================
// ヒット処理
//=====================================================
float CEnemy::Hit(CRikishi::E_Attack typeAttack, float fDamage)
{
	// 親クラスのヒットを呼び出し
	float fKnockBack = CRikishi::Hit(typeAttack, fDamage);

	// ゲームシーン以外なら抜ける
	CGame *pGame = CGame::GetInstance();
	if (pGame == nullptr) { return 0.0f; }

	// ノックバックさせる
	D3DXVECTOR3 addPos = { -fKnockBack,0.0f,0.0f };
	pGame->AddPosMid(addPos);

	// 連続攻撃頻度を下げる
	m_nMaxAtkStart += MOVE_ATK_START;
	universal::LimitMaxNum(m_nMaxAtkStart, MAX_ATK_START);

	return 0.0f;
}

//=====================================================
// デバッグ処理
//=====================================================
void CEnemy::Debug(void)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr)
		return;

	if (pKeyboard->GetTrigger(DIK_Z))
		Attack(CRikishi::E_Attack::ATTACK_LEFT);
	if (pKeyboard->GetTrigger(DIK_C))
		Attack(CRikishi::E_Attack::ATTACK_RIGHT);
	if (pKeyboard->GetTrigger(DIK_X))
		Attack(CRikishi::E_Attack::ATTACK_BOTHHAND);
}