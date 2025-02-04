//*****************************************************
//
// GÌ[enemy.cpp]
// Author:ûüRç
//
//*****************************************************

//*****************************************************
// CN[h
//*****************************************************
#include "seals.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "iceManager.h"
#include "player.h"
#include "particle.h"
#include "debugproc.h"
#include "UI_enemy.h"
#include "MyEffekseer.h"
#include "sound.h"

//*****************************************************
// èè`
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionSeals.txt";	// {fBÌpX

const float HEIGHT_APPER = -400.0f;	// o»Ì³
const float WIDTH_APPER = -340.0f;	// o»Ì¡Ì¸ê
const float POW_APPER_JUMP = 45.0f;	// o»ÌWvÍ
const float APPER_GRAVITY = -0.98f;	// o»ÌdÍ
const float FACT_MOVE_APPER = 0.04f;	// o»ÌÚ®W

const float RANGE_FIND_PLAYER = 1000.0f;	// vC[­©ÍÍ

const float SPEED_ONESTEP = 1.7f;	// êàÌXs[h
const float FACT_DECMOVE = 0.9f;	// Ú®¸W

const float RADIUS_HIT = 110.0f;	// qbg»èÌ¼a

const float HEIGHT_ICE = 100.0f;	// XÌ³
}

//=====================================================
// DæÊðßéRXgN^
//=====================================================
CSeals::CSeals(int nPriority) : CEnemy(nPriority), m_pPlayerTarget(nullptr)
{

}

//=====================================================
// fXgN^
//=====================================================
CSeals::~CSeals()
{

}

//=====================================================
// ¶¬
//=====================================================
CSeals* CSeals::Create(void)
{
	CSeals* pSeals = nullptr;

	pSeals = new CSeals;

	if (pSeals != nullptr)
		pSeals->Init();

	return pSeals;
}

//=====================================================
// ú»
//=====================================================
HRESULT CSeals::Init(void)
{
	// Ç
	Load((char*)&PATH_BODY[0]);

	if (FAILED(CEnemy::Init()))
	{ 
		return E_FAIL;
	}

	// Xs[hð0ÉÝè
	SetSpeedMove(0.0f);

	return S_OK;
}

//=====================================================
// o»ÌgXtH[Ýè
//=====================================================
void CSeals::SetApperTransform(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// ObhÔÌÊuðæ¾
	int nIdxV = GetGridV();
	int nIdxH = GetGridH();

	D3DXVECTOR3 posGrid = pIceMgr->GetGridPosition(&nIdxV, &nIdxH);

	// ÅÍObhÊuðRs[
	D3DXVECTOR3 posApper = posGrid;
	m_posApper = posApper;

	// ¶ºÉ¸ç·
	posApper.x += WIDTH_APPER;
	posApper.y += HEIGHT_APPER;

	SetPosition(posApper);

	// o»ÌWvð³¹é
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };
	move.y = POW_APPER_JUMP;
	SetMove(move);

	// ü«ðXÌûÉü¯é
	D3DXVECTOR3 vecDiff = posGrid - posApper;

	float fRot = atan2f(-vecDiff.x, -vecDiff.z);

	D3DXVECTOR3 rot = { 0.0f,fRot,0.0f };

	SetRotation(rot);

	// o»·éêÉµÔ«ð­¶
	MyEffekseer::CreateEffect(CMyEffekseer::TYPE_UPPERENEMY, posApper);


	// [VðúÝè
	SetMotion(E_Motion::MOTION_STARTJUMP);
}

//=====================================================
// I¹
//=====================================================
void CSeals::Uninit(void)
{
	CEnemy::Uninit();
}

//=====================================================
// XV
//=====================================================
void CSeals::Update(void)
{
	// p³NXÌXV
	CEnemy::Update();

	// Ú®ÊÌ¸
	Decreasemove();

	// [VÌÇ
	ManageMotion();
}

//=====================================================
// o»óÔÌXV
//=====================================================
void CSeals::UpdateApper(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	// n[VªI¹µ½çâ~óÔÉÚs
	if (bFinish && nMotion == E_Motion::MOTION_LANDING)
		SetState(CEnemy::E_State::STATE_STOP);

	if (E_Motion::MOTION_LANDING != nMotion)
	{// n[VªíçÈ¢æ¤É·é
		if (CollideLand())
		{
			SetMotion(E_Motion::MOTION_LANDING); // nµ½»èÉüÁ½çn[VÖÚs
			EnableFollowIce(true);	// XÉÇ]·éæ¤É·é
			CSound::GetInstance()->Play(CSound::LABEL_SE_SEAL_CRY);	// Â«º
		}
	}

	// XÉø«ñ¹é
	MoveToIce();

	// dÍð©¯é
	D3DXVECTOR3 move = GetMove();
	move.y += APPER_GRAVITY;
	SetMove(move);
}

