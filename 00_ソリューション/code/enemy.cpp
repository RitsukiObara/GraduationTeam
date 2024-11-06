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
#include "ocean.h"
#include "destroy_score.h"
#include "effect3D.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float HEIGHT_ICE = 100.0f;	// �X�̍���
const float SPPED_MOVE_INIT = 1.6f;	// �����ړ����x
const float SPEED_ROTATION = 0.1f;	// ��]���x
const float TIME_DEATH_IN_DRIFT = 10.0f;	// �Y�����Ď��ʂ܂ł̎���
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
std::vector<CEnemy*> CEnemy::s_vector = {};	// ���g�̃|�C���^

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CEnemy::CEnemy(int nPriority) : m_nGridV(0), m_nGridH(0),m_state(E_State::STATE_NONE), m_pIceLand(nullptr), m_bFollowIce(false),
m_move(),m_nGridVDest(0), m_nGridHDest(0), m_fSpeedMove(0.0f), m_fTimerDeath(0.0f)
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
// ��~���̓���
//=====================================================
void CEnemy::UpdateStop(void)
{
	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
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

	int aV[CIceManager::DIRECTION_MAX] = {};
	int aH[CIceManager::DIRECTION_MAX] = {};

	// ���ӂ̃O���b�h�ԍ��̌v�Z
	Grid::CalcAroundGrids(m_nGridV, m_nGridH, aV, aH);

	// �T���t���O�p�̕X�̔z��
	vector<CIce*> apIceSave[CIceManager::DIRECTION_MAX];

	// ���ݗ����Ă���X�̎擾
	CIce *pIceStand = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	CIce *pIceNext = nullptr;

	// ��ԃR�X�g�̒Ⴂ�ׂ荇���X���`�F�b�N
	size_t sizeMin = UINT_MAX;	// �ŏ��R�X�g�̗p��
	for (int i = 0; i < CIceManager::DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;

		if (enemy::IsEnemyOnIce(aV[i], aH[i]))
			continue;

		if (apIce[i]->IsPeck())
			continue;

		// �����Ă���X��ǉ����Ă���
		apIceSave[i].push_back(pIceStand);

		// �o�H�̒T��
		bool bFindPath = PathFind(aV[i], aH[i], apIceSave[i]);

		if (!bFindPath)
		{// �o�H���s���l�肾������z����N���A
			apIceSave[i].clear();
		}
		else
		{// ����������A�R�X�g���r����
			if (sizeMin <= apIceSave[i].size())
				continue;

			// �R�X�g���ł�������������ۑ�
			sizeMin = apIceSave[i].size();

			pIceNext = apIceSave[i][1];
		}
	}

	if (pIceNext != nullptr)
	{// ���̕X�������ł����炻�̔ԍ������̔ԍ��ɂ���
		pIceMgr->GetIceIndex(pIceNext, &m_nGridVNext, &m_nGridHNext);
	}
}

//=====================================================
// �T���̍ċA�֐�
//=====================================================
bool CEnemy::PathFind(int nIdxV, int nIdxH, vector<CIce*>& rIceSave)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return false;

	// ���ӃO���b�h�̌v�Z
	vector<CIce*> apIce(CIceManager::DIRECTION_MAX);
	int aV[CIceManager::DIRECTION_MAX] = {};
	int aH[CIceManager::DIRECTION_MAX] = {};

	// ���ӂ̃O���b�h�ԍ��̌v�Z
	Grid::CalcAroundGrids(nIdxV, nIdxH, aV, aH);

	// �T���ςݔz��ɓ����
	CIce *pIceFind = pIceMgr->GetGridIce(&nIdxV, &nIdxH);

	if (pIceFind == nullptr)
		return false;

	rIceSave.push_back(pIceFind);

	// �ڕW�ɓ������Ă�����^��Ԃ�
	if (nIdxV == m_nGridVDest && nIdxH == m_nGridHDest)
		return true;

	// �O���b�h�ԍ����͈͊O�ɂ����ĂȂ����̃`�F�b�N
	int nNumV = pIceMgr->GetNumGridV();
	int nNumH = pIceMgr->GetNumGridH();

	for (int i = 0; i < CIceManager::DIRECTION_MAX; i++)
	{
		int nV = aV[i];
		int nH = aH[i];

		if (!universal::LimitValueInt(&nV, nNumV - 1, 0) &&
			!universal::LimitValueInt(&nH, nNumH - 1, 0))
		{// �w�肵���ԍ����O���b�h���z���Ă��Ȃ��ꍇ�̂ݕۑ�
			apIce[i] = pIceMgr->GetGridIce(&aV[i], &aH[i]);
		}
	}

	D3DXVECTOR3 posDest = pIceMgr->GetGridPosition(&m_nGridVDest, &m_nGridHDest);
	float fDistMin = FLT_MAX;

	int nIdxMin = 0;
	for (int i = 0; i < CIceManager::DIRECTION_MAX; i++)
	{
		if (apIce[i] == nullptr)
			continue;	// ��񂪂Ȃ��Ȃ疳��

		if (universal::FindFromVector(rIceSave, apIce[i]))
			continue;	// �T���ς݂Ȃ疳��

		if (enemy::IsEnemyOnIce(aV[i], aH[i]))
			continue;	// ���̓G������Ă��疳��

		if (apIce[i]->IsPeck())
			continue;	// ��������Ȃ疳��

		D3DXVECTOR3 posIce = apIce[i]->GetPosition();
		float fDiff = 0.0f;

		if (universal::DistCmpFlat(posIce, posDest, fDistMin, &fDiff))
		{
			nIdxMin = i;
			fDistMin = fDiff;
		}
	}

	return PathFind(aV[nIdxMin], aH[nIdxMin], rIceSave);	// �T��

	// �����܂Œʂ�����s���l�܂�̃��[�g�A�U��Ԃ�
	return false;
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
	
