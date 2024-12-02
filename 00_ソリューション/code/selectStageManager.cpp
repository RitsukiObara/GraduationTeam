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

//*****************************************************
// �}�N����`
//*****************************************************
namespace
{
const string PATH_TEXT = "data\\TEXT\\selectStage.txt";	// �e�L�X�g�p�X
const string PATH_BANNER = "data\\MODEL\\other\\stageselect_banner.x";

const float RADIUS_COLLISION_PUSHOUT_STAGE = 500.0f;	// �����o������̔��a�i�X�e�[�W�j
const float RADIUS_COLLISION_PUSHOUT_BANNER = 250.0f;	// �����o������̔��a�i�Ŕj
const float RATE_SELECT_COLLISION = 1.4f;				// �I�����̔��a�̊���

const float SCALE_STATE[CSelectStageManager::E_StateStage::STATE_MAX] = { 0.0f, 1.0f, 1.4f };	// ��Ԃ��Ƃ̃X�P�[��

const float SPEED_SCALING_STAGE = 0.3f;	// �X�P�[�����O�̑��x
const float TIME_ENTER = 2.0f;			// �G���^�[�ɂ����鎞��

const float SPEED_SCALING_PENGUIN = 0.05f;	// �y���M���̃X�P�[�����O���x
const float SPEED_MOVE_ENTER = 0.01f;		// �G���^�[���̈ړ����x

const float SPEED_PARTICLE = 22.0f;								// �p�[�e�B�N���̑��x
const D3DXVECTOR3 BANNER_POS = D3DXVECTOR3(0.0f, 0.0, 1300.0);	// �Ŕ̈ʒu

const float HEIGHT_NUMBER = 0.14f;				// ���O�̍���
const D3DXVECTOR2 SIZE_NAME = { 0.1f, 0.06f };	// ���O�̃T�C�Y
const string PATH_TEX_NAME[] =					// �e�N�X�`���̃p�X
{
	"",
	"",
	"",
	"",
};
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
vector<CSelectStageManager::S_InfoStage*> CSelectStageManager::s_aInfoStage;	// �X�e�[�W���̔z��

//=====================================================
// �R���X�g���N�^
//=====================================================
CSelectStageManager::CSelectStageManager() : m_pPenguin(nullptr), m_bEnter(false), m_fTimerFade(0.0f), m_nIdxSelect(0)
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

	// �J�����̃X�e�C�g�ݒ�
	Camera::ChangeState(new CCameraStateSelectStage);

	// �X�e�[�W�I���Ŕݒu
	CObjectX* pBanner = CObjectX::Create(BANNER_POS);
	pBanner->BindModel(CModel::Load(&PATH_BANNER[0]));
	pBanner->SetScale(15.0f);
	CCollisionSphere *pCollision = CCollisionSphere::Create(CCollision::TAG::TAG_BLOCK, CCollision::TYPE::TYPE_SPHERE, this);
	if (pCollision != nullptr)
	{
		pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT_BANNER);
		pCollision->SetPosition(BANNER_POS);
	}

	// �X�e�[�W�̐ݒu
	SetStage();

	// �y���M���̐���
	m_pPenguin = CSelectStagePenguin::Create();

	// �C�̐���
	COcean::Create();

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

	int nIdx = 1;

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

		nIdx++;
	}
}

//=====================================================
// �I������
//=====================================================
void CSelectStageManager::Uninit(void)
{
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
	

#ifdef _DEBUG
	Debug();
#endif
}

//=====================================================
// �I������
//=====================================================
void CSelectStageManager::Select(void)
{
	for (int i = 0; i < (int)s_aInfoStage.size(); i++)
	{
		S_InfoStage *pInfoStage = s_aInfoStage[i];

		// �X�P�[�����O����
		Scaling(pInfoStage);

		// �p�[�e�B�N�����o��
		SetParticle(i);

		if (pInfoStage->pCollision == nullptr)
			continue;

		// ����̒Ǐ]
		pInfoStage->pCollision->SetPosition(pInfoStage->pModel->GetPosition());

		// ����̈ꎞ�g��
		pInfoStage->pCollision->SetRadius(RATE_SELECT_COLLISION * RADIUS_COLLISION_PUSHOUT_STAGE);

		// �v���C���[�������Ă�����I����Ԃɂ���
		bool bEnter = pInfoStage->pCollision->OnEnter(CCollision::TAG_PLAYER);

		// ����̑傫����߂�
		pInfoStage->pCollision->SetRadius(RADIUS_COLLISION_PUSHOUT_STAGE);

		if (bEnter)
		{
			pInfoStage->state = E_StateStage::STATE_SELECT;

			// �G���^�[����̌��o
			if (m_pPenguin != nullptr)
			{
				if (m_pPenguin->IsEnter())
				{// �G���^�[�̊J�n
					m_nIdxSelect = i;

					StartEnter();

					return;
				}
			}

			break;
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