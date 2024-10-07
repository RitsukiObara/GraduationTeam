//*****************************************************
//
// GÌ[enemy.cpp]
// Author:ûüRç
//
//*****************************************************

//*****************************************************
// CN[h
//*****************************************************
#include "enemy.h"
#include "game.h"
#include "player.h"
#include "manager.h"
#include "random.h"
#include "inputkeyboard.h"
#include "debugproc.h"

//*****************************************************
// èè`
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionPlayer.txt";	// {fBÌpX
	const D3DXVECTOR3 ROT_INITIAL = { 0.0f, -D3DX_PI * 0.5f, 0.0f };	// úÌü«

	const int ATK_NUM[] =	// ®ÌeUm¦
	{
		5,	// ¶UÌ®m¦
		5,	// EUÌ®m¦
		2,	// ­UÌ®m¦
	};
	const int ATK_START = 200;	// ®ÌUJnl

	const int MOVE_ATK_START = 5;	// UJnlÌÏ®l
	const int MIN_ATK_START = 450;	// UJnlÌÅ¬l
	const int MAX_ATK_START = 880;	// UJnlÌÅål
}

//*****************************************************
// ÃIoÏé¾
//*****************************************************
CEnemy *CEnemy::m_pEnemy = nullptr;	// ©gÌ|C^

//=====================================================
// DæÊðßéRXgN^
//=====================================================
CEnemy::CEnemy(int nPriority)
{
	// ¶¬µ½©gÌAhXðÛ¶
	m_pEnemy = this;

	// oÏðNA
	m_aAtkNum[ATTACK_LEFT] = ATK_NUM[ATTACK_LEFT];			// ¶UÌm¦
	m_aAtkNum[ATTACK_RIGHT] = ATK_NUM[ATTACK_RIGHT];		// EUÌm¦
	m_aAtkNum[ATTACK_BOTHHAND] = ATK_NUM[ATTACK_BOTHHAND];	// ­UÌm¦
	m_nMaxAtkStart = ATK_START;	// UJnlÌÅål
	m_fLastAtkTime = 0.0f;		// OñU©çÌoßÔ
}

//=====================================================
// fXgN^
//=====================================================
CEnemy::~CEnemy()
{

}

//=====================================================
// ¶¬
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
// ú»
//=====================================================
HRESULT CEnemy::Init(void)
{
	// p³NXÌú»
	if (FAILED(CRikishi::Init()))
	{ // ú»É¸sµ½ê

		return E_FAIL;
	}

	// |[YÌú»
	InitPose(MOTION_GUARD);

	// ü«Ìú»
	SetRotation(ROT_INITIAL);

	// ÊuÌXV
	UpdatePosition();

	return S_OK;
}

//=====================================================
// I¹
//=====================================================
void CEnemy::Uninit(void)
{
	// ©gÌÛ|C^ðú»
	m_pEnemy = nullptr;

	// p³NXÌI¹
	CRikishi::Uninit();
}

//=====================================================
// XV
//=====================================================
void CEnemy::Update(void)
{
	// p³NXÌXV
	CRikishi::Update();

	// ÊuÌXV
	UpdatePosition();

	// Q[ªÊíóÔÈOÌê²¯é
	if (CGame::GetState() != CGame::STATE_NORMAL) { return; }

	// UÌXV
	UpdateAttack();

#ifdef _DEBUG
	// fobOÌÝXV
	Debug();
#endif
}

//=====================================================
// `æ
//=====================================================
void CEnemy::Draw(void)
{
	// p³NXÌ`æ
	CRikishi::Draw();
}

//=====================================================
// U
//=====================================================
void CEnemy::Attack(E_Attack typeAttack)
{
	int nMotion = GetMotion();

	if (nMotion != MOTION_GUARD)
		return;

	// eNXÌUðÄÑoµ
	CRikishi::Attack(typeAttack);

	// TODOF±ÌÖªÄÎêÄàUÉ¸s·éÂ\«ª éêÍ±êÈ~ÌÊuðÏX

	// A±Upxðã°é
	m_nMaxAtkStart -= MOVE_ATK_START;
	universal::LimitMinNum(m_nMaxAtkStart, MIN_ATK_START);
}

