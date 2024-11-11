//*****************************************************
//
// �X�e�[�W�I���Ǘ�����[SelectStageManager.h]
// Author:���R����
//
//*****************************************************
#ifndef _SELECTSTAGEMANAGER_H_
#define _SELECTSTAGEMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "selectStageManager.h"
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
class CObjectX;
class CSelectStagePenguin;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSelectStageManager : public CObject
{
public:
	// �񋓌^��`
	enum E_StateStage
	{// �X�e�[�W�̏��
		STATE_NONE = 0,	// �������ĂȂ����
		STATE_NORMAL,	// �ʏ���
		STATE_SELECT,	// �I�����
		STATE_MAX
	};

	CSelectStageManager();	// �R���X�g���N�^
	~CSelectStageManager() {};	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Load(void);	// �ǂݍ��ݏ���

	// �ÓI�����o�֐�
	static CSelectStageManager *Create(void);	// ��������

private:
	// �\���̒�`
	struct S_InfoStage
	{// �X�e�[�W���
		CObjectX *pModel;	// X���f���̃|�C���^
		string pathModel;	// ���f���̃p�X
		D3DXVECTOR3 pos;	// �ʒu
		CCollisionSphere *pCollision;	// �����蔻��
		E_StateStage state;	// ���
		float fScaleDest;	// �ڕW�̃X�P�[��

		// �R���X�g���N�^
		S_InfoStage() : pModel(nullptr), pos(), pCollision(nullptr), state(E_StateStage::STATE_NONE), fScaleDest(0.0f) {}
	};

	// �����o�֐�
	void LoadStage(std::ifstream& file, string str, S_InfoStage *pInfoStage);	// �X�e�[�W���̓Ǎ�
	void SetStage(void);	// �X�e�[�W�̐ݒu
	void Select(void);	// �I������
	void Scaling(S_InfoStage *pInfoStage);	// �X�P�[�����O����

	void StartEnter(void);	// �G���^�[�J�n
	void StayEnter(void);	// �G���^�[���̏���
	void EndEnter(void);	// �G���^�[�I��

	void Debug(void);	// �f�o�b�O����

	// �����o�ϐ�
	vector<S_InfoStage*> m_aInfoStage;	// �X�e�[�W���̔z��
	CSelectStagePenguin *m_pPenguin;	// �y���M��
	bool m_bEnter;	// �G���^�[�����t���O
	float m_fTimerFade;	// �t�F�[�h�܂ł̃^�C�}�[
	int m_nIdxSelect;	// �I�񂾃X�e�[�W�ԍ�

	// �ÓI�����o�ϐ�
};

#endif