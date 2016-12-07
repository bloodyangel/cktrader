#include "LTSGateway.h"

namespace cktrader {

	IGateway* g_ltsGateway = nullptr;

	CK_EXPORTS  IGateway* CreateGateway(EventEngine* pEvent, const char* x)
	{
		if (!g_ltsGateway)
		{
			g_ltsGateway = new LTSGateway(pEvent,x);
		}

		return g_ltsGateway;
	}

	CK_EXPORTS int ReleaseGateway(IGateway*p)
	{
		if (g_ltsGateway)
		{
			delete g_ltsGateway;
			g_ltsGateway = nullptr;
		}

		return CK_TRUE;
	}
}//cktrader