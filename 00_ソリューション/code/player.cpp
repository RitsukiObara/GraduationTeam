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
#include "manager.h"
#include "game.h"
#include "enemy.h"
#include "polygon3D.h"
#include "debugproc.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPlayer.txt";	// �{�f�B�̃p�X
const float TIME_INPUT_BOTHHAND = 0.3f;	// ����U���̗P�\
const D3DXVECTOR3 ROT_INITIAL = { 0.0f, D3DX_PI * 0.5f, 0.0f };	// �����̌���
const D3DXVECTOR3 OFFSET_GUIDE = { 0.0f, 400.0f, 0.0f };	// �K�C�h�\���̃I�t�Z�b�g�ʒu
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CPlayer *CPlayer::m_pPlayer = nullptr;	// ���g�̃|�C���^

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	m_pPlayer = this;
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
CPlayer *CPlayer::Create(void)
{
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new CPlayer;

		if (m_pPlayer != nullptr)
		{
			m_pPlayer->Init();
		}
	}

	return m_pPlayer;
}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayer::Init(void)
{
	// �p���N���X�̏�����
	CRikishi::Init();

	InitPose(MOTION_GUARD);

	// ����
	SetRotation(ROT_INITIAL);

	m_pGuide = CPolygon3D::Create(OFFSET_GUIDE);
	m_pGuide->SetRotation(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));
	m_pGuide->SetSize(200.0f, 100.0f);

	int nIdx = Texture::GetIdx("data\\TEXTURE\\UI\\you.png");
	m_pGuide->SetIdxTexture(nIdx);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CPlayer::Uninit(void)
{
	m_pPlayer = nullptr;

	// �p���N���X�̏I��
	CRikishi::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CPlayer::Update(void)
{
	// ���͏���
	Input();

	CGame *pGame = CGame::GetInstance();

	if (pGame == nullptr)
		return;

	D3DXVECTOR3 posMid = pGame->GetPosMid();
	D3DXVECTOR3 pos = posMid;
	pos.x += Rikishi::DISTANCE_RIKISHI;

	SetPosition(pos);

	CRikishi::Update();

	if (m_pGuide != nullptr)
	{
		m_pGuide->SetMatrixParent(GetMatrix());
	}
}

//=====================================================
// ����
//=====================================================
void CPlayer::Input(void)
{
	if (CGame::GetState() != CGame::STATE::STATE_NORMAL)
		return;

	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_LEFTATTACK))
	{
		Attack(CRikishi::E_Attack::ATTACK_LEFT);

		m_aTimerAttack[CRikishi::ATTACK_LEFT] = TIME_INPUT_BOTHHAND;
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_RIGHTATTACK))
	{
		Attack(CRikishi::E_Attack::ATTACK_RIGHT);

		m_aTimerAttack[CRikishi::ATTACK_RIGHT] = TIME_INPUT_BOTHHAND;
	}

	// ����U���̔���
	if (m_aTimerAttack[CRikishi::ATTACK_LEFT] > 0 &&
		m_aTimerAttack[CRikishi::ATTACK_RIGHT] > 0)
	{
		Attack(CRikishi::E_Attack::ATTACK_BOTHHAND);

		m_aTimerAttack[CRikishi::ATTACK_LEFT] = -1.0f;
		m_aTimerAttack[CRikishi::ATTACK_RIGHT] = -1.0f;
	}

	// �U���^�C�}�[�̌��Z
	for (int i = 0; i < CRikishi::ATTACK_MAX; i++)
	{
		if (m_aTimerAttack[i] > 0)
		{
			m_aTimerAttack[i] -= CManager::GetDeltaTime();
		}
	}

	CDebugProc::GetInstance()->Print("\n�J�E���^�[[%f]", m_aTimerAttack[0]);
	CDebugProc::GetInstance()->Print("\n�J�E���^�[[%f]", m_aTimerAttack[1]);
}

//=====================================================
// �U��
//=====================================================
void CPlayer::Attack(E_Attack typeAttack)
{
	CRikishi::Attack(typeAttack);
}

//=====================================================
// �U���������̏���
//=====================================================
void CPlayer::HitAttack(E_Attack typeAttack)
{
	CEnemy *pEnemy = CEnemy::GetInstance();

	if (pEnemy == nullptr)
		return;

	float fKnockBack = pEnemy->Hit(typeAttack, Rikishi::DAMAGE_ATTACK);
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayer::Draw(void)
{
	// �p���N���X�̕`��
	CRikishi::Draw();
}

//=====================================================
// �q�b�g����
//=====================================================
float CPlayer::Hit(CRikishi::E_Attack typeAttack, float fDamage)
{
	float fKnockBack = CRikishi::Hit(typeAttack, fDamage);

	CGame *pGame = CGame::GetInstance();

	if (pGame == nullptr)
		return 0.0f;

	D3DXVECTOR3 addPos = { fKnockBack,0.0f,0.0f };

	pGame->AddPosMid(addPos);

	return 0.0f;
}