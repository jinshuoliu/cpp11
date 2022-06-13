#include <iostream>
using namespace std;

void print(std::initializer_list<int> vals)
{
  for(auto vals.begin(); p!=vals.end(); ++p)
    std::cout << *p << "\n";
}

print({1,2,3,4,5,6,7});