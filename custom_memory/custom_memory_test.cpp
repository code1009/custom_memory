// custom_memory.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
