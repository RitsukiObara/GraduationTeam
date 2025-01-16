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
#include "texture.h"
#include "peckLine.h"
#include "sound.h"
#include "shadow.h"
#include "MyEffekseer.h"
#include "gameManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// �{�f�B�̃p�X

const float RATE_DECREASE_MOVE = 0.5f;	// �ړ������̊���
const float LINE_FACT_ROT = 0.3f;	// ������␳����܂ł̓��͂������l
const float FACT_ROTATION = 0.1f;	// ��]�W��

const float LINE_INPUT_MOVE = 0.3f;	// �ړ�����܂ł̃X�e�B�b�N���͂̂������l
const float RANGE_SELECT_ICE = D3DX_PI * 2 / 6;	// �X��I������Ƃ��̊p�x�͈̔�
const float RATE_CHANGE_GRID = 0.58f;	// ���̃O���b�h�Ɉڂ锻��̊���

const float TIME_MAX_SPEED = 1.0f;	// �ő呬�x�ɒB����܂łɂ����鎞��
const float SPEED_MOVE_MAX = 2.5f;	// �ő�ړ����x

const float LINE_STOP_TURN = 0.2f;	// �U��������~���邵�����l
const float LINE_START_TURN = D3DX_PI * 0.6f;	// �U��������J�n���邵�����l
const float FACT_ROTATION_TURN = 0.2f;	// �U�������]�W��

const float RANGE_ROT_FORWARD = D3DX_PI * 2 / CIceManager::E_Direction::DIRECTION_MAX;	// �O�i����̂ɔ��f����p�x

const float DEATH_VIBRATION_POWER = 0.7f;	// ���S���̐U������
const int DEATH_VIBRATION_TIME = 30;			// ���S���̐U������

const float PECK_VIBRATION_POWER = 0.5f;	// ���鎞�̐U������
const int PECK_VIBRATION_TIME = 10;			// ���鎞�̐U������

const float POW_VIB_BREAK = 0.8f;	// �󂵂����̐U������
const int TIME_VIB_BREAK = 50;		// �󂵂����̐U������

const float POW_VIB_FLOW = 0.7f;	// ������Ă鎞�̐U������
const int TIME_VIB_FLOW = 10;		// ������Ă鎞�̐U������

const float GRAVITY = 0.98f;	// �d��

const float HEIGHT_BLOW = 250.0f;	// ������΂��̍���

const float HEIGHT_UI = 0.1f;											// UI�̍���
const D3DXVECTOR2 SIZE_UI = { 0.03f,0.06f };							// UI�̃T�C�Y
const string PATH_PLAYERNUM = "data\\TEXTURE\\UI\\player_Count.png";	// �v���C���[�ԍ��e�N�X�`���p�X

//-------------------------------
// �W�����v�̒萔
//-------------------------------
namespace jump
{
const float RANGE_JUMP = D3DX_PI;	// �W�����v��̖ڕW�ɂ���p�x�͈̔�
const float FACT_MOVE = 0.07f;	// �ړ��W��
const float LINE_END = 10.0f;	// �I���Ǝ����������l
const float HEIGHT = 100.0f;	// ����
}

//-------------------------------
// ����UI�̒萔
//-------------------------------
namespace DirUI
{
const string PATH_TEX = "data\\TEXTURE\\UI\\dirPeck.png";	// �e�N�X�`���p�X
const float WIDTH = 70.0f;		// ��
const float HEIGHT = 130.0f;	// ����
const float FACT_ROT = 0.6f;	// ��]�W��
}

const D3DXCOLOR COL_LINE[NUM_PLAYER] =									// �F
{
	D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),
	D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),
	D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),
	D3DXCOLOR(0.0f,1.0f,1.0f,1.0f),
};
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
vector<CPlayer*> CPlayer::s_apPlayer;	// �i�[�p�̔z��

