//*****************************************************
//
// �X�̏���[ice.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "ice.h"
#include "iceHard.h"
#include "iceHardMulti.h"
#include "iceManager.h"
#include "texture.h"
#include "gameObject.h"
#include "fan3D.h"
#include "meshcylinder.h"
#include "ocean.h"
#include "objectX.h"
#include "manager.h"
#include "particle.h"
#include "model.h"
#include "effect3D.h"
#include "enemy.h"
#include "player.h"
#include "MyEffekseer.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const string PATH_TEX = "data\\TEXTURE\\MATERIAL\\ice001.jpg";				// �e�N�X�`���p�X
const string PATH_TEX_OVERRAY = "data\\TEXTURE\\MATERIAL\\iceanimation.jpg";	// �I�[�o���C�e�N�X�`���p�X
const float SIZE_INIT = 100.0f;	// �����T�C�Y
const float HEIGHT_ICE = 50.0f;	// �X�̍���
const int NUM_CORNER = 6;	// �p�̐�
const D3DXVECTOR3 ROT_UP_INIT = { D3DX_PI * 0.5f,0.0f,0.0f };	// �㑤�̏�������

const float TIME_REPAIR_ICE = 11.0f;	// �X�̏C���ɂ����鎞��
const int MAX_SCALE = 50; // �X�P�[���̍ő�l
const int MIN_SCALE = 20; // �X�P�[���̍ŏ��l
const string PATH_ICE_DEBRIS = "data\\MODEL\\block\\Drift_ice_piece.x";	// �j�ЕX�̃��f���p�X
const float SPEED_SINK = 5.0f;	// ���ޑ��x
const float HEIGHT_DELETE = -100.0f;	// �폜����܂ł̍���

const float HEIGHT_DEFAULT_FROM_OCEAN = 50.0f;	// �C����̃f�t�H���g�̍���
const float HEIGHT_NORMALSINK_FROM_OCEAN = 10.0f;	// �C����̒ʏ풾�ލ���

const float LINE_STOP_ICE = 1.0f;	// �X���~�܂邵�����l

//------------------------------
// �X���̒萔
//------------------------------
namespace tilt
{
const float SPEED_ROT = 0.1f;			// ��]���x
const float MAX_TILT = D3DX_PI * 0.2f;	// �ő�̌X��
const float RATE_COLLIDE = 1.6f;		// ����̊���
}

//------------------------------
// �����g�̒萔
//------------------------------
namespace ripple
{
const int MAX_TIME = 7;	// �����ɂ�����ő厞��
const int MIN_TIME = 2;	// �����ɂ�����ŏ�����
}

