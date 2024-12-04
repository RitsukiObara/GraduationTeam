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
#include "iceHard.h"
#include "iceHardMulti.h"
#include "iceManager.h"
#include "texture.h"
#include "gameObject.h"
#include "fan3D.h"
#include "meshcylinder.h"
#include "ocean.h"
#include "objectX.h"
#include "manager.h"
#include "particle.h"
#include "model.h"
#include "effect3D.h"
#include "enemy.h"
#include "player.h"
#include "MyEffekseer.h"

//*****************************************************
// èè`
//*****************************************************
namespace
{
const string PATH_TEX = "data\\TEXTURE\\MATERIAL\\ice001.jpg";				// eNX`pX
const string PATH_TEX_OVERRAY = "data\\TEXTURE\\MATERIAL\\iceanimation.jpg";	// I[oCeNX`pX
const float SIZE_INIT = 100.0f;	// úTCY
const float HEIGHT_ICE = 50.0f;	// XÌ³
const int NUM_CORNER = 6;	// pÌ
const D3DXVECTOR3 ROT_UP_INIT = { D3DX_PI * 0.5f,0.0f,0.0f };	// ã¤Ìúü«

const float TIME_REPAIR_ICE = 13.0f;	// XÌCÉ©©éÔ
const int MAX_SCALE = 50; // XP[ÌÅål
const int MIN_SCALE = 20; // XP[ÌÅ¬l
const string PATH_ICE_DEBRIS = "data\\MODEL\\block\\Drift_ice_piece.x";	// jÐXÌfpX
const float SPEED_SINK = 5.0f;	// ¾Þ¬x
const float HEIGHT_DELETE = -100.0f;	// í·éÜÅÌ³

const float HEIGHT_DEFAULT_FROM_OCEAN = 50.0f;	// C©çÌftHgÌ³
const float HEIGHT_NORMALSINK_FROM_OCEAN = 10.0f;	// C©çÌÊí¾Þ³

const float LINE_STOP_ICE = 1.0f;	// Xª~Üéµ«¢l

//------------------------------
// X«Ìè
//------------------------------
namespace tilt
{
const float SPEED_ROT = 0.1f;			// ñ]¬x
const float MAX_TILT = D3DX_PI * 0.2f;	// ÅåÌX«
const float RATE_COLLIDE = 1.6f;		// »èÌ
}

//------------------------------
// ³´gÌè
//------------------------------
namespace ripple
{
const int MAX_TIME = 7;	// ¶¬É©©éÅåÔ
const int MIN_TIME = 2;	// ¶¬É©©éÅ¬Ô
}

//------------------------------
// õÌè
//------------------------------
namespace flash
{
const int MAX_TIME = 14;			// Ä¶É©©éÅåÔ
const int MIN_TIME = 7;				// Ä¶É©©éÅ¬Ô
const int FRAME_ANIMATION = 2;		// Aj[VðØèÖ¦ét[
const int PATERN_ANIM = 5;			// Aj[VÌp^[
}
}

//*****************************************************
// ÃIoÏé¾
//*****************************************************
int CIce::s_nNumAll = 0;
std::vector<CIce*> CIce::s_Vector = {};	// ©gÌ|C^

//=====================================================
// RXgN^
//=====================================================
CIce::CIce(int nPriority) : CObject3D(nPriority), m_state(E_State::STATE_NONE), m_bBreak(false), m_bCanFind(false), m_bPeck(false),
m_pSide(nullptr),m_pUp(nullptr), m_pState(nullptr), m_bSink(false), m_bStop(nullptr), m_abRipleFrag(), m_nCntAnimFlash(0), m_rotDest()
{
	s_nNumAll++;
	s_Vector.push_back(this);
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
		case CIce::TYPE_HARDMULTI:
			pIce = new CIceHardMulti;
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

	// XeCgú»
	ChangeState(new CIceStaeteNormal);

	// õéÌú»
	StartFlash();

	// p³NXÌú»
	CObject3D::Init();

	return S_OK;
}

