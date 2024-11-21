//*****************************************************
//
// �}���`���[�h�̃��U���g�X�e�[�g[npcpenguinstate_resultmulti.cpp]
// Author:�Ό��D�n
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "npcpenguinstate_resultmulti.h"
#include "collision.h"
#include "debugproc.h"
#include "motion.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionPenguin.txt";	// �{�f�B�̃p�X
	const float SCALE_BODY = 1.8f;	// �̂̃X�P�[��
	
	const float FACT_DECREASE_MOVE = 0.9f;	// �ړ��ʂ̌����W��
	const float ADD_MOVE = 4.0f;	// �ړ��̒ǉ���
	
	const float RADIUS_COLLISION = 200.0f;	// ���̔���̔��a
}

//*****************************************************
// ���҃o���U�C�X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CNPCPenguinState_BANZAI::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_NEUTRAL);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CNPCPenguinState_BANZAI::Uninit(void)
{
	delete this;
}

//=====================================================
// �X�V����
//=====================================================
void CNPCPenguinState_BANZAI::Update(CNPCPenguin* pPenguin)
{
	
}

//*****************************************************
// ���҃W�����v�X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CNPCPenguinState_Jump::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_WALK);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CNPCPenguinState_Jump::Uninit(void)
{
	delete this;
}

//=====================================================
// �X�V����
//=====================================================
void CNPCPenguinState_Jump::Update(CNPCPenguin* pPenguin)
{
	
}

//*****************************************************
// �s�ғ����X�e�[�g
//*****************************************************
//=====================================================
// ����������
//=====================================================
HRESULT CNPCPenguinState_Flee::Init(CNPCPenguin* pPenguin)
{
	pPenguin->SetMotion(CNPCPenguin::MOTION::MOTION_FALL);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CNPCPenguinState_Flee::Uninit(void)
{
	delete this;
}

//=====================================================
// �X�V����
//=====================================================
void CNPCPenguinState_Flee::Update(CNPCPenguin* pPenguin)
{
	
}
