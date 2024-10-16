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

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int NUM_PLACE = 2;	// �����̌���
const D3DXVECTOR3 POS_INIT_NUMBER = { 0.9f,0.08f,0.0f };	// �����̏����ʒu
const D3DXVECTOR2 SIZE_INIT_NUMBER = { 0.02f,0.05f };	// �����̏����T�C�Y

const D3DXVECTOR3 POS_INIT_ICON = { 0.83f,0.08f,0.0f };	// �A�C�R���̏����ʒu
const D3DXVECTOR2 SIZE_INIT_ICON = { 0.04f,0.06f };	// �����̏����T�C�Y
const string PATH_ICON = "data\\TEXTURE\\UI\\icon_seal.png";	// �A�C�R���̃p�X
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CUIEnemy *CUIEnemy::m_pUIEnemy = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CUIEnemy::CUIEnemy() : m_pNumber(nullptr), m_pIcon(nullptr)
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
	if (m_pUIEnemy == nullptr)
	{// �C���X�^���X����
		m_pUIEnemy = new CUIEnemy;

		if (m_pUIEnemy == nullptr)
			return nullptr;

		m_pUIEnemy->Init();
	}

	return m_pUIEnemy;
}

//=====================================================
// ����������
//=====================================================
HRESULT CUIEnemy::Init(void)
{
	// �����̐���
	m_pNumber = CNumber::Create(NUM_PLACE, 0);

	if (m_pNumber == nullptr)
		return E_FAIL;

	// �����̏�����
	m_pNumber->SetPosition(POS_INIT_NUMBER);
	m_pNumber->SetSizeAll(SIZE_INIT_NUMBER.x, SIZE_INIT_NUMBER.y);

	// �A�C�R���̐���
	m_pIcon = CUI::Create();

	if (m_pIcon == nullptr)
		return E_FAIL;

	// �A�C�R���̏�����
	m_pIcon->SetPosition(POS_INIT_ICON);
	m_pIcon->SetSize(SIZE_INIT_ICON.x, SIZE_INIT_ICON.y);
	m_pIcon->SetVtx();

	int nIdx = Texture::GetIdx(&PATH_ICON[0]);
	m_pIcon->SetIdxTexture(nIdx);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CUIEnemy::Uninit(void)
{
	m_pUIEnemy = nullptr;

	Object::DeleteObject((CObject**)&m_pNumber);
	Object::DeleteObject((CObject**)&m_pIcon);

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CUIEnemy::Update(void)
{
	// �����̊Ǘ�
	ManageNumber();
}

//=====================================================
// �����̊Ǘ�
//=====================================================
void CUIEnemy::ManageNumber(void)
{
	if (m_pNumber == nullptr)
		return;

	vector<CEnemy*> aEnemy = CEnemy::GetArray();

	int nSize = (int)aEnemy.size();

	m_pNumber->SetValue(nSize);
}

//=====================================================
// �`�揈��
//=====================================================
void CUIEnemy::Draw(void)
{
#ifdef _DEBUG

#endif
}