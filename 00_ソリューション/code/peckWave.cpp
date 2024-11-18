//*****************************************************
//
// �����g��̏���[peckWave.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "peckWave.h"
#include "manager.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
namespace polygon
{
const string PATH_TEX = "data\\TEXTURE\\UI\\peckWave.png";	// �e�N�X�`���p�X
const float SIZE_INIT = 0.0f;								// �����̃T�C�Y
const float SIZE_DEST = 50.0f;								// �ڕW�̃T�C�Y
const float SIZE_DIFF = SIZE_DEST - SIZE_INIT;				// �����̃T�C�Y����
const float TIME_SCALING = 1.0f;							// �X�P�[�����O�ɂ����鎞��
const float SPEED_ROTATE = 0.05f;							// ��]���x
}
}

//====================================================
// �R���X�g���N�^
//====================================================
CPeckWave::CPeckWave(int nPriority) : CPolygon3D(nPriority), m_fTimer(0.0f)
{

}

//====================================================
// �f�X�g���N�^
//====================================================
CPeckWave::~CPeckWave()
{

}

//====================================================
// ��������
//====================================================
CPeckWave *CPeckWave::Create(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	CPeckWave *pPeckWave = nullptr;

	pPeckWave = new CPeckWave;

	if (pPeckWave != nullptr)
	{
		pPeckWave->Init();
		pPeckWave->SetPosition(pos);
		pPeckWave->SetColor(col);
		pPeckWave->SetVtx();
	}

	return pPeckWave;
}

//====================================================
// ����������
//====================================================
HRESULT CPeckWave::Init(void)
{
	CPolygon3D::Init();

	SetSize(polygon::SIZE_INIT, polygon::SIZE_INIT);

	int nIdxTexture = Texture::GetIdx(&polygon::PATH_TEX[0]);
	SetIdxTexture(nIdxTexture);

	EnableZtest(true);

	return S_OK;
}

//====================================================
// �I������
//====================================================
void CPeckWave::Uninit(void)
{
	CPolygon3D::Uninit();
}

//====================================================
// �X�V����
//====================================================
void CPeckWave::Update(void)
{
	CPolygon3D::Update();

	if (polygon::TIME_SCALING < m_fTimer)
	{// ���Ԃ��z������I��
		Uninit();
		return;
	}

	//-----------------------------
	// �X�P�[�����O
	//-----------------------------
	m_fTimer += CManager::GetDeltaTime();

	float fTime = m_fTimer / polygon::TIME_SCALING;	// �^�C�}�[�̊������C�[�W���O
	float fRate = easing::EaseOutExpo(fTime);
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	float fSize = polygon::SIZE_INIT + polygon::SIZE_DIFF * fRate;

	SetSize(fSize,fSize);

	//-----------------------------
	// ��]
	//-----------------------------
	Rotate(D3DXVECTOR3(0.0f, polygon::SPEED_ROTATE, 0.0f));
}

//====================================================
// �`�揈��
//====================================================
void CPeckWave::Draw(void)
{
	CPolygon3D::Draw();
}