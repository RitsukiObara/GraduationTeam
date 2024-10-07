//*****************************************************
//
// �v���C���[�̏���[player.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "player.h"
#include "inputManager.h"
#include "iceManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPlayer.txt";	// �{�f�B�̃p�X
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CPlayer *CPlayer::m_pPlayer = nullptr;	// ���g�̃|�C���^

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	m_pPlayer = this;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CPlayer::~CPlayer()
{

}

//=====================================================
// ��������
//=====================================================
CPlayer *CPlayer::Create(void)
{
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new CPlayer;

		if (m_pPlayer != nullptr)
		{
			m_pPlayer->Init();
		}
	}

	return m_pPlayer;
}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayer::Init(void)
{
	// �Ǎ�
	Load((char*)&PATH_BODY[0]);

	// �p���N���X�̏�����
	CMotion::Init();

	InitPose(0);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CPlayer::Uninit(void)
{
	m_pPlayer = nullptr;

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CPlayer::Update(void)
{
	// ���͏���
	Input();

	CMotion::Update();
}

//=====================================================
// ����
//=====================================================
void CPlayer::Input(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	// �ړ��̓���========================================
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_LEFT))
	{
		pos.x -= Grid::SIZE;
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_RIGHT))
	{
		pos.x += Grid::SIZE;
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_UP))
	{
		pos.z += Grid::SIZE;
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_DOWN))
	{
		pos.z -= Grid::SIZE;
	}

	AddPosition(pos);

	// ���̓���========================================
	if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_PECK))
	{// ����Ă���X������
		CIceManager *pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
		{
			D3DXVECTOR3 pos = GetPosition();
			pIceManager->PeckIce(pos,CIceManager::E_Direction::DIRECTION_LEFT);	// ���鏈��
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayer::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}