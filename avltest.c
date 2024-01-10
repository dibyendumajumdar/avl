#include "avl.h"

#include <assert.h>
#include <stdlib.h>

AVLTREE(char)

static void *alloc(void *arg, size_t n) { return calloc(1, n); }

static void release(void *arg, void *p) { free(p); }

static int comp(char a, char b) { return a < b ? -1 : (a > b ? 1 : 0); }

static AVLAllocator allocator = {.userArg = NULL, .alloc = alloc, .release = release};

static void verify(AVLTree_char *T, const char *values)
{
	const char *cp = values;
	AVLNode_char *n;
	for (cp = values; *cp; cp++)
		assert(AVLTree_Search_char(T, *cp));
	for (cp = values, n = AVLTree_FindFirst_char(T); *cp; cp++, n = AVLTree_FindNext_char(T, n)) {
		assert(n);
		assert(n->value == *cp);
	}
}

static void insert(AVLTree_char *T, const char *values)
{
	const char *cp = values;
	for (cp = values; *cp; cp++)
		AVLTree_Insert_char(T, *cp);
}

// https://stackoverflow.com/questions/3955680/how-to-check-if-my-avl-tree-implementation-is-correct

static void test1()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "abc");
	verify(T, "abc");
}

static void test2()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "cba");
	verify(T, "abc");
}

static void test3()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "acb");
	verify(T, "abc");
}

static void test4()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "cab");
	verify(T, "abc");
}

static void test5()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "bcad");
	AVLTree_Delete_char(T, 'a');
	verify(T, "bcd");
}

static void test6()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "cbda");
	AVLTree_Delete_char(T, 'd');
	verify(T, "abc");
}

static void test7()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "bdac");
	AVLTree_Delete_char(T, 'a');
	verify(T, "bcd");
}

static void test8()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "cadb");
	AVLTree_Delete_char(T, 'd');
	verify(T, "abc");
}

static void test9()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "cbedfag");
	AVLTree_Delete_char(T, 'a');
	verify(T, "bcdefg");
}

static void test9b()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "cbedfg");
	verify(T, "bcdefg");
}

static void test10()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "ecfbdga");
	AVLTree_Delete_char(T, 'g');
	verify(T, "abcdef");
}

static void test10b()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "ecfbda");
	verify(T, "abcdef");
}

static void test11()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "ecjadhkgilbf");
	AVLTree_Delete_char(T, 'b');
	verify(T, "acdefghijkl");
}

static void test11b()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "ecjadhkgilf");
	verify(T, "acdefghijkl");
}

static void test12()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "hckbeiladfjg");
	AVLTree_Delete_char(T, 'j');
	verify(T, "abcdefghikl");
}

static void test12b()
{
	AVLTree_char *T = AVLTree_New_char(allocator, comp);

	insert(T, "hckbeiladfg");
	verify(T, "abcdefghikl");
}

int main(int argc, const char *argv[])
{
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
	test9();
	test9b();
	test10();
	test10b();
	test11();
	test11b();
	test12();
	test12b();

	return 0;
}