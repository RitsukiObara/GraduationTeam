//*****************************************************
//
// 氷マネージャー[iceManager.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _ICEMANAGER_H_
#define _ICEMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"
#include "ice.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CIce;	// 氷

//*****************************************************
// クラスの定義
//*****************************************************
class CIceManager : public CObject
{
public:
	// 列挙型定義
	enum E_Direction
	{// 方向
		DIRECTION_RIGHTUP = 0,	// 右上
		DIRECTION_RIGHT,	// 右
		DIRECTION_RIGHTDOWN,	// 右下
		DIRECTION_LEFTDOWN,	// 左下
		DIRECTION_LEFT,	// 左
		DIRECTION_LEFTUP,	// 左上
		DIRECTION_MAX
	};

	CIceManager(int nPriority = 2);	// コンストラクタ
	~CIceManager();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	CIce *CreateIce(int nGridV,int nGridH, CIce::E_Type type = CIce::E_Type::TYPE_NORMAL);	// 氷の生成
	void StopIce(CIce *pIce);	// 氷の停止
	void PeckIce(int nNumV, int nNumH, E_Direction direction);	// 氷をつつく
	void AddIce(CIce *pIce, D3DXVECTOR3 pos);	// 氷の追加
	bool FindIce(int nNumV, int nNumH, int nIdx,CIce *pIceStand,vector<CIce*> apIceLast,bool bBreak);	// アイスの発見
	vector<CIce*> GetAroundIce(int nNumV, int nNumH);	// 周辺の氷を取得

	// 変数取得・設定関数
	void EnableBreakIce(bool bBreakIce) { m_bBreakIce = bBreakIce; }	// 氷破壊フラグ
	bool IsBreakIce(void) { return m_bBreakIce; }
	D3DXVECTOR3 GetGridPosition(int *pNumV, int *pNumH);

	// 静的メンバ関数
	static CIceManager *Create(int nNumV, int nNumH);
	static CIceManager *GetInstance() { return s_pIceManager; }	// インスタンスの取得

private:
	// 列挙型定義
	enum E_StateGrid
	{// グリッドの状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_MID,	// 氷の真ん中がある状態
		STATE_CORNER,	// 氷の角がある状態
		STATE_MAX
	};
	// 構造体定義
	struct S_Grid
	{// グリッド情報
		D3DXVECTOR3 pos;	// 位置
		E_StateGrid state;	// 状態
		CIce *pIce;	// 氷のポインタ
	};
	
	// メンバ関数
	void SetGridPos(void);	// グリッドの位置を計算
	void ManageStateIce(void);	// 氷の状態管理
	bool JudgeBetweenPeck(int nNumV, int nNumH);	// つっついた氷に挟まれてる判定
	void DisableFind(void);	// 探索フラグの無効化
	void DisableFromPlayer(int nNumV, int nNumH, CIce *pIcePeck,vector<CIce*> apIce);	// プレイヤーから信号を出して、破壊信号を解除
	void DisableBreak(int nNumV, int nNumH);	// 氷の破壊を解除
	void BreakIce(void);	// 氷の破壊
	void CalcAroundGrids(int nNumV, int nNumH, int* aV, int* aH);	// 周辺グリッドの計算
	bool CheckStandBlock(vector<CIce*> apIce, CIce *pIce,int nIdx);
	bool CheckCorner(int nNumV, int nNumH);	// 角の確認
	bool CheckCommon(vector<CIce*> apIce, vector<CIce*> apIceLast,CIce* pIceStand, int nNumV, int nNumH,bool bBreakLast);
	void BreakPeck(int nNumV, int nNumH);	// 突っついた氷を沈める
	void Debug(void);	// デバッグ処理

	// メンバ変数
	int m_nNumGridVirtical;	// 縦グリッドの数
	int m_nNumGridHorizontal;	// 横グリッドの数
	vector<vector<S_Grid>> m_aGrid;	// グリッドの配列
	bool m_bBreakIce;	// 氷を壊すかのフラグ

	// 静的メンバ変数
	static CIceManager *s_pIceManager;	// 自身のポインタ
};

namespace Grid
{
const float SIZE = 200.0f;	// グリッドのサイズ
}

#endif

