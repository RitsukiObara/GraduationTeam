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

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const std::string PATH_BODY = "data\\MOTION\\motionSeals.txt";	// �{�f�B�̃p�X
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
//std::vector<CEnemy*> CEnemy::m_Vector = {};	// ���g�̃|�C���^

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CEnemy::CEnemy(int nPriority) : m_nGridV(0), m_nGridH(0)
{
	/*m_Vector.push_back(this);

	m_Itr = m_Vector[m_Vector.size() - 1];*/
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
CEnemy* CEnemy::Create(void)
{
	CEnemy* pEnemy = nullptr;

	pEnemy = new CEnemy;

	if (pEnemy != nullptr)
	{
		pEnemy->Init();
	}


	return pEnemy;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemy::Init(void)
{
	// �Ǎ�
	Load((char*)&PATH_BODY[0]);

	// �p���N���X�̏�����
	CMotion::Init();

	InitPose(0);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemy::Uninit(void)
{
	////Vector����폜
	//m_Vector.erase(m_Itr);

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemy::Update(void)
{
	CMotion::Update();

#ifdef _DEBUG
	Debug();
#endif
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

	pDebugProc->Print("\n�c[%d]��[%d]", m_nGridV, m_nGridH);

	if (pInputKeyboard->GetTrigger(DIK_RSHIFT))
	{
		CIceManager* pIceManager = CIceManager::GetInstance();

		if (pIceManager != nullptr)
		{
			pIceManager->CreateIce(m_nGridV, m_nGridH);
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemy::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}