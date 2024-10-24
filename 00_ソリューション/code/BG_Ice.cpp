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
#include "iceManager.h"

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