//*****************************************************
//
// ¬XÌ[flowIce.cpp]
// Author:ûüRç
//
//*****************************************************

//*****************************************************
// CN[h
//*****************************************************
#include "flowIce.h"
#include "ice.h"
#include "iceManager.h"
#include "effect3D.h"
#include "manager.h"
#include "sound.h"
#include "player.h"
#include "camera.h"
#include "inputjoypad.h"
#include "debugproc.h"

//*****************************************************
// èè`
//*****************************************************
namespace
{
const float TIME_DELETE = 20.0f;	// XªÁ¦éÜÅÌÔ

const float POW_CAMERAQUAKE_DEFAULT = 0.1f;		// JÌhêÌftHgl
const int FRAME_CAMERAQUAKE_DEFAULT = 30;		// JÌhêÌftHgt[

const float POW_VIB_CHAIN = 0.6f;	// AÌRg[[hê­³
const int TIME_VIB_CHAIN = 40;		// AÌRg[[hê·³
}

//*****************************************************
// ÃIoÏé¾
//*****************************************************
vector<CFlowIce*> CFlowIce::s_vector;	// i[pÌzñ

//=====================================================
// RXgN^
//=====================================================
CFlowIce::CFlowIce(int nPriority) : CObject(nPriority), m_fTimerDelete(0.0f), m_bInScrnAllIce(false)
{
	s_vector.push_back(this);
}

//=====================================================
// fXgN^
//=====================================================
CFlowIce::~CFlowIce()
{

}

//=====================================================
// ¶¬
//=====================================================
CFlowIce *CFlowIce::Create(void)
{
	CFlowIce *pFlowice = nullptr;

	pFlowice = new CFlowIce;
	
	if (pFlowice != nullptr)
		pFlowice->Init();

	return pFlowice;
}

//=====================================================
// ú»
//=====================================================
HRESULT CFlowIce::Init(void)
{
	return S_OK;
}

//=====================================================
// XðzñÉÇÁ
//=====================================================
void CFlowIce::AddIceToArray(CIce *pIce)
{
	m_apIce.push_back(pIce);
}

//=====================================================
// I¹
//=====================================================
void CFlowIce::Uninit(void)
{
	for (auto itr = s_vector.begin(); itr < s_vector.end(); itr++)
	{
		if (*itr == this)
		{
			s_vector.erase(itr);

			break;
		}
	}

	Release();
}

//=====================================================
// XV
//=====================================================
void CFlowIce::Update(void)
{
	// Çê©ÌXª~ÜÁÄ¢È¢©Ì`FbN
	CheckSomeIceStop();

	// ¼Ì¬XÆÌ»è
	CollideOtherFlowIce();

	if (!m_bInScrnAllIce)
		CheckInAllIce();	// SÄÌXªfÁ½©Ì»è
	else
		CheckDelete();	// ímF

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// SÄÌXªæÊÉfÁ½©Ì`FbN
//=====================================================
void CFlowIce::CheckInAllIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	for (int i = 0; i < (int)m_apIce.size(); i++)
	{
		if (m_apIce[i] == nullptr)
			continue;

		// æÊà»è
		D3DXVECTOR3 pos = m_apIce[i]->GetPosition();
		if (universal::IsInScreen(pos, nullptr))
		{// Çê©êÂÅàæÊàÉ êÎtOð§Äé
			m_bInScrnAllIce = true;
		}
	}
}

//=====================================================
// Çê©ÌXª~ÜÁÄ¢È¢©Ì`FbN
//=====================================================
void CFlowIce::CheckSomeIceStop(void)
{
	for (auto it : m_apIce)
	{
		if (it->IsStop())
		{// ~ÜÁÄ¢éÈçSÄÌXð~ßé
			StopAllIce();
			Uninit();	// ©gÌjü
			break;
		}
	}
}

//=====================================================
// ¼Ì¬XÆÌ»è
//=====================================================
void CFlowIce::CollideOtherFlowIce(void)
{
	
}

