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
#include "inputkeyboard.h"
#include "iceManager.h"
#include "debugproc.h"
#include "particle.h"

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
CPlayer::CPlayer(int nPriority) : m_nGridV(0), m_nGridH(0)
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

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// ����
//=====================================================
void CPlayer::Input(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// �ړ��̓���========================================
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_LEFT))
	{
		m_nGridH--;
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_RIGHT))
	{
		m_nGridH++;
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_UP))
	{
		m_nGridV++;
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_DOWN))
	{
		m_nGridV--;
	}

	D3DXVECTOR3 posGrid = pIceManager->GetGridPosition(&m_nGridV, &m_nGridH);
	SetPosition(posGrid);

	// ���̓���========================================
	if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_PECK))
	{// ����Ă���X������
		pIceManager->PeckIce(m_nGridV,m_nGridH, CIceManager::E_Direction::DIRECTION_LEFT);	// ���鏈��

		CParticle::Create(D3DXVECTOR3(posGrid.x, posGrid.y - 20.0f, posGrid.z), CParticle::TYPE_ICEBREAK, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CPlayer::Debug(void)
{
	CDebugProc *pDebugProc = CDebugProc::GetInstance();
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	if (pDebugProc == nullptr || pInputKeyboard == nullptr)
		return;

	pDebugProc->Print("\n�c[%d]��[%d]", m_nGridV, m_nGridH);

	if (pInputKeyboard->GetTrigger(DIK_RSHIFT))
	{
		CIceManager *pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
		{
			pIceManager->CreateIce(m_nGridV, m_nGridH);
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