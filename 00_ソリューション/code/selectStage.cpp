//*****************************************************
//
// �Q�[������[SelectStage.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "selectStage.h"
#include "object.h"
#include "camera.h"
#include "light.h"
#include "manager.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "skybox.h"
#include "selectStageManager.h"

//*****************************************************
// �}�N����`
//*****************************************************
namespace
{

}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************

//=====================================================
// �R���X�g���N�^
//=====================================================
CSelectStage::CSelectStage()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CSelectStage::Init(void)
{
	// �e�N���X�̏�����
	CScene::Init();

	// �X�e�[�W�I���Ǘ��N���X�̐���
	CSelectStageManager::Create();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSelectStage::Uninit(void)
{
	// �I�u�W�F�N�g�S��
	CObject::ReleaseAll();

	CScene::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CSelectStage::Update(void)
{
	// �V�[���̍X�V
	CScene::Update();

	// �J�����X�V
	UpdateCamera();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �J�����̍X�V
//=====================================================
void CSelectStage::UpdateCamera(void)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	pCamera->Update();
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CSelectStage::Debug(void)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
	{
		return;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CSelectStage::Draw(void)
{

}