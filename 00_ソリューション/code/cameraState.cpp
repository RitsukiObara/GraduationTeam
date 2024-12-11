//*****************************************************
//
// �J�����X�e�C�g[CameraState.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "CameraState.h"
#include "camera.h"
#include "effect3D.h"
#include "inputmouse.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "manager.h"
#include "title.h"
#include "resultSingle.h"
#include "player.h"
#include "enemy.h"
#include "selectStagePenguin.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float MOVE_SPEED = 21.0f;				// �ړ��X�s�[�h
const float ROLL_SPEED = 0.04f;				// ��]�X�s�[�h
const float FACT_CORRECT_CONTOROLLL = 0.9f;	// ���쎞�̈ʒu�␳�W��

const float LENGTH_FOLLOW = 1500.0f;	// �Ǐ]���̃J��������
const float ANGLE_FOLLOW = 0.33f;		// �Ǐ]���̃J�����p�x
const float SPPED_MOVE_FOLLOW = 0.09f;	// �Ǐ]���̃J�������x
const D3DXVECTOR3 POSR_GAME = { 0.0f,0.0f,-300.0f };		// �Q�[�����̒����_�ʒu
const D3DXVECTOR3 POSV_GAME = { 0.0f,1544.0f,-681.0f };		// �Q�[�����̎��_�ʒu
const D3DXVECTOR3 POSR_MULTI = { -80.0f,0.0f,-330.0f };		// �}���`���̒����_�ʒu
const D3DXVECTOR3 POSV_MULTI = { -80.0f,1670.0f,-1170.0f };	// �}���`���̎��_�ʒu

const D3DXVECTOR3 POSR_DEFAULT_SELECTSTAGE = { 0.0f,0.0f,-400.0f };		// �X�e�[�W�Z���N�g���̃f�t�H���g�����_�ʒu
const D3DXVECTOR3 POSV_DEFAULT_SELECTSTAGE = { 0.0f,2244.0f,-2001.0f };	// �X�e�[�W�Z���N�g���̃f�t�H���g���_�ʒu

const D3DXVECTOR3 POSR_DEFAULT_SELECTMODE = { 0.0f,50.0f,0.0f };		// ���[�h�Z���N�g���̃f�t�H���g�����_�ʒu
const D3DXVECTOR3 POSV_DEFAULT_SELECTMODE = { 0.0f,200.0f,-2000.0f };	// ���[�h�Z���N�g���̃f�t�H���g���_�ʒu

const D3DXVECTOR3 POSR_DEFAULT_RESULTMULTI = { 800.0f,50.0f,-400.0f };		// �}���`���[�h�̃��U���g���̃f�t�H���g�����_�ʒu
const D3DXVECTOR3 POSV_DEFAULT_RESULTMULTI = { 800.0f,400.0f,-2400.0f };	// �}���`���[�h�̃��U���g���̃f�t�H���g���_�ʒu

//-----------------------------------
// �v���C���[�Ǐ]�̒萔
//-----------------------------------
namespace followPlayer
{
const float DIST_WIDTH = 200.0f;			// �J�����̕ύX���镝
const float DEFAULT_DIST_CAMERA = 1500.0f;	// �f�t�H���g�̋���
const float NEAR_LINE = 500.0f;				// �߂��Ɣ��f���鋗��
const float TIME_MOVE = 1.5f;				// �ړ��ɂ����鎞��
const float SPEED_CLOSE = 0.05f;			// �߂Â����x
}

//-----------------------------------
// �V���O�����U���g�̒萔
//-----------------------------------
namespace resultSingle
{
const D3DXVECTOR3 POS_OFFSET = { 0.0f,100.0f,-500.0f };	// �ڕW�n�_�̃I�t�Z�b�g
const float SPEED_POSR = 0.1f;	// �����_�̑��x
}

//-----------------------------------
// �X�e�[�W�I���̒萔
//-----------------------------------
namespace selectStage
{
const D3DXVECTOR3 OFFSET = { 0.0f,2250.0f,-2000.0f };	// �I�t�Z�b�g
const float FACT_MOVE = 0.6f;							// �ړ��W��
}
}

//***********************************************************************************
// �v���C���[�̒Ǐ]
//***********************************************************************************
//=====================================================
// �R���X�g���N�^
//=====================================================
CFollowPlayer::CFollowPlayer() : m_fTimerMove(0.0f)
{

}

