
#define MINIOS_APP __attribute__ ((section (".entry_point"))) int
#define STACK_INFO __attribute__ ((section (".stack_info"))) int

STACK_INFO stack_sz = 0x100000;   //1MB

int main(void);

MINIOS_APP c_runtime(){
    //Manually Push LR (for some reason gcc doesn't do it)
    asm volatile ( "push {lr}" );


    //Set up bss
    //...missing...


    //Go main
    int ret_val = main();

    //Manually Restore LR and go back to the kernel
    asm volatile ( "pop {lr}" );
    return ret_val;
}
