#pragma once

#include <vector>
#include <algorithm>
#include <iterator>

#include <_safe_cv.h>
#include <ctools.h>
#include "example_dll.h"

namespace _example
{
	class cjob_of_dll
	{
	public:
		typedef	std::shared_ptr< cjob_of_dll >	type_ptr_cjob_of_dll;

		typedef	enum : long {
			req_type_unknown = -1,
			req_type_create_worker = 0,
			req_type_remove_worker = 1,
			req_type_cancel_job = 2,
			req_type_job = 3
		} type_requst;

	public:
		virtual ~cjob_of_dll()
		{
		}
		cjob_of_dll(
			const _example::cdll_const::type_result_index n_job_result_index,
			const _example::cjob_of_dll::type_requst req_type,
			const long n_worker_number,
			const unsigned char* ps_data,
			const unsigned long n_data,
			const _example::cdll_const::type_dll_callback cb,
			void* p_user_of_cb,
			long b_cancel_waiting_job
		)
		{
			_clear();
			_set(req_type, n_worker_number, ps_data, n_data, cb, p_user_of_cb, b_cancel_waiting_job);
			m_n_job_result_index = n_job_result_index;
		}

		cjob_of_dll(
			const _example::cdll_const::type_result_index n_job_result_index,
			const long n_worker_number
		)
		{
			_clear();
			m_n_worker_number = n_worker_number;
			m_type_requst = _example::cjob_of_dll::req_type_cancel_job;

			m_n_job_result_index = n_job_result_index;
		}

		_example::cdll_const::type_result_index get_result_index() const
		{
			return m_n_job_result_index;
		}

		std::pair<_example::cdll_const::type_result, ctools::type_buffer> get_result() const
		{
			return std::make_pair(m_result, m_v_rx);
		}

		_example::cdll_const::type_result notify()
		{
			_example::cdll_const::type_result result(_example::cdll_const::result_error);
			if (m_cb) {
				result = m_cb(m_n_worker_number, m_n_job_result_index, m_result, m_p_user_of_cb);
			}
			m_cv_for_sync_result.notify_one();
			return result;
		}

		void wait()
		{
			m_cv_for_sync_result.wait();
		}

		_example::cjob_of_dll::type_requst get_type() const
		{
			return m_type_requst;
		}

		void set_result(const _example::cdll_const::type_result result, const ctools::type_buffer& v_rx = std::vector<unsigned char>(0))
		{
			m_result = result;
			m_v_rx = v_rx;
		}
		void set_result(const _example::cdll_const::type_result result, long n_worker_number)
		{
			m_result = result;
			m_n_worker_number = n_worker_number;
		}

		bool is_cancel_type() const
		{
			if (m_type_requst == _example::cjob_of_dll::req_type_cancel_job)
				return true;
			else
				return false;
		}

		const ctools::type_buffer& get_tx() const
		{
			return m_v_tx;
		}

		long get_worker_number() const
		{
			return m_n_worker_number;
		}

		bool is_cancel_waiting_job() const
		{
			if (m_b_cancel_waiting_job)
				return true;
			else
				return false;
		}
	private:
		void _clear()
		{
			m_type_requst = _example::cjob_of_dll::req_type_job;
			m_n_worker_number = _example::cdll_const::const_invalid_worker_number;
			m_n_job_result_index = _example::cdll_const::result_index_error;
			m_result = _example::cdll_const::result_error;

			m_v_tx.resize(0);
			m_v_rx.resize(0);
			m_cb = nullptr;
			m_p_user_of_cb = nullptr;
			m_b_cancel_waiting_job = 1;
		}

		bool _set(
			const _example::cjob_of_dll::type_requst req_type,
			const long n_worker_number,
			const unsigned char* ps_data,
			const unsigned long n_data,
			const _example::cdll_const::type_dll_callback cb,
			void* p_user_of_cb,
			long b_cancel_waiting_job
		)
		{
			bool b_result(false);

			do {
				if (n_worker_number == _example::cdll_const::const_invalid_worker_number)
					continue;
				m_type_requst = req_type;
				m_n_worker_number = n_worker_number;

				m_v_tx.resize(0);

				if (ps_data != NULL && n_data != 0) {
					std::copy(&ps_data[0], &ps_data[n_data], std::back_inserter(m_v_tx));
				}

				m_cb = cb;
				m_p_user_of_cb = p_user_of_cb;
				m_b_cancel_waiting_job = b_cancel_waiting_job;

				b_result = true;
			} while (false);
			return b_result;
		}

	private:
		_example::cjob_of_dll::type_requst m_type_requst;
		long m_n_worker_number;
		_example::cdll_const::type_result_index m_n_job_result_index;
		_example::cdll_const::type_result m_result;

		ctools::type_buffer m_v_tx;
		ctools::type_buffer m_v_rx;
		_example::cdll_const::type_dll_callback m_cb;
		void* m_p_user_of_cb;
		_safe_cv m_cv_for_sync_result;
		long m_b_cancel_waiting_job;

	private://don't call these methods
		cjob_of_dll();
		cjob_of_dll(const cjob_of_dll& src);
		cjob_of_dll& operator=(const cjob_of_dll& src);

	};
}