//------------------------------
// ���̒萔
//------------------------------
namespace flash
{
const int MAX_TIME = 14;			// �Đ��ɂ�����ő厞��
const int MIN_TIME = 7;				// �Đ��ɂ�����ŏ�����
const int FRAME_ANIMATION = 2;		// �A�j���[�V������؂�ւ���t���[��
const int PATERN_ANIM = 5;			// �A�j���[�V�����̃p�^�[��
}
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
int CIce::s_nNumAll = 0;
std::vector<CIce*> CIce::s_Vector = {};	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CIce::CIce(int nPriority) : CObject3D(nPriority), m_state(E_State::STATE_NONE), m_bBreak(false), m_bCanFind(false), m_bPeck(false),
m_pSide(nullptr),m_pUp(nullptr), m_pState(nullptr), m_bSink(false), m_bStop(nullptr), m_abRipleFrag(), m_nCntAnimFlash(0), m_rotDest()
{
	s_nNumAll++;
	s_Vector.push_back(this);
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CIce::~CIce()
{
	s_nNumAll--;
}

//=====================================================
// ��������
//=====================================================
CIce *CIce::Create(E_Type type,E_State state)
{
	CIce *pIce = nullptr;

	if (pIce == nullptr)
	{
		switch (type)
		{
		case CIce::TYPE_NORMAL:
			pIce = new CIce;
			break;
		case CIce::TYPE_HARD:
			pIce = new CIceHard;
			break;
		case CIce::TYPE_HARDMULTI:
			pIce = new CIceHardMulti;
			break;
		default:
			assert(false);
			break;
		}

		if (pIce != nullptr)
		{
			pIce->m_state = state;
			pIce->Init();
		}
	}

	return pIce;
}

//=====================================================
// ����������
//=====================================================
HRESULT CIce::Init(void)
{
	EnableCanFind(true);

	// ���b�V���̐���
	CreateMesh();

	// �g�����X�t�H�[���̏����ݒ�
	SetTransform(SIZE_INIT);

	// �X�e�C�g������
	ChangeState(new CIceStaeteNormal);

	// ���鏈���̏�����
	StartFlash();

	// �p���N���X�̏�����
	CObject3D::Init();

	return S_OK;
}

//=====================================================
// ���b�V���̐���
//=====================================================
void CIce::CreateMesh(void)
{
	if (m_pUp == nullptr)
	{
		m_pUp = CFan3D::Create(4, NUM_CORNER);

		if (m_pUp != nullptr)
		{
			int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
			m_pUp->SetIdxTexture(nIdxTexture);

			int nIdxTextureOverray = Texture::GetIdx(&PATH_TEX_OVERRAY[0]);
			m_pUp->SetIdxTextureOverRay(nIdxTextureOverray);
			m_pUp->SetVtx();
		}
	}

	if (m_pSide == nullptr)
	{
		m_pSide = CMeshCylinder::Create(NUM_CORNER);
		int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
		m_pSide->SetIdxTexture(nIdxTexture);
	}

	SetTransform(Grid::SIZE);
}

//=====================================================
// ���b�V���̔j��
//=====================================================
void CIce::DeleteMesh(void)
{
	Object::DeleteObject((CObject**)&m_pUp);
	Object::DeleteObject((CObject**)&m_pSide);
}

//=====================================================
// �I������
//=====================================================
void CIce::Uninit(void)
{
	// ���b�V���̍폜
	DeleteMesh();

	for (auto itr = s_Vector.begin(); itr < s_Vector.end(); itr++)
	{
		//�폜�Ώۂ���Ȃ��ꍇ
		if (*itr != this)
		{
			continue;
		}
		//Vector����폜
		s_Vector.erase(itr);

		break;
	}

	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	// �O���b�h����폜
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager != nullptr)
		pIceManager->DeleteIce(this);

	// �p���N���X�̏I��
	CObject3D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CIce::Update(void)
{
	if (!IsSink())	// ���ރt���O�������Ă��Ȃ��Ƃ��̂ݍs��
		FollowWave();	// �g�ɒǏ]���鏈��

	// �X�e�C�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);

	// ��ɏ���Ă镨�̌��o
	SearchOnThis();

	// �X���̏���
	Tilt();

	// �����g�̏���
	Ripples();

	// ���b�V���̒Ǐ]
	FollowMesh();

	// ���鏈��
	Flash();

	// �p���N���X�̍X�V
	CObject3D::Update();
}

//=====================================================
// �g�ɒǏ]���鏈��
//=====================================================
void CIce::FollowWave(void)
{
	COcean *pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
	{
		return;
	}

	// �C�ƈꏏ�ɕX�𓮂�������
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos.y = pOcean->GetHeight(pos, &move) + HEIGHT_ICE;

	if (m_pUp != nullptr)
	{
		m_pUp->SetPosition(pos);
	}
	if (m_pSide != nullptr)
	{
		m_pSide->SetPosition(pos + D3DXVECTOR3(0.0f, -HEIGHT_ICE, 0.0f));
	}

	SetPosition(pos);
}

//=====================================================
// ���g�ɏ���Ă���̂̌��o
//=====================================================
void CIce::SearchOnThis(void)
{
	if (!IsCanPeck())
		return;

	vector<CGameObject*> apObject;

	GetOnTopObject(apObject);


}

//=====================================================
// �������炪����Ă锻��
//=====================================================
bool CIce::IsOnTopAnyObject(void)
{
	vector<CGameObject*> apObject;

	GetOnTopObject(apObject);

	// ��ɂǂꂩ������Ă���^��Ԃ�
	for (CGameObject* object : apObject)
	{
		D3DXVECTOR3 posObject = object->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		if (universal::DistCmpFlat(pos, posObject, SIZE_INIT, nullptr))
		{// ����������Ă�̂Ő^��Ԃ�
			return true;
		}
	}

	return false;
}

//=====================================================
// ���肷��I�u�W�F�N�g�̌��o
//=====================================================
void CIce::GetOnTopObject(vector<CGameObject*> &rVector, float fRate)
{
	vector<CGameObject*> apObject;

	// �G�̒ǉ�
	vector<CEnemy*> aEnemy = CEnemy::GetInstance();

	for (CEnemy* enemy : aEnemy)
		apObject.push_back((CGameObject*)enemy);

	// �v���C���[�̒ǉ�
	vector<CPlayer*> aPlayer = CPlayer::GetInstance();

	for (CPlayer* player : aPlayer)
		apObject.push_back((CGameObject*)player);

	// ��ɂǂꂩ������Ă���^��Ԃ�
	for (CGameObject* object : apObject)
	{
		D3DXVECTOR3 posObject = object->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		if (universal::DistCmpFlat(pos, posObject, SIZE_INIT * fRate, nullptr))
		{// ����������Ă�̂Ő^��Ԃ�
			rVector.push_back(object);
		}
	}

}

//=====================================================
// �X���̏���
//=====================================================
void CIce::Tilt(void)
{
	if (!IsCanPeck())
		return;	// �@���Ȃ��X�Ȃ�ʂ�Ȃ�

	//------------------------------
	// �X���̌v�Z
	//------------------------------
	// ����Ă���I�u�W�F�N�g�̎擾
	vector<CGameObject*> apObject;
	GetOnTopObject(apObject, tilt::RATE_COLLIDE);

	int nNumObject = (int)apObject.size();

	if (nNumObject != 0)
	{// �����������Ă��画��
		D3DXVECTOR3 vecDiff = { 0.0f,0.0f,0.0f };

		for (CGameObject* pObj : apObject)
		{
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 posObj = pObj->GetPosition();

			vecDiff += posObj - pos;
		}

		// �����x�N�g���𕽋ω�
		vecDiff /= (float)nNumObject;

		m_rotDest.x = vecDiff.z / Grid::SIZE * tilt::MAX_TILT;
		m_rotDest.z = -vecDiff.x / Grid::SIZE * tilt::MAX_TILT;
	}
	else	// ��ɉ�������ĂȂ������猳�ɖ߂�
		m_rotDest = { 0.0f,0.0f,0.0f };

	// �����̕␳
	D3DXVECTOR3 rot = GetRotation();

	rot += (m_rotDest - rot) * tilt::SPEED_ROT;

	SetRotation(rot);
}

//=====================================================
// �����g�̏���
//=====================================================
void CIce::Ripples(void)
{
	// �^�C�}�[�ł̂����~��
	m_fTimerRipples += CManager::GetDeltaTime();

	if (m_fTimerRipples <= m_fSpawnTimeRipples)
		return;

	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	COcean::E_Stream stream = pIceMgr->GetDirStream();

	// ����ɍ����������̃t���O�������Ă��珈����ʂ�
	if (!m_abRipleFrag[stream])
		return;

	// �G�t�F�N�g�̃g�����X�t�H�[���ݒ�
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[stream];

	D3DXVECTOR3 posEffect = GetPosition() - vecStream * SIZE_INIT;
	D3DXVECTOR3 rotEffect = { 0.0f,0.0f,0.0f };
	rotEffect.y = atan2f(vecStream.x, vecStream.z);

	// �G�t�F�N�g�̐���
	MyEffekseer::CreateEffect(CMyEffekseer::TYPE::TYPE_RIPPLESTRINGS, posEffect, rotEffect);

	// �^�C�}�[�̍Đݒ�
	m_fTimerRipples = 0.0f;

	int nRand = universal::RandRange(ripple::MAX_TIME, ripple::MIN_TIME);

	m_fSpawnTimeRipples = (float)nRand;
}

//=====================================================
// ���b�V���̒Ǐ]
//=====================================================
void CIce::FollowMesh(void)
{
	if (m_pUp == nullptr || m_pSide == nullptr)
		return;

	// �}�g���b�N�X���m�肳����
	CulcMatrix();

	D3DXMATRIX mtx = GetMatrix();

	// ��̐�|���S���̐ݒ�
	m_pUp->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUp->SetMatrixParent(mtx);

	// �T�C�h�̃V�����_�[�̐ݒ�
	m_pSide->SetPosition(D3DXVECTOR3(0.0f, -HEIGHT_ICE, 0.0f));
	m_pSide->SetMatrixParent(mtx);
}

//=====================================================
// ���鏈���̊J�n
//=====================================================
void CIce::StartFlash(void)
{
	if (m_pUp == nullptr)
		return;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *m_pUp->GetVtxBuff();
	VERTEX_3D* pVtx;

	if (pVtxBuff == nullptr)
		return;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �X�N���[�������Z�b�g
	int nNumVtx = m_pUp->GetNumVtx();

	for (int i = 0; i < nNumVtx + 2; i++)
	{
		pVtx[i].tex2 = pVtx[i].tex;

		pVtx[i].tex2.x *= 1.0f / flash::PATERN_ANIM;	// �A�j���[�V������ɏk�߂�
		pVtx[i].tex2.x += 1.0f / flash::PATERN_ANIM * flash::PATERN_ANIM;
	}

	// ���_�o�b�t�@�̃A�����b�N
	pVtxBuff->Unlock();

	// �^�C�}�[�̍Đݒ�
	m_fTimerFlash = 0.0f;
	int nRand = universal::RandRange(flash::MAX_TIME, flash::MIN_TIME);
	m_fTimeStartFlash = (float)nRand;
}

//=====================================================
// �����ƌ��鏈��
//=====================================================
void CIce::Flash(void)
{
	m_fTimerFlash += CManager::GetDeltaTime();

	if (m_fTimerFlash < m_fTimeStartFlash)
		return;

	if (m_pUp == nullptr)
		return;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *m_pUp->GetVtxBuff();
	VERTEX_3D* pVtx;

	if (pVtxBuff == nullptr)
		return;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �X�N���[��������
	int nNumVtx = m_pUp->GetNumVtx();

	bool bPassAll = true;	// �S���_���ʉ߂����t���O

	for (int i = 0; i < nNumVtx + 2; i++)
	{
		pVtx[i].tex2 = pVtx[i].tex;

		pVtx[i].tex2.x *= 1.0f / flash::PATERN_ANIM;	// �A�j���[�V������ɏk�߂�
		pVtx[i].tex2.x += 1.0f / flash::PATERN_ANIM * m_nPaternAnim;
	}

	// ���_�o�b�t�@�̃A�����b�N
	pVtxBuff->Unlock();

	// �J�E���^�[���Z
	m_nCntAnimFlash++;

	if (m_nCntAnimFlash % flash::FRAME_ANIMATION == 0)
	{
		m_nPaternAnim = (m_nPaternAnim + 1) % flash::PATERN_ANIM;

		if (m_nPaternAnim == 0)
			StartFlash();	// �A�j���[�V�������߂����烊�Z�b�g
	}
}

//=====================================================
// �F�̐ݒ�
//=====================================================
void CIce::SetColor(D3DXCOLOR col)
{
	if (m_pSide == nullptr || m_pUp == nullptr)
		return;

	m_pSide->SetCol(col);
	m_pUp->SetCol(col);
}

//=====================================================
// �`�揈��
//=====================================================
void CIce::Draw(void)
{
	CObject3D::Draw();
}

//=====================================================
// �g�����X�t�H�[���ݒ�
//=====================================================
void CIce::SetTransform(float fRadius)
{
	if (m_pUp == nullptr || m_pSide == nullptr)
		return;

	D3DXVECTOR3 posIce = GetPosition();

	// ��̐�|���S���̐ݒ�
	m_pUp->SetRadius(fRadius * 0.5f);
	m_pUp->SetPosition(posIce);
	m_pUp->SetVtx();

	// �T�C�h�̃V�����_�[�̐ݒ�
	m_pSide->SetRadius(fRadius * 0.5f);
	m_pSide->SetHeight(HEIGHT_ICE);
	D3DXVECTOR3 posSide = posIce;
	posSide.y -= HEIGHT_ICE;
	m_pSide->SetPosition(posSide);

	m_pSide->SetVtx();
}

//=====================================================
// �X�e�C�g�̕ύX
//=====================================================
void CIce::ChangeState(CIceState *pState)
{
	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	m_pState = pState;

	if (m_pState != nullptr)
		m_pState->Init(this);
}

//*******************************************************************************
// �ʏ�X�e�C�g
//*******************************************************************************
//=====================================================
// ����������
//=====================================================
void CIceStaeteNormal::Init(CIce *pIce)
{
	pIce->EnableStop(true);

	// �Y�����Ă���ԍ����擾
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->GetIceIndex(pIce, &m_nIdxDriftV, &m_nIdxDriftH);
}

//=====================================================
// �I������
//=====================================================
void CIceStaeteNormal::Uninit(CIce *pIce)
{

}

//=====================================================
// �X�V����
//=====================================================
void CIceStaeteNormal::Update(CIce *pIce)
{
	// �ԍ��擾�Ɏ��s���Ă��邩�̊m�F
	bool bMove = CheckFailGetIndex(pIce);

	if(bMove)
		MoveToGrid(pIce);	// �O���b�h�Ɍ������Ĉړ����鏈��
}

//=====================================================
// �ԍ��擾�Ɏ��s���Ă��邩�̊m�F
//=====================================================
bool CIceStaeteNormal::CheckFailGetIndex(CIce *pIce)
{
	if (m_nIdxDriftV == -1 ||
		m_nIdxDriftH == -1)
	{// �ԍ��擾�Ɏ��s���Ă���ꍇ�A�Ď擾
		CIceManager *pIceMgr = CIceManager::GetInstance();

		if (pIceMgr == nullptr)
			return false;

		pIceMgr->GetIceIndex(pIce, &m_nIdxDriftV, &m_nIdxDriftH);

		if (m_nIdxDriftV == 0 && m_nIdxDriftH == 0)
		{
			int n = 0;
		}

		if (m_nIdxDriftV == -1 ||
			m_nIdxDriftH == -1)
			return false;	// ����ł����s�����ꍇ�A�U��Ԃ�
	}

	return true;
}

//=====================================================
// �O���b�h�Ɍ������Ĉړ����鏈��
//=====================================================
void CIceStaeteNormal::MoveToGrid(CIce *pIce)
{
	// �O���b�h�̈ʒu���擾
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	D3DXVECTOR3 posGrid = pIceMgr->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);
	D3DXVECTOR3 posIce = pIce->GetPosition();

	// �����x�N�g����Y�����x�ɐ��K��
	float fSpeedFlow = pIceMgr->GetOceanLevel();
	D3DXVECTOR3 vecDiff = posGrid - posIce;
	D3DXVec3Normalize(&vecDiff, &vecDiff);

	vecDiff *= fSpeedFlow;

	// �X�̈ʒu�Ɉړ��ʂ����Z
	pIce->Translate(vecDiff);

	// ��̃I�u�W�F�N�g�𓮂���
	MoveObjectOnIce(vecDiff,pIce);
}

//=====================================================
// ��̃I�u�W�F�N�g�𓮂���
//=====================================================
void CIceStaeteNormal::MoveObjectOnIce(D3DXVECTOR3 vecMove,CIce *pIce)
{
	vector<CGameObject*> apObject;

	// �G�̒ǉ�
	vector<CEnemy*> aEnemy = CEnemy::GetInstance();

	for (CEnemy* enemy : aEnemy)
		apObject.push_back((CGameObject*)enemy);

	// �v���C���[�̒ǉ�
	vector<CPlayer*> aPlayer = CPlayer::GetInstance();

	for (CPlayer* player : aPlayer)
		apObject.push_back((CGameObject*)player);

	// ��ɂǂꂩ������Ă��瓮����
	for (CGameObject* object : apObject)
	{
		if (object == nullptr)
			continue;

		D3DXVECTOR3 posObject = object->GetPosition();
		D3DXVECTOR3 pos = pIce->GetPosition();

		if (universal::DistCmpFlat(pos, posObject, SIZE_INIT, nullptr))
			object->Translate(vecMove); // ����������Ă�̂œ�����
	}
}

//*******************************************************************************
// ����X�e�C�g
//*******************************************************************************
//=====================================================
// ����������
//=====================================================
void CIceStaeteBreak::Init(CIce *pIce)
{
	// �X�̃��b�V�����폜
	pIce->DeleteMesh();
	
	// �����ŕX�j�󎞂̃G�t�F�N�g�𔭐�
	D3DXVECTOR3 posIce = pIce->GetPosition();
	CParticle::Create(posIce, CParticle::TYPE::TYPE_ICEBREAK);

	// �X�j�Ђ𐶐�
	for (int i = 0; i < CIceStaeteBreak::NUM_ICE_BREAK; i++)
	{
		CObjectX *pPeace = CObjectX::Create();

		// ���f���̊��蓖��
		int nIdxModel = CModel::Load((char*)&PATH_ICE_DEBRIS[0]);
		pPeace->BindModel(nIdxModel);

		// �ʒu�������_���ݒ�
		D3DXVECTOR3 posPeace = posIce;
		posIce.x += (float)universal::RandRange((int)(Grid::SIZE * 0.25f), -(int)(Grid::SIZE * 0.25f));
		posIce.z += (float)universal::RandRange((int)(Grid::SIZE * 0.25f), -(int)(Grid::SIZE * 0.25f));

		pPeace->SetPosition(posIce);

		// �X�P�[���������_���ݒ�
		float fRand = universal::RandRange(MAX_SCALE, MIN_SCALE) * 0.1f;
		pPeace->SetScale(fRand);

		// ��]
		float fRotY = universal::RandRange(314, 0) * 0.01f;
		pPeace->SetRotation(D3DXVECTOR3(0.0f, fRotY, 0.0f));

		// �z��ɓ����
		m_aPeaceIce.push_back(pPeace);
	}

	// �J�E���^�[��������
	m_fTimerRepair = TIME_REPAIR_ICE;
}

//=====================================================
// �I������
//=====================================================
void CIceStaeteBreak::Uninit(CIce *pIce)
{
	for (auto it : m_aPeaceIce)
		it->Uninit();

	m_aPeaceIce.clear();
}

//=====================================================
// �X�V����
//=====================================================
void CIceStaeteBreak::Update(CIce *pIce)
{
	// �X�̏C���̍X�V
	UpdateRepair(pIce);

	// �X�̒Ǐ]
	FollowIce(pIce);
}

//=====================================================
// �C���̍X�V
//=====================================================
void CIceStaeteBreak::UpdateRepair(CIce *pIce)
{
	// ����ł����ԂȂ�ʂ�Ȃ�
	if (pIce->IsSink())
		return;

	// �^�C�}�[�����炷
	m_fTimerRepair -= CManager::GetDeltaTime();

	if (m_fTimerRepair <= 0.0f)
	{// ��莞�Ԍo�߂ŁA�X���C������
		pIce->CreateMesh();

		pIce->ChangeState(new CIceStaeteNormal);
		pIce->EnablePeck(false);

		// �p�[�e�B�N������
		CParticle::Create(pIce->GetPosition(), CParticle::TYPE::TYPE_REPAIRICE);
	}
}

//=====================================================
// �X�̒Ǐ]
//=====================================================
void CIceStaeteBreak::FollowIce(CIce *pIce)
{
	// �X�̒Ǐ]
	for (auto it : m_aPeaceIce)
	{
		D3DXVECTOR3 pos = { it->GetPosition().x, pIce->GetPosition().y, it->GetPosition().z };

		it->SetPosition(pos);
	}

	// ���ޏ�Ԃ̎��A�{�̂̈ʒu��������
	if (pIce->IsSink())
	{
		D3DXVECTOR3 posIce = pIce->GetPosition();
		posIce.y -= SPEED_SINK;
		pIce->SetPosition(posIce);

		if (posIce.y < HEIGHT_DELETE)
		{// ���܂Œ��񂾂�폜����
			pIce->Uninit();

			return;
		}
	}
}

//*******************************************************************************
// �����X�e�C�g
//*******************************************************************************
//=====================================================
// ����������
//=====================================================
void CIceStateFlow::Init(CIce *pIce)
{
	pIce->EnableStop(false);

	// ���g�̃|�C���^���O���b�h����O��
	CIceManager *pIceMgr = CIceManager::GetInstance();

	if (pIceMgr == nullptr)
		return;

	pIceMgr->DeleteIce(pIce);
}

//=====================================================
// �I������
//=====================================================
void CIceStateFlow::Uninit(CIce *pIce)
{

}

//=====================================================
// �X�V����
//=====================================================
void CIceStateFlow::Update(CIce *pIce)
{
	if (!m_bDrift)
		UpdateSearchIce(pIce);	// �X��T���Ă��鎞�̍X�V
	else
		UpdateDriftIce(pIce);	// �Y�����Ă�Ƃ��̍X�V
}

//=====================================================
// �X��T���Ă���Ƃ��̍X�V
//=====================================================
void CIceStateFlow::UpdateSearchIce(CIce *pIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// �C���̃x�N�g���擾
	COcean::E_Stream dir = pIceManager->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// ����鑬�x�ɐ��K�����Ĉʒu�����Z
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow;
	pIce->Translate(vecStream);

	// �X�Ƃ̔���
	CollideIce(pIce);
}

//=====================================================
// �Y������Ƃ��̍X�V
//=====================================================
void CIceStateFlow::UpdateDriftIce(CIce *pIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// �O���b�h�̈ʒu�擾
	D3DXVECTOR3 posDrift = pIceManager->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);

	// �C���̃x�N�g���擾
	D3DXVECTOR3 posIce = pIce->GetPosition();
	if (pIceManager == nullptr)
		return;

	// �C���̃x�N�g���擾
	D3DXVECTOR3 vecDiff = posDrift - posIce;

	// ����鑬�x�ɐ��K�����Ĉʒu�����Z
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecDiff, &vecDiff);
	vecDiff *= fSpeedFlow;
	pIce->Translate(vecDiff);

	// �O���b�h�̈ʒu�Ƃ̋������������l�����������~�߂�
	bool bStop = universal::DistCmpFlat(posIce, posDrift, LINE_STOP_ICE, nullptr);

#ifdef _DEBUG
	//CEffect3D::Create(posIce, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
#endif

	if (bStop)
	{
		return;
	}
}

