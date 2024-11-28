//*****************************************************
//
// ���e�̏���[fishshadow.cpp]
// Author:��؈�^�@���R����
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
namespace
{
const char* PATH_TEX = "data\\TEXTURE\\enemy\\Fish_shadow.png";	// �e�N�X�`���p�X
}

//====================================================
// �R���X�g���N�^
//====================================================
CFishShadow::CFishShadow(int nPriority) : CPolygon3D(nPriority), m_nTimerVanish(0.0f)
{

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
CFishShadow* CFishShadow::Create(int nPatern)
{
	CFishShadow* pFishShadow = nullptr;

	pFishShadow = new CFishShadow;

	if (pFishShadow != nullptr)
	{
		pFishShadow->Init();
		pFishShadow->InitSpawn(nPatern);
	}

	return pFishShadow;
}

//====================================================
// ����������
//====================================================
HRESULT CFishShadow::Init(void)
{
	// �p���N���X�̏�����
	CPolygon3D::Init();

	// �e�N�X�`���ݒ�
	int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
	SetIdxTexture(nIdxTexture);

	// �O�ʂɏo��
	EnableZtest(true);

	return S_OK;
}

//====================================================
// �X�|�[���̏�����
//====================================================
void CFishShadow::InitSpawn(int nPatern)
{

}

//====================================================
// �I������
//====================================================
void CFishShadow::Uninit(void)
{
	// �p���N���X�̏I��
	CPolygon3D::Uninit();
}

//====================================================
// �X�V����
//====================================================
void CFishShadow::Update(void)
{
	// �p���N���X�̍X�V
	CPolygon3D::Update();
}

//====================================================
// �`�揈��
//====================================================
void CFishShadow::Draw(void)
{
	// �p���N���X�̕`��
	CPolygon3D::Draw();
}