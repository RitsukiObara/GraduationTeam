//*****************************************************
//
// �G�̏���[enemy.cpp]
// Author:����F�M
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemy.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "iceManager.h"
#include "flowIce.h"
#include "debugproc.h"
#include "seals.h"
#include "bears.h"
#include "ocean.h"
#include "destroy_score.h"
#include "UI_combo.h"
#include "effect3D.h"
#include "manager.h"
#include "sound.h"
#include "shadow.h"
#include "player.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float HEIGHT_ICE = 100.0f;	// �X�̍���
const float SPPED_MOVE_INIT = 1.6f;	// �����ړ����x
const float SPEED_ROTATION = 0.05f;	// ��]���x
const float TIME_DEATH_IN_DRIFT = 6.0f;	// �Y�����Ď��ʂ܂ł̎���

const float LINE_STOP_TURN = 0.2f;	// �U��������~���邵�����l
const float LINE_START_TURN = D3DX_PI * 0.6f;	// �U��������J�n���邵�����l
const float FACT_ROTATION_TURN = 0.07f;	// �U�������]�W��

const float LINE_ENABLE_MOVE = 0.1f;	// �ړ��J�n�ł���p�x�̂������l

const float RATE_STOP_FLOW_ICE_RADIUS = 0.5f;	// �Y����~����ۂɌ��o����X�̔��a�̊���

const float RATE_STOP_CHARGE = 0.6f;	// �ːi���~�߂�Ƃ��̕X�̃T�C�Y�̊���
const float RANGE_STOP_MOVE = D3DX_PI * 1 / CIceManager::E_Direction::DIRECTION_MAX;	// �ړ����~�߂�p�x�͈̔�

const float RATE_COLLIDEICE = 1.0f;	// �X�Ƃ̔���̊���

const int MAX_DEPTH_FIND = 50;	// ����ȏ�T�����Ȃ��[��

const float LINE_FACT_ROT = 10.0f;	// ����ȏ�̒������Ȃ��ƌ�����␳���Ȃ�
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
std::vector<CEnemy*> CEnemy::s_vector = {};	// ���g�̃|�C���^

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CEnemy::CEnemy(int nPriority) : m_nGridV(0), m_nGridH(0), m_state(E_State::STATE_NONE), m_pIceLand(nullptr), m_bFollowIce(false),
m_move(), m_nGridVDest(0), m_nGridHDest(0), m_fSpeedMove(0.0f), m_fFactRot(0.0f), m_fTimerDeath(0.0f), m_bTurn(false), m_bEnableMove(false), m_pLandSystemFlow(nullptr),
m_bMoveByGrid(false), m_pShadow(nullptr), m_spawn(E_Spawn::SPAWN_RU), m_type(TYPE_SEALS)
{
	s_vector.push_back(this);
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemy::~CEnemy()
{

}

//=====================================================
// ��������
//=====================================================
CEnemy* CEnemy::Create(int nType, E_Spawn spawn)
{
	CEnemy* pEnemy = nullptr;

	switch (nType)
	{
	case CEnemy::TYPE_SEALS:
		pEnemy = new CSeals;
		break;
	case CEnemy::TYPE_BEARS:
		pEnemy = new CBears;
		break;
	default:
		assert(false);
		break;
	}

	if (pEnemy != nullptr)
	{// �G����
		// �O���b�h�ԍ�������
		pEnemy->InitGridIdx(spawn);
		pEnemy->m_spawn = spawn;

		pEnemy->m_type = (TYPE)nType;

		// ����������
		pEnemy->Init();

		// �C�����яo���T�E���h
		CSound* pSound = CSound::GetInstance();
		if (pSound != nullptr)
		{
			pSound->Play(CSound::LABEL_SE_SEA_SPLASH_01);
			pSound->Play(CSound::LABEL_SE_SEA_SPLASH_02);
		}
	}

	return pEnemy;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemy::Init(void)
{
	// �p���N���X�̏�����
	CMotion::Init();

	// ��ԏ�����
	m_state = E_State::STATE_APPER;

	// �o�����̃g�����X�t�H�[���ݒ�
	SetApperTransform();

	// �t���O��ݒ�
	m_bFollowIce = false;
	m_bEnableMove = true;
	m_bMoveByGrid = true;

	// �p�����[�^�[�̏����ݒ�
	m_fSpeedMove = SPPED_MOVE_INIT;
	m_fFactRot = SPEED_ROTATION;

	// �e�̐���
	m_pShadow = CShadow::Create();
	if (m_pShadow != nullptr)
		m_pShadow->SetPosition(GetPosition());

	return S_OK;
}

//=====================================================
// �O���b�h�ԍ��̏�����
//=====================================================
void CEnemy::InitGridIdx(E_Spawn spawn)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	if (pIceMgr->GetGridIce(&m_nGridV, &m_nGridH) != nullptr)
		return;

	switch (spawn)
	{
	case CEnemy::SPAWN_RU:
		pIceMgr->GetRightUpIdx(&m_nGridV, &m_nGridH);
		break;
	case CEnemy::SPAWN_LU:
		pIceMgr->GetLeftUpIdx(&m_nGridV, &m_nGridH);
		break;
	case CEnemy::SPAWN_RD:
		pIceMgr->GetRightDownIdx(&m_nGridV, &m_nGridH);
		break;
	case CEnemy::SPAWN_LD:
		pIceMgr->GetLeftDownIdx(&m_nGridV, &m_nGridH);
		break;
	default:
		assert(false);
		break;
	}

	m_nGridVNext = m_nGridV;
	m_nGridHNext = m_nGridH;

	D3DXVECTOR3 pos = pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH);
	SetPosition(pos);
}

