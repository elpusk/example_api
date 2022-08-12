
#include "example_dll.h"
#include "cjob_mgmt_of_dll.h"

#include <clogger.h>


/**
* local function prototype
*/
static _example::cdll_const::type_result_index  _example_dll_start_aync
(
	const _example::cjob_of_dll::type_requst req_type,
	const long n_woker_number,
	const unsigned char* ps_data,
	const unsigned long n_data,
	const _example::cdll_const::type_dll_callback cb,
	void* p_user_of_cb,
	const long b_cancel_waiting_job
);

/**
* local function body
*/
static _example::cdll_const::type_result_index  _example_dll_start_aync
(
	const _example::cjob_of_dll::type_requst req_type,
	const long n_woker_number,
	const unsigned char* ps_data,
	const unsigned long n_data,
	const _example::cdll_const::type_dll_callback cb,
	void* p_user_of_cb,
	const long b_cancel_waiting_job
)
{
	_example::cdll_const::type_result_index n_result(_example::cdll_const::result_index_error);

	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : ENT.");

	do {
		_example::cjob_mgmt_of_dll& mgmt = _example::cjob_mgmt_of_dll::get_instance();
		n_result = mgmt.push_job(req_type, n_woker_number, ps_data, n_data, cb, p_user_of_cb, b_cancel_waiting_job);
		if (n_result == _example::cdll_const::result_index_error) {
			SPDLOG_LOGGER_ERROR(clogger::get_instance().get(), " : push_job().");
			continue;
		}

	} while (false);
	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : EXT.");
	return n_result;
}


_example::cdll_const::type_result __stdcall example_dll_on()
{
	_example::cdll_const::type_result n_result(_example::cdll_const::result_error);

	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : ENT.");
	do {
		//
		n_result = _example::cdll_const::result_success;
	} while (false);
	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : EXT.");
	return n_result;
}

_example::cdll_const::type_result __stdcall example_dll_off()
{
	_example::cdll_const::type_result n_result(_example::cdll_const::result_error);

	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : ENT.");
	do {

		//
		n_result = _example::cdll_const::result_success;
	} while (false);
	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : EXT.");
	return n_result;
}



/**
* @brief create worker for running job - mbcs version
* @param ps_name - the name of worker.( null terminate string format ).
* @param n_name - the number of character in ps_name buffer.(including terminate-null character)
* @param cb - [in] callback function pointer
* @param p_user_of_cb - [in] cb function parameter.
* @return the index number of result
*/
_example::cdll_const::type_result_index example_dll_create_worker_a(
	const char* ps_name,
	const unsigned long n_name,
	const _example::cdll_const::type_dll_callback cb,
	void* p_user_of_cb
)
{
	_example::cdll_const::type_result_index result_index(_example::cdll_const::result_index_error);

	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : ENT.");

	do {
		if (ps_name == NULL || n_name == 0)
			continue;
		ctools::type_buffer v_name(0);

		std::transform(&ps_name[0], &ps_name[n_name], std::back_inserter(v_name), [=](char c)->unsigned char {
			return (unsigned char)c;
			});

		result_index = _example_dll_start_aync(
			_example::cjob_of_dll::req_type_create_worker,
			_example::cdll_const::const_invalid_worker_number,
			&v_name[0],
			v_name.size(),
			cb,
			p_user_of_cb,
			0//not cancel all job
		);
	} while (false);

	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : EXT.");

	return result_index;
}

