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
m_pSide(nullptr),m_pUp(nullptr), m_pState(nullptr)
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
}

//=====================================================
// I¹
//=====================================================
void CIce::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pUp);
	Object::DeleteObject((CObject**)&m_pSide);

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

	CGameObject::Uninit();
}

//=====================================================
// XV
//=====================================================
void CIce::Update(void)
{
	if (m_pState != nullptr)
		m_pState->Update(this);

	if (COcean::GetInstance() == nullptr)
	{
		return;
	}

	// CÆêÉXð®©·
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos.y = COcean::GetInstance()->GetHeight(pos,&move) + HEIGHT_ICE;

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
// ¬êé
//=====================================================
void CIce::Flows(void)
{
	AddPosition(D3DXVECTOR3(SPEED_FLOWS, 0.0f, 0.0f));
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

}

//=====================================================
// I¹
//=====================================================
void CIceStaeteBreak::Uninit(CIce *pIce)
{

}

//=====================================================
// XV
//=====================================================
void CIceStaeteBreak::Update(CIce *pIce)
{

}

//*******************************************************************************
// ¬êéXeCg
//*******************************************************************************
//=====================================================
// ú»
//=====================================================
void CIceStaeteFlow::Init(CIce *pIce)
{

}

//=====================================================
// I¹
//=====================================================
void CIceStaeteFlow::Uninit(CIce *pIce)
{

}

//=====================================================
// XV
//=====================================================
void CIceStaeteFlow::Update(CIce *pIce)
{
	pIce->AddPosition(D3DXVECTOR3(-SPEED_FLOWS, 0.0f, 0.0f));
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