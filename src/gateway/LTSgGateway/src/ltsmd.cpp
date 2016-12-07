#include "ltsmd.h"

//todo linux
#include <direct.h>
#include <io.h>

namespace cktrader{

LtsMd::LtsMd(EventEngine* pEvent, LTSGateway* gateway)
{
	this->m_event_service = pEvent;
	this->m_gateWay = gateway;
	this->gateWayName = gateway->getName();

	subscribedSymbols = new std::vector<SubscribeReq>;

	m_event_service->registerHandler(MDONFRONTCONNECTED, std::bind(&LtsMd::processFrontConnected, this, std::placeholders::_1));
	m_event_service->registerHandler(MDONFRONTDISCONNECTED, std::bind(&LtsMd::processFrontDisconnected, this, std::placeholders::_1));
	m_event_service->registerHandler(MDONHEARTBEATWARNING, std::bind(&LtsMd::processHeartBeatWarning, this, std::placeholders::_1));
	m_event_service->registerHandler(MDONRSPERROR, std::bind(&LtsMd::processRspError, this, std::placeholders::_1));
	m_event_service->registerHandler(MDONRSPUSERLOGIN, std::bind(&LtsMd::processRspUserLogin, this, std::placeholders::_1));
	m_event_service->registerHandler(MDONRSPUSERLOGOUT, std::bind(&LtsMd::processRspUserLogout, this, std::placeholders::_1));
	m_event_service->registerHandler(MDONRSPSUBMARKETDATA, std::bind(&LtsMd::processRspSubMarketData, this, std::placeholders::_1));
	m_event_service->registerHandler(MDONRSPUNSUBMARKETDATA, std::bind(&LtsMd::processRspUnSubMarketData, this, std::placeholders::_1));
	m_event_service->registerHandler(MDONRTNDEPTHMARKETDATA, std::bind(&LtsMd::processRtnDepthMarketData, this, std::placeholders::_1));
}

LtsMd::~LtsMd()
{
	delete subscribedSymbols;
	subscribedSymbols = nullptr;
}

void LtsMd::OnFrontConnected()
{
	LtsData data = LtsData();

	Task task = Task();
	task.type = MDONFRONTCONNECTED;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsMd::OnFrontDisconnected(int nReason)
{
	LtsData data = LtsData();
	data.task_id = nReason;

	Task task = Task();
	task.type = MDONFRONTDISCONNECTED;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsMd::OnHeartBeatWarning(int nTimeLapse)
{
	LtsData data = LtsData();
	data.task_id = nTimeLapse;

	Task task = Task();
	task.type = MDONHEARTBEATWARNING;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsMd::OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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
	task.type = MDONRSPERROR;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsMd::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	data.task_data = *pRspUserLogin;
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
	task.type = MDONRSPUSERLOGIN;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsMd::OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();

	data.task_data = *pUserLogout;
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
	task.type = MDONRSPUSERLOGOUT;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsMd::OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();
	
	data.task_data = *pSpecificInstrument;
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
	task.type = MDONRSPSUBMARKETDATA;
	task.task_data = data;
	m_event_service->put(task);
};

void LtsMd::OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	LtsData data = LtsData();
	
	data.task_data = *pSpecificInstrument;
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
	task.type = MDONRSPUNSUBMARKETDATA;
	m_event_service->put(task);
};

void LtsMd::OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData)
{
	LtsData data = LtsData();
	data.task_data = *pDepthMarketData;

	Task task = Task();
	task.type = MDONRTNDEPTHMARKETDATA;
	task.task_data = data;
	m_event_service->put(task);
};


///-------------------------------------------------------------------------------------
///工作线程从队列中取出数据,进行推送
///-------------------------------------------------------------------------------------

void LtsMd::processFrontConnected(Datablk& data)
{
	connectionStatus = true;

	LogData log = LogData();
	log.gateWayName = gateWayName;
	log.logContent = "行情服务器连接成功";
	m_gateWay->onLog(log);

	reqID++;

	CSecurityFtdcReqUserLoginField myreq = CSecurityFtdcReqUserLoginField();
	memset(&myreq, 0, sizeof(myreq));

	if (userID.length() != 0 && password.length() != 0 && brokerID.length() != 0)
	{
		strncpy(myreq.BrokerID, brokerID.c_str(), sizeof(myreq.BrokerID) - 1);
		strncpy(myreq.UserID, userID.c_str(), sizeof(myreq.UserID) - 1);
		strncpy(myreq.Password, password.c_str(), sizeof(myreq.Password) - 1);
	}

	int i = this->api->ReqUserLogin(&myreq, reqID);
};

void LtsMd::processFrontDisconnected(Datablk& data)
{
	connectionStatus = false;
	loginStatus = false;
	m_gateWay->mdConnected = false;

	LogData log = LogData();
	log.gateWayName = gateWayName;
	log.logContent = "行情服务器连接断开";
	m_gateWay->onLog(log);

};

void LtsMd::processHeartBeatWarning(Datablk& data)
{
	
};

void LtsMd::processRspError(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcRspInfoField task_error = lts_data.task_error.cast<CSecurityFtdcRspInfoField>();

	ErrorData err = ErrorData();
	err.gateWayName = gateWayName;
	err.errorID = task_error.ErrorID;
	err.errorMsg = task_error.ErrorMsg;
	m_gateWay->onError(err);
};

void LtsMd::processRspUserLogin(Datablk& data)
{
	LtsData lts_data = data.cast<LtsData>();

	CSecurityFtdcRspInfoField task_error = lts_data.task_error.cast<CSecurityFtdcRspInfoField>();
	if (task_error.ErrorID == 0)
	{
		loginStatus = true;
		m_gateWay->mdConnected = true;
		LogData log = LogData();
		log.gateWayName = gateWayName;
		log.logContent = "行情服务器登录完成";
		m_gateWay->onLog(log);

		for (std::vector<SubscribeReq>::iterator it = subscribedSymbols->begin(); it != subscribedSymbols->end(); it++)
		{
			subscribeMarketData(it->symbol,it->exchange);
		}
	}
	else
	{
		ErrorData err = ErrorData();
		err.gateWayName = gateWayName;
		err.errorID = task_error.ErrorID;
		err.errorMsg = task_error.ErrorMsg;
		m_gateWay->onError(err);
	}
};

void LtsMd::processRspUserLogout(Datablk& data)
{
	LtsData data_lts = data.cast<LtsData>();

	CSecurityFtdcRspInfoField task_error = data_lts.task_error.cast<CSecurityFtdcRspInfoField>();
	if (task_error.ErrorID == 0)
	{
		loginStatus = false;
		m_gateWay->mdConnected = false;
		LogData log = LogData();
		log.gateWayName = gateWayName;
		log.logContent = "行情服务器登出完成";
		m_gateWay->onLog(log);
	}
	else
	{
		ErrorData err = ErrorData();
		err.gateWayName = gateWayName;
		err.errorID = task_error.ErrorID;
		err.errorMsg = task_error.ErrorMsg;
		m_gateWay->onError(err);
	}
};

void LtsMd::processRspSubMarketData(Datablk& data)
{
	
};

void LtsMd::processRspUnSubMarketData(Datablk& data)
{
	
};

void LtsMd::processRtnDepthMarketData(Datablk& data)
{
	LtsData data_lts = data.cast<LtsData>();

	CSecurityFtdcDepthMarketDataField task_data = data_lts.task_data.cast<CSecurityFtdcDepthMarketDataField>();
	
	TickData tick;

	tick.gateWayName = gateWayName;

	tick.symbol = task_data.InstrumentID;
	tick.exchange = task_data.ExchangeID;
	tick.tSymbol = tick.symbol;

	tick.lastPrice = task_data.LastPrice;
	tick.volume = task_data.Volume;
	tick.openInterest = task_data.OpenInterest;

	tick.bidPrice5 = task_data.BidPrice5;
	tick.bidPrice4 = task_data.BidPrice4;
	tick.bidPrice1 = task_data.BidPrice1;
	tick.bidPrice3 = task_data.BidPrice3;
	tick.bidPrice2 = task_data.BidPrice2;

	tick.lowerLimit = task_data.LowerLimitPrice;
	tick.upperLimit = task_data.UpperLimitPrice;

	tick.highPrice = task_data.HighestPrice;
	tick.lowPrice = task_data.LowestPrice;
	tick.openPrice = task_data.OpenPrice;
	tick.preClosePrice = task_data.PreClosePrice;

	tick.askPrice5 = task_data.AskPrice5;
	tick.askPrice4 = task_data.AskPrice4;
	tick.askPrice3 = task_data.AskPrice3;
	tick.askPrice2 = task_data.AskPrice2;
	tick.askPrice1 = task_data.AskPrice1;		

	tick.time = std::string(task_data.UpdateTime) + "." + std::to_string(task_data.UpdateMillisec / 100);
	tick.date = task_data.TradingDay;

	tick.askVolume1 = task_data.AskVolume1;
	tick.askVolume2 = task_data.AskVolume2;
	tick.askVolume3 = task_data.AskVolume3;
	tick.askVolume4 = task_data.AskVolume4;
	tick.askVolume5 = task_data.AskVolume5;

	tick.bidVolume1 = task_data.BidVolume1;
	tick.bidVolume2 = task_data.BidVolume2;
	tick.bidVolume3 = task_data.BidVolume3;
	tick.bidVolume4 = task_data.BidVolume4;
	tick.bidVolume5 = task_data.BidVolume5;

	m_gateWay->onTick(tick);
};



///-------------------------------------------------------------------------------------
///主动函数
///-------------------------------------------------------------------------------------

void LtsMd::connect(std::string userID, std::string password, std::string brokerID, std::string mdAddress)
{
	this->userID = userID;
	this->password = password;
	this->brokerID = brokerID;
	this->address = mdAddress;

	//todo linux
	if (!connectionStatus)
	{
		char dir_ptr[512];
		std::string pwd = getcwd(dir_ptr, 512);
		pwd = pwd + LTS_PATH_COM;

		if (access(pwd.c_str(), 0) == -1)
		{
			mkdir(pwd.c_str());
		}

		this->api = CSecurityFtdcMdApi::CreateFtdcMdApi(pwd.c_str());
		this->api->RegisterSpi(this);

		this->api->RegisterFront((char*)mdAddress.c_str());
		this->api->Init();
	}
	else
	{
		if (!loginStatus)
		{
			reqID++;

			CSecurityFtdcReqUserLoginField myreq = CSecurityFtdcReqUserLoginField();
			memset(&myreq, 0, sizeof(myreq));

			if (userID.length() != 0 && password.length() != 0 && brokerID.length() != 0)
			{
				strncpy(myreq.BrokerID, brokerID.c_str(), sizeof(myreq.BrokerID) - 1);
				strncpy(myreq.UserID, userID.c_str(), sizeof(myreq.UserID) - 1);
				strncpy(myreq.Password, password.c_str(), sizeof(myreq.Password) - 1);
			}

			int i = this->api->ReqUserLogin(&myreq, reqID);
		}
	}
}

void LtsMd::subscribe(SubscribeReq& subscribeReq)
{
	if (loginStatus)
	{
		std::string s = subscribeReq.symbol;
		size_t last = 0;
		size_t index = s.find_first_of(CK_CONTRACT_SYMBOL_DELM, last);
		while (index != std::string::npos)
		{
			std::string symbol = s.substr(last, index - last);
			subscribeMarketData(symbol, subscribeReq.exchange);

			subscribedSymbols->push_back(subscribeReq);

			last = index + 1;
			index = s.find_first_of(CK_CONTRACT_SYMBOL_DELM, last);
		}
		if (index - last > 0)
		{
			std::string symbol = s.substr(last, index - last);
			subscribeMarketData(symbol, subscribeReq.exchange);
			
			subscribedSymbols->push_back(subscribeReq);
		}
	}
}

int LtsMd::subscribeMarketData(std::string instrumentID, std::string exchageID)
{
	char* buffer = (char*)instrumentID.c_str();
	char* myreq[1] = { buffer };

	char* exchangeID = (char*)exchageID.c_str();

	int i = this->api->SubscribeMarketData(myreq, 1,exchangeID);
	return i;
}

int LtsMd::unSubscribeMarketData(std::string instrumentID, std::string exchageID)
{
	char* buffer = (char*)instrumentID.c_str();
	char* myreq[1] = { buffer };

	char* exchangeID = (char*)exchageID.c_str();

	int i = this->api->UnSubscribeMarketData(myreq, 1, exchangeID);

	return i;
}

int LtsMd::close()
{
	//该函数在原生API里没有，用于安全退出API用，原生的join似乎不太稳定
	this->api->RegisterSpi(NULL);
	this->api->Release();
	this->api = NULL;
	return 1;
}

}//cktrader