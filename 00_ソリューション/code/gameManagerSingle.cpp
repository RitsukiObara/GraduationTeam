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
#include "resultSingle.h"
#include "game.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int NUM_ENEMY_DEFAULT = 5;	// �G�̐��̃f�t�H���g�l
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CGameManagerSingle::CGameManagerSingle() : m_pPlayer(nullptr)
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CGameManagerSingle::Init(void)
{
	// �G���\��UI����
	CUIEnemy::Create();

	// �v���C���[����
	m_pPlayer = CPlayer::Create();

	if (m_pPlayer != nullptr)
	{
		// ���̓}�l�[�W���[�̊��蓖��
		CInputManager *pInpuMgr = CInputManager::Create();
		m_pPlayer->BindInputMgr(pInpuMgr);
	}

	// �X�R�A�̐���
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
		pGame->CreateScore();

	// ���N���X�̏�����
	CGameManager::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGameManagerSingle::Uninit(void)
{
	// ���N���X�̏I��
	CGameManager::Uninit();

	m_pPlayer = nullptr;
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
// �J�n��Ԃ̍X�V
//=====================================================
void CGameManagerSingle::UpdateStart(void)
{
	// ���N���X�̍X�V
	CGameManager::UpdateStart();
}

//=====================================================
// �ʏ��Ԃ̍X�V
//=====================================================
void CGameManagerSingle::UpdateNormal(void)
{
	// ���N���X�̍X�V
	CGameManager::UpdateNormal();

	// �v���C���[�Ǘ�
	ManagePlayer();

	// �G�Ǘ�
	ManageEnemy();
}

//=====================================================
// �v���C���[�̊Ǘ�
//=====================================================
void CGameManagerSingle::ManagePlayer(void)
{
	if (m_pPlayer == nullptr)
		return;

	CPlayer::E_State state = m_pPlayer->GetState();

	if (state == CPlayer::E_State::STATE_DEATH)
		DeathPlayer();	// �v���C���[�̎��S
}

//=====================================================
// �v���C���[�̎��S
//=====================================================
void CGameManagerSingle::DeathPlayer(void)
{
	if (m_pPlayer == nullptr)
		return;

	m_pPlayer->Uninit();
	m_pPlayer = nullptr;

	// �v���C���[���S�Ŕs�k
	CResultSingle::Create(false);
}

//=====================================================
// �G�̊Ǘ�
//=====================================================
void CGameManagerSingle::ManageEnemy(void)
{
	CGame *pGame = CGame::GetInstance();

	if (pGame == nullptr)
		return;

	int nNumEnemy = pGame->GetNumEnemyMax();

	if(nNumEnemy == 0)	// �G�S�łŏ���
		CResultSingle::Create(true);
}

//=====================================================
// ���U���g��Ԃ̍X�V
//=====================================================
void CGameManagerSingle::UpdateResult(void)
{
	// ���N���X�̍X�V
	CGameManager::UpdateResult();
}

//=====================================================
// �I����Ԃ̍X�V
//=====================================================
void CGameManagerSingle::UpdateEnd(void)
{
	// ���N���X�̍X�V
	CGameManager::UpdateEnd();
}

//=====================================================
// �`�揈��
//=====================================================
void CGameManagerSingle::Draw(void)
{
	// ���N���X�̕`��
	CGameManager::Draw();
}