/**
 * Project: BMS, Project 1 - Error Correction Code (decoder)
 * Author: Anna Ostroukh <xostro00>
 * References: library rscode http://rscode.sourceforge.net (Reed Solomon code)
 * Error Correction Efficiency: 17% of random or burst errors
 * Output file increasing: 173%
 */

#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <algorithm>
#include <cstring>
#include "ecc.h"

using namespace std;

unsigned char codeword[255];
size_t const message_len = 147;        // length of the original message block
vector <unsigned char> output_file;

/** Decodes input file
 * @param file - input file
 * @return decodedStream - file after decoding
 * */
vector <unsigned char> decode (vector <unsigned char> file) {
    #define ML (message_len + NPAR)

    vector<unsigned char> decodedStream;    // data structure for decoding
    vector<unsigned char> block;            // data structure for slicing whole input file into blocks = sizeof(codeword) for encoding
    vector<unsigned char> :: iterator begin;
    vector<unsigned char> :: iterator end;

    begin = file.begin();
    end = file.end();

    auto to = begin;

    // read symbols from file till the end of the file
    while(to != end) {
        auto from = to;

        auto counter = sizeof(codeword);
        while (counter > 0 && to != end) {
            ++to;
            --counter;
        }

        block.clear();
        block.insert(block.end(), from, to);

        copy(block.begin(), block.end(), codeword);

        decode_data(codeword, ML);

        if (check_syndrome() != 0) {
            correct_errors_erasures(codeword, ML, 0, NULL);
        }

        decodedStream.insert(decodedStream.end(), codeword, codeword + (block.size() - NPAR));

        memset(codeword, 0, sizeof(codeword));
    }
    return decodedStream;
}

/** Deinterlieve data
 * @param input - input file
 * @param codeword_num - length of codeword
 * @return output - file after deinterleaving
 * */
vector <unsigned char> deinterleave_data(vector <unsigned char> input, int codeword_num) {
    vector <unsigned char> output;

    // interleaving of bytes - takes all first bytes from each codeword and wraps them together, then second and so on
    for(int i = 0; i < codeword_num; i++) {
        for (int n = 0; n < (int)sizeof(codeword); n++) {
            output.insert(output.end(), input [i + codeword_num * n]) ;
        }
    }

    // if we have not full codeword in file, get it between next to last and last full codewords
    if (input.size() % sizeof(codeword) != 0) {
        int rest_bytes = 0;
        rest_bytes = input.size() - codeword_num * sizeof(codeword);

        vector <unsigned char> nfcodeword (input.end() - rest_bytes, input.end());
        for (int i = 0; i < (int)nfcodeword.size(); i++)
            output.insert(output.end(), nfcodeword[i]);

        vector <unsigned char> bgcodeword (output.end() - sizeof(codeword) - rest_bytes, output.end() - sizeof(codeword));
        for (int i = 0; i < (int)bgcodeword.size(); i++) {
            output.insert(output.end(), bgcodeword[i]);
        }

        output.erase(output.end() - sizeof(codeword) - 2*rest_bytes, output.end() - sizeof(codeword) - rest_bytes);
    }

    return output;
}

int main(int argc, char *argv[]) {
    initialize_ecc ();

    vector <unsigned char> file;          // data structure for input file
    int codeword_num = 0;                 // number of full codewords
    string filename;

    if(argc < 2 ){
        cout << "File name is required" << endl;
        return 1;
    }

    filename = argv[1];

    ifstream stream (filename, ios::in | ios::binary); 

    // reading the content of input file
    for_each(istreambuf_iterator<char>(stream),
             istreambuf_iterator<char>(),
             [&file](const unsigned char c) {
                 file.push_back(c);
             });

    stream.close();

    codeword_num = file.size() / sizeof(codeword);
    output_file = decode(deinterleave_data(file, codeword_num));

    ofstream outFile((filename + ".ok").c_str(), ios::out | ios::binary);
    copy(output_file.begin(), output_file.end(), ostreambuf_iterator<char>(outFile));
}