//=====================================================
// �I������
//=====================================================
void CEnemy::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pShadow);

	for (auto itr = s_vector.begin(); itr < s_vector.end(); itr++ )
	{
		//�폜�Ώۂ���Ȃ��ꍇ
		if (*itr != this)
			continue;

		//Vector����폜
		s_vector.erase(itr);

		break;
	}

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemy::Update(void)
{
	CMotion::Update();

	// ��Ԃɉ������X�V
	UpdateState updateFuncs[CEnemy::E_State::STATE_MAX] =
	{
		nullptr,
		&CEnemy::UpdateApper,
		&CEnemy::UpdateStop,
		&CEnemy::UpdateMove,
		&CEnemy::UpdateAttack,
		&CEnemy::UpdateDrift
	};

	if (updateFuncs[m_state] != nullptr)
		(this->*updateFuncs[m_state])();

	// ���S�ȕX��T������
	SarchSafeIce();

	if (m_bFollowIce)
		FollowIce();	// �X�ɒǏ]

	// �ړ��ʂ��ʒu�ɉ��Z
	Translate(m_move);

	// �Y���J�n�̔���
	StartFlows();

	// �X�Ƃ̔���
	CollideIce();

	// �e�̒Ǐ]
	if (m_pShadow != nullptr)
		m_pShadow->SetPosition(GetPosition());

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// ���S�ȕX��T��
//=====================================================
void CEnemy::SarchSafeIce(void)
{
	if (m_state == E_State::STATE_APPER)
		return;

	CIceManager *pIceMgr = CIceManager::GetInstance();
	if (pIceMgr == nullptr)
		return;

	CIce *pIce = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	D3DXVECTOR3 pos = GetPosition();
	if (pIce == nullptr)
	{
		pIceMgr->GetNearestIce(pos, &m_nGridV, &m_nGridH);
		return;
	}

	if(pIce->IsPeck())
		pIceMgr->GetNearestIce(pos, &m_nGridV, &m_nGridH);
}

//=====================================================
// �X�ɒǏ]
//=====================================================
void CEnemy::FollowIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	// �X�̍����ɍ��킹��
	CIce *pIceStand = CIce::GetNearestIce(pos);
	
	if (pIceStand != nullptr)
		pos.y = pIceStand->GetPosition().y;	// ���������킹��
	else
		return;

	SetPosition(pos);
}

