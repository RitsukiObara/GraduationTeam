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

	CTitle();	// �R���X�g���N�^
	~CTitle();	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void ChangeBehavior(CTitleBehavior *pBehavior);
	STATE GetState(void) { return m_state; }

private:
	STATE m_state;				// ���
	CPolygon2D* m_pLogo;		// �^�C�g�����S�̃|�C���^
	CTitleBehavior *m_pBehavior;	// �r�w�C�r�A
	float m_fTImerSmoke;	// ���̃X�|�[���^�C�}�[
};

class CTitleBehavior
{// ��{�r�w�C�r�A
public:
	CTitleBehavior();	// �R���X�g���N�^
	virtual ~CTitleBehavior();	// �f�X�g���N�^

	virtual void Update(CTitle *pTItle) = 0;

private:
};

class CTitleStart : public CTitleBehavior
{// �X�^�[�g�\�����
public:
	CTitleStart();	// �R���X�g���N�^
	virtual ~CTitleStart();	// �f�X�g���N�^
	void Update(CTitle *pTItle) override;

private:
	CPolygon2D *m_pStart;	// �X�^�[�g�\���̃|�C���^
	CPolygon2D *m_pAfter;	// �X�^�[�g�\���̎c��
};

class CTitleMenu : public CTitleBehavior
{// ���j���[
public:
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

	CTitleMenu();	// �R���X�g���N�^
	virtual ~CTitleMenu();	// �f�X�g���N�^
	void Update(CTitle *pTItle) override;

private:
	void Input(void);
	void ManageCursor(void);
	void Fade(void);

	CPolygon2D *m_apMenu[MENU_MAX];	// ���j���[����
	CPolygon2D *m_pCursor;	// �J�[�\��
	MENU m_menu;	// �I�����j���[����
	TITLE_UI m_Title_UI;	// �^�C�g��UI
	CUI* m_pBg;	// �w�i�QD�I�u�W�F�N�g
};

#endif