//=====================================================
// �R���X�g���N�^
//=====================================================
CPlayer::CPlayer(int nPriority) : m_nGridV(0), m_nGridH(0), m_state(STATE_NONE), m_pIceMoveDest(nullptr), m_bEnableInput(false), m_fTimerStartMove(0.0f),
m_fragMotion(), m_bTurn(false), m_fRotTurn(0.0f), m_pLandSystemFlow(nullptr), m_pLandFlow(nullptr), m_nTimePeck(0), m_nID(0), m_pPeckLine(nullptr),
m_bEnableJump(false), m_pIceDestJump(nullptr), m_posInitJump(), m_pShadow(nullptr), m_pUI(nullptr)
{
	// �f�t�H���g�͓��������̔ԍ�
	m_nID = (int)s_apPlayer.size();

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
	// �Ǎ�
	Load((char*)&PATH_BODY[0]);

	// �p���N���X�̏�����
	CMotion::Init();

	InitPose(0);

	// �O���b�h�ԍ��̏�����
	InitGridIdx();
	
	// ����UI����
	CreateDirUI();

	// ���͉\�t���O��ݒ�
	m_bEnableInput = true;

	// ��Ԑݒ�
#ifdef _DEBUG
	m_state = STATE_NORMAL;
#else
	m_state = STATE_NORMAL;
#endif // _DEBUG

	// �e�̐���
	m_pShadow = CShadow::Create();

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

	switch (m_nID)
	{
	case 0:
		pIceMgr->GetLeftUpIdx(&m_nGridV, &m_nGridH);
		break;
	case 1:
		pIceMgr->GetRightUpIdx(&m_nGridV, &m_nGridH);
		break;
	case 2:
		pIceMgr->GetLeftDownIdx(&m_nGridV, &m_nGridH);
		break;
	case 3:
		pIceMgr->GetRightDownIdx(&m_nGridV, &m_nGridH);
		break;
	default:
		break;
	}
	
	D3DXVECTOR3 pos = pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH);
	SetPosition(pos);
}

//=====================================================
// ����UI�̐���
//=====================================================
void CPlayer::CreateDirUI(void)
{
	m_pPeckLine = CPeckLine::Create(COL_LINE[GetID()], GetPosition());
}

//=====================================================
// �I������
//=====================================================
void CPlayer::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pPeckLine);
	Object::DeleteObject((CObject**)&m_pShadow);
	Object::DeleteObject((CObject**)&m_pUI);

	for (auto itr = s_apPlayer.begin(); itr < s_apPlayer.end(); itr++)
	{
		//�폜�Ώۂ���Ȃ��ꍇ
		if (*itr != this)
			continue;

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

	// ������΂����̏���
	StayBlow();

	// ���͏���
	Input();

	// �X���Ȃ�����
	JudgeNoIce();

	if (m_state == STATE_FLOW)
		StayFlow();	// �Y�����̏���

	if (m_pPeckLine != nullptr)
		m_pPeckLine->SetPosition(GetPosition());

	// �d�͂�������
	D3DXVECTOR3 move = GetMove();
	move.y -= GRAVITY;
	SetMove(move);

	// �X�̒Ǐ]
	FollowIce();

	// UI�̒Ǐ]
	FollowUI();

	// ���[�V�����̊Ǘ�
	ManageMotion();

	// �e�̒Ǐ]
	if (m_pShadow != nullptr)
		m_pShadow->SetPosition(GetPosition());

	// ���[�V�����X�V
	CMotion::Update();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �X���Ȃ�����
//=====================================================
void CPlayer::JudgeNoIce(void)
{
	if (m_state == E_State::STATE_FLOW || m_state == E_State::STATE_BLOW)
		return;	// �Y�����ł���Βʂ�Ȃ�

	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// ����Ă���X�̎擾
	int nIdx;
	bool bResult = pIceMgr->GetIdxGridFromPosition(GetPosition(),&nIdx, &nIdx);

	//if (!bResult)
		//Hit(0.0f);	// ����Ă���X���Ȃ������瑦��
}

//=====================================================
// �X�̒Ǐ]
//=====================================================
void CPlayer::FollowIce(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	if (pIceStand != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		if (pIceStand->GetPosition().y >= pos.y)
		{
			pos.y = pIceStand->GetPosition().y;
			SetPosition(pos);

			// �d�͂̃��Z�b�g
			D3DXVECTOR3 move = GetMove();
			SetMove(D3DXVECTOR3(move.x, 0.0f, move.z));
		}
	}
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
}

//=====================================================
// �A�i���O�ړ�
//=====================================================
void CPlayer::MoveAnalog(void)
{
	// �A�i���O�ړ�����
	InputMoveAnalog();

	// ������O���b�h�ԍ��̎擾
	CheckGridChange();

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
	}
	else
	{// �ʏ�̑O�i���
		// �O�i����
		Forward();

		// �U��Ԃ�̌��o
		JudgeTurn();

		// �����̕␳
		FactingRot();
	}

	// �ړ��ʂ̌���
	DecreaseMove();
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

	// �˂������[�V�������͍s��Ȃ�
	int nMotion = GetMotion();
	if (nMotion == MOTION_PECK || nMotion == MOTION_CANNOTPECK)
		return;

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
	Translate(move);
}

