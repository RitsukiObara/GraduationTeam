//*****************************************************
//
// �X�e�[�W�I���Ǘ�����[SelectStageManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "selectStageManager.h"
#include "skybox.h"
#include "selectStagePenguin.h"
#include "camera.h"
#include "cameraState.h"
#include "inputManager.h"
#include "debugproc.h"
#include "inputkeyboard.h"
#include "fade.h"
#include "collision.h"
#include "manager.h"
#include "particle.h"
#include "ocean.h"
#include "gameManager.h"
#include "tutorial.h"
#include "number.h"
#include "texture.h"
#include "sound.h"
#include "iceManager.h"

//*****************************************************
// �}�N����`
//*****************************************************
namespace
{
const string PATH_TEXT = "data\\TEXT\\selectStage.txt";			// �e�L�X�g�p�X
const string PATH_TEXT_MULTI = "data\\TEXT\\stagemulti.txt";	// �}���`�̃e�L�X�g�p�X

const float RADIUS_COLLISION_PUSHOUT_STAGE = 500.0f;	// �����o������̔��a�i�X�e�[�W�j
const float RADIUS_COLLISION_PUSHOUT_BANNER = 250.0f;	// �����o������̔��a�i�Ŕj
const float RATE_SELECT_COLLISION = 1.4f;				// �I�����̔��a�̊���

const float SCALE_STATE[CSelectStageManager::E_StateStage::STATE_MAX] = { 0.0f, 1.0f, 1.4f };	// ��Ԃ��Ƃ̃X�P�[��

const float SPEED_SCALING_STAGE = 0.3f;	// �X�P�[�����O�̑��x
const float TIME_ENTER = 2.0f;			// �G���^�[�ɂ����鎞��

const float SPEED_SCALING_PENGUIN = 0.05f;	// �y���M���̃X�P�[�����O���x
const float SPEED_MOVE_ENTER = 0.01f;		// �G���^�[���̈ړ����x

const float SPEED_PARTICLE = 22.0f;								// �p�[�e�B�N���̑��x

const string PATH_BANNER = "data\\MODEL\\other\\stageselect_banner.x";	// �Ŕ��f���̃p�X
const D3DXVECTOR3 BANNER_POS = D3DXVECTOR3(0.0f, 0.0, -2900.0);	// �Ŕ̈ʒu
const float SCALE_BANNER = 15.0f;								// �Ŕ̃X�P�[��

const D3DXVECTOR3 INIT_POS_PLAYER = D3DXVECTOR3(0.0f, 0.0, -1300.0);	// �v���C���[�̏����ʒu

const float HEIGHT_NUMBER = 0.14f;				// ���O�̍���
const float ADULTWALL_LENGTH = 3850.0f;			// ��l�̕�
const D3DXVECTOR2 SIZE_NAME = { 0.25f, 0.09f };	// ���O�̃T�C�Y

const float DEPTH_PENGUIN = -500.0f;				// �y���M���̉��s��

//----------------------------------
// �߂�{�^��UI�̒萔
//----------------------------------
namespace backButtonUI
{
const string PATH = "data\\TEXTURE\\UI\\B_Back.png";
const float WIDTH = 0.09f;
const float HEIGHT = 0.049f;
const D3DXVECTOR3 POS = D3DXVECTOR3(0.88f, 0.92f, 0.0f);
}
//----------------------------------
// �X�^�[�g�{�^��UI�̒萔
//----------------------------------
namespace startButtonUI
{
	const string PATH = "data\\TEXTURE\\UI\\A_Start.png";
	const float WIDTH = 0.11f;
	const float HEIGHT = 0.039f;
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.78f, 0.92f, 0.0f);
}
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
vector<CSelectStageManager::S_InfoStage*> CSelectStageManager::s_aInfoStage;		// �X�e�[�W���̔z��
vector<CSelectStageManager::S_InfoStage*> CSelectStageManager::s_aInfoStageMulti;	// �}���`�X�e�[�W���̔z��

//=====================================================
// �R���X�g���N�^
//=====================================================
CSelectStageManager::CSelectStageManager() : m_pBanner(nullptr),m_pPenguin(nullptr), m_bEnter(false), m_fTimerFade(0.0f), m_nIdxSelect(0), m_pButtonUIBack(nullptr)
{

}