//=====================================================
// ��~���̓���
//=====================================================
void CEnemy::UpdateStop(void)
{
	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=====================================================
// �ړ����̓���
//=====================================================
void CEnemy::UpdateMove(void)
{
	MoveByGrid();		// �O���b�h��̈ړ�
}

//=====================================================
// �O���b�h��̈ړ�
//=====================================================
void CEnemy::MoveByGrid(void)
{
	// �ڕW�ɋ߂��X��T��
	SarchNearIceToDest();

	// �U������̌��o
	JudgeTurn();

	// ���̃O���b�h�Ɍ���������
	MoveToNextGrid();

	// �O���b�h���ڂ������̃`�F�b�N
	CheckChangeGrid();
}

//=====================================================
// �ڕW�ɋ߂��X��T��
//=====================================================
void CEnemy::SarchNearIceToDest(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;
	
	vector<CIce*> apIce = pIceMgr->GetAroundIce(m_nGridV, m_nGridH);

	// �ڕW�ʒu�̎擾
	D3DXVECTOR3 posDest = pIceMgr->GetGridPosition(&m_nGridVDest, &m_nGridHDest);

	// �S�����̕X�`�F�b�N
	float fLengthMin = FLT_MAX;

	int aV[CIceManager::DIRECTION_MAX] = {};
	int aH[CIceManager::DIRECTION_MAX] = {};

	// ���ӂ̃O���b�h�ԍ��̌v�Z
	Grid::CalcAroundGrids(m_nGridV, m_nGridH, aV, aH);

	// �T���t���O�p�̕X�̔z��
	vector<CIce*> apIceSave[CIceManager::DIRECTION_MAX];

	// ���ݗ����Ă���X�̎擾
	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	CIce *pIceNext = nullptr;

	// ��ԃR�X�g�̒Ⴂ�ׂ荇���X���`�F�b�N
	size_t sizeMin = UINT_MAX;	// �ŏ��R�X�g�̗p��
	for (int i = 0; i < CIceManager::DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (enemy::IsEnemyOnIce(aV[i], aH[i]))
			continue;

		if (apIce[i]->IsPeck())
			continue;

		// �����Ă���X��ǉ����Ă���
		apIceSave[i].push_back(pIceStand);

		// �o�H�̒T��
		bool bFindPath = PathFind(aV[i], aH[i], apIceSave[i]);

		if (!bFindPath)
		{// �o�H���s���l�肾������z����N���A
			apIceSave[i].clear();
		}
		else
		{// ����������A�R�X�g���r����
			if (sizeMin <= apIceSave[i].size())
				continue;

			// �R�X�g���ł�������������ۑ�
			sizeMin = apIceSave[i].size();

			pIceNext = apIceSave[i][1];
		}
	}

	if (pIceNext != nullptr)
	{// ���̕X�������ł����炻�̔ԍ������̔ԍ��ɂ���
		pIceMgr->GetIceIndex(pIceNext, &m_nGridVNext, &m_nGridHNext);
	}
}

//=====================================================
// �T���̍ċA�֐�
//=====================================================
bool CEnemy::PathFind(int nIdxV, int nIdxH, vector<CIce*>& rIceSave)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	if (rIceSave.size() > MAX_DEPTH_FIND)
		return false;

	// ���ӃO���b�h�̌v�Z
	vector<CIce*> apIce(CIceManager::DIRECTION_MAX);
	int aV[CIceManager::DIRECTION_MAX] = {};
	int aH[CIceManager::DIRECTION_MAX] = {};

	// ���ӂ̃O���b�h�ԍ��̌v�Z
	Grid::CalcAroundGrids(nIdxV, nIdxH, aV, aH);

	// �T���ςݔz��ɓ����
	CIce *pIceFind = pIceMgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIceFind == nullptr)
		return false;

	rIceSave.push_back(pIceFind);

	// �ڕW�ɓ������Ă�����^��Ԃ�
	if (nIdxV == m_nGridVDest && nIdxH == m_nGridHDest)
		return true;

	// �O���b�h�ԍ����͈͊O�ɂ����ĂȂ����̃`�F�b�N
	int nNumV = pIceMgr->GetNumGridV();
	int nNumH = pIceMgr->GetNumGridH();

	for (int i = 0; i < CIceManager::DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValue(&nV, nNumV - 1, 0) &&
			!universal::LimitValue(&nH, nNumH - 1, 0))
		{// �w�肵���ԍ����O���b�h���z���Ă��Ȃ��ꍇ�̂ݕۑ�
			apIce[i] = pIceMgr->GetGridIce(&aV[i], &aH[i]);
		}
	}

	D3DXVECTOR3 posDest = pIceMgr->GetGridPosition(&m_nGridVDest, &m_nGridHDest);
	float fDistMin = FLT_MAX;

	int nIdxMin = 0;
	for (int i = 0; i < CIceManager::DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;	// ��񂪂Ȃ��Ȃ疳��

		if (universal::FindFromVector(rIceSave, apIce[i]))
			continue;	// �T���ς݂Ȃ疳��

		if (enemy::IsEnemyOnIce(aV[i], aH[i]))
			continue;	// ���̓G������Ă��疳��

		if (apIce[i]->IsPeck())
			continue;	// ��������Ȃ疳��

		D3DXVECTOR3 posIce = apIce[i]->GetPosition();
		float fDiff = 0.0f;

		if (universal::DistCmpFlat(posIce, posDest, fDistMin, &fDiff))
		{
			nIdxMin = i;
			fDistMin = fDiff;
		}
	}

	return PathFind(aV[nIdxMin], aH[nIdxMin], rIceSave);	// �T��
}

