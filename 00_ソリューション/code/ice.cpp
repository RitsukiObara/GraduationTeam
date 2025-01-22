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
#include "collision.h"
#include "game.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const string PATH_TEX = "data\\TEXTURE\\MATERIAL\\ice001.jpg";					// �e�N�X�`���p�X
const string PATH_TEX_OVERRAY = "data\\TEXTURE\\MATERIAL\\iceanimation.jpg";	// �I�[�o���C�e�N�X�`���p�X
const string PATH_TEX_RIPPLE = "data\\TEXTURE\\EFFECT\\ice002.png";				// �����g�̃e�N�X�`���p�X
const float SIZE_INIT = 100.0f;									// �����T�C�Y
const float SIZE_COLLIDE = 200.0f;								// ����T�C�Y�̏����l
const float HEIGHT_ICE = 50.0f;									// �X�̍���
const int NUM_CORNER = 6;										// �p�̐�
const D3DXVECTOR3 ROT_UP_INIT = { D3DX_PI * 0.5f,0.0f,0.0f };	// �㑤�̏�������

const float TIME_REPAIR_ICE = 13.0f;									// �X�̏C���ɂ����鎞��
const int MAX_SCALE = 50;												// �X�P�[���̍ő�l
const int MIN_SCALE = 20;												// �X�P�[���̍ŏ��l
const string PATH_ICE_DEBRIS = "data\\MODEL\\block\\Drift_ice_piece.x";	// �j�ЕX�̃��f���p�X
const float SPEED_SINK = 5.0f;											// ���ޑ��x
const float HEIGHT_DELETE = -100.0f;									// �폜����܂ł̍���

const float HEIGHT_DEFAULT_FROM_OCEAN = 50.0f;		// �C����̃f�t�H���g�̍���
const float HEIGHT_NORMALSINK_FROM_OCEAN = 10.0f;	// �C����̒ʏ풾�ލ���

const float LINE_STOP_ICE = 1.0f;	// �X���~�܂邵�����l
const float TIME_MAXSPEED = 10.0f;	// �ő呬�x�ɂȂ�܂ł̎���
const float RATE_COLLISION = 1.0f;	// ����̊���
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

const int MAX_TIME_SMALL = 60;	// �����ɂ�����ő厞��
const int MIN_TIME_SMALL = 50;	// �����ɂ�����ŏ�����

const float SPEED_FOLLOW = 0.23f;	// �Ǐ]���x
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
m_pSide(nullptr),m_pUp(nullptr), m_pState(nullptr), m_bSink(false), m_bStop(false), m_abRipleFrag(), m_nCntAnimFlash(0), m_rotDest(), m_fHeightOcean(0.0f),
m_pCollision(nullptr), m_pRipple(nullptr)
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
CIce *CIce::Create(E_Type type,E_State state, int nIdxV, int nIdxH)
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
			pIce = new CIceHardMulti;
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
			CIceManager *pIceMgr = CIceManager::GetInstance();
			if (pIceMgr != nullptr && nIdxV != -1 && nIdxH != -1)
				pIceMgr->SetIceInGrid(nIdxV, nIdxH, pIce);

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
	// ��̃��b�V��
	if (m_pUp == nullptr)
	{
		m_pUp = CFan3D::Create(3, NUM_CORNER);

		if (m_pUp != nullptr)
		{
			int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
			m_pUp->SetIdxTexture(nIdxTexture);

			int nIdxTextureOverray = Texture::GetIdx(&PATH_TEX_OVERRAY[0]);
			m_pUp->SetIdxTextureOverRay(nIdxTextureOverray);
			m_pUp->SetVtx();
		}
	}

	// �����g�̐���
	CreateRipple();

	// ���ʂ̃|���S���̐���
	if (m_pSide == nullptr)
	{
		m_pSide = CMeshCylinder::Create(NUM_CORNER);
		int nIdxTexture = Texture::GetIdx(&PATH_TEX[0]);
		m_pSide->SetIdxTexture(nIdxTexture);
	}

	// �g�����X�t�H�[���̐ݒ�
	SetTransform(Grid::SIZE);
}

