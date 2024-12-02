//*****************************************************
//
// タイトル用NPCペンギンステート[npcpenguinstate_title.h]
// Author:石原颯馬
//
//*****************************************************
#ifndef _NPCPENGUINSTATE_TITLE_H_
#define _NPCPENGUINSTATE_TITLE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "npcpenguin.h"

//*****************************************************
// 前方宣言
//*****************************************************
__interface INPCPenguinState;

//*****************************************************
// クラスの定義
//*****************************************************
//****************************************
// 待機ステート
//****************************************
class CNPCPenguinState_StandTitle : public INPCPenguinState
{
public:
	CNPCPenguinState_StandTitle(){ m_nCounter = 0; }
	~CNPCPenguinState_StandTitle(){}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// 首振りステート
//****************************************
class CNPCPenguinState_ShakeHeadTitle : public INPCPenguinState
{
public:
	CNPCPenguinState_ShakeHeadTitle() { m_nCounter = 0; }
	~CNPCPenguinState_ShakeHeadTitle() {}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// 腹ベタステート
//****************************************
class CNPCPenguinState_StomachTitle : public INPCPenguinState
{
public:
	CNPCPenguinState_StomachTitle() { m_nCounter = 0; }
	~CNPCPenguinState_StomachTitle() {}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// うにょんステート
//****************************************
class CNPCPenguinState_UNYOTitle : public INPCPenguinState
{
public:
	CNPCPenguinState_UNYOTitle() {}
	~CNPCPenguinState_UNYOTitle() {}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
};

//****************************************
// 羽パタパタステート
//****************************************
class CNPCPenguinState_WingPTPTTitle : public INPCPenguinState
{
public:
	CNPCPenguinState_WingPTPTTitle() {}
	~CNPCPenguinState_WingPTPTTitle() {}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
};

#endif