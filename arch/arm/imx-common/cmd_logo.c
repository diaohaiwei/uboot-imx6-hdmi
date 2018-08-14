#include <common.h>
#include <command.h>


extern unsigned int __logo_start;
extern unsigned int __logo_end;

static int do_logo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    volatile char *p = (volatile char *)(&__logo_start);
    int i;
    unsigned int len = 0;


    len = (unsigned int)&__logo_end - (unsigned int)&__logo_start;


    printf("__logo_start = %p\n", &__logo_start);
    printf("__logo_end = %p\n", &__logo_end);
    printf("logo size is %x\n", len);

    for (i = 0; i < len; i++)
    {
        if(i % 16 == 0)
        {
            printf("\n");
        }

        printf("%3x ", *p++&0xff);
    }
    printf("\n");

    return 0;

}


U_BOOT_CMD(
    logo,    CONFIG_SYS_MAXARGS,    1,    do_logo,
    "minimal test like /bin/sh",
    "[args..]"
);


