#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "CTPGateway.h"
#include "ctpmd.h"
#include "eventservice/eventengine.h"

//todo linux
#include <direct.h>
#include <io.h>

namespace cktrader {

	CtpMd::CtpMd(EventEngine *event_service, CTPGateWay *gateWay)
	{
		this->m_event_service = event_service;
		this->gateWay = gateWay;
		this->gateWayName = gateWay->getName();
		subscribedSymbols = new std::set<std::string>;

		//register event handler
		m_event_service->registerHandler(MDONFRONTCONNECTED, std::bind(&CtpMd::processFrontConnected, this, std::placeholders::_1));
		m_event_service->registerHandler(MDONFRONTDISCONNECTED, std::bind(&CtpMd::processFrontDisconnected, this, std::placeholders::_1));
		m_event_service->registerHandler(MDONRSPUSERLOGIN, std::bind(&CtpMd::processRspUserLogin, this, std::placeholders::_1));
		m_event_service->registerHandler(MDONRSPUSERLOGOUT, std::bind(&CtpMd::processRspUserLogout, this, std::placeholders::_1));
		m_event_service->registerHandler(MDONRSPERROR, std::bind(&CtpMd::processRspError, this, std::placeholders::_1));
		m_event_service->registerHandler(MDONRTNDEPTHMARKETDATA, std::bind(&CtpMd::processRtnDepthMarketData, this, std::placeholders::_1));
	}

	CtpMd::~CtpMd()
	{
		delete subscribedSymbols;
		subscribedSymbols = nullptr;
	}

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	void CtpMd::OnFrontConnected()
	{
		gateWay->writeLog("CtpMd::OnFrontConnected");
		CtpData data = CtpData();

		Task task = Task();
		task.type = MDONFRONTCONNECTED;
		task.task_data = data;
		m_event_service->put(task);
	}

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	void CtpMd::OnFrontDisconnected(int nReason)
	{
		gateWay->writeLog("CtpMd::OnFrontDisconnected");
		CtpData data = CtpData();
		data.task_id = nReason;

		Task task = Task();
		task.type = MDONFRONTDISCONNECTED;
		task.task_data = data;
		m_event_service->put(task);
	}

	///��¼������Ӧ
	void CtpMd::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		gateWay->writeLog("CtpMd::OnRspUserLogin");
		CtpData data = CtpData();

		if (pRspUserLogin)
		{
			data.task_data = *pRspUserLogin;
		}
		else
		{
			CThostFtdcRspUserLoginField empty_data = CThostFtdcRspUserLoginField();
			memset(&empty_data, 0, sizeof(CThostFtdcRspUserLoginField));
			data.task_data = empty_data;
		}

		if (pRspInfo)
		{
			data.task_error = *pRspInfo;
		}
		else
		{
			CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
			memset(&empty_error, 0, sizeof(empty_error));
			data.task_error = empty_error;
		}
		data.task_id = nRequestID;
		data.task_last = bIsLast;

