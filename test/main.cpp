#include <kerchow/kerchow.h>
#include <iostream>

int main() {
   auto fuzzy_number = kerchow::picker.create_fuzzy_number(1, 10);

   std::cout << "Generating random number in [1, 10]: " << fuzzy_number << std::endl;

   return 0;
}