//=====================================================
// ���b�V���̔j��
//=====================================================
void CIce::DeleteMesh(void)
{
	Object::DeleteObject((CObject**)&m_pUp);
	Object::DeleteObject((CObject**)&m_pRipple);
	Object::DeleteObject((CObject**)&m_pSide);
}

//=====================================================
// ����̐���
//=====================================================
void CIce::CreateCollision(void)
{
	if (m_pCollision != nullptr)
		return;

	m_pCollision = CCollisionSphere::Create(CCollision::TAG_BLOCK, CCollision::TYPE::TYPE_SPHERE, this);

	if (m_pCollision == nullptr)
		return;

	// ���菉���ݒ�
	m_pCollision->SetRadius(RATE_COLLISION * SIZE_INIT);
	m_pCollision->SetPosition(GetPosition());
}

//=====================================================
// ����̔j��
//=====================================================
void CIce::DeleteCollision(void)
{
	Object::DeleteObject((CObject**)&m_pCollision);
}

//=====================================================
// �����g�̐���
//=====================================================
void CIce::CreateRipple(void)
{
	if (m_pRipple == nullptr)
	{
		m_pRipple = CFan3D::Create(4, NUM_CORNER);

		if (m_pRipple != nullptr)
		{
			int nIdxTexture = Texture::GetIdx(&PATH_TEX_RIPPLE[0]);
			m_pRipple->SetIdxTexture(nIdxTexture);

			m_pRipple->SetVtx();
		}
	}
}

//=====================================================
// �����g�̔j��
//=====================================================
void CIce::DeleteRipple(void)
{
	Object::DeleteObject((CObject**)&m_pRipple);
}

//=====================================================
// �I������
//=====================================================
void CIce::Uninit(void)
{
	// ���b�V���̍폜
	DeleteMesh();

	// ����폜
	DeleteCollision();

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
	// �X�e�C�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);

	// �g�ɒǏ]���鏈��
	FollowWave();

	// �X���̏���
	Tilt();

	// �����g�̏���
	Ripples();
	SmallRipples();

	// ���b�V���̒Ǐ]
	FollowMesh();

	// ���鏈��
	Flash();

	// �p���N���X�̍X�V
	CObject3D::Update();
}

//=====================================================
// ��̃I�u�W�F�N�g�𓮂���
//=====================================================
void CIce::MoveObjectOnIce(D3DXVECTOR3 vecMove)
{
	vector<CGameObject*> apObject;
	GetOnTopObject(apObject);

	// ��ɂǂꂩ������Ă��瓮����
	for (CGameObject* object : apObject)
	{
		if (object == nullptr)
			continue;

		D3DXVECTOR3 posObject = object->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		if (universal::DistCmpFlat(pos, posObject, SIZE_INIT, nullptr))
			object->Translate(vecMove); // ����������Ă�̂œ�����
	}
}

