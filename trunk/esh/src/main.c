char* gTestString = "### Kernel sys_write welcomes you! ###\n";

int main(void) {
    __asm__ __volatile__ ("pusha");
    __asm__ __volatile__ ("mov $4, %eax");   // syscall id (sys_write)
    __asm__ __volatile__ ("mov $1, %ebx");   // stdio
    __asm__ __volatile__ ("movl %0, %%ecx;" :: "m"(gTestString) : "%ecx"); //string offset
    __asm__ __volatile__ ("mov $30, %edx;"); // string length
    __asm__ __volatile__ ("int $69");
    __asm__ __volatile__ ("popa");
    for(;;);
}