//=====================================================
// �ړ��ʌ���
//=====================================================
void CPlayer::DecreaseMove(void)
{
	D3DXVECTOR3 move = GetMove();

	// �ړ��ʂ̌���
	move.x *= RATE_DECREASE_MOVE;
	move.z *= RATE_DECREASE_MOVE;

	SetMove(move);
}

//=====================================================
// �����̕␳
//=====================================================
void CPlayer::FactingRot(void)
{
	if (m_pInputMgr == nullptr)
		return;

	// �˂������[�V�������͍s��Ȃ�
	int nMotion = GetMotion();
	if (nMotion == MOTION_PECK || nMotion == MOTION_CANNOTPECK)
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

	// �˂������[�V�������͍s��Ȃ�
	int nMotion = GetMotion();
	if (nMotion == MOTION_PECK || nMotion == MOTION_CANNOTPECK)
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
		// ���݂̌����Ɛ����΂�ڕW�̌����ɐݒ�
		m_fRotTurn = fAngleInput;
		universal::LimitRot(&m_fRotTurn);

		m_bTurn = true;	// �������l���z���Ă�����U��Ԃ锻��
	}
}

//=====================================================
// �X�Ƃ̔���
//=====================================================
void CPlayer::CollideIce(void)
{
	if (m_state != E_State::STATE_NORMAL)
		return;

	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posTemp = pos;
	
	CIce *pIceForward = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	// �O���b�h�̈ʒu�ɍ��킹��
	pIceMgr->Collide(&pos, m_nGridV, m_nGridH,RATE_CHANGE_GRID);

	pos.y = posTemp.y;

	SetPosition(pos);
}

//=====================================================
// �O���ɕX�����邩�̔���
//=====================================================
bool CPlayer::CheckAnyIceForward(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	// ���ӂ̕X�̎擾
	vector<CIce*> apIce = pIceMgr->GetAroundIce(m_nGridV, m_nGridH);

	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	if (m_pInputMgr == nullptr)
		return false;

	// �ڕW�����̐ݒ�
	CInputManager::S_Axis axis = m_pInputMgr->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	float fRotInput = atan2f(axisMove.x, axisMove.z);
	universal::LimitRot(&fRotInput);

	CDebugProc::GetInstance()->Print("\n�X�e�B�b�N�p�x[%f]", fRotInput);

	for (int i = 0; i < (int)apIce.size(); i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (apIce[i]->IsPeck())
			continue;

		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 rotPlayer = GetRotation();
		D3DXVECTOR3 posIce = apIce[i]->GetPosition();

		rotPlayer.y += D3DX_PI;
		universal::LimitRot(&rotPlayer.y);

		// ����ɂ�������^�[�Q�b�g�ɂ���
		if (universal::IsInFanTargetYFlat(posPlayer, posIce, fRotInput, RANGE_ROT_FORWARD))
		{
			return true;
		}
	}

	return false;
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

		//fRot -= D3DX_PI * 0.5f;
		//universal::LimitRot(&fRot);

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
	if (m_state == STATE_FLOW)
		return false;	// �Y�����͓���Ȃ�

	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	int nIdxV = -1;
	int nIdxH = -1;

	// �O���b�h�ԍ��̎擾
	D3DXVECTOR3 posNext = GetPosition() + GetMove();
	if (!pIceMgr->GetIdxGridFromPosition(posNext, &nIdxV, &nIdxH, RATE_CHANGE_GRID))
		return false;	// �O���b�h�ԍ��擾���s�ŋU��Ԃ�

	CIce *pIceForward = pIceMgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIceForward == nullptr)
		return false;

	if (pIceForward->IsPeck())
		return false;

	if (IsOnTopAnyPlayer(nIdxV, nIdxH,this))
		return false;

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

		return true;
	}
}

