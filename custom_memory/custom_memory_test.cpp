// custom_memory.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

extern "C" int test_custom_memory_operator (void);
extern "C" int test_custom_memory_stl_allocator (void);

int _tmain(int argc, _TCHAR* argv[])
{
	test_custom_memory_operator();
	test_custom_memory_stl_allocator();

	return 0;
}
