#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h> 

typedef long (*syscall_fn_t)(long, long, long, long, long, long, long);

static syscall_fn_t next_sys_call = NULL;

static long hook_function(long a1, long a2, long a3,
			  long a4, long a5, long a6,
			  long a7)
{
	// printf("output from hook_function: syscall number %ld\n", a1);
	if (a1 == 59) {  
        char *const *argv = (char *const *)a3;

       	// int count;
        // for (count = 0; argv[count] != NULL; ++count) {
        //     printf("  argv[%d]: %s\n", count, argv[count]);
        // }

        char **new_argv = malloc(8 * sizeof(char *));
        if (!new_argv) {
            perror("Failed to allocate memory for new argv");
            return -1;
        }
		new_argv[0] = argv[0];
		new_argv[1] = "-f";
		new_argv[2] = "future";
		new_argv[3] = "-F";
		new_argv[4] = "border";
		new_argv[5] = "--gay";
		new_argv[6] = argv[5];
        new_argv[7] = NULL;

		uintptr_t new_argv_addr = (uintptr_t)new_argv;
		// printf("The address of new_argv is: %p or (as integer) %lu\n", (void *)new_argv, new_argv_addr);
		return next_sys_call(a1, a2, new_argv_addr, a4, a5, a6, a7);
      
    }

	return next_sys_call(a1, a2, a3, a4, a5, a6, a7);
}

int __hook_init(long placeholder __attribute__((unused)),
		void *sys_call_hook_ptr)
{
	// printf("output from __hook_init: we can do some init work here\n");

	next_sys_call = *((syscall_fn_t *) sys_call_hook_ptr);
	*((syscall_fn_t *) sys_call_hook_ptr) = hook_function;

	return 0;
}