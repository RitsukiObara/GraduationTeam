//*****************************************************
//
// �G�̏���[enemy.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemy.h"
#include "game.h"
#include "player.h"
#include "manager.h"
#include "random.h"
#include "inputkeyboard.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionPlayer.txt";	// �{�f�B�̃p�X
	const D3DXVECTOR3 ROT_INITIAL = { 0.0f, -D3DX_PI * 0.5f, 0.0f };	// �����̌���

	const int ATK_NUM[] =	// �����̊e�U���m��
	{
		5,	// ���U���̏����m��
		5,	// �E�U���̏����m��
		2,	// ���U���̏����m��
	};
	const int ATK_START = 200;	// �����̍U���J�n�l

	const int MOVE_ATK_START = 5;	// �U���J�n�l�̕ϓ��l
	const int MIN_ATK_START = 450;	// �U���J�n�l�̍ŏ��l
	const int MAX_ATK_START = 880;	// �U���J�n�l�̍ő�l
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CEnemy *CEnemy::m_pEnemy = nullptr;	// ���g�̃|�C���^

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CEnemy::CEnemy(int nPriority)
{
	// �����������g�̃A�h���X��ۑ�
	m_pEnemy = this;

	// �����o�ϐ����N���A
	m_aAtkNum[ATTACK_LEFT] = ATK_NUM[ATTACK_LEFT];			// ���U���̊m��
	m_aAtkNum[ATTACK_RIGHT] = ATK_NUM[ATTACK_RIGHT];		// �E�U���̊m��
	m_aAtkNum[ATTACK_BOTHHAND] = ATK_NUM[ATTACK_BOTHHAND];	// ���U���̊m��
	m_nMaxAtkStart = ATK_START;	// �U���J�n�l�̍ő�l
	m_fLastAtkTime = 0.0f;		// �O��U������̌o�ߎ���
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
CEnemy *CEnemy::Create(void)
{
	if (m_pEnemy == nullptr)
	{
		m_pEnemy = new CEnemy;
		if (m_pEnemy != nullptr)
		{
			m_pEnemy->Init();
		}
	}

	return m_pEnemy;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemy::Init(void)
{
	// �p���N���X�̏�����
	if (FAILED(CRikishi::Init()))
	{ // �������Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �|�[�Y�̏�����
	InitPose(MOTION_GUARD);

	// �����̏�����
	SetRotation(ROT_INITIAL);

	// �ʒu�̍X�V
	UpdatePosition();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemy::Uninit(void)
{
	// ���g�̕ێ��|�C���^��������
	m_pEnemy = nullptr;

	// �p���N���X�̏I��
	CRikishi::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemy::Update(void)
{
	// �p���N���X�̍X�V
	CRikishi::Update();

	// �ʒu�̍X�V
	UpdatePosition();

	// �Q�[�����ʏ��ԈȊO�̏ꍇ������
	if (CGame::GetState() != CGame::STATE_NORMAL) { return; }

	// �U���̍X�V
	UpdateAttack();

#ifdef _DEBUG
	// �f�o�b�O�̂ݍX�V
	Debug();
#endif
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemy::Draw(void)
{
	// �p���N���X�̕`��
	CRikishi::Draw();
}

//=====================================================
// �U��
//=====================================================
void CEnemy::Attack(E_Attack typeAttack)
{
	int nMotion = GetMotion();

	if (nMotion != MOTION_GUARD)
		return;

	// �e�N���X�̍U�����Ăяo��
	CRikishi::Attack(typeAttack);

	// TODO�F���̊֐����Ă΂�Ă��U���Ɏ��s����\��������ꍇ�͂���ȍ~�̏����ʒu��ύX

	// �A���U���p�x���グ��
	m_nMaxAtkStart -= MOVE_ATK_START;
	universal::LimitMinNum(m_nMaxAtkStart, MIN_ATK_START);
}

//=====================================================
// �U���������̏���
//=====================================================
void CEnemy::HitAttack(E_Attack typeAttack)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	float fKnockBack = pPlayer->Hit(typeAttack, Rikishi::DAMAGE_ATTACK);
}

//=====================================================
// �ʒu�̍X�V����
//=====================================================
void CEnemy::UpdatePosition(void)
{
	// �Q�[���V�[���ȊO�Ȃ甲����
	CGame *pGame = CGame::GetInstance();
	if (pGame == nullptr) { return; }

	// ���E�̒��S���玩�g�̈ʒu���v�Z
	D3DXVECTOR3 posMid = pGame->GetPosMid();
	D3DXVECTOR3 posEnemy = posMid;
	posEnemy.x -= Rikishi::DISTANCE_RIKISHI;

	// �ʒu�𔽉f
	SetPosition(posEnemy);
}

//=====================================================
// �U���̍X�V����
//=====================================================
void CEnemy::UpdateAttack(void)
{
	// �o�ߎ��Ԃ��X�V
	m_fLastAtkTime += CManager::GetDeltaTime();

	int nAtk = (int)(m_fLastAtkTime * 10.0f);	// �U���W��
	int nRand = rand() % m_nMaxAtkStart;		// �U���J�n�l
	if (nRand <= nAtk)
	{ // �U���W�����U���J�n�l�ȉ��̏ꍇ

		// �o�ߎ��Ԃ�������
		m_fLastAtkTime = 0.0f;

		// �U���̃����_������
		RandomAttack();
	}

	// ���ꂼ��̍U���m��
	CDebugProc::GetInstance()->Print("\n���U���m���F[%d]", m_aAtkNum[ATTACK_LEFT]);
	CDebugProc::GetInstance()->Print("\n�E�U���m���F[%d]", m_aAtkNum[ATTACK_RIGHT]);
	CDebugProc::GetInstance()->Print("\n���U���m���F[%d]", m_aAtkNum[ATTACK_BOTHHAND]);

	// �U���m��
	CDebugProc::GetInstance()->Print("\n�U���W���F[%d]", nAtk);
	CDebugProc::GetInstance()->Print("\n�U���J�n�l�F[%d]", nRand);
	CDebugProc::GetInstance()->Print("\n�U���J�n�l�̍ő�F[%d]", m_nMaxAtkStart);
}

//=====================================================
// �U���̃����_����������
//=====================================================
void CEnemy::RandomAttack(void)
{
	// �����_�������A���S���Y���𐶐�
	CRandom<E_Attack> *pRandom = CRandom<E_Attack>::Create();

	// �����\�ȍU����ݒ�
	pRandom->AddList(ATTACK_LEFT, m_aAtkNum[ATTACK_LEFT]);
	pRandom->AddList(ATTACK_RIGHT, m_aAtkNum[ATTACK_RIGHT]);
	pRandom->AddList(ATTACK_BOTHHAND, m_aAtkNum[ATTACK_BOTHHAND]);

	// �����_���őI�����ꂽ�U��������
	E_Attack select = pRandom->GetRandomNum();
	Attack(select);

	// ����I�����ꂽ�U���̎��񐶐��m����������
	m_aAtkNum[select]--;

	// ����I�����ꂽ�U���ȊO�̊m����������
	for (int i = 0; i < 3; i++)
	{
		if (i != select)
		m_aAtkNum[i] = ATK_NUM[i];
	}

	// �����_�������A���S���Y����j��
	CRandom<E_Attack>::Release(pRandom);
}

//=====================================================
// �q�b�g����
//=====================================================
float CEnemy::Hit(CRikishi::E_Attack typeAttack, float fDamage)
{
	// �e�N���X�̃q�b�g���Ăяo��
	float fKnockBack = CRikishi::Hit(typeAttack, fDamage);

	// �Q�[���V�[���ȊO�Ȃ甲����
	CGame *pGame = CGame::GetInstance();
	if (pGame == nullptr) { return 0.0f; }

	// �m�b�N�o�b�N������
	D3DXVECTOR3 addPos = { -fKnockBack,0.0f,0.0f };
	pGame->AddPosMid(addPos);

	// �A���U���p�x��������
	m_nMaxAtkStart += MOVE_ATK_START;
	universal::LimitMaxNum(m_nMaxAtkStart, MAX_ATK_START);

	return 0.0f;
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CEnemy::Debug(void)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr)
		return;

	if (pKeyboard->GetTrigger(DIK_Z))
		Attack(CRikishi::E_Attack::ATTACK_LEFT);
	if (pKeyboard->GetTrigger(DIK_C))
		Attack(CRikishi::E_Attack::ATTACK_RIGHT);
	if (pKeyboard->GetTrigger(DIK_X))
		Attack(CRikishi::E_Attack::ATTACK_BOTHHAND);
}