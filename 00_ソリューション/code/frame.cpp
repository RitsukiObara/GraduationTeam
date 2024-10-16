//*****************************************************
//
// FrameΜ[Frame.cpp]
// Author:ϋόRη
//
//*****************************************************

//*****************************************************
// CN[h
//*****************************************************
#include "frame.h"
#include "polygon2D.h"
#include "UIManager.h"

//*****************************************************
// }Nθ`
//*****************************************************
#define FRAME_HEIGHT	(50.0f)	// t[Μ³

//=====================================================
// DζΚπίιRXgN^
//=====================================================
CFrame::CFrame(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// fXgN^
//=====================================================
CFrame::~CFrame()
{

}

//=====================================================
// Ά¬
//=====================================================
CFrame *CFrame::Create(int nTimeIn, int nTimeWait, int nTimeOut)
{
	CFrame *pFrame = nullptr;

	if (pFrame == nullptr)
	{
		// CX^XΆ¬
		pFrame = new CFrame;

		if (pFrame != nullptr)
		{
			// ψσ―ζθ
			pFrame->m_info.nTimeIn = nTimeIn;
			pFrame->m_info.nTimeWait = nTimeWait;
			pFrame->m_info.nTimeOut = nTimeOut;

			// ϊ»
			pFrame->Init();
		}
	}

	return pFrame;
}

//=====================================================
// ϊ»
//=====================================================
HRESULT CFrame::Init(void)
{
	// UIπρ\¦Ι·ι
	CUIManager *pUIManager = CUIManager::GetInstance();

	m_info.state = STATE_IN;

	if (pUIManager != nullptr)
	{
		pUIManager->EnableDisp(false);
	}

	// |SΜΆ¬
	if (m_info.pUpper == nullptr)
	{
		m_info.pUpper = CPolygon2D::Create(6);

		if (m_info.pUpper != nullptr)
		{
			m_info.pUpper->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f));
			m_info.pUpper->SetSize(SCREEN_WIDTH * 0.5f, 0.0f);
			m_info.pUpper->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			m_info.pUpper->SetVtx();
		}
	}

	if (m_info.pUnder == nullptr)
	{
		m_info.pUnder = CPolygon2D::Create(6);

		if (m_info.pUnder != nullptr)
		{
			m_info.pUnder->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT, 0.0f));
			m_info.pUnder->SetSize(SCREEN_WIDTH * 0.5f, 0.0f);
			m_info.pUnder->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			m_info.pUnder->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// IΉ
//=====================================================
void CFrame::Uninit(void)
{
	// UIπ\¦·ι
	CUIManager *pUIManager = CUIManager::GetInstance();

	if (pUIManager != nullptr)
	{
		pUIManager->EnableDisp(true);
	}

	// |SΜjό
	if (m_info.pUnder != nullptr)
	{
		m_info.pUnder->Uninit();
		m_info.pUnder = nullptr;
	}

	if (m_info.pUpper != nullptr)
	{
		m_info.pUpper->Uninit();
		m_info.pUpper = nullptr;
	}

	// ©gΜjό
	Release();
}

//=====================================================
// XV
//=====================================================
void CFrame::Update(void)
{
	// σΤΜΗ
	ManageState();

	if (m_info.state != STATE_WAIT)
	{
		// γ€ΜΗ
		ManageUpper();

		// Ί€ΜΗ
		ManageUnder();
	}
}

//=====================================================
// σΤΜΗ
//=====================================================
void CFrame::ManageState(void)
{
	switch (m_info.state)
	{
	case STATE_IN:

		m_info.nCntFade++;

		if (m_info.nTimeIn <= m_info.nCntFade)
		{
			m_info.state = STATE_WAIT;

			m_info.nCntFade = 0;
		}

		break;
	case STATE_WAIT:

		m_info.nCntFade++;

		if (m_info.nTimeWait <= m_info.nCntFade)
		{
			m_info.state = STATE_OUT;

			m_info.nCntFade = m_info.nTimeOut;
		}

		break; 
	case STATE_OUT:

		m_info.nCntFade--;

		if (0 >= m_info.nCntFade)
		{// IΉ
			Uninit();
		}

		break;
	default:
		break;
	}
}

//=====================================================
// γ€ΜΗ
//=====================================================
void CFrame::ManageUpper(void)
{
	if (m_info.pUpper == nullptr)
	{
		return;
	}

	int nTime = 0;

	switch (m_info.state)
	{
	case STATE_IN:

		nTime = m_info.nTimeIn;

		break;
	case STATE_OUT:

		nTime = m_info.nTimeOut;

		break;
	default:
		break;
	}

	float fRate = (float)m_info.nCntFade / (float)nTime;

	float fHeight = fRate * FRAME_HEIGHT;

	// Κuέθ
	D3DXVECTOR3 pos = { SCREEN_WIDTH * 0.5f,fHeight,0.0f };

	// TCYέθ
	m_info.pUpper->SetPosition(pos);
	m_info.pUpper->SetSize(SCREEN_WIDTH * 0.5f, fHeight);
	m_info.pUpper->SetVtx();
}

//=====================================================
// Ί€ΜΗ
//=====================================================
void CFrame::ManageUnder(void)
{
	if (m_info.pUnder == nullptr)
	{
		return;
	}

	int nTime = 0;

	switch (m_info.state)
	{
	case STATE_IN:

		nTime = m_info.nTimeIn;

		break;
	case STATE_OUT:

		nTime = m_info.nTimeOut;

		break;
	default:
		break;
	}

	float fRate = (float)m_info.nCntFade / (float)nTime;

	float fHeight = fRate * FRAME_HEIGHT;

	// Κuέθ
	D3DXVECTOR3 pos = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT - fHeight,0.0f };

	// TCYέθ
	m_info.pUnder->SetPosition(pos);
	m_info.pUnder->SetSize(SCREEN_WIDTH * 0.5f, fHeight);
	m_info.pUnder->SetVtx();
}

//=====================================================
// `ζ
//=====================================================
void CFrame::Draw(void)
{

}