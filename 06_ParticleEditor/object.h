//*****************************************************
//
// オブジェクトの処理[object.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayer;
class CBlock;

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_OBJECT	(1024)	//	オブジェクトの数
#define NUM_PRIORITY	(8)	// 優先順位の最大数

//*****************************************************
// クラスの定義
//*****************************************************
class CObject
{
public:
	typedef enum
	{
		TYPE_NONE = 0,	// 何でもない
		TYPE_PLAYER,	// プレイヤー
		TYPE_ENEMY,	// 敵
		TYPE_BLOCK,	// ブロック
		TYPE_BULLET,	// 弾
		TYPE_PARTICLE,	// パーティクル
		TYPE_MAX
	}TYPE;

	CObject(int nPriority = 3);	// コンストラクタ
	virtual ~CObject();	//	デストラクタ

	virtual HRESULT Init(void) = 0;	// 初期化処理
	virtual void Uninit(void) = 0;	// 終了処理
	virtual void Update(void) = 0;	// 更新処理
	virtual void Draw(void) {};	// 描画処理
	static void ReleaseAll(void);	// 全てリリース
	static void UpdateAll(void);	// 全て更新
	static void DrawAll(void);	// 全て描画
	virtual void SetPosition(D3DXVECTOR3 pos) {};	// 設定処理
	virtual D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }	// 位置取得処理
	static CObject *GetObject(int nPriority,int nIdx) { return m_apObject[nPriority][nIdx]; }
	static int GetNumAll(void) { return m_nNumAll; }
	int GetID(void) { return m_nID; }

protected:
	void Release(void);	// 個別リリース処理

private:
	static CObject *m_apObject[NUM_PRIORITY][NUM_OBJECT];	// オブジェクトへのポインタ
	static int m_nNumAll;	// 総数
	int m_nID;	// 自分のID
	TYPE m_type;	// 種類
	int m_nPriority;	// 描画の優先順位
};

#endif