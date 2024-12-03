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
#include "player.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CUI_Combo* CUI_Combo::s_pCombo;	// �i�[�p�̔z��

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int	COMBO_MIN = 0;	// �ŏ��R���{
	const int	COMBO_MAX = 99;	// �ő�R���{
	const float DIST_NUMBER = 0.01f;	// �����Ԃ̋���
	const D3DXVECTOR2 SIZE_NORMAL_NUM = { 40.0f, 40.0f };	// �ʏ퐔���̃T�C�Y
	const D3DXVECTOR2 SIZE_MINI_NUM = { 0.014f, 0.028f };	// �~�j�����̃T�C�Y
	const D3DXVECTOR3 POS_INITIAL = { 0.0f,0.0f,0.0f };	// �����ʒu
	const int	WAITTIME = 100;	// �ؗ�����
	const float	GOAL_X = 0.5f;	// X�̃S�[���n�_
	const float	MOVE_SPEED = 10.0f;	// �ړ����x
	const float	VERTICAL_STOP = 100.0f;	// �c�ړ��̒�~�n�_
	const float	SLOW_MOVE = 1.0f;	// �X���[���x
	const float	THINITY_SPEED = 0.02f;	// �����ɂȂ��Ă������x
	const float	GOAL_Z = 100.0f;	// Y�̃S�[���n�_
	const float	THINITY_COL = 0.0f;	// �����ɂȂ�
	const int	VALUE_COMBO = 2;	// �ǉ�����X�R�A�̌���(�A�U���V)
	const float	COMBO_SCALE = 0.7f;	// �X�R�A�̃X�P�[��
	const D3DXCOLOR	NORMAL_COLOR = { 1.0f,1.0f,0.0f,1.0f };	// �X�R�A�̏����F
	const float	COMBO_POS_X = 50.0f;	// �R���{��X���W
	const float	COMBO_POS_Z = 70.0f;	// �R���{��Z���W
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CUI_Combo::CUI_Combo()
{
	m_Col = NORMAL_COLOR;
	m_nValue = 0;
	m_nCombo = 0;
	m_State = STATE_BESIDE;
	m_nCntState = 0;
	m_ShiftPos = POS_INITIAL;
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
	if (s_pCombo == nullptr)
	{
		s_pCombo = new CUI_Combo;

		s_pCombo->Init();

		s_pCombo->SetScaleNumber(COMBO_SCALE);

		//���̐ݒ�
		s_pCombo->SetCombo(VALUE_COMBO);

		s_pCombo->SetColor(NORMAL_COLOR);

		////�X�R�A�̐ݒ�
		//pScore->SetScore(pScore->m_nScore);
	}

	return s_pCombo;
}

//=====================================================
// ����������
//=====================================================
HRESULT CUI_Combo::Init(void)
{
	m_nCombo = COMBO_MIN;	// �X�R�A�̏�����
	m_nValue = VALUE_COMBO;	// �����̏�����
	m_fScaleNumber = COMBO_SCALE;	// �����X�P�[���ݒ�
	m_State = STATE_VERTICAL;	// ��Ԃ̏�����
	m_ShiftPos = D3DXVECTOR3(0.0f, 0.0f, COMBO_POS_Z);

	// �����ʒu�̐ݒ�
	SetPosition(POS_INITIAL);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CUI_Combo::Uninit(void)
{
	if (m_aNumber3D != nullptr)
	{
		m_aNumber3D->Uninit();
	}

	CGameObject::Uninit();

	s_pCombo = nullptr;
}

//=====================================================
// �X�V����
//=====================================================
void CUI_Combo::Update(void)
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
			m_State = STATE_ERASE;
		}

		break;

	case STATE_ERASE:

		m_ShiftPos.z += SLOW_MOVE;

		m_Col.a -= THINITY_SPEED;

		if (m_Col.a <= THINITY_COL)
		{
			Uninit();

			return;
		}

		break;
	}

	SetColor(m_Col);

	SetPosition(D3DXVECTOR3(pos.x + COMBO_POS_X, pos.y, pos.z + m_ShiftPos.z + COMBO_POS_Z));

	UpdateNumber();
}

//=====================================================
// �����̍X�V
//=====================================================
void CUI_Combo::UpdateNumber()
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

	m_aNumber3D->SetValue(m_nCombo, m_aNumber3D->GetNumPlace());

	//D3DXVECTOR3 pos = GetPosition();

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n���݂̃R���{�F[%d]", m_nCombo);
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
	m_aNumber3D->SetColor(col);
}

//=====================================================
// �ʒu�̐ݒ�
//=====================================================
void CUI_Combo::SetPosition(D3DXVECTOR3 pos)
{
	CGameObject::SetPosition(pos);

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();
}

//=====================================================
// �����̃X�P�[���̐ݒ�
//=====================================================
void CUI_Combo::SetScaleNumber(float fScale)
{
	m_fScaleNumber = fScale;

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();
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
void CUI_Combo::SetCombo(int nDigit)
{
	m_aNumber3D = CNumber3D::Create(nDigit, 0);	// �����̐���

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();
}

//=====================================================
// �R���{�̑���
//=====================================================
void CUI_Combo::AddCombo(void)
{
	m_nCombo++;

	m_State = STATE_WAIT;

	m_nCntState = 0;
}

//=====================================================
// �R���{�̃C���X�^���X�擾
//=====================================================
CUI_Combo* CUI_Combo::GetInstance()
{
	Create();

	return s_pCombo;
}