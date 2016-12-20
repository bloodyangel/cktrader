#include "IBGateway.h"

namespace cktrader {

	IGateway* g_ibGateway = nullptr;

	CK_EXPORTS IGateway* CreateGateway(EventEngine* pEvent, const char* x)
	{
		if (!g_ibGateway)
		{
			g_ibGateway = new IBGateway(pEvent, x);
		}

		return g_ibGateway;
	}

	CK_EXPORTS int ReleaseGateway(IGateway*p)
	{
		if (g_ibGateway)
		{
			delete g_ibGateway;
			g_ibGateway = nullptr;
		}

		return CK_TRUE;
	}
}//cktrader