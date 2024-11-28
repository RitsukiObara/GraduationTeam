//*****************************************************
//
// ÉIÅ[ÉVÉÉÉìèàóù[ocean.cpp]
// Author:ëÅêÏóFãM
//
//*****************************************************

//*****************************************************
// ÉCÉìÉNÉãÅ[Éh
//*****************************************************
#include "meshfield.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "game.h"
#include "effect3D.h"
#include "texture.h"
#include "ocean.h"
#include "iceManager.h"
#include "timer.h"
#include "universal.h"
#include "albatross.h"
#include "BG_Ice.h"

//*****************************************************
// íËêîíËã`
//*****************************************************
namespace
{
	const float OCEAN_ROT_CHANGE_TIME_DEFAULT = 10.0f;	// ÉfÉtÉHÉãÉgÇÃäCó¨å¸Ç´ïœçXéûä‘
	const int OCEAN_ROT_CHANGE_TIME_DEGREE = 10;		// äCó¨å¸Ç´ïœçXéûä‘Ç‘ÇÍïù
	const float FLOW_LEVEL_MULTIPLY = 0.008f;			// äCó¨ÇÃë¨ìxÇÃî{ó¶
	const float ALBATROSS_SPAWN_TIME = 3.0f;			// ÉAÉzÉEÉhÉäèoåªéûä‘
	const int MAX_ALBATROSS = 2;						// ÉAÉzÉEÉhÉäç≈ëÂêî
	const int MAX_RANGE_LEFT = -1800;					// ÉâÉìÉ_ÉÄÉåÉìÉWç∂ç≈ëÂêî
	const int MAX_RANGE_RIGHT = 1500;					// ÉâÉìÉ_ÉÄÉåÉìÉWâEç≈ëÂêî
	const int MAX_RANGE_UP = 1200;						// ÉâÉìÉ_ÉÄÉåÉìÉWè„ç≈ëÂêî
	const int MAX_RANGE_DOWN = -1500;					// ÉâÉìÉ_ÉÄÉåÉìÉWâ∫ç≈ëÂêî
	const float Z_UP = 1500.0f;							// Zï˚å¸è„ï˚å¸
	const float Z_DOWN = -1500.0f;						// Zï˚å¸â∫ï˚å¸
	const float X_LEFT = -2500.0f;						// Xï˚å¸è„ï˚å¸
	const float X_RIGHT = 2500.0f;						// Xï˚å¸â∫ï˚å¸
	const int BGICE_CREATE_CNT_L = 200;					// îwåiïXÇ™ê∂ê¨Ç≥ÇÍÇÈïb(ç∂)
	const int BGICE_CREATE_CNT_R = 200;					// îwåiïXÇ™ê∂ê¨Ç≥ÇÍÇÈïb(âE)
	const int BGICE_CREATE_CNT_UP = 200;				// îwåiïXÇ™ê∂ê¨Ç≥ÇÍÇÈïb(è„)
	const int BGICE_CREATE_CNT_DOWN = 200;				// îwåiïXÇ™ê∂ê¨Ç≥ÇÍÇÈïb(â∫)
}

//*****************************************************
// ê√ìIÉÅÉìÉoïœêîêÈåæ
//*****************************************************
COcean* COcean::m_pOcean = nullptr;	// é©êgÇÃÉ|ÉCÉìÉ^

//=====================================================
// ÉRÉìÉXÉgÉâÉNÉ^
//=====================================================
COcean::COcean(int nPriority) : CMeshField(nPriority)
{
	m_fSpeed = 0.0f;
	m_nRandKeep = 0;
	m_nRandNextKeep = 0;
	m_bRandState = false;
	m_fRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fProgressTime = 0.0f;
	m_fRotChangeTime = 0.0f;
	m_nBgiceCnt = 0;
}

//=====================================================
// ÉfÉXÉgÉâÉNÉ^
//=====================================================
COcean::~COcean()
{

}

//=====================================================
// ê∂ê¨èàóù
//=====================================================
COcean* COcean::Create(void)
{
	if (m_pOcean == nullptr)
	{
		m_pOcean = new COcean;

		if (m_pOcean != nullptr)
		{
			m_pOcean->Init();
		}
	}

	return m_pOcean;
}

//=====================================================
// èâä˙âªèàóù
//=====================================================
HRESULT COcean::Init(void)
{
	m_nBgiceCnt = 0;
	
	CMeshField::Init();

	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager != nullptr)
	{ // ó¨ïXÉ}ÉlÅ[ÉWÉÉÅ[Ç™ NULL Ç∂Ç·Ç»Ç¢èÍçá

		// ï€ë∂ópïœêîÇ…åªç›ÇÃäCó¨Çê›íËÇ∑ÇÈ
		m_nRandKeep = pIceManager->GetDirStream();
	}

	SetNextOceanRot();	// ç≈èâÇ…éüÇÃå¸Ç´ê›íË

	return S_OK;
}

//=====================================================
// èIóπèàóù
//=====================================================
void COcean::Uninit(void)
{
	m_pOcean = nullptr;

	CMeshField::Uninit();
}

