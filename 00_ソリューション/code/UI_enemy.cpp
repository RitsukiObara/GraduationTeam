//*****************************************************
//
// �GUI�̏���[UI_enemy.cpp]
// Author:����F�M
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "UI_enemy.h"
#include "UI.h"
#include "texture.h"
#include "inputManager.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const float DIST_NUMBER = 100.0f;	// �����Ԃ̋���
	D3DXVECTOR2 SIZE_NORMAL_NUM = { 0.02f, 0.06f };	// �ʏ퐔���̃T�C�Y
	D3DXVECTOR2 SIZE_MINI_NUM = { 0.005f, 0.010f };	// �~�j�����̃T�C�Y
	D3DXVECTOR3 POS_INITIAL = { 0.5f,0.5f,0.0f };	// �����ʒu
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CUI_Enemy::CUI_Enemy()
{
	m_nEnemy = 0;
	m_fScaleNumber = 0.0f;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CUI_Enemy::~CUI_Enemy()
{

}

//=====================================================
// ��������
//=====================================================
CUI_Enemy* CUI_Enemy::Create(int m_nEnemy)
{

	CUI_Enemy* pUI_enemy = nullptr;

	pUI_enemy = new CUI_Enemy;

	pUI_enemy->SetUI(m_nEnemy);

	m_nEnemy = pUI_enemy->GetUI();

	if (pUI_enemy != nullptr)
	{// ������
		pUI_enemy->Init();
	}

	return pUI_enemy;
}

//=====================================================
// ����������
//=====================================================
HRESULT CUI_Enemy::Init(void)
{
	m_fScaleNumber = 1.0f;	// �����X�P�[���ݒ�

	int m_nEnemy = CUI_Enemy::GetUI();

	// �����ʒu�̐ݒ�
	SetPosition(POS_INITIAL);

	// �����̔z��̃��T�C�Y
	m_aNumber.resize(m_nEnemy);

	// �����̐���
	for (int i = 0; i < m_nEnemy; i++)
	{
		m_aNumber[i] = CNumber::Create(m_nEnemy, 0);	// �����̐���
	}

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CUI_Enemy::Uninit(void)
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
void CUI_Enemy::Update(void)
{
	UpdateNumber();
}

//=====================================================
// �����̍X�V
//=====================================================
void CUI_Enemy::UpdateNumber()
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (m_aNumber.empty())
		return;

	/*for (int i = 0; i < SCORE_DIGIT; i++)
	{
		m_aNumber[i]->SetValue(aValue[i]);
	}*/

	D3DXVECTOR3 pos = GetPosition();

	SetPosition(pos);

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n���݂̐��l�F[%d]", m_nEnemy);
	CDebugProc::GetInstance()->Print("\n�X�R�A�̈ʒu�F[%f,%f,%f]", pos.x, pos.y, pos.z);
#endif
#endif
}

//=====================================================
// �����̃g�����X�t�H�[���ݒ�
//=====================================================
void CUI_Enemy::TransformNumber()
{
	if (m_aNumber.empty())
		return;

	D3DXVECTOR3 posBase = GetPosition();

	// �������A�������Đݒ�
	for (int i = 0; i < m_nEnemy; i++)
	{
		if (m_aNumber[i] == nullptr)
			continue;

		// �Q�Ƃ���T�C�Y�̔ԍ�
		int nIdx = i;

		if (nIdx > 0)
			nIdx--;	// 0�ԖڂłȂ���ΑO��̃T�C�Y���Q�Ƃ���

		/*D3DXVECTOR3 pos = D3DXVECTOR3(posBase.x + (1 * i), posBase.y, posBase.z);

		SetPosition(pos);*/

		//// �p�����[�^�[�ݒ�
		//float fWidth = aSize[nIdx].x * aDigit[nIdx] * 2 + DIST_NUMBER * m_fScaleNumber;	// �T�C�Y�ɉ����Đ����Ԃ̃X�y�[�X��������

		//D3DXVECTOR3 pos = { posBase.x + fWidth * (i - 1), posBase.y, 0.0f };

		//// �p�����[�^�[�ݒ�
		//float fWidth = aSize[nIdx].x * aDigit[nIdx] * 2 + DIST_NUMBER * m_fScaleNumber;	// �T�C�Y�ɉ����Đ����Ԃ̃X�y�[�X��������
		//D3DXVECTOR3 pos = { posBase.x + fWidth * (i - 1), posBase.y, 0.0f };
		//m_aNumber[i]->SetPosition(pos);
		//m_aNumber[i]->SetSizeAll(aSize[i].x, aSize[i].y);

		if (i == 0)	// 0�ȏ�̂Ƃ���������Ȃ�����
			continue;
	}
}

//=====================================================
// �ʒu�̐ݒ�
//=====================================================
void CUI_Enemy::SetPosition(D3DXVECTOR3 pos)
{
	CGameObject::SetPosition(pos);

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();
}

//=====================================================
// �����̃X�P�[���̐ݒ�
//=====================================================
void CUI_Enemy::SetScaleNumber(float fScale)
{
	m_fScaleNumber = fScale;

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();
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
void CUI_Enemy::Draw()
{

}