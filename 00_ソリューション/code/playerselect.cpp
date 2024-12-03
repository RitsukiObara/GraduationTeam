//*****************************************************
//
// �v���C���[�I������[playerselect.cpp]
// Author:�x�씋��
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "manager.h"
#include "playerselect.h"
#include "inputkeyboard.h"
#include "inputManager.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "debugproc.h"
#include "UIManager.h"
#include "texture.h"
#include "meshfield.h"
#include "CameraState.h"
#include "player.h"
#include "ocean.h"
#include "gameManager.h"
#include "meshcylinder.h"
#include "fan3D.h"
#include "particle.h"
#include "inputjoypad.h"
#include "collision.h"
#include "shadow.h"
#include "polygon3D.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SIZE_UI_PLAYERNUMBER = 0.05f;	// �v���C���[�i���o�[UI�̃T�C�Y
const float HEIGHT_UI_PLAYERNUMBER = 0.8f;	// �v���C���[�i���o�[UI�̈ʒu�̍���

const string PATH_UI_STANDBY = "data\\TEXTURE\\UI\\standby.png";	// �X�^���h�o�C�e�N�X�`���̃p�X
const string PATH_UI_READY = "data\\TEXTURE\\UI\\ready.png";	// ���������e�N�X�`���̃p�X
const string PATH_BANNER = "data\\MODEL\\other\\entry_banner.x";	// �Ŕ̃��f���p�X
const string PATH_PLAYERNUM = "data\\TEXTURE\\UI\\player_Count.png";	// �v���C���[�ԍ��e�N�X�`���p�X
const D3DXVECTOR3 BANNER_POS = D3DXVECTOR3(0.0f, 0.0f, 500.0f);	// �Ŕ̏����̈ʒu
const D3DXVECTOR3 BANNER_ROT = D3DXVECTOR3(0.1f, 0.0f, 0.07f);		// �Ŕ̏����̌���
const float BANNER_SCALE = 6.5f;	// �Ŕ̃T�C�Y
const float BANNER_COLLISION_SIZE = 20.0f;

const D3DXVECTOR3 INIT_MOVE_PLAYER = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	// �v���C���[�̏����̈ړ���
const D3DXVECTOR3 POS_PLAYER_INIT = D3DXVECTOR3(0.0f, -200.0f, 0.0f);		// �v���C���[�̏����̈ʒu

const float PLAYERNUM_SIZE = 80.0f;
const float PLAYERNUM_POS_Y = 250.0f;

const float GRAVITY = 0.6f;	// �d��

const float POW_VIB_APPER = 0.6f;	// �o�����̐U������
const int TIME_VIB_APPER = 15;		// �o�����̐U������

//----------------------------------
// �X�e�[�W�̒萔
//----------------------------------
namespace stage
{
const float RADIUS = 1000.0f;				// ���a
const float RADIUS_COLLIDE = RADIUS * 0.9f;	// ����̔��a
const int PRIOLITY = 4;						// �D�揇��
const int NUM_VTX = 32;						// ���_�̐�
const float HEIGHT = 100.0f;				// ����
const string PATH_TEX_FAN = "data\\TEXTURE\\MATERIAL\\field.jpg";			// �~�̃e�N�X�`���p�X
const string PATH_TEX_CYLINDER = "data\\TEXTURE\\MATERIAL\\small_ice.png";	// �~���̃e�N�X�`���p�X
}
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************

