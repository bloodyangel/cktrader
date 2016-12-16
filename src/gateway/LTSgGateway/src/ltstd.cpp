#include "ltstd.h"

#include <direct.h>
#include <io.h>
#include <sstream>

namespace cktrader {

LtsTd::LtsTd(EventEngine* pEvent, LTSGateway* gateway)
{
	this->m_event_service = pEvent;
	this->gateWay = gateway;
	this->gateWayName = gateway->getName();	

	m_event_service->registerHandler(TDONFRONTCONNECTED, std::bind(&LtsTd::processFrontConnected, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONFRONTDISCONNECTED, std::bind(&LtsTd::processFrontDisconnected, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONHEARTBEATWARNING, std::bind(&LtsTd::processHeartBeatWarning, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRSPERROR, std::bind(&LtsTd::processRspError, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRSPUSERLOGIN, std::bind(&LtsTd::processRspUserLogin, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRSPUSERLOGOUT,std::bind(&LtsTd::processRspUserLogout, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRSPFETCHAUTHRANDCODE, std::bind(&LtsTd::processRspFetchAuthRandCode, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRSPORDERINSERT, std::bind(&LtsTd::processRspOrderInsert, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRSPORDERACTION,std::bind(&LtsTd::processRspOrderAction, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRSPUSERPASSWORDUPDATE,  std::bind(&LtsTd::processRspUserPasswordUpdate, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRSPTRADINGACCOUNTPASSWORDUPDATE, std::bind(&LtsTd::processRspTradingAccountPasswordUpdate, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRTNORDER, std::bind(&LtsTd::processRtnOrder, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRTNTRADE, std::bind(&LtsTd::processRtnTrade, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONERRRTNORDERINSERT,std::bind(&LtsTd::processErrRtnOrderInsert, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONERRRTNORDERACTION,std::bind(&LtsTd::processErrRtnOrderAction, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRSPFUNDOUTBYLIBER,std::bind(&LtsTd::processRspFundOutByLiber, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRTNFUNDOUTBYLIBER,std::bind(&LtsTd::processErrRtnFundOutByLiber, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONERRRTNFUNDOUTBYLIBER,std::bind(&LtsTd::processErrRtnFundOutByLiber, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRTNFUNDINBYBANK, std::bind(&LtsTd::processRtnFundInByBank, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRSPFUNDINTERTRANSFER,  std::bind(&LtsTd::processRspFundInterTransfer, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONRTNFUNDINTERTRANSFERSERIAL,std::bind(&LtsTd::processRtnFundInterTransferSerial, this, std::placeholders::_1), gateWayName);
	m_event_service->registerHandler(TDONERRRTNFUNDINTERTRANSFER, std::bind(&LtsTd::processErrRtnFundInterTransfer, this, std::placeholders::_1), gateWayName);
}

LtsTd::~LtsTd()
{
	
}

void LtsTd::OnFrontConnected()
{
	LtsData data = LtsData();

	Task task = Task();
	task.type = TDONFRONTCONNECTED;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnFrontDisconnected(int nReason)
{
	LtsData data = LtsData();
	data.task_id = nReason;

	Task task = Task();
	task.type = TDONFRONTDISCONNECTED;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnHeartBeatWarning(int nTimeLapse)
{
	LtsData data = LtsData();
	data.task_id = nTimeLapse;

	Task task = Task();
	task.type = TDONHEARTBEATWARNING;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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
	task.type = TDONRSPERROR;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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
	task.type = TDONRSPUSERLOGIN;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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
	task.type = TDONRSPUSERLOGOUT;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRspFetchAuthRandCode(CSecurityFtdcAuthRandCodeField *pAuthRandCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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
	task.type = TDONRSPFETCHAUTHRANDCODE;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pInputOrder)
	{
		data.task_data = *pInputOrder;
	}
	else
	{
		CSecurityFtdcInputOrderField empty_data = CSecurityFtdcInputOrderField();
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
	task.type = TDONRSPORDERINSERT;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRspOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pInputOrderAction)
	{
		data.task_data = *pInputOrderAction;
	}
	else
	{
		CSecurityFtdcInputOrderActionField empty_data = CSecurityFtdcInputOrderActionField();
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
	task.type = TDONRSPORDERACTION;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRspUserPasswordUpdate(CSecurityFtdcUserPasswordUpdateField *pUserPasswordUpdate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pUserPasswordUpdate)
	{
		data.task_data = *pUserPasswordUpdate;
	}
	else
	{
		CSecurityFtdcUserPasswordUpdateField empty_data = CSecurityFtdcUserPasswordUpdateField();
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
	task.type = TDONRSPUSERPASSWORDUPDATE;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRspTradingAccountPasswordUpdate(CSecurityFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pTradingAccountPasswordUpdate)
	{
		data.task_data = *pTradingAccountPasswordUpdate;
	}
	else
	{
		CSecurityFtdcTradingAccountPasswordUpdateField empty_data = CSecurityFtdcTradingAccountPasswordUpdateField();
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
	task.type = TDONRSPTRADINGACCOUNTPASSWORDUPDATE;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRtnOrder(CSecurityFtdcOrderField *pOrder)
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

	Task task = Task();
	task.type = TDONRTNORDER;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRtnTrade(CSecurityFtdcTradeField *pTrade)
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

	Task task = Task();
	task.type = TDONRTNTRADE;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnErrRtnOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo)
{
	LtsData data = LtsData();

	if (pInputOrder)
	{
		data.task_data = *pInputOrder;
	}
	else
	{
		CSecurityFtdcInputOrderField empty_data = CSecurityFtdcInputOrderField();
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

	Task task = Task();
	task.type = TDONERRRTNORDERINSERT;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnErrRtnOrderAction(CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo)
{
	LtsData data = LtsData();

	if (pOrderAction)
	{
		data.task_data = *pOrderAction;
	}
	else
	{
		CSecurityFtdcOrderActionField empty_data = CSecurityFtdcOrderActionField();
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

	Task task = Task();
	task.type = TDONERRRTNORDERACTION;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRspFundOutByLiber(CSecurityFtdcInputFundTransferField *pInputFundTransfer, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pInputFundTransfer)
	{
		data.task_data = *pInputFundTransfer;
	}
	else
	{
		CSecurityFtdcInputFundTransferField empty_data = CSecurityFtdcInputFundTransferField();
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
	task.type = TDONRSPFUNDOUTBYLIBER;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRtnFundOutByLiber(CSecurityFtdcFundTransferField *pFundTransfer)
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

	Task task = Task();
	task.type = TDONRTNFUNDOUTBYLIBER;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnErrRtnFundOutByLiber(CSecurityFtdcInputFundTransferField *pInputFundTransfer, CSecurityFtdcRspInfoField *pRspInfo)
{
	LtsData data = LtsData();

	if (pInputFundTransfer)
	{
		data.task_data = *pInputFundTransfer;
	}
	else
	{
		CSecurityFtdcInputFundTransferField empty_data = CSecurityFtdcInputFundTransferField();
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

	Task task = Task();
	task.type = TDONERRRTNFUNDOUTBYLIBER;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRtnFundInByBank(CSecurityFtdcFundTransferField *pFundTransfer)
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

	Task task = Task();
	task.type = TDONRTNFUNDINBYBANK;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRspFundInterTransfer(CSecurityFtdcFundInterTransferField *pFundInterTransfer, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	if (pFundInterTransfer)
	{
		data.task_data = *pFundInterTransfer;
	}
	else
	{
		CSecurityFtdcFundInterTransferField empty_data = CSecurityFtdcFundInterTransferField();
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
	task.type = TDONRSPFUNDINTERTRANSFER;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnRtnFundInterTransferSerial(CSecurityFtdcFundInterTransferSerialField *pFundInterTransferSerial)
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
	
	Task task = Task();
	task.type = TDONRTNFUNDINTERTRANSFERSERIAL;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsTd::OnErrRtnFundInterTransfer(CSecurityFtdcFundInterTransferField *pFundInterTransfer, CSecurityFtdcRspInfoField *pRspInfo)
{
	LtsData data = LtsData();

	if (pFundInterTransfer)
	{
		data.task_data = *pFundInterTransfer;
	}
	else
	{
		CSecurityFtdcFundInterTransferField empty_data = CSecurityFtdcFundInterTransferField();
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

	Task task = Task();
	task.type = TDONERRRTNFUNDINTERTRANSFER;
	task.task_data = data;
	m_event_service->put(task);
};


///-------------------------------------------------------------------------------------
///工作线程从队列中取出数据
///-------------------------------------------------------------------------------------

void LtsTd::processFrontConnected(Datablk& data)
{
	connectionStatus = true;

	LogData log = LogData();
	log.gateWayName = gateWayName;
	log.logContent = "交易服务器连接成功";
	gateWay->onLog(log);

	reqID += 1;
	CSecurityFtdcAuthRandCodeField myreq = CSecurityFtdcAuthRandCodeField();
	memset(&myreq, 0, sizeof(myreq));
	this->api->ReqFetchAuthRandCode(&myreq, reqID);
};

void LtsTd::processFrontDisconnected(Datablk& data)
{
	connectionStatus = false;
	loginStatus = false;
	gateWay->tdConnected = false;

	LogData log = LogData();
	log.gateWayName = gateWayName;
	log.logContent = "交易服务器连接断开";
	gateWay->onLog(log);
};

void LtsTd::processHeartBeatWarning(Datablk& data)
{

};

void LtsTd::processRspError(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcRspInfoField task_error = lts_data.task_error.cast<CSecurityFtdcRspInfoField>();

	ErrorData err = ErrorData();
	err.gateWayName = gateWayName;
	err.errorID = task_error.ErrorID;
	err.errorMsg = task_error.ErrorMsg;
	gateWay->onError(err);
};

void LtsTd::processRspUserLogin(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcRspUserLoginField task_data = lts_data.task_data.cast<CSecurityFtdcRspUserLoginField>();

	CSecurityFtdcRspInfoField task_error = lts_data.task_error.cast<CSecurityFtdcRspInfoField>();
	if (task_error.ErrorID == 0)
	{
		frontID = task_data.FrontID;
		sessionID = task_data.SessionID;
		loginStatus = true;
		gateWay->tdConnected = true;

		LogData log = LogData();
		log.gateWayName = gateWayName;
		log.logContent = "交易服务器登录完成";
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

void LtsTd::processRspUserLogout(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcUserLogoutField task_data = lts_data.task_data.cast<CSecurityFtdcUserLogoutField>();

	CSecurityFtdcRspInfoField task_error = lts_data.task_error.cast<CSecurityFtdcRspInfoField>();
	if (task_error.ErrorID == 0)
	{
		loginStatus = false;
		gateWay->tdConnected = false;

		LogData log = LogData();
		log.gateWayName = gateWayName;
		log.logContent = "交易服务器登出完成";
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

void LtsTd::processRspFetchAuthRandCode(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcAuthRandCodeField task_data = lts_data.task_data.cast<CSecurityFtdcAuthRandCodeField>();
	randCode = task_data.RandCode;

	login();
};

void LtsTd::processRspOrderInsert(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcRspInfoField task_error = lts_data.task_error.cast<CSecurityFtdcRspInfoField>();
	
	ErrorData err = ErrorData();
	err.gateWayName = gateWayName;
	err.errorID = task_error.ErrorID;
	err.errorMsg = task_error.ErrorMsg;
	gateWay->onError(err);
};

void LtsTd::processRspOrderAction(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcRspInfoField task_error = lts_data.task_error.cast<CSecurityFtdcRspInfoField>();

	ErrorData err = ErrorData();
	err.gateWayName = gateWayName;
	err.errorID = task_error.ErrorID;
	err.errorMsg = task_error.ErrorMsg;
	gateWay->onError(err);
};

void LtsTd::processRspUserPasswordUpdate(Datablk& data)
{
	
};

void LtsTd::processRspTradingAccountPasswordUpdate(Datablk& data)
{
	
};

void LtsTd::processRtnOrder(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();
	CSecurityFtdcOrderField task_data = lts_data.task_data.cast<CSecurityFtdcOrderField>();
	int ref = atoi(task_data.OrderRef);
	orderRef = (int)orderRef > ref ? (int)orderRef : ref;

	OrderData order;

	order.gateWayName = gateWayName;
	order.symbol = task_data.InstrumentID;
	order.exchange = task_data.ExchangeID;
	order.tSymbol = order.symbol + task_data.ExchangeID;
	order.orderID = task_data.OrderRef;

	// 方向
	if (task_data.Direction == '0')
	{
		order.direction = DIRECTION_LONG;
	}
	else if (task_data.Direction == '1')
	{
		order.direction = DIRECTION_SHORT;
	}
	else
	{
		order.direction = DIRECTION_UNKNOWN;
	}
	
	//开平
	if (task_data.CombOffsetFlag == "0")
	{
		order.offset = OFFSET_OPEN;
	}
	else if (task_data.CombOffsetFlag == "1")
	{
		order.offset = OFFSET_CLOSE;
	}
	else
	{
		order.offset = OFFSET_UNKNOWN;
	}

	//状态
	if (task_data.OrderStatus == '0')
	{
		order.status = STATUS_ALLTRADED;
	}
	else if (task_data.OrderStatus == '1')
	{
		order.status = STATUS_PARTTRADED;
	}
	else if (task_data.OrderStatus == '3')
	{
		order.status = STATUS_NOTTRADED;
	}
	else if (task_data.OrderStatus == '5')
	{
		order.status = STATUS_CANCELLED;
	}
	else
	{
		order.status = STATUS_UNKNOWN;
	}

	// 价格、报单量等数值
	order.price = atof(task_data.LimitPrice);
	order.totalVolume = task_data.VolumeTotalOriginal;
	order.tradedVolume = task_data.VolumeTraded;
	order.orderTime = task_data.InsertTime;
	order.cancelTime = task_data.CancelTime;
	order.frontID = task_data.FrontID;
	order.sessionID = task_data.SessionID;
	
	order.tOrderID = gateWayName + order.orderID;

	gateWay->onOrder(order);
};

void LtsTd::processRtnTrade(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();
	CSecurityFtdcTradeField task_data = lts_data.task_data.cast<CSecurityFtdcTradeField>();

	TradeData trade;

	trade.gateWayName = gateWayName;
	trade.symbol = task_data.InstrumentID;
	trade.exchange = task_data.ExchangeID;
	trade.tSymbol = trade.symbol + task_data.ExchangeID;
	trade.orderID = task_data.OrderRef;

	// 方向
	if (task_data.Direction == SECURITY_FTDC_D_Buy)
	{
		trade.direction = DIRECTION_LONG;
	}
	else if (task_data.Direction == SECURITY_FTDC_D_Sell)
	{
		trade.direction = DIRECTION_SHORT;
	}

	//开平
	if (task_data.OffsetFlag == SECURITY_FTDC_OF_Open)
	{
		trade.offset = OFFSET_OPEN;
	}
	else if (task_data.OffsetFlag == SECURITY_FTDC_OF_Close)
	{
		trade.offset = OFFSET_CLOSE;
	}
	else if (task_data.OffsetFlag == SECURITY_FTDC_OF_CloseToday)
	{
		trade.offset = OFFSET_CLOSETODAY;
	}
	else if (task_data.OffsetFlag == SECURITY_FTDC_OF_CloseYesterday)
	{
		trade.offset = OFFSET_CLOSEYESTERDAY;
	}

	//价格、报单量等数值
	trade.price = atof(task_data.Price);
	trade.volume = task_data.Volume;
	trade.tradeTime = task_data.TradeTime;

	gateWay->onTrade(trade);
};

void LtsTd::processErrRtnOrderInsert(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcRspInfoField task_error = lts_data.task_error.cast<CSecurityFtdcRspInfoField>();

	ErrorData err = ErrorData();
	err.gateWayName = gateWayName;
	err.errorID = task_error.ErrorID;
	err.errorMsg = task_error.ErrorMsg;
	gateWay->onError(err);
};

void LtsTd::processErrRtnOrderAction(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcRspInfoField task_error = lts_data.task_error.cast<CSecurityFtdcRspInfoField>();

	ErrorData err = ErrorData();
	err.gateWayName = gateWayName;
	err.errorID = task_error.ErrorID;
	err.errorMsg = task_error.ErrorMsg;
	gateWay->onError(err);
};

void LtsTd::processRspFundOutByLiber(Datablk& data)
{
	
};

void LtsTd::processRtnFundOutByLiber(Datablk& data)
{
	
};

void LtsTd::processErrRtnFundOutByLiber(Datablk& data)
{

};

void LtsTd::processRtnFundInByBank(Datablk& data)
{

};

void LtsTd::processRspFundInterTransfer(Datablk& data)
{

};

void LtsTd::processRtnFundInterTransferSerial(Datablk& data)
{

};

void LtsTd::processErrRtnFundInterTransfer(Datablk& data)
{

};


///-------------------------------------------------------------------------------------
///主动函数
///-------------------------------------------------------------------------------------
void LtsTd::connect(std::string userID,
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

		this->api = CSecurityFtdcTraderApi::CreateFtdcTraderApi(pwd.c_str());
		this->api->RegisterSpi(this);

		this->api->SubscribePrivateTopic(SECURITY_TERT_RESTART);
		this->api->SubscribePublicTopic(SECURITY_TERT_RESTART);

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

void LtsTd::login()
{
	CSecurityFtdcReqUserLoginField myreq = CSecurityFtdcReqUserLoginField();
	memset(&myreq, 0, sizeof(myreq));
	strncpy(myreq.RandCode, randCode.c_str(), sizeof(myreq.RandCode)-1);
	strncpy(myreq.UserProductInfo, productInfo.c_str(), sizeof(myreq.UserProductInfo) - 1);
	strncpy(myreq.UserID, userID.c_str(), sizeof(myreq.UserID) - 1);
	strncpy(myreq.AuthCode, authCode.c_str(), sizeof(myreq.AuthCode) - 1);
	strncpy(myreq.Password, password.c_str(), sizeof(myreq.Password) - 1);
	strncpy(myreq.BrokerID, brokerID.c_str(), sizeof(myreq.BrokerID) - 1);

	reqID++;
	this->api->ReqUserLogin(&myreq, reqID);
}

std::string LtsTd::sendOrder(OrderReq& orderReq)
{
	reqID++;

	orderRef++;

	CSecurityFtdcInputOrderField myreq = CSecurityFtdcInputOrderField();
	memset(&myreq, 0, sizeof(myreq));

	strncpy(myreq.InstrumentID, orderReq.symbol.c_str(), sizeof(myreq.InstrumentID) - 1);

	std::stringstream price;
	price << orderReq.price;
	strncpy(myreq.LimitPrice,price.str().c_str(), sizeof(myreq.LimitPrice) - 1);

	myreq.VolumeTotalOriginal = orderReq.volume;

	strncpy(myreq.ExchangeID, orderReq.exchange.c_str(), sizeof(myreq.ExchangeID) - 1);

	//价格类型
	if (orderReq.priceType == PRICETYPE_LIMITPRICE)
	{
		myreq.OrderPriceType = SECURITY_FTDC_OPT_LimitPrice;
	}
	else if (orderReq.priceType == PRICETYPE_MARKETPRICE)
	{
		myreq.OrderPriceType = SECURITY_FTDC_OPT_AnyPrice;
	}
	else if (orderReq.priceType == PRICETYPE_FAK)
	{
		myreq.OrderPriceType = SECURITY_FTDC_OPT_BestPrice;
	}
	else
	{
		myreq.OrderPriceType = SECURITY_FTDC_OPT_AllLimitPrice;
	}

	//买卖方向
	if (orderReq.direction == DIRECTION_LONG)
	{
		myreq.Direction = SECURITY_FTDC_D_Buy;
	}
	if (orderReq.direction == DIRECTION_SHORT)
	{
		myreq.Direction = SECURITY_FTDC_D_Sell;
	}

	if (orderReq.offset == OFFSET_OPEN)
	{
		myreq.CombOffsetFlag[0] = SECURITY_FTDC_OF_Open;
	}
	if (orderReq.offset == OFFSET_CLOSE)
	{
		myreq.CombOffsetFlag[0] = SECURITY_FTDC_OF_Close;
	}
	if (orderReq.offset == OFFSET_CLOSETODAY)
	{
		myreq.CombOffsetFlag[0] = SECURITY_FTDC_OF_CloseToday;
	}
	if (orderReq.offset == OFFSET_CLOSEYESTERDAY)
	{
		myreq.CombOffsetFlag[0] = SECURITY_FTDC_OF_CloseYesterday;
	}

	strncpy(myreq.OrderRef, std::to_string(orderRef).c_str(), sizeof(myreq.OrderRef) - 1);
	strncpy(myreq.InvestorID, userID.c_str(), sizeof(myreq.InvestorID) - 1);
	strncpy(myreq.UserID, userID.c_str(), sizeof(myreq.UserID) - 1);
	strncpy(myreq.BrokerID, brokerID.c_str(), sizeof(myreq.BrokerID) - 1);

	myreq.CombHedgeFlag[0] = SECURITY_FTDC_HF_Speculation;
	myreq.ContingentCondition = SECURITY_FTDC_CC_Immediately;
	myreq.ForceCloseReason = SECURITY_FTDC_FCC_NotForceClose;
	myreq.IsAutoSuspend = 0;
	myreq.TimeCondition = SECURITY_FTDC_TC_GFD;
	myreq.VolumeCondition = SECURITY_FTDC_VC_AV;
	myreq.MinVolume = 1;
	myreq.UserForceClose = 0;

	this->api->ReqOrderInsert(&myreq, reqID);

	return gateWayName + std::to_string(reqID);
}

void LtsTd::cancelOrder(CancelOrderReq& req)
{
	reqID++;

	CSecurityFtdcInputOrderActionField myreq = CSecurityFtdcInputOrderActionField();
	memset(&myreq, 0, sizeof(myreq));

	strncpy(myreq.InstrumentID, req.symbol.c_str(), sizeof(myreq.InstrumentID) - 1);
	strncpy(myreq.ExchangeID, req.exchange.c_str(), sizeof(myreq.ExchangeID) - 1);
	strncpy(myreq.OrderRef, req.orderID.c_str(), sizeof(myreq.OrderRef));
	myreq.FrontID = frontID;
	myreq.SessionID = sessionID;

	myreq.ActionFlag = SECURITY_FTDC_AF_Delete;
	strncpy(myreq.BrokerID, brokerID.c_str(), sizeof(myreq.BrokerID) - 1);
	strncpy(myreq.UserID, userID.c_str(), sizeof(myreq.UserID) - 1);

	this->api->ReqOrderAction(&myreq, reqID);
}

void LtsTd::close()
{
	this->api->RegisterSpi(NULL);
	this->api->Release();
	this->api = NULL;
}

}//cktrader
