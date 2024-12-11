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
class CGameObject;
class CMeshCylinder;
class CFan3D;
class CUI;

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
	// �\���̒�`
	struct S_InfoStage
	{// �X�e�[�W���
		CObjectX *pModel;				// X���f���̃|�C���^
		string pathModel;				// ���f���̃p�X
		string pathMap;					// �}�b�v�̃p�X
		string pathEnemy;				// �G�̃p�X
		string pathName;				// �X�e�[�W���̃p�X
		int nDirStream;					// �C���̌���
		D3DXVECTOR3 pos;				// �ʒu
		CCollisionSphere *pCollision;	// �����蔻��
		E_StateStage state;				// ���
		float fScaleDest;				// �ڕW�̃X�P�[��
		CUI *pName;						// ���O�̃|�C���^

		// �R���X�g���N�^
		S_InfoStage() : pModel(nullptr), pos(), pCollision(nullptr), state(E_StateStage::STATE_NONE), fScaleDest(0.0f), pName(nullptr) {}
	};

	CSelectStageManager();	// �R���X�g���N�^
	~CSelectStageManager() {};	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// �ÓI�����o�֐�
	static void Load(void);						// �ǂݍ��ݏ���
	static void LoadMulti(void);				// �}���`�̓ǂݍ��ݏ���
	static CSelectStageManager *Create(void);	// ��������
	static vector<S_InfoStage*> GetInfoStage(void) { return s_aInfoStage; }				// �X�e�[�W���擾
	static vector<S_InfoStage*> GetInfoStageMulti(void) { return s_aInfoStageMulti; }	// �}���`�X�e�[�W���擾

private:
	// �����o�֐�
	void SetStage(void);						// �X�e�[�W�̐ݒu
	void Select(void);							// �I������
	void Scaling(S_InfoStage *pInfoStage);		// �X�P�[�����O����
	void FollowOcean(S_InfoStage *pInfoStage);	// �g�ɒǏ]
	void SetParticle(int nIdx);					// �p�[�e�B�N���̔���

	void StartEnter(void);	// �G���^�[�J�n
	void StayEnter(void);	// �G���^�[���̏���
	void EndEnter(void);	// �G���^�[�I��
	void CollisionAdultWall(void);	// ��l�̕ǔ���

	void ModeSelectBack(void);	// ���[�h�Z���N�g�ɖ߂�֐�

	void Debug(void);	// �f�o�b�O����

	// �ÓI�����o�֐�
	static void LoadStage(std::ifstream& file, string str, S_InfoStage *pInfoStage);		// �X�e�[�W���̓Ǎ�

	// �����o�ϐ�
	static vector<S_InfoStage*> s_aInfoStage;		// �X�e�[�W���̔z��
	static vector<S_InfoStage*> s_aInfoStageMulti;	// �}���`�X�e�[�W���̔z��
	CSelectStagePenguin *m_pPenguin;	// �y���M��
	bool m_bEnter;	// �G���^�[�����t���O
	float m_fTimerFade;	// �t�F�[�h�܂ł̃^�C�}�[
	int m_nIdxSelect;	// �I�񂾃X�e�[�W�ԍ�
	vector<CGameObject*> m_aParticlePos;	// �p�[�e�B�N���̈ʒu�p�I�u�W�F�N�g
	CUI *m_pButtonUI;	// �{�^��UI
};

#endif