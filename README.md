# example_api
example of device api structure.

device 지원을 위한 dynamic linked library(이하 dll)의 기본 구조 예시.

# Version
0.9.0.1  그냥 0.9

# Specification
## target platform 
- for windows 10 x64
- for multi-threading.
- for single process.

## Format
- win32 regular dll x64

## the exported function
prototype 은 example_dll.h 에 정의

### function list
- [example_dll_on()](#example_dll_on())
- [example_dll_off()](#example_dll_off())
- [example_dll_create_worker_a()](#example_dll_create_worker_a())
- [example_dll_create_worker_w()](#example_dll_create_worker_w())
- [example_dll_remove_worker()](#example_dll_remove_worker())
- [example_dll_run_job()](#example_dll_run_job())
- [example_dll_cancel_jobs()](#example_dll_cancel_jobs())
- [example_dll_get_result_of_job()](#example_dll_get_result_of_job())

### example_dll_on()
- 동기식, 취소 불가.
- dll 초기화 작업 수행.
- dll 이 application에 binding 된 직 후, 한 번만 실행.
- prototype 
```cpp
_example::cdll_const::type_result __stdcall example_dll_on()
```
- return 
  - 예제에서는 항상 _example::cdll_const::result_success
  - 실전에서는 아마 _example::cdll_const::result_success 또는 _example::cdll_const::result_index_error

### example_dll_off()
- 동기식, 취소 불가.
- dll 의 모든 종료 작업 수행.
- dll 이 application에서 unbinding 되기 직전에 한 번만 실행.
- prototype 
```cpp
 _example::cdll_const::type_result __stdcall example_dll_off()
```
- return 
  - 예제에서는 항상 _example::cdll_const::result_success
  - 실전에서도 아마 _example::cdll_const::result_success 만 있을 듯.
  
### example_dll_create_worker_a()
- 비동기식, 취소 불가.
- multi byte code set(이하 mbcs) 를 사용.
- dll를 사용하는 application 이 mbcs 를 사용하면, 이 함수를 사용.( example_dll_create_worker_w() 사용 불가)
- dll 에서 작업을 수행 할 내부 worker(thread를 포함한 작업처리 객체) 생성 및 초기화를 요청.
- 요청의 완려되면, 인자를 받은 callabck 함수를 호출 처리 결과를 전달.
- 이 함수가 성공하면, callback 함수에서 생성된 worker 의 number 를 얻을 수 있음.
- 이 함수는 마치 device 사용 전에, open 명령으로 device 에 대한 handle 를 얻는 것과 비슷한다.
- prototype 
```cpp
_example::cdll_const::type_result_index example_dll_create_worker_a(
    const char* ps_name,
	const unsigned long n_name,
	const _example::cdll_const::type_dll_callback cb,
	void* p_user_of_cb
)
```
- parameters
  - ps_name : ASCII code 형식으로 생성 할 worker 의 이름.
    - 고유한 값이어야 한다. 
    - C 언어에서 사용하는 끝이 null 로 끝나는 문자열 형식. 
    - ASCII code 중 표시 가능한 문자열만 사용해야 한다.( 제어 문자 사용 금지)
    - 만약 주어진 이름의 worker 가 이미 존재 하면, callback 함수는 에러를 가지고 호출된다.
  - n_name : ps_name 에 저장된 문자열의 개수
    - 끝에 있는 null 까지 포함.
  - cb : worker 생성 작업 완료시 dll 내부에서 호출되는 callback 함수. 
    - callback 함수 호출시 1'st 인자는 생성된 worker number
    - callback 함수 호출시 2'nd 인자는 처리 결과 얻어진 추가 데이타에 대한 result index
    - callback 함수 호출시 3'th 인자는 처리결과를 나타내는 숫자
    - callback 함수 호출시 4'th 인자는 application 제공한 메모리 주소.
  - p_user_of_cb : dll 내부에서 cb 를 호출 할때 4'th 인자로 제공되는 메모리 주소.
- return 
  - _example::cdll_const::result_index_error : 작업 요청 자체를 실패.
  - 0 또는 양수 : 작업 완료시, example_dll_get_result_of_job()를 통해 추가적으로 얻을 수 있는 데이타에 대한  result index.

### example_dll_create_worker_w()
- 비동기식, 취소 불가.
- unicode 를 사용.
- dll를 사용하는 application 이 unicode 를 사용하면, 이 함수를 사용.( example_dll_create_worker_a()) 사용 불가)
- dll 에서 작업을 수행 할 내부 worker 생성 및 초기화를 요청.
- 요청의 완려되면, 인자를 받은 callabck 함수를 호출 처리 결과를 전달.
- 이 함수가 성공하면, callback 함수에서 생성된 worker 의 number 를 얻을 수 있음.
- 이 함수는 마치 device 사용 전에, open 명령으로 device 에 대한 handle 를 얻는 것과 비슷한다.
- prototype 
```cpp
_example::cdll_const::type_result_index example_dll_create_worker_w(
	const wchar_t* ps_name,
	const unsigned long n_name,
	const _example::cdll_const::type_dll_callback cb,
	void* p_user_of_cb
)
```
- parameters
  - ps_name : unicode 형식으로 생성 할 worker 의 이름.
    - 고유한 값이어야 한다. 
    - C 언어에서 사용하는 끝이 null 로 끝나는 문자열 형식. 
    - ASCII code 중 표시 가능한 문자를 unicode로 변경한 문자만 사용해야 한다.( 제어 문자 사용 금지)
    - 만약 주어진 이름의 worker 가 이미 존재 하면, callback 함수는 에러를 가지고 호출된다.
  - n_name : ps_name 에 저장된 문자열의 개수
    - 끝에 있는 null 까지 포함.
  - cb : worker 생성 작업 완료시 dll 내부에서 호출되는 callback 함수. 
    - callback 함수 호출시 1'st 인자는 생성된 worker number
    - callback 함수 호출시 2'nd 인자는 처리 결과 얻어진 추가 데이타에 대한 result index
    - callback 함수 호출시 3'th 인자는 처리결과를 나타내는 숫자
    - callback 함수 호출시 4'th 인자는 application 제공한 메모리 주소.
  - p_user_of_cb : dll 내부에서 cb 를 호출 할때 4'th 인자로 제공되는 메모리 주소.
- return 
  - _example::cdll_const::result_index_error : 작업 요청 자체를 실패.
  - 0 또는 양수 : 작업 완료시, example_dll_get_result_of_job()를 통해 추가적으로 얻을 수 있는 데이타에 대한  result index.

### example_dll_remove_worker()
- 비동기식, 취소 불가.
- 특정 worker 를 삭제.
- 이 함수는 마치 device 사용 후에, close 명령으로 device handle 를 system 에 반환 하는 것과 비슷한다.
- prototype 
```cpp
_example::cdll_const::type_result_index example_dll_remove_worker(
	long n_worker_number,
	const _example::cdll_const::type_dll_callback cb,
	void* p_user_of_cb
)
```
- parameters
  - n_worker_number : 제거할 worker number.
  - cb : worker 생성 작업 완료시 dll 내부에서 호출되는 callback 함수. 
    - callback 함수 호출시 1'st 인자는 제거되는 worker number
    - callback 함수 호출시 2'nd 인자는 처리 결과 얻어진 추가 데이타에 대한 result index
    - callback 함수 호출시 3'th 인자는 처리결과를 나타내는 숫자
    - callback 함수 호출시 4'th 인자는 application 제공한 메모리 주소.
  - p_user_of_cb : dll 내부에서 cb 를 호출 할때 4'th 인자로 제공되는 메모리 주소.
- return 
  - _example::cdll_const::result_index_error : 작업 요청 자체를 실패.
  - 0 또는 양수 : 작업 완료시, example_dll_get_result_of_job()를 통해 추가적으로 얻을 수 있는 데이타에 대한  result index.

### example_dll_run_job()
- 비동기식, 취소 가능.
- dll 특정 worker 에게 어떤 작업 실행을 요청.
- 인자로 전달되는 buffer(ps_data, n_data)의 내용에 따라 다수의 작업 실행이 가능 하도록 할 수 있다.
- prototype 
```cpp
_example::cdll_const::type_result_index __stdcall example_dll_run_job(
	const long n_woker_number,
	const unsigned char* ps_data, 
	const unsigned long n_data,
	const _example::cdll_const::type_dll_callback cb,
	void *p_user_of_cb,
	const long b_cancel_waiting_job 
)
```
- parameters
  - n_worker_number : 작업을 수행 할 worker number.
  - ps_data : 작업에 필요한 데이타가 저장된 버퍼 주소.
  - n_data : ps_data 버퍼의 크기(단위 byte)
  - cb : worker 생성 작업 완료시 dll 내부에서 호출되는 callback 함수. 
    - callback 함수 호출시 1'st 인자는 작업을 수행 할 worker number
    - callback 함수 호출시 2'nd 인자는 처리 결과 얻어진 추가 데이타에 대한 result index
    - callback 함수 호출시 3'th 인자는 처리결과를 나타내는 숫자
    - callback 함수 호출시 4'th 인자는 application 제공한 메모리 주소.
  - p_user_of_cb : dll 내부에서 cb 를 호출 할때 4'th 인자로 제공되는 메모리 주소.
  - b_cancel_waiting_job : 기존 처리 중인 작업의 취소 여부
    - 0 : 선택된 worker 가 현재 다른 작업 중이면, 현재 요청을 에러 처리.
    - 1 : 선택된 worker 가 현재 다른 작업 중이면, 현재 작업 취소하고, 요청한 작업 수행. cb 의 3'th 인자 값이 _example::cdll_const::result_cancel_by_aonther 가 된다.
- return 
  - _example::cdll_const::result_index_error : 작업 요청 자체를 실패.
  - 0 또는 양수 : 작업 완료시, example_dll_get_result_of_job()를 통해 추가적으로 얻을 수 있는 데이타에 대한  result index.

### example_dll_cancel_jobs()
- 동기식, 취소 불가.
- dll 특정 worker의 현재 진행 중인 작업 중단 요청.
- 이 함수는 중단 요청만 하고, 실제 중단 되었는지는 고려하지 않음.
- 실제 중단이 발생하면, example_dll_run_job() 에 의해 요청된 작업이기 때문에, example_dll_run_job()에서 인자로 받은 callback 함수가 3'th 인자 값을 _example::cdll_const::result_cancel로 설정되어 호출됨.
- prototype 
```cpp
_example::cdll_const::type_result __stdcall example_dll_cancel_jobs( 
	const long n_woker_number
)
```
- parameters
  - n_worker_number : 현재 수행 중인 작업을 취소 할 worker number.
- return 
  - _example::cdll_const::result_index_error : 취소 요청 실패.
  - _example::cdll_const::result_success : 취소 요청 성공.

### example_dll_get_result_of_job()
- 동기식, 취소 불가.
- 비동기식 함수 실행시, 실행 결과가 callback 함수에 의해 전달되는데, 이 함수로 추가적인 처리 결과 데이타가 있으면 얻을 수 있다.
- prototype 
```cpp
_example::cdll_const::type_result __stdcall example_dll_get_result_of_job( 
	const _example::cdll_const::type_result_index n_result_index,
	unsigned char* ps_out_data,
	unsigned long* pn_out_data,
    const long b_remove_result
	)
```
- parameters
  - n_result_index : 추가적으로 얻을 수 있는 데이타에 대한 result index. 비동기 함수의 반환값. example_dll_run_job() 만 추가적인 데이타가 있을 수 있다.
  - ps_out_data : 추가적으로 얻은 데이타를 저장할 버퍼 주소. 이 값은 null 일 수 있다.
  - pn_out_data : 4 byte 크기의 주소.
    - 입력으로 ps_out_data 버퍼 크기(byte 단위)가 저장된 곳의 주소. 
    - 출력으로 ps_out_data 에 저장한 데이타의 크기(byte 단위). 이 값은 null 일수 없다.
  - b_remove_result : 
    - 0 : 데이타를 얻은 후, 데이타를 dll 에서 삭제.
    - 1 : 데이타를 얻은 후, 데이타를 dll 에서 유지.
- return 
  - _example::cdll_const::result_index_error : 취소 요청 실패.
  - _example::cdll_const::result_success : 취소 요청 성공.






