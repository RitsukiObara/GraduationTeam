//*****************************************************
//
// �J�����̏���[camera.h]
// Author:���R����
//
//*****************************************************

#ifndef _CAMERA_H_
#define _CAMERA_H_

// �}�N����`
#define MIN_DRAW		(10.0f)					// �`����J�n���鋗��
#define MAX_DRAW		(15000.0f)				// �`����I�����鋗��

//*****************************************************
// �N���X�̒�`
//****************************************************
class CCamera
{
public:
	typedef struct
	{
		D3DXVECTOR3 posV;							//���_
		D3DXVECTOR3 posVOld;						//���_�̑O��̈ʒu
		D3DXVECTOR3 posR;							//�����_
		D3DXVECTOR3 posVDest;						//�ړI�̎��_
		D3DXVECTOR3 posRDest;						//�ړI�̒����_
		D3DXVECTOR3 vecU;							//������x�N�g��
		D3DXVECTOR3 rot;							//����
		float fLength;								//���_���璍���_�܂ł̋���
		D3DXMATRIX mtxProjection;					//�v���W�F�N�V�����}�g���b�N�X
		D3DXMATRIX mtxView;							//�r���[�}�g���b�N�X
	}Camera;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void SetPosV(void);
	void SetPosR(void);
	void Control(void);
	Camera *GetCamera(void);

private:
	Camera m_camera;												//�\���̂̏��
};

#endif