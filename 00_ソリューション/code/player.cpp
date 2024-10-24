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
#include "flowIce.h"
#include "effect3D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// �{�f�B�̃p�X
	const int MOVE_FRAME = 25;	// �ړ��ɂ�����t���[����
	
	const float SPEED_MOVE_ANALOG = 3.0f;	// �A�i���O�ړ��ł̈ړ�����
	const float RATE_DECREASE_MOVE = 0.5f;	// �ړ������̊���
	const float LINE_FACT_ROT = 0.3f;	// ������␳����܂ł̓��͂������l
	const float FACT_ROTATION = 0.3f;	// ��]�W��
	const float DEFAULT_WEIGHT = 5.0f;	// ���̏d��
	const float POSDEST_NEAREQUAL = 0.01f;	// ��̖ڕW�ʒu�ɒ������Ƃ��鋗��

	const float LINE_JUDGE_MOVE = 50.0f;	// �ړ��Ɉڂ����Ɣ��f���郉�C��

	const float LINE_INPUT_MOVE = 0.3f;	// �ړ�����܂ł̃X�e�B�b�N���͂̂������l
	const float RANGE_SELECT_ICE = D3DX_PI * 2 / 6;	// �X��I������Ƃ��̊p�x�͈̔�
	const float RATE_CHANGE_GRID = 0.6f;	// ���̃O���b�h�Ɉڂ锻��̊���
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CPlayer* CPlayer::s_pPlayer = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CPlayer::CPlayer(int nPriority) : m_nGridV(0), m_nGridH(0), m_bAnalog(false), m_state(STATE_NONE), m_pIceMoveDest(nullptr)
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

	// �O���b�h�ԍ��̏�����
	InitGridIdx();

	// �A�i���O�ړ��ɐݒ�
	m_bAnalog = true;

	// ��Ԑݒ�
#ifdef _DEBUG
	m_state = STATE_INVINCIBLE;
#else
	m_state = STATE_NORMAL;
#endif // _DEBUG

	return S_OK;
}

//=====================================================
// �O���b�h�ԍ��̏�����
//=====================================================
void CPlayer::InitGridIdx(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->GetRightDownIdx(&m_nGridV, &m_nGridH);
	
	D3DXVECTOR3 pos = pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH);
	SetPosition(pos);
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
	// �ڕW�̕X������ł���null�ɂ���
	if (m_pIceMoveDest != nullptr)
	{
		if (m_pIceMoveDest->IsDeath())
			m_pIceMoveDest = nullptr;
	}

	// ���͏���
	Input();

	// ���[�V�����X�V
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
	if (m_state == STATE_NORMAL || m_state == STATE_INVINCIBLE)
	{
		if (m_bAnalog)
			MoveAnalog();
		else
			MoveGrid();

		// �˂����̓���
		InputPeck();
	}

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

	if(m_state != STATE::STATE_INVINCIBLE)	// ���G���͍s��Ȃ�
		CollideIce();	// �X�Ƃ̔���
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
	CInputManager::S_Axis axis = pInputManager->GetAxis();
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

	// ������O���b�h�ԍ��̎擾
	CheckGridChange();
}

//=====================================================
// �X�Ƃ̔���
//=====================================================
void CPlayer::CollideIce(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	// �O���b�h�̈ʒu�ɍ��킹��
	pIceMgr->Collide(&pos, m_nGridV, m_nGridH);

	// �X�̍����ɍ��킹��
	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);
		
	if (pIceStand != nullptr)
		pos.y = pIceStand->GetPosition().y;

	SetPosition(pos);
}

//=====================================================
// �O���b�h�ړ�
//=====================================================
void CPlayer::MoveGrid(void)
{
	bool bSarch = true;

	// �X��I�����Ă��邩�X�Ɍ������Ĉړ����Ă��邩�̔���
	bSarch = JudgeSarchOrMove();

	if (bSarch)
		UpdateInputSelectIce(); // �X�I����Ԃ̍X�V
	else
		UpdateInputMoveToIce();	// �X�Ɍ������Ĉړ����Ă����Ԃ̍X�V
}