//=====================================================
// çXêVèàóù
//=====================================================
void COcean::Update(void)
{
	float OceanFlowLevel = 0.0f;

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if(pIceMgr != nullptr)
		OceanFlowLevel = pIceMgr->GetOceanLevel();	//	äCó¨ÉåÉxÉãÇÃéÊìæ

	CMeshField::Update();

	m_fSpeed += FLOW_LEVEL_MULTIPLY * OceanFlowLevel;

	BgIceRotState();
	//OceanCycleTimer();
	OceanChangeCheck();

	universal::LimitRot(&m_fSpeed);

	CMeshField::Wave(m_fSpeed);
}

//=====================================================
// ï`âÊèàóù
//=====================================================
void COcean::Draw(void)
{
	CMeshField::Draw();

	// ÉfÉoÉCÉXÇÃéÊìæ
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//í∏ì_ÉtÉHÅ[É}ÉbÉgÇÃê›íË
	pDevice->SetFVF(FVF_VERTEX_3D);
}

//====================================================
// éüÇÃäCó¨ÇÃå¸Ç´ê›íËèàóù
//====================================================
void COcean::SetNextOceanRot(void)
{
	CGame* pGame = CGame::GetInstance();
	if (pGame == nullptr)
		return;

	// å¸Ç´ïœçX
	do
	{
		m_nRandNextKeep = universal::RandRange(COcean::E_Stream::STREAM_MAX, COcean::E_Stream::STREAM_UP);
	} while (m_nRandKeep == m_nRandNextKeep);	// éüÇÃå¸Ç´Ç™ïœÇÌÇÈÇ‹Ç≈óêêîÇâÒÇ∑

	// ïœçXéûä‘ê›íË
	m_fRotChangeTime = OCEAN_ROT_CHANGE_TIME_DEFAULT + (float)universal::RandRange(OCEAN_ROT_CHANGE_TIME_DEGREE, 0);
	m_fProgressTime = 0.0f;
}

//====================================================
// äCó¨ÇÃå¸Ç´ïœçXéûä‘Ç©ämîFèàóù
//====================================================
void COcean::OceanChangeCheck(void)
{
	CManager* pManager = CManager::GetInstance();
	COcean* pOcean = COcean::GetInstance();
	CIceManager* pIceManager = CIceManager::GetInstance();
	CGame* pGame = CGame::GetInstance();

	if (pManager == nullptr || pOcean == nullptr || pIceManager == nullptr || pGame == nullptr)
		return;

	COcean::E_Stream OceanFlow = pIceManager->GetDirStreamNext();

	m_fProgressTime += pManager->GetDeltaTime();
	if (m_fProgressTime >= m_fRotChangeTime)
	{// ïœçXéûä‘Ç…Ç»Ç¡ÇΩ
		pOcean->SetOceanSpeedState(COcean::OCEAN_STATE_DOWN);	// äCó¨ÇÃë¨ìxÇâ∫Ç∞ÇÈ
		pIceManager->SetDirStreamNext((COcean::E_Stream)(m_nRandNextKeep));	// äCó¨ÇÃå¸Ç´ÇÉâÉìÉ_ÉÄÇ…Ç∑ÇÈ
		m_nRandKeep = m_nRandNextKeep;	// åªç›ÇÃå¸Ç´Ç…ê›íË
		SetNextOceanRot();	// éüÇÃå¸Ç´ê›íË
		m_bRandState = false;	// ÉâÉìÉ_ÉÄÇÃèÛë‘Ç false Ç…Ç∑ÇÈ
	}

	if (m_fProgressTime >= m_fRotChangeTime - ALBATROSS_SPAWN_TIME)
	{
		if (m_bRandState == false)
		{
			for (int nCnt = 0; nCnt < MAX_ALBATROSS; nCnt++)
			{
				// ÉAÉzÉEÉhÉäê∂ê¨
				CAlbatross::Create((COcean::E_Stream)(m_nRandNextKeep));
			}

			m_bRandState = true;
		}
	}
}

//====================================================
// äCó¨ÇÃå¸Ç´Ç∆ÉÅÉbÉVÉÖÇÃå¸Ç´ÇòAìÆÇ≥ÇπÇÈèàóù
//====================================================
void COcean::BgIceRotState(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	int OceanFlowKeep = pIceManager->GetDirStream();
	m_fRot = CMeshField::GetRotation();

	//	ñÓàÛÇ™äCó¨ÇÃå¸Ç´Ç…ó¨ÇÍÇÈèàóù
	if (OceanFlowKeep == COcean::STREAM_UP)
	{
		//â∫Ç©ÇÁîwåiïXÇèoÇ∑èàóù
		BgIceSetPosDown();
	}

	if (OceanFlowKeep == COcean::STREAM_DOWN)
	{
		//è„Ç©ÇÁîwåiïXÇèoÇ∑èàóù
		BgIceSetPosUp();
	}

	if (OceanFlowKeep == COcean::STREAM_LEFT)
	{
		//â∫Ç©ÇÁîwåiïXÇèoÇ∑èàóù
		BgIceSetPosR();
	}

	if (OceanFlowKeep == COcean::STREAM_RIGHT)
	{
		//è„Ç©ÇÁîwåiïXÇèoÇ∑èàóù
		BgIceSetPosL();
	}
}

