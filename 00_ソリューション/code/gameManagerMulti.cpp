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
#include "selectStageManager.h"
#include "iceManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* PATH_SAMPLE_ICESTAGE = "data\\TEXT\\icestagemulti00.txt";	// �T���v���̏����z�u
const int SIZE_GRID = 15;												// �X�e�[�W�̃T�C�Y
const float MAX_SPEED_SUDDONDEATH = 10.0f;								// �T�h���f�X�̊C���̋���
const float ACCELE_SUDDONDEATH = 0.036f;								// �T�h���f�X�̊C���̉�����
const int NUM_PLAYER_SUDDONDEATH = 2;									// �T�h���f�X�ɂȂ�v���C���[�̐�
const int TIME_SUDDONDEATH = 60;										// �T�h���f�X�ɂȂ�c�莞��]
const float TIME_CHANGE_SUDDONDEATH = 10.0f;							// �T�h���f�X�ŊC���̕ς�鑬��

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
	//------------------------------------
	// �X�e�[�W�̐���
	//------------------------------------
	// ���[�h�̎擾
	vector<bool> abFrag;
	CGame::E_GameMode mode;
	gameManager::LoadMode(&mode, abFrag);

	// �}�b�v�ԍ��̓Ǎ�
	int nIdxMap = gameManager::LoadIdxMap();

	vector<CSelectStageManager::S_InfoStage*> apInfoStage = CSelectStageManager::GetInfoStageMulti();

	CIceManager* pIceManager = CIceManager::Create(SIZE_GRID, SIZE_GRID);
	if (pIceManager == nullptr)
		return E_FAIL;

	if (nIdxMap != -1)
	{// �C���̏�����
		pIceManager->SetDirStream((COcean::E_Stream)apInfoStage[nIdxMap]->nDirStream);
		pIceManager->SetDirStreamNext((COcean::E_Stream)apInfoStage[nIdxMap]->nDirStream);
	}

	if (apInfoStage.empty())	// �X�e�[�W��񂪋󂾂�����f�t�H���g�}�b�v
		pIceManager->Load(PATH_SAMPLE_ICESTAGE);
	else
	{// �����_���ŃX�e�[�W��ǂݍ���
		CSelectStageManager::S_InfoStage *pInfoCurrent = universal::RandomFromVector(apInfoStage);

		pIceManager->Load(&pInfoCurrent->pathMap[0]);
	}

	//------------------------------------
	// �e��ݒ�
	//------------------------------------
	// �J�����̃X�e�C�g�ݒ�
	Camera::ChangeState(new CMultiGame);

	for (int i = 0; i < (int)abFrag.size(); i++)
	{
		if (!abFrag[i])
			continue;

		CPlayer *pPlayer = CPlayer::Create();

		if (pPlayer == nullptr)
			continue;

		m_apPlayer.push_back(pPlayer);

		pPlayer->ReLoadModel(&player::PATH_BODY[i][0]);

		pPlayer->ShowNumber();
	}

	// ���N���X�̏�����
	CGameManager::Init();

	// �S�v���C���[�̓��͊��蓖��
	CPlayer::BindInputAllPlayer();

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
void CGameManagerMulti::UpdateNormal(void)
{
	// ���N���X�̍X�V
	CGameManager::UpdateNormal();

	// �v���C���[�Ǘ�
	ManagePlayer();

	// �C���̋����Ǘ�
	ManageLevelOcean();

	// �Q�[���I���̊m�F
	CheckEndGame();
}

//=====================================================
// �C���̋����̊Ǘ�
//=====================================================
void CGameManagerMulti::ManageLevelOcean(void)
{
	// �C���𑬂��������
	bool bAccele = false;

	// ��������
	bAccele = JudgeAccele();

	if (bAccele)
	{// �C���̋�����ݒ�
		COcean *pOcean = COcean::GetInstance();

		if (pOcean != nullptr)
		{
			pOcean->SetMaxSpeed(MAX_SPEED_SUDDONDEATH);
			pOcean->SetAccele(ACCELE_SUDDONDEATH);
			pOcean->SetBaseTimeChangeRot(TIME_CHANGE_SUDDONDEATH);
		}
	}
}

//=====================================================
// �������锻��
//=====================================================
bool CGameManagerMulti::JudgeAccele(void)
{
	// �v���C���[��2�l�ɂȂ�����
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	if (apPlayer.size() <= NUM_PLAYER_SUDDONDEATH)
		return true;

	// �c�莞�Ԃ����Ȃ��Ȃ�����
	CGame *pGame = CGame::GetInstance();

	if (pGame != nullptr)
	{
		int nTime = pGame->GetTimeSecond();

		if(nTime <= TIME_SUDDONDEATH)
			return true;
	}

	// ���̏����ɂ��ʂ�Ȃ�������������Ȃ�
	return false;
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