//=====================================================
// n»è
//=====================================================
bool CSeals::CollideLand(void)
{
	if (GetMotion() != MOTION_STAYJUMP)
		return false;	// Wv[VÉÈéÜÅüçÈ¢

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	// n·éObhÌXðæ¾
	int nIdxV = GetGridV();
	int nIdxH = GetGridH();

	CIce *pIce = pIceMgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIce == nullptr)
	{
		JudgeRetry();
		return false;
	}

	// Êuæ¾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posIce = pIce->GetPosition();

	if (pos.y < posIce.y + HEIGHT_ICE)
	{// XæèàÊuªá­ÈÁ½çn»è

		pos.y = posIce.y;
		SetPosition(pos);

		// GtFNg­¶
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_LANDING, posIce);

		return true;
	}

	return false;
}

//=====================================================
// Äxo»·é»è
//=====================================================
void CSeals::JudgeRetry(void)
{
	D3DXVECTOR3 pos = GetPosition();

	if (pos.y < 0)
	{
		// o»·éêÉµÔ«ð­¶
		CParticle::Create(m_posApper, CParticle::TYPE::TYPE_DROP);

		E_Spawn spawn = GetSpawn();

		// Äxo»Éüé
		InitGridIdx(spawn);
		SetApperTransform();
	}
}

//=====================================================
// XÉü©ÁÄÚ®·é
//=====================================================
void CSeals::MoveToIce(void)
{
	// n·éObhÌXðæ¾
	int nIdxV = GetGridV();
	int nIdxH = GetGridH();

	// Êuæ¾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posIce = m_posApper;

	D3DXVECTOR3 posMove = pos;
	
	universal::MoveToDest(&posMove, posIce, FACT_MOVE_APPER);

	posMove.y = pos.y;

	SetPosition(posMove);
}

//=====================================================
// Ú®ÊÌ¸
//=====================================================
void CSeals::Decreasemove(void)
{
	float fSpeed = GetSpeedMove();

	fSpeed *= FACT_DECMOVE;

	SetSpeedMove(fSpeed);
}

//=====================================================
// â~óÔÌXV
//=====================================================
void CSeals::UpdateStop(void)
{
	// vC[ÆÌ»è
	CollidePlayer();

	SarchTarget();

	if(m_pPlayerTarget == nullptr)
		DecideNextStrollGrid();

	CEnemy::UpdateStop();

	SetState(E_State::STATE_MOVE);
}

//=====================================================
// ^[QbgÌTõ
//=====================================================
void CSeals::SarchTarget(void)
{
	// vC[CX^XÌæ¾
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	if (apPlayer.empty())
		return;	// zñªóÈçI¹

	float fLengthMin = RANGE_FIND_PLAYER;

	D3DXVECTOR3 pos = GetPosition();

	CPlayer *pPlayer = nullptr;	// ­©µ½vC[

	for (auto it : apPlayer)
	{
		D3DXVECTOR3 posPlayer = it->GetPosition();

		float fDiff = 0.0f;

		if (it->GetState() == CPlayer::E_State::STATE_DEATH)
			continue;

		if (universal::DistCmpFlat(pos, posPlayer, fLengthMin, &fDiff))
		{// Å¬£æèß©Á½çÛ¶
			pPlayer = it;
			fLengthMin = fDiff;
		}
	}

	m_pPlayerTarget = pPlayer;

	if (m_pPlayerTarget != nullptr)	// ^[Qbgª©Â©Á½çÚ®óÔÉÚs
	{
		SetState(CEnemy::E_State::STATE_MOVE);

		SetMotion(E_Motion::MOTION_WALK);
	}
	else
	{

	}
}

//=====================================================
// Ú®óÔÌXV
//=====================================================
void CSeals::UpdateMove(void)
{
	if (m_pPlayerTarget == nullptr)
	{// vC[¢­©Ì
		// ^[QbgÌTõ
		SarchTarget();
	}
	else
	{// vC[­©ÍvC[ðÇ¢©¯é
		// vC[ObhÌ­©
		FindPlayerGrid();
	}

	// p³NXÌXV
	CEnemy::UpdateMove();

	// vC[ÆÌ»è
	CollidePlayer();
}

//=====================================================
// vC[ObhÌ­©
//=====================================================
void CSeals::FindPlayerGrid(void)
{
	if (m_pPlayerTarget == nullptr)
		return;

	int nGridV = m_pPlayerTarget->GetGridV();
	int nGridH = m_pPlayerTarget->GetGridH();

	SetGridVDest(nGridV);
	SetGridHDest(nGridH);
}

//=====================================================
// ÚWObhÉµ½Æ«Ì
//=====================================================
void CSeals::AliveDestGrid(void)
{
	if (m_pPlayerTarget == nullptr)
	{// vC[¢­©ÍÌUàæðT·
		DecideNextStrollGrid();
	}
}

