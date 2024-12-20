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
#include "game.h"
#include "inputkeyboard.h"

//*****************************************************
// èè`
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_INIT_ICON = { 0.9f,0.08f,0.0f };	// ACRÌúÊu
const D3DXVECTOR2 SIZE_INIT_ICON = { 0.02f,0.03f };	// ÌúTCY
const string PATH_ICON[CEnemy::TYPE_MAX] =			// ACRÌpX
{
	"data\\TEXTURE\\UI\\icon_seals.png",		// AUV
	"data\\TEXTURE\\UI\\icon_whitebear.png"		// VN}
};

const int NUM_ROW = 2; // sÌ
const int NUM_COLUMN = 5; // ñÌ

const float GRAVITY_ICON = 0.98f;	// ACRÉ©©édÍ
const D3DXVECTOR3 MOVE_STARTFALL_ICON = { 0.0f,-10.0f,0.0f };	// ACRª¿nßéÆ«É©©éÚ®Ê
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
	// ACRÌÇ
	ManageIcon();

#ifdef _DEBUG
	Debug();	// fobO
#endif
}

//=====================================================
// ACRÌÇ
//=====================================================
void CUIEnemy::ManageIcon(void)
{

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
		AddEnemy(0);
	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN))
	{
		DeleteEnemy(0);
		DeleteEnemy(1);
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
void CUIEnemy::AddEnemy(int nType)
{
	// ACRðâ·
	CIcon *pIcon = CIcon::Create(nType);

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
	pIcon->SetVtx();

	// eNX`Ýè
	int nIdx = Texture::GetIdx(PATH_ICON[nType].c_str());
	pIcon->SetIdxTexture(nIdx);

	m_apIcon.push_back(pIcon);
}

//=====================================================
// GÌí
//=====================================================
void CUIEnemy::DeleteEnemy(int nType)
{
	if (m_apIcon.empty())
		return;
	
	for (int i = 0; i < (int)m_apIcon.size(); i++)
	{
		CIcon *pIcon = m_apIcon[i];

		if (nType == pIcon->GetType())
		{
			pIcon->StartFall();	// ºðJn³¹é

			m_apIcon.erase(m_apIcon.begin() + i);

			// Q[ÌGÌÅå¸­
			CGame *pGame = CGame::GetInstance();

			if (pGame != nullptr)
				pGame->DecreaseNumEnemy();

			break;
		}
	}

}

//******************************************************************
// ACRÌ
//******************************************************************
//=====================================================
// ¶¬
//=====================================================
CIcon *CIcon::Create(int nType)
{
	CIcon *pIcon = new CIcon;

	if (pIcon == nullptr)
		return nullptr;

	pIcon->m_nType = nType;
	pIcon->Init();

	return pIcon;
}

//=====================================================
// ú»
//=====================================================
HRESULT CIcon::Init(void)
{
	// p³NXÌú»
	CUI::Init();

	// TCYÝè
	SetSize(SIZE_INIT_ICON.x, SIZE_INIT_ICON.y);

	return S_OK;
}

//=====================================================
// XV
//=====================================================
void CIcon::Update(void)
{
	// p³NXÌXV
	CUI::Update();

	if (m_state == E_State::STATE_FALL)
		UpdateFall();	// ºÌXV
}

//=====================================================
// ºµnßé
//=====================================================
void CIcon::StartFall(void)
{
	// ºóÔÉ·é
	m_state = E_State::STATE_FALL;

	// ãûüÉÚ®Êð«·
	m_move = MOVE_STARTFALL_ICON;
}

//=====================================================
// ºÌXV
//=====================================================
void CIcon::UpdateFall(void)
{
	m_move.y += GRAVITY_ICON;

	// ÊuÌÁZ
	Translate(m_move);
	SetVtx();

	// æÊOÉo½çI¹
	D3DXVECTOR3 pos = GetPosition();

	if (pos.y > 1.0f + SIZE_INIT_ICON.y * 2)
		Uninit();
}