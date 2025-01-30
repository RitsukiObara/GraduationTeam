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
#include "game.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const string PATH_TEXT = "data\\TEXT\\flowIce.txt";	// ���X���̃t�@�C���p�X

const float TIME_CREATE_FLOWICE = 15.0f;					// ���X����鎞��
const float TIME_DIFF_CREATE = TIME_CREATE_FLOWICE * 0.7f;	// ���X������Ă��鎞�Ԃ̍���

const int ADD_CREATE_FLOWICE = 8;	// ���X�̐�������ʒu�̑��₷�O���b�h��
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

				m_apInfoFlowIce.push_back(pInfoFlowIce);
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

		if (key == "END_SETFLOWICE")
		{// �I��
			break;
		}

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
	// �Q�[�����ɂ̒ʏ��Ԃ̂ݍX�V���ʂ�
	if (CGame::GetInstance() != nullptr && 
		CGame::GetState() != CGame::E_State::STATE_NORMAL)
		return;

	// �o�ߎ��ԉ��Z
	m_fTimerCreateFlowIce += CManager::GetDeltaTime();

	// �X�̐��̊����v�Z
	CIceManager *pIceMgr = CIceManager::GetInstance();
	if (pIceMgr == nullptr)
		return;

	float fRate = pIceMgr->GetRateNumIce();

	// ��莞�Ԃŗ��X�𗬂�
	if (m_fTimerCreateFlowIce > TIME_CREATE_FLOWICE - TIME_DIFF_CREATE * (1.0f - fRate))
	{
		// ���X�̐���
		CreateFlowIce();

		// ���̕X�̌`�����߂�
		DecideNextFlowIce();

		// �J�E���g���Z�b�g
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

	// ��������ꏊ���O���b�h�̒[���炸�炷
	int nNumGridV = pIceMgr->GetNumGridV();
	int nNumGridH = pIceMgr->GetNumGridH();

	// �����f�t�H���g�ʒu�̐ݒ�
	DecideDefaultGridSpawnIce(nNumGridV, nNumGridH);

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	S_InfoFlowIce *pInfo = m_apInfoFlowIce[m_nIdxNextIce];

	for (int i = 0; i < (int)pInfo->aIdx.size(); i++)
	{
		for (int j = 0; j < (int)pInfo->aIdx[i].size(); j++)
		{
			if (pInfo->aIdx[i][j] != 0)
			{// �X�𐶐�����}�X�Ȃ�X�𐶐�
				// �X�𐶐����A���X�V�X�e���ɒǉ�
				CIce *pIce = pIceManager->CreateFlowIce(nNumGridV + (int)pInfo->aIdx.size() - i - 1, nNumGridH + j);

				if (pIce == nullptr)
					continue;

				pIce->ChangeState(new CIceStateFlow);
				pFlowIce->AddIceToArray(pIce);
			}
		}
	}
}

//=====================================================
// �O���b�h�̃X�|�[���O���b�h�̃f�t�H���g����
//=====================================================
void CFlowIceFct::DecideDefaultGridSpawnIce(int &nGridV, int &nGridH)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	COcean::E_Stream dirStream = pIceMgr->GetDirStream();

	int nMaxV = m_apInfoFlowIce[m_nIdxNextIce]->aIdx.size();
	int nMaxH = 0;

	for (int i = 0; i < nMaxV; i++)
	{// ���̍ő吔
		int nSizeH = m_apInfoFlowIce[m_nIdxNextIce]->aIdx[i].size();

		if (nMaxH < nSizeH)
			nMaxH = nSizeH;
	}

	switch (dirStream)
	{
	case COcean::E_Stream::STREAM_UP:
		nGridV = 0;
		nGridV -= ADD_CREATE_FLOWICE + nMaxV;
		nGridH = universal::RandRange(nGridH - nMaxH, 0);
		break;
	case COcean::E_Stream::STREAM_RIGHT:
		nGridH = 0;
		nGridH -= ADD_CREATE_FLOWICE + nMaxH;
		nGridV = universal::RandRange(nGridV - nMaxV, 0);
		break;
	case COcean::E_Stream::STREAM_DOWN:
		nGridV += ADD_CREATE_FLOWICE;
		nGridH = universal::RandRange(nGridH - nMaxH, 0);
		break;
	case COcean::E_Stream::STREAM_LEFT:
		nGridH += ADD_CREATE_FLOWICE;
		nGridV = universal::RandRange(nGridV - nMaxV, 0);
		break;
	default:
		break;
	}
}

//=====================================================
// ���̗��X�̌`�����߂�
//=====================================================
void CFlowIceFct::DecideNextFlowIce(void)
{
	if (m_apInfoFlowIce.empty())
		return;

	// �z��̃T�C�Y���猈�߂�
	int nMax = (int)m_apInfoFlowIce.size();

	m_nIdxNextIce = universal::RandRange(nMax, 0);
}

//=====================================================
// �`�揈��
//=====================================================
void CFlowIceFct::Draw(void)
{

}