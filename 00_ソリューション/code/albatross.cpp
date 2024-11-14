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
#include "ocean.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionAlbatross.txt";	// �A�z�E�h���̃p�X

	const float HEIGHT_APPER = 400.0f;	// �o�����̍���
	const float WIDTH_APPER = 340.0f;	// �o�����̉��̂���
	const float POS_X = 1750.0f;	// �A�z�E�h���̏o���ʒuX
	const float POS_Z = 1300.0f;	// �A�z�E�h���̏o���ʒuZ
	const float POS_Y = 500.0f;	// �A�z�E�h���̏o���ʒuY
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
		pAlbatross->Init();

		pAlbatross->Stream(dir);
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

	D3DXVECTOR3 pos = CAlbatross::GetPosition();

	switch (COcean::GetInstance()->GetNextDirStream())
	{
	case CIceManager::STREAM_UP:	// ��

		pos = D3DXVECTOR3(0.0f, POS_Y, -POS_Z);

		break;
	case CIceManager::STREAM_RIGHT:	// �E

		pos = D3DXVECTOR3(-POS_X, POS_Y, 0.0f);

		break;
	case CIceManager::STREAM_DOWN:	// ��

		pos = D3DXVECTOR3(0.0f, POS_Y, POS_Z);

		break;
	case CIceManager::STREAM_LEFT:	// ��

		pos = D3DXVECTOR3(POS_X, POS_Y, 0.0f);

		break;
	default:

		assert(false);
		break;
	}

	CAlbatross::SetPosition(pos);

	SetMotion(MOTION_FLY);

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
	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CAlbatross::Update(void)
{
	D3DXVECTOR3 pos = CAlbatross::GetPosition();
	D3DXVECTOR3 rot = CAlbatross::GetRotation();

	m_Move = D3DXVECTOR3(sinf(rot.y + D3DX_PI), 0.0f, cosf(rot.y + D3DX_PI));	// �����Ă�����Ɉړ�����

	pos += m_Move * 6.0f;

	CAlbatross::SetPosition(pos);

	//// ���[�V�����̊Ǘ�
	//ManageMotion();

	// �A�z�E�h���̉�ʊO����
	if (pos.x > 2000.0f || pos.x < -2000.0f ||
		pos.z > 2000.0f || pos.z < -2000.0f)
	{
		Uninit();
	}

	// ���[�V�����X�V
	CMotion::Update();

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