//*****************************************************
//
// X�t�@�C���̏���[objectX.cpp]
// Author:���R����
//
//*****************************************************
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object3D.h"
#include "model.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CObjectX : public CObject3D
{
public:
	CObjectX(int nPriority = 5);	// �R���X�g���N�^
	~CObjectX();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void JustDraw(void);
	void DrawShadow(void);
	static CObjectX *Create(D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f }, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f }, int nPriority = 3);
	float GetWidth(void) { return 0.0f; }	// �T�C�Y�擾
	void SetPositionOld(D3DXVECTOR3 pos) { m_posOld = pos; }	// �ݒ菈��
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// �ݒ菈��
	D3DXVECTOR3 GetMove(void) { return m_move; }	// �擾����
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	void BindModel(int nIdx);
	int GetIdxModel(void) { return m_nIdxModel; }
	void SetIdxModel(int nIdx) { m_nIdxModel = nIdx; }
	void SetRadius(void);
	float GetRadius(void) { return m_fRadius; }
	void CalcMatrix(void);
	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }	// �擾����
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; }	// �擾����
	void SetVtxMax(D3DXVECTOR3 vtx) { m_vtxMax = vtx; }
	void SetVtxMin(D3DXVECTOR3 vtx) { m_vtxMin = vtx; }
	void EnableShadow(bool bEnable) { m_bShadow = bEnable; }
	bool IsDisp(void) { return m_bDisp; }
	void EnableDisp(bool bDisp) { m_bDisp = bDisp; }
	D3DXMATERIAL GetMaterial(int nIdx);					// �}�e���A��
	void SetMaterial(D3DXMATERIAL mat, int nIdx);
	D3DXCOLOR GetDeffuseeCol(int nIdx = 0);
	void SetDeffuseCol(D3DXCOLOR col, int nIdx = 0);
	void ResetMat(void);

private:
	D3DXVECTOR3 m_posOld;			// �O��̈ʒu
	D3DXVECTOR3 m_move;				// �ړ���
	D3DXCOLOR m_col;				// �F
	CModel::Model *m_pModel;		// ���f�����
	int m_nIdxModel;				// ���f���̔ԍ�
	float m_fRadius;				// ���f���̔��a
	D3DXVECTOR3 m_vtxMax;			// �ő咸�_
	D3DXVECTOR3 m_vtxMin;			// �ŏ����_
	vector<D3DXMATERIAL> m_aMat;	// �ω��p�̃}�e���A��
	bool m_bChangeMat;				// �F�ύX�t���O
	bool m_bShadow;					// �e�`��t���O
	bool m_bDisp;					// �`��t���O
};

#endif