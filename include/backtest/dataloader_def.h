#pragma once
#ifndef __DATALOADER_DEF_H__
#define __DATALOADER_DEF_H__

#include "utils/ckdef.h"
#include "utils/cktypes.h"
#include "gateway/tgateway_def.h"

namespace cktrader {

class DataLoader
{
	virtual void setBeiginDate(std::string date) = 0;
	virtual void setEndDate(std::string date) = 0;

	virtual void startLoadData()=0;
	virtual bool stopLoadData()=0;
};

}//cktrader
#endif
