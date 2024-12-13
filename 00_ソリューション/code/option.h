//*****************************************************
//
// �I�v�V��������[option.h]
// Author:�Ό��D�n
//
//*****************************************************
#ifndef _OPTION_H_
#define _OPTION_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class COption : public CScene
{
public:
	// �ݒ蒆���ڗ�
	enum OPTIONPARAM
	{
		PARAM_BGM = 0,
		PARAM_SE,
		PARAM_BIBE,
		PARAM_MAX
	};
	enum BIBESWITCH
	{
		BIBE_ON = 0,
		BIBE_OFF,
		BIBESWITCH_MAX
	};

	// �T�E���h�n�I�u�W�F�N�g�\����
	struct SoundUIObject
	{
		CUI *text;		// �e�L�X�g
		CUI *bar;		// �o�[
		CUI *point;		// ���ݒl
		CUI *penguin;	// �y���M��
	};

	COption();	// �R���X�g���N�^
	~COption();	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	// �����o�֐�
	void CreateSoundUIObj(void);	// �T�E���h�nUI�I�u�W�F�N�g����
	void Input(void);				// ���͌n����
	void Select(void);				// ���ڑI��
	void SettingBGM(void);			// BGM�ݒ�
	void SettingSE(void);			// SE�ݒ�
	void SettingBibe(void);			// �U���L���ݒ�

	// �����o�ϐ�
	// �I�u�W�F�N�g�n
	CUI *m_pOptionText;							// �u�I�v�V�����v�̃e�L�X�g
	SoundUIObject m_aSoundUIObj[PARAM_MAX - 1];	// �T�E���h�nUI�I�u�W�F�N�g
	CUI *m_pBibeText;							// �U���̃e�L�X�g
	CUI *m_apBibePenguin[BIBESWITCH_MAX];		// �U���L��UI�̃y���M��
	CUI *m_pButtonUI;							// �߂�{�^����UI
	
	// �l�n
	OPTIONPARAM m_optionParam;	// �ݒ蒆����
	float m_fBGMVolume;			// BGM�{�����[��
	float m_fSEVolume;			// SE�{�����[��
	BIBESWITCH m_Bibe;			// �o�C�u�L��
	bool m_bSetting;			// �ݒ蒆�t���O
};

#endif