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

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int	WAITTIME = 60;	// �ؗ�����
	const float	GOAL_X = 0.2f;	// X�̃S�[���n�_
	const float	MOVE_SPEED = 0.01f;	// �ړ����x
	const float	VERTICAL_STOP = 0.15f;	// �c�ړ��̒�~�n�_
	const float	SLOW_MOVE = 0.001f;	// �X���[���x
	const float	THINITY_SPEED = 0.02f;	// �����ɂȂ��Ă������x
	const float	GOAL_Y = 0.1f;	// Y�̃S�[���n�_
	const float	THINITY_COL = 0.0f;	// �����ɂȂ�
	const int	ADD_SEALS_SCORE = 1000;	// �ǉ�����X�R�A(�A�U���V)
	const int	VALUE_SEALS_SCORE = 4;	// �ǉ�����X�R�A�̌���(�A�U���V)
	const float	SCORE_SCALE = 0.6f;	// �X�R�A�̃X�P�[��
	D3DXVECTOR3	SCORE_PLACE = { 0.5f,0.6f,0.0f };	// �X�R�A�̏ꏊ
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CDestroyScore::CDestroyScore()
{
	m_State = STATE_BESIDE;
	m_nCntState = 0;
	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nScore = 0;
	m_nValue = 0;
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
		pScore->SetData(pScore->m_nValue);

		//�X�R�A�̐ݒ�
		pScore->SetScore(pScore->m_nScore);
	}

	return pScore;
}

//=====================================================
// ����������
//=====================================================
HRESULT CDestroyScore::Init(void)
{
	CScore::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CDestroyScore::Uninit(void)
{
	CScore::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CDestroyScore::Update(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(SCORE_PLACE);
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�G�|������UI�̏��
	switch (m_State)
	{
	case STATE_BESIDE:

		/*if (pos.x < GOAL_X)
		{
			move.x += MOVE_SPEED;

			pos.x += move.x;*/

			SetPosition(pos);
		/*}
		else
		{*/
			m_State = STATE_VERTICAL;
		/*}*/

		break;

	case STATE_VERTICAL:

		/*if (pos.x > GOAL_X)
		{
			move.y += MOVE_SPEED;

			pos.y -= move.y;

			SetPosition(pos);

			if (pos.y < VERTICAL_STOP)
			{*/
				/*SetPosition(pos);*/

				m_State = STATE_WAIT;
			/*}*/
		/*}*/

		break;

	case STATE_WAIT:

		m_nCntState++;

		if (m_nCntState >= WAITTIME)
		{
			m_State = STATE_ADD;
		}

		break;

	case STATE_ADD:

		
			//�������������
			move.y += SLOW_MOVE;

			pos.y -= move.y;

			SetPosition(pos);

			//�F�𓧖��ɂ��Ă���
			m_Col.a -= THINITY_SPEED;

			SetColor(m_Col);
		
		if (pos.y <= GOAL_Y || m_Col.a <= THINITY_COL)
		{
			//�X�R�A���Z
			CGame::GetInstance()->GetScore()->AddScore(m_nScore);

			//�I��
			Uninit();
		}

		break;
	}

	CScore::Update();
}

////=====================================================
//// �F�̐ݒ�
////=====================================================
//void CScore::SetColor(E_Number number, D3DXCOLOR col)
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
////=====================================================
//// �F�̎擾
////=====================================================
//D3DXCOLOR CScore::GetColor(E_Number number)
//{
//	if (number < 0 || number >= SCORE_DIGIT)
//		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//	return m_aNumber[number]->GetColor();
//}

//=====================================================
// �`�揈��
//=====================================================
void CDestroyScore::Draw()
{
	CScore::Draw();
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