#pragma once
#ifndef __LTSMD_H__
#define __LTSMD_H__

#include <string>
#include <atomic>
#include <vector>

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "SecurityFtdcMdApi.h"
#include "ltsdefs.h"
#include "LTSGateway.h"

namespace cktrader {

class LtsMd : public CSecurityFtdcMdSpi
{
private:
	CSecurityFtdcMdApi* api;			//API����

	EventEngine *m_event_service;
	LTSGateway *m_gateWay;

	std::string gateWayName;

	std::string userID;// �˺�
	std::string password;//����
	std::string brokerID;//        # �����̴���
	std::string address;//        # ��������ַ

	std::vector<SubscribeReq> *subscribedSymbols = nullptr;//    �Ѷ��ĺ�Լ����

	std::atomic<int> reqID = 0;//����ID

	std::atomic<bool> connectionStatus = false;      // ����״̬
	std::atomic<bool> loginStatus = false;//            ��¼״̬

public:
	LtsMd(EventEngine* pEvent,LTSGateway* gateway);
	~LtsMd();

	//-------------------------------------------------------------------------------------
	//API�ص�����
	//-------------------------------------------------------------------------------------

	//���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	//���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	//@param nReason ����ԭ��
	//        0x1001 �����ʧ��
	//        0x1002 ����дʧ��
	//        0x2001 ����������ʱ
	//        0x2002 ��������ʧ��
	//        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	//������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	//@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	//����Ӧ��
	virtual void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//��¼������Ӧ
	virtual void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//�ǳ�������Ӧ
	virtual void OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//��������Ӧ��
	virtual void OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//�������֪ͨ
	virtual void OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData);


	//-------------------------------------------------------------------------------------
	//task������
	//-------------------------------------------------------------------------------------

	void processFrontConnected(Datablk& data);

	void processFrontDisconnected(Datablk& data);

	void processHeartBeatWarning(Datablk& data);

	void processRspError(Datablk& data);

	void processRspUserLogin(Datablk& data);

	void processRspUserLogout(Datablk& data);

	void processRspSubMarketData(Datablk& data);

	void processRspUnSubMarketData(Datablk& data);

	void processRtnDepthMarketData(Datablk& data);

	//-------------------------------------------------------------------------------------
	//req:���������������ֵ�
	//-------------------------------------------------------------------------------------
	void connect(std::string userID, std::string password, std::string brokerID, std::string mdAddress);

	void subscribe(SubscribeReq& subscribeReq);

	int subscribeMarketData(std::string instrumentID,std::string exchageID);
	int unSubscribeMarketData(std::string instrumentID, std::string exchageID);
	int close();
};

}

#endif