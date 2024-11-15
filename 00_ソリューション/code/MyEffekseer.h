//===========================================================
//
// マイエフェクシア[MyEffekseer.h]
// Author : ��山桃也
//
//===========================================================
#ifndef _EFFEKSEER_H_     // このマクロが定義されてなかったら
#define _EFFEKSEER_H_     // 2重インクルードを防止のマクロを定義する

class CEffekseerEffect;

#define MAX_EFK   (64)

class CMyEffekseer
{// エフェクシアのマネージャー
public:
	CMyEffekseer();
	~CMyEffekseer();

	enum TYPE
	{// 種類
		TYPE_NONE = 0,  // なんもない
		TYPE_PECKWAVE,  // つっつき波紋
		TYPE_MAX
	};

	// メンバ関数
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CEffekseerEffect *CreateEffect(const char* FileName, ::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale);
	::EffekseerRendererDX9::RendererRef GetEffekseerRenderer(void) { return m_efkRenderer; }
	::Effekseer::ManagerRef GetEfkManager(void) { return m_efkManager; }
	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);
	void Release(int idx);
	void ReleaseEffect(CEffekseerEffect *pEffect);	// エフェクトの解放
	const char* GetPathEffect(CMyEffekseer::TYPE type);
	std::list<CEffekseerEffect*> *GetList(void) { return &m_listEffect; };	// リストの取得

	// 静的メンバ関数
	static CMyEffekseer *Create(void);
	static CMyEffekseer *GetInstance(void) { return s_pMyEffekseer; }	// インスタンス取得

private:
	// メンバ変数 
	EffekseerRendererDX9::RendererRef m_efkRenderer;
	::Effekseer::Matrix44 cameraMatrix;
	::Effekseer::Matrix44 projectionMatrix;
	::Effekseer::ManagerRef m_efkManager;
	::Effekseer::Vector3D viewerPosition;
	int m_nNum;
	std::list<CEffekseerEffect*> m_listEffect;	// エフェクトクラスのリスト

	// 静的メンバ変数
	static const char* m_apEfkName[CMyEffekseer::TYPE_MAX];
	static CMyEffekseer *s_pMyEffekseer;		// 自身のポインタ
};

class CEffekseerEffect
{
public:
	CEffekseerEffect();	// コンストラクタ
	~CEffekseerEffect();	// デストラクタ

	// メンバ関数
	void Init(::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale);	// 初期化
	void Uninit(void);	// 終了処理
	CEffekseerEffect *FollowPosition(D3DXVECTOR3 pos);	// 位置の追従

	// 変数取得・設定関数
	void SetEffect(Effekseer::EffectRef effect) { m_effect = effect; }	// エフェクト参照
	Effekseer::EffectRef GetEffect(void) { return m_effect; }
	void SetHandle(Effekseer::Handle handle) { m_efkHandle = handle; }	// ハンドル
	Effekseer::Handle GetHandle(void) { return m_efkHandle; }
	void SetTime(int32_t time) { m_time = time; }	// 時間
	int32_t GetTime(void) { return m_time; }
	Effekseer::Vector3D GetPosition(void) { return m_pos; }	// 位置
	D3DXVECTOR3 GetPositionD3D(void) { return D3DXVECTOR3(m_pos.X, m_pos.Y, m_pos.Z); }
	void SetPosition(Effekseer::Vector3D pos) { m_pos = pos; }
	Effekseer::Vector3D GetRotation(void) { return m_rot; }	// 向き
	D3DXVECTOR3 GetRotationD3D(void) { return D3DXVECTOR3(m_rot.X, m_rot.Y, m_rot.Z); }
	void SetRotation(Effekseer::Vector3D rot) { m_rot = rot; }
	Effekseer::Vector3D GetScale(void) { return m_scale; }	// スケール
	void SetScale(Effekseer::Vector3D scale) { m_scale = scale; }

private:
	// メンバ変数
	Effekseer::Vector3D m_pos;       // 位置
	Effekseer::Vector3D m_rot;       // 向き
	Effekseer::Vector3D m_scale;     // 大きさ
	Effekseer::EffectRef m_effect;   // エフェクト
	Effekseer::Handle m_efkHandle;   // ハンドル
	const char* m_EfkName;           // 名前
	int32_t m_time;                  // 時間
};

namespace MyEffekseer
{
CEffekseerEffect *CreateEffect(CMyEffekseer::TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f }, D3DXVECTOR3 scale = { 100.0f,100.0f,100.0f });
}
#endif