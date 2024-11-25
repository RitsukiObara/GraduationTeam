//*****************************************************
//
// �`���[�g���A���v���C���[�̏���[playerTutorial.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "playerTutorial.h"
#include "manager.h"
#include "inputManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
//------------------------------
// �ړ��̒萔
//------------------------------
namespace move
{
const float LINE_SPPED_MOVE = 2.0f;	// �ړ����Ă�Ƃ݂Ȃ��������l
const float TIME_COMPLETE = 2.0f;	// �����܂ł����鎞��
}
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CPlayerTutorial::CPlayerTutorial(int nPriority) : CPlayer(nPriority), m_fTimerMove(0.0f), m_bComplete(false)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CPlayerTutorial::~CPlayerTutorial()
{

}

//=====================================================
// ��������
//=====================================================
CPlayerTutorial* CPlayerTutorial::Create(void)
{
	CPlayerTutorial *pPlayerTutorial = nullptr;

	pPlayerTutorial = new CPlayerTutorial;

	if (pPlayerTutorial != nullptr)
		pPlayerTutorial->Init();

	return pPlayerTutorial;
}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayerTutorial::Init(void)
{
	// �p���N���X�̏�����
	CPlayer::Init();
	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CPlayerTutorial::Uninit(void)
{
	// �p���N���X�̏I��
	CPlayer::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CPlayerTutorial::Update(void)
{
	// �p���N���X�̍X�V
	CPlayer::Update();

	// �X�s�[�h�̊m�F
	CheckSpeed();

	// �����Ǘ�����
	ReadExplain();
}

//=====================================================
// �X�s�[�h�̊m�F
//=====================================================
void CPlayerTutorial::CheckSpeed(void)
{
	D3DXVECTOR3 move = GetMove();

	float fSpeed = D3DXVec3Length(&move);

	if (fSpeed > move::LINE_SPPED_MOVE)
	{// ��葬�x���z���Ă�����^�C�}�[�i�s
		m_fTimerMove += CManager::GetDeltaTime();

		// ��莞�Ԉړ��Ői�s
		if (m_fTimerMove > move::TIME_COMPLETE)
			AddProgress(CTutorial::E_State::STATE_MOVE);
	}
}

//=====================================================
// �����Ǘ�����
//=====================================================
void CPlayerTutorial::ReadExplain(void)
{
	CInputManager *pInputMgr = GetInpuManager();

	if (pInputMgr == nullptr)
		return;

	if(pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
		AddProgress(CTutorial::E_State::STATE_EXPLAIN_BREAK);

	if (pInputMgr->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
		AddProgress(CTutorial::E_State::STATE_EXPLAIN_ENEMY);
}

//=====================================================
// �˂�������
//=====================================================
bool CPlayerTutorial::Peck(void)
{
	// �`���[�g���A���i�s
	AddProgress(CTutorial::E_State::STATE_PECK);

	// �p���N���X�̓˂���
	CPlayer::Peck();

	return false;
}

//=====================================================
// �`���[�g���A���̐i�s
//=====================================================
void CPlayerTutorial::AddProgress(CTutorial::E_State state)
{
	CTutorial *pTutorial = CTutorial::GetInstance();
	if (pTutorial == nullptr)
		return;

	CTutorial::E_State stateCur = pTutorial->GetState();
	
	// �w�肵����Ԃƃ`���[�g���A���̏�Ԃ���v������i�s������
	if (stateCur == state)
	{
		pTutorial->AddCntProgress(this);

		m_bComplete = true;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayerTutorial::Draw(void)
{
	// �p���N���X�̕`��
	CPlayer::Draw();
}