//=====================================================
// �Y���̊J�n
//=====================================================
bool CPlayer::StartFlows(void)
{
	if (FindFlowIce())
	{// �Y������X��������΁A�Y����Ԃֈڍs
		m_state = E_State::STATE_FLOW;
		
		if (m_pPeckLine != nullptr)
			m_pPeckLine->EndMove();

		return true;
	}

	return false;
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
			int nIdxIceV;
			int nIdxIceH;
			pIceMgr->GetIceIndex(itIce, &nIdxIceV, &nIdxIceH);

			int nIdxPlayerV = GetGridV();
			int nIdxPlayerH = GetGridH();

			if(nIdxPlayerV == nIdxIceV && nIdxPlayerH == nIdxIceH)
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

		return;
	}

	// ���X���Ɉʒu�𐧌�
	LimitInSideFlowIce();
	
	// �R���g���[���[��U��������
	VibJoypad(POW_VIB_FLOW, TIME_VIB_FLOW);

	// �Y�����̎�
	FlowDeath();
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
			//pIceMgr->Collide(&posPlayer, itIce, 0.68f);	// �����̈�������Ă�����Ɨ��X�ŕ����邩��
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
// �Y�����̎�
//=====================================================
void CPlayer::FlowDeath(void)
{
	D3DXVECTOR3 pos = GetPosition();

	if (!universal::IsInScreen(pos))
		Hit(0.0f);
}

//=====================================================
// �Y���̏I��
//=====================================================
void CPlayer::EndFlows(void)
{
	if (m_pLandSystemFlow != nullptr)
		m_pLandSystemFlow->GetNerestGrid(GetPosition(), &m_nGridV, &m_nGridH);

	m_state = E_State::STATE_NORMAL;
	m_pLandSystemFlow = nullptr;
}

//=====================================================
// �˂����̓���
//=====================================================
void CPlayer::InputPeck(void)
{
	int nMotion = GetMotion();

	if (nMotion == MOTION_CANNOTPECK || nMotion == MOTION_PECK)
		return;

	if (m_pInputMgr == nullptr)
		return;

	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// �˂����邩�`�F�b�N
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 pos = GetPosition();

	rot.y += D3DX_PI;
	universal::LimitRot(&rot.y);

	CIceManager::E_Direction dir;

	CIce *pIcePeck = nullptr;
	CIceManager::E_PeckResult pr = pIceManager->CheckPeck(m_nGridV, m_nGridH, rot.y, pos, &dir, &pIcePeck);

	if (pr == CIceManager::PECK_OK && m_state == E_State::STATE_NORMAL)
	{// �˂�����Ƃ�
		// �����{�^���g���K�[�ŕX������
		if (m_pInputMgr->GetTrigger(CInputManager::BUTTON_PECK))
			SetMotion(MOTION::MOTION_PECK);

		if (m_pPeckLine != nullptr)
		{
			if (pIcePeck != nullptr)
			{
				D3DXVECTOR3 posIce = pIcePeck->GetPosition();

				m_pPeckLine->StartMove(posIce);
			}
		}
	}
	else
	{// �˂����Ȃ��Ƃ�
		if (pr == CIceManager::PECK_TOPANYOBJECT)
		{// �����������Ă���
			// �{�^���������Ă��e�����
			if (m_pInputMgr->GetTrigger(CInputManager::BUTTON_PECK))
				SetMotion(MOTION::MOTION_CANNOTPECK);
		}
		else if (pr == CIceManager::PECK_UNBREAKABLE)
		{// �󂹂Ȃ��X�̏ꍇ
			D3DXVECTOR3 posIce = pIcePeck->GetPosition();
			// �{�^���������Ă��e�����
			if (m_pInputMgr->GetTrigger(CInputManager::BUTTON_PECK))
			{
				MyEffekseer::CreateEffect(CMyEffekseer::TYPE_SPARK, posIce);
				SetMotion(MOTION::MOTION_CANNOTPECK);
			}
		}
		// ��������˂����Ȃ��ꍇ�͐����o���Ȃ�
		if (m_pPeckLine != nullptr)
			m_pPeckLine->EndMove();
	}
}

//=====================================================
// �˂����̏���
//=====================================================
bool CPlayer::Peck(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// �˂�������
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 pos = GetPosition();

	rot.y += D3DX_PI;
	universal::LimitRot(&rot.y);

	bool bResultBreak;

	if (pIceManager->PeckIce(m_nGridV, m_nGridH, rot.y, pos,&bResultBreak))
		m_nTimePeck++;

	// �����̃R���g���[���[�U��
	VibJoypad(PECK_VIBRATION_POWER, PECK_VIBRATION_TIME);

	if(bResultBreak)	// �j�󎞂̃R���g���[���[�U��
		VibJoypad(POW_VIB_BREAK, TIME_VIB_BREAK);

	return bResultBreak;
}