		Task task = Task();
		task.type = MDONRSPUSERLOGIN;
		task.task_data = data;
		m_event_service->put(task);
	}

	///�ǳ�������Ӧ
	void CtpMd::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		gateWay->writeLog("CtpMd::OnRspUserLogout");
		CtpData data = CtpData();

		if (pUserLogout)
		{
			data.task_data = *pUserLogout;
		}
		else
		{
			CThostFtdcUserLogoutField empty_data = CThostFtdcUserLogoutField();
			memset(&empty_data, 0, sizeof(empty_data));
			data.task_data = empty_data;
		}

		if (pRspInfo)
		{
			data.task_error = *pRspInfo;
		}
		else
		{
			CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
			memset(&empty_error, 0, sizeof(empty_error));
			data.task_error = empty_error;
		}
		data.task_id = nRequestID;
		data.task_last = bIsLast;

		Task task = Task();
		task.type = MDONRSPUSERLOGOUT;
		task.task_data = data;
		m_event_service->put(task);
	}

	///����Ӧ��
	void CtpMd::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		gateWay->writeLog("CtpMd::OnRspError");
		CtpData data = CtpData();

		if (pRspInfo)
		{
			data.task_error = *pRspInfo;
		}
		else
		{
			CThostFtdcRspInfoField empty_error = CThostFtdcRspInfoField();
			memset(&empty_error, 0, sizeof(empty_error));
			data.task_error = empty_error;
		}
		data.task_id = nRequestID;
		data.task_last = bIsLast;

		Task task = Task();
		task.type = MDONRSPERROR;
		task.task_data = data;
		m_event_service->put(task);
	}

	///�������֪ͨ
	void CtpMd::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
	{
		gateWay->writeLog("CtpMd::OnRtnDepthMarketData");
		CtpData data = CtpData();

		if (pDepthMarketData)
		{
			data.task_data = *pDepthMarketData;
		}
		else
		{
			CThostFtdcDepthMarketDataField empty_data = CThostFtdcDepthMarketDataField();
			memset(&empty_data, 0, sizeof(empty_data));
			data.task_data = empty_data;
		}

		Task task = Task();
		task.type = MDONRTNDEPTHMARKETDATA;
		task.task_data = data;
		m_event_service->put(task);
	}

	///�¼�������

	void CtpMd::processFrontConnected(Datablk& data)
	{
		gateWay->writeLog("CtpMd::processFrontConnected");
		connectionStatus = true;

		LogData log = LogData();
		log.gateWayName = gateWayName;
		log.logContent = "������������ӳɹ�";
		gateWay->onLog(log);

		reqID++;
		CThostFtdcReqUserLoginField myreq = CThostFtdcReqUserLoginField();
		memset(&myreq, 0, sizeof(myreq));

		if (userID.length() != 0 && password.length() != 0 && brokerID.length() != 0)
		{
			strncpy(myreq.BrokerID, brokerID.c_str(), sizeof(myreq.BrokerID) - 1);
			strncpy(myreq.UserID, userID.c_str(), sizeof(myreq.UserID) - 1);
			strncpy(myreq.Password, password.c_str(), sizeof(myreq.Password) - 1);
		}

		api->ReqUserLogin(&myreq, reqID);
	}

	void CtpMd::processFrontDisconnected(Datablk& data)
	{
		gateWay->writeLog("CtpMd::processFrontDisconnected");
		connectionStatus = false;
		loginStatus = false;
		gateWay->mdConnected = false;

		LogData log = LogData();
		log.gateWayName = gateWayName;
		log.logContent = "������������ӶϿ�";
		gateWay->onLog(log);
	}

	void CtpMd::processRspUserLogin(Datablk& data)
	{
		gateWay->writeLog("CtpMd::processRspUserLogin");
		CtpData ctpdata = data.cast<CtpData>();

		CThostFtdcRspInfoField task_error = ctpdata.task_error.cast<CThostFtdcRspInfoField>();

		if (task_error.ErrorID == 0)
		{
			loginStatus = true;
			gateWay->mdConnected = true;
			LogData log = LogData();
			log.gateWayName = gateWayName;
			log.logContent = "�����������¼���";
			gateWay->onLog(log);

			for (std::set<std::string>::iterator it = subscribedSymbols->begin(); it != subscribedSymbols->end(); it++)
			{
				subscribeMarketData(*it);
			}
		}
		else
		{
			ErrorData err = ErrorData();
			err.gateWayName = gateWayName;
			err.errorID = task_error.ErrorID;
			err.errorMsg = task_error.ErrorMsg;
			gateWay->onError(err);
		}
	}

	void CtpMd::processRspUserLogout(Datablk& data)
	{
		gateWay->writeLog("CtpMd::processRspUserLogout");
		CtpData ctpdata = data.cast<CtpData>();

		CThostFtdcRspInfoField task_error = ctpdata.task_error.cast<CThostFtdcRspInfoField>();

		if (task_error.ErrorID == 0)
		{
			loginStatus = false;
			gateWay->mdConnected = false;
			LogData log = LogData();
			log.gateWayName = gateWayName;
			log.logContent = "����������ǳ����";
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
	}

	void CtpMd::processRspError(Datablk& data)
	{
		gateWay->writeLog("CtpMd::processRspError");
		CtpData ctpdata = data.cast<CtpData>();

		CThostFtdcRspInfoField task_error = ctpdata.task_error.cast<CThostFtdcRspInfoField>();

		ErrorData err = ErrorData();
		err.gateWayName = gateWayName;
		err.errorID = task_error.ErrorID;
		err.errorMsg = task_error.ErrorMsg;
		gateWay->onError(err);
	}

	void CtpMd::processRtnDepthMarketData(Datablk& data)
	{
		CtpData ctpdata = data.cast<CtpData>();

		CThostFtdcDepthMarketDataField task_data = ctpdata.task_data.cast<CThostFtdcDepthMarketDataField>();

		TickData tick;

		tick.gateWayName = gateWayName;

		tick.symbol = task_data.InstrumentID;
		tick.exchange = task_data.ExchangeID;
		tick.tSymbol = tick.symbol;
		tick.lastPrice = task_data.LastPrice;
		tick.volume = task_data.Volume;
		tick.openInterest = task_data.OpenInterest;
		tick.time = std::string(task_data.UpdateTime) + "." + std::to_string(task_data.UpdateMillisec / 100);
		tick.date = task_data.TradingDay;

		tick.openPrice = task_data.OpenPrice;
		tick.highPrice = task_data.HighestPrice;
		tick.lowPrice = task_data.LowestPrice;
		tick.preClosePrice = task_data.PreClosePrice;

		tick.upperLimit = task_data.UpperLimitPrice;
		tick.lowerLimit = task_data.LowerLimitPrice;

		//CTPֻ��һ������
		tick.bidPrice1 = task_data.BidPrice1;
		tick.bidVolume1 = task_data.BidVolume1;
		tick.askPrice1 = task_data.AskPrice1;
		tick.askVolume1 = task_data.AskVolume1;

		gateWay->onTick(tick);
	}

	///////////////////////////////////////////////////////
	/// �������ú���
	///
	void CtpMd::connect(std::string userID, std::string password, std::string brokerID, std::string mdAddress)
	{
		this->userID = userID;
		this->password = password;
		this->brokerID = brokerID;
		this->address = mdAddress;

		if (!connectionStatus)
		{
			char dir_ptr[512];
			std::string pwd = getcwd(dir_ptr, 512);
			pwd = pwd + CTP_PATH_COM;

			//todo linux
			if (access(pwd.c_str(), 0) == -1)
			{
				mkdir(pwd.c_str());
			}
			this->api = CThostFtdcMdApi::CreateFtdcMdApi(pwd.c_str());
			this->api->RegisterSpi(this);

			this->api->RegisterFront((char*)mdAddress.c_str());
			this->api->Init();
		}
		else
		{
			if (!loginStatus)
			{
				reqID++;
				CThostFtdcReqUserLoginField myreq = CThostFtdcReqUserLoginField();

				if (userID.length() != 0 && password.length() != 0 && brokerID.length() != 0)
				{
					strncpy(myreq.BrokerID, brokerID.c_str(), sizeof(myreq.BrokerID) - 1);
					strncpy(myreq.UserID, userID.c_str(), sizeof(myreq.UserID) - 1);
					strncpy(myreq.Password, password.c_str(), sizeof(myreq.Password) - 1);
				}

				api->ReqUserLogin(&myreq, reqID);
			}
		}
	}

	void CtpMd::subscribe(SubscribeReq& subscribeReq)
	{
		if (loginStatus)
		{
			std::string s = subscribeReq.symbol;
			size_t last = 0;
			size_t index = s.find_first_of(CK_CONTRACT_SYMBOL_DELM, last);
			while (index != std::string::npos)
			{
				std::string symbol = s.substr(last, index - last);
				subscribeMarketData(symbol);
				subscribedSymbols->insert(symbol);

				last = index + 1;
				index = s.find_first_of(CK_CONTRACT_SYMBOL_DELM, last);
			}
			if (index - last>0)
			{
				std::string symbol = s.substr(last, index - last);
				subscribeMarketData(symbol);
				subscribedSymbols->insert(symbol);
			}
		}
	}

	int CtpMd::close()
	{
		//�ú�����ԭ��API��û�У����ڰ�ȫ�˳�API�ã�ԭ����join�ƺ���̫�ȶ�
		this->api->RegisterSpi(NULL);
		this->api->Release();
		this->api = NULL;
		return 1;
	}

	std::string CtpMd::getTradingDay()
	{
		return api->GetTradingDay();
	}

	int CtpMd::subscribeMarketData(std::string instrumentID)
	{
		char* buffer = (char*)instrumentID.c_str();
		char* myreq[1] = { buffer };

		int i = this->api->SubscribeMarketData(myreq, 1);
		return i;
	}

	int CtpMd::unSubscribeMarketData(std::string instrumentID)
	{
		char* buffer = (char*)instrumentID.c_str();
		char* myreq[1] = { buffer };

		int i = this->api->UnSubscribeMarketData(myreq, 1);
		return i;
	}

	int CtpMd::subscribeForQuoteRsp(std::string instrumentID)
	{
		char* buffer = (char*)instrumentID.c_str();
		char* myreq[1] = { buffer };
		int i = this->api->SubscribeForQuoteRsp(myreq, 1);
		return i;
	}

	int CtpMd::unSubscribeForQuoteRsp(std::string instrumentID)
	{
		char* buffer = (char*)instrumentID.c_str();
		char* myreq[1] = { buffer };;
		int i = this->api->UnSubscribeForQuoteRsp(myreq, 1);
		return i;
	}

}//cktrader