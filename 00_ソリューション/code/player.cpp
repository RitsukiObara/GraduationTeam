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
#include "collision.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// �{�f�B�̃p�X
const int MOVE_FRAME = 25;	// �ړ��ɂ�����t���[����

const float RATE_DECREASE_MOVE = 0.5f;	// �ړ������̊���
const float LINE_FACT_ROT = 0.3f;	// ������␳����܂ł̓��͂������l
const float FACT_ROTATION = 0.1f;	// ��]�W��
const float DEFAULT_WEIGHT = 5.0f;	// ���̏d��
const float POSDEST_NEAREQUAL = 0.01f;	// ��̖ڕW�ʒu�ɒ������Ƃ��鋗��

const float LINE_JUDGE_MOVE = 50.0f;	// �ړ��Ɉڂ����Ɣ��f���郉�C��

const float LINE_INPUT_MOVE = 0.3f;	// �ړ�����܂ł̃X�e�B�b�N���͂̂������l
const float RANGE_SELECT_ICE = D3DX_PI * 2 / 6;	// �X��I������Ƃ��̊p�x�͈̔�
const float RATE_CHANGE_GRID = 0.6f;	// ���̃O���b�h�Ɉڂ锻��̊���

const float TIME_MAX_SPEED = 1.0f;	// �ő呬�x�ɒB����܂łɂ����鎞��
const float SPEED_MOVE_MAX = 2.5f;	// �ő�ړ����x

const float LINE_STOP_TURN = 0.2f;	// �U��������~���邵�����l
const float LINE_START_TURN = D3DX_PI * 0.6f;	// �U��������J�n���邵�����l
const float FACT_ROTATION_TURN = 0.2f;	// �U�������]�W��
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
vector<CPlayer*> CPlayer::s_apPlayer;	// �i�[�p�̔z��

//=====================================================
// �R���X�g���N�^
//=====================================================
CPlayer::CPlayer(int nPriority) : m_nGridV(0), m_nGridH(0), m_state(STATE_NONE), m_pIceMoveDest(nullptr), m_bEnableInput(false), m_fTimerStartMove(0.0f),
m_fragMotion(), m_bTurn(false), m_fRotTurn(0.0f), m_pLandSystemFlow(nullptr), m_pLandFlow(nullptr)
{
	s_apPlayer.push_back(this);
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
	CPlayer *pPlayer = nullptr;

	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->Init();
	}

	return pPlayer;
}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayer::Init(void)
{
	// ���̓}�l�[�W���[����
	m_pInputMgr = CInputManager::Create();

	// �Ǎ�
	Load((char*)&PATH_BODY[0]);

	// �p���N���X�̏�����
	CMotion::Init();

	InitPose(0);

	// �O���b�h�ԍ��̏�����
	InitGridIdx();

	// ���͉\�t���O��ݒ�
	m_bEnableInput = true;

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

	pIceMgr->GetLeftDownIdx(&m_nGridV, &m_nGridH);
	
	D3DXVECTOR3 pos = pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH);
	SetPosition(pos);
}

