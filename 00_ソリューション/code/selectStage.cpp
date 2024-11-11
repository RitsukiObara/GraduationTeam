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
const char* PATH_SelectStage_ROAD = "data\\MAP\\road00.bin";	// �Q�[�����b�V�����[�h�̃p�X
const int NUM_LIGHT = 3;	// ���C�g�̐�
const D3DXCOLOR COL_LIGHT_DEFAULT = { 0.9f,0.9f,0.9f,1.0f };	// ���C�g�̃f�t�H���g�F
const float SPEED_CHANGE_LIGHTCOL = 0.1f;	// ���C�g�̐F���ς�鑬�x
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

	// ���C�g�̐���
	CreateLight();

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
// ���C�g�̐���
//=====================================================
void CSelectStage::CreateLight(void)
{
	D3DXVECTOR3 aDir[NUM_LIGHT] =
	{
		{ -1.4f, 0.24f, -2.21f, },
		{ 1.42f, -0.8f, 0.08f },
		{ -0.29f, -0.8f, 0.55f }
	};

	for (int i = 0; i < NUM_LIGHT; i++)
	{
		CLight *pLight = CLight::Create();

		if (pLight == nullptr)
			continue;

		D3DLIGHT9 infoLight = pLight->GetLightInfo();

		infoLight.Type = D3DLIGHT_DIRECTIONAL;
		infoLight.Diffuse = COL_LIGHT_DEFAULT;

		D3DXVECTOR3 vecDir = aDir[i];
		D3DXVec3Normalize(&vecDir, &vecDir);		//�x�N�g�����K��
		infoLight.Direction = vecDir;
		
		pLight->SetLightInfo(infoLight);
	}
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