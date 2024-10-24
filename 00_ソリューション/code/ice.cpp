//*****************************************************
//
// XÌ[ice.cpp]
// Author:ûüRç
//
//*****************************************************

//*****************************************************
// CN[h
//*****************************************************
#include "ice.h"
#include "texture.h"
#include "gameObject.h"
#include "fan3D.h"
#include "meshcylinder.h"
#include "ocean.h"
#include "iceHard.h"
#include "objectX.h"
#include "iceManager.h"
#include "manager.h"
#include "particle.h"
#include "model.h"
#include "effect3D.h"

//*****************************************************
// èè`
//*****************************************************
namespace
{
const string PATH_TEX = "data\\TEXTURE\\UI\\ice.png";	// eNX`pX
const float SIZE_INIT = 100.0f;	// úTCY
const float HEIGHT_ICE = 50.0f;	// XÌ³
const int NUM_CORNER = 6;	// pÌ
const D3DXVECTOR3 ROT_UP_INIT = { D3DX_PI * 0.5f,0.0f,0.0f };	// ã¤Ìúü«

const float TIME_REPAIR_ICE = 10.0f;	// XÌCÉ©©éÔ
const int MAX_SCALE = 50; // XP[ÌÅål
const int MIN_SCALE = 20; // XP[ÌÅ¬l
const string PATH_ICE_DEBRIS = "data\\MODEL\\block\\Drift_ice_piece.x";	// jÐXÌfpX
const float SPEED_SINK = 5.0f;	// ¾Þ¬x
const float HEIGHT_DELETE = -100.0f;	// í·éÜÅÌ³

const float LINE_STOP_ICE = 1.0f;	// Xª~Üéµ«¢l
}

//*****************************************************
// ÃIoÏé¾
//*****************************************************
int CIce::s_nNumAll = 0;
std::vector<CIce*> CIce::m_Vector = {};	// ©gÌ|C^

//=====================================================
// RXgN^
//=====================================================
CIce::CIce(int nPriority) : CGameObject(nPriority), m_state(E_State::STATE_NONE), m_bBreak(false), m_bCanFind(false), m_bPeck(false), m_bAliveStandBlock(false),
m_pSide(nullptr),m_pUp(nullptr), m_pState(nullptr), m_bSink(false), m_bStop(nullptr)
{
	s_nNumAll++;
	m_Vector.push_back(this);
}

//=====================================================
// fXgN^
//=====================================================
CIce::~CIce()
{
	s_nNumAll--;
}

//=====================================================
// ¶¬
//=====================================================
CIce *CIce::Create(E_Type type,E_State state)
{
	CIce *pIce = nullptr;

	if (pIce == nullptr)
	{
		switch (type)
		{
		case CIce::TYPE_NORMAL:
			pIce = new CIce;
			break;
		case CIce::TYPE_HARD:
			pIce = new CIceHard;
			break;
		default:
			assert(false);
			break;
		}

		if (pIce != nullptr)
		{
			pIce->m_state = state;
			pIce->Init();
		}
	}

	return pIce;
}

//=====================================================
// ú»
//=====================================================
HRESULT CIce::Init(void)
{
	EnableCanFind(true);

	// bVÌ¶¬
	CreateMesh();

	// gXtH[ÌúÝè
	SetTransform(SIZE_INIT);

	ChangeState(new CIceStaeteNormal);

	return S_OK;
}

