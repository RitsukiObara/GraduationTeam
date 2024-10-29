//*****************************************************
//
// �^�C�g������[title.h]
// Author:��؈�^
//
//*****************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************
class CMenu;
class CUI;
class CPolygon2D;
class CMotion;
class CTitleBehavior;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTitle : public CScene
{
public:
	enum STATE
	{
		STATE_NONE = 0,	// �������ĂȂ����
		STATE_OUT,		// �t�F�[�h�A�E�g���
		START_MAX
	};

	enum MENU
	{
		MENU_GAME = 0,	// �Q�[��
		MENU_OPTION,	// �ݒ�
		MENU_MAX
	};

	enum TITLE_UI
	{
		TITLE_UI_LEFT = 0,	// ����
		TITLE_UI_RIGHT,	// �E��
		TITLE_UI_LOGO,	// ���S
		TITLE_UI_ICE,	// �X
		TITLE_UI_PENGUIN,	// �y���M��
		TITLE_UI_MAX
	};

	CTitle();	// �R���X�g���N�^
	~CTitle();	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	STATE GetState(void) { return m_state; }

private:
	void Input(void);
	void Fade(void);
	void ManageState(void);
	void TitleUIState(void);

	STATE m_state;				// ���
	CPolygon2D* m_pLogo;		// �^�C�g�����S�̃|�C���^
	CUI *m_apTitle_UI[TITLE_UI_MAX];	// �^�C�g��UI
	CUI *m_apMenu_UI[MENU_MAX];	// �^�C�g��UI
	D3DXVECTOR3 m_aPosDest[TITLE_UI_MAX];	// �ڕW�̈ʒu
	MENU m_menu;	// �I�����j���[����
	TITLE_UI m_Title_UI;	// �^�C�g��UI
};

#endif