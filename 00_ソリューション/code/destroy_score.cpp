//*****************************************************
//
// �G��|�������̃X�R�A����[destroy_score.cpp]
// Author:����F�M
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "destroy_score.h"
#include "UI.h"
#include "texture.h"
#include "inputManager.h"
#include "debugproc.h"
#include "player.h"
#include "UI_combo.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int	SCORE_MIN = 0;	// �ŏ��X�R�A
	const float DIST_NUMBER = 0.01f;	// �����Ԃ̋���
	const D3DXVECTOR2 SIZE_NORMAL_NUM = { 30.0f, 30.0f };	// �ʏ퐔���̃T�C�Y
	const D3DXVECTOR3 POS_INITIAL = { 0.0f,0.0f,0.0f };	// �����ʒu
	const float	GOAL_X = 0.5f;	// X�̃S�[���n�_
	const float	MOVE_SPEED = 10.0f;	// �ړ����x
	const float	VERTICAL_STOP = 100.0f;	// �c�ړ��̒�~�n�_
	const float	GOAL_Z = 100.0f;	// Y�̃S�[���n�_
	const float	SCORE_SCALE = 1.0f;	// �X�R�A�̃X�P�[��
	const float	NORMAL_ALPHA = 1.0f;	// �X�R�A�̏��������x
	const int	ADD_SCORE[CEnemy::TYPE_MAX] =				// �A�U���V�̃X�R�A
	{
		1000,		// �X�R�A
		2000,		// �V���N�}
	};
	const int	VALUE_SCORE = 5;		// ����
	const float	SCORE_POS_X = 150.0f;	// �X�R�A��X���W
	const float	SCORE_POS_Z = 50.0f;	// �X�R�A��Z���W
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CDestroyScore::CDestroyScore()
{
	m_nScore = 0;
	m_state = CUI_Combo::STATE_WAIT;
	m_ShiftPos = POS_INITIAL;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CDestroyScore::~CDestroyScore()
{

}

//=====================================================
// ��������
//=====================================================
CDestroyScore* CDestroyScore::Create()
{
	CDestroyScore* pScore = new CDestroyScore;

	if (pScore != nullptr)
	{
		pScore->Init();

		pScore->SetScaleNumber(SCORE_SCALE);

		//���̐ݒ�
		pScore->SetScore();

		pScore->SetAlpha(NORMAL_ALPHA);
	}
	
	return pScore;
}

//=====================================================
// ����������
//=====================================================
HRESULT CDestroyScore::Init(void)
{
	m_nScore = SCORE_MIN;	// �X�R�A�̏�����
	m_fScaleNumber = SCORE_SCALE;	// �����X�P�[���ݒ�
	m_state = CUI_Combo::STATE_WAIT;	// ��Ԃ̏�����
	m_ShiftPos = D3DXVECTOR3(0.0f, 0.0f, SCORE_POS_Z);

	// �����ʒu�̐ݒ�
	SetPosition(POS_INITIAL);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CDestroyScore::Uninit(void)
{
	if (m_aNumber3D != nullptr)
	{
		m_aNumber3D->Uninit();
		m_aNumber3D = nullptr;
	}

	CGameObject::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CDestroyScore::Update(void)
{
	if (CPlayer::GetInstance().empty())
	{
		return;
	}

	D3DXVECTOR3 pos = (*CPlayer::GetInstance().begin())->GetPosition();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetPosition(D3DXVECTOR3(pos.x - SCORE_POS_X, pos.y, pos.z + m_ShiftPos.z));

	UpdateNumber();
}

//=====================================================
// �����̍X�V
//=====================================================
void CDestroyScore::UpdateNumber()
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (m_aNumber3D == nullptr)
		return;

	m_aNumber3D->SetValue(m_nScore, m_aNumber3D->GetNumPlace());

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n���݂̃X�R�A�F[%d]", m_nScore);
#endif
#endif
}

//=====================================================
// �����x�̐ݒ�
//=====================================================
void CDestroyScore::SetAlpha(float fAlpha)
{
	if (m_aNumber3D != nullptr)
		m_aNumber3D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));
}

//=====================================================
// ��Ԃ̐ݒ�
//=====================================================
void CDestroyScore::SetState(CUI_Combo::E_State state)
{
	m_state = state;
}

//=====================================================
// ���炷���̐ݒ�
//=====================================================
void CDestroyScore::SetShiftPos(D3DXVECTOR3 shiftpos)
{
	m_ShiftPos = shiftpos;
}

//=====================================================
// �ʒu�̐ݒ�
//=====================================================
void CDestroyScore::SetPosition(D3DXVECTOR3 pos)
{
	CGameObject::SetPosition(pos);

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();
}

//=====================================================
// �����̃X�P�[���̐ݒ�
//=====================================================
void CDestroyScore::SetScaleNumber(float fScale)
{
	m_fScaleNumber = fScale;

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();
}

//=====================================================
// �`�揈��
//=====================================================
void CDestroyScore::Draw()
{

}

//=====================================================
// �����̃g�����X�t�H�[���ݒ�
//=====================================================
void CDestroyScore::TransformNumber()
{
	if (m_aNumber3D == nullptr)
		return;

	// �����̃T�C�Y
	D3DXVECTOR2 Size = SIZE_NORMAL_NUM * m_fScaleNumber;

	D3DXVECTOR3 posBase = GetPosition();

	// �p�����[�^�[�ݒ�
	m_aNumber3D->SetPosition(posBase);
	m_aNumber3D->SetSizeAll(Size.x, Size.y);

}

//=====================================================
// ���̐ݒ�
//=====================================================
void CDestroyScore::SetScore(void)
{
	m_aNumber3D = CNumber3D::Create(VALUE_SCORE, 0);	// �����̐���

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();
}

//=====================================================
// �X�R�A�̎擾
//=====================================================
int CDestroyScore::GetScore(void)
{
	return m_nScore;
}

//=====================================================
// �G��|�������̃X�R�A����
//=====================================================
void CDestroyScore::AddDestroyScore(CEnemy::TYPE type)
{
	// ��ނ����݂��Ȃ��ꍇ�A���̊֐��𔲂���
	if (type >= CEnemy::TYPE_MAX) { assert(false); return; }

	m_nScore += ADD_SCORE[type];
}
