#include <iostream>
#include "hidrometro.cpp"
#include "cano.hpp"

using namespace std;

int main(void) {

    Hidrometro h(1);
    Cano c(1, 2, 3);

    cout << h.get_valor() << endl;
    cout << c.getBitola() << c.getPressao() << c.getVazao() << endl;
    
    return 0;
}