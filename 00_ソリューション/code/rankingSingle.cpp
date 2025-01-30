//*****************************************************
//
// �V���O���̃����L���O����[rankingSingle.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "rankingSingle.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int NUM_DATA_DEFAULT = 3;	// �f�t�H���g�̃f�[�^��
}

//====================================================
// �R���X�g���N�^
//====================================================
CRankingSingle::CRankingSingle()
{

}

//====================================================
// �f�X�g���N�^
//====================================================
CRankingSingle::~CRankingSingle()
{

}

//====================================================
// ��������
//====================================================
CRankingSingle *CRankingSingle::Create(string pathSave)
{
	CRankingSingle *pRanking = nullptr;

	pRanking = new CRankingSingle;

	if (pRanking != nullptr)
	{
		pRanking->m_pathSave = pathSave;
		pRanking->Init();
	}

	return pRanking;
}

//====================================================
// ����������
//====================================================
HRESULT CRankingSingle::Init(void)
{
#if 0
	Reset();	// ���Z�b�g����

	Save();
#endif

	Load();

	return S_OK;
}

//====================================================
// ���Z�b�g
//====================================================
void CRankingSingle::Reset(void)
{
	int aScore[NUM_DATA_DEFAULT] = { 500,200,100 };
	int nTimePeck[NUM_DATA_DEFAULT] = { 50, 80, 90 };

	for (int i = 0; i < NUM_DATA_DEFAULT; i++)
	{
		S_InfoRank *pInfoRank = new S_InfoRank;

		if (pInfoRank == nullptr)
			continue;
		
		pInfoRank->nScore = aScore[i];
		pInfoRank->nTimePeck = nTimePeck[i];

		m_apRank.push_back(pInfoRank);
	}
}

//====================================================
// �z��̉��
//====================================================
void CRankingSingle::ReleaseArray(void)
{
	for (int i = 0; i < (int)m_apRank.size(); i++)
	{
		if (m_apRank[i] == nullptr)
			continue;

		delete m_apRank[i];
		m_apRank[i] = nullptr;
	}

	m_apRank.clear();
}

//====================================================
// �\�[�g
//====================================================
void CRankingSingle::Sort(void)
{
	// �~���Ƀ\�[�g (value�����)
	std::sort(m_apRank.begin(), m_apRank.end(), [](S_InfoRank* a, S_InfoRank* b) { return a->nScore > b->nScore; });

	// �����Ƀ\�[�g (peck�����)
	std::sort(m_apRank.begin(), m_apRank.end(), [](S_InfoRank* a, S_InfoRank* b) { return a->nTimePeck < b->nTimePeck; });
}

//====================================================
// �����N�̒ǉ�
//====================================================
int CRankingSingle::AddRank(int nScore, int nTimePeck)
{
	int nUpdate = -1;

	// �\�[�g���āA��ԏ������l�Ɣ�r
	Sort();
	
	S_InfoRank *pInfoMin = m_apRank[m_apRank.size() - 1];

	if (pInfoMin->nScore <= nScore)
	{
		pInfoMin->nScore = nScore;
		pInfoMin->nTimePeck = nTimePeck;

		// �ă\�[�g���Ă���
		Sort();

		// �X�V�����ԍ��̕ۑ�
		for (int i = 0; i < (int)m_apRank.size(); ++i)
		{
			if (m_apRank[i] != pInfoMin)
				continue;

			nUpdate = i;
			break;
		}
	}

#ifndef _DEBUG
	// �ۑ�����
	Save();
#endif

	return nUpdate;
}

//====================================================
// �I������
//====================================================
void CRankingSingle::Uninit(void)
{
	ReleaseArray();

	Release();
}

//====================================================
// �X�V����
//====================================================
void CRankingSingle::Update(void)
{

}

//====================================================
// �`�揈��
//====================================================
void CRankingSingle::Draw(void)
{

}

//====================================================
// �Ǎ�
//====================================================
void CRankingSingle::Load(void)
{
	// �z��̉��
	ReleaseArray();

	// �t�@�C�����J��
	std::ifstream inputFile(m_pathSave, std::ios::binary);

	if (!inputFile.is_open())
		assert(("�����L���O�t�@�C�����J���܂���ł���", false));

	while (true)
	{// �t�@�C���̏I���܂œǂݍ���
		S_InfoRank *pInfoRank = new S_InfoRank;

		inputFile.read(reinterpret_cast<char*>(pInfoRank), sizeof(S_InfoRank));

		if (inputFile.eof())
		{
			delete pInfoRank;
			break;
		}

		m_apRank.push_back(pInfoRank);
	}

	inputFile.close();
}

//====================================================
// �ۑ�
//====================================================
void CRankingSingle::Save(void)
{
	// �t�@�C�����J��
	std::ofstream outputFile(m_pathSave, std::ios::binary);

	if (!outputFile.is_open())
		assert(("�����L���O�t�@�C�����J���܂���ł���", false));

	// ���T�C�Y
	size_t size = m_apRank.size();

	for (S_InfoRank *pInfo : m_apRank)
	{
		// �f�[�^�̕ۑ�
		outputFile.write(reinterpret_cast<char*>(&pInfo->nScore), sizeof(int));
		outputFile.write(reinterpret_cast<char*>(&pInfo->nTimePeck), sizeof(int));
	}

	outputFile.close();
}