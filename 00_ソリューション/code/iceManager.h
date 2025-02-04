//*****************************************************
//
// 氷マネージャー[iceManager.h]
// Author:��山桃也
//
//*****************************************************
#ifndef _ICEMANAGER_H_
#define _ICEMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"
#include "ice.h"
#include "ocean.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CIce;	// 氷
class CFlowIce;	// 流氷システム

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
	enum E_Pecker
	{// 突っついてくる人の番号
		PECKER_PLAYER0 = 0,	// 0番目のプレイヤー
		PECKER_PLAYER1,		// 1番目のプレイヤー
		PECKER_PLAYER2,		// 2番目のプレイヤー
		PECKER_PLAYER3,		// 3番目のプレイヤー
		PECKER_MAX
	};
	enum E_PeckResult
	{
		PECK_OK = 0,		// 突ける
		PECK_NONE_ICE,		// 氷がない
		PECK_UNBREAKABLE,	// 突っつけない
		PECK_ALREADY,		// 既に突いてる
		PECK_TOPANYOBJECT,	// 何かしら乗っている
		PECKRESULT_MAX
	};
	// 構造体定義
	struct S_Grid
	{// グリッド情報
		D3DXVECTOR3 pos;	// 位置
		CIce *pIce;	// 氷のポインタ
	};

	CIceManager(int nPriority = 2);	// コンストラクタ
	~CIceManager();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	CIce *CreateIce(int nGridV, int nGridH, CIce::E_Type type = CIce::E_Type::TYPE_NORMAL);	// 氷の生成
	CIce *CreateFlowIce(int nGridV,int nGridH, CIce::E_Type type = CIce::E_Type::TYPE_NORMAL);	// 流氷の生成
	void StopIce(CIce *pIce);	// 氷の停止
	E_PeckResult CheckPeck(int nNumV, int nNumH, float fRot, D3DXVECTOR3 pos, E_Direction *pDir = nullptr,CIce **ppIce = nullptr);	// 氷をつつけるかのチェック
	bool PeckIce(int nNumV, int nNumH, float fRot, D3DXVECTOR3 pos, bool *bResultBreak = nullptr);	// 氷をつつく
	bool PeckIce(int nIdxV, int nIdxH, bool *bResultBreak = nullptr,int nIdxStandV = -1, int nIdxStandH = -1);	// 氷をつつく
	void AddIce(CIce *pIce, D3DXVECTOR3 pos);	// 氷の追加
	bool FindIce(int nNumV, int nNumH, int nIdx,CIce *pIceStand,vector<CIce*> apIceLast,bool bBreak);	// アイスの発見
	vector<CIce*> GetAroundIce(int nNumV, int nNumH);	// 周辺の氷を取得
	void DeleteIce(CIce *pIce);	// 指定したポインタと同じ氷を削除する
	void Collide(D3DXVECTOR3 *pPos, int nIdxV, int nIdxH,float fRate);	// 外に出さない判定処理
	void Collide(D3DXVECTOR3 *pPos, CIce *pice,float fRate = 0.5f);	// 外に出さない判定処理
	bool GetIdxGridFromPosition(D3DXVECTOR3 pos, int *pIdxV, int *pIdxH,float fRate = 0.7f);	// グリッド番号を位置から取得する処理
	bool IsInIce(D3DXVECTOR3 pos, CIce *pIce, float fRate = 0.7f);	// 氷に乗ってるかの判定
	bool SetIceInGrid(int nNumV, int nNumH, CIce *pIce);	// グリッドに氷を設定
	bool CheckIceInGrid(int nNumV, int nNumH, CIce *pIce);	// グリッドに氷が入れるかのチェック
	void GetIceIndex(CIce *pIce, int *pNumV, int *pNumH);	// 氷のグリッド番号を取得
	CIce* GetLeftDownIdx(int *pNumV, int *pNumH);	// 左下の氷取得
	CIce* GetLeftUpIdx(int *pNumV, int *pNumH);	// 左上の氷取得
	CIce* GetRightDownIdx(int *pNumV, int *pNumH);	// 右上の氷取得
	CIce* GetRightUpIdx(int *pNumV, int *pNumH);	// 右上の氷取得
	bool IsInGrid(D3DXVECTOR3 pos, float fRate);	// グリッド内かの取得
	CIce* GetRandomIce(int *pNumV = nullptr, int *pNumH = nullptr);	// ランダムな氷の取得
	float GetRateNumIce(void);	// 氷の数の割合を取得
	bool IsIceInGrid(CIce *pIce);	// グリッドに氷があるかの確認

	S_Grid GetLeftDownGrid(void) { return m_aGrid[0][0]; }												// 左下のグリッド取得
	S_Grid GetLeftUpGrid(void) { return m_aGrid[m_nNumGridVirtical - 1][0]; }							// 左上のグリッド取得
	S_Grid GetRightDownGrid(void) { return m_aGrid[0][m_nNumGridHorizontal - 1]; };						// 右下のグリッド取得
	S_Grid GetRightUpGrid(void) { return m_aGrid[m_nNumGridVirtical - 1][m_nNumGridHorizontal - 1]; }	// 右上のグリッド取得
	S_Grid *GetGrid(int nIdxV, int nIdxH) { return &m_aGrid[nIdxV][nIdxH]; }							// 指定番号グリッドの取得

	void DisableFind(void);	// 探索フラグの無効化
	void Load(const char* pPath);	// 初期配置読み込み
	void BindRippleElements(void);	// さざ波の属性を割り当てる処理
	CIce *GetNearestIce(D3DXVECTOR3 pos, int *pNumV = nullptr, int *pNumH = nullptr);	// 最も近い氷の取得
	S_Grid *GetNearestEmptyGrid(D3DXVECTOR3 pos, int *pNumV = nullptr, int *pNumH = nullptr, CIce *pIce = nullptr,float fDistMax = FLT_MAX);	// 最も近い空グリッドの取得
	S_Grid *GetNearestGrid(D3DXVECTOR3 pos, int *pNumV = nullptr, int *pNumH = nullptr);	// 最も近いグリッドの取得

	// 変数取得・設定関数
	D3DXVECTOR3 GetGridPosition(int *pNumV, int *pNumH);
	CIce* GetGridIce(int* pNumV, int* pNumH);
	int GetNumGridV(void) { return m_nNumGridVirtical; }	// 縦のグリッド数
	int GetNumGridH(void) { return m_nNumGridHorizontal; }	// 横のグリッド数
	float GetOceanLevel(void) { return m_fOceanLevel; }	// 海流レベルの取得
	COcean::E_Stream GetDirStream(void) { return m_dirStream; }	// 海流の方向取得
	COcean::E_Stream GetDirStreamNext(void) { return m_dirStreamNext; }	// 次の海流の方向取得
	vector<CIce*> GetFlows(void);	// 流れてる氷の取得

	void SetOceanLevel(float level) { m_fOceanLevel = level; }
	void SetDirStream(COcean::E_Stream direction) { m_dirStream = direction; }	// 海流の方向
	void SetDirStreamNext(COcean::E_Stream directionNext) { m_dirStreamNext = directionNext; }	// 次の海流の方向

	// 静的メンバ関数
	static CIceManager *Create(int nNumV, int nNumH);
	static CIceManager *GetInstance() { return s_pIceManager; }	// インスタンスの取得

