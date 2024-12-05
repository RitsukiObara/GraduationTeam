//*****************************************************
//
// �}���`�p�̍d���X�̏���[iceHardMulti.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "iceHardMulti.h"
#include "player.h"
#include "iceManager.h"
#include "manager.h"
#include "fan3D.h"
#include "meshcylinder.h"
#include "MyEffekseer.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float TIME_JUMP = 3.0f;						// ��΂��܂ł̎���
const float SPEED_REPAIR = 0.3f;					// �񕜂̑��x(1.0f�Ői�s�Ɠ������x)
const D3DXCOLOR COL_INIT = { 1.0f,1.0f,1.0f,1.0f };	// �����̐F
const float SIZE_COLLIDE = Grid::SIZE * 0.5f;		// ����̃T�C�Y

const float POW_VIB_JUMP = 0.7f;	// ��т����Ȏ��̐U������
const int TIME_VIB_JUMP = 10;		// ��т����Ȏ��̐U������

//--------------------------------
// �W�����v�̒萔
//--------------------------------
namespace jump
{
const float POW = 30.0f;		// �W�����v��
const float HEIGHT = 200.0f;	// ����
const float TIME = 1.0f;		// ����
}
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************

//=====================================================
// �R���X�g���N�^
//=====================================================
CIceHardMulti::CIceHardMulti(int nPriority) : CIceHard(nPriority), m_state(E_State::STATE_NONE), m_fTimerJump(0.0f)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CIceHardMulti::~CIceHardMulti()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CIceHardMulti::Init(void)
{
	CIceHard::Init();

	// ��Ԃ̏�����
	m_state = E_State::STATE_NORMAL;

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CIceHardMulti::Uninit(void)
{
	CIceHard::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CIceHardMulti::Update(void)
{
	// �p���N���X�̍X�V
	CIceHard::Update();

	// �v���C���[������Ă锻��
	IsOnPlayer();

	// ��Ԃ��Ƃ̍X�V
	UpdateState();
}

//=====================================================
// �v���C���[������Ă锻��
//=====================================================
void CIceHardMulti::IsOnPlayer(void)
{
	// �z��̃N���A
	m_apPlayerJump.clear();

	// �v���C���[�̎擾
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	for (CPlayer *pPlayer : apPlayer)
	{
		if (pPlayer == nullptr)
			continue;

		if (pPlayer->GetState() == CPlayer::E_State::STATE_BLOW)
			continue;

		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

		if (universal::DistCmpFlat(pos, posPlayer, SIZE_COLLIDE, nullptr))
		{// ��苗���ȓ��ɂ�����J�E���^�[�����Z
			m_fTimerJump += CManager::GetDeltaTime();

			// �R���g���[���[��U��������
			float fRate = m_fTimerJump / TIME_JUMP;
			pPlayer->VibJoypad(POW_VIB_JUMP * fRate, TIME_VIB_JUMP);

			m_apPlayerJump.push_back(pPlayer);
		}
	}

	if (m_apPlayerJump.empty())
	{// �N������ĂȂ�������^�C�}�[���Z
		m_fTimerJump -= CManager::GetDeltaTime() * SPEED_REPAIR;
	}
}

//=====================================================
// ��Ԃ��Ƃ̍X�V
//=====================================================
void CIceHardMulti::UpdateState(void)
{
	switch (m_state)
	{
	case CIceHardMulti::STATE_NORMAL:	// �ʏ�X�V
		UpdateNormal();
		break;
	case CIceHardMulti::STATE_JUMP:	// �W�����v���̍X�V
		UpdateJump();
		break;
	default:
		assert(false);
		break;
	}
}

//=====================================================
// �ʏ�̍X�V
//=====================================================
void CIceHardMulti::UpdateNormal(void)
{
	if (m_fTimerJump > TIME_JUMP)
	{// ��莞�ԏ���Ă���W�����v������
		StartJump();
	}

	// �l�̕␳
	universal::LimitValuefloat(&m_fTimerJump, TIME_JUMP, 0.0f);

	// �F�̐ݒ�
	D3DXCOLOR col = COL_INIT;

	// �����̌v�Z
	float fRate = m_fTimerJump / TIME_JUMP;

	// �ԈȊO�̐F�������������Ă���
	col.g -= fRate;
	col.b -= fRate;

	// �e���b�V���ɐF��ݒ�
	CMeshCylinder *pCylinder = GetMeshCyliner();
	CFan3D *pFan = GetFan3D();

	if (pCylinder == nullptr || pFan == nullptr)
		return;

	pCylinder->SetCol(col);
	pFan->SetCol(col);
}

//=====================================================
// �W�����v���̍X�V
//=====================================================
void CIceHardMulti::UpdateJump(void)
{
	if (m_fTimerMove > jump::TIME)
	{// ���Ԃ��z������ʏ�ɖ߂�
		m_fTimerMove = 0.0f;
		m_state = E_State::STATE_NORMAL;
	}

	m_fTimerMove += CManager::GetDeltaTime();

	// ���Ԃ𐳋K��
	float t = m_fTimerMove / jump::TIME;

	universal::LimitValuefloat(&t, 1.0f, 0.0f);

	// �������v�Z
	float fHeight = jump::HEIGHT * (1.0f - 4.0f * (t - 0.5f) * (t - 0.5f));

	SetHeightOcean(fHeight);
}

//=====================================================
// �W�����v�J�n����
//=====================================================
void CIceHardMulti::StartJump(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();
	if (pIceMgr == nullptr)
		return;

	for (CPlayer *pPlayer : m_apPlayerJump)
	{
		if (pPlayer == nullptr)
			continue;

		// ��Ɉړ��ʂ�^����
		pPlayer->SetMove(D3DXVECTOR3(0.0f, jump::POW, 0.0f));

		// �����_���ȕX�̎擾
		CIce *pIce = pIceMgr->GetRandomIce();

		// �v���C���[������΂��̊J�n
		pPlayer->StartBlow(pIce);
	}

	// �����l�����݈ʒu�ɐݒ�
	m_posInit = GetPosition();

	// ��Ԃ�ݒ�
	m_state = E_State::STATE_JUMP;
}

//=====================================================
// �`�揈��
//=====================================================
void CIceHardMulti::Draw(void)
{
	CIceHard::Draw();
}