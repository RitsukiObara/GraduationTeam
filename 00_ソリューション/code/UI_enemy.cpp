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
#include "game.h"
#include "inputkeyboard.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_INIT_ICON = { 0.9f,0.08f,0.0f };	// �A�C�R���̏����ʒu
const D3DXVECTOR2 SIZE_INIT_ICON = { 0.02f,0.03f };	// �����̏����T�C�Y
const string PATH_ICON[CEnemy::TYPE_MAX] =			// �A�C�R���̃p�X
{
	"data\\TEXTURE\\UI\\icon_seals.png",		// �A�U���V
	"data\\TEXTURE\\UI\\icon_whitebear.png"		// �V���N�}
};

const int NUM_ROW = 2; // �s�̐�
const int NUM_COLUMN = 5; // ��̐�

const float GRAVITY_ICON = 0.98f;	// �A�C�R���ɂ�����d��
const D3DXVECTOR3 MOVE_STARTFALL_ICON = { 0.0f,-10.0f,0.0f };	// �A�C�R���������n�߂�Ƃ��ɂ�����ړ���
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
	// �A�C�R���̊Ǘ�
	ManageIcon();

#ifdef _DEBUG
	Debug();	// �f�o�b�O����
#endif
}

//=====================================================
// �A�C�R���̊Ǘ�
//=====================================================
void CUIEnemy::ManageIcon(void)
{

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
		AddEnemy(0);
	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN))
	{
		DeleteEnemy(0);
		DeleteEnemy(1);
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
void CUIEnemy::AddEnemy(int nType)
{
	// �A�C�R���𑝂₷
	CIcon *pIcon = CIcon::Create(nType);

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
	pIcon->SetVtx();

	// �e�N�X�`���ݒ�
	int nIdx = Texture::GetIdx(PATH_ICON[nType].c_str());
	pIcon->SetIdxTexture(nIdx);

	m_apIcon.push_back(pIcon);
}

//=====================================================
// �G�̍폜
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
			pIcon->StartFall();	// �������J�n������

			m_apIcon.erase(m_apIcon.begin() + i);

			// �Q�[���̓G�̍ő吔����
			CGame *pGame = CGame::GetInstance();

			if (pGame != nullptr)
				pGame->DecreaseNumEnemy();

			break;
		}
	}

}

//******************************************************************
// �A�C�R���̏���
//******************************************************************
//=====================================================
// ����
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
// ������
//=====================================================
HRESULT CIcon::Init(void)
{
	// �p���N���X�̏�����
	CUI::Init();

	// �T�C�Y�ݒ�
	SetSize(SIZE_INIT_ICON.x, SIZE_INIT_ICON.y);

	return S_OK;
}

//=====================================================
// �X�V
//=====================================================
void CIcon::Update(void)
{
	// �p���N���X�̍X�V
	CUI::Update();

	if (m_state == E_State::STATE_FALL)
		UpdateFall();	// �������̍X�V
}

//=====================================================
// �������n�߂鏈��
//=====================================================
void CIcon::StartFall(void)
{
	// ������Ԃɂ���
	m_state = E_State::STATE_FALL;

	// ������Ɉړ��ʂ𑫂�
	m_move = MOVE_STARTFALL_ICON;
}

//=====================================================
// �������̍X�V
//=====================================================
void CIcon::UpdateFall(void)
{
	m_move.y += GRAVITY_ICON;

	// �ʒu�̉��Z
	Translate(m_move);
	SetVtx();

	// ��ʊO�ɏo����I��
	D3DXVECTOR3 pos = GetPosition();

	if (pos.y > 1.0f + SIZE_INIT_ICON.y * 2)
		Uninit();
}