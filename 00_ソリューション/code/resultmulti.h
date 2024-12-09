//*****************************************************
//
// �}���`���[�h�̃��U���g����[resultmulti.h]
// Author:�Ό��D�n
//
//*****************************************************
#ifndef _RESULT_MULTI_H_
#define _RESULT_MULTI_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CResultMulti : public CScene
{
public:
	CResultMulti();		// �R���X�g���N�^
	~CResultMulti();	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	
private:
	// �����o�֐�
	void SetUI(CUI** pUI, string path, D3DXVECTOR3 pos, float fWidth, float fHeight);
	void InitNormal(int nIdxWinner);	// �ʏ�̏�����
	void InitDraw(void);	// ���������̏�����

	// �����o�ϐ�
	CUI *m_pWinnerNum;		// ���Ҕԍ�UI
	CUI *m_pWinnerText;		// ���҃e�L�X�gUI
	bool m_bDraw;			// ���������t���O
};

#endif