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
#include "game.h"
#include "player.h"
#include "UI_combo.h"
//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CDestroyScore* CDestroyScore::s_pDestroyScore;	// �i�[�p�̔z��

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int	SCORE_MIN = 0;	// �ŏ��X�R�A
	const float DIST_NUMBER = 0.01f;	// �����Ԃ̋���
	const D3DXVECTOR2 SIZE_NORMAL_NUM = { 30.0f, 30.0f };	// �ʏ퐔���̃T�C�Y
	const D3DXVECTOR3 POS_INITIAL = { 0.0f,0.0f,0.0f };	// �����ʒu
	const int	WAITTIME = 60;	// �ؗ�����
	const float	GOAL_X = 0.5f;	// X�̃S�[���n�_
	const float	MOVE_SPEED = 10.0f;	// �ړ����x
	const float	VERTICAL_STOP = 100.0f;	// �c�ړ��̒�~�n�_
	const float	SLOW_MOVE = 1.0f;	// �X���[���x
	const float	THINITY_SPEED = 0.02f;	// �����ɂȂ��Ă������x
	const float	GOAL_Z = 100.0f;	// Y�̃S�[���n�_
	const float	THINITY_COL = 0.0f;	// �����ɂȂ�
	const float	SCORE_SCALE = 1.0f;	// �X�R�A�̃X�P�[��
	const D3DXCOLOR	NORMAL_COLOR = { 1.0f,1.0f,1.0f,1.0f };	// �X�R�A�̏����F
	const int	ADD_SEALS_SCORE = 1000;	// �A�U���V�̃X�R�A
	const int	VALUE_SEALS_SCORE = 5;	// �A�U���V�̌���
	const float	SCORE_POS_X = 150.0f;	// �X�R�A��X���W
	const float	SCORE_POS_Z = 50.0f;	// �X�R�A��Z���W
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CDestroyScore::CDestroyScore()
{
	m_Col = NORMAL_COLOR;
	m_nValue = 0;
	m_nScore = 0;
	m_State = STATE_BESIDE;
	m_nCntState = 0;
	m_ShiftPos = POS_INITIAL;
	m_nAddScore = 0;
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
	if (s_pDestroyScore == nullptr)
	{
		s_pDestroyScore = new CDestroyScore;

		s_pDestroyScore->Init();

		s_pDestroyScore->SetScaleNumber(SCORE_SCALE);

		//���̐ݒ�
		s_pDestroyScore->SetScore(s_pDestroyScore->m_nValue);

		s_pDestroyScore->SetColor(NORMAL_COLOR);
	}
	
	return s_pDestroyScore;
}

//=====================================================
// ����������
//=====================================================
HRESULT CDestroyScore::Init(void)
{
	m_nScore = SCORE_MIN;	// �X�R�A�̏�����
	m_nValue = VALUE_SEALS_SCORE; //�����̏�����
	m_fScaleNumber = SCORE_SCALE;	// �����X�P�[���ݒ�
	m_State = STATE_VERTICAL;	//��Ԃ̏�����
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
	}

	CGameObject::Uninit();

	s_pDestroyScore = nullptr;
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

	//�R���{UI�̏��
	switch (m_State)
	{
	case STATE_BESIDE:

		m_State = STATE_VERTICAL;

		break;

	case STATE_VERTICAL:

		m_ShiftPos.z += MOVE_SPEED;

		if (m_ShiftPos.z >= VERTICAL_STOP)
		{
			m_State = STATE_WAIT;
		}

		break;

	case STATE_WAIT:

		m_nCntState++;

		m_Col = NORMAL_COLOR;

		if (m_nCntState >= WAITTIME)
		{
			m_State = STATE_ADD;
		}

		break;

	case STATE_ADD:

		m_ShiftPos.z += SLOW_MOVE;

		m_Col.a -= THINITY_SPEED;

		if (m_Col.a <= THINITY_COL)
		{
			

			return;
		}

		break;
	}

	SetColor(D3DXCOLOR(m_Col));

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
	//CDebugProc::GetInstance()->Print("\n�X�R�A�̈ʒu�F[%f,%f,%f]", pos.x, pos.y, pos.z);
#endif
#endif
}

//=====================================================
// �F�̐ݒ�
//=====================================================
void CDestroyScore::SetColor(D3DXCOLOR col)
{
	m_aNumber3D->SetColor(col);
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
void CDestroyScore::SetScore(int nDigit)
{
	m_aNumber3D = CNumber3D::Create(nDigit, 0);	// �����̐���

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();

}

//=====================================================
// �Z�b�g�X�R�A����
//=====================================================
void CDestroyScore::SetEnemyScore(CEnemy::TYPE type)
{
	switch (type)
	{
	case CEnemy::TYPE_SEALS:

		m_nAddScore = ADD_SEALS_SCORE;

		break;

	default:
		break;
	}
}

//=====================================================
// �G��|�������̃X�R�A����
//=====================================================
void CDestroyScore::AddDestroyScore(CEnemy::TYPE type)
{
	//�G���Ƃ̃X�R�A�̐ݒ�
	SetEnemyScore(type);

	m_nScore += m_nAddScore;

	m_State = STATE_WAIT;

	m_nCntState = 0;
}

//=====================================================
// �R���{�X�R�A���v�Z���ĉ��Z
//=====================================================
void CDestroyScore::AddComboScore(void)
{
	CUI_Combo *pUICombo = CUI_Combo::GetInstance();
	if (pUICombo == nullptr)
		return;

	int nCombo = pUICombo->GetCombo();

	//�R���{�{���Ƃ̃X�R�A�v�Z
	m_nScore = m_nScore * nCombo;

	//�X�R�A�����Z
	game::AddScore(m_nScore);

	Uninit();
}

//=====================================================
// �G��|�������̃X�R�A�C���X�^���X�擾
//=====================================================
CDestroyScore* CDestroyScore::GetInstance()
{
	Create();

	return s_pDestroyScore;
}