//====================================================
// äCó¨é¸ä˙Çéûä‘Ç≈ä«óùÇ∑ÇÈèàóù
//====================================================
void COcean::OceanCycleTimer(void)
{
	COcean* pOcean = COcean::GetInstance();
	CIceManager* pIceManager = CIceManager::GetInstance();

	CGame *pGame = CGame::GetInstance();

	if (pOcean == nullptr || pIceManager == nullptr || pGame == nullptr)
		return;

	int OceanCycleTimer = pGame->GetTimeSecond();	 // åªç›ÇÃÉ^ÉCÉÄÇéÊìæ

	// 10ÇÃî{êîÇÃéûÇ…ì¸ÇÈ
	if (OceanCycleTimer % 10 == 0)
	{
		if (m_bRandState == false)
		{
			m_bRandState = true;

			m_nRandKeep = m_nRandNextKeep;
		}

		// åªèÛÇ∆éüÇÃå¸Ç´Ç™ìØÇ∂Ç∆Ç´êîílÇÉäÉZÉbÉg
		if (m_nRandKeep == m_nRandNextKeep)
		{
			m_nRandNextKeep = universal::RandRange(COcean::E_Stream::STREAM_MAX, COcean::E_Stream::STREAM_UP);
		}

		// åªèÛÇ∆éüÇÃå¸Ç´Ç™ìØÇ∂Ç∂Ç·Ç»Ç¢Ç∆Ç´
		if (m_nRandKeep != m_nRandNextKeep)
		{
			pOcean->SetOceanSpeedState(COcean::OCEAN_STATE_DOWN);	// äCó¨ÇÃë¨ìxÇâ∫Ç∞ÇÈ
			pIceManager->SetDirStreamNext((COcean::E_Stream)(m_nRandNextKeep));	// äCó¨ÇÃå¸Ç´ÇÉâÉìÉ_ÉÄÇ…Ç∑ÇÈ
		}
	}
}

//====================================================
// è„ï˚å¸Ç©ÇÁîwåiïXÇ™èoÇƒÇ≠ÇÈèàóù
//====================================================
void COcean::BgIceSetPosUp(void)
{
	float posX = (float)universal::RandRange(MAX_RANGE_LEFT, MAX_RANGE_RIGHT);
	int type = universal::RandRange(bgice::BGICE_MODEL, 0);

	m_nBgiceCnt++;

	if (m_nBgiceCnt >= BGICE_CREATE_CNT_UP)
	{
		// îwåiïXÇÃÉçÅ[Éh
		CBgIce::Create(D3DXVECTOR3(posX, 0.0f, Z_UP), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);

		m_nBgiceCnt = 0;
	}
}

//====================================================
// â∫ï˚å¸Ç©ÇÁîwåiïXÇ™èoÇƒÇ≠ÇÈèàóù
//====================================================
void COcean::BgIceSetPosDown(void)
{
	float posX = (float)universal::RandRange(MAX_RANGE_LEFT, MAX_RANGE_RIGHT);
	int type = universal::RandRange(bgice::BGICE_MODEL, 0);

	m_nBgiceCnt++;

	if (m_nBgiceCnt >= BGICE_CREATE_CNT_DOWN)
	{
		// îwåiïXÇÃÉçÅ[Éh
		CBgIce::Create(D3DXVECTOR3(posX, 0.0f, Z_DOWN), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);

		m_nBgiceCnt = 0;
	}
}

//====================================================
// ç∂ï˚å¸Ç©ÇÁîwåiïXÇ™èoÇƒÇ≠ÇÈèàóù
//====================================================
void COcean::BgIceSetPosL(void)
{
	float posZ = (float)universal::RandRange(MAX_RANGE_UP, MAX_RANGE_DOWN);
	int type = universal::RandRange(bgice::BGICE_MODEL, 0);

	m_nBgiceCnt++;

	if (m_nBgiceCnt >= BGICE_CREATE_CNT_L)
	{
		// îwåiïXÇÃÉçÅ[Éh
		CBgIce::Create(D3DXVECTOR3(X_LEFT, 0.0f, posZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);

		m_nBgiceCnt = 0;
	}
}

//====================================================
// âEï˚å¸Ç©ÇÁîwåiïXÇ™èoÇƒÇ≠ÇÈèàóù
//====================================================
void COcean::BgIceSetPosR(void)
{
	float posZ = (float)universal::RandRange(MAX_RANGE_UP, MAX_RANGE_DOWN);
	int type = universal::RandRange(bgice::BGICE_MODEL, 0);

	m_nBgiceCnt++;

	if (m_nBgiceCnt >= BGICE_CREATE_CNT_R)
	{
		// îwåiïXÇÃÉçÅ[Éh
		CBgIce::Create(D3DXVECTOR3(X_RIGHT, 0.0f, posZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);

		m_nBgiceCnt = 0;
	}
}

