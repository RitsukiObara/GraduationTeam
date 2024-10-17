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
#include "inputjoypad.h"
#include "iceManager.h"
#include "debugproc.h"
#include "particle.h"
#include "camera.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// �{�f�B�̃p�X
	const int MOVE_FRAME = 25;	// �ړ��ɂ�����t���[����

	const float SPEED_MOVE_ANALOG = 1.0f;	// �A�i���O�ړ��ł̈ړ�����
	const float RATE_DECREASE_MOVE = 0.5f;	// �ړ������̊���
	const float LINE_FACT_ROT = 0.3f;	// ������␳����܂ł̓��͂������l
	const float FACT_ROTATION = 0.3f;	// ��]�W��
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CPlayer* CPlayer::s_pPlayer = nullptr;	// ���g�̃|�C���^

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CPlayer::CPlayer(int nPriority) : m_nGridV(0), m_nGridH(0), m_bMove(false), m_bAnalog(false)
{

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
CPlayer* CPlayer::Create(void)
{
	if (s_pPlayer == nullptr)
	{
		s_pPlayer = new CPlayer;

		if (s_pPlayer != nullptr)
		{
			s_pPlayer->Init();
		}
	}

	return s_pPlayer;
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
	s_pPlayer = nullptr;

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
	if (m_bAnalog)
		MoveAnalog();
	else
		MoveGrid();

	// �˂����̓���
	InputPeck();

#ifdef _DEBUG
	if (CInputJoypad::GetInstance()->GetTrigger(CInputJoypad::PADBUTTONS_UP, 0) ||
		CInputKeyboard::GetInstance()->GetTrigger(DIK_2))
		m_bAnalog = m_bAnalog ? false : true;
#endif
}

//=====================================================
// �A�i���O�ړ�
//=====================================================
void CPlayer::MoveAnalog(void)
{
	// �A�i���O�ړ�����
	InputMoveAnalog();

	// �X�Ƃ̔���
	CollideIce();
}

//=====================================================
// �A�i���O�ړ�����
//=====================================================
void CPlayer::InputMoveAnalog(void)
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
	{
		return;
	}

	// �J�����擾
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	// �ڕW�����̐ݒ�
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	// ���𐳋K��
	float fLengthAxis = D3DXVec3Length(&axisMove);

	D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = GetRotation();

	fLengthAxis *= SPEED_MOVE_ANALOG;

	// �ړ����x�̐ݒ�
	D3DXVECTOR3 move = GetMove();

	// �����Ă�������Ƀx�N�g����L�΂�
	vecMove -= {sinf(rot.y)* fLengthAxis, 0.0f, cosf(rot.y)* fLengthAxis};
	D3DXVec3Normalize(&vecMove, &vecMove);
	vecMove *= SPEED_MOVE_ANALOG;
	move += vecMove;

	SetMove(move);

	// �ړ��ʂ̔��f
	AddPosition(move);

	// �ړ��ʂ̌���
	move *= RATE_DECREASE_MOVE;

	SetMove(move);

	if (fLengthAxis >= LINE_FACT_ROT)
	{// ���͂��������l���z���Ă�����␳
		// �ڕW�̌����ɕ␳����
		float fRotDest = atan2f(-axisMove.x, -axisMove.z);

		universal::FactingRot(&rot.y, fRotDest, FACT_ROTATION);
		SetRotation(rot);
	}
}

//=====================================================
// �X�Ƃ̔���
//=====================================================
void CPlayer::CollideIce(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	pIceManager->Collide(&pos);
	pIceManager->GetIdxGridFromPosition(pos, &m_nGridV, &m_nGridH);

	SetPosition(pos);
}

//=====================================================
// �O���b�h�ړ�
//=====================================================
void CPlayer::MoveGrid(void)
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// �O���b�h�擾========================================
	D3DXVECTOR3 posGrid = pIceManager->GetGridPosition(&m_nGridV, &m_nGridH);

	// �ړ����Ă��Ȃ���Γ��͎�t========================================
	if (m_bMove == false)
	{
		// �ړ��̓���========================================
		if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_LEFT))
		{
			m_nGridH--;
			m_bMove = true;
		}
		else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_RIGHT))
		{
			m_nGridH++;
			m_bMove = true;
		}
		else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_UP))
		{
			m_nGridV++;
			m_bMove = true;
		}
		else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_AXIS_DOWN))
		{
			m_nGridV--;
			m_bMove = true;
		}

		if (m_bMove == true && GetMotion() == 0)
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

	// �ړ�����
	MoveToGrid();
}

//=====================================================
// �O���b�h�܂ł̈ړ�
//=====================================================
void CPlayer::MoveToGrid(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// �O���b�h�擾========================================
	D3DXVECTOR3 posGrid = pIceManager->GetGridPosition(&m_nGridV, &m_nGridH);

#ifndef _DEBUG
	if (m_bMove == true)
	{// �f�o�b�O���u�Ԉړ�
		SetPosition(posGrid);
		SetMotion(MOTION_NEUTRAL);
		m_bMove = false;
	}
#else
	if (m_bMove == true && GetMotion() == MOTION_JUMPSTART && IsFinish() == true)
	{// �ǂ����ړ�����
		// �ڕW�ʒu�ݒ�========================================
		SetPositionDest(posGrid);

		// �ړ��ʐݒ�========================================
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 move = (posGrid - pos) / MOVE_FRAME;
		move.y = 10.0f;
		SetMove(move);

		// �W�����v���[�V����
		SetMotion(MOTION_JUMPFLY);
	}
#endif
	if (m_bMove == false)
	{
		CIce* pIce = pIceManager->GetGridObject(&m_nGridV, &m_nGridH);
		if (pIce != nullptr)
		{
			D3DXVECTOR3 posObject = pIce->GetPosition();
			SetPosition(posObject);
		}
	}
}

//=====================================================
// �˂����̓���
//=====================================================
void CPlayer::InputPeck(void)
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	if (pInputManager->GetTrigger(CInputManager::BUTTON_PECK))
		pIceManager->PeckIce(m_nGridV, m_nGridH, CIceManager::E_Direction::DIRECTION_LEFT);
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CPlayer::Debug(void)
{
	CDebugProc* pDebugProc = CDebugProc::GetInstance();
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	if (pDebugProc == nullptr || pInputKeyboard == nullptr || pJoypad == nullptr)
		return;

	pDebugProc->Print("\n�c[%d]��[%d]", m_nGridV, m_nGridH);
	pDebugProc->Print("\n�ʒu[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);

	if (pInputKeyboard->GetTrigger(DIK_RSHIFT) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RB, 0))
	{
		CIceManager* pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
		{
			pIceManager->CreateIce(m_nGridV, m_nGridH);
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RCONTROL) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_LB, 0))
	{
		CIceManager* pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
		{
			pIceManager->CreateIce(m_nGridV, m_nGridH, CIce::E_Type::TYPE_HARD);
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
			m_bMove = false;
		}
		if (CMotion::GetMotion() == MOTION_PECK)
		{// �X����I���i�ʏ��ԑJ�ځj
			SetMotion(MOTION_NEUTRAL);
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