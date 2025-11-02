#include <iostream>
int main()
{
    int* p = new int(42);
    std::cout << "Memory address of p: ";
    std::cout << p << "\n";
    std::cout << "Value of p: ";
    std::cout << *p << "\n";
    return 0;
}