//=====================================================
// �I����Ԃ��ړ���Ԃ��̔���
//=====================================================
bool CPlayer::JudgeSarchOrMove(void)
{
	// �X�̔ԍ����擾
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	// ���݂̃O���b�h���S������ȏ㗣��Ă�����ړ����
	D3DXVECTOR3 posPlayer = GetPosition();
	D3DXVECTOR3 posCurrentGrid = pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH);

	// �����x�N�g���̋����Ŕ���
	posPlayer.y = posCurrentGrid.y;
	D3DXVECTOR3 vecDiff = posPlayer - posCurrentGrid;
	float fDist = D3DXVec3Length(&vecDiff);

	return fDist < LINE_JUDGE_MOVE || m_pIceMoveDest == nullptr;
}

//=====================================================
// �X�I����Ԃ̍X�V
//=====================================================
void CPlayer::UpdateInputSelectIce(void)
{
#ifdef _DEBUG
	CEffect3D::Create(GetPosition(), 50.0f, 5, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
#endif

	CInputManager* pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	// ���쎲�̎擾
	CInputManager::S_Axis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	float fLengthAxis = D3DXVec3Length(&axisMove);

	if (fLengthAxis < LINE_INPUT_MOVE)
		return;	// ���ȏ���͂��Ȃ���Ό�̏�����ʂ�Ȃ�

	// ���͊p�x�̎擾
	float fRotInput = atan2f(axisMove.x, axisMove.z);

	// �����ɂ��X�̑I��
	m_pIceMoveDest = SelectIceByRot(fRotInput);

	if (m_pIceMoveDest == nullptr)
		return;	// �����ɍ����X��������Ί֐����I��

	// �X�̔ԍ����擾
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	int nGridNextV = 0;
	int nGridNextH = 0;
	pIceMgr->GetIceIndex(m_pIceMoveDest, &nGridNextV, &nGridNextH);

	// �ڕW�̕X�܂ŕ�������
	WalkToDestIce(m_pIceMoveDest);

	// �O���b�h�ύX�̌��o
	CheckGridChange();

#ifdef _DEBUG
	CEffect3D::Create(m_pIceMoveDest->GetPosition(), 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif
}

//=====================================================
// �����ɂ��X�̑I��
//=====================================================
CIce *CPlayer::SelectIceByRot(float fRot)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return nullptr;

	// ���ӂ̕X�̎擾
	vector<CIce*> apIce = pIceMgr->GetAroundIce(m_nGridV, m_nGridH);

	for (auto it : apIce)
	{
		if (it == nullptr)
			continue;

		D3DXVECTOR3 posCurrentGrid = pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH);
		D3DXVECTOR3 posIce = it->GetPosition();

		// �X�ƃX�e�B�b�N�p�x�̔�r
		bool bSelect = universal::IsInFanTargetYFlat(posCurrentGrid, posIce, fRot, RANGE_SELECT_ICE);

		if (bSelect)
		{// �X���I�ׂ���for�����I��
			m_pIceMoveDest = it;
			break;
		}
	}

	return m_pIceMoveDest;
}

//=====================================================
// �ڕW�̕X�Ɍ������Ĉړ����鏈��
//=====================================================
void CPlayer::WalkToDestIce(CIce *pIceDest)
{
	if (m_pIceMoveDest == nullptr)
		return;

	// �����x�N�g�����ړ��ʂɐ��K��
	D3DXVECTOR3 posPlayer = GetPosition();
	D3DXVECTOR3 posIce = m_pIceMoveDest->GetPosition();

	D3DXVECTOR3 vecDiff = posIce - posPlayer;
	D3DXVec3Normalize(&vecDiff, &vecDiff);

	D3DXVECTOR3 move = vecDiff *= SPEED_MOVE_ANALOG;

	// �ړ��ʂ𔽉f
	AddPosition(move);
}