//=====================================================
// �W���C�p�b�h��U��������
//=====================================================
void CPlayer::VibJoypad(float fPow, int nFrame)
{
	CInputJoypad* pInputJoypad = CInputJoypad::GetInstance();

	if (pInputJoypad == nullptr)
		return;

	// joypad�U��������
	pInputJoypad->Vibration(m_nID, fPow, nFrame);
}

//=====================================================
// ����UI�̉�]
//=====================================================
void CPlayer::RotationDirUI(int nDir)
{
#if 0
	if (m_pDir == nullptr)
		return;

	D3DXVECTOR3 rot = m_pDir->GetRotation();

	float fRate = D3DX_PI * 2 / CIceManager::E_Direction::DIRECTION_MAX;

	float fRotDest = D3DX_PI * 1.5f;
	fRotDest += fRate * (nDir - 1);

	universal::FactingRot(&rot.y, fRotDest, DirUI::FACT_ROT);

	m_pDir->SetRotation(rot);
#else

#endif
}

//=====================================================
// �W�����v�̓���
//=====================================================
void CPlayer::InputJump(void)
{
	if (m_pInputMgr == nullptr)
		return;

	SarchJumpIce();	// �W�����v��̕X��T��
}

//=====================================================
// �W�����v��̕X��T��
//=====================================================
void CPlayer::SarchJumpIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	CIce *pIceTarget = nullptr;

	//-------------------------------
	// �����Ă�������̕X���擾
	//-------------------------------
	D3DXVECTOR3 rotPlayer = GetRotation();

	rotPlayer.y += D3DX_PI;
	universal::LimitRot(&rotPlayer.y);

	CIce *pIceForward = SelectIceByRot(rotPlayer.y);

	if (pIceForward == nullptr)
		return;

	if (!pIceForward->IsPeck())
		return;

	// �X�̔ԍ��擾
	int nIdxV;
	int nIdxH;
	pIceMgr->GetIceIndex(pIceForward, &nIdxV, &nIdxH);

	vector<CIce*> apIce = pIceMgr->GetAroundIce(nIdxV, nIdxH);

	//-------------------------------
	// �����Ă���X�̎��ӂ��`�F�b�N
	//-------------------------------
	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);
	float fDiffMin = D3DX_PI * 2;
	for (int i = 0; i < (int)apIce.size(); i++)
	{
		if (pIceStand == nullptr)
			return;

		if (apIce[i] == nullptr)
			continue;

		if (apIce[i]->IsPeck())
			continue;

		if (pIceStand == apIce[i])
			continue;

		int nIdxJumpV;
		int nIdxJumpH;
		pIceMgr->GetIceIndex(apIce[i], &nIdxJumpV, &nIdxJumpH);
		if (IsOnTopAnyPlayer(nIdxJumpV, nIdxJumpH, this))
			continue;

		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 posIce = apIce[i]->GetPosition();

		// �����x�N�g���̊p�x���擾
		D3DXVECTOR3 vecDiff = posIce - pIceStand->GetPosition();
		float fRotToTarget = atan2f(vecDiff.x, vecDiff.z);

		// �����p�x���͈͓����ǂ����擾
		float fRotDiff = fRotToTarget - rotPlayer.y;
		universal::LimitRot(&fRotDiff);

		// ����ɂ�������^�[�Q�b�g�ɂ���
		if (universal::IsInFanTargetYFlat(pIceStand->GetPosition(), posIce, rotPlayer.y, jump::RANGE_JUMP))
		{
			if (fRotDiff * fRotDiff < fDiffMin * fDiffMin)
			{
 				fDiffMin = fRotDiff;
				pIceTarget = apIce[i];
			}
		}
	}

	//-------------------------------
	// �W�����v�̑���
	//-------------------------------
	m_pIceDestJump = pIceTarget;

	if (pIceTarget == nullptr)
		return;	// ����������Ȃ������珈����ʂ�Ȃ�

	if (m_pInputMgr->GetTrigger(CInputManager::BUTTON_JUMP))
	{
		pIceMgr->GetIceIndex(pIceTarget, &m_nGridV, &m_nGridH);
		StartJump();	// ����Ă�X������΃W�����v���J�n
	}
}

