//*****************************************************
//
// �e�̏���[shadow.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "shadow.h"
#include "texture.h"
#include "iceManager.h"
#include "ocean.h"

//*****************************************************
// �}�N����`
//*****************************************************
namespace
{
const float SIZE_INIT = 100.0f;									// �����̃T�C�Y
const D3DXCOLOR COL_INIT = { 0.0f,0.0f,0.0f,1.0f };				// �����F
const string PATH_TEX = "data\\TEXTURE\\EFFECT\\effect000.png";	// �e�N�X�`���p�X

const float RATE_COLLIDE_ICE = 1.0f;	// �X�Ƃ̔���̊���
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************

//=====================================================
// �R���X�g���N�^
//=====================================================
CShadow::CShadow(int nPriority) : CPolygon3D(nPriority)
{

}

//=====================================================
// ��������
//=====================================================
CShadow *CShadow::Create(void)
{
	CShadow *pShadow = new CShadow;

	if (pShadow != nullptr)
		pShadow->Init();

	return pShadow;
}

//=====================================================
// ����������
//=====================================================
HRESULT CShadow::Init(void)
{
	// �p���N���X�̏�����
	CPolygon3D::Init();

	// �e�N�X�`���ݒ�
	int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
	SetIdxTexture(nIdxTexture);

	// �T�C�Y�ݒ�
	SetSize(SIZE_INIT, SIZE_INIT);
	SetVtx();

	// �O�ʂɏo��
	EnableZtest(true);

	// �F�ݒ�
	SetColor(COL_INIT);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CShadow::Uninit(void)
{
	// �p���N���X�̏I��
	CPolygon3D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CShadow::Update(void)
{
	// �p���N���X�̍X�V
	CPolygon3D::Update();
}

//=====================================================
// �X�Ƃ̔���
//=====================================================
bool CShadow::CollideIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	// ��ԋ߂��X�̎擾
	D3DXVECTOR3 pos = GetPosition();
	CIce *pIce = pIceMgr->GetNearestIce(pos);

	if (pIce == nullptr)
		return false;

	// �X�̏�ɗ����Ă��Ȃ���Ώ������U��Ԃ�
	if (!pIceMgr->IsInIce(pos, pIce, RATE_COLLIDE_ICE))
		return false;

	pos.y = pIce->GetPosition().y;

	CPolygon3D::SetPosition(pos);

	return true;
}

//=====================================================
// �C�Ƃ̔���
//=====================================================
void CShadow::CollideOcean(void)
{
	COcean *pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
		return;

	// �C�̍����ɍ��킹��
	D3DXVECTOR3 pos = GetPosition();
	float fHeight = pOcean->GetHeight(pos, nullptr);

	pos.y = fHeight;

	CPolygon3D::SetPosition(pos);
}

//=====================================================
// �ʒu�̐ݒ�
//=====================================================
void CShadow::SetPosition(D3DXVECTOR3 pos)
{
	CPolygon3D::SetPosition(pos);

	if (!CollideIce())	// �X�ɓ�����Ȃ�������C�Ƃ̔�����s��
		CollideOcean();
}

//=====================================================
// �`�揈��
//=====================================================
void CShadow::Draw(void)
{
	// �p���N���X�̕`��
	CPolygon3D::Draw();
}