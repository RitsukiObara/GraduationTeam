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
		PARAM_VIBRATION,
		PARAM_MAX
	};
	enum VIBRATIONSWITCH
	{
		VIBRATION_ON = 0,
		VIBRATION_OFF,
		VIBRATIONSWITCH_MAX
	};
	enum VOLUME_LEVEL
	{
		LEVEL_MUTE = 0,	// ����
		LEVEL_LOW,		// �Œ��
		LEVEL_MIDDLE,	// ����
		LEVEL_HIGH,		// �傫��
		VOLUME_LEVEL_MAX
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

	// �ݒ�O���t�@�C��
	static void SaveSetting(void);			// �ݒ�ۑ�
	static void LoadSetting(void);			// �ݒ�ǂݍ���

private:
	typedef void (COption::*SettingFunc)(void);

	// �����o�֐�
	void CreateUIAll(void);			// UI�����S��
	void CreateSoundUIObj(void);	// �T�E���h�nUI�I�u�W�F�N�g����
	void CreateVibrationUIObj(void);		// �U��UI�I�u�W�F�N�g����
	void Input(void);				// ���͌n����
	void ModeSelectBack(void);		// ���[�h�I����ʂɖ߂�
	void Select(void);				// ���ڑI��
	void MoveSelect(void);			// ���ړ�����
	void SettingBGM(void);			// BGM�ݒ�
	void SettingSE(void);			// SE�ݒ�
	void SettingSound(CUI* pUI, float* volumeBase, float volumeAfter);
	void SettingVibration(void);			// �U���L���ݒ�
	void MoveBGM(void);				// BGM��UI������
	void MoveSE(void);				// SE��UI������
	void MoveVibration(void);			// �U���L����UI������
	void ColChangeVibration(void);		// �U���L���̐F�ύX
	void BackSelect(void);			// �I����Ԃɖ߂�
	void CreateSingleUI(CUI** ppUI, string path, D3DXVECTOR3 pos, float width, float height);
	float ScaleChange(float* angle,float speed, float range);	// �T�C�Y�ύX
	VOLUME_LEVEL CulcVolumeLevel(float volume);	// ���l���{�����[�����x���񋓂ɕϊ�

	// �����o�ϐ�
	// �I�u�W�F�N�g�n
	CUI *m_pOptionText;								// �u�I�v�V�����v�̃e�L�X�g
	SoundUIObject m_aSoundUIObj[PARAM_MAX - 1];		// �T�E���h�nUI�I�u�W�F�N�g
	CUI *m_pVibrationText;							// �U���̃e�L�X�g
	CUI *m_apVibrationPenguin[VIBRATIONSWITCH_MAX];	// �U���L��UI�̃y���M��
	CUI *m_pBackButtonUI;							// �߂�{�^����UI
	CUI *m_pAButtonUI;								// A�{�^����UI
	
	// �l�n
	static SettingFunc s_settingFunc[PARAM_MAX];	// �ݒ荀�ڂ̊֐��ɔ�Ԋ֐��|�C���^
	OPTIONPARAM m_optionParam;	// �ݒ蒆����
	float m_fOptionTextScale;	// �ݒ蒆���ڂ̊g�k�i0�`2PI�j
	static float m_fBGMVolume;			// BGM�{�����[��
	static float m_fSEVolume;			// SE�{�����[��
	static VIBRATIONSWITCH m_Vibration;		// �o�C�u�L��
	float m_fParamScale;		// �ݒ荀�ڂ̊g�k�i0�`2PI�j
	bool m_bSetting;			// �ݒ蒆�t���O
};

#endif