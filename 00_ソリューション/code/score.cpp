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

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const float	TIME_MIN = 0.0f;	// �ŏ��^�C��
	const float	TIME_MAX = 60.0f * 99.0f + 59.0f + 0.999f;	// �ő�^�C��
	const int MINUTES_LIMIT = 9;	// ���̏���l
	const int SECOND_LIMIT = 59;	// �b�̏���l
	const int MILLI_LIMIT = 99;	// �~���b�̏��
	const int MINUTES_DIGIT = 2;	// ���\���̌���
	const int TIME_DIGIT = 2;	// ���ꂼ��̌���
	const float MAGNIFICATION = 100.0f;	// �|����{��
	const float DIST_NUMBER = 0.03f;	// �����Ԃ̋���
	D3DXVECTOR2 SIZE_NORMAL_NUM = { 0.02f, 0.06f };	// �ʏ퐔���̃T�C�Y
	D3DXVECTOR2 SIZE_MINI_NUM = { 0.014f, 0.028f };	// �~�j�����̃T�C�Y
	D3DXVECTOR3 POS_INITIAL = { 0.1f,0.5f,0.0f };	// �����ʒu
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CScore::CScore()
{
	m_fScore = 0.0f;
	m_fScaleNumber = 0.0f;
	m_bStop = false;
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
	CScore* pTimer = nullptr;

	pTimer = new CScore;

	if (pTimer != nullptr)
	{// ������
		pTimer->Init();
	}

	return pTimer;
}

//=====================================================
// �X�R�A�ۑ�����
//=====================================================
void CScore::SaveScore(float fSecond)
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
	file.write((char*)&fSecond, sizeof(float));

	// �t�@�C�������
	file.close();
}

//=====================================================
// �X�R�A�Ǎ�����
//=====================================================
float CScore::LoadScore(void)
{
	// �t�@�C�����J��
	std::ifstream file("data\\TEMP\\score.bin", std::ios_base::binary);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�X�R�A�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return 0.0f;
	}

	// �����̃X�R�A��ǂݍ���
	float fTime = 0.0f;
	file.read((char*)&fTime, sizeof(float));

	// �t�@�C�������
	file.close();

	// �ǂݍ��񂾃X�R�A��Ԃ�
	return fTime;
}

//=====================================================
// ����������
//=====================================================
HRESULT CScore::Init(void)
{
	m_fScore = 0.0f;	// �X�R�A�̏�����
	m_fScaleNumber = 1.0f;	// �����X�P�[���ݒ�
	m_bStop = false;	// �^�C�}�[��~�̃t���O

	// �����ʒu�̐ݒ�
	SetPosition(POS_INITIAL);

	// �����̔z��̃��T�C�Y
	m_aNumber.resize(E_Number::NUMBER_MAX);

	int aDigit[E_Number::NUMBER_MAX] =
	{// ����
		MINUTES_DIGIT,
		TIME_DIGIT,
		TIME_DIGIT
	};

	// �����̐���
	for (int i = 0; i < E_Number::NUMBER_MAX; i++)
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

}

//=====================================================
// �����̍X�V
//=====================================================
void CScore::UpdateNumber()
{
	if (m_aNumber.empty())
		return;

	// �l�̗p��
	int aValue[E_Number::NUMBER_MAX] =
	{
#if 0
		(int)m_fSecond / 60,
		(int)m_fSecond % 60,
		(int)(m_fSecond * MAGNIFICATION) % (int)MAGNIFICATION
#else
		(int)((DWORD)(m_fScore * 1000.0f) / 60000),
		(int)((DWORD)(m_fScore * 1000.0f) / 1000 % 60),
		(int)((DWORD)(m_fScore * 1000.0f) % 1000)
#endif
	};

	for (int i = 0; i < E_Number::NUMBER_MAX; i++)
	{
		m_aNumber[i]->SetValue(aValue[i]);
	}
}

//=====================================================
// �����̃g�����X�t�H�[���ݒ�
//=====================================================
void CScore::TransformNumber()
{
	if (m_aNumber.empty())
		return;

	int aDigit[E_Number::NUMBER_MAX] =
	{// ����
		MINUTES_DIGIT,
		TIME_DIGIT,
		TIME_DIGIT
	};

	D3DXVECTOR2 aSize[E_Number::NUMBER_MAX] =
	{// �����̃T�C�Y
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber,
		SIZE_NORMAL_NUM * m_fScaleNumber
	};

	D3DXVECTOR3 posBase = GetPosition();

	// �������A�������Đݒ�
	for (int i = 0; i < E_Number::NUMBER_MAX; i++)
	{
		if (m_aNumber[i] == nullptr)
			continue;

		// �Q�Ƃ���T�C�Y�̔ԍ�
		int nIdx = i;

		if (nIdx > 0)
			nIdx--;	// 0�ԖڂłȂ���ΑO��̃T�C�Y���Q�Ƃ���

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
void CScore::SetColor(E_Number number, D3DXCOLOR col)
{
	if (number < 0 || number > E_Number::NUMBER_MAX)
		return;

	if (number == E_Number::NUMBER_MAX)
	{// �S�����̐F�ݒ�
		for (auto it : m_aNumber)	// ����
			it->SetColor(col);
	}
	else
	{// �e�����̐F�ݒ�
		m_aNumber[number]->SetColor(col);
	}
}

//=====================================================
// �F�̎擾
//=====================================================
D3DXCOLOR CScore::GetColor(E_Number number)
{
	if (number < 0 || number >= E_Number::NUMBER_MAX)
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	return m_aNumber[number]->GetColor();
}

//=====================================================
// �`�揈��
//=====================================================
void CScore::Draw()
{

}