//=====================================================
// ��������
//=====================================================
CSelectStageManager *CSelectStageManager::Create(void)
{
	CSelectStageManager *pManager = nullptr;

	pManager = new CSelectStageManager;

	if (pManager != nullptr)
	{
		// ����������
		pManager->Init();
	}

	return pManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CSelectStageManager::Init(void)
{
	// �X�J�C�{�b�N�X�̐���
	CSkybox::Create();

	// �X�e�[�W�I���Ŕݒu
	if (m_pBanner == nullptr)
		m_pBanner = CObjectX::Create(BANNER_POS);
	
	if (m_pBanner != nullptr)
	{
		// ���f���̊��蓖��
		m_pBanner->BindModel(CModel::Load(&PATH_BANNER[0]));
		m_pBanner->SetScale(SCALE_BANNER);

		// �Ŕp�̔���̐���
		CCollisionSphere *pCollision = CCollisionSphere::Create(CCollision::TAG::TAG_BLOCK, CCollision::TYPE::TYPE_SPHERE, this);
		
		if (pCollision != nullptr)
		{
			pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT_BANNER);
			pCollision->SetPosition(BANNER_POS);
		}
	}

	// �X�e�[�W�̐ݒu
	SetStage();

	// �y���M���̐���
	m_pPenguin = CSelectStagePenguin::Create();
	if (m_pPenguin != nullptr)
	{
		int nIdxMapLast = gameManager::LoadIdxMap();

		if (nIdxMapLast == -1)
		{// �O��̃}�b�v�ԍ���������΋K��ʒu
			m_pPenguin->SetPosition(INIT_POS_PLAYER);
		}
		else
		{// �O��̃}�b�v�̑O�ɔz�u
			if (nIdxMapLast >= (int)s_aInfoStage.size() ||
				nIdxMapLast < 0)
				return E_FAIL;

			D3DXVECTOR3 posPenguin = s_aInfoStage[nIdxMapLast]->pos;
			posPenguin.z += DEPTH_PENGUIN;
			m_pPenguin->SetPosition(posPenguin);
		}
	}

	// �J�����̃X�e�C�g�ݒ�
	Camera::ChangeState(new CCameraStateSelectStage(m_pPenguin));

	// �C�̐���
	COcean::Create();

	// �X�}�l�[�W���[�����AOcean�������̂ɕK�v
	CIceManager::Create(0, 0);

	// �{�^��UI�z�u
	m_pButtonUIBack = CUI::Create();
	if (m_pButtonUIBack != nullptr)
	{
		// �ݒ�
		m_pButtonUIBack->SetIdxTexture(CTexture::GetInstance()->Regist(&backButtonUI::PATH[0]));	// �e�N�X�`������
		m_pButtonUIBack->SetPosition(backButtonUI::POS);					// �ʒu
		m_pButtonUIBack->SetSize(backButtonUI::WIDTH, backButtonUI::HEIGHT);	// �傫��
		m_pButtonUIBack->SetVtx();	// ���_���f
	}

	return S_OK;
}

//=====================================================
// �ǂݍ��ݏ���
//=====================================================
void CSelectStageManager::Load(void)
{
	if (!s_aInfoStage.empty())
		return;

	std::ifstream file(PATH_TEXT);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// �ǂݍ��ނ��̂��Ȃ��Ȃ�܂œǍ�
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "SETSTAGE")
			{// �X�e�[�W���Ǎ��J�n
				// �X�e�[�W���̐���
				S_InfoStage *pInfoStage = new S_InfoStage;

				if (pInfoStage == nullptr)
					continue;

				LoadStage(file, temp, pInfoStage);

				s_aInfoStage.push_back(pInfoStage);
			}

			if (file.eof())
			{// �ǂݍ��ݏI��
				break;
			}
		}

		file.close();
	}
	else
	{
		assert(("�t�@�C�����J���܂���ł���", false));
	}

	// �}���`�̓Ǎ�
	LoadMulti();
}

