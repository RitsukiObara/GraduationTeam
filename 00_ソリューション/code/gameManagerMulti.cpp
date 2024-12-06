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
#include "fade.h"
#include "camera.h"
#include "cameraState.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
}

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
	// �J�����̃X�e�C�g�ݒ�
	Camera::ChangeState(new CMultiGame);

	// ���N���X�̏�����
	CGameManager::Init();

	// ���[�h�̎擾
	vector<bool> abFrag;
	CGame::E_GameMode mode;
	gameManager::LoadMode(&mode, abFrag);

	for (int i = 0; i < (int)abFrag.size(); i++)
	{
		if (!abFrag[i])
			continue;

		CPlayer *pPlayer = CPlayer::Create();

		if (pPlayer == nullptr)
			continue;

		m_apPlayer.push_back(pPlayer);

		pPlayer->ReLoadModel(&player::PATH_BODY[i][0]);
	}

	// �S�v���C���[�̓��͊��蓖��
	CPlayer::BindInputAllPlayer();

	// �v���C���[UI�̐���
	CUIPlayer *pUIPlayer = CUIPlayer::Create();

	if (pUIPlayer != nullptr)
		pUIPlayer->StartScatter();

	CPlayer::EnableInputAll(false);

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
void CGameManagerMulti::UpdateNormal(void)
{
	// ���N���X�̍X�V
	CGameManager::UpdateNormal();

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
	// �v���C���[�������S�������������Q�[���I��
	if ((int)m_apPlayer.size() - 1 <= m_nNumDeathPlayer)
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

//=====================================================
// �Q�[���̏I��
//=====================================================
void CGameManagerMulti::EndGame(void)
{
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
		pFade->SetFade(CScene::MODE_RESULTMULTI);

	// ���ҏ��擾
	int winner = -1;
	for (int cnt = 0; cnt < (int)m_apPlayer.size(); cnt++)
	{
		if(m_apPlayer[cnt] != nullptr)
		{
			winner = cnt;
			break;
		}
	}
	// �Q���l���擾
	int playerNum = m_nNumDeathPlayer + 1;

	// ���ҏ�񏑂��o��
	gameManager::SaveWinner(playerNum, winner);
}