/**
* @brief create worker for running job - unicode version
* @param ps_name - the name of worker.( null terminate string format ).
* @param n_name - the number of character in ps_name buffer.(including terminate-null character)
* @param cb - [in] callback function pointer
* @param p_user_of_cb - [in] cb function parameter.
* @return the index number of result
*/
_example::cdll_const::type_result_index example_dll_create_worker_w(
	const wchar_t* ps_name,
	const unsigned long n_name,
	const _example::cdll_const::type_dll_callback cb,
	void* p_user_of_cb
)
{
	_example::cdll_const::type_result_index result_index(_example::cdll_const::result_index_error);
	
	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : ENT.");

	do {
		if (ps_name == NULL || n_name == 0)
			continue;
		ctools::type_buffer v_name(0);

		std::transform(&ps_name[0], &ps_name[n_name], std::back_inserter(v_name), [=](wchar_t c)->unsigned char {
			return (unsigned char)c;
			});

		result_index = _example_dll_start_aync(
			_example::cjob_of_dll::req_type_create_worker,
			_example::cdll_const::const_invalid_worker_number,
			&v_name[0],
			v_name.size(),
			cb,
			p_user_of_cb,
			0//not cancel all job
		);
	} while (false);

	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : EXT.");

	return result_index;
}

/**
* @brief remove worker for running job
* @param n_worker_number - worker number( greater then equal zero ).
* @param cb - [in] callback function pointer
* @param p_user_of_cb - [in] cb function parameter.
* @return the index number of result
*/
_example::cdll_const::type_result_index example_dll_remove_worker(
	long n_worker_number,
	const _example::cdll_const::type_dll_callback cb,
	void* p_user_of_cb
)
{
	_example::cdll_const::type_result_index result_index(_example::cdll_const::result_index_error);

	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : ENT.");

	result_index = _example_dll_start_aync(
		_example::cjob_of_dll::req_type_remove_worker,
		n_worker_number,
		NULL,
		0,
		cb,
		p_user_of_cb,
		1 //cancel all job
	);

	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : EXT.");

	return result_index;

}


_example::cdll_const::type_result_index __stdcall example_dll_run_job
(
	const long n_woker_number,
	const unsigned char* ps_data,
	const unsigned long n_data,
	const _example::cdll_const::type_dll_callback cb,
	void* p_user_of_cb,
	const long b_cancel_waiting_job
)
{
	return _example_dll_start_aync(_example::cjob_of_dll::req_type_job, n_woker_number, ps_data, n_data, cb, p_user_of_cb, b_cancel_waiting_job);
}


_example::cdll_const::type_result __stdcall example_dll_cancel_jobs(const long n_woker_number)
{
	_example::cdll_const::type_result n_result(_example::cdll_const::result_error);

	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : ENT.");
	do {
		_example::cjob_mgmt_of_dll& mgmt = _example::cjob_mgmt_of_dll::get_instance();
		if (mgmt.push_job(n_woker_number) == _example::cdll_const::result_index_error) {
			SPDLOG_LOGGER_ERROR(clogger::get_instance().get(), " : push_job().");
			continue;
		}
		//
		n_result = _example::cdll_const::result_success;
	} while (false);
	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : EXT.");
	return n_result;
}

_example::cdll_const::type_result __stdcall example_dll_get_result_of_job(
	const _example::cdll_const::type_result_index n_result_index,
	unsigned char* ps_out_data, unsigned long* pn_out_data,
	const long b_remove_result
)
{
	_example::cdll_const::type_result n_result(_example::cdll_const::result_error);

	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : ENT.");

	do {
		if (ps_out_data != NULL && pn_out_data == NULL) {
			continue;
		}

		_example::cjob_mgmt_of_dll& mgmt = _example::cjob_mgmt_of_dll::get_instance();

		ctools::type_buffer v_rx(0);

		bool b_remove(true);

		if (!b_remove_result)
			b_remove = false;

		n_result = mgmt.get_result(n_result_index, v_rx, b_remove);

		if (v_rx.empty())
			continue;
		if (pn_out_data)
			*pn_out_data = v_rx.size();
		if (ps_out_data)
			memcpy(ps_out_data, &v_rx[0], v_rx.size());
		//
		
	} while (false);
	SPDLOG_LOGGER_INFO(clogger::get_instance().get(), " : EXT.");
	return n_result;
}