//=====================================================
// bVÌ¶¬
//=====================================================
void CIce::CreateMesh(void)
{
	if (m_pUp == nullptr)
	{
		m_pUp = CFan3D::Create(4, NUM_CORNER);

		if (m_pUp != nullptr)
		{
			int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
			m_pUp->SetIdxTexture(nIdxTexture);

			int nIdxTextureOverray = Texture::GetIdx(&PATH_TEX_OVERRAY[0]);
			m_pUp->SetIdxTextureOverRay(nIdxTextureOverray);
			m_pUp->SetVtx();
		}
	}

	if (m_pSide == nullptr)
	{
		m_pSide = CMeshCylinder::Create(NUM_CORNER);
		int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
		m_pSide->SetIdxTexture(nIdxTexture);
	}

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

	for (auto itr = s_Vector.begin(); itr < s_Vector.end(); itr++)
	{
		//íÎÛ¶áÈ¢ê
		if (*itr != this)
		{
			continue;
		}
		//Vector©çí
		s_Vector.erase(itr);

		break;
	}

	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	// Obh©çí
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager != nullptr)
		pIceManager->DeleteIce(this);

	// p³NXÌI¹
	CObject3D::Uninit();
}

//=====================================================
// XV
//=====================================================
void CIce::Update(void)
{
	if (!IsSink())	// ¾ÞtOª½ÁÄ¢È¢Æ«ÌÝs¤
		FollowWave();	// gÉÇ]·é

	// XeCgÌXV
	if (m_pState != nullptr)
		m_pState->Update(this);

	// ãÉæÁÄé¨Ìo
	SearchOnThis();

	// X«Ì
	Tilt();

	// ³´gÌ
	Ripples();

	// bVÌÇ]
	FollowMesh();

	// õé
	Flash();

	// p³NXÌXV
	CObject3D::Update();
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
// ©gÉæÁÄéàÌÌo
//=====================================================
void CIce::SearchOnThis(void)
{
	if (!IsCanPeck())
		return;

	vector<CGameObject*> apObject;

	GetOnTopObject(apObject);


}

//=====================================================
// ½©µçªæÁÄé»è
//=====================================================
bool CIce::IsOnTopAnyObject(void)
{
	vector<CGameObject*> apObject;

	GetOnTopObject(apObject);

	// ãÉÇê©ªæÁÄ½ç^ðÔ·
	for (CGameObject* object : apObject)
	{
		D3DXVECTOR3 posObject = object->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		if (universal::DistCmpFlat(pos, posObject, SIZE_INIT, nullptr))
		{// ½©ªæÁÄéÌÅ^ðÔ·
			return true;
		}
	}

	return false;
}

//=====================================================
// »è·éIuWFNgÌo
//=====================================================
void CIce::GetOnTopObject(vector<CGameObject*> &rVector, float fRate)
{
	vector<CGameObject*> apObject;

	// GÌÇÁ
	vector<CEnemy*> aEnemy = CEnemy::GetInstance();

	for (CEnemy* enemy : aEnemy)
		apObject.push_back((CGameObject*)enemy);

	// vC[ÌÇÁ
	vector<CPlayer*> aPlayer = CPlayer::GetInstance();

	for (CPlayer* player : aPlayer)
		apObject.push_back((CGameObject*)player);

	// ãÉÇê©ªæÁÄ½ç^ðÔ·
	for (CGameObject* object : apObject)
	{
		D3DXVECTOR3 posObject = object->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		if (universal::DistCmpFlat(pos, posObject, SIZE_INIT * fRate, nullptr))
		{// ½©ªæÁÄéÌÅ^ðÔ·
			rVector.push_back(object);
		}
	}

}

//=====================================================
// X«Ì
//=====================================================
void CIce::Tilt(void)
{
	if (!IsCanPeck())
		return;	// @¯È¢XÈçÊçÈ¢

	//------------------------------
	// X«ÌvZ
	//------------------------------
	// æÁÄ¢éIuWFNgÌæ¾
	vector<CGameObject*> apObject;
	GetOnTopObject(apObject, tilt::RATE_COLLIDE);

	int nNumObject = (int)apObject.size();

	if (nNumObject != 0)
	{// ½©µçæÁÄ½ç»è
		D3DXVECTOR3 vecDiff = { 0.0f,0.0f,0.0f };

		for (CGameObject* pObj : apObject)
		{
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 posObj = pObj->GetPosition();

			vecDiff += posObj - pos;
		}

		// ·ªxNgð½Ï»
		vecDiff /= (float)nNumObject;

		m_rotDest.x = vecDiff.z / Grid::SIZE * tilt::MAX_TILT;
		m_rotDest.z = -vecDiff.x / Grid::SIZE * tilt::MAX_TILT;
	}
	else	// ãÉ½àæÁÄÈ©Á½ç³Éß·
		m_rotDest = { 0.0f,0.0f,0.0f };

	// ü«Ìâ³
	D3DXVECTOR3 rot = GetRotation();

	rot += (m_rotDest - rot) * tilt::SPEED_ROT;

	SetRotation(rot);
}

//=====================================================
// ³´gÌ
//=====================================================
void CIce::Ripples(void)
{
	// ^C}[ÅÌ¹«~ß
	m_fTimerRipples += CManager::GetDeltaTime();

	if (m_fTimerRipples <= m_fSpawnTimeRipples)
		return;

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	COcean::E_Stream stream = pIceMgr->GetDirStream();

	// ¬êÉÁ½ûüÌtOª§ÁÄ½çðÊé
	if (!m_abRipleFrag[stream])
		return;

	// GtFNgÌgXtH[Ýè
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[stream];

	D3DXVECTOR3 posEffect = GetPosition() - vecStream * SIZE_INIT;
	D3DXVECTOR3 rotEffect = { 0.0f,0.0f,0.0f };
	rotEffect.y = atan2f(vecStream.x, vecStream.z);

	// GtFNgÌ¶¬
	MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_RIPPLESTRINGS, posEffect, rotEffect);

	// ^C}[ÌÄÝè
	m_fTimerRipples = 0.0f;

	int nRand = universal::RandRange(ripple::MAX_TIME, ripple::MIN_TIME);

	m_fSpawnTimeRipples = (float)nRand;
}

