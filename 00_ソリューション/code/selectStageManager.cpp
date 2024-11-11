//*****************************************************
//
// �X�e�[�W�I���Ǘ�����[SelectStageManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "selectStageManager.h"
#include "skybox.h"
#include "camera.h"
#include "cameraState.h"

//*****************************************************
// �}�N����`
//*****************************************************
namespace
{
const string PATH_TEXT = "data\\TEXT\\selectStage.txt";	// �e�L�X�g�p�X
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************

//=====================================================
// �R���X�g���N�^
//=====================================================
CSelectStageManager::CSelectStageManager()
{

}

//=====================================================
// ��������
//=====================================================
CSelectStageManager *CSelectStageManager::Create(void)
{
	CSelectStageManager *pManager = nullptr;

	pManager = new CSelectStageManager;

	if (pManager != nullptr)
	{
		// ����������
		pManager->Init();
	}

	return pManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CSelectStageManager::Init(void)
{
	// �X�J�C�{�b�N�X�̐���
	CSkybox::Create();

	// �J�����̃X�e�C�g�ݒ�
	Camera::ChangeState(new CCameraStateSelectStage);

	// �Ǎ�
	Load();

	// �X�e�[�W�̐ݒu
	SetStage();

	return S_OK;
}

//=====================================================
// �ǂݍ��ݏ���
//=====================================================
void CSelectStageManager::Load(void)
{
	std::ifstream file(PATH_TEXT);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// �ǂݍ��ނ��̂��Ȃ��Ȃ�܂œǍ�
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "SETSTAGE")
			{// �X�e�[�W���Ǎ��J�n
				// �X�e�[�W���̐���
				S_InfoStage *pInfoStage = new S_InfoStage;

				if (pInfoStage == nullptr)
					continue;

				LoadStage(file, temp, pInfoStage);

				m_aInfoStage.push_back(pInfoStage);
			}

			if (file.eof())
			{// �ǂݍ��ݏI��
				break;
			}
		}

		file.close();
	}
	else
	{
		assert(("�t�@�C�����J���܂���ł���", false));
	}
}

//=====================================================
// �X�e�[�W�̓Ǎ�
//=====================================================
void CSelectStageManager::LoadStage(std::ifstream& file, string str, S_InfoStage *pInfoStage)
{
	if (pInfoStage == nullptr)
		return;

	while (std::getline(file, str))
	{// �Ǎ��J�n
		std::istringstream iss(str);
		std::string key;
		iss >> key;

		if (key == "END_SETSTAGE")
		{// �I��
			break;
		}

		if (key == "MODEL")
		{// �p�X
			iss >> str >> pInfoStage->pathModel;
		}

		if (key == "POS")
		{// �ʒu
			iss >> str >> pInfoStage->pos.x >> pInfoStage->pos.y >> pInfoStage->pos.z;
		}
	}
}

//=====================================================
// �X�e�[�W�̐ݒu
//=====================================================
void CSelectStageManager::SetStage(void)
{
	if (m_aInfoStage.empty())
		return;

	for (S_InfoStage *pInfoStage : m_aInfoStage)
	{
		// �X�e�[�W��X���f���̐ݒu
		CObjectX *pObjectX = CObjectX::Create();

		int nIdxModel = CModel::Load(&pInfoStage->pathModel[0]);
		pObjectX->BindModel(nIdxModel);

		pObjectX->SetPosition(pInfoStage->pos);
	}
}

//=====================================================
// �I������
//=====================================================
void CSelectStageManager::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CSelectStageManager::Update(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CSelectStageManager::Draw(void)
{

}