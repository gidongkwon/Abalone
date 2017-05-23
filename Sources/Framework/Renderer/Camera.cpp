#include "pch.h"
#include "Camera.h"

Camera::Camera():
	position(0, 0, -1),
	right(1, 0, 0),
	up(0, 1, 0),
	facing(0, 0, 1)
{
	set_lens(0.25f * XM_PI, 1.0f, 1.0f, 1000.0f);
}

void Camera::set_lens(float fov_y, float aspect_ratio, float near_z, float far_z)
{
	XMMATRIX proj = XMMatrixPerspectiveFovLH(fov_y, aspect_ratio, near_z, far_z);
	XMStoreFloat4x4(&proj_, proj);
}

void Camera::look_at(FXMVECTOR target)
{
	XMVECTOR position = XMLoadFloat3(&this->position);
	XMVECTOR up = XMLoadFloat3(&this->up);

	XMMATRIX view = XMMatrixLookAtLH(position, target, up);
	XMStoreFloat4x4(&view_, view);
}

XMMATRIX Camera::view() const
{
	return XMLoadFloat4x4(&view_);
}

XMMATRIX Camera::proj() const
{
	return XMLoadFloat4x4(&proj_);
}

XMMATRIX Camera::view_proj() const
{
	return view() * proj();
}