//=====================================================
// �R���X�g���N�^
//=====================================================
CPlayerSelect::CPlayerSelect()
{
	m_nNumPlayer = 0;
	ZeroMemory(&m_StandbyState[0], sizeof(m_StandbyState));
	ZeroMemory(&m_apPlayerUI[0], sizeof(m_apPlayerUI));
	ZeroMemory(&m_apPlayer[0], sizeof(m_apPlayer));
	ZeroMemory(&m_apInputMgr[0], sizeof(m_apInputMgr));
	ZeroMemory(&m_apBillboard[0], sizeof(m_apBillboard));
	m_pCylinder = nullptr;
	m_pFan = nullptr;
	m_pCollisionSphere = nullptr;
	m_pShadow = nullptr;
}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayerSelect::Init(void)
{
	// �e�N���X�̏�����
	CScene::Init();

	// UI�}�l�[�W���[�̒ǉ�
	CUIManager::Create();

	Camera::ChangeState(new CFollowPlayer);

	// �e�N�X�`���ԍ��擾
	int nIdx[2]; 
	nIdx[0] = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\player_Count.png");
	nIdx[1]= CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\Abutton.png");

	float fRateOnePlayer = 1.0f / MAX_PLAYER;

	// UI����
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		// �v���C���[�i���o�[UI�̐���
		m_apPlayerUI[nCount] = CUI::Create();
		if (m_apPlayerUI[nCount] != nullptr)
		{
			m_apPlayerUI[nCount]->SetSize(SIZE_UI_PLAYERNUMBER, SIZE_UI_PLAYERNUMBER);
			m_apPlayerUI[nCount]->SetPosition(D3DXVECTOR3(0.1f + fRateOnePlayer * nCount, HEIGHT_UI_PLAYERNUMBER, 0.0f));
			m_apPlayerUI[nCount]->SetTex(D3DXVECTOR2(0.0f + fRateOnePlayer * nCount, 0.0f), D3DXVECTOR2(fRateOnePlayer + fRateOnePlayer * nCount, 1.0f));
			m_apPlayerUI[nCount]->SetIdxTexture(nIdx[0]);
			m_apPlayerUI[nCount]->SetVtx();
		}

		// �G���g���[UI�̐���
		m_apStateUI[nCount] = CUI::Create();
		if (m_apStateUI[nCount] != nullptr)
		{
			m_apStateUI[nCount]->SetSize(0.07f, 0.05f);
			m_apStateUI[nCount]->SetPosition(D3DXVECTOR3(0.1f + fRateOnePlayer * nCount, 0.9f, 0.0f));
			m_apStateUI[nCount]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_apStateUI[nCount]->SetIdxTexture(nIdx[1]);
			m_apStateUI[nCount]->SetVtx();
		}

		// ���̓}�l�[�W���[�̐���
		CInputManager *pInputMgr = CInputManager::Create();
		m_apInputMgr[nCount] = pInputMgr;
	}

	// ���b�V���̐���
	CreateMesh();

	// �C�̐���
	COcean::Create();

	// �Ŕz�u
	CObjectX* pBanner = CObjectX::Create(BANNER_POS, BANNER_ROT, 5);
	if (pBanner != nullptr)
	{
		pBanner->BindModel(CModel::Load(&PATH_BANNER[0]));	// ���f���ǂݍ���Őݒ�
		pBanner->SetScale(BANNER_SCALE);	// �T�C�Y�ݒ�

		//// ���̔��萶��
		//m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG::TAG_PLAYER, CCollision::TYPE::TYPE_SPHERE, pBanner);

		//if (m_pCollisionSphere != nullptr)
		//{
		//	m_pCollisionSphere->SetRadius(BANNER_COLLISION_SIZE);
		//	m_pCollisionSphere->SetPosition(D3DXVECTOR3(BANNER_POS.x, 50.0f, BANNER_POS.z + 141.0f));
		//}

		// �e�̐���
		m_pShadow = CShadow::Create(4);
		if (m_pShadow != nullptr)
		{
			m_pShadow->SetPosition(D3DXVECTOR3(BANNER_POS.x, 0.0f, BANNER_POS.z + 141.0f));
		}
	}

	return S_OK;
}

//=====================================================
// ���b�V���̐���
//=====================================================
void CPlayerSelect::CreateMesh(void)
{
	m_pFan = CFan3D::Create(stage::PRIOLITY, stage::NUM_VTX);

	if (m_pFan == nullptr)
		return;

	m_pFan->SetPosition(D3DXVECTOR3(0.0f, stage::HEIGHT, 0.0f));
	m_pFan->SetRadius(stage::RADIUS);
	m_pFan->SetVtx();

	int nIdxTexture = Texture::GetIdx(&stage::PATH_TEX_FAN[0]);
	m_pFan->SetIdxTexture(nIdxTexture);

	m_pCylinder = CMeshCylinder::Create(stage::NUM_VTX);

	if (m_pCylinder == nullptr)
		return;

	m_pCylinder->SetHeight(stage::HEIGHT);
	m_pCylinder->SetRadius(stage::RADIUS);
	m_pCylinder->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pCylinder->SetVtx();

	nIdxTexture = Texture::GetIdx(&stage::PATH_TEX_CYLINDER[0]);
	m_pCylinder->SetIdxTexture(nIdxTexture);
}

