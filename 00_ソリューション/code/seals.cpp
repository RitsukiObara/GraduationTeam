//*****************************************************
//
// �G�̏���[enemy.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "seals.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "iceManager.h"
#include "player.h"
#include "particle.h"
#include "debugproc.h"
#include "UI_enemy.h"
#include "MyEffekseer.h"
#include "sound.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionSeals.txt";	// �{�f�B�̃p�X

const float HEIGHT_APPER = -400.0f;	// �o�����̍���
const float WIDTH_APPER = -340.0f;	// �o�����̉��̂���
const float POW_APPER_JUMP = 45.0f;	// �o�����̃W�����v��
const float APPER_GRAVITY = -0.98f;	// �o�����̏d��
const float FACT_MOVE_APPER = 0.04f;	// �o�����̈ړ��W��

const float RANGE_FIND_PLAYER = 1000.0f;	// �v���C���[�����͈�

const float SPEED_ONESTEP = 1.7f;	// ����̃X�s�[�h
const float FACT_DECMOVE = 0.9f;	// �ړ������W��

const float RADIUS_HIT = 110.0f;	// �q�b�g����̔��a

const float HEIGHT_ICE = 100.0f;	// �X�̍���
}

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CSeals::CSeals(int nPriority) : CEnemy(nPriority), m_pPlayerTarget(nullptr)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CSeals::~CSeals()
{

}

//=====================================================
// ��������
//=====================================================
CSeals* CSeals::Create(void)
{
	CSeals* pSeals = nullptr;

	pSeals = new CSeals;

	if (pSeals != nullptr)
	{
		pSeals->Init();
	}

	return pSeals;
}

//=====================================================
// ����������
//=====================================================
HRESULT CSeals::Init(void)
{
	// �Ǎ�
	Load((char*)&PATH_BODY[0]);

	if (FAILED(CEnemy::Init()))
	{ 
		return E_FAIL;
	}

	// �X�s�[�h��0�ɐݒ�
	SetSpeedMove(0.0f);

	return S_OK;
}

//=====================================================
// �o�����̃g�����X�t�H�[���ݒ�
//=====================================================
void CSeals::SetApperTransform(void)
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
	m_posApper = posApper;

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

	// ���[�V�����������ݒ�
	SetMotion(E_Motion::MOTION_STARTJUMP);
}

//=====================================================
// �I������
//=====================================================
void CSeals::Uninit(void)
{
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CSeals::Update(void)
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
void CSeals::UpdateApper(void)
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
			CSound::GetInstance()->Play(CSound::LABEL_SE_SEAL_CRY);	// ����
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
bool CSeals::CollideLand(void)
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
	{
		JudgeRetry();
		return false;
	}

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posIce = pIce->GetPosition();

	if (pos.y < posIce.y + HEIGHT_ICE)
	{// �X�����ʒu���Ⴍ�Ȃ����璅�n����

		pos.y = posIce.y;
		SetPosition(pos);

		// �G�t�F�N�g����
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_LANDING, posIce);

		return true;
	}

	return false;
}

//=====================================================
// �ēx�o�����锻��
//=====================================================
void CSeals::JudgeRetry(void)
{
	D3DXVECTOR3 pos = GetPosition();

	if (pos.y < 0)
	{
		// �o������ꏊ�ɐ����Ԃ��𔭐�
		CParticle::Create(m_posApper, CParticle::TYPE::TYPE_DROP);

		E_Spawn spawn = GetSpawn();

		// �ēx�o�������ɓ���
		InitGridIdx(spawn);
		SetApperTransform();
	}
}

//=====================================================
// �X�Ɍ������Ĉړ�����
//=====================================================
void CSeals::MoveToIce(void)
{
	// ���n����O���b�h�̕X���擾
	int nIdxV = GetGridV();
	int nIdxH = GetGridH();

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posIce = m_posApper;

	D3DXVECTOR3 posMove = pos;
	
	universal::MoveToDest(&posMove, posIce, FACT_MOVE_APPER);

	posMove.y = pos.y;

	SetPosition(posMove);
}

//=====================================================
// �ړ��ʂ̌���
//=====================================================
void CSeals::Decreasemove(void)
{
	float fSpeed = GetSpeedMove();

	fSpeed *= FACT_DECMOVE;

	SetSpeedMove(fSpeed);
}

//=====================================================
// ��~��Ԃ̍X�V
//=====================================================
void CSeals::UpdateStop(void)
{
	// �v���C���[�Ƃ̔���
	CollidePlayer();

	SarchTarget();

	if(m_pPlayerTarget == nullptr)
		DecideNextStrollGrid();

	CEnemy::UpdateStop();

	SetState(E_State::STATE_MOVE);
}