//=====================================================
// ������
//=====================================================
void CFollowPlayer::Init(CCamera *pCamera)
{
	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->fLength = LENGTH_FOLLOW;
	pInfoCamera->posR = POSR_GAME;

	pInfoCamera->rot.x = ANGLE_FOLLOW;
	pInfoCamera->rot.y = D3DX_PI;

	pCamera->SetPosV();
}

//=====================================================
// �X�V
//=====================================================
void CFollowPlayer::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// �����̊Ǘ�
	ManageDist();

	// �p�x�̐ݒ�
	pInfoCamera->rot.x = ANGLE_FOLLOW;
	pInfoCamera->rot.y = D3DX_PI;

	// �����_�ʒu�̐ݒ�
	pInfoCamera->posR = POSR_GAME;

	// ���_�̐ݒ�
	pCamera->SetPosV();
}

//=====================================================
// �����̊Ǘ�
//=====================================================
void CFollowPlayer::ManageDist(void)
{
	CCamera *pCamera = CManager::GetCamera();
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	if (pInfoCamera == nullptr)
		return;

	// �G���߂������狗���̊����ɉ����ăJ�����𓮂���
	float fRate;
	if (IsNearEnemy(&fRate))
		m_fTimerMove += CManager::GetDeltaTime() * followPlayer::SPEED_CLOSE;
	else
		m_fTimerMove -= CManager::GetDeltaTime();

	// �l�̐���
	universal::LimitValue(&m_fTimerMove, followPlayer::TIME_MOVE, 0.0f);
	
	// �^�C�}�[���C�[�W���O
	float fTime = m_fTimerMove / followPlayer::TIME_MOVE;
	fRate = easing::EaseOutExpo(fTime);
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	pInfoCamera->fLength = followPlayer::DEFAULT_DIST_CAMERA - followPlayer::DIST_WIDTH * fRate;
}

//=====================================================
// �G���߂�����
//=====================================================
bool CFollowPlayer::IsNearEnemy(float *pRate)
{
	if (CPlayer::GetInstance().empty())
		return false;

	CPlayer *pPlayer = *CPlayer::GetInstance().begin();
	vector<CEnemy*> apEnemy = CEnemy::GetInstance();

	if (pPlayer == nullptr || apEnemy.empty())
		return false;

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// �S�G�Ƃ̋����`�F�b�N
	float fDistMin = followPlayer::NEAR_LINE;
	CEnemy *pEnemyNearest = nullptr;

	for (CEnemy *pEnemy : apEnemy)
	{
		float fDiff = 0.0f;

		if (universal::DistCmpFlat(posPlayer, pEnemy->GetPosition(), fDistMin, &fDiff))
		{
			fDistMin = fDiff;

			pEnemyNearest = pEnemy;

			if (pRate != nullptr)
				*pRate = 1.0f - fDiff / followPlayer::NEAR_LINE;
		}
	}

	return pEnemyNearest != nullptr;
}

//***********************************************************************************
// �}���`�̃J����
//***********************************************************************************
//=====================================================
// �R���X�g���N�^
//=====================================================
CMultiGame::CMultiGame()
{

}

//=====================================================
// �X�V
//=====================================================
void CMultiGame::Update(CCamera* pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posR = POSR_MULTI;
	pInfoCamera->posV = POSV_MULTI;
}

