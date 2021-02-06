#include "bigint.h"
#include <iostream>
#include <fstream>
using namespace std;


int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout<<"\033[1;41m----Missing an argument----\033[0m"<<endl;
	    cout << "Usage: ./decipher <file>"<<endl;
	    cout << "Examples:" << endl;
	    cout << "\t./decipher secret/message1.txt \033[1;90m//to decipher message 1\033[0m" << endl;
	    cout << "\t./decipher secret/message2.txt \033[1;90m//to decipher message 2\033[0m"<<endl;
		return -1;
	}

    try{
        
    int base;
    string d;
    string n;
    string secretCode;
    BigInt actualCode;
    
    ifstream ifile;
    ifile.open(argv[1]);
    
    ifile >> base >> d >> n;
    BigInt key(d, base);
    BigInt mod(n, base);

    while (ifile >> secretCode) {
        BigInt code(secretCode, base);
        actualCode = modPow(code, key, mod);
        cout << (char)actualCode.to_int();
    }
    
    cout << endl;
        
    }
    catch(const exception& e){
            cout<<e.what()<<endl;//print the message of the exception defined in its "what" function.
            return -1;//end the program.
        }
    
  
  
	return 0;
}
