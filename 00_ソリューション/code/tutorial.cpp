//*****************************************************
//
// �`���[�g���A������[tutorial.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "tutorial.h"
#include "iceManager.h"
#include "camera.h"
#include "cameraState.h"
#include "inputManager.h"
#include "game.h"
#include "gameManager.h"
#include "player.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
//------------------------------
// �X�e�[�W�̒萔
//------------------------------
namespace stage
{
const string PATH_MAP = "data\\TEXT\\ice_stage_tutorial.txt";	// �}�b�v�̃p�X
const int SIZE_MAP = 15;										// �}�b�v�̃T�C�Y
}
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CTutorial::CTutorial()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CTutorial::~CTutorial()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CTutorial::Init(void)
{
	// �V�[���̏�����
	if (FAILED(CScene::Init()))
	{ // �������Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �`���[�g���A���p�}�b�v�̐���
	CIceManager* pIceManager = CIceManager::Create(stage::SIZE_MAP, stage::SIZE_MAP);

	if (pIceManager != nullptr)
		pIceManager->Load(&stage::PATH_MAP[0]);
	
	// �J�����̐ݒ�
	Camera::ChangeState(new CFollowPlayer);

	// �C�̐���
	COcean::Create();
	
	//--------------------------------
	// �v���C���[�̐���
	//--------------------------------
	// ���[�h�̎擾
	vector<bool> abFrag;
	CGame::E_GameMode mode;
	gameManager::LoadMode(&mode, abFrag);

	for (int i = 0; i < (int)abFrag.size(); i++)
	{// �v���C���[�̐���
		CInputManager *pInpuMgr = CInputManager::Create();

		if (!abFrag[i])
			continue;

		CPlayer *pPlayer = CPlayer::Create();

		if (pPlayer == nullptr)
			continue;

		pPlayer->BindInputMgr(pInpuMgr);
		pPlayer->SetID(i);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTutorial::Uninit(void)
{
	// �V�[���̏I��
	CScene::Uninit();

	// �I�u�W�F�N�g�S�j��
	CObject::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CTutorial::Update(void)
{
	// �V�[���̍X�V
	CScene::Update();
}

//=====================================================
// �`�揈��
//=====================================================
void CTutorial::Draw(void)
{
	// �V�[���̕`��
	CScene::Draw();
}