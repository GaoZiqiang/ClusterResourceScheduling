#include <iostream>
#include <fstream>          // file I/O support
#include <cstdlib>          // support for exit()

double * getAHPParams(char * file_path)
{
    using namespace std;
    ifstream inFile;        // object for handling file input

    inFile.open(file_path);  // associate inFile with a file
    if (!inFile.is_open())  // failed to open file
    {
        cout << "Could not open the file " << file_path << endl;
        cout << "Program terminating.\n";
        exit(1);
    }
    double value;
    double *ahp_params = new double[4];
    int i = 0;

    inFile >> value;        // get first value
    while (inFile.good())   // while input good and not at EOF
    {
        ahp_params[i++] = value;
        inFile >> value;    // get next value
    }
    if (inFile.eof()) {
//        cout << "End of file reached.\n";
        inFile.close();
        return ahp_params;
    }
    else if (inFile.fail()) {
        cout << "Input terminated by data mismatch.\n";
        exit(1);
    }
    else {
        cout << "Input terminated for unknown reason.\n";
        exit(1);
    }

}