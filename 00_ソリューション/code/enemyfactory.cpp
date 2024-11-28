//*****************************************************
//
// �G�t�@�N�g���[[enemyfactory.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemyfactory.h"
#include "manager.h"
#include "enemy.h"
#include "game.h"
#include "fishshadow.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{

}

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyFct::CEnemyFct(int nPriority) : CObject(nPriority), m_fTimerSpawn(0.0f)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyFct::~CEnemyFct()
{

}

//=====================================================
// ��������
//=====================================================
CEnemyFct *CEnemyFct::Create(void)
{
	CEnemyFct *pFct = nullptr;

	pFct = new CEnemyFct;

	if (pFct != nullptr)
	{
		pFct->Init();
	}

	return pFct;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyFct::Init(void)
{
	return S_OK;
}

//=====================================================
// �Ǎ�
//=====================================================
void CEnemyFct::Load(string path)
{
	std::ifstream file(path);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// �ǂݍ��ނ��̂��Ȃ��Ȃ�܂œǍ�
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "SETENEMY")
			{// �G���Ǎ�
				S_InfoEnemy *pInfoEnemy = new S_InfoEnemy;

				LoadInfo(file, temp, pInfoEnemy);

				m_apInfoEnemy.push_back(pInfoEnemy);
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

	// �G�̐ݒ�
	SetEnemy();
}

//=====================================================
// ���̓Ǎ�
//=====================================================
void CEnemyFct::LoadInfo(std::ifstream& file, string str, S_InfoEnemy *pInfoEnemy)
{
	while (std::getline(file, str))
	{// �Ǎ��J�n
		std::istringstream iss(str);
		std::string key;
		iss >> key;

		if (key == "DELAY")
		{// �f�B���C
			iss >> str >> pInfoEnemy->fDelaySpawn;
		}

		if (key == "TYPE")
		{// ���
			iss >> str >> pInfoEnemy->nType;
		}

		if (key == "SPAWN")
		{// �X�|�[���p�^�[��
			iss >> str >> pInfoEnemy->nPaternSpawn;
		}

		if (key == "END_SETENEMY")
		{// �I��
			break;
		}
	}
}

//=====================================================
// �G�̐ݒ�
//=====================================================
void CEnemyFct::SetEnemy(void)
{
	CGame *pGame = CGame::GetInstance();

	if (pGame == nullptr)
		return;

	for (S_InfoEnemy *pInfoEnemy : m_apInfoEnemy)
	{
		pGame->AddEnemy(pInfoEnemy->nType);
	}
}

//=====================================================
// �I������
//=====================================================
void CEnemyFct::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyFct::Update(void)
{
	// �o�ߎ��ԉ��Z
	float fTimeOld = m_fTimerSpawn;
	m_fTimerSpawn += CManager::GetDeltaTime();

	for (S_InfoEnemy *pInfo : m_apInfoEnemy)
	{
		if (pInfo->fDelaySpawn > fTimeOld && pInfo->fDelaySpawn <= m_fTimerSpawn)
		{// �X�|�[���f�B���C�̉��b�O�ɋ��e������
			CFishShadow::Create(CEnemy::TYPE::TYPE_SEALS, D3DXVECTOR3(0.5f, 0.5f, 0.0f));
		}


		if (pInfo->fDelaySpawn > fTimeOld && pInfo->fDelaySpawn <= m_fTimerSpawn)
		{// �X�|�[���f�B���C���z�����u�ԂɃX�|�[��
			CEnemy::Create(pInfo->nType,(CEnemy::E_Spawn)pInfo->nPaternSpawn);
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyFct::Draw(void)
{

}