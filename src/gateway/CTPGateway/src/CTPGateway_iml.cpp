#include "CTPGateway.h"

namespace cktrader {

	IGateway* g_ctpGateway = nullptr;

	CK_EXPORTS IGateway* CreateGateway(EventEngine* pEvent,const char* x)
	{
		if (!g_ctpGateway)
		{
			g_ctpGateway = new CTPGateWay(pEvent,x);
		}

		return g_ctpGateway;
	}

	CK_EXPORTS int ReleaseGateway(IGateway*p)
	{
		if (g_ctpGateway)
		{
			delete g_ctpGateway;
			g_ctpGateway = nullptr;
		}

		return CK_TRUE;
	}
}//cktrader