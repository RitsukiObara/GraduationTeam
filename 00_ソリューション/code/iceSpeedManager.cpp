//*****************************************************
//
// ���X������Ă��鏈��[iceSpeedManager.cpp]
// Author:��� ��^
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "iceSpeedManager.h"
#include "ice.h"
#include "iceManager.h"
#include "effect3D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int NUM_ICE = 4;	// �X�̐�
	const std::string ICE_FLOW = "data\\TEXT\\kari.txt";	// �����X�̃p�X
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************


//=====================================================
// �R���X�g���N�^
//=====================================================
CIceSpeedManager::CIceSpeedManager(int nPriority) : CObject(nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CIceSpeedManager::~CIceSpeedManager()
{

}

//=====================================================
// ��������
//=====================================================
CIceSpeedManager* CIceSpeedManager::Create(void)
{
	CIceSpeedManager* pIceSpeedManager = nullptr;

	pIceSpeedManager = new CIceSpeedManager;

	if (pIceSpeedManager == nullptr)
		return nullptr;

	pIceSpeedManager->Init();

	return pIceSpeedManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CIceSpeedManager::Init(void)
{
	// ���X�̐���
	CreateIce();

	// �Ǎ�
	Load((char*)&ICE_FLOW[0]);

	return S_OK;
}

//=====================================================
// �Ǎ�����
//=====================================================
void CIceSpeedManager::Load(char* pPath)
{
	//for (int nCntIce = 0; nCntIce < MAX_ICEFLOW; nCntIce++)
	//{// �p�[�e�B�N�����̔j��
	//	if (m_aMotionInfo[nCntIce].pEvent != nullptr)
	//	{
	//		delete m_aMotionInfo[nCntIce].pEvent;
	//		m_aMotionInfo[nCntIce].pEvent = nullptr;
	//	}
	//}

	ZeroMemory(&m_apIce[0], sizeof(m_apIce));
	m_nNumIce = 0;

	//�ϐ��錾
	char cTemp[MAX_STRING];
	int nCntIce = 0;

	//�t�@�C������ǂݍ���
	FILE* pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{
		// �t�@�C�����J�����ꍇ
		while (true)
		{
			// �����ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			// �X�̑����ǂݍ���
			if (strcmp(cTemp, "NUM_ICE") == 0)
			{
				// "="�ǂݍ��� (����)
				(void)fscanf(pFile, "%s", &cTemp[0]);

				// �X�̑�����ǂݍ���
				(void)fscanf(pFile, "%d", &m_nNumIce);
			}

			// "SET_FLOW_ICE" �u���b�N�J�n
			if (strcmp(cTemp, "SET_FLOW_ICE") == 0)
			{
				int row = 0;
				while (true)
				{
					// �e�L�X�g��ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// "END_SET_FLOW_ICE" �����o������I��
					if (strcmp(cTemp, "END_SET_FLOW_ICE") == 0)
					{
						break;
					}

					// �s�̐��l������
					std::istringstream iss(cTemp);
					int value;
					int col = 0;
					while (iss >> value)
					{
						// ���l��1�̏ꍇ�A�X�𐶐�
						if (value == 1)
						{
							D3DXVECTOR3 pos;
							//pos.x = static_cast<float>(row); // X�ʒu
							//pos.y = 0.0f;                   // �Œ�lY
							//pos.z = static_cast<float>(col); // Z�ʒu

							// �X�̈ʒu���Z�b�g
							m_apIce[nCntIce]->pIce->SetPosition(pos);

							// �X�J�E���g����
							nCntIce++;
						}
						col++;
					}
					row++;
				}
			}

			// "END_SCRIPT" �����o������I��
			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
		assert(("�����X�̃f�[�^�ǂݍ��݂Ɏ��s", false));
	}
}

//=====================================================
// �X�̐���
//=====================================================
void CIceSpeedManager::CreateIce(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	int aV[NUM_ICE] =
	{// �c�̃O���b�h�ԍ��̔z��
		3,4,5,6
	};
	int aH[NUM_ICE] =
	{// ���̃O���b�h�ԍ��̔z��
		9,9,9,9
	};

	for (int i = 0; i < NUM_ICE; i++)
	{
		CIce* pIce = pIceManager->CreateIce(aV[i], aH[i]);
		pIce->ChangeState(new CIceStateFlow);
		m_apIceSpeed.push_back(pIce);
	}
}

//=====================================================
// �X��z��ɒǉ�
//=====================================================
void CIceSpeedManager::AddIceToArray(CIce* pIce)
{
	m_apIceSpeed.push_back(pIce);
}

//=====================================================
// �I������
//=====================================================
void CIceSpeedManager::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CIceSpeedManager::Update(void)
{
	// �ǂꂩ�̕X���~�܂��Ă��Ȃ����̃`�F�b�N
	CheckSomeIceStop();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �ǂꂩ�̕X���~�܂��Ă��Ȃ����̃`�F�b�N
//=====================================================
void CIceSpeedManager::CheckSomeIceStop(void)
{
	for (auto it : m_apIceSpeed)
	{
		if (it->IsStop())
		{// �~�܂��Ă���Ȃ�S�Ă̕X���~�߂�
			StopAllIce();
			Uninit();	// ���g�̔j��
			break;
		}
	}
}

//=====================================================
// �S�Ă̕X���~�߂�
//=====================================================
void CIceSpeedManager::StopAllIce(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	for (auto it : m_apIceSpeed)
	{
		it->ChangeState(new CIceStaeteNormal);

		pIceManager->AddIce(it, it->GetPosition());
	}
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CIceSpeedManager::Debug(void)
{
	for (auto it : m_apIceSpeed)
	{
		D3DXVECTOR3 posIce = it->GetPosition();

		CEffect3D::Create(posIce, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CIceSpeedManager::Draw(void)
{

}