//=====================================================
// �W�����v�̊J�n
//=====================================================
void CPlayer::StartJump(void)
{
	// �W�����v���[�V�����t���O�𗧂Ă�
	m_fragMotion.bJump = true;

	// ���͂𖳌���
	EnableInput(false);

	// �W�����v�����ʒu�̕ۑ�
	m_posInitJump = GetPosition();
}

//=====================================================
// �W�����v���̏���
//=====================================================
void CPlayer::StayJump(void)
{
	if (m_pIceDestJump == nullptr && m_state != E_State::STATE_BLOW)
		return;

	JumpToDest(m_pIceDestJump, jump::HEIGHT);

	// �I���̔���
	D3DXVECTOR3 posPlayer = GetPosition();
	D3DXVECTOR3 posIce = m_pIceDestJump->GetPosition();

	if (universal::DistCmpFlat(posPlayer, posIce, jump::LINE_END, nullptr))
	{// �X�ɒ��n������W�����v���I��
		EndJump();
	}
}

//=====================================================
// �ڕW�̕X�Ɍ������Ĕ�ԏ���
//=====================================================
void CPlayer::JumpToDest(CIce *pIceDest, float fHeightJump)
{
	//--------------------------------
	// �ڕW�̕X�Ɍ������Ĉړ�
	//--------------------------------
	D3DXVECTOR3 posPlayer = GetPosition();
	D3DXVECTOR3 posIce = pIceDest->GetPosition();
	universal::MoveToDest(&posPlayer, posIce, jump::FACT_MOVE);

	SetPosition(posPlayer);

	//--------------------------------
	// �����̊Ǘ�
	//--------------------------------
	// �����ʒu����ڕW�ʒu�܂ł̋����Ō��݂̊������v�Z
	D3DXVECTOR3 vecDiffMax = posIce - m_posInitJump;
	float fLengthMax = sqrtf(vecDiffMax.x * vecDiffMax.x + vecDiffMax.z * vecDiffMax.z);

	D3DXVECTOR3 vecDiff = posPlayer - m_posInitJump;
	float fLength = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

	float fRate = fLength / fLengthMax;

	// �������̌v�Z
	float fHeight = universal::ParabolaY(fRate - 0.5f, 10.0f);

	if (std::isnan(fHeight))
		fHeight = 0;	// �܂����������ꏊ�ɔ�΂��ꂽ�Ƃ��̖h�~

	// �������̕␳
	fHeight *= -fHeightJump;
	fHeight += fHeightJump * 2;

	// �����𔽉f
	D3DXVECTOR3 pos = posPlayer;
	pos.y = m_posInitJump.y + fHeight;
	SetPosition(pos);
}

//=====================================================
// �W�����v�̏I��
//=====================================================
void CPlayer::EndJump(void)
{
	// �G�t�F�N�g����
	D3DXVECTOR3 posPlayer = GetPosition();
	MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_LANDING, posPlayer);

	// �W�����v���[�V�����t���O��܂�
	m_fragMotion.bJump = false;

	if (CGame::GetState() == CGame::E_State::STATE_NORMAL || CGame::GetInstance() == nullptr)
		EnableInput(true);

	m_pIceDestJump = nullptr;
}

//=====================================================
// ������΂��̊J�n
//=====================================================
void CPlayer::StartBlow(CIce *pIce)
{
	// ����s�\�ɂ���
	EnableInput(false);

	// ������΂���Ԃɂ���
	m_state = E_State::STATE_BLOW;

	// �����ʒu��ݒ�
	m_posInitJump = GetPosition();

	// �ڕW�̕X���擾
	m_pIceDestJump = pIce;

	// �ڕW�̃O���b�h�ԍ����擾
	CIceManager* pIceMgr = CIceManager::GetInstance();
	if (pIceMgr == nullptr)
		return;

	pIceMgr->GetIceIndex(m_pIceDestJump, &m_nGridV, &m_nGridH);

	if (m_pPeckLine != nullptr)
		m_pPeckLine->EndMove();
}

//=====================================================
// ������΂����̏���
//=====================================================
void CPlayer::StayBlow(void)
{
	if (m_state != E_State::STATE_BLOW)
		return;

	JumpToDest(m_pIceDestJump, HEIGHT_BLOW);

	// �I���̔���
	D3DXVECTOR3 posPlayer = GetPosition();
	D3DXVECTOR3 posIce = m_pIceDestJump->GetPosition();

	if (universal::DistCmpFlat(posPlayer, posIce, jump::LINE_END, nullptr))
	{// �X�ɒ��n�����痧���オ�胂�[�V����
		int nMotion = GetMotion();
		if (nMotion != MOTION::MOTION_STANDUP)
		{
			SetMotion(MOTION::MOTION_STANDUP);
			MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_LANDING, posPlayer);
		}
	}
}

