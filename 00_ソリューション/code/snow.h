//*****************************************************
//
// �Ꮘ��[snow.h]
// Author:����F�M
//
//*****************************************************

#ifndef _SNOW_H_
#define _SNOW_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "polygon3D.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSnow : public CPolygon3D
{
public:
	CSnow(int nPriority = 4);	// �R���X�g���N�^
	~CSnow();	// �f�X�g���N�^

	static CSnow* Create(D3DXVECTOR3 pos, float fRadius, int nLife, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f });

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Move(void);
	static void SetSnow(int nMaxPos_X,int nMinPos_X ,float fHeight, int nMaxRadius, int nMinRadius, int nLife, D3DXVECTOR3 move, int nFrequency);

private:
	int m_nLife;	// ����
	D3DXVECTOR3 m_move;
	static int m_nsnowCnt;				// �Ⴊ����^�C�~���O�J�E���g
};

#endif