//=====================================================
// �I������
//=====================================================
void CPlayerSelect::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pCylinder);
	Object::DeleteObject((CObject**)&m_pFan);
	Object::DeleteObject((CObject**)&m_pShadow);
	//Object::DeleteObject((CObject**)&m_pCollisionSphere);

	// �e��I�u�W�F�N�g�̔j��
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apStateUI[nCount] != nullptr)
		{
			m_apStateUI[nCount]->Uninit();
			m_apStateUI[nCount] = nullptr;
		}
		if (m_apPlayerUI[nCount] != nullptr)
		{
			m_apPlayerUI[nCount]->Uninit();
			m_apPlayerUI[nCount] = nullptr;
		}
		if (m_apPlayer[nCount] != nullptr)
		{
			m_apPlayer[nCount]->Uninit();
			m_apPlayer[nCount] = nullptr;
		}
		if (m_apBillboard[nCount] != nullptr)
		{
			m_apBillboard[nCount]->Uninit();
			m_apBillboard[nCount] = nullptr;
		}
	}

	// �I�u�W�F�N�g�S��
	CObject::ReleaseAll();

	CScene::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CPlayerSelect::Update(void)
{
	CFade* pFade = CFade::GetInstance();
	
	// �V�[���̍X�V
	CScene::Update();

	// ����
	Input();

	// �v���C���[�̈ʒu����
	LimitPlayerPos();

	// �v���C���[�̏d�͏���
	GravityPlayer();

	// �J�n���邩�̊m�F
	CheckStart();

	// �v���C���[�ԍ��r���{�[�h�ʒu�X�V
	UpdateBillboard();
	
#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// ����
//=====================================================
void CPlayerSelect::Input(void)
{
	// �t�F�[�h�̊J�n
	CFade* pFade = CFade::GetInstance();

	CInputManager* pInput = CInputManager::GetInstance();

	if (pInput == nullptr)
		return;

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (m_apInputMgr[i] == nullptr)
			continue;

		if (m_apInputMgr[i]->GetTrigger(CInputManager::E_Button::BUTTON_ENTER))
			CreatePlayer(i);	// �v���C���[�̃G���g���[

		if (m_apPlayer[i] != nullptr &&
			m_apInputMgr[i]->GetTrigger(CInputManager::E_Button::BUTTON_READY))
			Ready(i);	// ����

		if (m_apInputMgr[i]->GetTrigger(CInputManager::BUTTON_BACK))	// BACK�������Ƃ�
		{
			pFade->SetFade(CScene::MODE::MODE_SELECTMODE);
		}
	}
}

//=====================================================
// �v���C���[�̈ʒu����
//=====================================================
void CPlayerSelect::LimitPlayerPos(void)
{
	for (CPlayer *pPlayer : m_apPlayer)
	{
		if (pPlayer == nullptr)
			continue;

		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

		universal::LimitDistCylinderInSide(stage::RADIUS_COLLIDE, &posPlayer, D3DXVECTOR3(0.0f, posPlayer.y, 0.0f));

		pPlayer->SetPosition(posPlayer);

		posPlayer.y = stage::HEIGHT;	// �e�̈ʒu���X�e�[�W�̍����ɌŒ�
		pPlayer->SetShadowPos(posPlayer);
	}
}

//=====================================================
// �v���C���[�̏d�͏���
//=====================================================
void CPlayerSelect::GravityPlayer(void)
{
	for (CPlayer *pPlayer : m_apPlayer)
	{
		if (pPlayer == nullptr)
			continue;

		// �ړ��ʂ̉��Z
		D3DXVECTOR3 move = pPlayer->GetMove();
		move.y -= GRAVITY;
		pPlayer->SetMove(move);

		// ���Ƃ̔���
		D3DXVECTOR3 pos = pPlayer->GetPosition();

		if (pos.y <= stage::HEIGHT && move.y < 0)
		{
			pos.y = stage::HEIGHT;
			pPlayer->SetMove(D3DXVECTOR3(move.x, 0.0f, move.z));
		}

		pPlayer->SetPosition(pos);
	}
}