//=====================================================
// ���삷��
//=====================================================
void CMoveControl::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// ���͎擾
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();

	float fMove = MOVE_SPEED;

	//�}�E�X����======================================================
	if (pMouse->GetPress(CInputMouse::BUTTON_RMB) == true)
	{//�E�N���b�N���A���_����
		D3DXVECTOR3 rot;

		//�}�E�X�̈ړ��ʑ��
		rot = { (float)pMouse->GetMoveIX() * ROLL_SPEED, (float)pMouse->GetMoveIY() * ROLL_SPEED, 0.0f };

		D3DXVec3Normalize(&rot, &rot);

		//���_�̐���
		pInfoCamera->rot.y += rot.x * ROLL_SPEED;
		pInfoCamera->rot.x -= rot.y * ROLL_SPEED;

		if (pKeyboard->GetPress(DIK_LSHIFT) == true)
		{//����
			fMove *= 7;
		}

		D3DXVECTOR3 rotMove = pInfoCamera->rot;
		D3DXVECTOR3 vecPole = { 0.0f,0.0f,0.0f };

		// ���_�ړ�===============================================
		if (pKeyboard->GetPress(DIK_A) == true)
		{// ���ړ�
			pInfoCamera->posVDest.x -= sinf(pInfoCamera->rot.y - D3DX_PI * 0.5f) * fMove;
			pInfoCamera->posVDest.z -= cosf(pInfoCamera->rot.y - D3DX_PI * 0.5f) * fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_D) == true)
		{// �E�ړ�
			pInfoCamera->posVDest.x -= sinf(pInfoCamera->rot.y - D3DX_PI * -0.5f) * fMove;
			pInfoCamera->posVDest.z -= cosf(pInfoCamera->rot.y - D3DX_PI * -0.5f) * fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_W) == true)
		{// �O�ړ�
			pInfoCamera->posVDest.x += sinf(-pInfoCamera->rot.x) * sinf(pInfoCamera->rot.y) * fMove;
			pInfoCamera->posVDest.y -= cosf(-pInfoCamera->rot.x) * MOVE_SPEED;
			pInfoCamera->posVDest.z += sinf(-pInfoCamera->rot.x) * cosf(pInfoCamera->rot.y) * fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_S) == true)
		{// ��ړ�
			pInfoCamera->posVDest.x += sinf(-pInfoCamera->rot.x + D3DX_PI) * sinf(pInfoCamera->rot.y) * fMove;
			pInfoCamera->posVDest.y -= cosf(-pInfoCamera->rot.x + D3DX_PI) * MOVE_SPEED;
			pInfoCamera->posVDest.z += sinf(-pInfoCamera->rot.x + D3DX_PI) * cosf(pInfoCamera->rot.y) * fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_E) == true)
		{// �㏸
			pInfoCamera->posVDest.y += fMove;
			pCamera->SetPosR();
		}
		if (pKeyboard->GetPress(DIK_Q) == true)
		{// ���~
			pInfoCamera->posVDest.y -= fMove;
			pCamera->SetPosR();
		}

		pCamera->SetPosR();
	}

	pCamera->MoveDist(FACT_CORRECT_CONTOROLLL);
}

//**************************************************************************
// �^�C�g��
//**************************************************************************
//=====================================================
// �X�V
//=====================================================
void CCameraStateTitle::Update(CCamera* pCamera)
{
	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	pInfoCamera->rot.y += 0.005f;

	universal::LimitRot(&pInfoCamera->rot.y);

	pCamera->SetPosV();

	CDebugProc::GetInstance()->Print("\n���_  [%f, %f, %f]", pInfoCamera->posV.x, pInfoCamera->posV.y, pInfoCamera->posV.z);
	CDebugProc::GetInstance()->Print("\n�����_[%f, %f, %f]", pInfoCamera->posR.x, pInfoCamera->posR.y, pInfoCamera->posR.z);
	CDebugProc::GetInstance()->Print("\n�J��������[%f]", pInfoCamera->fLength);
	CDebugProc::GetInstance()->Print("\n�p�x  [%f, %f, %f]", pInfoCamera->rot.x, pInfoCamera->rot.y, pInfoCamera->rot.z);
}

//**************************************************************************
// �X�e�[�W�Z���N�g
//**************************************************************************
//=====================================================
// ������
//=====================================================
void CCameraStateSelectStage::Init(CCamera* pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// �J�������_�A�����_�ʒu������
	pInfoCamera->posR = POSR_DEFAULT_SELECTSTAGE;
	pInfoCamera->posV = POSV_DEFAULT_SELECTSTAGE;
}

//=====================================================
// �X�V
//=====================================================
void CCameraStateSelectStage::Update(CCamera* pCamera)
{
	// �J�����̒Ǐ]
	D3DXVECTOR3 posPenguin = m_pPenguin->GetPosition();

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posRDest = posPenguin;
	pInfoCamera->posRDest.y = 0.0f;
	pInfoCamera->posVDest = posPenguin + selectStage::OFFSET;

	pCamera->MoveDist(selectStage::FACT_MOVE);

	pInfoCamera->posV.y = selectStage::OFFSET.y;
}

