//*****************************************************
//
// マルチモードのリザルトステート[npcpenguinstate_resultmulti.h]
// Author:石原颯馬
//
//*****************************************************
#ifndef _NPCPENGUINSTATE_RESULTMULTI_H_
#define _NPCPENGUINSTATE_RESULTMULTI_H_

//*****************************************************
// インクルード
//*****************************************************
#include "npcpenguin.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;

//*****************************************************
// クラスの定義
//*****************************************************
//****************************************
// 勝者バンザイステート
//****************************************
class CNPCPenguinState_BANZAI : public INPCPenguinState
{
public:
	CNPCPenguinState_BANZAI(){ m_nCounter = 0; }
	~CNPCPenguinState_BANZAI(){}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// 勝者ジャンプステート
//****************************************
class CNPCPenguinState_Jump : public INPCPenguinState
{
public:
	CNPCPenguinState_Jump() { m_nCounter = 0; }
	~CNPCPenguinState_Jump() {}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// 敗者逃げステート
//****************************************
class CNPCPenguinState_Flee : public INPCPenguinState
{
public:
	CNPCPenguinState_Flee() {}
	~CNPCPenguinState_Flee() {}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
};

#endif