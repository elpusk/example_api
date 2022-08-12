#pragma once

#include <thread>
#include <condition_variable>

#include <atltrace.h>

class _safe_cv
{
public:
	typedef	std::shared_ptr<_safe_cv>	type_ptr;

public:

	_safe_cv(void)
	{
		_reset();
	}

	virtual ~_safe_cv(void)
	{
		m_n_notify = 0;
		if (m_ptr_cv) {
			m_ptr_cv.reset();
		}
	}

	bool is_always_notified()
	{
		bool b_result(false);
		do {
			std::lock_guard<std::mutex> lock(m_mutex);
			if (m_n_notify != SIZE_MAX)
				continue;
			b_result = true;
		} while (false);
		return b_result;
	}

	void notify_one(bool fixed_status = false)
	{
		do {
			std::lock_guard<std::mutex> lock(m_mutex);

			if (fixed_status)
				m_n_notify = SIZE_MAX;
			else {
				if (m_n_notify == SIZE_MAX - 1)
					continue;//error over notify
				//
				++m_n_notify;
			}
			//
			m_ptr_cv->notify_one();
		} while (false);
	}

	void wait()
	{
		std::unique_lock<std::mutex>  lock(m_mutex);

		while (true) {
			if (m_n_notify > 0) {
				if (m_n_notify != SIZE_MAX) {
					--m_n_notify;
				}
				break;//exit while
			}
			m_ptr_cv->wait(lock);
		}//end while
	}
	std::cv_status wait_for(const unsigned long dw_mmsec_time_out)
	{
		std::unique_lock<std::mutex>  lock(m_mutex);

		std::chrono::milliseconds w = std::chrono::milliseconds(dw_mmsec_time_out);// *dw_mmsec_time_out;

		while (true) {
			if (m_n_notify > 0) {
				if (m_n_notify != SIZE_MAX) {
					--m_n_notify;
				}
				return std::cv_status::no_timeout;//exit while
			}
			
			if (m_ptr_cv->wait_for(lock, w) == std::cv_status::timeout)
				return std::cv_status::timeout;
		}//end while
	}
	std::cv_status wait_until(const unsigned long dw_mmsec_time_out)
	{
		std::unique_lock<std::mutex>  lock(m_mutex);
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();


		while (true) {
			if (m_n_notify > 0) {
				if (m_n_notify != SIZE_MAX) {
					--m_n_notify;
				}
				return std::cv_status::no_timeout;//exit while
			}

			std::chrono::system_clock::time_point time_out = now + std::chrono::milliseconds(dw_mmsec_time_out);
			if (m_ptr_cv->wait_until(lock, time_out) == std::cv_status::timeout)
				return std::cv_status::timeout;
		}//end while
	}

protected:
	void _reset()
	{
		m_n_notify = 0;
		if (m_ptr_cv) {
			m_ptr_cv.reset();
		}
		m_ptr_cv = std::shared_ptr<std::condition_variable>(new std::condition_variable());
	}

protected:
	size_t m_n_notify;
	std::mutex m_mutex;
	std::shared_ptr<std::condition_variable> m_ptr_cv;
};

