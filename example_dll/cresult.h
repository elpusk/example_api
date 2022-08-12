#pragma once

#include <map>

#include "example_dll_const.h"
#include "cjob_of_dll.h"

namespace _example
{

	class cresult
	{
	private:
		typedef	std::map< cdll_const::type_result_index, cjob_of_dll::type_ptr_cjob_of_dll >	_type_map_index_result;

	public:
		cresult() : m_n_cur_result_index(_example::cdll_const::result_index_error)
		{}

		virtual ~cresult() {

		}

		bool emplace_result_notify_remove_ptr_job(_example::cjob_of_dll::type_ptr_cjob_of_dll& ptr_job)
		{
			bool b_result(false);
			do {
				std::lock_guard<std::mutex> lock(m_mutex_result);
				if (!ptr_job)
					continue;
				auto result = m_map_result.emplace(ptr_job->get_result_index(), ptr_job);
				b_result = result.second;
			} while (false);

			if (ptr_job)
				ptr_job->notify();
			ptr_job.reset();
			//
			return b_result;
		}

		_example::cdll_const::type_result get_result(const _example::cdll_const::type_result_index result_index, std::vector<unsigned char>& v_rx, bool b_remove_result_after_get = true)
		{
			_example::cdll_const::type_result result(_example::cdll_const::result_error);
			do {
				std::lock_guard<std::mutex> lock(m_mutex_result);

				v_rx.resize(0);

				auto it = m_map_result.find(result_index);
				if (it == std::end(m_map_result))
					continue;

				_example::cjob_of_dll::type_ptr_cjob_of_dll ptr_job(it->second);

				std::tie(result, v_rx) = ptr_job->get_result();

				if (b_remove_result_after_get)
					m_map_result.erase(it);
				//
			} while (false);
			return result;
		}

		cdll_const::type_result_index generate_new_result_index()
		{
			if (m_n_cur_result_index == cdll_const::result_index_error)
				m_n_cur_result_index = 0;
			else
				++m_n_cur_result_index;
			//
			return (cdll_const::type_result_index)m_n_cur_result_index;
		}

	protected:
		std::mutex m_mutex_result;
		_type_map_index_result m_map_result;
		long m_n_cur_result_index;


	};
}