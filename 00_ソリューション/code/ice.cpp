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

//*****************************************************
// èè`
//*****************************************************
namespace
{
const float SPEED_FLOWS = 1.0f;	// ¬êé¬x
const string PATH_TEX = "data\\TEXTURE\\UI\\ice.png";	// eNX`pX
const float SIZE_INIT = 100.0f;	// úTCY
const float HEIGHT_ICE = 50.0f;	// XÌ³
const int NUM_CORNER = 6;	// pÌ
const D3DXVECTOR3 ROT_UP_INIT = { D3DX_PI * 0.5f,0.0f,0.0f };	// ã¤Ìúü«

const float TIME_REPAIR_ICE = 10.0f;	// XÌCÉ©©éÔ
const int MAX_SCALE = 50; // XP[ÌÅål
const int MIN_SCALE = 20; // XP[ÌÅ¬l
const string PATH_ICE_DEBRIS = "data\\MODEL\\block\\Drift_ice_small.x";	// jÐXÌfpX
const float SPEED_SINK = 5.0f;	// ¾Þ¬x
const float HEIGHT_DELETE = -100.0f;	// í·éÜÅÌ³
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
m_pSide(nullptr),m_pUp(nullptr), m_pState(nullptr), m_bSink(false)
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
	if (m_pState != nullptr)
		m_pState->Update(this);
	
	if (!IsSink())	// ¾ÞtOª½ÁÄ¢È¢Æ«ÌÝs¤
		FollowWave();	// gÉÇ]·é
}

//=====================================================
// ¬êé
//=====================================================
void CIce::Flows(void)
{
	AddPosition(D3DXVECTOR3(SPEED_FLOWS, 0.0f, 0.0f));
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
	// ^C}[ð¸ç·
	m_fTimerRepair -= CManager::GetDeltaTime();

	if (m_fTimerRepair <= 0.0f)
	{// êèÔoßÅAXðC·é
		pIce->CreateMesh();

		pIce->ChangeState(new CIceStaeteNormal);
		pIce->EnablePeck(false);
	}

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
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// C¬ÌxNgæ¾
	CIceManager::E_Stream dir = pIceManager->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// ¬êé¬xÉ³K»µÄÊuðÁZ
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= SPEED_FLOWS;
	pIce->AddPosition(vecStream);

	// XÆÌ»è
	CollideIce();
}

//=====================================================
// XÆÌ»è
//=====================================================
void CIceStateFlow::CollideIce(void)
{
	// ¡¢éObhÌæ¾

	// C¬ÌûüÉí¹½»èÖÌÄÑoµ
	DirectionFunc directionFuncs[CIceManager::E_Stream::STREAM_MAX] = 
	{ 
		&CIceStateFlow::CheckUp, 
		&CIceStateFlow::CheckDown, 
		&CIceStateFlow::CheckRight, 
		&CIceStateFlow::CheckLeft 
	};

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	CIceManager::E_Stream stream = pIceManager->GetDirStream();

	(this->*directionFuncs[stream])();
}

//=====================================================
// ãûüÌmF
//=====================================================
void CIceStateFlow::CheckUp(void)
{

}

//=====================================================
// EûüÌmF
//=====================================================
void CIceStateFlow::CheckRight(void)
{

}

//=====================================================
// ºûüÌmF
//=====================================================
void CIceStateFlow::CheckDown(void)
{

}

//=====================================================
// ¶ûüÌmF
//=====================================================
void CIceStateFlow::CheckLeft(void)
{

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