//=====================================================
// ������΂��̏I��
//=====================================================
void CPlayer::EndBlow(void)
{
	if (m_pIceDestJump == nullptr)
		return;

	if(CGame::GetState() == CGame::E_State::STATE_NORMAL || CGame::GetInstance() == nullptr)
		EnableInput(true);	// ���͂�L����

	// �ڕW�X�����Z�b�g
	m_pIceDestJump = nullptr;

	// ��Ԃ�ʏ�ɂ���
	m_state = E_State::STATE_NORMAL;

	// �W�����v�t���O��܂�
	m_bEnableJump = false;
	m_fragMotion.bJump = false;
}

//=====================================================
// �����̕\��
//=====================================================
void CPlayer::ShowNumber(void)
{
	// UI�̐���
	m_pUI = CUI::Create();

	if (m_pUI == nullptr)
		return;

	m_pUI->SetSize(SIZE_UI.x, SIZE_UI.y);

	// �e�N�X�`���ݒ�
	int nID = GetID();
	int nIdx = Texture::GetIdx(&PATH_PLAYERNUM[0]);
	float fRateOnePlayer = 1.0f / NUM_PLAYER;
	m_pUI->SetTex(D3DXVECTOR2(0.0f + fRateOnePlayer * nID, 0.0f), D3DXVECTOR2(fRateOnePlayer + fRateOnePlayer * nID, 1.0f));
	m_pUI->SetIdxTexture(nIdx);
	m_pUI->SetVtx();
}

//=====================================================
// ���[�V�����̊Ǘ�
//=====================================================
void CPlayer::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();
	
	if (nMotion == MOTION::MOTION_GUTS)
	{

	}
	else if (m_state == E_State::STATE_BLOW)
	{
		if (nMotion == MOTION::MOTION_STANDUP)
		{
			if (bFinish)
			{
				EndBlow();
			}
		}
		else if (nMotion != MOTION::MOTION_BLOW)
			SetMotion(MOTION::MOTION_BLOW);
	}
	else if (nMotion == MOTION::MOTION_MULTIAPPEAR && !bFinish)
	{

	}
	else if (m_fragMotion.bJump)
	{// �W�����v��
		if (nMotion == MOTION::MOTION_LANDING)
		{// ���n���[�V�����I���ŃW�����v�I��
			if(bFinish)
				EndJump();
		}
		else if (nMotion == MOTION::MOTION_STAYJUMP)
		{// �؋󃂁[�V������
			// �؋󒆂̏���
			StayJump();
		}
		else if (nMotion == MOTION::MOTION_STARTJUMP)
		{// �W�����v�J�n���[�V�����I������̑J��
			if (bFinish)
			{
				SetMotion(MOTION::MOTION_STAYJUMP);
				CSound::GetInstance()->Play(CSound::LABEL_SE_PENGUIN_JUMP);
				MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_JUMP, GetPosition());
			}
		}
		else if (nMotion != MOTION::MOTION_STARTJUMP)	// �W�����v�J�n���[�V�����̊J�n
			SetMotion(MOTION::MOTION_STARTJUMP);
	}
	else if ((nMotion == MOTION_PECK || nMotion == MOTION_CANNOTPECK) && !bFinish)
	{

	}
	else if (m_fragMotion.bWalk)
	{// �������[�V�����t���O�L��
		if (nMotion != MOTION::MOTION_WALK)
			SetMotion(MOTION::MOTION_WALK);
	}
	else if (nMotion == MOTION::MOTION_VICTORY)
	{

	}
	else
	{// �����t���O�������Ă��Ȃ����
		if(nMotion != MOTION::MOTION_NEUTRAL)
			SetMotion(MOTION::MOTION_NEUTRAL);
	}
}

