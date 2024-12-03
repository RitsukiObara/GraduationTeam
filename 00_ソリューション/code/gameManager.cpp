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
#include "timer.h"
#include "ocean.h"
#include "BG_Ice.h"
#include "camera.h"
#include "cameraState.h"
#include "iceManager.h"
#include "UI_ready.h"
#include "ocean_flow_UI.h"
#include "flowIceFactory.h"
#include "gameManagerSingle.h"
#include "gameManagerMulti.h"
#include "sound.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const string PATH_TEX = "data\\TEMP\\mode.txt";	// ���[�h�̃p�X
const string PATH_IDXMAP = "data\\TEMP\\mapIdx.bin";	// �}�b�v�ԍ��̃p�X
const string PATH_WINNER = "data\\TEMP\\winner.bin";	// ���҂̃p�X
const string PATH_IDXINPUT = "data\\TEMP\\idxInput.txt";	// ���͔ԍ��̃p�X
const int PLAY_SOUND_SEA_COUNT = 60 * 24;	// �Â��ȊC�T�E���h�Đ����ԁi�E���ύX�ŕb���ύX�j
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CGameManager::CGameManager()
{
	m_nCounterSoundSea = 0;
}

//=====================================================
// ��������
//=====================================================
CGameManager *CGameManager::Create(CGame::E_GameMode mode)
{
	CGameManager* pGameManager = nullptr;

	switch (mode)
	{
	case CGame::MODE_SINGLE:
		pGameManager = new CGameManagerSingle;
		break;
	case CGame::MODE_MULTI:
		pGameManager = new CGameManagerMulti;
		break;
	default:
		assert(false);
		break;
	}

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

	//�C�̐���
	COcean::Create();

#if 1
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
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CGameManager::Update(void)
{
	// �Q�[���̏�ԊǗ�
	ManageGameState();
}

//=====================================================
// �Q�[���̏�ԊǗ�
//=====================================================
void CGameManager::ManageGameState(void)
{
	CGame::E_State state = CGame::GetState();

	switch (state)
	{
	case CGame::STATE_READY:

		break;
	case CGame::STATE_START:
		UpdateStart();
		break;
	case CGame::STATE_NORMAL:
		UpdateNormal();
		break;
	case CGame::STATE_RESULT:
		UpdateResult();
		break;
	case CGame::STATE_END:
		UpdateEnd();
		break;
	default:
		assert(false);
		break;
	}
}

//=====================================================
// �J�n��Ԃ̍X�V
//=====================================================
void CGameManager::UpdateStart(void)
{

}

//=====================================================
// �ʏ��Ԃ̍X�V
//=====================================================
void CGameManager::UpdateNormal(void)
{
	m_nCounterSoundSea++;
	if (m_nCounterSoundSea >= PLAY_SOUND_SEA_COUNT)
	{// ���𗬂�
		m_nCounterSoundSea = 0;
		CSound::GetInstance()->Play(CSound::LABEL_SE_SEA);
	}
}

//=====================================================
// ���U���g��Ԃ̍X�V
//=====================================================
void CGameManager::UpdateResult(void)
{

}

//=====================================================
// �I����Ԃ̍X�V
//=====================================================
void CGameManager::UpdateEnd(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CGameManager::Draw(void)
{

}

namespace gameManager
{
// ���[�h�ۑ�
void SaveMode(CGame::E_GameMode mode, vector<bool> abPlayerEnter)
{
	std::ofstream file(PATH_TEX);

	if (file.is_open())
	{
		file << "MODE = " << (int)mode << '\n';			// ���[�h

		for(bool bEnter : abPlayerEnter)
			file << "PLAYER = " << bEnter << '\n';	// �G���^�[�t���O

		file.close();
	}
	else
	{
		assert(("�t�@�C�����J���܂���ł���", false));
	}
}

// ���[�h�Ǎ�
void LoadMode(CGame::E_GameMode *pMode, vector<bool> &rbPlayerEnter)
{
	std::ifstream file(PATH_TEX);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// �ǂݍ��ނ��̂��Ȃ��Ȃ�܂œǍ�
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "MODE")
			{// ���[�h
				int nMode;
				iss >> temp >> nMode;
				*pMode = (CGame::E_GameMode)nMode;
			}

			if (key == "PLAYER")
			{// �l��
				int nData;
				iss >> temp >> nData;

				rbPlayerEnter.push_back(nData);
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

// ���͔ԍ��ۑ�
void SaveIdxInput(vector<int> vectorIdxInput)
{
	std::ofstream file(PATH_IDXINPUT);

	if (file.is_open())
	{
		for (int nIdx : vectorIdxInput)
			file << nIdx << '\n';	// �ԍ�

		file.close();
	}
	else
	{
		assert(("�t�@�C�����J���܂���ł���", false));
	}
}

// ���͔ԍ��Ǎ�
void LoadIdxInput(vector<int> &rVectorIdxInput)
{
	std::ifstream file(PATH_IDXINPUT);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// �ǂݍ��ނ��̂��Ȃ��Ȃ�܂œǍ�
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "IDX")
			{// ���[�h
				int nIdx;
				iss >> temp >> nIdx;
				rVectorIdxInput.push_back(nIdx);
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

// �}���`���̔���
bool IsMulti(void)
{
	CGame::E_GameMode mode;
	vector<bool> abFrag;

	LoadMode(&mode, abFrag);

	return mode == CGame::E_GameMode::MODE_MULTI;
}

// �}�b�v�ԍ��ۑ�
void SaveIdxMap(int nIdx)
{
	// �t�@�C�����J��
	std::ofstream outputFile(PATH_IDXMAP, std::ios::binary);

	if (!outputFile.is_open())
		assert(false);

	// �f�[�^�̕ۑ�
	outputFile.write(reinterpret_cast<char*>(&nIdx), sizeof(int));

	outputFile.close();
}

// �}�b�v�ԍ��Ǎ�
int LoadIdxMap(void)
{
	// �t�@�C�����J��
	std::ifstream inputFile(PATH_IDXMAP, std::ios::binary);

	if (!inputFile.is_open())
		assert(false);

	int nIdx;

	inputFile.read(reinterpret_cast<char*>(&nIdx), sizeof(int));

	inputFile.close();

	return nIdx;
}

// ���ҕۑ�
void SaveWinner(int playerNum, int winner)
{
	// �t�@�C�����J��
	std::ofstream outputFile(PATH_WINNER, std::ios::binary);

	if (!outputFile.is_open())
		assert(false);

	// �f�[�^�̕ۑ�
	outputFile.write(reinterpret_cast<char*>(&playerNum), sizeof(int));
	outputFile.write(reinterpret_cast<char*>(&winner), sizeof(int));

	outputFile.close();
}

// ���ғǍ�
bool LoadWinner(int *playerNum, int *winner)
{
	// �t�@�C�����J��
	std::ifstream inputFile(PATH_WINNER, std::ios::binary);

	if (!inputFile.is_open())
		return false;

	inputFile.read(reinterpret_cast<char*>(playerNum), sizeof(int));
	inputFile.read(reinterpret_cast<char*>(winner), sizeof(int));

	inputFile.close();

	return true;
}
}