//=====================================================
// bVÌÇ]
//=====================================================
void CIce::FollowMesh(void)
{
	if (m_pUp == nullptr || m_pSide == nullptr)
		return;

	// }gbNXðmè³¹é
	CulcMatrix();

	D3DXMATRIX mtx = GetMatrix();

	// ãÌî|SÌÝè
	m_pUp->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUp->SetMatrixParent(mtx);

	// TChÌV_[ÌÝè
	m_pSide->SetPosition(D3DXVECTOR3(0.0f, -HEIGHT_ICE, 0.0f));
	m_pSide->SetMatrixParent(mtx);
}

//=====================================================
// õéÌJn
//=====================================================
void CIce::StartFlash(void)
{
	if (m_pUp == nullptr)
		return;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *m_pUp->GetVtxBuff();
	VERTEX_3D* pVtx;

	if (pVtxBuff == nullptr)
		return;

	// ¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// XN[ðZbg
	int nNumVtx = m_pUp->GetNumVtx();

	for (int i = 0; i < nNumVtx + 2; i++)
	{
		pVtx[i].tex2 = pVtx[i].tex;

		pVtx[i].tex2.x *= 1.0f / flash::PATERN_ANIM;	// Aj[VîÉkßé
		pVtx[i].tex2.x += 1.0f / flash::PATERN_ANIM * flash::PATERN_ANIM;
	}

	// ¸_obt@ÌAbN
	pVtxBuff->Unlock();

	// ^C}[ÌÄÝè
	m_fTimerFlash = 0.0f;
	int nRand = universal::RandRange(flash::MAX_TIME, flash::MIN_TIME);
	m_fTimeStartFlash = (float)nRand;
}

