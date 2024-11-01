//*****************************************************
//
// �G�̏���[enemy.cpp]
// Author:����F�M
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

	// ���[�V�����������ݒ�
	SetMotion(E_Motion::MOTION_STARTJUMP);

	// �|�[�Y������
	InitPose(0);

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
void CSeals::MoveToIce(void)
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
// ��~��Ԃ̍X�V
//=====================================================
void CSeals::UpdateStop(void)
{
	// �v���C���[�Ƃ̔���
	CollidePlayer();

	// �U����̏����ݒ�
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

	// �v���C���[�Ƃ̔���
	CollidePlayer();

	// �p���N���X�̍X�V
	CEnemy::UpdateMove();
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
	if (GetState() == CEnemy::E_State::STATE_MOVE)
	{
		if (m_pPlayerTarget == nullptr)
		{// ����������
			if (nMotion != E_Motion::MOTION_NEUTRAL)
				SetMotion(E_Motion::MOTION_NEUTRAL);
		}
		else
		{// ������
			if (nMotion != E_Motion::MOTION_WALK)
				SetMotion(E_Motion::MOTION_WALK);
		}
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

		// �v���C���[�̃O���b�h�ԍ��擾
		int nIdxVPlayer = it->GetGridV();
		int nIdxHPlayer = it->GetGridH();

		// ���g�̃O���b�h�ԍ�
		int nIdxV = GetGridV();
		int nIdxH = GetGridH();

		if (nIdxV == nIdxVPlayer && nIdxH == nIdxHPlayer)
		{// ���g�̃O���b�h�ԍ��Əc������v����ꍇ�A����̃q�b�g�������Ă�
			it->Hit(0.0f);	// �����Ȃ̂Ń_���[�W��0
			// ��~���ĖڕW�̃��Z�b�g
			SetState(CEnemy::E_State::STATE_STOP);
			m_pPlayerTarget = nullptr;	
		}
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