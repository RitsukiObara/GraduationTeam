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
#include "selectStageManager.h"
#include "enemyfactory.h"
#include "UI_combo.h"
#include "game.h"
#include "fade.h"
#include "camera.h"
#include "cameraState.h"
#include "iceManager.h"
#include "manager.h"
#include "blurEvent.h"
#include "blur.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const string PATH_ENEMY_DEFAULT = "data\\TEXT\\enemy00.txt";		// �G�z�u���̃e�L�X�g
const char* PATH_SAMPLE_ICESTAGE = "data\\TEXT\\ice_stage_00.txt";	// �T���v���̏����z�u
const int SIZE_GRID = 10;											// �X�e�[�W�̃T�C�Y
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CGameManagerSingle::CGameManagerSingle() : m_pPlayer(nullptr), m_pEnemyFct(nullptr)
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CGameManagerSingle::Init(void)
{
	//------------------------------------
	// �X�e�[�W�̐���
	//------------------------------------
	// ���[�h�̎擾
	vector<bool> abFrag;
	CGame::E_GameMode mode;
	gameManager::LoadMode(&mode, abFrag);

	// �X�}�l�[�W���[�̓Ǎ�����
	int nIdxMap = gameManager::LoadIdxMap();

	vector<CSelectStageManager::S_InfoStage*> apInfoStage = CSelectStageManager::GetInfoStage();

	CIceManager* pIceManager = CIceManager::Create(SIZE_GRID, SIZE_GRID);
	if (pIceManager == nullptr)
		return E_FAIL;

	if (apInfoStage.empty() || nIdxMap == -1)
	{// �X�e�[�W��񂪋󂾂�����f�t�H���g�}�b�v
		pIceManager->Load(PATH_SAMPLE_ICESTAGE);
	}
	else						
	{// �Ή������X�e�[�W��Ǎ���
		pIceManager->SetDirStream((COcean::E_Stream)apInfoStage[nIdxMap]->nDirStream);
		pIceManager->SetDirStreamNext((COcean::E_Stream)apInfoStage[nIdxMap]->nDirStream);

		pIceManager->Load(&apInfoStage[nIdxMap]->pathMap[0]);
	}

	//------------------------------------
	// �e��I�u�W�F�N�g����
	//------------------------------------
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

	// �J�����̃X�e�C�g�ݒ�
	Camera::ChangeState(new CFollowPlayer);

	// ���N���X�̏�����
	CGameManager::Init();
	
	m_pEnemyFct = CEnemyFct::Create();

	if (m_pEnemyFct == nullptr)
		return E_FAIL;

	if (apInfoStage.empty() || nIdxMap == -1)
		m_pEnemyFct->Load(PATH_ENEMY_DEFAULT);
	else
		m_pEnemyFct->Load(apInfoStage[nIdxMap]->pathEnemy);

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

	// �J�����̊Ǘ�
	ManageCamera();

	// �v���C���[�Ǘ�
	ManagePlayer();

	// �G�Ǘ�
	ManageEnemy();
}

//=====================================================
// �J�����̊Ǘ�
//=====================================================
void CGameManagerSingle::ManageCamera(void)
{
	
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
	CResultSingle::Create(CResultSingle::RESULT_DEAD);
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

	if (nNumEnemy == 0)	// �G�S�łŏ���
	{
		// ���݂���R���{�̃X�R�A�����Z
		CUI_Combo *pCombo = CUI_Combo::GetInstance();
		if (pCombo != nullptr)
			pCombo->AddComboScore();
			pCombo->Uninit();

		// ���U���g�̐���
		CResultSingle::Create(CResultSingle::RESULT_WIN);
	}
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

//=====================================================
// �Q�[���̏I��
//=====================================================
void CGameManagerSingle::EndGame(void)
{
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
		pFade->SetFade(CScene::MODE_TITLE);
}