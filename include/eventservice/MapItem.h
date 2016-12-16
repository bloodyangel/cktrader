#pragma once
#ifndef __MAPITEM_H__
#define __MAPITEM_H__

#include "utils/ckdef.h"
#include "utils/cktypes.h"

#include "concurrentqueue.h"

#include <map>
#include <atomic>

using namespace moodycamel;

namespace cktrader {

class MapItem
{
private:
	ConcurrentQueue<Task>* the_queue;

	//��һ���¼�ֻ����һ���̴߳�����
	//�����ִ�����дqueue�ֿ������֤Ч��
	std::atomic_flag unique_thread_flag = ATOMIC_FLAG_INIT;

public:
	
	MapItem();
	~MapItem();

	void get_run_lock();
	void free_run_lock();

	void write(Task& t);
	bool read(Task& t);
};

}

#endif