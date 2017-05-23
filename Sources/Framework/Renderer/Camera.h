#pragma once

class Camera
{
public:
	Camera();

	void set_lens(float fov_y, float aspect_ratio, float near_z, float far_z);
	void XM_CALLCONV look_at(FXMVECTOR target);

	XMMATRIX view() const;
	XMMATRIX proj() const;
	XMMATRIX view_proj() const;

	XMFLOAT3 position;
	XMFLOAT3 right;
	XMFLOAT3 up;
	XMFLOAT3 facing;
private:
	XMFLOAT4X4 view_;
	XMFLOAT4X4 proj_;
};