//=====================================================
// U½Ì
//=====================================================
void CEnemy::HitAttack(E_Attack typeAttack)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	float fKnockBack = pPlayer->Hit(typeAttack, Rikishi::DAMAGE_ATTACK);
}

//=====================================================
// ÊuÌXV
//=====================================================
void CEnemy::UpdatePosition(void)
{
	// Q[V[ÈOÈç²¯é
	CGame *pGame = CGame::GetInstance();
	if (pGame == nullptr) { return; }

	// ¢EÌS©ç©gÌÊuðvZ
	D3DXVECTOR3 posMid = pGame->GetPosMid();
	D3DXVECTOR3 posEnemy = posMid;
	posEnemy.x -= Rikishi::DISTANCE_RIKISHI;

	// Êuð½f
	SetPosition(posEnemy);
}

//=====================================================
// UÌXV
//=====================================================
void CEnemy::UpdateAttack(void)
{
	// oßÔðXV
	m_fLastAtkTime += CManager::GetDeltaTime();

	int nAtk = (int)(m_fLastAtkTime * 10.0f);	// UW
	int nRand = rand() % m_nMaxAtkStart;		// UJnl
	if (nRand <= nAtk)
	{ // UWªUJnlÈºÌê

		// oßÔðú»
		m_fLastAtkTime = 0.0f;

		// UÌ_¶¬
		RandomAttack();
	}

	// »ê¼êÌUm¦
	CDebugProc::GetInstance()->Print("\n¶Um¦F[%d]", m_aAtkNum[ATTACK_LEFT]);
	CDebugProc::GetInstance()->Print("\nEUm¦F[%d]", m_aAtkNum[ATTACK_RIGHT]);
	CDebugProc::GetInstance()->Print("\n­Um¦F[%d]", m_aAtkNum[ATTACK_BOTHHAND]);

	// Um§
	CDebugProc::GetInstance()->Print("\nUWF[%d]", nAtk);
	CDebugProc::GetInstance()->Print("\nUJnlF[%d]", nRand);
	CDebugProc::GetInstance()->Print("\nUJnlÌÅåF[%d]", m_nMaxAtkStart);
}

//=====================================================
// UÌ_¶¬
//=====================================================
void CEnemy::RandomAttack(void)
{
	// _¶¬ASYð¶¬
	CRandom<E_Attack> *pRandom = CRandom<E_Attack>::Create();

	// ¶¬Â\ÈUðÝè
	pRandom->AddList(ATTACK_LEFT, m_aAtkNum[ATTACK_LEFT]);
	pRandom->AddList(ATTACK_RIGHT, m_aAtkNum[ATTACK_RIGHT]);
	pRandom->AddList(ATTACK_BOTHHAND, m_aAtkNum[ATTACK_BOTHHAND]);

	// _ÅIð³ê½Uð·é
	E_Attack select = pRandom->GetRandomNum();
	Attack(select);

	// ¡ñIð³ê½UÌñ¶¬m§ðº°é
	m_aAtkNum[select]--;

	// ¡ñIð³ê½UÈOÌm¦ðú»
	for (int i = 0; i < 3; i++)
	{
		if (i != select)
		m_aAtkNum[i] = ATK_NUM[i];
	}

	// _¶¬ASYðjü
	CRandom<E_Attack>::Release(pRandom);
}

//=====================================================
// qbg
//=====================================================
float CEnemy::Hit(CRikishi::E_Attack typeAttack, float fDamage)
{
	// eNXÌqbgðÄÑoµ
	float fKnockBack = CRikishi::Hit(typeAttack, fDamage);

	// Q[V[ÈOÈç²¯é
	CGame *pGame = CGame::GetInstance();
	if (pGame == nullptr) { return 0.0f; }

	// mbNobN³¹é
	D3DXVECTOR3 addPos = { -fKnockBack,0.0f,0.0f };
	pGame->AddPosMid(addPos);

	// A±Upxðº°é
	m_nMaxAtkStart += MOVE_ATK_START;
	universal::LimitMaxNum(m_nMaxAtkStart, MAX_ATK_START);

	return 0.0f;
}

//=====================================================
// fobO
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