//*****************************************************
//
// �V���O���Q�[���}�l�[�W���[[gameManagerSingle.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameManagerSingle.h"
#include "enemy.h"
#include "player.h"
#include "inputManager.h"

//=====================================================
// �R���X�g���N�^
//=====================================================
CGameManagerSingle::CGameManagerSingle()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CGameManagerSingle::Init(void)
{
	// ���N���X�̏�����
	CGameManager::Init();

	// �G���\��UI����
	CUIEnemy::Create();

	// �G����
	CEnemy::Create((int)CEnemy::TYPE::TYPE_SEALS);

	// �v���C���[����
	m_pPlayer = CPlayer::Create();

	if (m_pPlayer != nullptr)
	{
		// ���̓}�l�[�W���[�̊��蓖��
		CInputManager *pInpuMgr = CInputManager::Create();
		m_pPlayer->BindInputMgr(pInpuMgr);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGameManagerSingle::Uninit(void)
{
	// ���N���X�̏I��
	CGameManager::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CGameManagerSingle::Update(void)
{
	// ���N���X�̍X�V
	CGameManager::Update();
}

//=====================================================
// �`�揈��
//=====================================================
void CGameManagerSingle::Draw(void)
{
	// ���N���X�̕`��
	CGameManager::Draw();
}