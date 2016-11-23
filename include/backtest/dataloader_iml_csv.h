#pragma once
#ifndef __DATALOADER_IML_CSV_H__
#define __DATALOADER_IML_CSV_H__

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "dataloader_def.h"
#include "gateway/tgateway_def.h"

#include <atomic>

namespace cktrader {

class CK_EXPORTS DataLoaderCsv:public DataLoader
{
public:
	DataLoaderCsv(IGateway* gate);
	~DataLoaderCsv();

	void setStartDate(std::string startDate = "20100416");
	void setEndDate(std::string endDate = "");
	void setPath(std::string path);

	void startLoadData();
	bool stopLoadData();

protected:
	void pullTick();

private:
	std::atomic<bool> isStarting = false;

	IGateway* backtestGate;

	std::string startDate;
	std::string endDate;
	std::string path;

	std::string dataInitDate;
	std::string strategyInitDate;

	std::thread* dataPull_thread = nullptr;
};

}//cktrader

#endif