#ifdef _DEBUG
	//CEffect3D::Create(posNext, 100.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//CEffect3D::Create(pos, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
#endif
}

//=====================================================
// �O���b�h���ς�������̃`�F�b�N
//=====================================================
void CEnemy::CheckChangeGrid(void)
{
	CIceManager* pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// �O���b�h�ԍ��̎擾
	D3DXVECTOR3 pos = GetPosition();
	CIce *pIce = pIceMgr->GetGridIce(&m_nGridVNext, &m_nGridHNext);
	
	if (!pIceMgr->IsInIce(pos, pIce))
	{// �O���b�h���ς���ĂȂ����͋U��Ԃ�
		return;
	}
	else
	{// �O���b�h���ς���Ă���l��ۑ����Đ^��Ԃ�
		m_nGridV = m_nGridVNext;
		m_nGridH = m_nGridHNext;

		if (m_nGridV == m_nGridVDest && m_nGridH == m_nGridHDest)
			AliveDestGrid();	// �ړI�n�ɂ�������

		return;
	}
}

//=====================================================
// �Y�����̓���
//=====================================================
void CEnemy::UpdateDrift(void)
{
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	// �C���̃x�N�g���擾
	CIceManager::E_Stream dir = pIceMgr->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// ����鑬�x�ɐ��K�����Ĉʒu�����Z
	float fSpeedFlow = pIceMgr->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow;
	AddPosition(vecStream);

	COcean *pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
		return;

	// �C�ƈꏏ�ɕX�𓮂�������
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SetMove(move);

	pos.y = pOcean->GetHeight(pos, &move) + HEIGHT_ICE;

	SetPosition(pos);

	// �O���b�h����������~�܂�
	pIceMgr->GetIdxGridFromPosition(pos, &m_nGridV, &m_nGridH);

	CIce *pIce = pIceMgr->GetGridIce(&m_nGridV, &m_nGridH);

	if (pIce != nullptr)
		SetState(CEnemy::E_State::STATE_MOVE);

	// �Y�����̎�
	DriftDeath();
}

//=====================================================
// �Y�����̎�
//=====================================================
void CEnemy::DriftDeath(void)
{
	m_fTimerDeath += CManager::GetDeltaTime();

	if (m_fTimerDeath > TIME_DEATH_IN_DRIFT)
		Death();	// ��莞�Ԍo�߂����玀�S���̏����ɓ���
}

//=====================================================
// ���S���̏���
//=====================================================
void CEnemy::Death(void)
{
	Uninit();
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

	pDebugProc->Print("\n���݃O���b�h[%d,%d]", m_nGridV, m_nGridH);
	pDebugProc->Print("\n���̃O���b�h[%d,%d]", m_nGridVNext, m_nGridHNext);
	pDebugProc->Print("\n�ڕW�O���b�h[%d,%d]", m_nGridVDest, m_nGridHDest);

	pDebugProc->Print("\n���݂̏��[%d]", m_state);
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemy::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}