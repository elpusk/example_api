#pragma once

#include <queue>
#include <mutex>
#include <map>
#include <thread>
#include <map>

#include <_safe_cv.h>
#include <cworker.h>

#include "example_dll_const.h"
#include "cjob_of_dll.h"

#include "cprocess_of_job.h"
#include "cjob_mgmt.h"

namespace _example
{
	class cjob_mgmt_of_dll
	{
	private:
		typedef	std::map< cdll_const::type_result_index, cjob_of_dll::type_ptr_cjob_of_dll >	_type_map_index_result;

		typedef	_example::cjob_mgmt::type_ptr	_type_ptr_mgmt;
		typedef cdk_map< _type_ptr_mgmt > _type_dk_map_ptr_mgmt;
	public:
		virtual ~cjob_mgmt_of_dll()
		{
			m_ptr_mgmt.reset();
		}

		static cjob_mgmt_of_dll& get_instance()
		{
			static _example::cjob_mgmt_of_dll obj;
			return obj;
		}

		cdll_const::type_result_index push_job(
			const _example::cjob_of_dll::type_requst req_type,
			const long n_woker_number,
			const unsigned char* ps_data,
			const unsigned long n_data,
			const _example::cdll_const::type_dll_callback cb,
			void* p_user_of_cb,
			long b_cancel_waiting_job
		)
		{
			cdll_const::type_result_index n_result_index(_example::cdll_const::result_index_error);
			_example::cjob_mgmt::type_ptr ptr_mgmt;

			switch (req_type)
			{
			case _example::cjob_of_dll::req_type_create_worker:
			case _example::cjob_of_dll::req_type_remove_worker:
				m_ptr_mgmt->push_job(req_type, n_woker_number,ps_data, n_data, cb, p_user_of_cb, b_cancel_waiting_job);
				break;
			case _example::cjob_of_dll::req_type_cancel_job:
			case _example::cjob_of_dll::req_type_job:
				ptr_mgmt = m_dk_map_ptr_mgmt.get(n_woker_number);
				if (!ptr_mgmt)
					break;
				n_result_index = ptr_mgmt->push_job(req_type, ps_data, n_data, cb, p_user_of_cb, b_cancel_waiting_job);
				break;
			default:
				break;
			}//end switch
			return n_result_index;
		}

		cdll_const::type_result_index push_job(const long n_woker_number)
		{
			cdll_const::type_result_index n_result_index(_example::cdll_const::result_index_error);
			_example::cjob_mgmt::type_ptr ptr_mgmt = m_dk_map_ptr_mgmt.get(n_woker_number);
			if (ptr_mgmt) {
				n_result_index = ptr_mgmt->push_job();
			}
			return n_result_index;
		}

		_example::cdll_const::type_result get_result(const _example::cdll_const::type_result_index result_index, std::vector<unsigned char>& v_rx, bool b_remove_result_after_get = true)
		{
			return _example::cjob_mgmt::get_result(result_index, v_rx, b_remove_result_after_get);
		}
	private:
		cjob_mgmt_of_dll()
		{
			m_ptr_mgmt = _example::cjob_mgmt::type_ptr(new _example::cjob_mgmt(cjob_mgmt_of_dll::_worker_main, this));
		}

		bool _pop_job(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_job_out)
		{
			return m_ptr_mgmt->pop_job(ptr_job_out);
		}

		static void _worker_main(cjob_mgmt_of_dll* p_mgmt)
		{
			bool b_result(false), b_continue(false);
			bool b_run(true), b_notify(false);
			_example::cjob_of_dll::type_ptr_cjob_of_dll ptr_job_new;
			std::cv_status cs(std::cv_status::timeout);
			long n_worker_number(-1);
			_example::cjob_mgmt::type_ptr ptr_mgmt;

			do {

				do {
					cs = p_mgmt->m_ptr_mgmt->wait_for(1);
					if (cs == std::cv_status::timeout)
						continue;
					//new requst case
					if (p_mgmt->m_ptr_mgmt->is_always_notified()) {
						//terminates thread.
						b_run = false;
						continue;
					}
					if (!p_mgmt->m_ptr_mgmt->pop_job(ptr_job_new))
						continue;

					switch (ptr_job_new->get_type()) {
					case _example::cjob_of_dll::req_type_create_worker:
						p_mgmt->_create_worker(ptr_job_new);
						p_mgmt->m_ptr_mgmt->emplace_result_notify_remove_ptr_job(ptr_job_new);
						continue;;
					case _example::cjob_of_dll::req_type_remove_worker:
						p_mgmt->_remove_worker(ptr_job_new);
						p_mgmt->m_ptr_mgmt->emplace_result_notify_remove_ptr_job(ptr_job_new);
						continue;;
					default:
						ptr_mgmt.reset();
						continue;
					}//end switch
				} while (false);

			} while (b_run);

		}

		void _create_worker(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob)
		{
			do {
				if (!ptr_cjob)
					continue;
				std::wstring s_name = ctools::get_string(ptr_cjob->get_tx());
				if (s_name.empty()) {
					ptr_cjob->set_result(_example::cdll_const::result_error);
					continue;
				}

				_example::cjob_mgmt::type_ptr ptr_mgmt;
				ptr_mgmt = m_dk_map_ptr_mgmt.get(s_name);
				if (ptr_mgmt) {
					ptr_cjob->set_result(_example::cdll_const::result_error);
					continue;
				}

				ptr_mgmt = _example::cjob_mgmt::type_ptr(new _example::cjob_mgmt());
				long n_long_key = m_dk_map_ptr_mgmt.insert(
					s_name, 
					ptr_mgmt
				);

				if (n_long_key<0) {
					ptr_cjob->set_result(_example::cdll_const::result_error);
					continue;
				}

				ptr_mgmt->set_worker_number(n_long_key);

				ptr_cjob->set_result(_example::cdll_const::result_success, n_long_key);
			} while (false);
		}
		void _remove_worker(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_cjob)
		{
			do {
				if (!ptr_cjob)
					continue;

				long n_worker_number = ptr_cjob->get_worker_number();

				_example::cjob_mgmt::type_ptr ptr_mgmt;
				ptr_mgmt = m_dk_map_ptr_mgmt.get(n_worker_number);
				if (!ptr_mgmt) {
					ptr_cjob->set_result(_example::cdll_const::result_error);
					continue;
				}
				m_dk_map_ptr_mgmt.erase(n_worker_number);
				ptr_mgmt.reset();
			} while (false);
		}

	private:
		cjob_mgmt_of_dll::_type_dk_map_ptr_mgmt m_dk_map_ptr_mgmt;
		_example::cjob_mgmt::type_ptr m_ptr_mgmt;

	private://don't call these methods.
		cjob_mgmt_of_dll(const cjob_mgmt_of_dll&);
		cjob_mgmt_of_dll& operator=(const cjob_mgmt_of_dll&);
	};
}

