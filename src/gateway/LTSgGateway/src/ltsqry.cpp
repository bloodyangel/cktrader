
#include "ltsqry.h"

#include <direct.h>
#include <io.h>
#include <sstream>

///-------------------------------------------------------------------------------------
///C++的回调函数将数据保存到队列中
///-------------------------------------------------------------------------------------

namespace cktrader {
	
LtsQry::LtsQry(EventEngine* pEvent, LTSGateway* gateway)
{
	this->m_event_service = pEvent;
	this->gateWay = gateway;
	this->gateWayName = gateway->getName();

	m_event_service->registerHandler(QRYONFRONTCONNECTED, std::bind(&LtsQry::processFrontConnected, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONFRONTDISCONNECTED, std::bind(&LtsQry::processFrontDisconnected, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONHEARTBEATWARNING, std::bind(&LtsQry::processHeartBeatWarning, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPERROR, std::bind(&LtsQry::processRspError, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPUSERLOGIN, std::bind(&LtsQry::processRspUserLogin, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPUSERLOGOUT, std::bind(&LtsQry::processRspUserLogout, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPFETCHAUTHRANDCODE, std::bind(&LtsQry::processRspFetchAuthRandCode, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYEXCHANGE, std::bind(&LtsQry::processRspQryExchange, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYINSTRUMENT, std::bind(&LtsQry::processRspQryInstrument, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYINVESTOR, std::bind(&LtsQry::processRspQryInvestor, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYTRADINGCODE, std::bind(&LtsQry::processRspQryTradingCode, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYTRADINGACCOUNT, std::bind(&LtsQry::processRspQryTradingAccount, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYBONDINTEREST, std::bind(&LtsQry::processRspQryBondInterest, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYMARKETRATIONINFO,  std::bind(&LtsQry::processRspQryMarketDataStaticInfo, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYINSTRUMENTCOMMISSIONRATE, std::bind(&LtsQry::processRspQryInstrumentCommissionRate, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYETFBASKET,  std::bind(&LtsQry::processRspQryETFBasket, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYOFINSTRUMENT,  std::bind(&LtsQry::processRspQryOFInstrument, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYSFINSTRUMENT,  std::bind(&LtsQry::processRspQrySFInstrument, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYINSTRUMENTUNITMARGIN, std::bind(&LtsQry::processRspQryInstrumentUnitMargin, this, std::placeholders::_1), gateWayName);

	m_event_service->registerHandler(QRYONRSPQRYPREDELIVINFO, std::bind(&LtsQry::processRspQryPreDelivInfo, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYCREDITSTOCKASSIGNINFO, std::bind(&LtsQry::processRspQryCreditStockAssignInfo, this, std::placeholders::_1), gateWayName);

	m_event_service->registerHandler(QRYONRSPQRYCREDITCASHASSIGNINFO,  std::bind(&LtsQry::processRspQryCreditCashAssignInfo, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYCONVERSIONRATE, std::bind(&LtsQry::processRspQryConversionRate, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYHISCREDITDEBTINFO, std::bind(&LtsQry::processRspQryHisCreditDebtInfo, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYMARKETDATASTATICINFO,  std::bind(&LtsQry::processRspQryMarketDataStaticInfo, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYEXPIREREPURCHINFO,  std::bind(&LtsQry::processRspQryExpireRepurchInfo, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYBONDPLEDGERATE,  std::bind(&LtsQry::processRspQryBondPledgeRate, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYPLEDGEBOND, std::bind(&LtsQry::processRspQryPledgeBond, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYORDER,  std::bind(&LtsQry::processRspQryOrder, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYTRADE,  std::bind(&LtsQry::processRspQryTrade, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYINVESTORPOSITION, std::bind(&LtsQry::processRspQryInvestorPosition, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYFUNDTRANSFERSERIAL, std::bind(&LtsQry::processRspQryFundTransferSerial, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(QRYONRSPQRYFUNDINTERTRANSFERSERIAL, std::bind(&LtsQry::processRspQryFundInterTransferSerial, this, std::placeholders::_1), gateWayName);
}

LtsQry::~LtsQry()
{

}

void LtsQry::OnFrontConnected()
{
	LtsData data = LtsData();

	Task task = Task();
	task.type = QRYONFRONTCONNECTED;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnFrontDisconnected(int nReason)
{
	LtsData data = LtsData();
	data.task_id = nReason;

	Task task = Task();
	task.type = QRYONFRONTDISCONNECTED;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnHeartBeatWarning(int nTimeLapse)
{
	LtsData data = LtsData();
	data.task_id = nTimeLapse;

	Task task = Task();
	task.type = QRYONHEARTBEATWARNING;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();	

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPERROR;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pRspUserLogin)
	{
		data.task_data = *pRspUserLogin;
	}
	else
	{
		CSecurityFtdcRspUserLoginField empty_data = CSecurityFtdcRspUserLoginField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPUSERLOGIN;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pUserLogout)
	{
		data.task_data = *pUserLogout;
	}
	else
	{
		CSecurityFtdcUserLogoutField empty_data = CSecurityFtdcUserLogoutField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPUSERLOGOUT;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspFetchAuthRandCode(CSecurityFtdcAuthRandCodeField *pAuthRandCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pAuthRandCode)
	{
		data.task_data = *pAuthRandCode;
	}
	else
	{
		CSecurityFtdcAuthRandCodeField empty_data = CSecurityFtdcAuthRandCodeField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPFETCHAUTHRANDCODE;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryExchange(CSecurityFtdcExchangeField *pExchange, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pExchange)
	{
		data.task_data = *pExchange;
	}
	else
	{
		CSecurityFtdcExchangeField empty_data = CSecurityFtdcExchangeField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYEXCHANGE;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryInstrument(CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pInstrument)
	{
		data.task_data = *pInstrument;
	}
	else
	{
		CSecurityFtdcInstrumentField empty_data = CSecurityFtdcInstrumentField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYINSTRUMENT;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryInvestor(CSecurityFtdcInvestorField *pInvestor, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pInvestor)
	{
		data.task_data = *pInvestor;
	}
	else
	{
		CSecurityFtdcInvestorField empty_data = CSecurityFtdcInvestorField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYINVESTOR;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryTradingCode(CSecurityFtdcTradingCodeField *pTradingCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pTradingCode)
	{
		data.task_data = *pTradingCode;
	}
	else
	{
		CSecurityFtdcTradingCodeField empty_data = CSecurityFtdcTradingCodeField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYTRADINGCODE;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryTradingAccount(CSecurityFtdcTradingAccountField *pTradingAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pTradingAccount)
	{
		data.task_data = *pTradingAccount;
	}
	else
	{
		CSecurityFtdcTradingAccountField empty_data = CSecurityFtdcTradingAccountField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYTRADINGACCOUNT;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryBondInterest(CSecurityFtdcBondInterestField *pBondInterest, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pBondInterest)
	{
		data.task_data = *pBondInterest;
	}
	else
	{
		CSecurityFtdcBondInterestField empty_data = CSecurityFtdcBondInterestField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYBONDINTEREST;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryMarketRationInfo(CSecurityFtdcMarketRationInfoField *pMarketRationInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pMarketRationInfo)
	{
		data.task_data = *pMarketRationInfo;
	}
	else
	{
		CSecurityFtdcMarketRationInfoField empty_data = CSecurityFtdcMarketRationInfoField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYMARKETRATIONINFO;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryInstrumentCommissionRate(CSecurityFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pInstrumentCommissionRate)
	{
		data.task_data = *pInstrumentCommissionRate;
	}
	else
	{
		CSecurityFtdcInstrumentCommissionRateField empty_data = CSecurityFtdcInstrumentCommissionRateField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYINSTRUMENTCOMMISSIONRATE;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryETFInstrument(CSecurityFtdcETFInstrumentField *pETFInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pETFInstrument)
	{
		data.task_data = *pETFInstrument;
	}
	else
	{
		CSecurityFtdcETFInstrumentField empty_data = CSecurityFtdcETFInstrumentField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYETFINSTRUMENT;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryETFBasket(CSecurityFtdcETFBasketField *pETFBasket, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pETFBasket)
	{
		data.task_data = *pETFBasket;
	}
	else
	{
		CSecurityFtdcETFBasketField empty_data = CSecurityFtdcETFBasketField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYETFBASKET;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryOFInstrument(CSecurityFtdcOFInstrumentField *pOFInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pOFInstrument)
	{
		data.task_data = *pOFInstrument;
	}
	else
	{
		CSecurityFtdcOFInstrumentField empty_data = CSecurityFtdcOFInstrumentField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYOFINSTRUMENT;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQrySFInstrument(CSecurityFtdcSFInstrumentField *pSFInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pSFInstrument)
	{
		data.task_data = *pSFInstrument;
	}
	else
	{
		CSecurityFtdcSFInstrumentField empty_data = CSecurityFtdcSFInstrumentField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYSFINSTRUMENT;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryInstrumentUnitMargin(CSecurityFtdcInstrumentUnitMarginField *pInstrumentUnitMargin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pInstrumentUnitMargin)
	{
		data.task_data = *pInstrumentUnitMargin;
	}
	else
	{
		CSecurityFtdcInstrumentUnitMarginField empty_data = CSecurityFtdcInstrumentUnitMarginField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYINSTRUMENTUNITMARGIN;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryPreDelivInfo(CSecurityFtdcPreDelivInfoField *pPreDelivInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pPreDelivInfo)
	{
		data.task_data = *pPreDelivInfo;
	}
	else
	{
		CSecurityFtdcPreDelivInfoField empty_data = CSecurityFtdcPreDelivInfoField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYPREDELIVINFO;
	task.task_data = data;
	m_event_service->put(task);
}

void LtsQry::OnRspQryCreditStockAssignInfo(CSecurityFtdcCreditStockAssignInfoField *pCreditStockAssignInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pCreditStockAssignInfo)
	{
		data.task_data = *pCreditStockAssignInfo;
	}
	else
	{
		CSecurityFtdcCreditStockAssignInfoField empty_data = CSecurityFtdcCreditStockAssignInfoField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYPREDELIVINFO;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryCreditCashAssignInfo(CSecurityFtdcCreditCashAssignInfoField *pCreditCashAssignInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pCreditCashAssignInfo)
	{
		data.task_data = *pCreditCashAssignInfo;
	}
	else
	{
		CSecurityFtdcCreditCashAssignInfoField empty_data = CSecurityFtdcCreditCashAssignInfoField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYCREDITCASHASSIGNINFO;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryConversionRate(CSecurityFtdcConversionRateField *pConversionRate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pConversionRate)
	{
		data.task_data = *pConversionRate;
	}
	else
	{
		CSecurityFtdcConversionRateField empty_data = CSecurityFtdcConversionRateField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYCONVERSIONRATE;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryHisCreditDebtInfo(CSecurityFtdcHisCreditDebtInfoField *pHisCreditDebtInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pHisCreditDebtInfo)
	{
		data.task_data = *pHisCreditDebtInfo;
	}
	else
	{
		CSecurityFtdcHisCreditDebtInfoField empty_data = CSecurityFtdcHisCreditDebtInfoField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYHISCREDITDEBTINFO;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryMarketDataStaticInfo(CSecurityFtdcMarketDataStaticInfoField *pMarketDataStaticInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pMarketDataStaticInfo)
	{
		data.task_data = *pMarketDataStaticInfo;
	}
	else
	{
		CSecurityFtdcMarketDataStaticInfoField empty_data = CSecurityFtdcMarketDataStaticInfoField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYMARKETDATASTATICINFO;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryExpireRepurchInfo(CSecurityFtdcExpireRepurchInfoField *pExpireRepurchInfo, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pExpireRepurchInfo)
	{
		data.task_data = *pExpireRepurchInfo;
	}
	else
	{
		CSecurityFtdcExpireRepurchInfoField empty_data = CSecurityFtdcExpireRepurchInfoField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYEXPIREREPURCHINFO;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryBondPledgeRate(CSecurityFtdcBondPledgeRateField *pBondPledgeRate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pBondPledgeRate)
	{
		data.task_data = *pBondPledgeRate;
	}
	else
	{
		CSecurityFtdcBondPledgeRateField empty_data = CSecurityFtdcBondPledgeRateField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYBONDPLEDGERATE;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryPledgeBond(CSecurityFtdcPledgeBondField *pPledgeBond, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pPledgeBond)
	{
		data.task_data = *pPledgeBond;
	}
	else
	{
		CSecurityFtdcPledgeBondField empty_data = CSecurityFtdcPledgeBondField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYPLEDGEBOND;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryOrder(CSecurityFtdcOrderField *pOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pOrder)
	{
		data.task_data = *pOrder;
	}
	else
	{
		CSecurityFtdcOrderField empty_data = CSecurityFtdcOrderField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYORDER;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryTrade(CSecurityFtdcTradeField *pTrade, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pTrade)
	{
		data.task_data = *pTrade;
	}
	else
	{
		CSecurityFtdcTradeField empty_data = CSecurityFtdcTradeField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYTRADE;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryInvestorPosition(CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pInvestorPosition)
	{
		data.task_data = *pInvestorPosition;
	}
	else
	{
		CSecurityFtdcInvestorPositionField empty_data = CSecurityFtdcInvestorPositionField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYINVESTORPOSITION;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryFundTransferSerial(CSecurityFtdcFundTransferField *pFundTransfer, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pFundTransfer)
	{
		data.task_data = *pFundTransfer;
	}
	else
	{
		CSecurityFtdcFundTransferField empty_data = CSecurityFtdcFundTransferField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYFUNDTRANSFERSERIAL;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsQry::OnRspQryFundInterTransferSerial(CSecurityFtdcFundInterTransferSerialField *pFundInterTransferSerial, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pFundInterTransferSerial)
	{
		data.task_data = *pFundInterTransferSerial;
	}
	else
	{
		CSecurityFtdcFundInterTransferSerialField empty_data = CSecurityFtdcFundInterTransferSerialField();
		memset(&empty_data, 0, sizeof(empty_data));
		data.task_data = empty_data;
	}

	if (pRspInfo)
	{
		data.task_error = *pRspInfo;
	}
	else
	{
		CSecurityFtdcRspInfoField empty_error = CSecurityFtdcRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		data.task_error = empty_error;
	}
	data.task_id = nRequestID;
	data.task_last = bIsLast;

	Task task = Task();
	task.type = QRYONRSPQRYFUNDINTERTRANSFERSERIAL;
	task.task_data = data;
	m_event_service->put(task);
};


///-------------------------------------------------------------------------------------
///工作线程从队列中取出数据
///-------------------------------------------------------------------------------------

void LtsQry::processFrontConnected(Datablk& data)
{
	connectionStatus = true;

	LogData log = LogData();
	log.gateWayName = gateWayName;
	log.logContent = "查询服务器连接成功";
	gateWay->onLog(log);

	reqID++;
	CSecurityFtdcAuthRandCodeField myreq = CSecurityFtdcAuthRandCodeField();
	memset(&myreq, 0, sizeof(myreq));
	this->api->ReqFetchAuthRandCode(&myreq, reqID);
};

void LtsQry::processFrontDisconnected(Datablk& data)
{
	connectionStatus = false;	

	LogData log = LogData();
	log.gateWayName = gateWayName;
	log.logContent = "查询服务器连接断开";
	gateWay->onLog(log);
};

void LtsQry::processHeartBeatWarning(Datablk& data)
{

};

void LtsQry::processRspError(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcRspInfoField task_error = lts_data.task_error.cast<CSecurityFtdcRspInfoField>();
	
	ErrorData err = ErrorData();
	err.gateWayName = gateWayName;
	err.errorID = task_error.ErrorID;
	err.errorMsg = task_error.ErrorMsg;
	gateWay->onError(err);
};

void LtsQry::processRspUserLogin(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcRspUserLoginField task_data = lts_data.task_data.cast<CSecurityFtdcRspUserLoginField>();
	CSecurityFtdcRspInfoField task_error = lts_data.task_error.cast<CSecurityFtdcRspInfoField>();
	if (task_error.ErrorID == 0)
	{
		frontID = task_data.FrontID;
		sessionID = task_data.SessionID;
		loginStatus = true;
		gateWay->qryConnected = true;

		LogData log = LogData();
		log.gateWayName = gateWayName;
		log.logContent = "查询服务器登录完成";
		gateWay->onLog(log);

		reqID++;
		CSecurityFtdcQryInstrumentField myreq = CSecurityFtdcQryInstrumentField();
		memset(&myreq, 0, sizeof(myreq));
		this->api->ReqQryInstrument(&myreq, reqID);
	}
	else
	{
		ErrorData err = ErrorData();
		err.gateWayName = gateWayName;
		err.errorID = task_error.ErrorID;
		err.errorMsg = task_error.ErrorMsg;
		gateWay->onError(err);
	}
};

void LtsQry::processRspUserLogout(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcRspUserLoginField task_data = lts_data.task_data.cast<CSecurityFtdcRspUserLoginField>();
	CSecurityFtdcRspInfoField task_error = lts_data.task_error.cast<CSecurityFtdcRspInfoField>();
	if (task_error.ErrorID == 0)
	{
		loginStatus = false;
		gateWay->qryConnected = false;

		LogData log = LogData();
		log.gateWayName = gateWayName;
		log.logContent = "查询服务器登出完成";
		gateWay->onLog(log);
	}
	else
	{
		ErrorData err = ErrorData();
		err.gateWayName = gateWayName;
		err.errorID = task_error.ErrorID;
		err.errorMsg = task_error.ErrorMsg;
		gateWay->onError(err);
	}
};

void LtsQry::processRspFetchAuthRandCode(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();
	CSecurityFtdcAuthRandCodeField task_data = lts_data.task_data.cast<CSecurityFtdcAuthRandCodeField>();
	randCode = task_data.RandCode;

	login();
};

void LtsQry::processRspQryExchange(Datablk& data)
{
	
};

void LtsQry::processRspQryInstrument(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();
	CSecurityFtdcInstrumentField task_data = lts_data.task_data.cast<CSecurityFtdcInstrumentField>();

	ContractData contract = ContractData();
	
	contract.gateWayName = gateWayName;
	contract.symbol = task_data.InstrumentID;
	contract.exchange = task_data.ExchangeID;
	contract.tSymbol = contract.symbol + contract.exchange;
	contract.name = task_data.InstrumentName;

	contract.size = task_data.VolumeMultiple;
	contract.priceTick = task_data.PriceTick;
	contract.strikePrice = task_data.ExecPrice;
	contract.underlyingSymbol = task_data.MarketID;

	//合约类型
	if (task_data.ProductClass == '1')
	{
		contract.productClass = PRODUCT_FUTURES;
	}
	else if (task_data.ProductClass == '2')
	{
		contract.productClass = PRODUCT_OPTION;
	}
	else if (task_data.ProductClass == '3')
	{
		contract.productClass = PRODUCT_COMBINATION;
	}
	else if (task_data.ProductClass == '6')
	{
		contract.productClass = PRODUCT_EQUITY;
	}
	else if (task_data.ProductClass == '8')
	{
		contract.productClass = PRODUCT_EQUITY;
	}
	else
	{
		contract.productClass = PRODUCT_UNKNOWN;
	}

	// 期权类型
	if (task_data.InstrumentType == '1')
	{
		contract.optionType = OPTION_CALL;
	}
	else if(task_data.InstrumentType == '2')
	{
		contract.optionType = OPTION_PUT;
	}

	gateWay->onContract(contract);

	if (lts_data.task_last)
	{
		LogData log = LogData();
		log.gateWayName = gateWayName;
		log.logContent = "交易合约信息获取完成";
		gateWay->onLog(log);
	}
};

void LtsQry::processRspQryInvestor(Datablk& data)
{
	
};

void LtsQry::processRspQryTradingCode(Datablk& data)
{
	
};

void LtsQry::processRspQryTradingAccount(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();
	CSecurityFtdcTradingAccountField task_data = lts_data.task_data.cast<CSecurityFtdcTradingAccountField>();

	AccountData account = AccountData();

	account.gateWayName = gateWayName;
	account.accountID = task_data.AccountID;
	account.tAccountID = account.gateWayName + account.accountID;

	account.preBalance = task_data.PreBalance;
	account.available = task_data.Available;
	account.commission = task_data.Commission;
	account.margin = task_data.CurrMargin;

	account.balance = task_data.Balance;

	gateWay->onAccount(account);
};

void LtsQry::processRspQryBondInterest(Datablk& data)
{
	
};

void LtsQry::processRspQryMarketRationInfo(Datablk& data)
{
	
};

void LtsQry::processRspQryInstrumentCommissionRate(Datablk& data)
{
	
};

void LtsQry::processRspQryETFInstrument(Datablk& data)
{

};

void LtsQry::processRspQryETFBasket(Datablk& data)
{
	
};

void LtsQry::processRspQryOFInstrument(Datablk& data)
{
	
};

void LtsQry::processRspQrySFInstrument(Datablk& data)
{

};

void LtsQry::processRspQryInstrumentUnitMargin(Datablk& data)
{

};

void LtsQry::processRspQryPreDelivInfo(Datablk& data)
{
	
};

void LtsQry::processRspQryCreditStockAssignInfo(Datablk& data)
{

};

void LtsQry::processRspQryCreditCashAssignInfo(Datablk& data)
{
	
};

void LtsQry::processRspQryConversionRate(Datablk& data)
{
	
};

void LtsQry::processRspQryHisCreditDebtInfo(Datablk& data)
{
	
};

void LtsQry::processRspQryMarketDataStaticInfo(Datablk& data)
{
	
};

void LtsQry::processRspQryExpireRepurchInfo(Datablk& data)
{
	
};

void LtsQry::processRspQryBondPledgeRate(Datablk& data)
{
	
};

void LtsQry::processRspQryPledgeBond(Datablk& data)
{
	
};

void LtsQry::processRspQryOrder(Datablk& data)
{
	
};

void LtsQry::processRspQryTrade(Datablk& data)
{
	
};

void LtsQry::processRspQryInvestorPosition(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();
	CSecurityFtdcInvestorPositionField task_data = lts_data.task_data.cast<CSecurityFtdcInvestorPositionField>();
	
	PositionData pos = PositionData();

	pos.gateWayName = gateWayName;

	pos.symbol = task_data.InstrumentID;
	pos.exchange = task_data.ExchangeID;
	pos.tSymbol = pos.symbol+"."+pos.exchange;

	if (task_data.PosiDirection == SECURITY_FTDC_PD_Net)
	{
		pos.direction == DIRECTION_NET;
		pos.frozen = task_data.LongFrozen;
	}
	else if (task_data.PosiDirection == SECURITY_FTDC_PD_Long)
	{
		pos.direction == DIRECTION_LONG;
		pos.frozen = task_data.LongFrozen;
	}
	else
	{
		pos.direction == DIRECTION_SHORT;
		pos.frozen = task_data.ShortFrozen;
	}

	pos.position = task_data.Position;
	pos.ydPosition = task_data.YdPosition;

	if (pos.position)
	{
		pos.price = task_data.OpenCost / pos.position;
	}

	pos.tPositionName = pos.tSymbol + "." + pos.direction;

	gateWay->onPosition(pos);
};

void LtsQry::processRspQryFundTransferSerial(Datablk& data)
{

};

void LtsQry::processRspQryFundInterTransferSerial(Datablk& data)
{
	
};


///-------------------------------------------------------------------------------------
///主动函数
///-------------------------------------------------------------------------------------
void LtsQry::connect(std::string userID,
					std::string password,
					std::string brokerID,
					std::string address,
					std::string productInfo,
					std::string authCode)
{
	this->userID = userID;
	this->password = password;
	this->brokerID = brokerID;
	this->address = address;
	this->productInfo = productInfo;
	this->authCode = authCode;

	if (!connectionStatus)
	{
		char dir_ptr[512];
		std::string pwd = getcwd(dir_ptr, 512);
		pwd = pwd + LTS_PATH_COM;

		if (access(pwd.c_str(), 0) == -1)
		{
			mkdir(pwd.c_str());
		}

		this->api = CSecurityFtdcQueryApi::CreateFtdcQueryApi(pwd.c_str());
		this->api->RegisterSpi(this);

		this->api->RegisterFront((char*)address.c_str());
		this->api->Init();
	}
	else
	{
		if (!loginStatus)
		{
			login();
		}
	}
}

void LtsQry::login()
{
	CSecurityFtdcReqUserLoginField myreq = CSecurityFtdcReqUserLoginField();
	memset(&myreq, 0, sizeof(myreq));
	strncpy(myreq.RandCode, randCode.c_str(), sizeof(myreq.RandCode) - 1);
	strncpy(myreq.UserProductInfo, productInfo.c_str(), sizeof(myreq.UserProductInfo) - 1);
	strncpy(myreq.UserID, userID.c_str(), sizeof(myreq.UserID) - 1);
	strncpy(myreq.AuthCode, authCode.c_str(), sizeof(myreq.AuthCode) - 1);
	strncpy(myreq.Password, password.c_str(), sizeof(myreq.Password) - 1);
	strncpy(myreq.BrokerID, brokerID.c_str(), sizeof(myreq.BrokerID) - 1);

	reqID++;
	this->api->ReqUserLogin(&myreq, reqID);
}

void LtsQry::qryAccount()
{
	CSecurityFtdcQryTradingAccountField myreq = CSecurityFtdcQryTradingAccountField();
	memset(&myreq, 0, sizeof(myreq));
	strncpy(myreq.BrokerID, brokerID.c_str(), sizeof(myreq.BrokerID) - 1);
	strncpy(myreq.InvestorID, userID.c_str(), sizeof(myreq.InvestorID) - 1);

	reqID++;
	this->api->ReqQryTradingAccount(&myreq, reqID);
}

void LtsQry::qryPosition()
{
	CSecurityFtdcQryInvestorPositionField myreq = CSecurityFtdcQryInvestorPositionField();
	memset(&myreq, 0, sizeof(myreq));
	strncpy(myreq.BrokerID, brokerID.c_str(), sizeof(myreq.BrokerID) - 1);
	strncpy(myreq.InvestorID, userID.c_str(), sizeof(myreq.InvestorID) - 1);

	reqID++;
	this->api->ReqQryInvestorPosition(&myreq, reqID);
}

void LtsQry::close()
{
	this->api->RegisterSpi(NULL);
	this->api->Release();
	this->api = NULL;
}

}//cktrader