//*****************************************************
//
// �}���`�p�̍d���X�̏���[iceHardMulti.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "iceHardMulti.h"
#include "player.h"
#include "iceManager.h"
#include "manager.h"
#include "fan3D.h"
#include "meshcylinder.h"
#include "MyEffekseer.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float TIME_SINK = 10.0f;						// ���ނ܂ł̎���
const float SPEED_REPAIR = 0.3f;					// �񕜂̑��x(1.0f�Ői�s�Ɠ������x)
const D3DXCOLOR COL_INIT = { 1.0f,1.0f,1.0f,1.0f };	// �����̐F
const float SIZE_COLLIDE = Grid::SIZE * 0.5f;		// ����̃T�C�Y

const float POW_VIB_SINK = 0.7f;	// ���݂����Ȏ��̐U������
const int TIME_VIB_SINK = 10;		// ���݂����Ȏ��̐U������
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************

//=====================================================
// �R���X�g���N�^
//=====================================================
CIceHardMulti::CIceHardMulti(int nPriority) : CIceHard(nPriority), m_fTimerSink(0.0f)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CIceHardMulti::~CIceHardMulti()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CIceHardMulti::Init(void)
{
	CIceHard::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CIceHardMulti::Uninit(void)
{
	CIceHard::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CIceHardMulti::Update(void)
{
	// �p���N���X�̍X�V
	CIceHard::Update();

	// �v���C���[������Ă锻��
	IsOnPlayer();

	// ��Ԃ��Ƃ̍X�V
	UpdateState();
}

//=====================================================
// �v���C���[������Ă锻��
//=====================================================
void CIceHardMulti::IsOnPlayer(void)
{
	// �v���C���[�̎擾
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	for (CPlayer *pPlayer : apPlayer)
	{
		if (pPlayer == nullptr)
			continue;

		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

		if (universal::DistCmpFlat(pos, posPlayer, SIZE_COLLIDE, nullptr))
		{// ��苗���ȓ��ɂ�����J�E���^�[�����Z
			m_fTimerSink += CManager::GetDeltaTime();

			// �R���g���[���[��U��������
			float fRate = m_fTimerSink / TIME_SINK;
			pPlayer->VibJoypad(POW_VIB_SINK * fRate, TIME_VIB_SINK);

			break;
		}
		else
			m_fTimerSink -= CManager::GetDeltaTime() * SPEED_REPAIR;
	}
}

//=====================================================
// ��Ԃ��Ƃ̍X�V
//=====================================================
void CIceHardMulti::UpdateState(void)
{
	if (m_fTimerSink > TIME_SINK)
	{// ��莞�ԏ���Ă���폜
		Uninit();
		MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_ICEBREAK, GetPosition());
		return;
	}

	// �l�̕␳
	universal::LimitValuefloat(&m_fTimerSink, TIME_SINK, 0.0f);

	// �F�̐ݒ�
	D3DXCOLOR col = COL_INIT;

	// �����̌v�Z
	float fRate = m_fTimerSink / TIME_SINK;

	// �ԈȊO�̐F�������������Ă���
	col.g -= fRate;
	col.b -= fRate;

	// �e���b�V���ɐF��ݒ�
	CMeshCylinder *pCylinder = GetMeshCyliner();
	CFan3D *pFan = GetFan3D();

	if (pCylinder == nullptr || pFan == nullptr)
		return;

	pCylinder->SetCol(col);
	pFan->SetCol(col);
}

//=====================================================
// �`�揈��
//=====================================================
void CIceHardMulti::Draw(void)
{
	CIceHard::Draw();
}