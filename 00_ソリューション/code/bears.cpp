//*****************************************************
//
// �V���N�}�̏���[bears.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "bears.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "iceManager.h"
#include "player.h"
#include "particle.h"
#include "debugproc.h"
#include "UI_enemy.h"
#include "effect3D.h"
#include "manager.h"
#include "MyEffekseer.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionWhitebear.txt";	// �{�f�B�̃p�X

const float HEIGHT_APPER = -400.0f;		// �o�����̍���
const float WIDTH_APPER = -340.0f;		// �o�����̉��̂���
const float POW_APPER_JUMP = 45.0f;		// �o�����̃W�����v��
const float APPER_GRAVITY = -0.98f;		// �o�����̏d��
const float FACT_MOVE_APPER = 0.04f;	// �o�����̈ړ��W��

const float RANGE_FIND_PLAYER = 1000.0f;	// �v���C���[�����͈�

const float SPEED_ONESTEP = 1.1f;	// ����̃X�s�[�h
const float FACT_DECMOVE = 0.96f;	// �ړ������W��

const float RADIUS_HIT = 150.0f;	// �q�b�g����̔��a

//-----------------------------
// �ːi�̒萔
//-----------------------------
namespace charge
{
const float RATE_START = 0.7f;		// �ːi���J�n����̂ɕX�ɋ߂Â��Ă銄��
const float RATE_RANGE = D3DX_PI / CIceManager::E_Direction::DIRECTION_MAX;	// �ˌ��̊p�x�͈�
const float SPEED_ONESTEP = 1.0f;	// ����̑��x
}
}

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CBears::CBears(int nPriority) : CEnemy(nPriority), m_pPlayerTarget(nullptr), m_vecCharge(), m_fTimerAcceleCharge(0.0f), m_bCharge(false)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBears::~CBears()
{

}

//=====================================================
// ��������
//=====================================================
CBears* CBears::Create(void)
{
	CBears* pBears = nullptr;

	pBears = new CBears;

	if (pBears != nullptr)
	{
		pBears->Init();
	}

	return pBears;
}

//=====================================================
// ����������
//=====================================================
HRESULT CBears::Init(void)
{
	// �Ǎ�
	Load((char*)&PATH_BODY[0]);

	if (FAILED(CEnemy::Init()))
	{ 
		return E_FAIL;
	}

	// ���[�V�����������ݒ�
	SetMotion(E_Motion::MOTION_STARTJUMP);

	// �|�[�Y������
	InitPose(0);

	// �GUI�̓G�������Z
	CUIEnemy *pUIEnemy = CUIEnemy::GetInstance();

	if (pUIEnemy != nullptr)
		pUIEnemy->AddEnemy();

	// �X�s�[�h��0�ɐݒ�
	SetSpeedMove(0.0f);

	return S_OK;
}

//=====================================================
// �o�����̃g�����X�t�H�[���ݒ�
//=====================================================
void CBears::SetApperTransform(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// �O���b�h�ԍ��̈ʒu���擾
	int nIdxV = GetGridV();
	int nIdxH = GetGridH();

	D3DXVECTOR3 posGrid = pIceMgr->GetGridPosition(&nIdxV, &nIdxH);

	// �ŏ��̓O���b�h�ʒu���R�s�[
	D3DXVECTOR3 posApper = posGrid;

	// �����ɂ��炷
	posApper.x += WIDTH_APPER;
	posApper.y += HEIGHT_APPER;

	SetPosition(posApper);

	// �o�����̃W�����v��������
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };
	move.y = POW_APPER_JUMP;
	SetMove(move);

	// ������X�̕��Ɍ�����
	D3DXVECTOR3 vecDiff = posGrid - posApper;

	float fRot = atan2f(-vecDiff.x, -vecDiff.z);

	D3DXVECTOR3 rot = { 0.0f,fRot,0.0f };

	SetRotation(rot);

	// �o������ꏊ�ɐ����Ԃ��𔭐�
	CParticle::Create(posApper, CParticle::TYPE::TYPE_DROP);
}

