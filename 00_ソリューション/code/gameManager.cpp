//*****************************************************
//
// �Q�[���}�l�[�W���[[gameManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameManager.h"
#include "UIManager.h"
#include "skybox.h"
#include "ocean.h"
#include "BG_Ice.h"
#include "camera.h"
#include "cameraState.h"
#include "iceManager.h"
#include "UI_ready.h"
#include "ocean_flow_UI.h"
#include "flowIceFactory.h"

//=====================================================
// �R���X�g���N�^
//=====================================================
CGameManager::CGameManager()
{

}

//=====================================================
// ��������
//=====================================================
CGameManager *CGameManager::Create(void)
{
	CGameManager* pGameManager = nullptr;

	pGameManager = new CGameManager;

	if (pGameManager == nullptr)
		return nullptr;

	pGameManager->Init();

	return pGameManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CGameManager::Init(void)
{
	// UI�}�l�[�W���[�̒ǉ�
	CUIManager::Create();

	// �X�J�C�{�b�N�X�̐���
	CSkybox::Create();

	//�C�̐���
	COcean::Create();

#if 0
	//�w�i�X�̃��[�h
	CBgIce::Load("data\\TEXT\\BG_Ice.txt");

	//�Q�[���X�^�[�g���mUI
	CUIready::Create();
#endif

	// �J�����̃X�e�C�g�ݒ�
	Camera::ChangeState(new CFollowPlayer);

	// ��󃂃f���̐���
	COceanFlowUI::Create();

	// ���X�t�@�N�g���[�̐���
	CFlowIceFct::Create();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGameManager::Uninit(void)
{

}

//=====================================================
// �X�V����
//=====================================================
void CGameManager::Update(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CGameManager::Draw(void)
{

}