//=====================================================
// «çèÆõé
//=====================================================
void CIce::Flash(void)
{
	m_fTimerFlash += CManager::GetDeltaTime();

	if (m_fTimerFlash < m_fTimeStartFlash)
		return;

	if (m_pUp == nullptr)
		return;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *m_pUp->GetVtxBuff();
	VERTEX_3D* pVtx;

	if (pVtxBuff == nullptr)
		return;

	// ¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// XN[³¹é
	int nNumVtx = m_pUp->GetNumVtx();

	bool bPassAll = true;	// S¸_ªÊßµ½tO

	for (int i = 0; i < nNumVtx + 2; i++)
	{
		pVtx[i].tex2 = pVtx[i].tex;

		pVtx[i].tex2.x *= 1.0f / flash::PATERN_ANIM;	// Aj[VîÉkßé
		pVtx[i].tex2.x += 1.0f / flash::PATERN_ANIM * m_nPaternAnim;
	}

	// ¸_obt@ÌAbN
	pVtxBuff->Unlock();

	// JE^[ÁZ
	m_nCntAnimFlash++;

	if (m_nCntAnimFlash % flash::FRAME_ANIMATION == 0)
	{
		m_nPaternAnim = (m_nPaternAnim + 1) % flash::PATERN_ANIM;

		if (m_nPaternAnim == 0)
			StartFlash();	// Aj[VªßÁ½çZbg
	}
}

//=====================================================
// FÌÝè
//=====================================================
void CIce::SetColor(D3DXCOLOR col)
{
	if (m_pSide == nullptr || m_pUp == nullptr)
		return;

	m_pSide->SetCol(col);
	m_pUp->SetCol(col);
}

//=====================================================
// `æ
//=====================================================
void CIce::Draw(void)
{
	CObject3D::Draw();
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
	m_pSide->SetRadius(fRadius * 0.5f);
	m_pSide->SetHeight(HEIGHT_ICE);
	D3DXVECTOR3 posSide = posIce;
	posSide.y -= HEIGHT_ICE;
	m_pSide->SetPosition(posSide);

	m_pSide->SetVtx();
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

		if (m_nIdxDriftV == 0 && m_nIdxDriftH == 0)
		{
			int n = 0;
		}

		if (m_nIdxDriftV == -1 ||
			m_nIdxDriftH == -1)
			return false;	// »êÅà¸sµ½êAUðÔ·
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
	pIce->Translate(vecDiff);

	// ãÌIuWFNgð®©·
	MoveObjectOnIce(vecDiff,pIce);
}

//=====================================================
// ãÌIuWFNgð®©·
//=====================================================
void CIceStaeteNormal::MoveObjectOnIce(D3DXVECTOR3 vecMove,CIce *pIce)
{
	vector<CGameObject*> apObject;

	// GÌÇÁ
	vector<CEnemy*> aEnemy = CEnemy::GetInstance();

	for (CEnemy* enemy : aEnemy)
		apObject.push_back((CGameObject*)enemy);

	// vC[ÌÇÁ
	vector<CPlayer*> aPlayer = CPlayer::GetInstance();

	for (CPlayer* player : aPlayer)
		apObject.push_back((CGameObject*)player);

	// ãÉÇê©ªæÁÄ½ç®©·
	for (CGameObject* object : apObject)
	{
		if (object == nullptr)
			continue;

		D3DXVECTOR3 posObject = object->GetPosition();
		D3DXVECTOR3 pos = pIce->GetPosition();

		if (universal::DistCmpFlat(pos, posObject, SIZE_INIT, nullptr))
			object->Translate(vecMove); // ½©ªæÁÄéÌÅ®©·
	}
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

		// p[eBN­¶
		CParticle::Create(pIce->GetPosition(), CParticle::TYPE::TYPE_REPAIRICE);
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
		UpdateSearchIce(pIce);	// XðTµÄ¢éÌXV
	else
		UpdateDriftIce(pIce);	// YµÄéÆ«ÌXV
}

