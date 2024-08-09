
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <cmath>
// #include <algorithm>
// #include "int2bmp.cpp"

// using namespace std;

// vector<uint8_t> read_file_as_binary(const string &file_path)
// {

//     ifstream file(file_path, ios::binary | ios::ate);

//     if (!file)
//     {

//         throw runtime_error("Error opening file.");
//     }

//     streamsize size = file.tellg();

//     file.seekg(0, ios::beg);

//     vector<uint8_t> buffer(size);

//     if (file.read(reinterpret_cast<char *>(buffer.data()), size))
//     {

//         return buffer;
//     }
//     else
//     {

//         throw runtime_error("Error reading file.");
//     }
// }

// int main()
// {
//     cout << "This code ran" << endl;

//     try
//     {

//         string pdf_path = "Chapter7-OrbitalMechanics.PDF.pdf";

//         vector<uint8_t> binary_data = read_file_as_binary(pdf_path);

//         unsigned long huge_integer = 0;

//         long total_size = binary_data.size() * 8;

//         long total_compress_size = 0, total_net_size = 0;

//         int size1 = 0;

//         generateBMP("output1.bmp", &binary_data);

        
//     }
//     catch (const exception &e)
//     {

//         cerr << "Error: " << e.what() << endl;
//     }

//     return 0;
// }