//=====================================================
// �O���b�h���ς�������ǂ����̔���
//=====================================================
bool CPlayer::CheckGridChange(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	int nIdxV = -1;
	int nIdxH = -1;

	// �O���b�h�ԍ��̎擾
	D3DXVECTOR3 pos = GetPosition();
	pIceMgr->GetIdxGridFromPosition(pos, &nIdxV, &nIdxH, RATE_CHANGE_GRID);

	if(m_state != STATE::STATE_INVINCIBLE &&
		pIceMgr->GetGridIce(&nIdxV, &nIdxH) == nullptr)
		return false;	// ���G��ԂłȂ��ꍇ�A�X���Ȃ��O���b�h�̏�ɍs���Ă��ԍ���ς��Ȃ�

	if ((nIdxV == m_nGridV &&
		nIdxH == m_nGridH) ||
		nIdxV == -1 ||
		nIdxH == -1)
	{// �O���b�h���ς���ĂȂ����͋U��Ԃ�
		return false;
	}
	else
	{// �O���b�h���ς���Ă���l��ۑ����Đ^��Ԃ�
		m_nGridV = nIdxV;
		m_nGridH = nIdxH;

#ifdef _DEBUG
		CEffect3D::Create(pIceMgr->GetGridPosition(&nIdxV, &nIdxH), 50.0f, 120, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
#endif
		return true;
	}
}

//=====================================================
// �X�Ɍ������Ĉړ����Ă����Ԃ̍X�V
//=====================================================
void CPlayer::UpdateInputMoveToIce(void)
{
#ifdef _DEBUG
	CEffect3D::Create(GetPosition(), 50.0f, 5, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
#endif

	if (m_pIceMoveDest == nullptr)
		return;

	CInputManager* pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// ���쎲�̎擾
	CInputManager::S_Axis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	float fLengthAxis = D3DXVec3Length(&axisMove);

	if (fLengthAxis < LINE_INPUT_MOVE)
		return;	// ���ȏ���͂��Ȃ���Ό�̏�����ʂ�Ȃ�

	// ���͊p�x�̎擾
	float fRotInput = atan2f(axisMove.x, axisMove.z);

	// �I�����Ă�X�ƖڕW�̕X�̂ݔ�����Ƃ�
	D3DXVECTOR3 posPlayer = GetPosition();
	D3DXVECTOR3 posCurrentGrid = pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH);
	D3DXVECTOR3 posDest = m_pIceMoveDest->GetPosition();

	D3DXVECTOR3 vecDiff = {};

	if (universal::IsInFanTargetYFlat(posPlayer, posCurrentGrid, fRotInput, RANGE_SELECT_ICE * 1.5f))
	{// �߂낤�Ƃ��Ă�ꍇ
		vecDiff = posCurrentGrid - posPlayer;

		// �ړ��ʂ𔽉f
		universal::VecConvertLength(&vecDiff, SPEED_MOVE_ANALOG);
		AddPosition(vecDiff);
	}
	else if (universal::IsInFanTargetYFlat(posPlayer, posDest, fRotInput, RANGE_SELECT_ICE * 1.5f))
	{// �i�����Ƃ��Ă�ꍇ
		vecDiff = posDest - posPlayer;

		// �ړ��ʂ𔽉f
		universal::VecConvertLength(&vecDiff, SPEED_MOVE_ANALOG);
		AddPosition(vecDiff);
	}

	// �����z������O���b�h�ԍ���ύX
	CheckGridChange();
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
	{
		// �˂�������
		D3DXVECTOR3 rot = GetRotation();
		D3DXVECTOR3 pos = GetPosition();

		rot.y += D3DX_PI;
		universal::LimitRot(&rot.y);

		pIceManager->PeckIce(m_nGridV, m_nGridH, rot.y, pos);
	}
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

	if (pInputKeyboard->GetTrigger(DIK_LSHIFT))
	{// �����X�̐ݒu
		CIceManager* pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
		{
			CFlowIce::Create();
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_F8))
	{// ���G�؂�ւ�
		m_state = (m_state == STATE_INVINCIBLE) ? STATE_NORMAL : STATE_INVINCIBLE;
	}
	if (m_state == STATE_INVINCIBLE)
	{
		pDebugProc->Print("\n<<���G��(*�e�ցe *)�iF8�Œʏ�j>>");
	}
	else
	{
		pDebugProc->Print("\n<<�ʏ�(-_-)zzz�iF8�Ŗ��G�j>>");
	}

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	CEffect3D::Create(pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH), 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayer::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}