//*****************************************************
//
// 氷の処理[ice.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ICE_H_
#define _ICE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMeshCylinder;
class CFan3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CIce : public CGameObject
{
public:
	// 列挙型定義
	enum E_State
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_STOP,	// 停止状態
		STATE_FLOWS,	// 流れてる状態
		STATE_MAX
	};
	enum E_Type
	{// 種類
		TYPE_NORMAL = 0,	// 通常
		TYPE_HARD,	// 硬い氷
		TYPE_MAX
	};

	CIce(int nPriority = 2);	// コンストラクタ
	~CIce();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateMesh(void);	// メッシュの生成
	void SetTransform(float fRadius);	// トランスフォーム設定
	virtual bool IsCanPeck(void) { return true; };	// 突っつけるかの判定を返す

	// 変数取得・設定関数
	void SetState(E_State state) { m_state = state; }	// 状態
	E_State GetState(void) { return m_state; }
	void EnableCanFind(bool bCanFind) { m_bCanFind = bCanFind; }	// 探索可能フラグ
	bool IsCanFind(void) { return m_bCanFind; }
	void EnableBreak(bool bBreak) { m_bBreak = bBreak; }	// 破壊フラグ
	bool IsBreak(void) { return m_bBreak; }
	void EnablePeck(bool bPeck) { m_bPeck = bPeck; }	// 突っつかれたフラグ
	bool IsPeck(void) { return m_bPeck; }
	void EnableAliveStandIce(bool bPeck) { m_bAliveStandBlock = bPeck; }	// 立っている氷に辿り着いたフラグ
	bool IsAliveStandIce(void) { return m_bAliveStandBlock; }

	// 静的メンバ関数
	static CIce *Create(E_Type type = E_Type::TYPE_NORMAL, E_State state = E_State::STATE_FLOWS);	// 生成処理
	static int GetNumAll(void) { return s_nNumAll; }	// 総数取得
	static std::vector<CIce*> GetInstance(void) { return m_Vector; }

private:
	// メンバ関数
	void Flows(void);	// 流れる処理

	// メンバ変数
	E_State m_state;	// 状態
	bool m_bCanFind;	// 探索できるフラグ
	bool m_bBreak;	// 壊れるフラグ
	bool m_bPeck;	// 突っつかれたフラグ
	bool m_bAliveStandBlock;	// 立っているブロックに到達したフラグ
	CFan3D *m_pUp;	// 上側に貼る扇ポリゴン
	CMeshCylinder *m_pSide;	// サイドのシリンダー

	// 静的メンバ変数
	static int s_nNumAll;	// 総数
	static std::vector<CIce*> m_Vector;	// 自身のポインタ
};

#endif