private:
	// メンバ関数
	void SetGridPos(void);	// グリッドの位置を計算
	void ManageStateIce(void);	// 氷の状態管理
	void SearchInvailStopIce(void);	// 無効な停止氷の検出
	void SearchNotConnectIce(vector<CIce*> &rpIce);	// 繋がってない氷の検出
	bool JudgeBetweenPeck(int nNumV, int nNumH);	// つっついた氷に挟まれてる判定
	E_PeckResult CanPeck(CIce* pIce,int nNumV, int nNumH,CObject *pOwn = nullptr);	// つっつける氷かのチェック
	bool CheckIdxPlayer(CObject *pObject, int nNumV, int nNumH);
	void DisableFromHardIce(int nNumV, int nNumH,bool bPeck = true);	// 硬い氷から信号を出して、破壊信号を解除
	void DisableFromPlayer(int nNumV, int nNumH, CIce *pIcePeck,vector<CIce*> apIce);	// プレイヤーから信号を出して、破壊信号を解除
	void DisableBreak(int nNumV, int nNumH);					// 氷の破壊を解除
	void SummarizeIce(int nNumV, int nNumH);					// 氷をまとめる処理
	void SaveFlowIce(int nNumV, int nNumH, CFlowIce *pFlowIce);	// 流氷にまとめる処理
	bool BreakIce(void);										// 氷の破壊
	bool CheckCorner(int nNumV, int nNumH);						// 角の確認
	void BreakPeck(int nNumV, int nNumH);						// 突っついた氷を沈める
	void LimitInIce(D3DXVECTOR3 *pPos, int nNumV, int nNumH);	// 氷内に収める処理
	void Debug(void);											// デバッグ処理

	// メンバ変数
	int m_nNumGridVirtical;	// 縦グリッドの数
	int m_nNumGridHorizontal;	// 横グリッドの数
	float m_fOceanLevel;	// 海流の強さ
	vector<vector<S_Grid>> m_aGrid;	// グリッドの配列
	COcean::E_Stream m_dirStream;	// 海流の方向
	COcean::E_Stream m_dirStreamNext;	// 次の海流の方向

	// 静的メンバ変数
	static CIceManager *s_pIceManager;	// 自身のポインタ
};

