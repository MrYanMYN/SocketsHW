#include<iostream>
#include<fstream>
#include<string>
using namespace std;

const int MAXNAME = 20;

int main() {
    ifstream inData("input.txt");
    string str = "";

    while (!inData.eof()) {//use eof=END OF FILE
        //getline(inData, str); use to take the hole row

        /*can use string also
        string firstName;
        string lastName;
        string avg;*/

        char decorator[MAXNAME];
        float value;

        //you can read from a file like you read from console(cin)
        inData >> decorator >> value;//split the row
    }

    inData.close();
    return 0;
}