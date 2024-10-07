//*****************************************************
//
// �͎m�̏���[rikishi.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "rikishi.h"
#include "effect3D.h"
#include "MyEffekseer.h"
#include "debugproc.h"
#include "sound.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const std::string PATH_BODY = "data\\MOTION\\motionPlayer.txt";	// �{�f�B�̃p�X
const float GUARD_DEFENCE = 0.2f;	// �K�[�h���Ɏ󂯂�_���[�W����
const float RATE_PUNCHGUARD = 0.4f;	// ���莞�̃K�[�h����
const float RATE_BOTHHANDDAMAGE = 2.0f;	// ����U���̊���
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CRikishi::CRikishi(int nPriority) : m_fragMotion()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CRikishi::~CRikishi()
{

}

//=====================================================
// ��������
//=====================================================
CRikishi *CRikishi::Create(void)
{
	CRikishi *pRikishi = nullptr;

	return pRikishi;
}

//=====================================================
// ����������
//=====================================================
HRESULT CRikishi::Init(void)
{
	// �Ǎ�
	Load((char*)&PATH_BODY[0]);

	// �p���N���X�̏�����
	CMotion::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CRikishi::Uninit(void)
{
	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CRikishi::Update(void)
{
	// �p���N���X�̍X�V
	CMotion::Update();

	ManageMotion();

#ifdef  _DEBUG
	for (int i = 0; i < E_Attack::ATTACK_MAX; i++)
	{
		CDebugProc::GetInstance()->Print("\n�U�����[�V�����t���O[%d]", m_fragMotion.bAttack[i]);
	}
	CDebugProc::GetInstance()->Print("\n�K�[�h���[�V�����t���O[%d]", m_fragMotion.bGuard);
	CDebugProc::GetInstance()->Print("\n���[�V����[%d]", GetMotion());
#endif //  _DEBUG
}

//=====================================================
// �`�揈��
//=====================================================
void CRikishi::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}

//=====================================================
// �U��
//=====================================================
void CRikishi::Attack(E_Attack typeAttack)
{
	if (typeAttack == E_Attack::ATTACK_BOTHHAND)
	{
		m_fragMotion.bAttack[E_Attack::ATTACK_LEFT] = false;
		m_fragMotion.bAttack[E_Attack::ATTACK_RIGHT] = false;
	}

	m_fragMotion.bAttack[typeAttack] = true;
	m_fragMotion.bGuard = false;
}

//=====================================================
// �K�[�h
//=====================================================
void CRikishi::Guard(void)
{
	m_fragMotion.bGuard = true;
}

//=====================================================
// ���[�V�����Ǘ�
//=====================================================
void CRikishi::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (m_fragMotion.bGuard)
	{// �K�[�h���[�V����
		if (nMotion != E_Motion::MOTION_GUARD)
			SetMotion(E_Motion::MOTION_GUARD);
	}
	else if (m_fragMotion.bAttack[E_Attack::ATTACK_RIGHT])
	{// �E�艣��
		if (nMotion != E_Motion::MOTION_ATTACK_RIGHT)
		{
			SetMotion(E_Motion::MOTION_ATTACK_RIGHT);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bAttack[E_Attack::ATTACK_RIGHT] = false;
				SetMotion(E_Motion::MOTION_GUARD);
			}
		}
	}
	else if (m_fragMotion.bAttack[E_Attack::ATTACK_LEFT])
	{// ���艣��
		if (nMotion != E_Motion::MOTION_ATTACK_LEFT)
		{
			SetMotion(E_Motion::MOTION_ATTACK_LEFT);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bAttack[E_Attack::ATTACK_LEFT] = false;
				SetMotion(E_Motion::MOTION_GUARD);
			}
		}
	}
	else if (m_fragMotion.bAttack[E_Attack::ATTACK_BOTHHAND])
	{// ���艣��
		if (nMotion != E_Motion::MOTION_ATTACK_BOTHHAND)
		{
			SetMotion(E_Motion::MOTION_ATTACK_BOTHHAND);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bAttack[E_Attack::ATTACK_BOTHHAND] = false;
				SetMotion(E_Motion::MOTION_GUARD);
			}
		}
	}
}

