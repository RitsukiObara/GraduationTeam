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
const std::string PATH_BODY = "data\\MOTION\\motionPing-.txt";	// �{�f�B�̃p�X
const int MOVE_FRAME = 25;	// �ړ��ɂ�����t���[����
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
	m_isMove = false;
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

	// ���[�V�����X�V
	CMotion::Update();

	// ���[�V����������̏���
	MotionFinishCheck();

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

	// �O���b�h�擾========================================
	D3DXVECTOR3 posGrid = pIceManager->GetGridPosition(&m_nGridV, &m_nGridH);

	// �ړ����Ă��Ȃ���Γ��͎�t========================================
	D3DXVECTOR3 pos = GetPosition();
	if (m_isMove == false)
	{
		// �ړ��̓���========================================
		if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_LEFT))
		{
			m_nGridH--;
			m_isMove = true;
		}
		else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_RIGHT))
		{
			m_nGridH++;
			m_isMove = true;
		}
		else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_UP))
		{
			m_nGridV++;
			m_isMove = true;
		}
		else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_DOWN))
		{
			m_nGridV--;
			m_isMove = true;
		}

		if (m_isMove == true && GetMotion() == 0)
		{
			// �W�����v���[�V����
			SetMotion(MOTION_JUMPSTART);
		}

		// ���̓���========================================
		if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_PECK))
		{// ����Ă���X������
			pIceManager->PeckIce(m_nGridV, m_nGridH, CIceManager::E_Direction::DIRECTION_LEFT);	// ���鏈��

			CParticle::Create(D3DXVECTOR3(posGrid.x, posGrid.y - 20.0f, posGrid.z), CParticle::TYPE_ICEBREAK, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			// �X���胂�[�V����
			SetMotion(MOTION_PECK);
		}
	}

	if (m_isMove == true && GetMotion() == MOTION_JUMPSTART && IsFinish() == true)
	{// �ǂ����ړ�����
		// �ڕW�ʒu�ݒ�========================================
		SetPositionDest(posGrid);

		// �ړ��ʐݒ�========================================
		D3DXVECTOR3 move = (posGrid - pos) / MOVE_FRAME;
		move.y = 10.0f;
		SetMove(move);

		// �W�����v���[�V����
		SetMotion(MOTION_JUMPFLY);
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

	if (pInputKeyboard->GetTrigger(DIK_RCONTROL))
	{
		CIceManager *pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
		{
			pIceManager->CreateIce(m_nGridV, m_nGridH,CIce::E_Type::TYPE_HARD);
		}
	}
}

//=====================================================
// ���[�V����������̏���
//=====================================================
void CPlayer::MotionFinishCheck(void)
{
	if (CMotion::IsFinish() == true)
	{// ��������̃��[�V�����I��
		if (CMotion::GetMotion() == MOTION_LANDING)
		{// ���n�I���i�ʏ��ԑJ�ځj
			SetMotion(MOTION_NEUTRAL);
			m_isMove = false;
		}
		if (CMotion::GetMotion() == MOTION_PECK)
		{// �X����I���i�ʏ��ԑJ�ځj
			SetMotion(MOTION_NEUTRAL);
			m_isMove = false;
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