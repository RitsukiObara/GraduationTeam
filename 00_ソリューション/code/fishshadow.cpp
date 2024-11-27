//*****************************************************
//
// ���e�̏���[fishshadow.cpp]
// Author:��؈�^
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "fishshadow.h"
#include "texture.h"

//*****************************************************
// �}�N����`
//*****************************************************

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const char* SEALS_LOGO_PATH = "data\\TEXTURE\\UI\\stage_clear.png";	// �N���A���S�̃p�X
	const char* BEARS_LOGO_PATH = "data\\TEXTURE\\UI\\gameover.png";	// ���s���S�̃p�X
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CFishShadow* CFishShadow::m_pFishShadow = nullptr;	// ���g�̃|�C���^

//====================================================
// �R���X�g���N�^
//====================================================
CFishShadow::CFishShadow()
{
	m_FishShadow = FISHSHADOW_SEALS;
}

//====================================================
// �f�X�g���N�^
//====================================================
CFishShadow::~CFishShadow()
{

}

//====================================================
// ��������
//====================================================
CFishShadow* CFishShadow::Create(void)
{
	if (m_pFishShadow == nullptr)
	{
		m_pFishShadow = new CFishShadow;

		if (m_pFishShadow != nullptr)
		{
			m_pFishShadow->Init();
		}
	}

	return m_pFishShadow;
}

//====================================================
// ����������
//====================================================
HRESULT CFishShadow::Init(void)
{
	//�N���A�̎�
	if (m_FishShadow == FISHSHADOW_SEALS)
	{
		// �N���A���S�̐���
		m_apFishShadow[FISHSHADOW_SEALS] = CObject3D::Create();

		if (m_apFishShadow[FISHSHADOW_SEALS] != nullptr)
		{
			m_apFishShadow[FISHSHADOW_SEALS]->SetScale(D3DXVECTOR3(300.0f, 100.0f, 100.0f));
			m_apFishShadow[FISHSHADOW_SEALS]->SetPosition(D3DXVECTOR3(640.0f, 200.0f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(SEALS_LOGO_PATH);
		}
	}

	//���s�̎�
	else if (m_FishShadow == FISHSHADOW_BEARS)
	{
		// ���s���S�̐���
		m_apFishShadow[FISHSHADOW_BEARS] = CObject3D::Create();

		if (m_apFishShadow[FISHSHADOW_BEARS] != nullptr)
		{
			m_apFishShadow[FISHSHADOW_BEARS]->SetScale(D3DXVECTOR3(300.0f, 100.0f, 100.0f));
			m_apFishShadow[FISHSHADOW_BEARS]->SetPosition(D3DXVECTOR3(640.0f, 200.0f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(BEARS_LOGO_PATH);
		}
	}

	//m_aPosDest[0].x = RESULT_WIDTH;

	EnableNotStop(true);

	return S_OK;
}

//====================================================
// �I������
//====================================================
void CFishShadow::Uninit(void)
{
	for (int nCnt = 0; nCnt < FISHSHADOW_MAX; nCnt++)
	{// ���j���[���ڂ̔j��
		if (m_apFishShadow[nCnt] != nullptr)
		{
			m_apFishShadow[nCnt]->Uninit();

			m_apFishShadow[nCnt] = nullptr;
		}
	}

	m_pFishShadow = nullptr;

	Release();
}

//====================================================
// �X�V����
//====================================================
void CFishShadow::Update(void)
{
	// ��ԊǗ�
	ResultState();
}

//====================================================
// ���e��ԊǗ�
//====================================================
void CFishShadow::ResultState(void)
{
	// �A�U���V�̋��e����
	FishShadowSeals();

	// ���낭�܂̋��e����
	FishShadowBears();
}

//====================================================
// �A�U���V�̋��e����
//====================================================
void CFishShadow::FishShadowSeals(void)
{
	// �N���A�̎�
	if (m_FishShadow == FISHSHADOW_SEALS)
	{
		if (m_apFishShadow[FISHSHADOW_SEALS] != nullptr)
		{

		}
	}
}

//====================================================
// ���낭�܂̋��e����
//====================================================
void CFishShadow::FishShadowBears(void)
{
	// ���s�̎�
	if (m_FishShadow == FISHSHADOW_BEARS)
	{
		if (m_apFishShadow[FISHSHADOW_BEARS] != nullptr)
		{

		}
	}
}

//====================================================
// �`�揈��
//====================================================
void CFishShadow::Draw(void)
{

}