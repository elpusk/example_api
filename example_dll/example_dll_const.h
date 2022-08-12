#pragma once

#include <utility>
/**
* history
* 
*/
#ifndef __WFUNCTION__

#define WIDEN(x)           L ## x
#define WIDEN2(x)         WIDEN(x)
#define __WFILE__          WIDEN2(__FILE__)
#define __WFUNCTION__ WIDEN2(__FUNCTION__)

#endif // !__WFUNCTION__


namespace _example
{
	/**
	* Don't create the instance of this class.
	*/
	class cdll_const {

	public:
		typedef enum : long {
			const_invalid_worker_number = -1
		};

		typedef enum : long {
			result_success = 0,
			result_ing = 1,					//async processing start OK!
			result_cancel = 2,				//async processing have been canceled by cancel request
			result_cancel_by_aonther = 3,	//async processing have been canceled by another request( except cencel request )
			result_error = -1				//generic error
		} type_result;

		typedef enum : long {
			//zero and postive value - the index of processing result
			result_index_error = -1		//fail start async processing
		} type_result_index;


		/**
		* @parameters first - worker number
		* @parameters second - result index
		* @parameters third - result value
		* @parameters fourth - user parameter
		*/
		typedef	type_result(__stdcall* type_dll_callback)(long, _example::cdll_const::type_result_index, _example::cdll_const::type_result,void*);

		/**
		* first : process is completed successfully or not, second : this processing is running or not( running - must be called again )
		*/
		typedef std::pair<bool, bool>	type_pair_success_continue;

		/**
		* first : process is run or not, second : the result of processing is success or not.
		*/
		typedef std::pair<bool, bool>	type_pair_run_success;

	private://don't call these methods
		cdll_const();
		cdll_const(const cdll_const&);
		cdll_const& operator=(const cdll_const&);
	};
}