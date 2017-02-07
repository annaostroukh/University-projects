/**
 * Project: BMS, Project 1 - Error Correction Code (encoder)
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
size_t const message_len = 147;      // length of the original message
vector <unsigned char> output_file;

/** Encodes input file
 * @param file - input file
 * @return encodedStream - file after encoding
 * */
vector <unsigned char> encode (vector <unsigned char> file) {
    vector <unsigned char> encodedStream;   // data structure for encoded data
    vector<unsigned char> block;            // data structure for slicing whole input file into blocks = message_len for encoding
    vector<unsigned char> :: iterator begin;
    vector<unsigned char> :: iterator end;

    begin = file.begin();
    end = file.end();

    auto to = begin;

    while(to != end) {
        auto from = to;

        auto counter = message_len;
        while (counter > 0 && to != end) {
            ++to;
            --counter;
        }

        block.clear();
        block.insert(block.end(), from, to);

        encode_data(block.data(), block.size(), codeword);

        // if we have not full codeword in file, put it between next to last and last full codewords
        if (block.size() < message_len) {
            encodedStream.insert(encodedStream.end() - sizeof(codeword), codeword, codeword + (block.size() + NPAR));
        }
        else
            encodedStream.insert(encodedStream.end(), codeword, codeword + (block.size() + NPAR));
        memset(codeword, 0, sizeof(codeword));
    }

    return encodedStream;
}

/** Interlieve data
 * @param input - input file
 * @param codeword_num - length of codeword
 * @return output - file after interleaving
 * */
vector <unsigned char> interleave_data(vector <unsigned char> encodedStream, int codeword_num) {
    vector <unsigned char> output;

    // interleaving of bytes - takes all first bytes from each codeword and wraps them together, then second and so on
    for(int i = 0; i < (int)sizeof(codeword); i++) {
        for (int n = 0; n < codeword_num; n++) {
            output.insert(output.end(), encodedStream [i + sizeof(codeword)* n]) ;
        }
    }

    // if we have not full codeword in file, insert the rest block of bytes in the end of output stream
    if (encodedStream.size() % sizeof(codeword) != 0) {
        int rest_bytes = 0;
        rest_bytes = encodedStream.size() - codeword_num * sizeof(codeword);
        vector <unsigned char> nfcodeword (encodedStream.end() - rest_bytes, encodedStream.end());
        for (int i = 0; i < (int)nfcodeword.size(); i++) {
            output.insert(output.end(), nfcodeword[i]);
        }
    }

    return output;
}

int main(int argc, char *argv[]) {
    initialize_ecc ();

    vector <unsigned char> file;            // data structure for input file
    int codeword_num = 0;                   // number of full codewords
    string filename;

    if(argc < 2 ){
        cout << "File name is required" << endl;
        return 1;
    }

    filename = argv[1];

    ifstream stream (filename, ios::in);

    for_each(istreambuf_iterator<char>(stream),
             istreambuf_iterator<char>(),
            [&file](const unsigned char c) {
                file.push_back(c);
    });

    stream.close();

    codeword_num = file.size() / message_len;
    output_file = interleave_data(encode(file), codeword_num);

    ofstream outFile((filename + ".out").c_str(), ios::out | ios::binary);
    copy(output_file.begin(), output_file.end(), ostreambuf_iterator<char>(outFile));

    return 0;
}