//=====================================================
// ÌUàæðT·
//=====================================================
void CSeals::DecideNextStrollGrid(void)
{
	vector<CIce*> apIce = CIce::GetInstance();

	if (apIce.empty())
		return;

	// ~ÜÁÄÈ¢XðO
	universal::RemoveIfFromVector(apIce, [](CIce* ice) { return ice != nullptr && !ice->IsStop(); });

	// TCY©ç_ÅXðwè
	int nRand = universal::RandRange((int)apIce.size() - 1, 0);

	CIce *pIce = apIce[nRand];

	if (pIce == nullptr)
		return;	// àµIñ¾XªÈ©Á½çðI¹

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// Iñ¾XÌObhÔæ¾
	int nIdxV = 0;
	int nIdxH = 0;
	pIceMgr->GetIceIndex(pIce, &nIdxV, &nIdxH);

	// ÚWÌObhÉÝè
	SetGridVDest(nIdxV);
	SetGridHDest(nIdxH);
}

//=====================================================
// SÌ
//=====================================================
void CSeals::Death(void)
{
	if (IsDeath())
		return;

	// GUIÌGð¸Z
	CUIEnemy *pUIEnemy = CUIEnemy::GetInstance();

	if (pUIEnemy != nullptr)
		pUIEnemy->DeleteEnemy(TYPE::TYPE_SEALS);

	CEnemy::Death();

	CSound::GetInstance()->Play(CSound::LABEL_SE_SEAL_CRY);	// Â«º
}

//=====================================================
// UóÔÌXV
//=====================================================
void CSeals::UpdateAttack(void)
{

}

//=====================================================
// Y¬óÔÌXV
//=====================================================
void CSeals::UpdateDrift(void)
{
	CEnemy::UpdateDrift();

	int nMotion = GetMotion();
	if (nMotion != E_Motion::MOTION_NEUTRAL)
		SetMotion(E_Motion::MOTION_NEUTRAL);
}

//=====================================================
// [VÌÇ
//=====================================================
void CSeals::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (nMotion == E_Motion::MOTION_STARTJUMP)
	{// WvJn[V
		// p[eBNÌ­¶
		D3DXMATRIX mtx = GetParts(3)->pParts->GetMatrix();
		D3DXMATRIX mtxOffset;
		universal::SetOffSet(&mtxOffset, mtx, D3DXVECTOR3(0.0f, 0.0f, 70.0f));
		D3DXVECTOR3 pos = { mtxOffset._41,mtxOffset._42,mtxOffset._43 };

		CParticle::Create(pos, CParticle::TYPE::TYPE_SMALLDROP);

		if (bFinish)	// IíèæØó[VÖÚs
			SetMotion(E_Motion::MOTION_STAYJUMP);
	}

	// Ú®óÔÌ[VÇ
	if (IsTurn())
	{// Uèü«[V
		if (nMotion != E_Motion::MOTION_TURN || bFinish)
			SetMotion(E_Motion::MOTION_TURN);
	}
	else if (!IsEnableMove())
	{// Ú®sÂÌÍÒ@[V
		if (nMotion != E_Motion::MOTION_NEUTRAL)
			SetMotion(E_Motion::MOTION_NEUTRAL);
	}
	else if (GetState() == CEnemy::E_State::STATE_MOVE)
	{
		if (nMotion != E_Motion::MOTION_WALK)
			SetMotion(E_Motion::MOTION_WALK);
	}
}

//=====================================================
// vC[ÆÌ»è
//=====================================================
void CSeals::CollidePlayer(void)
{
	// vC[CX^Xæ¾
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	if (apPlayer.empty())
		return;	// zñªóÈçI¹

	for (auto it : apPlayer)
	{
		if (it == nullptr)
			continue;

		if (it->GetState() == CPlayer::E_State::STATE_DEATH)
			continue;

		if (it->GetState() == CPlayer::E_State::STATE_BLOW)
			continue;

		D3DXVECTOR3 posPlayer = it->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		if (universal::DistCmpFlat(pos, posPlayer,RADIUS_HIT,nullptr))
		{// ©gÌObhÔÆc¡ªêv·éêAèÌqbgðÄÔ
			it->Hit(0.0f);	// ¦ÈÌÅ_[WÍ0
			// â~µÄÚWÌZbg
			SetState(CEnemy::E_State::STATE_STOP);
			m_pPlayerTarget = nullptr;	
		}
	}
}

//=====================================================
// [VCxg
//=====================================================
void CSeals::Event(EVENT_INFO* pEventInfo)
{
	int nMotion = GetMotion();

	if (nMotion == CSeals::E_Motion::MOTION_WALK)
	{// à«ÌACxg­¶ÅXs[hðo·
		float fSpeed = GetSpeedMove();

		fSpeed += SPEED_ONESTEP;

		SetSpeedMove(fSpeed);
	}

	if (nMotion == E_Motion::MOTION_TURN)
	{// ûü]·AµËé^C~OÌÝñ]³¹é
		// Uèü«Ì³ø»
		DisableTurn();
	}
}

//=====================================================
// `æ
//=====================================================
void CSeals::Draw(void)
{
	// p³NXÌ`æ
	CEnemy::Draw();
}