//=====================================================
// �}���`�̓ǂݍ��ݏ���
//=====================================================
void CSelectStageManager::LoadMulti(void)
{
	if (!s_aInfoStageMulti.empty())
		return;

	std::ifstream file(PATH_TEXT_MULTI);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// �ǂݍ��ނ��̂��Ȃ��Ȃ�܂œǍ�
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "SETSTAGE")
			{// �X�e�[�W���Ǎ��J�n
				// �X�e�[�W���̐���
				S_InfoStage *pInfoStage = new S_InfoStage;

				if (pInfoStage == nullptr)
					continue;

				LoadStage(file, temp, pInfoStage);

				s_aInfoStageMulti.push_back(pInfoStage);
			}

			if (file.eof())
			{// �ǂݍ��ݏI��
				break;
			}
		}

		file.close();
	}
	else
	{
		assert(("�t�@�C�����J���܂���ł���", false));
	}
}

//=====================================================
// �X�e�[�W�̓Ǎ�
//=====================================================
void CSelectStageManager::LoadStage(std::ifstream& file, string str, S_InfoStage *pInfoStage)
{
	if (pInfoStage == nullptr)
		return;

	while (std::getline(file, str))
	{// �Ǎ��J�n
		std::istringstream iss(str);
		std::string key;
		iss >> key;

		if (key == "END_SETSTAGE")
		{// �I��
			break;
		}

		if (key == "MODEL")
		{// �p�X
			iss >> str >> pInfoStage->pathModel;
		}

		if (key == "POS")
		{// �ʒu
			iss >> str >> pInfoStage->pos.x >> pInfoStage->pos.y >> pInfoStage->pos.z;
		}

		if (key == "PATHMAP")
		{// �}�b�v�̃p�X
			iss >> str >> pInfoStage->pathMap;
		}

		if (key == "PATHENEMY")
		{// �G�̃p�X
			iss >> str >> pInfoStage->pathEnemy;
		}

		if (key == "PATHNAME")
		{// �X�e�[�W���̃p�X
			iss >> str >> pInfoStage->pathName;
		}

		if (key == "DIR_STREAM")
		{// �C���̌���
			iss >> str >> pInfoStage->nDirStream;
		}
	}
}

//=====================================================
// �X�e�[�W�̐ݒu
//=====================================================
void CSelectStageManager::SetStage(void)
{
	if (s_aInfoStage.empty())
		return;

	for (S_InfoStage *pInfoStage : s_aInfoStage)
	{
		// �X�e�[�W��X���f���̐ݒu
		pInfoStage->pModel = CObjectX::Create();

		if (pInfoStage->pModel == nullptr)
			return;

		int nIdxModel = CModel::Load(&pInfoStage->pathModel[0]);
		pInfoStage->pModel->BindModel(nIdxModel);

		pInfoStage->pModel->SetPosition(pInfoStage->pos);

		// �����蔻��̐���
		pInfoStage->pCollision = CCollisionSphere::Create(CCollision::TAG::TAG_BLOCK, CCollision::TYPE::TYPE_SPHERE, this);

		if (pInfoStage->pCollision == nullptr)
			return;

		pInfoStage->pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT_STAGE);
		pInfoStage->pCollision->SetPosition(GetPosition());

		// ��Ԃ̏����ݒ�
		pInfoStage->state = E_StateStage::STATE_NORMAL;

		if (pInfoStage != *(s_aInfoStage.end() - 1))
		{// �p�[�e�B�N���ʒu�̐���
			CGameObject *pObject = new CGameObject;

			m_aParticlePos.push_back(pObject);
			pObject->SetPosition(pInfoStage->pos);
		}

		// ���O�̐���
		pInfoStage->pName = CUI::Create();

		if (pInfoStage->pName == nullptr)
			return;

		D3DXVECTOR3 posScreen;
		universal::IsInScreen(pInfoStage->pos,&posScreen);
		universal::ConvertScreenRate(posScreen);

		pInfoStage->pName->SetPosition(D3DXVECTOR3(posScreen.x, posScreen.y - HEIGHT_NUMBER, 0.0f));
		pInfoStage->pName->SetSize(SIZE_NAME.x, SIZE_NAME.y);
		pInfoStage->pName->SetVtx();

		// �e�N�X�`�����f
		int nIdxTexture = Texture::GetIdx(&pInfoStage->pathName[0]);
		pInfoStage->pName->SetIdxTexture(nIdxTexture);

		// �X�^�[�g�{�^��UI����
		pInfoStage->pStartButtonUI = CUI::Create();

		if (pInfoStage->pStartButtonUI == nullptr)
			return;

		// �ݒ�
		pInfoStage->pStartButtonUI->SetIdxTexture(CTexture::GetInstance()->Regist(&startButtonUI::PATH[0]));	// �e�N�X�`������
		pInfoStage->pStartButtonUI->SetPosition(posScreen);					// �ʒu
		pInfoStage->pStartButtonUI->SetSize(0.0f, 0.0f);	// �傫��
		pInfoStage->pStartButtonUI->SetVtx();	// ���_���f
	}
}

