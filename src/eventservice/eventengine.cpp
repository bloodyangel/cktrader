#include "eventservice/eventengine.h"

#include <memory>
#include <chrono>
#include <algorithm>

namespace cktrader {

EventEngine::EventEngine():the_mutex_handlers(), m_active(false)
{
	m_task_thread_pool = nullptr;

	task_pool = new TaskContainer;//事件任务队列
	
	handlers = new std::multimap<std::string, Handle>;

	m_timer_thread = nullptr;;//for timer
}

EventEngine::EventEngine(EventEngine& engine):the_mutex_handlers(), m_active(false)
{
	m_task_thread_pool = engine.m_task_thread_pool;

	task_pool = engine.task_pool;//事件任务队列

	handlers = engine.handlers;

	m_timer_thread = engine.m_timer_thread;//for timer
}

EventEngine::~EventEngine()
{
	m_active = false;

	//删除任务处理线程池
	if (m_task_thread_pool)
	{		
		for (std::vector<std::thread*>::iterator it = m_task_thread_pool->begin(); it != m_task_thread_pool->end(); it++)
		{
			(*it)->join();
			delete (*it);
			m_task_thread_pool->erase(it);
		}

		delete m_task_thread_pool;
		m_task_thread_pool = nullptr;
	}

	if (m_timer_thread)
	{
		m_timer_thread->join();
		delete m_timer_thread;
		m_timer_thread = nullptr;
	}	

	if (task_pool)
	{
		delete task_pool;
		task_pool = nullptr;
	}
	
	if (handlers)
	{
		delete handlers;
		handlers = nullptr;
	}	
}

//注册事件处理函数，type为事件类型，accepter是注册者标识，f是注册的处理函数
bool EventEngine::registerHandler(std::string type, Handler f, std::string accepter)
{
	the_mutex_handlers.lock();
	if (!handlers)
	{
		return false;
	}
	Handle h;
	h.fh = f;
	h.handle_register = accepter;

	handlers->insert(std::make_pair(type,h));
	the_mutex_handlers.unlock();
	return true;
}

//删除accepter注册的type类型处理函数
bool EventEngine::unRegisterHandler(std::string type, std::string accepter)
{
	the_mutex_handlers.lock();
	if (!handlers)
	{
		return false;
	}

	std::pair <std::multimap<std::string, Handle>::iterator, std::multimap<std::string, Handle>::iterator> ret;

	ret = handlers->equal_range(type);
	for (std::multimap<std::string, Handle>::iterator it = ret.first; it != ret.second; ++it)
	{
		if (it->second.handle_register == accepter)
		{
			handlers->erase(it);
		}
	}
	
	the_mutex_handlers.unlock();
	return true;
}

//推送task
bool EventEngine::put(Task &t)
{
	if (!task_pool)
	{
		return false;
	}

	task_pool->put_task(t);
	return true;
}

//启动引擎，简单的线程池，复用线程，并启动一个timer线程，用于产生时间信号
bool EventEngine::startEngine()
{
	if (m_active || m_task_thread_pool || m_timer_thread)
	{
		return false;
	}

	m_active = true;

	m_task_thread_pool = new std::vector<std::thread*>;

	std::function<void()> f = std::bind(&EventEngine::processTask, this);

	for (unsigned i = 0; i < 2*std::thread::hardware_concurrency(); i++)
	{
		std::thread* thread_worker = new std::thread(f);
		m_task_thread_pool->push_back(thread_worker);
	}

	std::function<void()> f_timer = std::bind(&EventEngine::trigerTimer, this);
	m_timer_thread = new std::thread(f_timer);

	return true;
}

//停止事件引擎
bool EventEngine::stopEngine()
{
	if (!m_active || !m_task_thread_pool || !m_timer_thread)
	{
		return false;
	}

	m_active = false;

	if (m_timer_thread)
	{
		m_timer_thread->join();
		delete m_timer_thread;
		m_timer_thread = nullptr;
	}

	if (m_task_thread_pool)
	{
		for (std::vector<std::thread*>::iterator it = m_task_thread_pool->begin(); it != m_task_thread_pool->end(); it++)
		{
			(*it)->join();
			delete (*it);
			m_task_thread_pool->erase(it);
		}

		delete m_task_thread_pool;
		m_task_thread_pool = nullptr;
	}

	return true;
}

//事件引擎的处理函数，用于读取task，并调用事件处理函数
void EventEngine::processTask()
{
	while (m_active)
	{
		Task task;

		task = task_pool->wait_and_get_task();

		if (task.handle_flag)
		{
			FuncData func;

			try
			{
				func = task.task_data.cast<FuncData>();
				func.h(func.arg);
			}
			catch (...)
			{

			}

		}else
		{
			//找到事件对应的处理函数,多线程注册是否有问题？
			std::pair <std::multimap<std::string, Handle>::iterator, std::multimap<std::string, Handle>::iterator> ret;
			the_mutex_handlers.lock();
			ret = handlers->equal_range(task.type);
			the_mutex_handlers.unlock();

			//分发任务，插入优先级高的队列，事件类型的生成尽量散开，这样防止线程等待
			//事件类型一般为：事件类型+task类型+处理者
			for (std::multimap<std::string, Handle>::iterator it = ret.first; it != ret.second; ++it)
			{
				FuncData func;
				func.h = it->second.fh;
				func.arg = task.task_data;

				//生成函数处理事件，这样同一个事件类型有过个处理函数的时候，可以快速被调用
				Task handlerTask;
				handlerTask.task_priority = Task::high;
				handlerTask.type = task.type+it->second.handle_register;
				handlerTask.handle_flag = true;
				handlerTask.task_data = func;
				put(handlerTask);
			}//for			
		}//else

		//通知下一个线程可以处理事件了，这样保证一个事件类型在同一时间只能处理一个合约
		task_pool->task_run_end(task.type);
		
	}//while
}

//timer线程，产生时间信号
void EventEngine::trigerTimer()
{
	while (m_active)
	{
		Task task = Task();
		task.type = EVENT_TIMER;
		task.task_data = Datablk();
		put(task);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

}//cktrader