//=====================================================
// �U������̌��o
//=====================================================
void CEnemy::JudgeTurn(void)
{
	// ���Ɍ������X�̈ʒu�̎擾
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	CIce *pIceNext = pIceMgr->GetGridIce(&m_nGridVNext, &m_nGridHNext);

	if (pIceNext == nullptr)
		return;

	// �ʒu�擾
	D3DXVECTOR3 posNext = pIceNext->GetPosition();
	D3DXVECTOR3 vecDiff = posNext - GetPosition();

	// �����p�x���쐬
	float fAngleDest = atan2f(-vecDiff.x, -vecDiff.z);
	D3DXVECTOR3 rot = GetRotation();

	// �����̔���
	float fRotDiff = fAngleDest - rot.y;
	universal::LimitRot(&fRotDiff);

	if (LINE_START_TURN * LINE_START_TURN < fRotDiff * fRotDiff)
	{
		// ���݂̌����Ɛ����΂�ڕW�̌����ɐݒ�
		m_fRotTurn = fAngleDest;
		universal::LimitRot(&m_fRotTurn);

		m_bTurn = true;	// �������l���z���Ă�����U��Ԃ锻��

		// �ړ��ʂ����Z�b�g
		m_fSpeedMove = 0.0f;
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
}

//=====================================================
// �U��Ԃ�̖�����
//=====================================================
bool CEnemy::DisableTurn(void)
{
	if (!m_bTurn)
		return true;

	// �ڕW�̌����ɕ␳����
	D3DXVECTOR3 rot = GetRotation();
	universal::FactingRot(&rot.y, m_fRotTurn, FACT_ROTATION_TURN);
	SetRotation(rot);

	// �����p�x�����ȉ��ɂȂ�����U��Ԃ���~����
	float fRotDiff = m_fRotTurn - rot.y;

	universal::LimitRot(&fRotDiff);

	bool bFinishMotion = IsFinish();

	if (LINE_STOP_TURN * LINE_STOP_TURN > fRotDiff * fRotDiff)
		m_bTurn = false;

	return !m_bTurn;
}

//=====================================================
// ���̃O���b�h�Ɍ������Ĉړ�����
//=====================================================
void CEnemy::MoveToNextGrid(void)
{
	if (m_bTurn)	// �U��������͈ړ����Ȃ�
		return;

	CDebugProc::GetInstance()->Print("\n�����܂Œʂ��Ă郈����������������������");

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// ���݂Ǝ��̃O���b�h�̍��W���擾
	D3DXVECTOR3 posNext = pIceMgr->GetGridPosition(&m_nGridVNext, &m_nGridHNext);
	D3DXVECTOR3 pos = GetPosition();

	// �����x�N�g�����X�s�[�h���ɐ��K��
	D3DXVECTOR3 vecDiff = posNext - pos;

	// ������␳����
	float fLengthDiff = D3DXVec3Length(&vecDiff);
	if (fLengthDiff > LINE_FACT_ROT)
	{// �����x�N�g�������ȏ�̒���������Ƃ��̂�
		D3DXVECTOR3 rot = GetRotation();
		float fRotDest = atan2f(-vecDiff.x, -vecDiff.z);
		universal::FactingRot(&rot.y, fRotDest, m_fFactRot);

		SetRotation(rot);
	}

	// �ړ��\����
	if (!JudgeCanMove())
		return;

	// �������X���������~����
	CIce *pIce = pIceMgr->GetGridIce(&m_nGridVNext, &m_nGridHNext);

	if (pIce != nullptr)
	{
		if (pIce->IsPeck())
		{
			SetState(CEnemy::E_State::STATE_STOP);
			SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			AliveDestGrid();
			return;
		}
	}

	if (!m_bEnableMove)
		return;

	// �����x�N�g�����X�s�[�h���ɐ��K��
	universal::VecConvertLength(&vecDiff, m_fSpeedMove);
	SetMove(vecDiff);
	
#ifdef _DEBUG
	//CEffect3D::Create(posNext, 100.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//CEffect3D::Create(pos, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
#endif
}

//=====================================================
// �ړ��ł��邩�̔���
//=====================================================
bool CEnemy::JudgeCanMove(void)
{
	// ���Ɍ������X�̈ʒu�̎擾
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	CIce *pIceNext = pIceMgr->GetGridIce(&m_nGridVNext, &m_nGridHNext);

	if (pIceNext == nullptr)
		return false;

	// �ʒu�擾
	D3DXVECTOR3 posNext = pIceNext->GetPosition();
	D3DXVECTOR3 vecDiff = posNext - GetPosition();

	// �����p�x���쐬
	float fAngleDest = atan2f(-vecDiff.x, -vecDiff.z);

	// �����p�x�����ȉ��ɂȂ�����ړ��ł���
	D3DXVECTOR3 rot = GetRotation();
	float fRotDiff = fAngleDest - rot.y;

	universal::LimitRot(&fRotDiff);

	if (LINE_ENABLE_MOVE * LINE_ENABLE_MOVE > fRotDiff * fRotDiff)
	{
		m_bEnableMove = true;	// �ړI�̌������������̂ňړ��\
		return true;
	}

	// �����܂Œʂ�����ړ����Ȃ�
	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_bEnableMove = false;

	return false;
}

//=====================================================
// �O���b�h���ς�������̃`�F�b�N
//=====================================================
void CEnemy::CheckChangeGrid(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// �O���b�h�ԍ��̎擾
	D3DXVECTOR3 pos = GetPosition();
	CIce *pIce = pIceMgr->GetGridIce(&m_nGridVNext, &m_nGridHNext);
	
	if (!pIceMgr->IsInIce(pos, pIce,0.1f))
	{// �O���b�h���ς���ĂȂ����͋U��Ԃ�
		return;
	}
	else
	{// �O���b�h���ς���Ă���l��ۑ����Đ^��Ԃ�
		m_nGridV = m_nGridVNext;
		m_nGridH = m_nGridHNext;

		if (m_nGridV == m_nGridVDest && m_nGridH == m_nGridHDest)
			AliveDestGrid();	// �ړI�n�ɂ�������

		return;
	}
}

//=====================================================
// �X�Ƃ̔���
//=====================================================
void CEnemy::CollideIce(void)
{
	if (m_state == E_State::STATE_DRIFT)
		return;

	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	CIce *pIce = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	if (pIce == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();
	pIceMgr->Collide(&pos, pIce, RATE_COLLIDEICE);

	SetPosition(pos);
}

//=====================================================
// �Y�����̓���
//=====================================================
void CEnemy::UpdateDrift(void)
{
	// �Y�����̏���
	StayFlow();

	// �Y�����̎�
	DriftDeath();
}

//=====================================================
// �Y�����̎�
//=====================================================
void CEnemy::DriftDeath(void)
{
	D3DXVECTOR3 pos = GetPosition();

	if (!universal::IsInScreen(pos, nullptr))
		Death();	// ��ʊO�ɏo���玀�S���̏����ɓ���
}

//=====================================================
// �Y���̊J�n
//=====================================================
void CEnemy::StartFlows(void)
{
	if (FindFlowIce())
	{// �Y������X��������΁A�Y����Ԃֈڍs
		m_state = E_State::STATE_DRIFT;
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_bTurn = false;
	}
}

//=====================================================
// �Y������X�̌��o
//=====================================================
bool CEnemy::FindFlowIce(void)
{
	if (GetState() == E_State::STATE_APPER)
		return false;	// �o����ԂȂ�ʂ�Ȃ�

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	int nTemp;	// ���̏�ɕX�����������玞�̂ݕY��
	if (!pIceMgr->GetIdxGridFromPosition(GetPosition(), &nTemp, &nTemp,1.0f))
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

			if (pIceMgr->IsInIce(posPlayer, itIce, RATE_STOP_FLOW_ICE_RADIUS))
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
void CEnemy::StayFlow(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	if (m_pLandSystemFlow != nullptr)
	{// ���X�V�X�e�������o�o�����ꍇ�̏���
		if (m_pLandSystemFlow->IsDeath())
		{
			// �Y���̏I��
			EndFlows();
		}
	}
	else
	{// ���X�V�X�e�������o�ł��Ȃ��ꍇ�̏���
		JudgeEndFlow();
	}
}

//=====================================================
// �Y���I���̔���
//=====================================================
void CEnemy::JudgeEndFlow(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// �O���b�h����������~�܂�
	D3DXVECTOR3 pos = GetPosition();
	pIceMgr->GetIdxGridFromPosition(pos, &m_nGridV, &m_nGridH, RATE_STOP_FLOW_ICE_RADIUS);

	CIce *pIce = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	if (pIce != nullptr)
		EndFlows();	// �Y���̏I��
}

//=====================================================
// �Y���̏I��
//=====================================================
void CEnemy::EndFlows(void)
{
	m_state = E_State::STATE_STOP;
	m_pLandSystemFlow = nullptr;

	// �~�܂�����ł̃O���b�h���o
	CIceManager *pIceMgr = CIceManager::GetInstance();
	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	// �O���b�h���Ŗ��������瑦��
	bool bResult = pIceMgr->IsInGrid(pos, 0.6f);

	if (!bResult)
	{
		Death();
		return;
	}

	pIceMgr->GetNearestIce(pos, &m_nGridV, &m_nGridH);

	m_nGridVNext = m_nGridV;
	m_nGridHNext = m_nGridH;

	m_nGridVDest = m_nGridV;
	m_nGridHDest = m_nGridH;
}

//=====================================================
// ���S���̏���
//=====================================================
void CEnemy::Death(void)
{
	// �R���{�A���j���̃X�R�A���Z
	CUI_Combo* pUICombo = CUI_Combo::GetInstance();

	if (pUICombo != nullptr)
	{
		pUICombo->AddCombo(m_type);
	}

	// �I���������Ă�
	Uninit();
}

//=====================================================
// �O���b�h�ɂ��ړ�
//=====================================================
void CEnemy::TranslateByGrid(int nIdxV, int nIdxH)
{
	CIceManager *pIcemgr = CIceManager::GetInstance();

	if (pIcemgr == nullptr)
		return;

	D3DXVECTOR3 pos = pIcemgr->GetGridPosition(&nIdxV, &nIdxH);

	CIce *pIce = pIcemgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIce == nullptr)
		return;

	pos.y = pIce->GetPosition().y;

	SetPosition(pos);
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CEnemy::Debug(void)
{
	CDebugProc* pDebugProc = CDebugProc::GetInstance();
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	if (pDebugProc == nullptr || pInputKeyboard == nullptr)
		return;

	if (pInputKeyboard->GetTrigger(DIK_7))
		Death();

#if 1
	pDebugProc->Print("\n�G���==========================");
	pDebugProc->Print("\n�ʒu[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	pDebugProc->Print("\n���݃O���b�h[%d,%d]", m_nGridV, m_nGridH);
	pDebugProc->Print("\n���̃O���b�h[%d,%d]", m_nGridVNext, m_nGridHNext);
	pDebugProc->Print("\n�ڕW�O���b�h[%d,%d]", m_nGridVDest, m_nGridHDest);
	pDebugProc->Print("\n���[�V����[%d]", GetMotion());

	//pDebugProc->Print("\n���݂̏��[%d]", m_state);
#endif

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	debug::Effect3DShort(pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	debug::Effect3DShort(pIceMgr->GetGridPosition(&m_nGridVNext, &m_nGridHNext), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	debug::Effect3DShort(pIceMgr->GetGridPosition(&m_nGridVDest, &m_nGridHDest), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemy::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}