//=====================================================
// �I������
//=====================================================
void CBears::Uninit(void)
{
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CBears::Update(void)
{
	// �p���N���X�̍X�V
	CEnemy::Update();

	// �ړ��ʂ̌���
	Decreasemove();

	// ���[�V�����̊Ǘ�
	ManageMotion();
}

//=====================================================
// �o����Ԃ̍X�V
//=====================================================
void CBears::UpdateApper(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	// ���n���[�V�������I���������~��ԂɈڍs
	if (bFinish && nMotion == E_Motion::MOTION_LANDING)
		SetState(CEnemy::E_State::STATE_STOP);

	if (E_Motion::MOTION_LANDING != nMotion)
	{// ���n���[�V���������Ȃ��悤�ɂ���
		if (CollideLand())
		{
			SetMotion(E_Motion::MOTION_LANDING); // ���n��������ɓ������璅�n���[�V�����ֈڍs
			EnableFollowIce(true);	// �X�ɒǏ]����悤�ɂ���
		}
	}

	// �X�Ɉ����񂹂�
	MoveToIce();

	// �d�͂�������
	D3DXVECTOR3 move = GetMove();
	move.y += APPER_GRAVITY;
	SetMove(move);
}

//=====================================================
// ���n����
//=====================================================
bool CBears::CollideLand(void)
{
	if (GetMotion() != MOTION_STAYJUMP)
		return false;	// �W�����v�����[�V�����ɂȂ�܂œ���Ȃ�

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	// ���n����O���b�h�̕X���擾
	int nIdxV = GetGridV();
	int nIdxH = GetGridH();

	CIce *pIce = pIceMgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIce == nullptr)
		return false;

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posIce = pIce->GetPosition();

	if (pos.y < posIce.y)
	{// �X�����ʒu���Ⴍ�Ȃ����璅�n����
		pos.y = posIce.y;
		SetPosition(pos);
		return true;
	}

	return false;
}

//=====================================================
// �X�Ɍ������Ĉړ�����
//=====================================================
void CBears::MoveToIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// ���n����O���b�h�̕X���擾
	int nIdxV = GetGridV();
	int nIdxH = GetGridH();

	CIce *pIce = pIceMgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIce == nullptr)
		return;

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posIce = pIce->GetPosition();

	D3DXVECTOR3 posMove = pos;
	
	universal::MoveToDest(&posMove, posIce, FACT_MOVE_APPER);

	posMove.y = pos.y;

	SetPosition(posMove);
}

//=====================================================
// �ړ��ʂ̌���
//=====================================================
void CBears::Decreasemove(void)
{
	float fSpeed = GetSpeedMove();

	fSpeed *= FACT_DECMOVE;

	SetSpeedMove(fSpeed);
}

//=====================================================
// ��~��Ԃ̍X�V
//=====================================================
void CBears::UpdateStop(void)
{
	// �v���C���[�Ƃ̔���
	CollidePlayer();

	// �U����̏����ݒ�
	DecideNextStrollGrid();

	CEnemy::UpdateStop();

	SetState(CEnemy::E_State::STATE_MOVE);
}

