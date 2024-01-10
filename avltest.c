#include "avl.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static void *alloc(void *arg, size_t n) {
   return calloc(1, n);
}

static void release(void *arg, void *p) {
   free(p);
}

static int comp(ValueType a, ValueType b) {
   return a < b ? -1 : (a > b ? 1 : 0);
}

static AVLAllocator allocator = {
      .userArg = NULL,
      .alloc = alloc,
      .release = release
};

static void verify(AVLTree *T, const char *values) {
   const char *cp = values;
   AVLNode *n;
   for (cp = values; *cp; cp++)
      assert(AVLTree_Search(T, *cp));
   for (cp = values, n = AVLTree_FindFirst(T); *cp; cp++, n = AVLTree_FindNext(T, n)) {
      assert(n);
      assert(n->value == *cp);
   }
}

static void insert(AVLTree *T, const char *values) {
   const char *cp = values;
   for (cp = values; *cp; cp++)
      AVLTree_Insert(T, *cp);
}

// https://stackoverflow.com/questions/3955680/how-to-check-if-my-avl-tree-implementation-is-correct

static void test1() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "abc");
   verify(T, "abc");
}

static void test2() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "cba");
   verify(T, "abc");
}

static void test3() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "acb");
   verify(T, "abc");
}

static void test4() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "cab");
   verify(T, "abc");
}

static void test5() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "bcad");
   AVLTree_Delete(T, 'a');
   verify(T, "bcd");
}

static void test6() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "cbda");
   AVLTree_Delete(T, 'd');
   verify(T, "abc");
}

static void test7() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "bdac");
   AVLTree_Delete(T, 'a');
   verify(T, "bcd");
}

static void test8() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "cadb");
   AVLTree_Delete(T, 'd');
   verify(T, "abc");
}

static void test9() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "cbedfag");
   AVLTree_Delete(T, 'a');
   verify(T, "bcdefg");
}

static void test9b() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "cbedfg");
   verify(T, "bcdefg");
}

static void test10() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "ecfbdga");
   AVLTree_Delete(T, 'g');
   verify(T, "abcdef");
}

static void test10b() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "ecfbda");
   verify(T, "abcdef");
}

static void test11() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "ecjadhkgilbf");
   AVLTree_Delete(T, 'b');
   verify(T, "acdefghijkl");
}

static void test11b() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "ecjadhkgilf");
   verify(T, "acdefghijkl");
}

static void test12() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "hckbeiladfjg");
   AVLTree_Delete(T, 'j');
   verify(T, "abcdefghikl");
}

static void test12b() {
   AVLTree *T = AVLTree_New(allocator, comp);

   insert(T, "hckbeiladfg");
   verify(T, "abcdefghikl");
}

int main(int argc, const char *argv[]) {
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