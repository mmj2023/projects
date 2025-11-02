#include <unistd.h>

int main(int argc, char *argv[])
{
	write(1, "\nHello World!\n", 14);
}