//=====================================================
// bVÌ¶¬
//=====================================================
void CIce::CreateMesh(void)
{
	if (m_pUp == nullptr)
	{
		m_pUp = CFan3D::Create(3, NUM_CORNER);

		if (m_pUp != nullptr)
		{
			m_pUp->SetRotation(ROT_UP_INIT);
			m_pUp->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	if (m_pSide == nullptr)
		m_pSide = CMeshCylinder::Create(NUM_CORNER);

	SetTransform(Grid::SIZE);
}

//=====================================================
// bVÌjü
//=====================================================
void CIce::DeleteMesh(void)
{
	Object::DeleteObject((CObject**)&m_pUp);
	Object::DeleteObject((CObject**)&m_pSide);
}

//=====================================================
// I¹
//=====================================================
void CIce::Uninit(void)
{
	// bVÌí
	DeleteMesh();

	for (auto itr = m_Vector.begin(); itr < m_Vector.end(); itr++)
	{
		//íÎÛ¶áÈ¢ê
		if (*itr != this)
		{
			continue;
		}
		//Vector©çí
		m_Vector.erase(itr);

		break;
	}

	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	CGameObject::Uninit();
}

//=====================================================
// XV
//=====================================================
void CIce::Update(void)
{	
	if (!IsSink())	// ¾ÞtOª½ÁÄ¢È¢Æ«ÌÝs¤
		FollowWave();	// gÉÇ]·é

	if (m_pState != nullptr)
		m_pState->Update(this);
}

//=====================================================
// gÉÇ]·é
//=====================================================
void CIce::FollowWave(void)
{
	COcean *pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
	{
		return;
	}

	// CÆêÉXð®©·
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos.y = pOcean->GetHeight(pos, &move) + HEIGHT_ICE;

	if (m_pUp != nullptr)
	{
		m_pUp->SetPosition(pos);
	}
	if (m_pSide != nullptr)
	{
		m_pSide->SetPosition(pos + D3DXVECTOR3(0.0f, -HEIGHT_ICE, 0.0f));
	}

	SetPosition(pos);
}

//=====================================================
// `æ
//=====================================================
void CIce::Draw(void)
{

}

//=====================================================
// gXtH[Ýè
//=====================================================
void CIce::SetTransform(float fRadius)
{
	if (m_pUp == nullptr || m_pSide == nullptr)
		return;

	D3DXVECTOR3 posIce = GetPosition();

	// ãÌî|SÌÝè
	m_pUp->SetRadius(fRadius * 0.5f);
	m_pUp->SetPosition(posIce);
	m_pUp->SetVtx();

	// TChÌV_[ÌÝè
	m_pSide->SetRadius(fRadius);
	m_pSide->SetHeight(HEIGHT_ICE);
	D3DXVECTOR3 posSide = posIce;
	posSide.y -= HEIGHT_ICE;
	m_pSide->SetPosition(posSide);
}

//=====================================================
// XeCgÌÏX
//=====================================================
void CIce::ChangeState(CIceState *pState)
{
	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	m_pState = pState;

	if (m_pState != nullptr)
		m_pState->Init(this);
}

//*******************************************************************************
// ÊíXeCg
//*******************************************************************************
//=====================================================
// ú»
//=====================================================
void CIceStaeteNormal::Init(CIce *pIce)
{
	pIce->EnableStop(true);

	// YµÄ¢éÔðæ¾
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->GetIceIndex(pIce, &m_nIdxDriftV, &m_nIdxDriftH);
}

//=====================================================
// I¹
//=====================================================
void CIceStaeteNormal::Uninit(CIce *pIce)
{

}

//=====================================================
// XV
//=====================================================
void CIceStaeteNormal::Update(CIce *pIce)
{
	// Ôæ¾É¸sµÄ¢é©ÌmF
	bool bMove = CheckFailGetIndex(pIce);

	if(bMove)
		MoveToGrid(pIce);	// ObhÉü©ÁÄÚ®·é
}

//=====================================================
// Ôæ¾É¸sµÄ¢é©ÌmF
//=====================================================
bool CIceStaeteNormal::CheckFailGetIndex(CIce *pIce)
{
	if (m_nIdxDriftV == -1 ||
		m_nIdxDriftH == -1)
	{// Ôæ¾É¸sµÄ¢éêAÄæ¾
		CIceManager *pIceMgr = CIceManager::GetInstance();

		if (pIceMgr == nullptr)
			return false;

		pIceMgr->GetIceIndex(pIce, &m_nIdxDriftV, &m_nIdxDriftH);

		if (m_nIdxDriftV == -1 ||
			m_nIdxDriftH == -1)
			return false;	// »êÅà¸sµ½êAUðÔ·s
	}

	return true;
}

//=====================================================
// ObhÉü©ÁÄÚ®·é
//=====================================================
void CIceStaeteNormal::MoveToGrid(CIce *pIce)
{
	// ObhÌÊuðæ¾
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 posGrid = pIceMgr->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);
	D3DXVECTOR3 posIce = pIce->GetPosition();

	// ·ªxNgðY¬¬xÉ³K»
	float fSpeedFlow = pIceMgr->GetOceanLevel();
	D3DXVECTOR3 vecDiff = posGrid - posIce;
	D3DXVec3Normalize(&vecDiff, &vecDiff);

	vecDiff *= fSpeedFlow;

	// XÌÊuÉÚ®ÊðÁZ
	pIce->AddPosition(vecDiff);
}

//*******************************************************************************
// öóXeCg
//*******************************************************************************
//=====================================================
// ú»
//=====================================================
void CIceStaeteBreak::Init(CIce *pIce)
{
	// XÌbVðí
	pIce->DeleteMesh();
	
	// ±±ÅXjóÌGtFNgð­¶
	D3DXVECTOR3 posIce = pIce->GetPosition();
	CParticle::Create(posIce, CParticle::TYPE::TYPE_ICEBREAK);

	// XjÐð¶¬
	for (int i = 0; i < CIceStaeteBreak::NUM_ICE_BREAK; i++)
	{
		CObjectX *pPeace = CObjectX::Create();

		// fÌèÄ
		int nIdxModel = CModel::Load((char*)&PATH_ICE_DEBRIS[0]);
		pPeace->BindModel(nIdxModel);

		// Êuð_Ýè
		D3DXVECTOR3 posPeace = posIce;
		posIce.x += (float)universal::RandRange((int)(Grid::SIZE * 0.25f), -(int)(Grid::SIZE * 0.25f));
		posIce.z += (float)universal::RandRange((int)(Grid::SIZE * 0.25f), -(int)(Grid::SIZE * 0.25f));

		pPeace->SetPosition(posIce);

		// XP[ð_Ýè
		float fRand = universal::RandRange(MAX_SCALE, MIN_SCALE) * 0.1f;
		pPeace->SetScale(fRand);

		// ñ]
		float fRotY = universal::RandRange(314, 0) * 0.01f;
		pPeace->SetRotation(D3DXVECTOR3(0.0f, fRotY, 0.0f));

		// zñÉüêé
		m_aPeaceIce.push_back(pPeace);
	}

	// JE^[ðú»
	m_fTimerRepair = TIME_REPAIR_ICE;
}

//=====================================================
// I¹
//=====================================================
void CIceStaeteBreak::Uninit(CIce *pIce)
{
	for (auto it : m_aPeaceIce)
		it->Uninit();

	m_aPeaceIce.clear();
}

//=====================================================
// XV
//=====================================================
void CIceStaeteBreak::Update(CIce *pIce)
{
	// XÌCÌXV
	UpdateRepair(pIce);

	// XÌÇ]
	FollowIce(pIce);
}

//=====================================================
// CÌXV
//=====================================================
void CIceStaeteBreak::UpdateRepair(CIce *pIce)
{
	// ¾ñÅ¢éóÔÈçÊçÈ¢
	if (pIce->IsSink())
		return;

	// ^C}[ð¸ç·
	m_fTimerRepair -= CManager::GetDeltaTime();

	if (m_fTimerRepair <= 0.0f)
	{// êèÔoßÅAXðC·é
		pIce->CreateMesh();

		pIce->ChangeState(new CIceStaeteNormal);
		pIce->EnablePeck(false);
	}
}

//=====================================================
// XÌÇ]
//=====================================================
void CIceStaeteBreak::FollowIce(CIce *pIce)
{
	// XÌÇ]
	for (auto it : m_aPeaceIce)
	{
		D3DXVECTOR3 pos = { it->GetPosition().x, pIce->GetPosition().y, it->GetPosition().z };

		it->SetPosition(pos);
	}

	// ¾ÞóÔÌA{ÌÌÊuðº°é
	if (pIce->IsSink())
	{
		D3DXVECTOR3 posIce = pIce->GetPosition();
		posIce.y -= SPEED_SINK;
		pIce->SetPosition(posIce);

		if (posIce.y < HEIGHT_DELETE)
		{// êèÜÅ¾ñ¾çí·é
			pIce->Uninit();

			return;
		}
	}
}

//*******************************************************************************
// ¬êéXeCg
//*******************************************************************************
//=====================================================
// ú»
//=====================================================
void CIceStateFlow::Init(CIce *pIce)
{
	pIce->EnableStop(false);

	// ©gÌ|C^ðObh©çO·
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->DeleteIce(pIce);
}

//=====================================================
// I¹
//=====================================================
void CIceStateFlow::Uninit(CIce *pIce)
{

}

//=====================================================
// XV
//=====================================================
void CIceStateFlow::Update(CIce *pIce)
{
	if (!m_bDrift)
		UpdateSarchIce(pIce);	// XðTµÄ¢éÌXV
	else
		UpdateDriftIce(pIce);	// YµÄéÆ«ÌXV
}

//=====================================================
// XðTµÄ¢éÆ«ÌXV
//=====================================================
void CIceStateFlow::UpdateSarchIce(CIce *pIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// C¬ÌxNgæ¾
	CIceManager::E_Stream dir = pIceManager->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// ¬êé¬xÉ³K»µÄÊuðÁZ
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow;
	pIce->AddPosition(vecStream);

	// XÆÌ»è
	CollideIce(pIce);
}

//=====================================================
// Y·éÆ«ÌXV
//=====================================================
void CIceStateFlow::UpdateDriftIce(CIce *pIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// ObhÌÊuæ¾
	D3DXVECTOR3 posDrift = pIceManager->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);

	// C¬ÌxNgæ¾
	D3DXVECTOR3 posIce = pIce->GetPosition();
	if (pIceManager == nullptr)
		return;

	// C¬ÌxNgæ¾
	D3DXVECTOR3 vecDiff = posDrift - posIce;

	// ¬êé¬xÉ³K»µÄÊuðÁZ
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecDiff, &vecDiff);
	vecDiff *= fSpeedFlow;
	pIce->AddPosition(vecDiff);

	// ObhÌÊuÆÌ£ªµ«¢lðºñÁ½ç~ßé
	bool bStop = universal::DistCmpFlat(posIce, posDrift, LINE_STOP_ICE, nullptr);

