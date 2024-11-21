//*****************************************************
//
// モード選択ステート[npcpenguinstate_selectmode.h]
// Author:石原颯馬
//
//*****************************************************
#ifndef _NPCPENGUINSTATE_SELECTMODE_H_
#define _NPCPENGUINSTATE_SELECTMODE_H_

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
class CNPCPenguinState_Stand : public INPCPenguinState
{
public:
	CNPCPenguinState_Stand(){ m_nCounter = 0; }
	~CNPCPenguinState_Stand(){}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// 歩きステート
//****************************************
class CNPCPenguinState_Move : public INPCPenguinState
{
public:
	CNPCPenguinState_Move() { m_nCounter = 0; }
	~CNPCPenguinState_Move() {}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// 転びステート
//****************************************
class CNPCPenguinState_Fall : public INPCPenguinState
{
public:
	CNPCPenguinState_Fall() {}
	~CNPCPenguinState_Fall() {}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
};

//****************************************
// 首振りステート
//****************************************
class CNPCPenguinState_ShakeHead : public INPCPenguinState
{
public:
	CNPCPenguinState_ShakeHead() { m_nCounter = 0; }
	~CNPCPenguinState_ShakeHead() {}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// 腹ベタステート
//****************************************
class CNPCPenguinState_Stomach : public INPCPenguinState
{
public:
	CNPCPenguinState_Stomach() { m_nCounter = 0; }
	~CNPCPenguinState_Stomach() {}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
private:
	int m_nCounter;	// ステート実行カウンタ
};

//****************************************
// うにょんステート
//****************************************
class CNPCPenguinState_UNYO : public INPCPenguinState
{
public:
	CNPCPenguinState_UNYO() {}
	~CNPCPenguinState_UNYO() {}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
};

//****************************************
// 羽パタパタステート
//****************************************
class CNPCPenguinState_WingPTPT : public INPCPenguinState
{
public:
	CNPCPenguinState_WingPTPT() {}
	~CNPCPenguinState_WingPTPT() {}
	HRESULT Init(CNPCPenguin* pPenguin);
	void Uninit(void);
	void Update(CNPCPenguin* pPenguin);
};

#endif