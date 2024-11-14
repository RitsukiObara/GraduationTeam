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
#include "particle.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionAlbatross.txt";	// �A�z�E�h���̃p�X

	const float HEIGHT_APPER = 400.0f;	// �o�����̍���
	const float WIDTH_APPER = 340.0f;	// �o�����̉��̂���
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
CAlbatross* CAlbatross::Create(CIceManager::E_Stream dir)
{
	CAlbatross* pAlbatross = nullptr;

	pAlbatross = new CAlbatross;

	if (pAlbatross != nullptr)
	{
		CIceManager::E_Stream OceanFlow = CIceManager::GetInstance()->GetDirStreamNext();

		pAlbatross->Init();

		pAlbatross->Stream(OceanFlow);
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

	// �p���N���X�̏�����
	CMotion::Init();

	return S_OK;
}

//=====================================================
// �A�z�E�h���̌��������߂鏈��
//=====================================================
void CAlbatross::Stream(CIceManager::E_Stream dir)
{
	D3DXVECTOR3 rot = CAlbatross::GetRotation();

	switch (dir)
	{
	case CIceManager::STREAM_UP:	// ��

		rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

		break;
	case CIceManager::STREAM_RIGHT:	// �E

		rot = D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f);

		break;
	case CIceManager::STREAM_DOWN:	// ��

		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		break;
	case CIceManager::STREAM_LEFT:	// ��

		rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

		break;
	default:

		assert(false);
		break;
	}

	CAlbatross::SetRotation(rot);
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
	D3DXVECTOR3 pos = CAlbatross::GetPosition();
	D3DXVECTOR3 rot = CAlbatross::GetRotation();

	m_Move = D3DXVECTOR3(sinf(rot.y + D3DX_PI), 0.5f, cosf(rot.y + D3DX_PI));

	pos += m_Move;

	CAlbatross::SetPosition(pos);

	//// ���[�V�����̊Ǘ�
	//ManageMotion();

	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	pDebugProc->Print("\n�A�z�E�h��==========================");
	pDebugProc->Print("\n�ʒu[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
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
	// �p���N���X�̕`��
	CMotion::Draw();
}