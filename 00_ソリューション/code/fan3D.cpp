//*****************************************************
//
// 3D~Μ[fan3D.h]
// Author:ϋόRη
//
//*****************************************************

//*****************************************************
// CN[h
//*****************************************************
#include "fan3D.h"
#include "renderer.h"
#include "texture.h"
#include "universal.h"
#include "debugproc.h"

//*****************************************************
// θθ`
//*****************************************************
namespace
{
	const float SWING_WIDTH = 10.0f;  // sinJ[uΜUκ
}

//=====================================================
// DζΚπίιRXgN^
//=====================================================
CFan3D::CFan3D(int nPriority) : CFan(nPriority)
{
	m_bBillboard = false;
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));
	m_nStartOffsetCount = 0;
}

//=====================================================
// fXgN^
//=====================================================
CFan3D::~CFan3D()
{

}

//=====================================================
// Ά¬
//=====================================================
CFan3D *CFan3D::Create(int nPriority, int nNumVtx)
{
	CFan3D *pFan3D = nullptr;

	// CX^XΆ¬
	pFan3D = new CFan3D(nPriority);

	if (pFan3D != nullptr)
	{
		pFan3D->SetNumVtx(nNumVtx);

		// ϊ»
		pFan3D->Init();
	}

	return pFan3D;
}

//=====================================================
// ϊ»
//=====================================================
HRESULT CFan3D::Init(void)
{
	CFan::Init();

	// foCXΜζΎ
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (pDevice != nullptr)
	{
		LPDIRECT3DVERTEXBUFFER9 *pVtxBuff = GetVtxBuff();
		int nNumVtx = GetNumVtx();

		if (*pVtxBuff == nullptr)
		{
			// Έ_obt@ΜΆ¬
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (nNumVtx + 2),
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				pVtxBuff,
				nullptr);
		}
	}

	// Έ_έθ
	SetVtx();

	EnableLighting(false);

	return S_OK;
}

//=====================================================
// IΉ
//=====================================================
void CFan3D::Uninit(void)
{
	CFan::Uninit();
}

//=====================================================
// XV
//=====================================================
void CFan3D::Update(void)
{

}

//=====================================================
// Έ_Κuέθ
//=====================================================
void CFan3D::SetVtx(void)
{
	// Έ_ξρΜ|C^
	VERTEX_3D *pVtx;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();
	int nNumVtx = GetNumVtx();
	float fAngleMax = GetAngleMax();
	float fRadius = GetRadius();
	float fRateAngle = GetRateAngle();

	if (pVtxBuff != nullptr)
	{
		// Έ_obt@πbN΅AΈ_ξρΦΜ|C^πζΎ
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// SΜΈ_Μέθ
		pVtx[0].pos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pVtx[0].tex = D3DXVECTOR2{ 0.5f,0.5f };
		pVtx[0].col = GetCol();

		for (int i = 1;i < nNumVtx + 2;i++)
		{// ~όΜΈ_Μέθ
			float fAngle = (fAngleMax * fRateAngle) * ((float)(i - 1) / (float)nNumVtx);

			universal::LimitRot(&fAngle);

			pVtx[i].pos =
			{
				sinf(fAngle) * fRadius,
				cosf(fAngle) * fRadius,
				0.0f,
			};

			float f = 0.1f * sinf((float)i * 0.01f);

			D3DXVECTOR2 tex =
			{
				0.5f + sinf(fAngle) * 0.5f,
				0.5f - cosf(fAngle) * 0.5f,
			};

			pVtx[i].tex = tex;

			pVtx[i].col = GetCol();
		}

		// Έ_obt@ΜAbN
		pVtxBuff->Unlock();
	}
}

//=====================================================
// Fέθ
//=====================================================
void CFan3D::SetCol(D3DXCOLOR col)
{
	CFan::SetCol(col);

	// Έ_ξρΜ|C^
	VERTEX_3D *pVtx;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();

	int nNumVtx = GetNumVtx();

	if (pVtxBuff != nullptr)
	{
		// Έ_obt@πbN΅AΈ_ξρΦΜ|C^πζΎ
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < nNumVtx + 2; nCnt++)
		{
			pVtx[nCnt].col = col;
		}

		// Έ_obt@ΜAbN
		pVtxBuff->Unlock();
	}
}

//=====================================================
// eNX`ΐWέθ
//=====================================================
void CFan3D::SetTex(float fScrollX, float fScrollY)
{
	// Έ_ξρΜ|C^
	VERTEX_3D* pVtx;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();
	int nNumVtx = GetNumVtx();
	float fAngleMax = GetAngleMax();
	float fRadius = GetRadius();
	float fRateAngle = GetRateAngle();

	if (pVtxBuff != nullptr)
	{
		// Έ_obt@πbN΅AΈ_ξρΦΜ|C^πζΎ
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < nNumVtx + 2; i++)
		{
			float fTexY = fScrollY * sinf((float)(m_nStartOffsetCount + i) / SWING_WIDTH);

			pVtx[i].tex.x = pVtx[i].tex.x + fScrollX;
			pVtx[i].tex.y = pVtx[i].tex.y + fTexY;
		}

		m_nStartOffsetCount++;
		
		// Έ_obt@ΜAbN
		pVtxBuff->Unlock();
	}
}

//=====================================================
// `ζ
//=====================================================
void CFan3D::Draw(void)
{
	// foCXΜζΎ
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// Έ_obt@ΜζΎ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();

	if (pDevice != nullptr && pVtxBuff != nullptr)
	{
		//[h}gbNXϊ»
		D3DXMatrixIdentity(&m_mtxWorld);

		if (m_bBillboard)
		{// r{[h`ζ
			DrawBillboard();
		}
		else
		{// ΚνΜ`ζ
			DrawNormal();
		}

		//[h}gbNXέθ
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//Έ_obt@πf[^Xg[Ιέθ
		pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));

		//Έ_tH[}bgΜέθ
		pDevice->SetFVF(FVF_VERTEX_3D);

		// JOπ³ψ»
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// |SΜ`ζ
		CFan::Draw();

		// JOπLψ»
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

//=====================================================
// Κν`ζ
//=====================================================
void CFan3D::DrawNormal(void)
{
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	//ό«π½f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//Κuπ½f
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}

//=====================================================
// r{[h`ζ
//=====================================================
void CFan3D::DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DXMATRIX mtxView, mtxTrans;
	D3DXVECTOR3 pos = GetPosition();

	//r[}gbNXζΎ
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//|SπJΙό―ι
	D3DXMatrixInverse(&m_mtxWorld, nullptr, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// Κuπ½f
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}