//=====================================================
// UI�̒Ǐ]
//=====================================================
void CPlayer::FollowUI(void)
{
	if (m_pUI == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 posScreen;
	universal::IsInScreen(pos, &posScreen);
	universal::ConvertScreenRate(posScreen);

	m_pUI->SetPosition(D3DXVECTOR3(posScreen.x, posScreen.y - HEIGHT_UI, 0.0f));
	m_pUI->SetVtx();
}

//=====================================================
// ���[�V�����C�x���g
//=====================================================
void CPlayer::Event(EVENT_INFO* pEventInfo)
{
	int nMotion = GetMotion();

	if (nMotion == MOTION::MOTION_PECK)
	{
		// �˂�������
		Peck();
	}

	if (nMotion == MOTION::MOTION_CANNOTPECK)
	{
		// ����炷
		Sound::Play(CSound::LABEL::LABEL_SE_NOTPECK);
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
	CParticle::Create(pos, CParticle::TYPE::TYPE_STAR_HIT);

	// ����\�t���O��܂�
	m_bEnableInput = false;

	// joypad�U��������
	VibJoypad(DEATH_VIBRATION_POWER, DEATH_VIBRATION_TIME);

	// �y���M���̖���
	CSound::GetInstance()->Play(CSound::LABEL_SE_PENGUIN_VOICE00);
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

	if (pDebugProc == nullptr || pInputKeyboard == nullptr || pJoypad == nullptr || pInputMgr == nullptr || m_pShadow == nullptr)
		return;

#if 1
	pDebugProc->Print("\n�v���C���[���==========================");
	pDebugProc->Print("\n�c[%d]��[%d]", m_nGridV, m_nGridH);
	pDebugProc->Print("\n�ʒu[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	pDebugProc->Print("\n�ړ���[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);
	pDebugProc->Print("\n���[%d]", GetState());

	if (m_state == STATE_INVINCIBLE)
	{
		pDebugProc->Print("\n<<���G�iF8�Œʏ�j>>");
	}
	else
	{
		pDebugProc->Print("\n<<�ʏ�iF8�Ŗ��G�j>>");
	}
#endif

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

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	CEffect3D::Create(pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH), 50.0f, 5, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayer::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}

//=====================================================
// �e�̈ʒu�ݒ�
//=====================================================
void CPlayer::SetShadowPos(D3DXVECTOR3 pos)
{
	if (m_pShadow != nullptr)
		m_pShadow->SetJustPosition(pos);
}

//=====================================================
// �S�v���C���[�̓��̓t���O�ݒ�
//=====================================================
void CPlayer::EnableInputAll(bool bValue)
{
	for (CPlayer *pPlayer : s_apPlayer)
		pPlayer->EnableInput(bValue);
}

//=====================================================
// �S�v���C���[�ɓ��͂�����U��
//=====================================================
void CPlayer::BindInputAllPlayer(void)
{
	// ���͔ԍ��擾
	vector<int> aIdxInput;
	gameManager::LoadIdxInput(aIdxInput);

	for (int i = 0; i < NUM_PLAYER; i++)
		CInputManager::Create();

	for (int i = 0; i < (int)s_apPlayer.size(); i++)
	{
		if (s_apPlayer[i] == nullptr)
			return;

		CInputManager *pInputMgr = CInputManager::GetInstance(aIdxInput[i]);
		if (pInputMgr == nullptr)
			continue;

		s_apPlayer[i]->BindInputMgr(pInputMgr);
	}
}

//=====================================================
// �S�v���C���[�̕Y���J�n�`�F�b�N
//=====================================================
void CPlayer::CheckStartDriftAll(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();
	if (pIceMgr == nullptr)
		return;

	for (CPlayer *pPlayer : s_apPlayer)
	{
		int nIdxV = -1;
		int nIdxH = -1;

		/*if (!pIceMgr->GetIdxGridFromPosition(pPlayer->GetPosition(), &nIdxV, &nIdxH))
			continue;*/

		if (pIceMgr->GetGridIce(&nIdxV, &nIdxH) != nullptr)
			continue;

		// �X��������ΕY���J�n
		pPlayer->StartFlows();
	}
}

//=====================================================
// �v���C���[���X�̏�ɏ���Ă��锻��
//=====================================================
bool CPlayer::IsOnTopAnyPlayer(int nIdxV, int nIdxH, CPlayer *pPlayer)
{
	for (CPlayer *pPlayerTarget : s_apPlayer)
	{
		if (pPlayerTarget == pPlayer)
			continue;

		int nIdxPlayerV = pPlayerTarget->GetGridV();
		int nIdxPlayerH = pPlayerTarget->GetGridH();

		if (nIdxPlayerV == nIdxV && nIdxPlayerH == nIdxH)
			return true;
	}

	return false;
}