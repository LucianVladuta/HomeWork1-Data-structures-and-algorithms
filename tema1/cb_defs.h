#pragma once

typedef void * (*cb_allocator)(void);
typedef void (*cb_deallocator)(void *data);
typedef void (*cb_data_copy)(void *src, void *dst);
typedef void (*cb_iter)(void *data);
typedef int (*cb_find)(void * data, void *look_for);
