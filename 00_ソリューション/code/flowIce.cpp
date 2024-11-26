//*****************************************************
//
// ¬XΜ[flowIce.cpp]
// Author:ϋόRη
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

//*****************************************************
// θθ`
//*****************************************************
namespace
{
const float TIME_DELETE = 20.0f;	// XͺΑ¦ιάΕΜΤ
}

//*****************************************************
// ΓIoΟιΎ
//*****************************************************
vector<CFlowIce*> CFlowIce::s_vector;	// i[pΜzρ

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
// Ά¬
//=====================================================
CFlowIce *CFlowIce::Create(void)
{
	CFlowIce* pFlowice = nullptr;

	pFlowice = new CFlowIce;

	if (pFlowice == nullptr)
		return nullptr;

	pFlowice->Init();

	return pFlowice;
}

//=====================================================
// ϊ»
//=====================================================
HRESULT CFlowIce::Init(void)
{
	return S_OK;
}

//=====================================================
// XπzρΙΗΑ
//=====================================================
void CFlowIce::AddIceToArray(CIce *pIce)
{
	m_apIce.push_back(pIce);
}

//=====================================================
// IΉ
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
	// Ηκ©ΜXͺ~άΑΔ’Θ’©Μ`FbN
	CheckSomeIceStop();

	if (!m_bInScrnAllIce)
		CheckInAllIce();	// SΔΜXͺfΑ½©Μ»θ
	else
		CheckDelete();	// νmF

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// SΔΜXͺζΚΙfΑ½©Μ`FbN
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

		// ζΚΰ»θ
		D3DXVECTOR3 pos = m_apIce[i]->GetPosition();
		if (universal::IsInScreen(pos, nullptr))
		{// Ηκ©κΒΕΰζΚΰΙ κΞtOπ§Δι
			m_bInScrnAllIce = true;
		}
	}
}

//=====================================================
// Ηκ©ΜXͺ~άΑΔ’Θ’©Μ`FbN
//=====================================================
void CFlowIce::CheckSomeIceStop(void)
{
	for (auto it : m_apIce)
	{
		if (it->IsStop())
		{// ~άΑΔ’ιΘηSΔΜXπ~ίι
			StopAllIce();
			Uninit();	// ©gΜjό
			break;
		}
	}
}

//=====================================================
// SΔΜXπ~ίι
//=====================================================
void CFlowIce::StopAllIce(void)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	for (auto it : m_apIce)
	{
		it->ChangeState(new CIceStaeteNormal);

		pIceManager->AddIce(it, it->GetPosition());
	}

	CSound::GetInstance()->Play(CSound::LABEL_SE_ICE_UNION);
}

//=====================================================
// γΙ ιΰΜπ~ίι
//=====================================================
void CFlowIce::StopOnTopObject(void)
{

}

//=====================================================
// SXΜν
//=====================================================
void CFlowIce::DeleteAllIce(void)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// vC[ΜζΎ
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	for (int i = 0; i < (int)m_apIce.size(); i++)
	{
		if (m_apIce[i] == nullptr)
			continue;
		
		for (CPlayer *pPlayer : apPlayer)
		{// vC[ͺγΙζΑΔ½ηHit·ι
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
// XνΜmF
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

		// ζΚΰ»θ
		D3DXVECTOR3 pos = m_apIce[i]->GetPosition();
		if (universal::IsInScreen(pos, nullptr))
			return;	// Ηκ©κΒΕΰζΚΰΙ κΞΦπIΉ
	}

	// ±±άΕΚΑ½ηXπν
	DeleteAllIce();
	Uninit();
}

//=====================================================
// fobO
//=====================================================
void CFlowIce::Debug(void)
{
	for (auto it : m_apIce)
	{
		D3DXVECTOR3 posIce = it->GetPosition();

		//CEffect3D::Create(posIce, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

//=====================================================
// `ζ
//=====================================================
void CFlowIce::Draw(void)
{

}