//=====================================================
// �C�x���g�̊Ǘ�
//=====================================================
void CRikishi::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	D3DXVECTOR3 offset = pEventInfo->offset;
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxPart = GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

	universal::SetOffSet(&mtxParent, mtxPart, offset);

	D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };

	if (nMotion == E_Motion::MOTION_ATTACK_RIGHT)
	{
		HitAttack(E_Attack::ATTACK_RIGHT);
	}

	if (nMotion == E_Motion::MOTION_ATTACK_LEFT)
	{
		HitAttack(E_Attack::ATTACK_LEFT);
	}

	if (nMotion == E_Motion::MOTION_ATTACK_BOTHHAND)
	{
		HitAttack(E_Attack::ATTACK_BOTHHAND);
	}
}

//=====================================================
// �q�b�g����
//=====================================================
float CRikishi::Hit(E_Attack typeAttack, float fDamage)
{
	CSound* pSound = CSound::GetInstance();	// �T�E���h���
	assert(pSound != nullptr);

	float fKnockBack = 0.0f;

	int nMotion = GetMotion();

	fKnockBack = fDamage;

	// ����U���̈З͂𑝂₷
	if (typeAttack == E_Attack::ATTACK_BOTHHAND)
	{
		fKnockBack *= RATE_BOTHHANDDAMAGE;

		if (nMotion != E_Motion::MOTION_GUARD)
		{// �K�[�h���Ă��Ȃ��Ƃ��ɖ����������G�t�F�N�g����
			// �K�[�h�q�b�g�G�t�F�N�g�̔���
			D3DXVECTOR3 posEffect = GetMtxPos(E_Parts::IDX_BODY);

			D3DXVECTOR3 rotEffect = GetRotation();
			MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_HIT02, posEffect, rotEffect);

			// �T�E���h�̍Đ�
			pSound->Play(CSound::LABEL_SE_ATTACK02);
		}
	}

	// �m�b�N�o�b�N�ʂ̔���
	if (typeAttack == CRikishi::E_Attack::ATTACK_RIGHT &&
		nMotion == E_Motion::MOTION_ATTACK_RIGHT)
	{// �E�ŉ���ꂽ���ǉE���肵�Ă�����K�[�h
		fKnockBack *= RATE_PUNCHGUARD;

		// �K�[�h�q�b�g�G�t�F�N�g�̔���
		D3DXVECTOR3 posEffect = GetMtxPos(E_Parts::IDX_BODY);

		D3DXVECTOR3 rotEffect = GetRotation();
		MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_HIT01, posEffect, rotEffect);

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_ATTACK01);
	}
	else if (typeAttack == CRikishi::E_Attack::ATTACK_LEFT &&
		nMotion == E_Motion::MOTION_ATTACK_LEFT)
	{// ���ŉ���ꂽ���Ǎ����肵�Ă�����K�[�h
		fKnockBack *= RATE_PUNCHGUARD;

		// �K�[�h�q�b�g�G�t�F�N�g�̔���
		D3DXVECTOR3 posEffect = GetMtxPos(E_Parts::IDX_BODY);

		D3DXVECTOR3 rotEffect = GetRotation();
		MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_HIT01, posEffect, rotEffect);

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_ATTACK01);
	}
	else if (nMotion == E_Motion::MOTION_GUARD)
	{// �h�䎞�͊����Ō��炷
		fKnockBack *= GUARD_DEFENCE;

		// �K�[�h�q�b�g�G�t�F�N�g�̔���
		D3DXVECTOR3 posEffect = GetMtxPos(E_Parts::IDX_BODY);

		D3DXVECTOR3 rotEffect = GetRotation();
		MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_HIT01, posEffect, rotEffect);

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_Guard01);
	}
	else
	{
		// �ʏ�q�b�g�G�t�F�N�g�̔���
		D3DXVECTOR3 posEffect = GetMtxPos(E_Parts::IDX_BODY);
		posEffect.x += universal::RandRange(50, -25);
		posEffect.y += universal::RandRange(50, -25);
		posEffect.z += universal::RandRange(50, -25);

		MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_HIT00, posEffect);

		// �T�E���h�̍Đ�
		pSound->Play(CSound::LABEL_SE_ATTACK01);
	}

	return fKnockBack;	// �G�ƃv���C���[�Ńm�b�N�o�b�N�������Ⴄ�̂Œl��Ԃ�
}