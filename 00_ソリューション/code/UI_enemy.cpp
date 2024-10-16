//*****************************************************
//
// G\¦ÌUI[UIEnemy.cpp]
// Author:ûüRç
//
//*****************************************************

//*****************************************************
// CN[h
//*****************************************************
#include "UI_enemy.h"
#include "number.h"
#include "enemy.h"
#include "UI.h"
#include "texture.h"
#include "inputkeyboard.h"

//*****************************************************
// èè`
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_INIT_ICON = { 0.9f,0.08f,0.0f };	// ACRÌúÊu
const D3DXVECTOR2 SIZE_INIT_ICON = { 0.02f,0.03f };	// ÌúTCY
const string PATH_ICON = "data\\TEXTURE\\UI\\icon_seal.png";	// ACRÌpX

const int NUM_ROW = 2; // sÌ
const int NUM_COLUMN = 5; // ñÌ
}

//*****************************************************
// ÃIoÏé¾
//*****************************************************
CUIEnemy *CUIEnemy::s_pUIEnemy = nullptr;	// ©gÌ|C^

//=====================================================
// RXgN^
//=====================================================
CUIEnemy::CUIEnemy()
{

}

//=====================================================
// fXgN^
//=====================================================
CUIEnemy::~CUIEnemy()
{

}

//=====================================================
// ¶¬
//=====================================================
CUIEnemy *CUIEnemy::Create(void)
{
	if (s_pUIEnemy == nullptr)
	{// CX^X¶¬
		s_pUIEnemy = new CUIEnemy;

		if (s_pUIEnemy == nullptr)
			return nullptr;

		s_pUIEnemy->Init();
	}

	return s_pUIEnemy;
}

//=====================================================
// ú»
//=====================================================
HRESULT CUIEnemy::Init(void)
{
	return S_OK;
}

//=====================================================
// I¹
//=====================================================
void CUIEnemy::Uninit(void)
{
	s_pUIEnemy = nullptr;

	Release();
}

//=====================================================
// XV
//=====================================================
void CUIEnemy::Update(void)
{
#ifdef _DEBUG
	Debug();	// fobO
#endif
}

//=====================================================
// fobO
//=====================================================
void CUIEnemy::Debug(void)
{
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	if (pInputKeyboard == nullptr)
		return;

	if (pInputKeyboard->GetTrigger(DIK_UP))
	{
		AddEnemy();
	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN))
	{
		DeleteEnemy();
	}
}

//=====================================================
// `æ
//=====================================================
void CUIEnemy::Draw(void)
{

}

//=====================================================
// GÌÇÁ
//=====================================================
void CUIEnemy::AddEnemy(void)
{
	// ACRðâ·
	CUI *pIcon = CUI::Create();

	if (pIcon == nullptr)
		return;

	// ÊuÔÌvZ
	int nSizeArray = (int)m_apIcon.size();
	int nIdxRow = nSizeArray % NUM_COLUMN;
	int nIdxColumn = nSizeArray / NUM_COLUMN;

	// ÊuÔð½f
	D3DXVECTOR3 posIcon = POS_INIT_ICON;
	posIcon += D3DXVECTOR3( -SIZE_INIT_ICON.x * nIdxRow * 2, SIZE_INIT_ICON.y * nIdxColumn * 2, 0.0f );
	pIcon->SetPosition(posIcon);

	// TCYÝè
	pIcon->SetSize(SIZE_INIT_ICON.x, SIZE_INIT_ICON.y);
	pIcon->SetVtx();

	// eNX`Ýè
	int nIdx = Texture::GetIdx(&PATH_ICON[0]);
	pIcon->SetIdxTexture(nIdx);

	m_apIcon.push_back(pIcon);
}

//=====================================================
// GÌí
//=====================================================
void CUIEnemy::DeleteEnemy(void)
{
	if (m_apIcon.empty())
		return;
	
	int nSizeArray = (int)m_apIcon.size();

	CUI* pIcon = m_apIcon[nSizeArray - 1];

	pIcon->Uninit();

	m_apIcon.erase(m_apIcon.end() - 1);
}