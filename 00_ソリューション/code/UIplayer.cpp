//*****************************************************
//
// �v�����[UI�̏���[UIplayer.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "UIplayer.h"
#include "gameManager.h"
#include "manager.h"
#include "texture.h"
#include "player.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
namespace icon
{
const float WIDTH = 0.05f;							// ��
const float HEIGHT = 0.086f;						// ����
const D3DXVECTOR3 POS_INIT = { 0.35f,0.3f,0.0f };	// �����ʒu
const D3DXVECTOR3 POS_DEST[] =						// �ڕW�ʒu
{
	{ WIDTH,HEIGHT,0.0f },
	{ 1.0f - WIDTH,HEIGHT,0.0f },
	{ WIDTH,1.0f - HEIGHT,0.0f },
	{ 1.0f - WIDTH,1.0f - HEIGHT,0.0f },
};
const float TIME_MOVE = 3.0f;	// �ړ��ɂ����鎞��
const string PATH_TEX[] =		// �e�N�X�`���p�X
{
	"data\\TEXTURE\\UI\\playericon_00.png",
	"data\\TEXTURE\\UI\\playericon_01.png",
	"data\\TEXTURE\\UI\\playericon_02.png",
	"data\\TEXTURE\\UI\\playericon_03.png",
};
const D3DXCOLOR COL_INIT = { 1.0f,1.0f,1.0f,1.0f };	// �����F
const D3DXCOLOR COL_DEST = { 0.7f,0.7f,0.7f,1.0f };	// �ڕW�F
const D3DXCOLOR COL_DIFF = COL_DEST - COL_INIT;		// �����F
const float TIME_FADE_ICON = 2.0f;					// �A�C�R�����Â��Ȃ鎞��
}
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CUIPlayer::FuncUpdateState CUIPlayer::s_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,					// ���ł��Ȃ����
	&CUIPlayer::UpdateScatter,	// �U��΂���
};

//=====================================================
// �R���X�g���N�^
//=====================================================
CUIPlayer::CUIPlayer(int nPriority) : CObject(nPriority), m_state(E_State::STATE_NONE), m_fTimerScatter(0.0f)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CUIPlayer::~CUIPlayer()
{

}

//=====================================================
// ��������
//=====================================================
CUIPlayer *CUIPlayer::Create()
{
	CUIPlayer *pUIPlayer = new CUIPlayer;

	if (pUIPlayer != nullptr)
		pUIPlayer->Init();

	return pUIPlayer;
}

//=====================================================
// ����������
//=====================================================
HRESULT CUIPlayer::Init(void)
{
	// �v���C���[�G���g���[�t���O�擾
	CGame::E_GameMode mode;
	vector<bool> abFrag;

	gameManager::LoadMode(&mode, abFrag);

	for (int i = 0; i < (int)abFrag.size(); i++)
	{
		if (!abFrag[i])	// �t���O�������Ă��Ȃ�������ʂ�Ȃ�
			continue;

		S_Icon *pIcon = new S_Icon;

		if (pIcon == nullptr)
			continue;

		pIcon->pPolygon = CUI::Create();

		if (pIcon->pPolygon == nullptr)
			continue;

		// UI�̏����ݒ�
		pIcon->pPolygon->SetSize(icon::WIDTH, icon::HEIGHT);
		D3DXVECTOR3 pos = icon::POS_INIT;
		pos.x += icon::WIDTH * i * 2;
		pIcon->pPolygon->SetPosition(pos);
		pIcon->pPolygon->SetVtx();

		int nIdxTexture = Texture::GetIdx(&icon::PATH_TEX[i][0]);
		pIcon->pPolygon->SetIdxTexture(nIdxTexture);

		m_aIcon.push_back(pIcon);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CUIPlayer::Uninit(void)
{
	for (int i = 0; i < (int)m_aIcon.size(); i++)
	{
		if (m_aIcon[i]->pPolygon != nullptr)
		{
			m_aIcon[i]->pPolygon->Uninit();
			m_aIcon[i]->pPolygon = nullptr;
		}

		delete m_aIcon[i];
	}

	m_aIcon.clear();

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CUIPlayer::Update(void)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (s_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ
		// �e��Ԃ��Ƃ̍X�V
		(this->*(s_aFuncUpdateState[m_state]))();
	}

	// �A�C�R���̃t�F�[�h
	UpdateFadeIcon();
}

//=====================================================
// �U��΂�n�߂鏈��
//=====================================================
void CUIPlayer::StartScatter(void)
{
	m_state = E_State::STATE_SCATTER;
}

//=====================================================
// �U��΂�X�V
//=====================================================
void CUIPlayer::UpdateScatter(void)
{
	if (m_fTimerScatter < icon::TIME_MOVE)
		m_fTimerScatter += CManager::GetDeltaTime();

	float fRate = easing::EaseOutExpo(m_fTimerScatter);

	for (int i = 0; i < (int)m_aIcon.size(); i++)
	{
		if (m_aIcon[i]->pPolygon == nullptr)
			continue;

		D3DXVECTOR3 vecDiff = icon::POS_DEST[i] - icon::POS_INIT;

		D3DXVECTOR3 pos = icon::POS_INIT + vecDiff * fRate;

		m_aIcon[i]->pPolygon->SetPosition(pos);
		m_aIcon[i]->pPolygon->SetVtx();
	}
}

//=====================================================
// �A�C�R���t�F�[�h
//=====================================================
void CUIPlayer::UpdateFadeIcon(void)
{
	//------------------------------
	// ���񂾃v���C���[�̌��o
	//------------------------------
	vector<CPlayer*> apPlayer = CPlayer::GetInstance();

	for (CPlayer *pPlayer : apPlayer)
	{
		if (pPlayer == nullptr)
			continue;

		CPlayer::E_State state = pPlayer->GetState();

		if (state == CPlayer::E_State::STATE_DEATH)
		{
			int nIDPlayer = pPlayer->GetID();

			m_aIcon[nIDPlayer]->bFade = true;
		}
	}

	//------------------------------
	// �A�C�R���̃t�F�[�h
	//------------------------------
	for (S_Icon *pIcon : m_aIcon)
	{
		if (!pIcon->bFade)
			continue;

		pIcon->fTimerFade += CManager::GetDeltaTime();

		float fTime = pIcon->fTimerFade / icon::TIME_FADE_ICON;
		float fRate = easing::EaseOutExpo(fTime);
		universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

		D3DXCOLOR col = icon::COL_INIT + icon::COL_DIFF * fRate;

		pIcon->pPolygon->SetCol(col);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CUIPlayer::Draw(void)
{

}