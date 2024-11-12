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

//=====================================================
// �R���X�g���N�^
//=====================================================
CGameManagerMulti::CGameManagerMulti()
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
		m_apPlayer.push_back(pPlayer);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGameManagerMulti::Uninit(void)
{
	// ���N���X�̏I��
	CGameManager::Uninit();
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
// �`�揈��
//=====================================================
void CGameManagerMulti::Draw(void)
{
	// ���N���X�̕`��
	CGameManager::Draw();
}