//**************************************************************************
// �V���O�����U���g
//**************************************************************************
//=====================================================
// �R���X�g���N�^
//=====================================================
CCameraResultSingle::CCameraResultSingle(CResultSingle *pResult)
{
	// ���U���g�̎󂯎��
	m_pResult = pResult;
}

//=====================================================
// ������
//=====================================================
void CCameraResultSingle::Init(CCamera* pCamera)
{
	// �ړI�ʒu�ݒ�
	DecidePosDest(pCamera);
}

//=====================================================
// �ړI�ʒu�ݒ�
//=====================================================
void CCameraResultSingle::DecidePosDest(CCamera* pCamera)
{
	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// �J�������ݒn�������ʒu�ɐݒ�
	m_posInitiial = pInfoCamera->posV;

	//-------------------------------
	// �ڕW�ʒu�̐ݒ�
	//-------------------------------
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();	// �v���C���[�̎擾
	assert(!apPlayer.empty());

	CPlayer *pPlayer = *apPlayer.begin();
	assert(pPlayer != nullptr);

	// �v���C���[�̈ʒu�ɃI�t�Z�b�g��ݒ肵�āA�ڕW�ʒu�ɂ���
	m_posDest = pPlayer->GetPosition() + resultSingle::POS_OFFSET;

	// ���������x�N�g���̐ݒ�
	m_vecDiffInitial = m_posDest - m_posInitiial;
}

//=====================================================
// �X�V
//=====================================================
void CCameraResultSingle::Update(CCamera* pCamera)
{
	// �v���C���[�̑O�܂ňړ����鏈��
	MoveToPlayerFront(pCamera);

	//-------------------------------
	// �ڕW�ʒu�̐ݒ�
	//-------------------------------
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();	// �v���C���[�̎擾
	assert(!apPlayer.empty());

	CPlayer* pPlayer = *apPlayer.begin();
	assert(pPlayer != nullptr);
	D3DXVECTOR3 rot = pPlayer->GetRotation();

	universal::FactingRot(&rot.y, 0.0f, 0.02f);

	pPlayer->SetRotation(rot);
}

//=====================================================
// �v���C���[�̑O�܂ňړ����鏈��
//=====================================================
void CCameraResultSingle::MoveToPlayerFront(CCamera* pCamera)
{
	//-------------------------------
	// ���_�̈ړ�
	//-------------------------------
	m_fTimerMove += CManager::GetDeltaTime();

	if (m_fTimerMove > CCameraResultSingle::TIME_MOVE)
		m_pResult->SetState(CResultSingle::E_State::STATE_FADE);

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	float fTime = m_fTimerMove / CCameraResultSingle::TIME_MOVE;
	float fRate = easing::EaseOutExpo(fTime);

	pInfoCamera->posV = m_posInitiial + m_vecDiffInitial * fRate;

	//-------------------------------
	// �����_�̈ړ�
	//-------------------------------
	D3DXVECTOR3 posDestR = m_posDest - resultSingle::POS_OFFSET;

	pInfoCamera->posR += (posDestR - pInfoCamera->posR) * resultSingle::SPEED_POSR;
}

//**************************************************************************
// �}���`���[�h�̃��U���g
//**************************************************************************
//=====================================================
// ������
//=====================================================
void CCameraStateResultMulti::Init(CCamera* pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	// �J�������_�A�����_�ʒu������
	pInfoCamera->posR = POSR_DEFAULT_RESULTMULTI;
	pInfoCamera->posV = POSV_DEFAULT_RESULTMULTI;
}

//=====================================================
// �X�V
//=====================================================
void CCameraStateResultMulti::Update(CCamera* pCamera)
{

}

//**************************************************************************
// ���[�h�Z���N�g
//**************************************************************************
//=====================================================
// ������
//=====================================================
void CCameraStateSelectMode::Init(CCamera* pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	// �J�������_�A�����_�ʒu������
	pInfoCamera->posR = POSR_DEFAULT_SELECTMODE;
	pInfoCamera->posV = POSV_DEFAULT_SELECTMODE;
}

//=====================================================
// �X�V
//=====================================================
void CCameraStateSelectMode::Update(CCamera* pCamera)
{

}