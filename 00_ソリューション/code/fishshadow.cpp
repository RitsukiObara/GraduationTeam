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
namespace
{
	const char* FISHSHADOW_LOGO_PATH = "data\\TEXTURE\\enemy\\Fish_shadow.png";	// �N���A���S�̃p�X
	const float SIZE_INIT[CEnemy::TYPE::TYPE_MAX] = {70.0f , 200.0f};	// �����̃T�C�Y
	const D3DXCOLOR COL_INIT = { 0.0f,0.0f,0.0f,1.0f };	// �����F
}

//====================================================
// �R���X�g���N�^
//====================================================
CFishShadow::CFishShadow()
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
CFishShadow* CFishShadow::Create(CEnemy::TYPE type)
{
	CFishShadow* pFishShadow = nullptr;

	pFishShadow = new CFishShadow;

	if (pFishShadow != nullptr)
	{
		pFishShadow->Init();

		// �T�C�Y�ݒ�
		pFishShadow->SetSize(SIZE_INIT[type], SIZE_INIT[type]);
		pFishShadow->SetVtx();
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
	int nIdxTexture = Texture::GetIdx(&FISHSHADOW_LOGO_PATH[0]);
	SetIdxTexture(nIdxTexture);

	// �O�ʂɏo��
	EnableZtest(true);

	// �F�ݒ�
	SetColor(COL_INIT);

	EnableNotStop(true);

	return S_OK;
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