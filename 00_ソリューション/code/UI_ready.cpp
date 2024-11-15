//*****************************************************
//
// �^�C���̏���[timer.cpp]
// Author:�X��x��
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "UI_ready.h"
#include "UI.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const int TIME_DIGIT = 1;	// ���ꂼ��̌���
	const int TIME_SECOND = 3;	// �b��
	const int STATE_COUNT_MAX = 60;	// �X�e�[�g�̃J�E���g�ő�l
	const float DIST_NUMBER = 0.03f;	// �����Ԃ̋���
	const D3DXVECTOR2 SIZE_NORMAL_NUM = { 0.05f, 0.09f };	// �ʏ퐔���̃T�C�Y
	const D3DXVECTOR2 SIZE_MINI_NUM = { 0.014f, 0.028f };	// �~�j�����̃T�C�Y
	const float SIZE_NUM = 0.5;	// Go�����̃T�C�Y
	const D3DXVECTOR3 POS_INITIAL = { 0.6f,0.5f,0.0f };	// �����ʒu
	const D3DXVECTOR3 POS_GO = { 0.5f,0.5f,0.0f };	// �����ʒu
	const string PATH_TEX_OK = "data\\TEXTURE\\UI\\ready.png";	// �R�����̃e�N�X�`���p�X
	const D3DXCOLOR NORMAL_COL = { 1.0f,1.0f,1.0f,1.0f };		//��F
	const int FRAME_CNT = 60;		// �t���[���b��
	const int SECOND_ELAPSED = 1;		// �b���o��
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CUIready::CUIready()
{
	m_nSecond = 0;
	m_fScaleNumber = 0;
	m_bStop = false;
	m_nFrame = 0;
	m_state = STATE_NUMBER;
	m_StateCnt = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CUIready::~CUIready()
{

}

//=====================================================
// ��������
//=====================================================
CUIready* CUIready::Create(void)
{
	CUIready* pUI_ready = nullptr;

	pUI_ready = new CUIready;

	if (pUI_ready != nullptr)
	{// ������
		pUI_ready->Init();
	}

	return pUI_ready;
}

//=====================================================
// ���ԕۑ�����
//=====================================================
void CUIready::SaveSecond(int fSecond)
{
	//// �t�@�C�����J��
	//std::ofstream file("data\\TEMP\\time.bin", std::ios_base::binary);	// �t�@�C���X�g���[��
	//if (file.fail())
	//{ // �t�@�C�����J���Ȃ������ꍇ

	//	// �G���[���b�Z�[�W�{�b�N�X
	//	MessageBox(nullptr, "���Ԃ̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
	//	return;
	//}

	//// �����̎��Ԃ������o��
	//file.write((char*)&fSecond, sizeof(float));

	//// �t�@�C�������
	//file.close();
}

////=====================================================
//// ���ԓǍ�����
////=====================================================
//int CUIready::LoadSecond(void)
//{
//	//// �t�@�C�����J��
//	//std::ifstream file("data\\TEMP\\time.bin", std::ios_base::binary);	// �t�@�C���X�g���[��
//	//if (file.fail())
//	//{ // �t�@�C�����J���Ȃ������ꍇ
//
//	//	// �G���[���b�Z�[�W�{�b�N�X
//	//	MessageBox(nullptr, "���Ԃ̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
//	//	return 0;
//	//}
//
//	//// �����̎��Ԃ�ǂݍ���
//	//int nTime = 0;
//	//file.read((char*)&nTime, sizeof(float));
//
//	//// �t�@�C�������
//	//file.close();
//
//	//// �ǂݍ��񂾎��Ԃ�Ԃ�
//	//return nTime;
//}

//=====================================================
// ����������
//=====================================================
HRESULT CUIready::Init(void)
{
	m_nSecond = 0;	// �b�̏�����
	m_fScaleNumber = 1.0f;	// �����X�P�[���ݒ�
	m_bStop = false;	// �^�C�}�[��~�̃t���O
	m_nFrame = 0;		//�t���[����������
	m_StateCnt = 0;		//��ԑJ�ڃJ�E���g�̏�����

	// �����ʒu�̐ݒ�
	SetPosition(POS_INITIAL);

	SetSecond(TIME_SECOND);

	// �����̔z��̃��T�C�Y
	m_aNumber.resize(E_Number::NUMBER_MAX);

	int aDigit[E_Number::NUMBER_MAX] =
	{// ����
		TIME_DIGIT,
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
void CUIready::Uninit(void)
{
	for (auto it : m_aNumber)
	{
		it->Uninit();
	}

	m_aNumber.clear();

	if (m_Go != nullptr)
	{
		m_Go->Uninit();
	}

	CGameObject::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CUIready::Update(void)
{
	// �b�������炤
	m_nSecond = GetSecond();

	switch (m_state)
	{
	case STATE_NUMBER:

		if (m_nSecond <= 0)
		{
			m_state = STATE_GO;

			for (int i = 0; i < E_Number::NUMBER_MAX; i++)
			{
				m_aNumber[i]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			}

			// GOUI�𐶐�
			m_Go = CUI::Create();

			m_Go->SetPosition(POS_GO);
			int nIdxTexture = Texture::GetIdx(&PATH_TEX_OK[0]);
			m_Go->SetIdxTexture(nIdxTexture);
			m_Go->SetSize(SIZE_NUM, SIZE_NUM);

			m_Go->SetVtx();
		}

		break;

	case STATE_GO:

		if (m_Go == nullptr)
		{
			return;
		}	
		
		//�X�e�[�g�J�E���g���Z
		m_StateCnt++;

		if (m_StateCnt >= STATE_COUNT_MAX)
		{
			// �I��
			Uninit();
		}

		break;
	}

	UpdateNumber();

#ifdef _DEBUG
#if 1
	CDebugProc::GetInstance()->Print("\n���݂̕b�F[%d]", m_nSecond);
	//CDebugProc::GetInstance()->Print("\n�X�R�A�̈ʒu�F[%f,%f,%f]", pos.x, pos.y, pos.z);
#endif
#endif
}

//=====================================================
// �����̍X�V
//=====================================================
void CUIready::UpdateNumber()
{
	if (m_aNumber.empty())
		return;

	if (m_nSecond < 0)
	{
		m_nSecond = 0;
	}

	//�t���[�����Z
	m_nFrame++;

	if (m_nFrame >= FRAME_CNT)
	{
		m_nSecond = m_nSecond - SECOND_ELAPSED;

		m_nFrame = 0;
	}

	// �l�̗p��
	int aValue[E_Number::NUMBER_MAX] =
	{
#if 1

#else
		(int)((DWORD)(m_nSecond * 1000) / 60000),
		(int)((DWORD)(m_nSecond * 1000) / 1000 % 60),
#endif
	};

	for (int i = 0; i < E_Number::NUMBER_MAX; i++)
	{
		m_aNumber[i]->SetValue(m_nSecond);
	}
}

//=====================================================
// �����̃g�����X�t�H�[���ݒ�
//=====================================================
void CUIready::TransformNumber()
{
	if (m_aNumber.empty())
		return;

	int aDigit[E_Number::NUMBER_MAX] =
	{// ����
		TIME_DIGIT,
	};

	D3DXVECTOR2 aSize[E_Number::NUMBER_MAX] =
	{// �����̃T�C�Y
		SIZE_NORMAL_NUM * m_fScaleNumber,
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
void CUIready::SetPosition(D3DXVECTOR3 pos)
{
	CGameObject::SetPosition(pos);

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();
}

//=====================================================
// �����̃X�P�[���̐ݒ�
//=====================================================
void CUIready::SetScaleNumber(float fScale)
{
	m_fScaleNumber = fScale;

	// �����̃g�����X�t�H�[���̐ݒ�
	TransformNumber();
}

//=====================================================
// �F�̐ݒ�
//=====================================================
void CUIready::SetColor(E_Number number, D3DXCOLOR col)
{
	if (number < 0 || number > E_Number::NUMBER_MAX)
		return;

	if (number == E_Number::NUMBER_MAX)
	{// �S�����̐F�ݒ�
		for (auto it : m_aNumber)	// ����
			it->SetColor(col);

			m_Go->SetCol(col);
	}
	else
	{// �e�����̐F�ݒ�
		m_aNumber[number]->SetColor(col);
	}
}

//=====================================================
// �F�̎擾
//=====================================================
D3DXCOLOR CUIready::GetColor(E_Number number)
{
	if (number < 0 || number >= E_Number::NUMBER_MAX)
		return D3DXCOLOR(NORMAL_COL);

	return m_aNumber[number]->GetColor();
}

//=====================================================
// �`�揈��
//=====================================================
void CUIready::Draw()
{

}