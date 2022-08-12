#pragma once

#include "example_dll_const.h"
/**
* history
* 2022.08.XX - starting
* 
*/


/**
* @brief initialize dll before using dll's functions 
* @return the result of processing
*/
_example::cdll_const::type_result __stdcall example_dll_on();

/**
* @brief free resource before unbinding dll.
* @return the result of processing
*/
_example::cdll_const::type_result __stdcall example_dll_off();


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
);

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
);

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
);

/**
* @brief start a type job. 
* @param n_worker_number - worker number( greater then equal zero ).
* @param ps_data - [in] the given data for running job.
* @param n_data - [in] the size of ps_data.
* @param cb - [in] callback function pointer
* @param p_user_of_cb - [in] cb function parameter.
* @param b_cancel_waiting_job - [in] 0 not be canceled(callback with error), else be canceled the waiting job.
* @return the index number of result
*/
_example::cdll_const::type_result_index __stdcall example_dll_run_job(
	const long n_woker_number,
	const unsigned char* ps_data, 
	const unsigned long n_data,
	const _example::cdll_const::type_dll_callback cb,
	void *p_user_of_cb,
	const long b_cancel_waiting_job 
);

/**
* @brief cancel the running job of example_dll_run_job1() or example_dll_run_job2().
* @param n_worker_number - worker number( greater then equal zero ).
* @return the result of processing
* 
*/
_example::cdll_const::type_result __stdcall example_dll_cancel_jobs( 
	const long n_woker_number
);


/**
* @brief get the result of job processing. this function may be called in callback function( type_dll_callback type ).
* @param n_result_index - [in] return value of example_dll_run_job1 or 2
* @param ps_out_data - [out] for returning, the allocated buffer.
* @param pn_out_data - [in] the size of ps_out_data's buffer. [out]the size of the saved data.
* @param b_remove_result - [in] 0 don't remove result data. else remove result data.
* @return the result of processing
* @remark
*/
_example::cdll_const::type_result __stdcall example_dll_get_result_of_job( 
	const _example::cdll_const::type_result_index n_result_index,
	unsigned char* ps_out_data,
	unsigned long* pn_out_data,
	const long b_remove_result
	);