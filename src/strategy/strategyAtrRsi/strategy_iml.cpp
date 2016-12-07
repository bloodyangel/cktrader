#include "strategyartrsi.h"

IStrategy* g_strategy_art_rsi = nullptr;

CK_EXPORTS IStrategy* CreateStrategy(IServiceMgr*x, const char*name)
{
	if (!g_strategy_art_rsi)
	{
		g_strategy_art_rsi = new StrategyAtrRsi(x, name);
	}

	return g_strategy_art_rsi;
}

CK_EXPORTS int ReleaseStrategy(IStrategy*p)
{
	if (g_strategy_art_rsi)
	{
		delete g_strategy_art_rsi;
		g_strategy_art_rsi = nullptr;
	}

	return CK_TRUE;
}