//=====================================================
// �v���C���[�̐���
//=====================================================
void CPlayerSelect::CreatePlayer(int nIdx)
{
	if (m_apPlayer[nIdx] != nullptr)
		return;	// �g�����܂��Ă��珈����ʂ�Ȃ�

	CSound* pSound = CSound::GetInstance();

	if (pSound == nullptr)
		return;

	// �y���M���G���g���[��
	pSound->Play(CSound::LABEL_SE_PENGUIN_VOICE00);

	// �e�N�X�`���ύX
	int nIdxTexture = Texture::GetIdx(&PATH_UI_STANDBY[0]);
	if(m_apStateUI[nIdx] != nullptr)
		m_apStateUI[nIdx]->SetIdxTexture(nIdxTexture);

	// �G���g���[��Ԃ�ݒ�
	m_StandbyState[nIdx] = STANDBY_PLAY;
	
	// �v���C���[�̐���
	m_apPlayer[nIdx] = CPlayer::Create();
	
	if (m_apPlayer[nIdx] != nullptr)
	{
		m_apPlayer[nIdx]->ReLoadModel(&player::PATH_BODY[nIdx][0]);

		// �v���C���[�����ݒ�
		m_apPlayer[nIdx]->SetMove(INIT_MOVE_PLAYER);
		m_apPlayer[nIdx]->SetState(CPlayer::STATE_NORMAL);
		m_apPlayer[nIdx]->SetPosition(POS_PLAYER_INIT);

		// ���̓}�l�[�W���[�̊��蓖��
		m_apPlayer[nIdx]->BindInputMgr(m_apInputMgr[nIdx]);	

		// �v���C���[ID�̊��蓖��
		m_apPlayer[nIdx]->SetID(nIdx);

		// ���Ƀr���{�[�h�z�u
		m_apBillboard[nIdx] = CPolygon3D::Create(D3DXVECTOR3(POS_PLAYER_INIT.x,POS_PLAYER_INIT.y + PLAYERNUM_POS_Y, POS_PLAYER_INIT.z));
		if (m_apBillboard[nIdx] != nullptr)
		{
			m_apBillboard[nIdx]->SetMode(CPolygon3D::MODE_BILLBOARD);
			m_apBillboard[nIdx]->SetSize(PLAYERNUM_SIZE, PLAYERNUM_SIZE);
			m_apBillboard[nIdx]->SetTex(D3DXVECTOR2((float)(nIdx + 1) / MAX_PLAYER, 1.0f), D3DXVECTOR2((float)(nIdx) / MAX_PLAYER, 0.0f));

			CTexture* pTexture = CTexture::GetInstance();
			if (pTexture != nullptr)
			{
				m_apBillboard[nIdx]->SetIdxTexture(pTexture->Regist(&PATH_PLAYERNUM[0]));
			}
			
			m_apBillboard[nIdx]->SetVtx();
		}

		// ���[�V�����ݒ�
		m_apPlayer[nIdx]->SetMotion(CPlayer::MOTION::MOTION_MULTIAPPEAR);

		// joypad�U��������
		m_apPlayer[nIdx]->VibJoypad(POW_VIB_APPER, TIME_VIB_APPER);

		// �p�[�e�B�N���̔���
		CParticle::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CParticle::TYPE::TYPE_ENTERPLAYER);
	}

	m_nNumPlayer++;
}

//=====================================================
// ����
//=====================================================
void CPlayerSelect::Ready(int nIdx)
{
	// �G���g���[��Ԃ�ݒ�
	m_StandbyState[nIdx] = STANDBY_OK;

	// �e�N�X�`���ύX
	int nIdxTexture = Texture::GetIdx(&PATH_UI_READY[0]);
	if (m_apStateUI[nIdx] != nullptr)
		m_apStateUI[nIdx]->SetIdxTexture(nIdxTexture);
}

//=====================================================
// �J�n���邩�̊m�F
//=====================================================
void CPlayerSelect::CheckStart(void)
{
	if (m_nNumPlayer <= 1)
		return;	// �v���C���[�������l���Ȃ��ꍇ�A�ʂ�Ȃ�

	bool bStart = true;

	for (int i = 0; i < MAX_PLAYER; i++)
		if (m_StandbyState[i] != E_StandyrState::STANDBY_OK && m_apPlayer[i] != nullptr)
			bStart = false;

	if (bStart)
		StartFade();
}

//=====================================================
// �t�F�[�h�̊J�n
//=====================================================
void CPlayerSelect::StartFade(void)
{
	CFade* pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	if (pFade->GetState() != CFade::FADE::FADE_NONE)
		return;

	// �v���C���[�G���^�[�t���O�̐ݒ�
	vector<bool> abEnter(MAX_PLAYER);

	for (int i = 0; i < MAX_PLAYER; i++)
		abEnter[i] = m_apPlayer[i] != nullptr;

	// ���[�h�̕ۑ�
	gameManager::SaveMode(CGame::E_GameMode::MODE_MULTI, abEnter);

	// �Q�[���ɑJ��
	pFade->SetFade(CScene::MODE_SELECTSTAGE);
}

//=====================================================
// �v���C���[�ԍ��r���{�[�h�ʒu�X�V
//=====================================================
void CPlayerSelect::UpdateBillboard(void)
{
	for (int cnt = 0; cnt < MAX_PLAYER; cnt++)
	{
		if (m_apBillboard[cnt] != nullptr)
		{
			D3DXVECTOR3 pos = m_apPlayer[cnt]->GetPosition();
			m_apBillboard[cnt]->SetPosition(D3DXVECTOR3(pos.x, pos.y + PLAYERNUM_POS_Y, pos.z));
			m_apBillboard[cnt]->SetVtx();
		}
	}
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CPlayerSelect::Debug(void)
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pKeyboard == nullptr || pDebugProc == nullptr)
		return;
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayerSelect::Draw(void)
{

}