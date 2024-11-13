//*****************************************************
//
// �A�z�E�h���̏���[albatross.cpp]
// Author:��؈�^
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "albatross.h"
#include "iceManager.h"
#include "particle.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionAlbatross.txt";	// �A�z�E�h���̃p�X

	const float HEIGHT_APPER = -400.0f;	// �o�����̍���
	const float WIDTH_APPER = -340.0f;	// �o�����̉��̂���
}

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CAlbatross::CAlbatross(int nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CAlbatross::~CAlbatross()
{

}

//=====================================================
// ��������
//=====================================================
CAlbatross* CAlbatross::Create(void)
{
	CAlbatross* pAlbatross = nullptr;

	pAlbatross = new CAlbatross;

	if (pAlbatross != nullptr)
	{
		pAlbatross->Init();
	}

	return pAlbatross;
}

//=====================================================
// ����������
//=====================================================
HRESULT CAlbatross::Init(void)
{
	// �Ǎ�
	Load((char*)&PATH_BODY[0]);

	// ���[�V�����������ݒ�
	SetMotion(E_Motion::MOTION_FLY);

	// �|�[�Y������
	InitPose(0);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CAlbatross::Uninit(void)
{

}

//=====================================================
// �X�V����
//=====================================================
void CAlbatross::Update(void)
{
	CIceManager* pIce = CIceManager::GetInstance();

	m_DirStream = pIce->GetDirStream();	// ���݂̊C���̕������擾
	m_DirStreamNext = pIce->GetDirStreamNext();	// ���̊C���̕������擾

	if (m_DirStream != m_DirStreamNext)	// ���݂Ǝ��̊C�����Ⴄ�Ƃ��A�z�E�h�������̊C���̌����ɔ�΂�
	{
		if (m_DirStreamNext == CIceManager::STREAM_UP)	// ��
		{
			m_FlyDirection = FLYDIRECTION_UP;
		}

		if (m_DirStreamNext == CIceManager::STREAM_RIGHT)	// �E
		{
			m_FlyDirection = FLYDIRECTION_RIGHT;
		}

		if (m_DirStreamNext == CIceManager::STREAM_DOWN)	// ��
		{
			m_FlyDirection = FLYDIRECTION_DOWN;
		}

		if (m_DirStreamNext == CIceManager::STREAM_LEFT)	// ��
		{
			m_FlyDirection = FLYDIRECTION_LEFT;
		}
	}

	if (m_FlyDirection == FLYDIRECTION_UP)
	{

	}

	if (m_FlyDirection == FLYDIRECTION_RIGHT)
	{

	}

	if (m_FlyDirection == FLYDIRECTION_DOWN)
	{

	}

	if (m_FlyDirection == FLYDIRECTION_LEFT)
	{

	}

	// ���[�V�����̊Ǘ�
	ManageMotion();
}

//=====================================================
// ���[�V�����̊Ǘ�
//=====================================================
void CAlbatross::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (nMotion == E_Motion::MOTION_FLY)
	{// �W�����v�J�n���[�V����
		if (bFinish)	// �I��莟��؋󃂁[�V�����ֈڍs
			SetMotion(E_Motion::MOTION_FLY);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CAlbatross::Draw(void)
{

}