//=====================================================
// �I������
//=====================================================
void CPlayer::Uninit(void)
{
	for (auto itr = s_apPlayer.begin(); itr < s_apPlayer.end(); itr++)
	{
		//�폜�Ώۂ���Ȃ��ꍇ
		if (*itr != this)
		{
			continue;
		}

		//Vector����폜
		s_apPlayer.erase(itr);

		break;
	}

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CPlayer::Update(void)
{
	if (m_pIceMoveDest != nullptr)
	{// �ڕW�̕X������ł���null�ɂ���
		if (m_pIceMoveDest->IsDeath())
			m_pIceMoveDest = nullptr;
	}

	// ���͏���
	Input();

	if (m_state == STATE_FLOW)
		StayFlow();	// �Y�����̏���

	// ���[�V�����̊Ǘ�
	ManageMotion();

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
	if (m_bEnableInput)
	{
		// �A�i���O�ړ�
		MoveAnalog();

		// �˂����̓���
		InputPeck();

		// �W�����v�̏���
		InputJump();
	}

	if (false)
		StayJump();	// �W�����v���̏���
}

//=====================================================
// �A�i���O�ړ�
//=====================================================
void CPlayer::MoveAnalog(void)
{
	// �A�i���O�ړ�����
	InputMoveAnalog();

	if(m_state != E_State::STATE_INVINCIBLE && m_state != E_State::STATE_FLOW)	// ���G���͍s��Ȃ�
		CollideIce();	// �X�Ƃ̔���
}

//=====================================================
// �A�i���O�ړ�����
//=====================================================
void CPlayer::InputMoveAnalog(void)
{
	if (m_bTurn)
	{// �U��Ԃ��Ă锻��̓���
		// �U��Ԃ�̖�����
		DisableTurn();

#ifdef _DEBUG
		CDebugProc::GetInstance()->Print("\n�U��Ԃ��Ă��I�I�I�I�I�I�I�I�I�I�I�I�I�I");
#endif
	}
	else
	{// �ʏ�̑O�i���
#ifdef _DEBUG
		CDebugProc::GetInstance()->Print("\n�U��Ԃ��ĂȂ�");
#endif
		// �O�i����
		Forward();

		// �U��Ԃ�̌��o
		JudgeTurn();

		// �����̕␳
		FactingRot();
	}

	// �ړ��ʂ̌���
	DecreaseMove();

	// ������O���b�h�ԍ��̎擾
	CheckGridChange();
}

//=====================================================
// �U��Ԃ�̖�����
//=====================================================
void CPlayer::DisableTurn(void)
{
	// �ڕW�̌����ɕ␳����
	D3DXVECTOR3 rot = GetRotation();
	universal::FactingRot(&rot.y, m_fRotTurn, FACT_ROTATION_TURN);
	SetRotation(rot);

	// �����p�x�����ȉ��ɂȂ�����U��Ԃ���~����
	float fRotDiff = m_fRotTurn - rot.y;

	universal::LimitRot(&fRotDiff);

	if (LINE_STOP_TURN * LINE_STOP_TURN > fRotDiff * fRotDiff)
		m_bTurn = false;
}

//=====================================================
// �O�i����
//=====================================================
void CPlayer::Forward(void)
{
	if (m_pInputMgr == nullptr)
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
	CInputManager::S_Axis axis = m_pInputMgr->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	// ���𐳋K��
	float fLengthAxis = D3DXVec3Length(&axisMove);

	D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = GetRotation();

	float fSpeed = SPEED_MOVE_MAX;

	if (LINE_INPUT_MOVE < fLengthAxis)
	{// �ړ������삪�������l���z���Ă�����A�ړ����x�̗����オ����J�n
		m_fTimerStartMove += CManager::GetDeltaTime();

		m_fragMotion.bWalk = true;
	}
	else
	{// ����
		m_fTimerStartMove = 0.0f;

		m_fragMotion.bWalk = false;
	}

	// �l�̕␳
	universal::LimitValuefloat(&m_fTimerStartMove, TIME_MAX_SPEED, 0.0f);

	// �C�[�W���O�ŕ␳
	fSpeed *= easing::EaseOutQuart(m_fTimerStartMove / TIME_MAX_SPEED);

	// �ړ����x�̐ݒ�
	D3DXVECTOR3 move = GetMove();

	// �����Ă�������Ƀx�N�g����L�΂�
	vecMove -= {sinf(rot.y) * fSpeed, 0.0f, cosf(rot.y) * fSpeed};
	move += vecMove;

	SetMove(move);

	// �ړ��ʂ̔��f
	AddPosition(move);
}

//=====================================================
// �ړ��ʌ���
//=====================================================
void CPlayer::DecreaseMove(void)
{
	D3DXVECTOR3 move = GetMove();

	// �ړ��ʂ̌���
	move *= RATE_DECREASE_MOVE;

	SetMove(move);
}

//=====================================================
// �����̕␳
//=====================================================
void CPlayer::FactingRot(void)
{
	if (m_pInputMgr == nullptr)
		return;

	// �ڕW�����̐ݒ�
	CInputManager::S_Axis axis = m_pInputMgr->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	// ���𐳋K��
	float fLengthAxis = D3DXVec3Length(&axisMove);

	if (fLengthAxis >= LINE_FACT_ROT)
	{// ���͂��������l���z���Ă�����␳
		// �ڕW�̌����ɕ␳����
		float fRotDest = atan2f(-axisMove.x, -axisMove.z);

		D3DXVECTOR3 rot = GetRotation();
		universal::FactingRot(&rot.y, fRotDest, FACT_ROTATION);
		SetRotation(rot);
	}
}

//=====================================================
// �U������̌��o
//=====================================================
void CPlayer::JudgeTurn(void)
{
	if (m_pInputMgr == nullptr)
		return;

	// �����p�x���쐬
	float fAngleInput = m_pInputMgr->GetAngleMove();
	D3DXVECTOR3 rot = GetRotation();

	// �ڕW�����̐ݒ�
	CInputManager::S_Axis axis = m_pInputMgr->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	// ���𐳋K��
	float fLengthAxis = D3DXVec3Length(&axisMove);

	// ���ȏ���͂��Ă��Ȃ���Βʂ�Ȃ�
	if (fLengthAxis <= LINE_FACT_ROT)
		return;

	// �����̔���
	float fRotDiff = fAngleInput - rot.y;
	universal::LimitRot(&fRotDiff);

	if (LINE_START_TURN * LINE_START_TURN < fRotDiff * fRotDiff)
	{
#if 0
		m_fRotTurn = fAngleInput;
#else 
		m_fRotTurn = rot.y + D3DX_PI;
		universal::LimitRot(&m_fRotTurn);
#endif

		m_bTurn = true;	// �������l���z���Ă�����U��Ԃ锻��
	}
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

	if (m_state != E_State::STATE_INVINCIBLE &&
		pIceMgr->GetGridIce(&nIdxV, &nIdxH) == nullptr)
	{// ���G��ԂłȂ��ꍇ�A�X���Ȃ��O���b�h�̏�ɍs���Ă��ԍ���ς��Ȃ�
		// �Y�����J�n
		StartFlows();

		return false;
	}

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
// �Y���̊J�n
//=====================================================
void CPlayer::StartFlows(void)
{
	if (FindFlowIce())
	{// �Y������X��������΁A�Y����Ԃֈڍs
		m_state = E_State::STATE_FLOW;
	}
}

//=====================================================
// �Y������X�̌��o
//=====================================================
bool CPlayer::FindFlowIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();
	
	if (pIceMgr == nullptr)
		return false;

	vector<CFlowIce*> apSystemFlow = CFlowIce::GetInstance();

	for (auto itSystem : apSystemFlow)
	{
		if (itSystem == nullptr)
			continue;

		// ���X�V�X�e������������X�̎擾
		vector<CIce*> apIce = itSystem->GetIce();

		for (auto itIce : apIce)
		{
			D3DXVECTOR3 posPlayer = GetPosition();
			D3DXVECTOR3 posIce = itIce->GetPosition();

			if (pIceMgr->IsInIce(posPlayer, itIce, 0.7f))
			{// �ǂꂩ�ɏ���Ă����猻�݂̃V�X�e����ۑ����Ċ֐����I��
				m_pLandSystemFlow = itSystem;

				return true;
			}
		}
	}

	return false;
}

//=====================================================
// �Y�����̏���
//=====================================================
void CPlayer::StayFlow(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	if (m_pLandSystemFlow == nullptr)
		return;

	if (m_pLandSystemFlow->IsDeath())
	{
		// �Y���̏I��
		EndFlows();
	}

	// �C���̃x�N�g���擾
	CIceManager::E_Stream dir = pIceMgr->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// ����鑬�x�ɐ��K�����Ĉʒu�����Z
	float fSpeedFlow = pIceMgr->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow;
	AddPosition(vecStream);

	// ���X���Ɉʒu�𐧌�
	LimitInSideFlowIce();
}

//=====================================================
// ���X�̓����ɐ���
//=====================================================
void CPlayer::LimitInSideFlowIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	if (m_pLandSystemFlow == nullptr)
		return;

	// ���X�V�X�e������������X�̎擾
	vector<CIce*> apIce = m_pLandSystemFlow->GetIce();

	for (auto itIce : apIce)
	{
		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 posIce = itIce->GetPosition();

		if (pIceMgr->IsInIce(posPlayer, itIce,0.7f))
		{// ��ɏ���Ă���ʒu�𐧌�
			pIceMgr->Collide(&posPlayer, itIce);
			posPlayer.y = posIce.y;
			SetPosition(posPlayer);
			m_pLandFlow = itIce;
			return;
		}
	}

	if (m_pLandFlow != nullptr)
	{// �����ǂ̕X�ɂ�����������Ȃ���΁A�O��̕X�Ƃ̔�����s��
		D3DXVECTOR3 posPlayer = GetPosition();
		pIceMgr->Collide(&posPlayer, m_pLandFlow);
		posPlayer.y = m_pLandFlow->GetPosition().y;
		SetPosition(posPlayer);
	}
}