//=====================================================
// �^�[�Q�b�g�̒T��
//=====================================================
void CBears::SarchTarget(void)
{
	//------------------------------
	// �ːi���邩�̔���
	//------------------------------
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// ���݂̕X���擾
	int nIdxV = GetGridV();
	int nIdxH = GetGridH();
	CIce *pIce = pIceMgr->GetGridIce(&nIdxV, &nIdxH);
	D3DXVECTOR3 pos = GetPosition();

	if (!pIceMgr->IsInIce(pos, pIce, charge::RATE_START))
		return;	// ���ȏ�X�ɋ߂��Ȃ���΁A�ːi���Ȃ�

	//------------------------------
	// �v���C���[��T������
	//------------------------------
	// �v���C���[�C���X�^���X�̎擾
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	if (apPlayer.empty())
		return;	// �z�񂪋�Ȃ�I��

	float fLengthMin = RANGE_FIND_PLAYER;

	CPlayer *pPlayer = nullptr;	// ���������v���C���[

	for (auto it : apPlayer)
	{
		D3DXVECTOR3 posPlayer = it->GetPosition();

		float fDiff = 0.0f;

		if (it->GetState() == CPlayer::E_State::STATE_DEATH)
			continue;

		int nIdxPlayerV = it->GetGridV();
		int nIdxPlayerH = it->GetGridH();

		if (CanCharge(posPlayer, nIdxPlayerV, nIdxPlayerH))
		{// �ˌ��ł�����^�[�Q�b�g�ɂ��ēˌ��J�n
#ifdef _DEBUG
			CEffect3D::Create(GetPosition(), 200.0f, 4, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
#endif
			// �ːi�x�N�g���̕ۑ�
			m_vecCharge = posPlayer - pos;
			D3DXVec3Normalize(&m_vecCharge, &m_vecCharge);

			// �U������t���O�𗧂Ă�
			EnableTurn(true);

			// �v���C���[��ۑ�����for�����I��
			pPlayer = it;
			break;
		}
	}

	//------------------------------
	// ���ʂ�ۑ�
	//------------------------------
	m_pPlayerTarget = pPlayer;

	if (m_pPlayerTarget != nullptr)	// �^�[�Q�b�g������������ړ���ԂɈڍs
	{
		SetState(CEnemy::E_State::STATE_MOVE);

		SetMotion(E_Motion::MOTION_WALK);
	}
}

//=====================================================
// �O���b�h�����Ȃ��ړ����~�߂�
//=====================================================
void CBears::StopMoveByNotGrid(CIce *pIce)
{
	// �ːi�̏I��
	EndCharge();

	// �p���N���X�̏���
	CEnemy::StopMoveByNotGrid(pIce);
}

//=====================================================
// �ˌ��ł��邩�̔���
//=====================================================
bool CBears::CanCharge(D3DXVECTOR3 posTarget, int nIdxTargetV, int nIdxTargetH)
{
	// �����̊p�x���擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 vecDiff = posTarget - pos;

	float fRot = atan2f(vecDiff.x, vecDiff.z);

	int nIdxV = GetGridV();
	int nIdxH = GetGridH();

	return IsAliveTarget(nIdxV, nIdxH, fRot, nIdxTargetV, nIdxTargetH);
}

//=====================================================
// �^�[�Q�b�g�ɓ��B�������̍ċA�֐�
//=====================================================
bool CBears::IsAliveTarget(int nIdxV, int nIdxH, float fRot, int nIdxTargetV, int nIdxTargetH)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	// ���ӂ̕X�̎擾
	vector<CIce*> apIce = pIceMgr->GetAroundIce(nIdxV, nIdxH);

	D3DXVECTOR3 posCurrentGrid = pIceMgr->GetGridPosition(&nIdxV, &nIdxH);

	for (int i = 0; i < (int)apIce.size(); i++)
	{
		if (apIce[i] == nullptr)
			continue;	// �X���Ȃ���΃X�L�b�v

		if (apIce[i]->IsPeck())
			continue;	// �������X�Ȃ�X�L�b�v

		// �X�̔ԍ��擾
		int nIdxIceV;
		int nIdxIceH;
		pIceMgr->GetIceIndex(apIce[i], &nIdxIceV, &nIdxIceH);

		D3DXVECTOR3 posIce = apIce[i]->GetPosition();

		// �p�x�̐ݒ�
		D3DXVECTOR3 posTarget = pIceMgr->GetGridPosition(&nIdxTargetV, &nIdxTargetH);
		D3DXVECTOR3 vecDiff = posTarget - posCurrentGrid;
		fRot = atan2f(vecDiff.x, vecDiff.z);

		// �ːi�����ɕX������Ȃ�ʂ�
		if (universal::IsInFanTargetYFlat(posCurrentGrid, posIce, fRot, charge::RATE_RANGE))
		{// �i�߂�ꍇ�́A�i�ސ�̕X�ɍċA�֐���i�߂�
#ifdef _DEBUG
			CEffect3D::Create(posIce, 100.0f, 4, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
#endif
			if (nIdxIceV == nIdxTargetV && nIdxIceH == nIdxTargetH)
				return true;	// ���B������^��Ԃ�

			if (IsAliveTarget(nIdxIceV, nIdxIceH, fRot, nIdxTargetV, nIdxTargetH))
				return true;
		}
	}

	return false;
}

//=====================================================
// �ˌ��̏���
//=====================================================
void CBears::ReadyCharge(void)
{
	if(DisableTurn())
		StartCharge(); // ���̌�������������ːi�J�n
}

//=====================================================
// �ˌ��̊J�n
//=====================================================
void CBears::StartCharge(void)
{
	// �U������t���O��܂�
	EnableTurn(false);

	// �ˌ��t���O�𗧂Ă�
	m_bCharge = true;

	// �v���C���[�O���b�h�̔���
	FindPlayerGrid();
}

//=====================================================
// �ˌ��̏I��
//=====================================================
void CBears::EndCharge(void)
{
	// ���ŒT����Ԃɂ���
	SetState(CEnemy::E_State::STATE_STOP);

	// �^�[�Q�b�g�̃v���C���[��null�ɂ���
	m_pPlayerTarget = nullptr;

	// �����^�C�}�[���Z�b�g
	m_fTimerAcceleCharge = 0.0f;

	// ���̎U�����T��
	DecideNextStrollGrid();

	// �ˌ��t���O��܂�
	m_bCharge = false;
}

//=====================================================
// �ړ���Ԃ̍X�V
//=====================================================
void CBears::UpdateMove(void)
{
	if (m_pPlayerTarget == nullptr)
	{// �v���C���[���������̏���
		// �^�[�Q�b�g�̒T��
		SarchTarget();
	}
	else
	{// �v���C���[�������̓v���C���[��ǂ�������
		// �ːi����
		Charge();
	}

	// �v���C���[�Ƃ̔���
	CollidePlayer();

	// �p���N���X�̍X�V
	CEnemy::UpdateMove();

	if(!IsTurn() || m_pPlayerTarget != nullptr)
		MoveToNextGrid(); // ���̃O���b�h�ɐi��
}

//=====================================================
// �ːi���̏���
//=====================================================
void CBears::Charge(void)
{
	DisableTurn();
}

//=====================================================
// �v���C���[�O���b�h�̔���
//=====================================================
void CBears::FindPlayerGrid(void)
{
	if (m_pPlayerTarget == nullptr)
		return;
	
	int nGridV = m_pPlayerTarget->GetGridV();
	int nGridH = m_pPlayerTarget->GetGridH();

	SetGridVDest(nGridV);
	SetGridHDest(nGridH);
}

//=====================================================
// �ڕW�O���b�h�ɓ��������Ƃ��̏���
//=====================================================
void CBears::AliveDestGrid(void)
{
	if (m_pPlayerTarget == nullptr)
	{// �v���C���[���������͎��̎U�����T��
		DecideNextStrollGrid();
	}
	else
	{// �ːi���Ă������̓I�[�o�[�q�[�g��Ԃɂ���
		EndCharge();
	}
}

//=====================================================
// ���̎U�����T������
//=====================================================
void CBears::DecideNextStrollGrid(void)
{
	vector<CIce*> apIce = CIce::GetInstance();

	if (apIce.empty())
		return;

	// �~�܂��ĂȂ��X�����O
	universal::RemoveIfFromVector(apIce, [](CIce* ice) { return ice != nullptr && !ice->IsStop(); });

	// �T�C�Y���烉���_���ŕX���w��
	int nRand = universal::RandRange((int)apIce.size() - 1, 0);

	CIce *pIce = apIce[nRand];

#ifdef _DEBUG
	CEffect3D::Create(apIce[nRand]->GetPosition(), 200.0f, 120, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif

	if (pIce == nullptr)
		return;	// �����I�񂾕X���Ȃ������珈�����I��

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// �I�񂾕X�̃O���b�h�ԍ��擾
	int nIdxV = 0;
	int nIdxH = 0;
	pIceMgr->GetIceIndex(pIce, &nIdxV, &nIdxH);

	// �ڕW�̃O���b�h�ɐݒ�
	SetGridVDest(nIdxV);
	SetGridHDest(nIdxH);
}

//=====================================================
// ���S���̏���
//=====================================================
void CBears::Death(void)
{
	// �GUI�̓G�������Z
	CUIEnemy *pUIEnemy = CUIEnemy::GetInstance();

	if (pUIEnemy != nullptr)
		pUIEnemy->DeleteEnemy();

	CEnemy::Death();
}

//=====================================================
// �U����Ԃ̍X�V
//=====================================================
void CBears::UpdateAttack(void)
{

}

//=====================================================
// �Y����Ԃ̍X�V
//=====================================================
void CBears::UpdateDrift(void)
{
	CEnemy::UpdateDrift();
}

//=====================================================
// ���[�V�����̊Ǘ�
//=====================================================
void CBears::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	//---------------------------------
	// �o����Ԃ̃��[�V����
	//---------------------------------
	if (nMotion == E_Motion::MOTION_STARTJUMP)
	{// �W�����v�J�n���[�V����
		if (bFinish)	// �I��莟��؋󃂁[�V�����ֈڍs
			SetMotion(E_Motion::MOTION_STAYJUMP);
	}

	//---------------------------------
	// �ړ���Ԃ̃��[�V����
	//---------------------------------
	if (m_pPlayerTarget != nullptr)
	{
		if (m_bCharge)
		{// �ːi��
			if (nMotion != E_Motion::MOTION_CHARGE || bFinish)
				SetMotion(E_Motion::MOTION_CHARGE);
		}
		else
		{// �U��������̎�
			if (nMotion != E_Motion::MOTION_TURNCHARGE || bFinish)
				SetMotion(E_Motion::MOTION_TURNCHARGE);
		}
	}
	else if (IsTurn())
	{// �U��������[�V����
		if (m_pPlayerTarget == nullptr)
		{// �ʏ�U�����
			if (nMotion != E_Motion::MOTION_TURN || bFinish)
				SetMotion(E_Motion::MOTION_TURN);
		}
		else
		{// �ːi�U�����
			if (nMotion != E_Motion::MOTION_TURN || bFinish)
				SetMotion(E_Motion::MOTION_TURN);
		}
	}
	else if (!IsEnableMove())
	{// �ړ��s�̎��͑ҋ@���[�V����
		if (nMotion != E_Motion::MOTION_TURN)
			SetMotion(E_Motion::MOTION_TURN);
	}
	else if (GetState() == CEnemy::E_State::STATE_MOVE)
	{
		if (nMotion != E_Motion::MOTION_WALK)
			SetMotion(E_Motion::MOTION_WALK);
	}
}

//=====================================================
// �v���C���[�Ƃ̔���
//=====================================================
void CBears::CollidePlayer(void)
{
	// �v���C���[�C���X�^���X�擾
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	if (apPlayer.empty())
		return;	// �z�񂪋�Ȃ�I��

	for (auto it : apPlayer)
	{
		if (it == nullptr)
			continue;

		if (it->GetState() == CPlayer::E_State::STATE_DEATH)
			continue;

		D3DXVECTOR3 posPlayer = it->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		if (universal::DistCmpFlat(pos, posPlayer,RADIUS_HIT,nullptr))
		{// ���g�̃O���b�h�ԍ��Əc������v����ꍇ�A����̃q�b�g�������Ă�
			// �����Ȃ̂Ń_���[�W��0
			it->Hit(0.0f);

			// ��~���ĖڕW�̃��Z�b�g
			SetState(CEnemy::E_State::STATE_STOP);
			m_pPlayerTarget = nullptr;

			// �ːi�̏I��
			EndCharge();
		}
	}
}

//=====================================================
// ���[�V�����C�x���g
//=====================================================
void CBears::Event(EVENT_INFO* pEventInfo)
{
	int nMotion = GetMotion();

	if (nMotion == CBears::E_Motion::MOTION_WALK)
	{// �����̎��A�C�x���g�����ŃX�s�[�h���o��
		float fSpeed = GetSpeedMove();

		fSpeed += SPEED_ONESTEP;

		SetSpeedMove(fSpeed);
	}

	if (nMotion == E_Motion::MOTION_TURN)
	{// �����]�����A���˂�^�C�~���O�̂݉�]������
		// �U������̖�����
		DisableTurn();
	}

	if (nMotion == E_Motion::MOTION_TURNCHARGE)
	{// �ːi�O�̉�]
		ReadyCharge();
	}

	if (nMotion == E_Motion::MOTION_CHARGE)
	{// �ːi���̓C�x���g�����ŃX�s�[�h����
		float fSpeed = GetSpeedMove();

		fSpeed += charge::SPEED_ONESTEP;

		SetSpeedMove(fSpeed);

		// �G�t�F�N�g�̔���
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_BEARSTEP, GetPosition());
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CBears::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}