#ifdef _DEBUG
	//CEffect3D::Create(posIce, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
#endif

	if (bStop)
	{
		return;
	}
}

//=====================================================
// XÆÌ»è
//=====================================================
void CIceStateFlow::CollideIce(CIce *pIce)
{
	// ¡¢éObhÌæ¾
	int nIdxV = 0;
	int nIdxH = 0;

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	D3DXVECTOR3 posIce = pIce->GetPosition();
	bool bOk = pIceManager->GetIdxGridFromPosition(posIce, &nIdxV, &nIdxH);

	if (bOk)
	{
		// ÔðÛ¶
		m_nIdxDriftV = nIdxV;
		m_nIdxDriftH = nIdxH;
	}

	// C¬ÌûüÉí¹½»èÖÌÄÑoµ
	DirectionFunc directionFuncs[CIceManager::E_Stream::STREAM_MAX] = 
	{
		&CIceStateFlow::CheckUp,
		&CIceStateFlow::CheckRight,
		&CIceStateFlow::CheckDown,
		&CIceStateFlow::CheckLeft
	};

	CIceManager::E_Stream stream = pIceManager->GetDirStream();
	
	// Y·éXª Á½çAtOð§ÄÄYObhÔðÛ¶
	m_bDrift = (this->*directionFuncs[stream])(pIce, m_nIdxDriftV, m_nIdxDriftH);

#ifdef _DEBUG
	//D3DXVECTOR3 posGrid = pIceManager->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);
	//CEffect3D::Create(posGrid, 100.0f, 5, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
#endif

	if (m_bDrift)
	{
		// ObhÉXîñðÛ¶
		pIceManager->SetIceInGrid(nIdxV, nIdxH, pIce);
		pIce->ChangeState(new CIceStaeteNormal);
	}

#ifdef _DEBUG
	//CEffect3D::Create(posIce, 100.0f, 5, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
#endif

}

