//*****************************************************
//
// �w�i�X�̏���[BG_Ice.cpp]
// Author:����F�M
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "BG_Ice.h"
#include "ice.h"
#include "ocean.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define RESULT_WIDTH	(0.18f)	// ���ڂ̕�
#define RESULT_HEIGHT	(0.05f)	// ���ڂ̍���
#define MOVE_FACT	(0.15f)	// �ړ����x
#define LINE_ARRIVAL	(0.05f)	// ���������Ƃ���邵�����l
#define LINE_UNINIT	(3.0f)	// �I������܂ł̂������l

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const char* MODEL[CBgIce::TYPE_MAX] = { "data\\MODEL\\block\\Drift_ice.x","data\\MODEL\\block\\Drift_ice_small.x" };
	const float GRAVITY_SPEED = 0.1f;	// �w�i�X������ł������x
	const float MAX_HEIGHT = -300.0f;	// �X�����ލ���
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************

//====================================================
// �R���X�g���N�^
//====================================================
CBgIce::CBgIce()
{
	m_type = TYPE_BIG;
	m_state = STATE_FLOW;
}

//====================================================
// �f�X�g���N�^
//====================================================
CBgIce::~CBgIce()
{

}

//====================================================
// ��������
//====================================================
CBgIce* CBgIce::Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,TYPE type)
{
	CBgIce* pBgIce = new CBgIce;

	pBgIce->SetPosition(pos);
	pBgIce->SetRotation(rot);

	pBgIce->m_type = type;

	pBgIce->Init();

	return pBgIce;
}

//====================================================
// ����������
//====================================================
HRESULT CBgIce::Init(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return S_OK;

	// �C���̃x�N�g���擾
	CIceManager::E_Stream dir = pIceManager->GetDirStream();

	//���݂̊C�����擾
	m_streamOld = dir;

	CObjectX::Init();

	CObjectX::BindModel(CModel::Load(MODEL[m_type]));

	return S_OK;
}

//====================================================
// �I������
//====================================================
void CBgIce::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================
// �X�V����
//====================================================
void CBgIce::Update(void)
{
	//�ړ�����
	Move();

	CObjectX::Update();
}

//====================================================
// �`�揈��
//====================================================
void CBgIce::Draw(void)
{
	CObjectX::Draw();
}

//=====================================================
// �Ǎ�����
//=====================================================
void CBgIce::Load(char* pPath)
{
	//�ϐ��錾
	char cTemp[MAX_STRING];

	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	TYPE type;

	//�t�@�C������ǂݍ���
	FILE* pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ
		while (true)
		{
			//�����ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);
			if (strcmp(cTemp, "SET") == 0)
					{//�L�[�X�^�[�g
						while (strcmp(cTemp, "END_SET") != 0)
						{//�I���܂ŃL�[�ݒ�

									(void)fscanf(pFile, "%s", &cTemp[0]);

									if (strcmp(cTemp, "POS") == 0)
									{//�ʒu�擾

										(void)fscanf(pFile, "%s", &cTemp[0]);

										for (int nCntPos = 0; nCntPos < 3; nCntPos++)
										{
											(void)fscanf(pFile, "%f", &pos[nCntPos]);
										}
									}

									if (strcmp(cTemp, "ROT") == 0)
									{//�����擾

										(void)fscanf(pFile, "%s", &cTemp[0]);

										for (int nCntRot = 0; nCntRot < 3; nCntRot++)
										{
											(void)fscanf(pFile, "%f", &rot[nCntRot]);
										}
									}

									if (strcmp(cTemp, "TYPE") == 0)
									{//�^�C�v�擾
										(void)fscanf(pFile, "%s", &cTemp[0]);

										(void)fscanf(pFile, "%d", &type);
									}
								}
						CBgIce::Create(pos, rot, type);
					}

					if (strcmp(cTemp, "END_SCRIPT") == 0)
					{
						break;
					}
		}//while
	}//file
	else
	{
		assert(("���[�V�����f�[�^�ǂݍ��݂Ɏ��s", false));
	}

		fclose(pFile);
}

//====================================================
// �ړ�����
//====================================================
void CBgIce::Move(void)
{
	CIceManager* pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// �C���̃x�N�g���擾
	CIceManager::E_Stream dir = pIceManager->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	if (pIceManager->GetDirStream() != m_streamOld)
	{
		m_state = STATE_SINK;
	}

	//���݂̊C�����擾
	m_streamOld = dir;

	// ����鑬�x�ɐ��K�����Ĉʒu�����Z
	float fSpeedFlow = pIceManager->GetOceanLevel();

	switch (m_state)
	{
	case STATE_FLOW:
		D3DXVec3Normalize(&vecStream, &vecStream);
		/*vecStream *= fSpeedFlow;*/
		AddPosition(vecStream);

		//�C�ʂɉ��킹��
		Flow();

		break;

	case STATE_SINK:

		D3DXVECTOR3 pos = GetPosition();

		fgravity_speed += GRAVITY_SPEED;

		pos.y -= fgravity_speed;

		if (pos.y <= MAX_HEIGHT)
		{
			Uninit();
		}

		SetPosition(pos);

		break;
	}
}

//====================================================
// �C�ʂɉ��킹�鏈��
//====================================================
void CBgIce::Flow(void)
{
	COcean* pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
	{
		return;
	}

	// �C�ƈꏏ�ɕX�𓮂�������
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos.y = pOcean->GetHeight(pos, &move);

	SetPosition(pos);
}