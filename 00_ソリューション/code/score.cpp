//*****************************************************
//
// �X�R�A�̏���[Score.cpp]
// Author:��؈�^
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "score.h"
#include "UI.h"
#include "texture.h"
#include "inputManager.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int	SCORE_MIN = 0;	// �ŏ��X�R�A
	const int	SCORE_MAX = 999999;	// �ő�X�R�A
	const int SCORE_LIMIT = 9;	// �X�R�A�̏���l
	const int SCORE_DIGIT = 6;	// �X�R�A�\���̌���
	const float DIST_NUMBER = 0.01f;	// �����Ԃ̋���
	D3DXVECTOR2 SIZE_NORMAL_NUM = { 0.02f, 0.06f };	// �ʏ퐔���̃T�C�Y
	D3DXVECTOR2 SIZE_MINI_NUM = { 0.014f, 0.028f };	// �~�j�����̃T�C�Y
	D3DXVECTOR3 POS_INITIAL = { 0.01f,0.5f,0.0f };	// �����ʒu
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CScore::CScore()
{
	m_nScore = SCORE_MIN;
	m_fScaleNumber = 0.0f;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CScore::~CScore()
{

}

//=====================================================
// ��������
//=====================================================
CScore* CScore::Create(void)
{
	CScore* pScore = nullptr;

	pScore = new CScore;

	if (pScore != nullptr)
	{// ������
		pScore->Init();
	}

	return pScore;
}

//=====================================================
// �X�R�A�ۑ�����
//=====================================================
void CScore::SaveScore(int nSecond)
{
	// �t�@�C�����J��
	std::ofstream file("data\\TEMP\\score.bin", std::ios_base::binary);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�X�R�A�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return;
	}

	// �����̃X�R�A�������o��
	file.write((char*)&nSecond, sizeof(int));

	// �t�@�C�������
	file.close();
}

//=====================================================
// �X�R�A�Ǎ�����
//=====================================================
int CScore::LoadScore(void)
{
	// �t�@�C�����J��
	std::ifstream file("data\\TEMP\\score.bin", std::ios_base::binary);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�X�R�A�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return 0;
	}

	// �����̃X�R�A��ǂݍ���
	int nScore = 0;
	file.read((char*)&nScore, sizeof(int));

	// �t�@�C�������
	file.close();

	// �ǂݍ��񂾃X�R�A��Ԃ�
	return nScore;
}

//=====================================================
// ����������
//=====================================================
HRESULT CScore::Init(void)
{
	m_nScore = SCORE_MIN;	// �X�R�A�̏�����
	m_fScaleNumber = 1.0f;	// �����X�P�[���ݒ�

	// �����ʒu�̐ݒ�
	SetPosition(POS_INITIAL);

	// �����̔z��̃��T�C�Y
	m_aNumber.resize(SCORE_DIGIT);

	int aDigit[SCORE_DIGIT] =
	{// ����
		1,
		1,
		1,
		1,
		1,
		1,
	};

	// �����̐���
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		m_aNumber[i] = CNumber::Create(aDigit[i], 0);	// �����̐���
	}

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CScore::Uninit(void)
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
void CScore::Update(void)
{
	UpdateNumber();
}

//=====================================================
// �����̍X�V
//=====================================================
void CScore::UpdateNumber()
{
	CInputManager* pInputManager = CInputManager::GetInstance();

	if (m_aNumber.empty())
		return;

	// �l�̗p��
	int aValue[SCORE_DIGIT] =
	{
		(m_nScore % 1000000 / 100000),
		(m_nScore % 100000 / 10000),
		(m_nScore % 10000 / 1000),
		(m_nScore % 1000 / 100),
		(m_nScore % 100 / 10),
		(m_nScore % 10),
	};

	// �X�R�A�̉��Z========================================
	if (pInputManager->GetTrigger(CInputManager::BUTTON_SCORE))
	{// �X�R�A�����Z����
		AddScore(1000);
	}

	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		m_aNumber[i]->SetValue(aValue[i]);
	}

	//D3DXVECTOR3 pos = GetPosition();

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n���݂̃X�R�A�F[%d]", m_nScore);
	//CDebugProc::GetInstance()->Print("\n�X�R�A�̈ʒu�F[%f,%f,%f]", pos.x, pos.y, pos.z);
#endif
#endif
}

//=====================================================
// �����̃g�����X�t�H�[���ݒ�
//=====================================================
void CScore::TransformNumber()
{
	if (m_aNumber.empty())
		return;

	int aDigit[SCORE_DIGIT] =
	{// ����
		1,
		1,
		1,
		1,
		1,
		1,
	};

	D3DXVECTOR2 aSize[SCORE_DIGIT] =
	{// �����̃T�C�Y
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber
	};

	D3DXVECTOR3 posBase = GetPosition();

	// �������A�������Đݒ�
	for (int i = 0; i < SCORE_DIGIT; i++)
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
		float fWidth = aSize[nIdx].x * aDigit[nIdx] * 2 + DIST_NUMBER * m_fScaleNumber;	// �T�C�Y�ɉ����Đ����Ԃ̃X�y�[�X��������
		D3DXVECTOR3 pos = { posBase.x + fWidth * (i - 1), posBase.y, 0.0f };
		m_aNumber[i]->SetPosition(pos);
		m_aNumber[i]->SetSizeAll(aSize[i].x, aSize[i].y);

		if (i == 0)	// 0�ȏ�̂Ƃ���������Ȃ�����
			continue;
	}
}

//=====================================================
// �ʒu�̐ݒ�
//=====================================================
void CScore::SetPosition(D3DXVECTOR3 pos)
{
	CGameObject::SetPosition(pos);

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();
}

//=====================================================
// �����̃X�P�[���̐ݒ�
//=====================================================
void CScore::SetScaleNumber(float fScale)
{
	m_fScaleNumber = fScale;

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();
}

//=====================================================
// �F�̐ݒ�
//=====================================================
void CScore::SetColor(D3DXCOLOR col)
{
	for (auto it : m_aNumber)	// ����
			it->SetColor(col);
}

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
void CScore::Draw()
{

}