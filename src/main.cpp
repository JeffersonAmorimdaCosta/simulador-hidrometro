#include <iostream>
#include "hidrometro.cpp"

using namespace std;

int main(void) {

    Hidrometro h(1);

    cout << h.get_valor() << endl;
    
    return 0;
}