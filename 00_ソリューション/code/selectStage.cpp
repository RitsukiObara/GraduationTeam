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
#include "ice.h"
#include "sound.h"

//*****************************************************
// �}�N����`
//*****************************************************
namespace
{
	const char* BG_ICE = { "data\\TEXT\\selectstageIce.txt" };		// �X�z�u�e�L�X�g
	const float RADIUS_ICE = 400.0f;	// �X�̔��a
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

	// �X�̓ǂݍ���
	Load(BG_ICE);

	// BGM�Đ�
	Sound::Play(CSound::LABEL::LABEL_BGM_SELECTSTAGE);
	
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
		return;

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

//=====================================================
// �Ǎ�����
//=====================================================
void CSelectStage::Load(const char* pPath)
{
	//�ϐ��錾
	char cTemp[MAX_STRING];

	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	//�t�@�C������ǂݍ���
	FILE* pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ
		while (true)
		{
			//�����ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "SET") == 0)
			{//�L�[�X�^�[�g
				while (strcmp(cTemp, "END_SET") != 0)
				{//�I���܂ŃL�[�ݒ�

					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "POS") == 0)
					{//�ʒu�擾
						(void)fscanf(pFile, "%s", &cTemp[0]);

						for (int nCntPos = 0; nCntPos < 3; nCntPos++)
						{
							(void)fscanf(pFile, "%f", &pos[nCntPos]);
						}
					}

					if (strcmp(cTemp, "ROT") == 0)
					{//�����擾
						(void)fscanf(pFile, "%s", &cTemp[0]);

						for (int nCntRot = 0; nCntRot < 3; nCntRot++)
						{
							(void)fscanf(pFile, "%f", &rot[nCntRot]);
						}
					}
				}

				// �X�̐���
				CIce* pIce = CIce::Create(CIce::TYPE_NORMAL, CIce::STATE_NORMAL);
				if (pIce == nullptr)
					return;

				// �ʒu�Z�b�g
				pIce->SetPosition(pos);

				// �����Z�b�g
				pIce->SetRotation(rot);

				// �T�C�Y�Z�b�g
				pIce->SetTransform(RADIUS_ICE);
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
				break;
		}//while
	}//file
	else
	{
		assert(("SelectStageIce�ǂݍ��݂Ɏ��s", false));
	}

	fclose(pFile);
}