//=====================================================
// �Y���̏I��
//=====================================================
void CPlayer::EndFlows(void)
{
	m_state = E_State::STATE_NORMAL;
	m_pLandSystemFlow = nullptr;
}

//=====================================================
// �˂����̓���
//=====================================================
void CPlayer::InputPeck(void)
{
	if (m_pInputMgr == nullptr)
		return;

	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	if (m_pInputMgr->GetTrigger(CInputManager::BUTTON_PECK))
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
// �W�����v�̓���
//=====================================================
void CPlayer::InputJump(void)
{
	//if (m_pInputMgr == nullptr)
	return;

	if (m_pInputMgr->GetTrigger(CInputManager::BUTTON_JUMP))
		SarchJumpIce();	// �W�����v��̕X��T��
}

//=====================================================
// �W�����v��̕X��T��
//=====================================================
void CPlayer::SarchJumpIce(void)
{
	// �v���C���[�̐悩���Ŕ�����Ƃ�

	if (true)
		StartJump();	// ����Ă�X������΃W�����v���J�n
}

//=====================================================
// �W�����v�̊J�n
//=====================================================
void CPlayer::StartJump(void)
{
	// �W�����v���[�V�����t���O�𗧂Ă�


	// ���͂𖳌���
	EnableInput(false);
}

//=====================================================
// �W�����v���̏���
//=====================================================
void CPlayer::StayJump(void)
{
	// �ڕW�̕X�Ɍ������Ĉړ�


	// �X�ɒ��n������W�����v���I��
	EndJump();
}

//=====================================================
// �W�����v�̏I��
//=====================================================
void CPlayer::EndJump(void)
{
	// �W�����v���[�V�����t���O��܂�


	// ���͂�L����
	EnableInput(true);
}

//=====================================================
// ���[�V�����̊Ǘ�
//=====================================================
void CPlayer::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinifh = IsFinish();

	if (m_fragMotion.bWalk)
	{// �������[�V�����t���O�L��
		if (nMotion != MOTION::MOTION_WALK)
			SetMotion(MOTION::MOTION_WALK);
	}
	else
	{// �����t���O�������Ă��Ȃ����
		if(nMotion != MOTION::MOTION_NEUTRAL)
			SetMotion(MOTION::MOTION_NEUTRAL);
	}
}

//=====================================================
// �q�b�g����
//=====================================================
void CPlayer::Hit(float fDamage)
{
	if (m_state == E_State::STATE_DEATH ||
		m_state == E_State::STATE_INVINCIBLE)
		return;	// �����ɂ����Hit�����𖳌���

	// ���S��Ԃɂ���
	m_state = E_State::STATE_DEATH;

	// �p�[�e�B�N���̔���
	D3DXVECTOR3 pos = GetPosition();
	CParticle::Create(pos, CParticle::TYPE::TYPE_HITENEMY);

	// ����\�t���O��܂�
	m_bEnableInput = false;
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CPlayer::Debug(void)
{
	CDebugProc *pDebugProc = CDebugProc::GetInstance();
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad *pJoypad = CInputJoypad::GetInstance();
	CInputManager *pInputMgr = CInputManager::GetInstance();

	if (pDebugProc == nullptr || pInputKeyboard == nullptr || pJoypad == nullptr)
		return;

	pDebugProc->Print("\n�c[%d]��[%d]", m_nGridV, m_nGridH);
	pDebugProc->Print("\n�ʒu[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);

	if (pInputMgr->GetTrigger(CInputManager::BUTTON_SETICE))
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