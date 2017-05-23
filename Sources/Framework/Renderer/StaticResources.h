#pragma once

#include "InputLayouts.h"
#include "RenderStates.h"
#include "Effects.h"

struct StaticResources
{
	static void init_all(ID3D11Device* device)
	{
		RenderStates::init(device);

		Effects::init(device);
		InputLayouts::init(device);
	}

	static void destroy_all()
	{
		InputLayouts::destroy();
		Effects::destroy();

		RenderStates::destroy();
	}
};