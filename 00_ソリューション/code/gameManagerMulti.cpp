//*****************************************************
//
// �}���`�Q�[���}�l�[�W���[[gameManagerMulti.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameManagerMulti.h"
#include "inputManager.h"
#include "player.h"
#include "UIplayer.h"

//=====================================================
// �R���X�g���N�^
//=====================================================
CGameManagerMulti::CGameManagerMulti() : m_nNumDeathPlayer(0)
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CGameManagerMulti::Init(void)
{
	// ���N���X�̏�����
	CGameManager::Init();

	// ���[�h�̎擾
	vector<bool> abFrag;
	CGame::E_GameMode mode;
	gameManager::LoadMode(&mode, abFrag);

	for (int i = 0; i < (int)abFrag.size(); i++)
	{
		CInputManager *pInpuMgr = CInputManager::Create();

		if (!abFrag[i])
			continue;

		CPlayer *pPlayer = CPlayer::Create();

		if (pPlayer == nullptr)
			continue;

		pPlayer->BindInputMgr(pInpuMgr);
		pPlayer->SetID(i);
		m_apPlayer.push_back(pPlayer);
	}

	// �v���C���[UI�̐���
	CUIPlayer *pUIPlayer = CUIPlayer::Create();

	if (pUIPlayer != nullptr)
		pUIPlayer->StartScatter();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGameManagerMulti::Uninit(void)
{
	// ���N���X�̏I��
	CGameManager::Uninit();

	m_apPlayer.clear();
}

//=====================================================
// �X�V����
//=====================================================
void CGameManagerMulti::Update(void)
{
	// ���N���X�̍X�V
	CGameManager::Update();
}

//=====================================================
// �J�n��Ԃ̍X�V
//=====================================================
void CGameManagerMulti::UpdateStart(void)
{
	// ���N���X�̍X�V
	CGameManager::UpdateStart();
}

//=====================================================
// �ʏ��Ԃ̍X�V
//=====================================================
void CGameManagerMulti::UpdateMove(void)
{
	// ���N���X�̍X�V
	CGameManager::UpdateMove();

	// �v���C���[�Ǘ�
	ManagePlayer();

	// �Q�[���I���̊m�F
	CheckEndGame();
}

//=====================================================
// �v���C���[�̊Ǘ�
//=====================================================
void CGameManagerMulti::ManagePlayer(void)
{
	for (int i = 0; i < (int)m_apPlayer.size(); i++)
	{
		if (m_apPlayer[i] == nullptr)
			continue;

		if (m_apPlayer[i]->GetState() == CPlayer::E_State::STATE_DEATH)
			DeathPlayer(i);
	}
}

//=====================================================
// �v���C���[�̎��S
//=====================================================
void CGameManagerMulti::DeathPlayer(int nIdx)
{
	m_apPlayer[nIdx]->Uninit();
	m_apPlayer[nIdx] = nullptr;

	m_nNumDeathPlayer++;
}

//=====================================================
// �Q�[���I���̊m�F
//=====================================================
void CGameManagerMulti::CheckEndGame(void)
{
	// �v���C���[���Ǝ��S������v������Q�[���I��
	if ((int)m_apPlayer.size() - 1 == m_nNumDeathPlayer)
	{
		CGame::SetState(CGame::E_State::STATE_END);
	}
}

//=====================================================
// ���U���g��Ԃ̍X�V
//=====================================================
void CGameManagerMulti::UpdateResult(void)
{
	// ���N���X�̍X�V
	CGameManager::UpdateResult();
}

//=====================================================
// �I����Ԃ̍X�V
//=====================================================
void CGameManagerMulti::UpdateEnd(void)
{
	// ���N���X�̍X�V
	CGameManager::UpdateEnd();
}

//=====================================================
// �`�揈��
//=====================================================
void CGameManagerMulti::Draw(void)
{
	// ���N���X�̕`��
	CGameManager::Draw();
}