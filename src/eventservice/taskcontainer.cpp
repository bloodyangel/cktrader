#include "eventservice/taskcontainer.h"

#include <utility>

namespace cktrader {

TaskContainer::TaskContainer()
{
	task_map = new CKMap;

	task_type_queue_high = new BlockingConcurrentQueue<std::string>;
	task_type_queue_normal= new BlockingConcurrentQueue<std::string>;
}

TaskContainer::~TaskContainer()
{
	//释放map
	for (CKMap::const_iterator it = task_map->begin(); it != task_map->end(); ++it)
	{
		delete it->second;
	}

	delete task_map;

	delete task_type_queue_high;
	delete task_type_queue_normal;
}

Task TaskContainer::wait_and_get_task()
{
	bool ret = false;
	Task t;

	while (!ret)
	{
		std::string type = read_task_type_queue();
		if (type != "")
		{
			MapItem* item = read_task_map(type);
			if (item)
			{
				ret = item->read(t);
			}
		}
	}

	return t;
}

void TaskContainer::put_task(Task& data)
{	
	write_task_map(data);

	write_task_type_queue(data);
}

void TaskContainer::task_run_end(std::string task_type)
{
	(*task_map)[task_type]->free_run_lock();
}

std::string TaskContainer::read_task_type_queue()
{
	//等待队列有数据??
	std::string type;

	if (task_type_queue_high->size_approx() != 0)
	{
		task_type_queue_high->wait_dequeue(type);

		return type;
	}
	else
	{
		task_type_queue_normal->wait_dequeue(type);

		return type;
	}
}
void TaskContainer::write_task_type_queue(Task& data)
{
	//把事件类型放到队列里面，等着线程取出处理
	if (data.task_priority == Task::high)
	{
		task_type_queue_high->enqueue(data.type);
	}
	else
	{
		task_type_queue_normal->enqueue(data.type);
	}
}

void TaskContainer::write_task_map(Task& data)
{
	CKMap::const_iterator it;

	it = task_map->find(data.type);

	if (it == task_map->end())
	{
		MapItem* item = new MapItem();
		item->write(data);
		task_map->insert(std::make_pair(data.type, item));
	}
	else
	{
		MapItem* item = (*task_map)[data.type];	
		item->write(data);
	}
}
MapItem* TaskContainer::read_task_map(std::string type)
{
	CKMap::const_iterator it;
	MapItem* item = nullptr;
	it = task_map->find(type);

	if (it != task_map->end())
	{
		item = it->second;
	}
	//释放权限
	return item;
}

}//cktrader