//=====================================================
// �g�ɒǏ]���鏈��
//=====================================================
void CIce::FollowWave(void)
{
	if (IsSink())
		return;

	COcean *pOcean = COcean::GetInstance();

	if (pOcean == nullptr)
		return;

	// �C�ƈꏏ�ɕX�𓮂�������
	D3DXVECTOR3 pos = GetPosition();

	pos.y = pOcean->GetHeight(pos, nullptr) + HEIGHT_ICE + m_fHeightOcean;

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

	GetOnTopObject(apObject,1.2f);

	// ��ɂǂꂩ������Ă���^��Ԃ�
	for (CGameObject* object : apObject)
	{
		D3DXVECTOR3 posObject = object->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		if (universal::DistCmpFlat(pos, posObject, SIZE_COLLIDE, nullptr))
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
// �����������g�̏���
//=====================================================
void CIce::SmallRipples(void)
{
	D3DXVECTOR3 posEffect = GetPosition();

	// �^�C�}�[�ł̂����~��
	m_fTimerSmallRipples += 1.0f;

	if (m_fTimerSmallRipples <= m_fSpawnTimeSmallRipples)
		return;

	// �����̃����_���ݒ�
	float fAngle = universal::GetRandomDirection();

	// �G�t�F�N�g�̔���
	posEffect.x += sinf(fAngle) * Grid::SIZE * 0.5f;
	posEffect.z += cosf(fAngle) * Grid::SIZE * 0.5f;
	posEffect.y -= HEIGHT_ICE;
	//CParticle::Create(posEffect, CParticle::TYPE::TYPE_SMALLRIPLE);

	// �^�C�}�[�̍Đݒ�
	m_fTimerSmallRipples = 0.0f;

	int nRand = universal::RandRange(ripple::MAX_TIME_SMALL, ripple::MIN_TIME_SMALL);

	m_fSpawnTimeSmallRipples = (float)nRand;
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

	if (m_pRipple != nullptr)
	{
		// �����g�̃|���S���̒Ǐ]
		D3DXVECTOR3 posDest = GetPosition();
		posDest.y -= HEIGHT_ICE - m_fHeightOcean;

		D3DXVECTOR3 posRipple = m_pRipple->GetPosition();
		universal::MoveToDest(&posRipple, posDest, ripple::SPEED_FOLLOW);
		m_pRipple->SetPosition(posRipple);
		m_pRipple->SetVtx();

		D3DXVECTOR3 rotRipple = { 0.0f,GetRotation().y,0.0f };
		m_pRipple->SetRotation(rotRipple);
	}
}

//=====================================================
// ����̒Ǐ]
//=====================================================
void CIce::FollowCollision(void)
{

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

	// �����g�|���S���̐ݒ�
	D3DXVECTOR3 posRipple = GetPosition();
	posRipple.y -= HEIGHT_ICE;
	m_pRipple->SetPosition(posRipple);
	m_pRipple->SetRadius(fRadius * 0.7f);
	m_pRipple->SetVtx();

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

//=====================================================
// ����n�߂̃^�C�}�[�Z�b�g
//=====================================================
void CIce::SetTimerStartMove(float fTime)
{
	if (m_pState != nullptr)
		m_pState->SetTimerStartMove(fTime);
}

//=====================================================
// ����n�߂̃^�C�}�[�擾
//=====================================================
float CIce::GetTimerStartMove(void)
{
	float fTime = 0.0f;

	if (m_pState != nullptr)
		fTime = m_pState->GetTimerStartMove();

	return fTime;
}

//=====================================================
// �ł��߂��X�̎擾
//=====================================================
CIce *CIce::GetNearestIce(D3DXVECTOR3 pos)
{
	CIce *pIceNear = nullptr;

	float fLenghtMin = FLT_MAX;
	for (CIce *pIce : s_Vector)
	{
		D3DXVECTOR3 posIce = pIce->GetPosition();

		float fDiff = 0.0f;
		if (universal::DistCmpFlat(pos, posIce, fLenghtMin, &fDiff))
		{
			pIceNear = pIce;
			fLenghtMin = fDiff;
		}
	}

	return pIceNear;
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

		D3DXVECTOR3 pos = pIce->GetPosition();

		pIceMgr->GetNearestEmptyGrid(pos, &m_nIdxDriftV, &m_nIdxDriftH, pIce);

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
	pIce->MoveObjectOnIce(vecDiff);
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
	// �X��T���Ă��鎞�̍X�V
	UpdateSearchIce(pIce);

	// ���̗��X�Ƃ̔���
	CollideOtherFlow(pIce);
}

//=====================================================
// �X��T���Ă���Ƃ��̍X�V
//=====================================================
void CIceStateFlow::UpdateSearchIce(CIce *pIce)
{
	// �Q�[�����ɂ̒ʏ��Ԃ̂ݍX�V���ʂ�
	if (CGame::GetInstance() != nullptr &&
		CGame::GetState() != CGame::E_State::STATE_NORMAL)
		return;

	CIceManager *pIceManager = CIceManager::GetInstance();

	if (pIceManager == nullptr)
		return;

	// �C���̃x�N�g���擾
	COcean::E_Stream dir = pIceManager->GetDirStream();
	D3DXVECTOR3 vecStream = stream::VECTOR_STREAM[dir];

	// �����オ��̑��x�̊������v�Z
	m_fTimerStartMove += CManager::GetDeltaTime();

	float t = m_fTimerStartMove / TIME_MAXSPEED;
	float fRate = easing::EaseOutExpo(t);
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	// ����鑬�x�ɐ��K�����Ĉʒu�����Z
	float fSpeedFlow = pIceManager->GetOceanLevel();
	D3DXVec3Normalize(&vecStream, &vecStream);
	vecStream *= fSpeedFlow * fRate;
	pIce->Translate(D3DXVECTOR3(vecStream.x, 0.0f, vecStream.z));

	pIce->MoveObjectOnIce(vecStream);

	m_vecStream = vecStream;

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
	pIce->Translate(D3DXVECTOR3(vecDiff.x,0.0f,vecDiff.z));

	m_vecStream = vecDiff;

	// �O���b�h�̈ʒu�Ƃ̋������������l�����������~�߂�
	bool bStop = universal::DistCmpFlat(posIce, posDrift, LINE_STOP_ICE, nullptr);

#ifdef _DEBUG
	//CEffect3D::Create(posIce, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
#endif

	if (bStop)
		return;
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

	// �O���b�h�����̃`�F�b�N
	if (!pIceManager->GetIdxGridFromPosition(pIce->GetPosition(), &nIdxV, &nIdxH, 1.0f))
		return;

	if (pIceManager->GetGridIce(&nIdxV, &nIdxH) != nullptr)
	{// ���ɂ��̏�ɕX����������ʏ��ԂɈڍs
		pIceManager->AddIce(pIce, pIce->GetPosition());
		pIce->ChangeState(new CIceStaeteNormal);
		return;
	}

	D3DXVECTOR3 posIce = pIce->GetPosition();
	pIceManager->GetNearestEmptyGrid(posIce - m_vecStream, &nIdxV, &nIdxH);

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
	
	vector<CIce*> apIceHit;
	bool bHit = (this->*directionFuncs[stream])(pIce, m_nIdxDriftV, m_nIdxDriftH, apIceHit);

#ifdef _DEBUG
	D3DXVECTOR3 posEffect = pIceManager->GetGridPosition(&m_nIdxDriftV, &m_nIdxDriftH);
	debug::Effect3DShort(posEffect, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
#endif

	if (bHit)
	{
		// �������t���O
		bool bStick = false;

		// �O���b�h�ɕX����ۑ�
		if (pIceManager->CheckIceInGrid(nIdxV, nIdxH, pIce))
		{
			for (CIce* pIceHit : apIceHit)
			{
				if (pIceHit == nullptr)
					continue;
				
				// �p�[�e�B�N���𔭐�
				D3DXVECTOR3 pos = pIce->GetPosition();
				D3DXVECTOR3 posHitIce = pIceHit->GetPosition();
				D3DXVECTOR3 vecDIff = posHitIce - pos;

				pos += vecDIff * 0.5f;

				CParticle::Create(pos, CParticle::TYPE::TYPE_STICK_ICE);

				if (pIceHit->IsPeck())
				{
					pIceManager->DeleteIce(pIceHit);
					pIceHit->EnableSink(true);

					// ���݃p�[�e�B�N���̔���
					CParticle::Create(posHitIce, CParticle::TYPE::TYPE_BUBBLE_SINK);
				}
				else
				{// ��ł��ʏ�X���������炭�����t���O�𗧂Ă�
					bStick = true;
				}
			}
		}

		if (bStick)
		{
			pIceManager->AddIce(pIce,pIce->GetPosition());
			pIce->ChangeState(new CIceStaeteNormal);
		}
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

//=====================================================
// ���̗��X�Ƃ̔���
//=====================================================
void CIceStateFlow::CollideOtherFlow(CIce *pIceOwn)
{
	vector<CIce*> apIce = CIce::GetInstance();

	// �~�܂��Ă�X�����O
	universal::RemoveIfFromVector(apIce, [](CIce* ice) { return ice != nullptr && ice->IsStop(); });

	D3DXVECTOR3 pos = pIceOwn->GetPosition();

	for (CIce *pIce : apIce)
	{
		D3DXVECTOR3 posTarget = pIce->GetPosition();

		if (universal::DistCmpFlat(pos, posTarget, Grid::SIZE, nullptr))
		{// ��苗���܂ŋ߂Â��Ă����炻�̑��x�̒��Ԃ̒l�ɂ���
			float fTimeTarget = pIce->GetTimerStartMove();
			float fTime = pIceOwn->GetTimerStartMove();

			fTime += (fTimeTarget - fTime) * 0.5f;

			pIce->SetTimerStartMove(fTime);
			pIceOwn->SetTimerStartMove(fTime);
		}
	}
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