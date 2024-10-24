//*****************************************************
//
// όΝ[input.cpp]
// Author:ϋόRη
//
//*****************************************************

//*****************************************************
// CN[h
//*****************************************************
#include "input.h"

//*****************************************************
// ΓIoΟιΎ
//*****************************************************
LPDIRECTINPUT8 CInput::m_pInput = nullptr;	// DirectInputIuWFNgΦΜ|C^

//=====================================================
// RXgN^
//=====================================================
CInput::CInput()
{
	m_pDevice = nullptr;
}

//=====================================================
// fXgN^
//=====================================================
CInput::~CInput()
{

}

//=====================================================
// ϊ»
//=====================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		//DirectInputIuWFNgΜΆ¬
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=====================================================
// IΉ
//=====================================================
void CInput::Uninit(void)
{
	if (m_pDevice != NULL)
	{//όΝfoCXjό
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	if (m_pInput != NULL)
	{//DirectInputIuWFNgΜjό
		m_pInput->Release();
		m_pInput = NULL;
	}
}
