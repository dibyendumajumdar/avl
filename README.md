# AVL Tree in C

AVL Tree where values are copyable, i.e. primitive types, pointers or struct types.
A macro acts as template to generate the code for given value type.

Example:

```C

#include "avl.h"

// Generate AVL Tree code for int values.
AVLTREE(int)
```

See `avltest.c` for example of how to use it.

You must provide some helper functions:

* Memory allocation and release
* Comparison

Enjoy!
