//*****************************************************
//
// �X�e�[�W�I���y���M��[slectStagePenguin.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "selectStagePenguin.h"
#include "inputManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// �{�f�B�̃p�X
const float SCALE_BODY = 2.5f;	// �̂̃X�P�[��
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CSelectStagePenguin::CSelectStagePenguin(int nPriority) : CMotion(nPriority), m_pInputMgr(nullptr)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CSelectStagePenguin::~CSelectStagePenguin()
{

}

//=====================================================
// ��������
//=====================================================
CSelectStagePenguin *CSelectStagePenguin::Create(void)
{
	CSelectStagePenguin *pSelectStagePenguin = nullptr;

	pSelectStagePenguin = new CSelectStagePenguin;

	if (pSelectStagePenguin != nullptr)
	{
		pSelectStagePenguin->Init();
	}

	return pSelectStagePenguin;
}

//=====================================================
// ����������
//=====================================================
HRESULT CSelectStagePenguin::Init(void)
{
	// ���̓}�l�[�W���[����
	m_pInputMgr = CInputManager::Create();

	// �Ǎ�
	Load((char*)&PATH_BODY[0]);

	// �p���N���X�̏�����
	CMotion::Init();

	InitPose(0);

	// �X�P�[���̐ݒ�
	SetScale(SCALE_BODY);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSelectStagePenguin::Uninit(void)
{
	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CSelectStagePenguin::Update(void)
{
	// ���͏���
	Input();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// ����
//=====================================================
void CSelectStagePenguin::Input(void)
{

}

//=====================================================
// �f�o�b�O����
//=====================================================
void CSelectStagePenguin::Debug(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CSelectStagePenguin::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}