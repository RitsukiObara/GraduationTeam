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

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int	SCORE_MIN = 0;	// �ŏ��X�R�A
	const float DIST_NUMBER = 0.01f;	// �����Ԃ̋���
	const D3DXVECTOR2 SIZE_NORMAL_NUM = { 50.0f, 50.0f };	// �ʏ퐔���̃T�C�Y
	const D3DXVECTOR3 POS_INITIAL = { 0.0f,0.0f,0.0f };	// �����ʒu
	const int	WAITTIME = 60;	// �ؗ�����
	const float	GOAL_X = 0.5f;	// X�̃S�[���n�_
	const float	MOVE_SPEED = 0.1f;	// �ړ����x
	const float	VERTICAL_STOP = 0.15f;	// �c�ړ��̒�~�n�_
	const float	SLOW_MOVE = 0.001f;	// �X���[���x
	const float	THINITY_SPEED = 0.02f;	// �����ɂȂ��Ă������x
	const float	GOAL_Y = 0.1f;	// Y�̃S�[���n�_
	const float	THINITY_COL = 0.0f;	// �����ɂȂ�
	const int	VALUE_SCORE = 2;	// �ǉ�����X�R�A�̌���(�A�U���V)
	const float	SCORE_SCALE = 1.0f;	// �X�R�A�̃X�P�[��
	const D3DXCOLOR	NORMAL_COLOR = { 1.0f,1.0f,1.0f,1.0f };	// �X�R�A�̏����F
	const int	ADD_SEALS_SCORE = 1000;	// �A�U���V�̃X�R�A
	const int	VALUE_SEALS_SCORE = 4;	// �A�U���V�̌���
	const float	SCORE_POS_X = 200.0f;	// �X�R�A��X���W
	const float	SCORE_POS_Y = 150.0f;	// �X�R�A��Y���W
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
CDestroyScore* CDestroyScore::Create(CEnemy::TYPE type)
{
	CDestroyScore* pScore = nullptr;

	pScore = new CDestroyScore;

	if (pScore != nullptr)
	{// ������
		pScore->Init();

		pScore->SetScaleNumber(SCORE_SCALE);

		//�G���Ƃ̃X�R�A�̐ݒ�
		pScore->SetEnemyScore(type);

		//���̐ݒ�
		pScore->SetScore(pScore->m_nValue);
	}

	return pScore;
}

//=====================================================
// ����������
//=====================================================
HRESULT CDestroyScore::Init(void)
{
	m_nScore = SCORE_MIN;	// �X�R�A�̏�����
	m_nValue = VALUE_SCORE; //�����̏�����
	m_fScaleNumber = SCORE_SCALE;	// �����X�P�[���ݒ�
	m_State = STATE_WAIT;	//��Ԃ̏�����


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

	SetPosition(D3DXVECTOR3(pos.x - SCORE_POS_X,pos.y,pos.z + SCORE_POS_Y));

	//�R���{UI�̏��
	switch (m_State)
	{
	case STATE_BESIDE:

		/*if (pos.x < GOAL_X)
		{
			move.x = MOVE_SPEED;

			pos.x += move.x;

			SetPosition(pos);
		}
		else
		{*/

		m_State = STATE_VERTICAL;
		//}

		break;

	case STATE_VERTICAL:

		/*if (pos.x >= GOAL_X)
		{
			move.y += MOVE_SPEED;

			pos.y -= move.y;

			SetPosition(pos);

			if (pos.y < VERTICAL_STOP)
			{*/

		m_State = STATE_WAIT;
		/*	}
		}*/

		break;

	case STATE_WAIT:

		m_nCntState++;

		if (m_nCntState >= WAITTIME)
		{
			m_State = STATE_ADD;
		}

		break;

	case STATE_ADD:

		/*if (pos.x > GOAL_Y)
		{*/

		/*move.y = SLOW_MOVE;

		pos.y -= move.y;

		SetPosition(pos);*/

		m_Col.a -= THINITY_SPEED;

		SetColor(D3DXCOLOR(m_Col));

		//}
		if (m_Col.a <= THINITY_COL)
		{
			CGame::GetInstance()->GetScore()->AddScore(m_nScore);

			Uninit();
		}

		break;
	}

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

	//// �l�̗p��
	//int aValue[SCORE_DIGIT] =
	//{
	//	(m_nScore % 1000000 / 100000),
	//	(m_nScore % 100000 / 10000),
	//	(m_nScore % 10000 / 1000),
	//	(m_nScore % 1000 / 100),
	//	(m_nScore % 100 / 10),
	//	(m_nScore % 10),
	//};

	//std::vector<int> value;

	//value.resize(m_aNumber3D->GetNumAll());

	//for (int nCnt = 0; nCnt < (int)m_aNumber3D->GetNumAll(); nCnt++)
	//{
	//	// �l���v�Z
	//	value[nCnt] = (m_nCombo % (int)(pow(10, (m_aNumber3D->GetNumAll() - (nCnt)))) / (int)(pow(10, (m_aNumber3D->GetNumAll() - (nCnt + 1)))));
	//}

	// �X�R�A�̉��Z========================================
	if (pInputManager->GetTrigger(CInputManager::BUTTON_SCORE))
	{// �X�R�A�����Z����

	}

	m_aNumber3D->SetValue(m_nScore, m_aNumber3D->GetNumPlace());

	//D3DXVECTOR3 pos = GetPosition();

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n���݂̃X�R�A�F[%d]", m_nScore);
	//CDebugProc::GetInstance()->Print("\n�X�R�A�̈ʒu�F[%f,%f,%f]", pos.x, pos.y, pos.z);
#endif
#endif
}

////=====================================================
//// �F�̐ݒ�
////=====================================================
//void CUI_Combo::SetColor(E_Number number, D3DXCOLOR col)
//{
//	if (number < 0 || number > SCORE_DIGIT)
//		return;
//
//	if (number == SCORE_DIGIT)
//	{// �S�����̐F�ݒ�
//		for (auto it : m_aNumber)	// ����
//			it->SetColor(col);
//	}
//	else
//	{// �e�����̐F�ݒ�
//		m_aNumber[number]->SetColor(col);
//	}
//}
//
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

	//// �p�����[�^�[�ݒ�
	//float fWidth = aSize[nIdx].x * aDigit[nIdx] * 2 + DIST_NUMBER * m_fScaleNumber;	// �T�C�Y�ɉ����Đ����Ԃ̃X�y�[�X��������

	//D3DXVECTOR3 pos = { posBase.x + fWidth * (i - 1), posBase.y, 0.0f };

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

		m_nScore = ADD_SEALS_SCORE;
		m_nValue = VALUE_SEALS_SCORE;

		break;

	default:
		break;
	}
}