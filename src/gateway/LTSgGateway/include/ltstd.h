#pragma once
#ifndef __LTSTD_H__
#define __LTSTD_H__

#include "ltsdefs.h"
#include "LTSGateway.h"

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "SecurityFtdcTraderApi.h"

namespace cktrader {

class LtsTd : public CSecurityFtdcTraderSpi
{
private:
	CSecurityFtdcTraderApi* api;			//API����
	
	EventEngine *m_event_service;
	LTSGateway *gateWay;
	std::string gateWayName;

	std::atomic<int> reqID = 0;//����������
	std::atomic<int> orderRef = 0;// �������

	std::atomic<bool> connectionStatus = false;//      ����״̬
	std::atomic<bool> loginStatus = false;// ��¼״̬

	std::string userID;// �˺�
	std::string password;//����
	std::string brokerID;//�����̴���
	std::string address;// ��������ַ
	std::string productInfo;
	std::string authCode;
	std::string randCode;

	int frontID;//ǰ�û����
	int sessionID;// �Ự���	

public:
	LtsTd(EventEngine* pEvent, LTSGateway* gateway);
	~LtsTd();

	//-------------------------------------------------------------------------------------
	//API�ص�����
	//-------------------------------------------------------------------------------------

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///����Ӧ��
	virtual void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��¼������Ӧ
	virtual void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��ȡ��֤�����������Ӧ
	virtual void OnRspFetchAuthRandCode(CSecurityFtdcAuthRandCodeField *pAuthRandCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����¼��������Ӧ
	virtual void OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������������Ӧ
	virtual void OnRspOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û��������������Ӧ
	virtual void OnRspUserPasswordUpdate(CSecurityFtdcUserPasswordUpdateField *pUserPasswordUpdate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ʽ��˻��������������Ӧ
	virtual void OnRspTradingAccountPasswordUpdate(CSecurityFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����֪ͨ
	virtual void OnRtnOrder(CSecurityFtdcOrderField *pOrder);

	///�ɽ�֪ͨ
	virtual void OnRtnTrade(CSecurityFtdcTradeField *pTrade);

	///����¼�����ر�
	virtual void OnErrRtnOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo);

	///������������ر�
	virtual void OnErrRtnOrderAction(CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo);

	///Liber�������Ӧ��
	virtual void OnRspFundOutByLiber(CSecurityFtdcInputFundTransferField *pInputFundTransfer, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Liber�������֪ͨ
	virtual void OnRtnFundOutByLiber(CSecurityFtdcFundTransferField *pFundTransfer);

	///Liber����������ر�
	virtual void OnErrRtnFundOutByLiber(CSecurityFtdcInputFundTransferField *pInputFundTransfer, CSecurityFtdcRspInfoField *pRspInfo);

	///���з������֪ͨ
	virtual void OnRtnFundInByBank(CSecurityFtdcFundTransferField *pFundTransfer);

	///�ʽ���תӦ��
	virtual void OnRspFundInterTransfer(CSecurityFtdcFundInterTransferField *pFundInterTransfer, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ʽ���ת��ˮ֪ͨ
	virtual void OnRtnFundInterTransferSerial(CSecurityFtdcFundInterTransferSerialField *pFundInterTransferSerial);

	///�ʽ���ת����ر�
	virtual void OnErrRtnFundInterTransfer(CSecurityFtdcFundInterTransferField *pFundInterTransfer, CSecurityFtdcRspInfoField *pRspInfo);

	//-------------------------------------------------------------------------------------
	//task������
	//-------------------------------------------------------------------------------------

	void processFrontConnected(Datablk& data);

	void processFrontDisconnected(Datablk& data);

	void processHeartBeatWarning(Datablk& data);

	void processRspError(Datablk& data);

	void processRspUserLogin(Datablk& data);

	void processRspUserLogout(Datablk& data);

	void processRspFetchAuthRandCode(Datablk& data);

	void processRspOrderInsert(Datablk& data);

	void processRspOrderAction(Datablk& data);

	void processRspUserPasswordUpdate(Datablk& data);

	void processRspTradingAccountPasswordUpdate(Datablk& data);

	void processRtnOrder(Datablk& data);

	void processRtnTrade(Datablk& data);

	void processErrRtnOrderInsert(Datablk& data);

	void processErrRtnOrderAction(Datablk& data);

	void processRspFundOutByLiber(Datablk& data);

	void processRtnFundOutByLiber(Datablk& data);

	void processErrRtnFundOutByLiber(Datablk& data);

	void processRtnFundInByBank(Datablk& data);

	void processRspFundInterTransfer(Datablk& data);

	void processRtnFundInterTransferSerial(Datablk& data);

	void processErrRtnFundInterTransfer(Datablk& data);

	//-------------------------------------------------------------------------------------
	//req:���������������ֵ�
	//-------------------------------------------------------------------------------------
	void connect(std::string userID,
					std::string password, 
					std::string brokerID, 
					std::string address,
					std::string productInfo,
					std::string authCode);

	void login();

	std::string sendOrder(OrderReq& orderReq);
	void cancelOrder(CancelOrderReq& req);

	void close();
};

}//cktrader

#endif