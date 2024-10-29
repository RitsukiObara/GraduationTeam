//*****************************************************
//
// �G�̏���[enemy.cpp]
// Author:����F�M
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemy.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "iceManager.h"
#include "debugproc.h"
#include "seals.h"
#include "UI_enemy.h"
#include "ocean.h"
#include "destroy_score.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float HEIGHT_ICE = 100.0f;	// �X�̍���
const float SPPED_MOVE_INIT = 5.0f;	// �����ړ����x
const float SPEED_ROTATION = 0.1f;	// ��]���x
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
std::vector<CEnemy*> CEnemy::s_vector = {};	// ���g�̃|�C���^

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CEnemy::CEnemy(int nPriority) : m_nGridV(0), m_nGridH(0),m_state(E_State::STATE_NONE), m_pIceLand(nullptr), m_bFollowIce(false),
m_move(),m_nGridVDest(0), m_nGridHDest(0), m_fSpeedMove(0.0f)
{
	s_vector.push_back(this);
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemy::~CEnemy()
{

}

//=====================================================
// ��������
//=====================================================
CEnemy* CEnemy::Create(int nType, int nGridV, int nGridH)
{
	CEnemy* pEnemy = nullptr;

	switch (nType)
	{
	case CEnemy::TYPE_SEALS:

		pEnemy = new CSeals;

		break;
	
	default:

		assert(false);

		break;
	}

	if (pEnemy != nullptr)
	{// �G����
		// �O���b�h������
		pEnemy->SetGridV(nGridV);
		pEnemy->SetGridH(nGridH);

		// ����������
		pEnemy->Init();

		CUIEnemy *pUIEnemy = CUIEnemy::GetInstance();

		if (pUIEnemy != nullptr)
			pUIEnemy->AddEnemy();
	}

	return pEnemy;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemy::Init(void)
{
	// �p���N���X�̏�����
	CMotion::Init();

	// �O���b�h�ԍ�������
	InitGridIdx();

	// ��ԏ�����
	m_state = E_State::STATE_APPER;

	// �o�����̃g�����X�t�H�[���ݒ�
	SetApperTransform();

	// �X�Ǐ]�t���O��ݒ�
	m_bFollowIce = false;

	// �ړ����x�̏����ݒ�
	m_fSpeedMove = SPPED_MOVE_INIT;

	return S_OK;
}

//=====================================================
// �O���b�h�ԍ��̏�����
//=====================================================
void CEnemy::InitGridIdx(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	if (pIceMgr->GetGridIce(&m_nGridV, &m_nGridH) != nullptr)
		return;

	pIceMgr->GetLeftDownIdx(&m_nGridV, &m_nGridH);
	m_nGridVNext = m_nGridV;
	m_nGridHNext = m_nGridH;

	D3DXVECTOR3 pos = pIceMgr->GetGridPosition(&m_nGridV, &m_nGridH);
	SetPosition(pos);
}

//=====================================================
// �I������
//=====================================================
void CEnemy::Uninit(void)
{
	CUIEnemy *pUIEnemy = CUIEnemy::GetInstance();

	if (pUIEnemy != nullptr)
		pUIEnemy->DeleteEnemy();

	for (auto itr = s_vector.begin(); itr < s_vector.end(); itr++ )
	{
		//�폜�Ώۂ���Ȃ��ꍇ
		if (*itr != this)
		{
			continue;
		}
		//Vector����폜
		s_vector.erase(itr);

		break;
	}

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemy::Update(void)
{
	CMotion::Update();

	if(m_bFollowIce)
		FollowIce();	// �X�ɒǏ]

	// ��Ԃɉ������X�V
	UpdateState updateFuncs[CEnemy::E_State::STATE_MAX] =
	{
		nullptr,
		&CEnemy::UpdateApper,
		&CEnemy::UpdateStop,
		&CEnemy::UpdateMove,
		&CEnemy::UpdateAttack,
		&CEnemy::UpdateDrift
	};

	if (updateFuncs[m_state] != nullptr)
		(this->*updateFuncs[m_state])();

	// �ړ��ʂ��ʒu�ɉ��Z
	AddPosition(m_move);

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �X�ɒǏ]
//=====================================================
void CEnemy::FollowIce(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	// �X�̍����ɍ��킹��
	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);
	
	if (pIceStand != nullptr)
		pos.y = pIceStand->GetPosition().y;	// ���������킹��
	else
		SetState(E_State::STATE_DRIFT);	// �Y����Ԃɂ���

	pos.y += HEIGHT_ICE;

	SetPosition(pos);
}

//=====================================================
// �ړ����̓���
//=====================================================
void CEnemy::UpdateMove(void)
{
	// �ڕW�ɋ߂��X��T��
	SarchNearIceToDest();

	// ���̃O���b�h�Ɍ���������
	MoveToNextGrid();

	// �O���b�h���ڂ������̃`�F�b�N
	CheckChangeGrid();
}

//=====================================================
// �ڕW�ɋ߂��X��T��
//=====================================================
void CEnemy::SarchNearIceToDest(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;
	
	vector<CIce*> apIce = pIceMgr->GetAroundIce(m_nGridV, m_nGridH);

	// �ڕW�ʒu�̎擾
	D3DXVECTOR3 posDest = pIceMgr->GetGridPosition(&m_nGridVDest, &m_nGridHDest);

	// �S�����̕X�`�F�b�N
	float fLengthMin = FLT_MAX;

	for (auto it : apIce)
	{
		if (it == nullptr)
			continue;

		D3DXVECTOR3 posIce = it->GetPosition();
		float fDiff = 0.0f;

		if (universal::DistCmpFlat(posIce, posDest, fLengthMin, &fDiff))
		{// ���̃O���b�h�������ɐݒ�
			pIceMgr->GetIceIndex(it, &m_nGridVNext, &m_nGridHNext);
			fLengthMin = fDiff;
		}
	}
}

//=====================================================
// ���̃O���b�h�Ɍ������Ĉړ�����
//=====================================================
void CEnemy::MoveToNextGrid(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// ���݂Ǝ��̃O���b�h�̍��W���擾
	D3DXVECTOR3 posNext = pIceMgr->GetGridPosition(&m_nGridVNext, &m_nGridHNext);
	D3DXVECTOR3 pos = GetPosition();

	// �����x�N�g�����X�s�[�h���ɐ��K��
	D3DXVECTOR3 vecDiff = posNext - pos;
	universal::VecConvertLength(&vecDiff, m_fSpeedMove);
	SetMove(vecDiff);

	// ������␳����
	D3DXVECTOR3 rot = GetRotation();
	float fRotDest = atan2f(-vecDiff.x, -vecDiff.z);
	universal::FactingRot(&rot.y, fRotDest, SPEED_ROTATION);

	SetRotation(rot);
}

//=====================================================
// �O���b�h���ς�������̃`�F�b�N
//=====================================================
void CEnemy::CheckChangeGrid(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	int nIdxV = -1;
	int nIdxH = -1;

	// �O���b�h�ԍ��̎擾
	D3DXVECTOR3 pos = GetPosition();
	pIceMgr->GetIdxGridFromPosition(pos, &nIdxV, &nIdxH);

	if ((nIdxV == m_nGridV &&
		nIdxH == m_nGridH) ||
		nIdxV == -1 ||
		nIdxH == -1)
	{// �O���b�h���ς���ĂȂ����͋U��Ԃ�
		return;
	}
	else
	{// �O���b�h���ς���Ă���l��ۑ����Đ^��Ԃ�
		m_nGridV = nIdxV;
		m_nGridH = nIdxH;

		return;
	}
}

//=====================================================
// �Y�����̓���
//=====================================================
void CEnemy::UpdateDrift(void)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// �C���̃x�N�g���擾
	CIceManager::E_Stream dir = pIceManager->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// ����鑬�x�ɐ��K�����Ĉʒu�����Z
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow;
	AddPosition(vecStream);

	COcean *pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
	{
		return;
	}

	// �C�ƈꏏ�ɕX�𓮂�������
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos.y = pOcean->GetHeight(pos, &move) + HEIGHT_ICE;

	SetPosition(pos);
}

//=====================================================
// �O���b�h�ɂ��ړ�
//=====================================================
void CEnemy::TranslateByGrid(int nIdxV, int nIdxH)
{
	CIceManager *pIcemgr = CIceManager::GetInstance();

	if (pIcemgr == nullptr)
		return;

	D3DXVECTOR3 pos = pIcemgr->GetGridPosition(&nIdxV, &nIdxH);

	CIce *pIce = pIcemgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIce == nullptr)
		return;

	pos.y = pIce->GetPosition().y;

	SetPosition(pos);
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CEnemy::Debug(void)
{
	CDebugProc* pDebugProc = CDebugProc::GetInstance();
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	if (pDebugProc == nullptr || pInputKeyboard == nullptr)
		return;

	pDebugProc->Print("\n�ڕW�O���b�h[%d,%d]", m_nGridVDest, m_nGridHDest);
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemy::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}