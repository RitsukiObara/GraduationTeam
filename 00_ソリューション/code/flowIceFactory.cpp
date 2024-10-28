//*****************************************************
//
// ���X�t�@�N�g���[[flowIceFactory.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "flowIceFactory.h"
#include "iceManager.h"
#include "ice.h"
#include "flowIce.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const string PATH_TEXT = "data\\TEXT\\flowIce.txt";	// ���X���̃t�@�C���p�X

const float TIME_CREATE_FLOWICE = 5.0f;	// ���X����鎞��
const int NUM_ICE = 4;	// �K���ȏ����A�C�X��
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CFlowIceFct::CFlowIceFct(int nPriority) : CObject(nPriority), m_fTimerCreateFlowIce(0.0f)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CFlowIceFct::~CFlowIceFct()
{

}

//=====================================================
// ��������
//=====================================================
CFlowIceFct *CFlowIceFct::Create(void)
{
	CFlowIceFct *pFct = nullptr;

	pFct = new CFlowIceFct;

	if (pFct != nullptr)
	{
		pFct->Init();
	}

	return pFct;
}

//=====================================================
// ����������
//=====================================================
HRESULT CFlowIceFct::Init(void)
{
	// �Ǎ�����
	Load();

	return S_OK;
}

//=====================================================
// �Ǎ�
//=====================================================
void CFlowIceFct::Load(void)
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

			if (key == "SETFLOWICE")
			{// ���X���Ǎ��J�n
				// ���X���̐���
				S_InfoFlowIce *pInfoFlowIce = new S_InfoFlowIce;

				if (pInfoFlowIce == nullptr)
					continue;

				// �ő�̍s�������߂�
				int nNumV;
				int nNumH;
				GetNumFlowIce(file, temp, &nNumV, &nNumH, pInfoFlowIce);

				while (std::getline(file, temp))
				{
					std::istringstream iss(temp);
					std::string key;
					iss >> key;

					for (int i = 0; i < (int)temp.length(); i++)
					{
						char cData = temp[i];


					}

					//if (key == "TYPE")
					//{// ���
					//	int nType;
					//	iss >> temp >> nType;
					//	lightInfo.Type = (D3DLIGHTTYPE)nType;
					//}

					if (key == "END_SETFLOWICE")
					{// �I��
						if (pInfoFlowIce != nullptr)
							m_apInfoFlowIce.push_back(pInfoFlowIce);

						break;
					}
				}
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
// �X�̏c���̐����擾
//=====================================================
void CFlowIceFct::GetNumFlowIce(std::ifstream& file, string str, int *pNumV, int *pNumH, S_InfoFlowIce* pInfoFlowIce)
{
	if (pInfoFlowIce == nullptr)
		return;

	// �����̃��Z�b�g
	*pNumV = 0;
	*pNumH = 0;

	while (std::getline(file, str))
	{// �Ǎ��J�n
		std::istringstream iss(str);
		std::string key;
		iss >> key;

		if (str.length() == 0)
			continue;	// �ǂݍ��񂾕�������Ȃ�ʂ�Ȃ�

		// �s�̔z��ǉ�
		vector<int> aIdx;
		pInfoFlowIce->aIdx.push_back(aIdx);

		for (int i = 0; i < (int)str.length(); i++)
		{// ������̍s�̒������m
			char cData = str[i];

			if (cData != ' ')
			{// �󔒂���Ȃ��ꍇ�A�s�̒����𑝂₷
				*pNumH += 1;
			}
		}

		for (int i = 0; i < (int)str.length(); i++)
		{// ������𕪉����ĉ��
			char cData = str[i];

			if (cData != ' ')
			{// �󔒂���Ȃ��ꍇ�A�s�̒����𑝂₷
				int nData = cData - '0';

				pInfoFlowIce->aIdx[*pNumV].push_back(nData);
			}
		}

		// �Ȃɂ����當��������΍s���𑝂₷
		*pNumV += 1;

		if (key == "END_SETFLOWICE")
		{// �l��ۑ����ďI��
			break;
		}
	}
}

//=====================================================
// �I������
//=====================================================
void CFlowIceFct::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CFlowIceFct::Update(void)
{
	// �o�ߎ��ԉ��Z
	m_fTimerCreateFlowIce += CManager::GetDeltaTime();

	// ��莞�Ԃŗ��X�𗬂�
	if (m_fTimerCreateFlowIce > TIME_CREATE_FLOWICE)
	{
		// ���X�̐���
		//CreateFlowIce();

		m_fTimerCreateFlowIce = 0.0f;
	}
}

//=====================================================
// ���X�̐���
//=====================================================
void CFlowIceFct::CreateFlowIce(void)
{
	// ���X�V�X�e���̐���
	CFlowIce *pFlowIce = CFlowIce::Create();

	if (pFlowIce == nullptr)
		return;

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// �X�𐶐����A�V�X�e���ɒǉ�
	int nNumGridV = pIceMgr->GetNumGridV();
	int nNumGridH = pIceMgr->GetNumGridH();

	CIceManager *pIceManager = CIceManager::GetInstance();

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
		// ��������ꏊ�Ɋ��ɕX������ꍇ�́A�X�L�b�v
		if (pIceManager->GetGridIce(&aV[i], &aH[i]) != nullptr)
			continue;

		// �X�𐶐����A���X�V�X�e���ɒǉ�
		CIce *pIce = pIceManager->CreateIce(aV[i], aH[i]);

		if (pIce == nullptr)
			continue;

		pIce->ChangeState(new CIceStateFlow);
		pFlowIce->AddIceToArray(pIce);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CFlowIceFct::Draw(void)
{

}