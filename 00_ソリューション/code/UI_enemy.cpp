//*****************************************************
//
// �G���\����UI[UIEnemy.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "UI_enemy.h"
#include "number.h"
#include "enemy.h"
#include "UI.h"
#include "texture.h"
#include "inputkeyboard.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_INIT_ICON = { 0.9f,0.08f,0.0f };	// �A�C�R���̏����ʒu
const D3DXVECTOR2 SIZE_INIT_ICON = { 0.02f,0.03f };	// �����̏����T�C�Y
const string PATH_ICON = "data\\TEXTURE\\UI\\icon_seal.png";	// �A�C�R���̃p�X

const int NUM_ROW = 2; // �s�̐�
const int NUM_COLUMN = 5; // ��̐�
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CUIEnemy *CUIEnemy::s_pUIEnemy = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CUIEnemy::CUIEnemy()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CUIEnemy::~CUIEnemy()
{

}

//=====================================================
// ��������
//=====================================================
CUIEnemy *CUIEnemy::Create(void)
{
	if (s_pUIEnemy == nullptr)
	{// �C���X�^���X����
		s_pUIEnemy = new CUIEnemy;

		if (s_pUIEnemy == nullptr)
			return nullptr;

		s_pUIEnemy->Init();
	}

	return s_pUIEnemy;
}

//=====================================================
// ����������
//=====================================================
HRESULT CUIEnemy::Init(void)
{
	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CUIEnemy::Uninit(void)
{
	s_pUIEnemy = nullptr;

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CUIEnemy::Update(void)
{
#ifdef _DEBUG
	Debug();	// �f�o�b�O����
#endif
}

//=====================================================
// �f�o�b�O����
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
// �`�揈��
//=====================================================
void CUIEnemy::Draw(void)
{

}

//=====================================================
// �G�̒ǉ�
//=====================================================
void CUIEnemy::AddEnemy(void)
{
	// �A�C�R���𑝂₷
	CUI *pIcon = CUI::Create();

	if (pIcon == nullptr)
		return;

	// �ʒu�ԍ��̌v�Z
	int nSizeArray = (int)m_apIcon.size();
	int nIdxRow = nSizeArray % NUM_COLUMN;
	int nIdxColumn = nSizeArray / NUM_COLUMN;

	// �ʒu�ԍ��𔽉f
	D3DXVECTOR3 posIcon = POS_INIT_ICON;
	posIcon += D3DXVECTOR3( -SIZE_INIT_ICON.x * nIdxRow * 2, SIZE_INIT_ICON.y * nIdxColumn * 2, 0.0f );
	pIcon->SetPosition(posIcon);

	// �T�C�Y�ݒ�
	pIcon->SetSize(SIZE_INIT_ICON.x, SIZE_INIT_ICON.y);
	pIcon->SetVtx();

	// �e�N�X�`���ݒ�
	int nIdx = Texture::GetIdx(&PATH_ICON[0]);
	pIcon->SetIdxTexture(nIdx);

	m_apIcon.push_back(pIcon);
}

//=====================================================
// �G�̍폜
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