namespace Grid
{
const float SIZE = 200.0f;	// グリッドのサイズ
inline void CalcAroundGrids(int nNumV, int nNumH, int* aV, int* aH)
{
	// 右上・右下
	aV[CIceManager::DIRECTION_RIGHTUP] = nNumV + 1;
	aV[CIceManager::DIRECTION_RIGHTDOWN] = nNumV - 1;

	if (nNumV % 2 == 0) {  // 偶数の時
		aH[CIceManager::DIRECTION_RIGHTUP] = nNumH + 1;
		aH[CIceManager::DIRECTION_RIGHTDOWN] = nNumH + 1;
		aH[CIceManager::DIRECTION_LEFTUP] = nNumH;
		aH[CIceManager::DIRECTION_LEFTDOWN] = nNumH;
	}
	else {  // 奇数の時
		aH[CIceManager::DIRECTION_RIGHTUP] = nNumH;
		aH[CIceManager::DIRECTION_RIGHTDOWN] = nNumH;
		aH[CIceManager::DIRECTION_LEFTUP] = nNumH - 1;
		aH[CIceManager::DIRECTION_LEFTDOWN] = nNumH - 1;
	}

	// 左側・左右
	aV[CIceManager::DIRECTION_LEFTUP] = nNumV + 1;
	aV[CIceManager::DIRECTION_LEFTDOWN] = nNumV - 1;
	aV[CIceManager::DIRECTION_RIGHT] = nNumV;
	aV[CIceManager::DIRECTION_LEFT] = nNumV;
	aH[CIceManager::DIRECTION_RIGHT] = nNumH + 1;
	aH[CIceManager::DIRECTION_LEFT] = nNumH - 1;
}
}

namespace stream
{
const D3DXVECTOR3 VECTOR_STREAM[COcean::E_Stream::STREAM_MAX] =
{
	{ 0.0f, 0.0f, 1.0f },   // 上
	{ 1.0f, 0.0f, 0.0f },   // 右
	{ 0.0f, 0.0f, -1.0f }, // 下
	{ -1.0f, 0.0f, 0.0f },  // 左
};
}

#endif

