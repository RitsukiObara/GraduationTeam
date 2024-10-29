//*****************************************************
//
// �R���{UI�̏���[UI_combo.cpp]
// Author:����F�M
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "UI_combo.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "UI.h"
#include "inputManager.h"
#include "debugproc.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int	SCORE_MIN = 0;	// �ŏ��X�R�A
	const int	SCORE_MAX = 999999;	// �ő�X�R�A
	const int SCORE_LIMIT = 9;	// �X�R�A�̏���l
	//const int SCORE_DIGIT = 6;	// �X�R�A�\���̌���
	const float DIST_NUMBER = 0.01f;	// �����Ԃ̋���
	D3DXVECTOR2 SIZE_NORMAL_NUM = { 0.02f, 0.04f };	// �ʏ퐔���̃T�C�Y
	D3DXVECTOR2 SIZE_MINI_NUM = { 0.014f, 0.028f };	// �~�j�����̃T�C�Y
	D3DXVECTOR3 POS_INITIAL = { 0.1f,0.5f,0.0f };	// �����ʒu
	const int	WAITTIME = 60;	// �ؗ�����
	const float	GOAL_X = 0.5f;	// X�̃S�[���n�_
	const float	MOVE_SPEED = 0.1f;	// �ړ����x
	const float	VERTICAL_STOP = 0.15f;	// �c�ړ��̒�~�n�_
	const float	SLOW_MOVE = 0.001f;	// �X���[���x
	const float	THINITY_SPEED = 0.02f;	// �����ɂȂ��Ă������x
	const float	GOAL_Y = 0.08f;	// Y�̃S�[���n�_
	const float	THINITY_COL = 0.0f;	// �����ɂȂ�
	const int	ADD_SEALS_SCORE = 10000;	// �ǉ�����X�R�A(�A�U���V)
	const int	VALUE_SEALS_SCORE = 5;	// �ǉ�����X�R�A�̌���(�A�U���V)
	const float	SCORE_SCALE = 0.8f;	// �X�R�A�̃X�P�[��
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CUI_Combo::CUI_Combo()
{
	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nValue = 0;
	m_nCombo = 0;
	m_State = STATE_BESIDE;
	m_nCntState = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CUI_Combo::~CUI_Combo()
{

}

//=====================================================
// ��������
//=====================================================
CUI_Combo* CUI_Combo::Create()
{
	CUI_Combo* pCombo = nullptr;

	pCombo = new CUI_Combo;

	if (pCombo != nullptr)
	{// ������
		pCombo->Init();

		//���̐ݒ�
		pCombo->SetCombo(2);

		////�X�R�A�̐ݒ�
		//pScore->SetScore(pScore->m_nScore);
	}

	return pCombo;
}

//=====================================================
// ����������
//=====================================================
HRESULT CUI_Combo::Init(void)
{
	m_nCombo = SCORE_MIN;	// �X�R�A�̏�����
	m_fScaleNumber = 1.0f;	// �����X�P�[���ݒ�

	// �����ʒu�̐ݒ�
	SetPosition(POS_INITIAL);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CUI_Combo::Uninit(void)
{
	for (auto it : m_aNumber)
	{
		it->Uninit();
	}

	m_aNumber.clear();

	CGameObject::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CUI_Combo::Update(void)
{
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�R���{UI�̏��
	switch (m_State)
	{
	case STATE_BESIDE:

		if (pos.x < GOAL_X)
		{
			move.x = MOVE_SPEED;

			pos.x += move.x;

			SetPosition(pos);
		}
		else
		{
			m_State = STATE_VERTICAL;
		}

		break;

	case STATE_VERTICAL:

		if (pos.x >= GOAL_X)
		{
			move.y += MOVE_SPEED;

			pos.y -= move.y;

			SetPosition(pos);

			if (pos.y < VERTICAL_STOP)
			{
				SetPosition(pos);

				m_State = STATE_WAIT;
			}
		}

		break;

	case STATE_WAIT:

		m_nCntState++;

		if (m_nCntState >= WAITTIME)
		{
			m_State = STATE_ERASE;
		}

		break;

	case STATE_ERASE:

		if (pos.x > GOAL_Y)
		{
			move.y += SLOW_MOVE;

			pos.y -= move.y;

			SetPosition(pos);

			m_Col.a -= THINITY_SPEED;

			SetColor(m_Col);

		}
		if (m_Col.a <= THINITY_COL)
		{
			Uninit();
		}

		break;
	}

	UpdateNumber();
}

//=====================================================
// �����̍X�V
//=====================================================
void CUI_Combo::UpdateNumber()
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (m_aNumber.empty())
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

	std::vector<int> value;

	value.resize(m_aNumber.size());

	for (int nCnt = 0; nCnt < (int)m_aNumber.size(); nCnt++)
	{
		// �l���v�Z
		value[nCnt] = (m_nCombo % (int)(pow(10, (m_aNumber.size() - (nCnt)))) / (int)(pow(10, (m_aNumber.size() - (nCnt + 1)))));
	}

	// �X�R�A�̉��Z========================================
	if (pInputManager->GetTrigger(CInputManager::BUTTON_SCORE))
	{// �X�R�A�����Z����
		
	}

	for (int i = 0; i < (int)m_aNumber.size(); i++)
	{
		m_aNumber[i]->SetValue(value[i]);
	}

	//D3DXVECTOR3 pos = GetPosition();

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n���݂̃X�R�A�F[%d]", m_nCombo);
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
void CUI_Combo::SetColor(D3DXCOLOR col)
{
	for (auto it : m_aNumber)	// ����
		it->SetColor(col);
}

//=====================================================
// �`�揈��
//=====================================================
void CUI_Combo::Draw()
{
	
}

//=====================================================
// �����̃g�����X�t�H�[���ݒ�
//=====================================================
void CUI_Combo::TransformNumber()
{
	if (m_aNumber.empty())
		return;

	// �����̃T�C�Y
	D3DXVECTOR2 Size = SIZE_NORMAL_NUM * m_fScaleNumber;

	D3DXVECTOR3 posBase = GetPosition();

	// �������A�������Đݒ�
	for (int i = 0; i < (int)m_aNumber.size(); i++)
	{
		if (m_aNumber[i] == nullptr)
			continue;

		// �Q�Ƃ���T�C�Y�̔ԍ�
		int nIdx = i;

		if (nIdx > 0)
			nIdx--;	// 0�ԖڂłȂ���ΑO��̃T�C�Y���Q�Ƃ���

		//// �p�����[�^�[�ݒ�
		//float fWidth = aSize[nIdx].x * aDigit[nIdx] * 2 + DIST_NUMBER * m_fScaleNumber;	// �T�C�Y�ɉ����Đ����Ԃ̃X�y�[�X��������

		//D3DXVECTOR3 pos = { posBase.x + fWidth * (i - 1), posBase.y, 0.0f };

		// �p�����[�^�[�ݒ�
		float fWidth = Size.x * 2 + DIST_NUMBER * m_fScaleNumber;	// �T�C�Y�ɉ����Đ����Ԃ̃X�y�[�X��������
		D3DXVECTOR3 pos = { posBase.x + fWidth * (i - 1), posBase.y, 0.0f };
		m_aNumber[i]->SetPosition(pos);
		m_aNumber[i]->SetSizeAll(Size.x, Size.y);

		if (i == 0)	// 0�ȏ�̂Ƃ���������Ȃ�����
			continue;
	}
}

//=====================================================
// ���̐ݒ�
//=====================================================
void CUI_Combo::SetCombo(int nDigit)
{
	// �����̔z��̃��T�C�Y
	m_aNumber.resize(nDigit);

	// �����̐���
	for (int i = 0; i < nDigit; i++)
	{
		m_aNumber[i] = CNumber::Create(1, 0);	// �����̐���
	}

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();

}