//=====================================================
// �I������
//=====================================================
void CSelectStageManager::Uninit(void)
{
	Object::DeleteObject((CObject**)&m_pButtonUIBack);
	if (m_pBanner != nullptr)
	{
		m_pBanner->Uninit();
		m_pBanner = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CSelectStageManager::Update(void)
{
	// �I������
	Select();

	if (m_bEnter)
		StayEnter();	// �G���^�[���̏���
	else
		// ���[�h�Z���N�g�ɖ߂鏈��
		ModeSelectBack();

	// ��l�̕ǔ���
	CollisionAdultWall();

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �I������
//=====================================================
void CSelectStageManager::Select(void)
{
	CSound* pSound = CSound::GetInstance();

	//=======================================
	// �e�I�u�W�F�N�g����ɍs������
	//=======================================
	for (int i = 0; i < (int)s_aInfoStage.size(); i++)
	{
		S_InfoStage *pInfoStage = s_aInfoStage[i];

		// �X�P�[�����O����
		Scaling(pInfoStage);

		// �g�ɒǏ]������
		FollowOcean(pInfoStage);

		// �p�[�e�B�N�����o��
		SetParticle(i);

		if (pInfoStage->pCollision == nullptr)
			continue;

		// ����̒Ǐ]
		pInfoStage->pCollision->SetPosition(pInfoStage->pModel->GetPosition());
	}

	//=======================================
	// �Ŕ̔g�Ǐ]����
	//=======================================
	if (m_pBanner != nullptr)
	{
		FollowOcean(m_pBanner);
	}

	//=======================================
	// �I�u�W�F�N�g��I�ԏ���
	//=======================================
	bool bEnterOnce = false;
	for (int i = 0; i < (int)s_aInfoStage.size(); i++)
	{
		S_InfoStage *pInfoStage = s_aInfoStage[i];

		if (pInfoStage->pCollision == nullptr)
			continue;

		// ����̈ꎞ�g��
		pInfoStage->pCollision->SetRadius(RATE_SELECT_COLLISION * RADIUS_COLLISION_PUSHOUT_STAGE);

		// �v���C���[�������Ă�����I����Ԃɂ���
		bool bEnter = pInfoStage->pCollision->OnEnter(CCollision::TAG_PLAYER);

		// ����̑傫����߂�
		pInfoStage->pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT_STAGE);

		if (bEnter && !bEnterOnce)
		{
			pInfoStage->state = E_StateStage::STATE_SELECT;

			// �G���^�[����̌��o
			if (m_pPenguin != nullptr)
			{
				if (m_pPenguin->IsEnter())
				{// �G���^�[�̊J�n

					if (pSound != nullptr)
					{
						// �X�e�[�W�ɓ����Ă�����
						pSound->Play(pSound->LABEL_SE_STAGE_ENTRY);
					}

					m_nIdxSelect = i;

					StartEnter();
				}
			}

			bEnterOnce = true;
		}
		else
			pInfoStage->state = E_StateStage::STATE_NORMAL;
	}
}

//=====================================================
// �X�P�[�����O����
//=====================================================
void CSelectStageManager::Scaling(S_InfoStage *pInfoStage)
{
	CObjectX *pModel = pInfoStage->pModel;

	if (pModel == nullptr)
		return;

	float fScaleDest = SCALE_STATE[pInfoStage->state];
	float fScale = pModel->GetScale().x;

	fScale += (fScaleDest - fScale) * SPEED_SCALING_STAGE;

	pModel->SetScale(fScale);

	// ���O�̈ʒu�ݒ�
	if (pInfoStage->pName == nullptr)
		return;

	D3DXVECTOR3 posScreen;
	universal::IsInScreen(pInfoStage->pos, &posScreen);
	universal::ConvertScreenRate(posScreen);

	pInfoStage->pName->SetPosition(D3DXVECTOR3(posScreen.x, posScreen.y - HEIGHT_NUMBER, 0.0f));
	pInfoStage->pName->SetVtx();

	// �X�^�[�g�{�^��UI�̐ݒ�
	if (pInfoStage->pStartButtonUI == nullptr)
		return;

	float fScaleUIDest = (pInfoStage->state == E_StateStage::STATE_SELECT) ? 1.0f : 0.0f;
	float fScaleUI = (pInfoStage->pStartButtonUI->GetSize().x / startButtonUI::WIDTH);
	fScaleUI += (fScaleUIDest - fScaleUI) * SPEED_SCALING_STAGE;

	pInfoStage->pStartButtonUI->SetPosition(posScreen);					// �ʒu
	pInfoStage->pStartButtonUI->SetSize(startButtonUI::WIDTH * fScaleUI, startButtonUI::HEIGHT * fScaleUI);	// �傫��
	pInfoStage->pStartButtonUI->SetVtx();	// ���_���f
}

//=====================================================
// �g�ɒǏ]������
//=====================================================
void CSelectStageManager::FollowOcean(S_InfoStage *pInfoStage)
{
	COcean*pOcean = COcean::GetInstance();
	if (pOcean == nullptr)
		return;

	if (pInfoStage->pModel == nullptr)
		return;

	D3DXVECTOR3 pos = pInfoStage->pModel->GetPosition();
	float fHeight = pOcean->GetHeight(pos, nullptr);

	pos.y = fHeight;

	pInfoStage->pModel->SetPosition(pos);
}

//=====================================================
// �g�ɒǏ]������(ObjectX��)
//=====================================================
void CSelectStageManager::FollowOcean(CObjectX* pObject)
{
	COcean* pOcean = COcean::GetInstance();
	if (pOcean == nullptr)
		return;

	if (pObject == nullptr)
		return;

	D3DXVECTOR3 pos = pObject->GetPosition();
	float fHeight = pOcean->GetHeight(pos, nullptr);

	pos.y = fHeight;

	pObject->SetPosition(pos);
}

//=====================================================
// �p�[�e�B�N���̔���
//=====================================================
void CSelectStageManager::SetParticle(int nIdx)
{
	if (nIdx >= (int)s_aInfoStage.size() - 1)
		return;

	// �����x�N�g�����ړ����x�ɐ��K��
	D3DXVECTOR3 pos = s_aInfoStage[nIdx]->pos;
	D3DXVECTOR3 posNext = s_aInfoStage[nIdx + 1]->pos;

	D3DXVECTOR3 vecDiff = posNext - pos;

	D3DXVECTOR3 move = vecDiff;
	universal::VecConvertLength(&move, SPEED_PARTICLE);

	// �ړ��ʂ𔽉f
	m_aParticlePos[nIdx]->Translate(move);

	// �p�[�e�B�N���𔭐�
	D3DXVECTOR3 posParticle = m_aParticlePos[nIdx]->GetPosition();
	CParticle::Create(posParticle, CParticle::TYPE::TYPE_STAGESELECT_SNOW);

	// ���̈ʒu�Ɉ��ȏ�߂Â����猳�̈ʒu�ɖ߂�
	if (universal::DistCmpFlat(posParticle, posNext, RADIUS_COLLISION_PUSHOUT_STAGE * RATE_SELECT_COLLISION, nullptr))
	{
		D3DXVECTOR3 posBack = pos;
		D3DXVECTOR3 vecLength = vecDiff;

		universal::VecConvertLength(&vecLength, RADIUS_COLLISION_PUSHOUT_STAGE * RATE_SELECT_COLLISION);

		posBack += vecLength;

		m_aParticlePos[nIdx]->SetPosition(posBack);
	}
}

//=====================================================
// �G���^�[�J�n
//=====================================================
void CSelectStageManager::StartEnter(void)
{
	// �^�C�}�[�̃��Z�b�g
	m_fTimerFade = 0.0f;

	// �G���^�[�t���O�𗧂Ă�
	m_bEnter = true;

	// �y���M���𑀍�s�\�ɂ���
	if (m_pPenguin != nullptr)
		m_pPenguin->EnableInput(false);

	// �����蔻��̍폜
	for (S_InfoStage *pInfoStage : s_aInfoStage)
	{
		if (pInfoStage->pCollision != nullptr)
		{
			pInfoStage->pCollision->Uninit();
			pInfoStage->pCollision = nullptr;
		}
	}

	// �X�e�[�W�ԍ��ۑ�
	gameManager::SaveIdxMap(m_nIdxSelect);
}

//=====================================================
// �G���^�[���̏���
//=====================================================
void CSelectStageManager::StayEnter(void)
{
	m_fTimerFade += CManager::GetDeltaTime();

	if (TIME_ENTER < m_fTimerFade)
		EndEnter();	// �G���^�[�̏I��

	if (m_pPenguin == nullptr)
		return;	// �ȉ��A�y���M���̏���

	// �X�e�[�W�Ɉ��������鏈��
	D3DXVECTOR3 pos = m_pPenguin->GetPosition();
	D3DXVECTOR3 posStage = s_aInfoStage[m_nIdxSelect]->pos;

	universal::MoveToDest(&pos, posStage, SPEED_MOVE_ENTER);

	m_pPenguin->SetPosition(pos);

	// �X�P�[�����O�̏���
	float fScale = m_pPenguin->GetParts(0)->pParts->GetScale().x;

	fScale += (0 - fScale) * SPEED_SCALING_PENGUIN;

	m_pPenguin->GetParts(0)->pParts->SetScale(fScale);
}

//=====================================================
// �G���^�[�I��
//=====================================================
void CSelectStageManager::EndEnter(void)
{
	// �t�F�[�h�̊J�n
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	bool bEnterTutorial = tutorial::LoadFrag();

	if(bEnterTutorial)
		pFade->SetFade(CScene::MODE::MODE_TUTORIAL);
	else
		pFade->SetFade(CScene::MODE::MODE_GAME);
}

//=====================================================
// ��l�̕ǔ���
//=====================================================
void CSelectStageManager::CollisionAdultWall(void)
{
	D3DXVECTOR3 pos = m_pPenguin->GetPosition();
	float length = D3DXVec3Length(&pos);	// �����v�Z

	if (length >= ADULTWALL_LENGTH)
	{// ��l�̕ǔ͈͊O
		D3DXVECTOR3 vec;
		D3DXVec3Normalize(&vec, &pos);	// ���K��
		pos = vec * ADULTWALL_LENGTH;	// ��l�̕ǔ͈͓��Ɏ��߂�
		m_pPenguin->SetPosition(pos);	// �ʒu�ݒ�
	}
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CSelectStageManager::Debug(void)
{
	CDebugProc* pDebugProc = CDebugProc::GetInstance();
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();

	if (pDebugProc == nullptr || pKeyboard == nullptr)
		return;

	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	if (pKeyboard->GetTrigger(DIK_1))
		pFade->SetFade(CScene::MODE::MODE_GAME);
}

//=====================================================
// �`�揈��
//=====================================================
void CSelectStageManager::Draw(void)
{

}

//=====================================================
// ���[�h�Z���N�g�ɖ߂鏈��
//=====================================================
void CSelectStageManager::ModeSelectBack(void)
{
	// �t�F�[�h�̊J�n
	CFade* pFade = CFade::GetInstance();

	CInputManager* pInput = CInputManager::GetInstance();

	if (pInput == nullptr)
		return;

	if (pInput->GetTrigger(CInputManager::BUTTON_BACK))	// BACK�������Ƃ�
	{
		pFade->SetFade(CScene::MODE::MODE_SELECTMODE);
	}
}