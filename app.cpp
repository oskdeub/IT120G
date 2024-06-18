#include <iostream>
using namespace std;


/*
- #include <ctime>;
- srand(time(0));
- rand() % 36 + 1;
*/
int main(){
    string namn;
    std::cout<<"Vad heter du? ";
    cin >> namn;
    cout << "Du heter alltså " << namn << endl;
    return 0;
}