//=====================================================
// SÄÌXð~ßé
//=====================================================
void CFlowIce::StopAllIce(void)
{
	CIceManager *pIceManager = CIceManager::GetInstance();
	
	if (pIceManager == nullptr)
		return;

	for (auto it : m_apIce)
	{
		if (pIceManager->IsIceInGrid(it))
			continue;

		pIceManager->AddIce(it, it->GetPosition());

		it->ChangeState(new CIceStaeteNormal);
	}

	Sound::Play(CSound::LABEL_SE_ICE_CHAIN);

	// Jðhç·
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
		pCamera->SetQuake(POW_CAMERAQUAKE_DEFAULT, POW_CAMERAQUAKE_DEFAULT, FRAME_CAMERAQUAKE_DEFAULT);

	// Rg[[ðU®³¹é
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	for (CPlayer *pPlayer : apPlayer)
		pPlayer->VibJoypad(POW_VIB_CHAIN, TIME_VIB_CHAIN);
}

//=====================================================
// êÔß¢Obhðßé
//=====================================================
void CFlowIce::GetNearestGrid(void)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	for (auto it : m_apIce)
	{
		if (pIceManager->IsIceInGrid(it))
			continue;

		pIceManager->AddIce(it, it->GetPosition());

		it->ChangeState(new CIceStaeteNormal);
	}
}

//=====================================================
// ãÉ éàÌð~ßé
//=====================================================
void CFlowIce::StopOnTopObject(void)
{

}

//=====================================================
// SXÌí
//=====================================================
void CFlowIce::DeleteAllIce(void)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// vC[Ìæ¾
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	for (int i = 0; i < (int)m_apIce.size(); i++)
	{
		if (m_apIce[i] == nullptr)
			continue;
		
		for (CPlayer *pPlayer : apPlayer)
		{// vC[ªãÉæÁÄ½çHit·é
			D3DXVECTOR3 pos = pPlayer->GetPosition();
			D3DXVECTOR3 posIce = m_apIce[i]->GetPosition();

			if (universal::DistCmpFlat(posIce, pos, Grid::SIZE, nullptr))
				pPlayer->Hit(0.0f);
		}

		pIceManager->DeleteIce(m_apIce[i]);
		m_apIce[i]->Uninit();
	}

	m_apIce.clear();
}

//=====================================================
// XíÌmF
//=====================================================
void CFlowIce::CheckDelete(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	for (int i = 0; i < (int)m_apIce.size(); i++)
	{
		if (m_apIce[i] == nullptr)
			continue;

		// æÊà»è
		D3DXVECTOR3 pos = m_apIce[i]->GetPosition();
		if (universal::IsInScreen(pos, nullptr))
			return;	// Çê©êÂÅàæÊàÉ êÎÖðI¹
	}

	// ±±ÜÅÊÁ½çXðí
	DeleteAllIce();
	Uninit();
}

//=====================================================
// fobO
//=====================================================
void CFlowIce::Debug(void)
{
	CDebugProc::GetInstance()->Print("\n¬XVXeÌ[%d]", s_vector.size());
}

//=====================================================
// Åàß¢ObhÌæ¾
//=====================================================
void CFlowIce::GetNerestGrid(D3DXVECTOR3 pos, int *pIdxV, int *pIdxH)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	float fDistMin = FLT_MAX;
	CIce *pIceNearest = nullptr;

	for (int i = 0; i < (int)m_apIce.size(); i++)
	{
		if (m_apIce[i] == nullptr)
			continue;

		CIce *pIce = m_apIce[i];

		float fDiff = 0.0f;

		if (universal::DistCmpFlat(pos, m_apIce[i]->GetPosition(), fDistMin, &fDiff))
		{
			fDistMin = fDiff;

			pIceNearest = pIce;

			if (pIdxV != nullptr && pIdxH != nullptr)
			{
				pIceMgr->GetIceIndex(pIceNearest, pIdxV, pIdxH);
			}
		}
	}
}

//=====================================================
// `æ
//=====================================================
void CFlowIce::Draw(void)
{

}