//=====================================================
// �X�Ƃ̔���
//=====================================================
void CIceStateFlow::CollideIce(CIce *pIce)
{
	// ������O���b�h�̎擾
	int nIdxV = -1;
	int nIdxH = -1;

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	D3DXVECTOR3 posIce = pIce->GetPosition();
	bool bOk = pIceManager->GetIdxGridFromPosition(posIce, &nIdxV, &nIdxH);

	// �ԍ���ۑ�
	m_nIdxDriftV = nIdxV;
	m_nIdxDriftH = nIdxH;

	// �C���̕����ɍ��킹������֐��̌Ăяo��
	DirectionFunc directionFuncs[COcean::E_Stream::STREAM_MAX] = 
	{
		&CIceStateFlow::CheckUp,
		&CIceStateFlow::CheckRight,
		&CIceStateFlow::CheckDown,
		&CIceStateFlow::CheckLeft
	};

	COcean::E_Stream stream = pIceManager->GetDirStream();
	
	// �Y������X����������A�t���O�𗧂ĂĕY���O���b�h�ԍ���ۑ�
	if (m_nIdxDriftV == -1 || m_nIdxDriftH == -1)
		return;

	if (pIceManager->GetGridIce(&nIdxV, &nIdxH) != nullptr)
	{
		pIce->ChangeState(new CIceStaeteNormal);
		return;
	}

	vector<CIce*> apIceHit;
	m_bDrift = (this->*directionFuncs[stream])(pIce, m_nIdxDriftV, m_nIdxDriftH, apIceHit);

#ifdef _DEBUG
	D3DXVECTOR3 posEffect = pIceManager->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);
	debug::Effect3DShort(posEffect, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
#endif

	if (m_bDrift)
	{
		debug::Effect3DShort(pIce->GetPosition(), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 120);
		// �O���b�h�ɕX����ۑ�
		if (pIceManager->SetIceInGrid(nIdxV, nIdxH, pIce))
		{
			for (CIce* pIce : apIceHit)
			{
				if (pIce == nullptr)
					continue;

				// �p�[�e�B�N���𔭐�
				D3DXVECTOR3 pos = pIce->GetPosition();
				D3DXVECTOR3 posHitIce = pIce->GetPosition();
				D3DXVECTOR3 vecDIff = posHitIce - pos;

				pos += vecDIff * 0.5f;

				CParticle::Create(pos, CParticle::TYPE::TYPE_STICK_ICE);
			}
		}

		pIce->ChangeState(new CIceStaeteNormal);
	}
}