//=====================================================
// XðTµÄ¢éÆ«ÌXV
//=====================================================
void CIceStateFlow::UpdateSearchIce(CIce *pIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// C¬ÌxNgæ¾
	COcean::E_Stream dir = pIceManager->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// ¬êé¬xÉ³K»µÄÊuðÁZ
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow;
	pIce->Translate(vecStream);

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
	pIce->Translate(vecDiff);

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
	int nIdxV = -1;
	int nIdxH = -1;

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	D3DXVECTOR3 posIce = pIce->GetPosition();
	bool bOk = pIceManager->GetIdxGridFromPosition(posIce, &nIdxV, &nIdxH);

	// ÔðÛ¶
	m_nIdxDriftV = nIdxV;
	m_nIdxDriftH = nIdxH;

	// C¬ÌûüÉí¹½»èÖÌÄÑoµ
	DirectionFunc directionFuncs[COcean::E_Stream::STREAM_MAX] = 
	{
		&CIceStateFlow::CheckUp,
		&CIceStateFlow::CheckRight,
		&CIceStateFlow::CheckDown,
		&CIceStateFlow::CheckLeft
	};

	COcean::E_Stream stream = pIceManager->GetDirStream();
	
	// Y·éXª Á½çAtOð§ÄÄYObhÔðÛ¶
	if (m_nIdxDriftV == -1 || m_nIdxDriftH == -1)
		return;

	if (pIceManager->GetGridIce(&nIdxV, &nIdxH) != nullptr)
	{// ùÉ»ÌêÉXª Á½çÊíóÔÉÚs
		pIce->ChangeState(new CIceStaeteNormal);
		return;
	}

	vector<CIce*> apIceHit;
	m_bDrift = (this->*directionFuncs[stream])(pIce, m_nIdxDriftV, m_nIdxDriftH, apIceHit);

#ifdef _DEBUG
	D3DXVECTOR3 posEffect = pIceManager->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);
	debug::Effect3DShort(posEffect, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
#endif

	if (m_bDrift)
	{
		debug::Effect3DShort(pIce->GetPosition(), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 120);
		// ObhÉXîñðÛ¶
		if (pIceManager->SetIceInGrid(nIdxV, nIdxH, pIce))
		{
			for (CIce* pIce : apIceHit)
			{
				if (pIce == nullptr)
					continue;

				// p[eBNð­¶
				D3DXVECTOR3 pos = pIce->GetPosition();
				D3DXVECTOR3 posHitIce = pIce->GetPosition();
				D3DXVECTOR3 vecDIff = posHitIce - pos;

				pos += vecDIff * 0.5f;

				CParticle::Create(pos, CParticle::TYPE::TYPE_STICK_ICE);
			}
		}

		pIce->ChangeState(new CIceStaeteNormal);
	}
}

//=====================================================
// ãûüÌmF
//=====================================================
bool CIceStateFlow::CheckUp(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// üÓÌXÌæ¾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// ã¤ÌObhÇê©ÉXª êÎY
	if (apIce[CIceManager::DIRECTION_LEFTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHTUP] != nullptr)
	{
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFTUP]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHTUP]);

		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// EûüÌmF
//=====================================================
bool CIceStateFlow::CheckRight(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// üÓÌXÌæ¾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// E¤ÌObhÇê©ÉXª êÎY
	if (apIce[CIceManager::DIRECTION_RIGHTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHT] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHTDOWN] != nullptr)
	{
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHTUP]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHT]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHTDOWN]);

		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// ºûüÌmF
//=====================================================
bool CIceStateFlow::CheckDown(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// üÓÌXÌæ¾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// º¤ÌObhÇê©ÉXª êÎY
	if (apIce[CIceManager::DIRECTION_RIGHTDOWN] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFTDOWN] != nullptr)
	{
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHTDOWN]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFTDOWN]);

		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// ¶ûüÌmF
//=====================================================
bool CIceStateFlow::CheckLeft(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce)
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
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFTUP]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFT]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFTDOWN]);

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