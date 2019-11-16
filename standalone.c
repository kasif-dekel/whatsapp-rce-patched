#include <stdio.h>
#include <dlfcn.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

typedef void (*target_func)(char *filename, struct timespec *tp, int boolean);

int main(int argc, char ** argv) {
	char *whatsapp_lib = getenv("WHATSAPPLIB"); ///data/app/com.whatsapp-jGNjjt4b30_iU-zrMmQ2eA==/lib/x86/libwhatsapp/libwhatsapp.so
	struct timespec tp;
	clock_gettime(1, &tp);

    if (argc < 2) {
        printf("no args provided\n");
        return 1;
    }

	void *handle = dlopen(whatsapp_lib, RTLD_LAZY);

	if (NULL == handle) {
		printf("library error\n");
		return 1;
	}

	void *offset_func = dlsym(handle, "Java_com_whatsapp_Mp4Ops_mp4check");

	if (NULL == offset_func) {
		printf("getprocaddress error\n");
		return 1;
	}

	target_func target = (target_func)((unsigned char *)offset_func + 0x46bf0);
	printf("target function addr: %x\n", (unsigned int)offset_func + 0x46bf0);

#ifdef __AFL_HAVE_MANUAL_CONTROL
      __AFL_INIT();
#endif

	target(argv[1], &tp, 1);

	return 0;
}