//=====================================================
// ������̊m�F
//=====================================================
bool CIceStateFlow::CheckUp(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// ���ӂ̕X�̎擾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// �㑤�̃O���b�h�ǂꂩ�ɕX������ΕY��
	if (apIce[CIceManager::DIRECTION_LEFTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHTUP] != nullptr)
	{
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFTUP]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHTUP]);

		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// �E�����̊m�F
//=====================================================
bool CIceStateFlow::CheckRight(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// ���ӂ̕X�̎擾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// �E���̃O���b�h�ǂꂩ�ɕX������ΕY��
	if (apIce[CIceManager::DIRECTION_RIGHTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHT] != nullptr ||
		apIce[CIceManager::DIRECTION_RIGHTDOWN] != nullptr)
	{
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHTUP]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHT]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHTDOWN]);

		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// �������̊m�F
//=====================================================
bool CIceStateFlow::CheckDown(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// ���ӂ̕X�̎擾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// �����̃O���b�h�ǂꂩ�ɕX������ΕY��
	if (apIce[CIceManager::DIRECTION_RIGHTDOWN] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFTDOWN] != nullptr)
	{
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_RIGHTDOWN]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFTDOWN]);

		bDrift = true;
	}

	return bDrift;
}

//=====================================================
// �������̊m�F
//=====================================================
bool CIceStateFlow::CheckLeft(CIce *pIce, int nIdxV, int nIdxH, vector<CIce*> &rpHitIce)
{
	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return false;

	// ���ӂ̕X�̎擾
	vector<CIce*> apIce = pIceManager->GetAroundIce(nIdxV, nIdxH);

	bool bDrift = false;

	// �����̃O���b�h�ǂꂩ�ɕX������ΕY��
	if (apIce[CIceManager::DIRECTION_LEFTUP] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFT] != nullptr ||
		apIce[CIceManager::DIRECTION_LEFTDOWN] != nullptr)
	{
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFTUP]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFT]);
		rpHitIce.push_back(apIce[CIceManager::DIRECTION_LEFTDOWN]);

		bDrift = true;
	}

	return bDrift;
}

//*******************************************************************************
// ���ރX�e�C�g
//*******************************************************************************
//=====================================================
// ����������
//=====================================================
void CIceStaeteSink::Init(CIce *pIce)
{

}

//=====================================================
// �I������
//=====================================================
void CIceStaeteSink::Uninit(CIce *pIce)
{

}

//=====================================================
// �X�V����
//=====================================================
void CIceStaeteSink::Update(CIce *pIce)
{

}