//=====================================================
// ãûüÌmF
//=====================================================
bool CIceStateFlow::CheckUp(CIce *pIce, int nIdxV, int nIdxH)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// üÓÌXÌæ¾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// ¶¤ÌObhÇê©ÉXª êÎY
	if (apIce[CIceManager::DIRECTION_LEFTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHTUP] != nullptr)
	{
		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// EûüÌmF
//=====================================================
bool CIceStateFlow::CheckRight(CIce *pIce, int nIdxV, int nIdxH)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// üÓÌXÌæ¾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// ¶¤ÌObhÇê©ÉXª êÎY
	if (apIce[CIceManager::DIRECTION_RIGHTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHT] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHTDOWN] != nullptr)
	{
		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// ºûüÌmF
//=====================================================
bool CIceStateFlow::CheckDown(CIce *pIce, int nIdxV, int nIdxH)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// üÓÌXÌæ¾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// ¶¤ÌObhÇê©ÉXª êÎY
	if (apIce[CIceManager::DIRECTION_RIGHTDOWN] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFTDOWN] != nullptr)
	{
		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// ¶ûüÌmF
//=====================================================
bool CIceStateFlow::CheckLeft(CIce *pIce, int nIdxV, int nIdxH)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// üÓÌXÌæ¾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// ¶¤ÌObhÇê©ÉXª êÎY
	if (apIce[CIceManager::DIRECTION_LEFTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFT] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFTDOWN] != nullptr)
	{
		bDrift = true;
	}

	return bDrift;
}

//*******************************************************************************
// ¾ÞXeCg
//*******************************************************************************
//=====================================================
// ú»
//=====================================================
void CIceStaeteSink::Init(CIce *pIce)
{

}

//=====================================================
// I¹
//=====================================================
void CIceStaeteSink::Uninit(CIce *pIce)
{

}

//=====================================================
// XV
//=====================================================
void CIceStaeteSink::Update(CIce *pIce)
{

}