//=====================================================
// �^�[�Q�b�g�̒T��
//=====================================================
void CSeals::SarchTarget(void)
{
	// �v���C���[�C���X�^���X�̎擾
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	if (apPlayer.empty())
		return;	// �z�񂪋�Ȃ�I��

	float fLengthMin = RANGE_FIND_PLAYER;

	D3DXVECTOR3 pos = GetPosition();

	CPlayer *pPlayer = nullptr;	// ���������v���C���[

	for (auto it : apPlayer)
	{
		D3DXVECTOR3 posPlayer = it->GetPosition();

		float fDiff = 0.0f;

		if (it->GetState() == CPlayer::E_State::STATE_DEATH)
			continue;

		if (universal::DistCmpFlat(pos, posPlayer, fLengthMin, &fDiff))
		{// �ŏ��������߂�������ۑ�
			pPlayer = it;
			fLengthMin = fDiff;
		}
	}

	m_pPlayerTarget = pPlayer;

	if (m_pPlayerTarget != nullptr)	// �^�[�Q�b�g������������ړ���ԂɈڍs
	{
		SetState(CEnemy::E_State::STATE_MOVE);

		SetMotion(E_Motion::MOTION_WALK);
	}
	else
	{

	}
}

//=====================================================
// �ړ���Ԃ̍X�V
//=====================================================
void CSeals::UpdateMove(void)
{
	if (m_pPlayerTarget == nullptr)
	{// �v���C���[���������̏���
		// �^�[�Q�b�g�̒T��
		SarchTarget();
	}
	else
	{// �v���C���[�������̓v���C���[��ǂ�������
		// �v���C���[�O���b�h�̔���
		FindPlayerGrid();
	}

	// �p���N���X�̍X�V
	CEnemy::UpdateMove();

	// �v���C���[�Ƃ̔���
	CollidePlayer();
}

//=====================================================
// �v���C���[�O���b�h�̔���
//=====================================================
void CSeals::FindPlayerGrid(void)
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
void CSeals::AliveDestGrid(void)
{
	if (m_pPlayerTarget == nullptr)
	{// �v���C���[���������͎��̎U�����T��
		DecideNextStrollGrid();
	}
}

//=====================================================
// ���̎U�����T������
//=====================================================
void CSeals::DecideNextStrollGrid(void)
{
	vector<CIce*> apIce = CIce::GetInstance();

	if (apIce.empty())
		return;

	// �~�܂��ĂȂ��X�����O
	universal::RemoveIfFromVector(apIce, [](CIce* ice) { return ice != nullptr && !ice->IsStop(); });

	// �T�C�Y���烉���_���ŕX���w��
	int nRand = universal::RandRange((int)apIce.size() - 1, 0);

	CIce *pIce = apIce[nRand];

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
void CSeals::Death(void)
{
	if (IsDeath())
		return;

	// �GUI�̓G�������Z
	CUIEnemy *pUIEnemy = CUIEnemy::GetInstance();

	if (pUIEnemy != nullptr)
		pUIEnemy->DeleteEnemy(TYPE::TYPE_SEALS);

	CEnemy::Death();

	CSound::GetInstance()->Play(CSound::LABEL_SE_SEAL_CRY);	// ����
}

//=====================================================
// �U����Ԃ̍X�V
//=====================================================
void CSeals::UpdateAttack(void)
{

}

//=====================================================
// �Y����Ԃ̍X�V
//=====================================================
void CSeals::UpdateDrift(void)
{
	CEnemy::UpdateDrift();

	int nMotion = GetMotion();
	if (nMotion != E_Motion::MOTION_NEUTRAL)
		SetMotion(E_Motion::MOTION_NEUTRAL);
}

//=====================================================
// ���[�V�����̊Ǘ�
//=====================================================
void CSeals::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (nMotion == E_Motion::MOTION_STARTJUMP)
	{// �W�����v�J�n���[�V����
		if (bFinish)	// �I��莟��؋󃂁[�V�����ֈڍs
			SetMotion(E_Motion::MOTION_STAYJUMP);
	}

	// �ړ���Ԃ̃��[�V�����Ǘ�
	if (IsTurn())
	{// �U��������[�V����
		if (nMotion != E_Motion::MOTION_TURN || bFinish)
			SetMotion(E_Motion::MOTION_TURN);
	}
	else if (!IsEnableMove())
	{// �ړ��s�̎��͑ҋ@���[�V����
		if (nMotion != E_Motion::MOTION_NEUTRAL)
			SetMotion(E_Motion::MOTION_NEUTRAL);
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
void CSeals::CollidePlayer(void)
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

		if (it->GetState() == CPlayer::E_State::STATE_BLOW)
			continue;

		D3DXVECTOR3 posPlayer = it->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		if (universal::DistCmpFlat(pos, posPlayer,RADIUS_HIT,nullptr))
		{// ���g�̃O���b�h�ԍ��Əc������v����ꍇ�A����̃q�b�g�������Ă�
			it->Hit(0.0f);	// �����Ȃ̂Ń_���[�W��0
			// ��~���ĖڕW�̃��Z�b�g
			SetState(CEnemy::E_State::STATE_STOP);
			m_pPlayerTarget = nullptr;	
		}
	}
}

//=====================================================
// ���[�V�����C�x���g
//=====================================================
void CSeals::Event(EVENT_INFO* pEventInfo)
{
	int nMotion = GetMotion();

	if (nMotion == CSeals::E_Motion::MOTION_WALK)
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
}

//=====================================================
// �`�揈��
//=====================================================
void CSeals::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}