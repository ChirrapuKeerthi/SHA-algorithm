#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdint>
#include <bitset>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;



//initial Constants and variables
 uint32_t h_initial[] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
 uint32_t k_constants[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

//utility functions
const uint32_t MOD = 0xFFFFFFFF;
uint32_t modularAddition(uint32_t a, uint32_t b) {
    return (a + b) % MOD;
}

uint32_t rotr(uint32_t n, unsigned int d)
{
    return (n >> d)|(n << (32 - d));
}
//converting input message to binary values
string stringToBinary(string& input) {
    string binaryString = "";
    for (char c : input) {
        // Convert each character to its 8-bit binary representation
        bitset<8> charBits(c);
        
        // Append the binary representation to the result string
        binaryString += charBits.to_string();
    }
    return binaryString;
}


//padding funciton
string sha256Padding(const string& message) {
    uint64_t originalLength = message.size();
    string paddedMessage = message;

    // Append a single '1' bit
    paddedMessage += '1';

    // Append zeros to make the total length congruent to 448 (mod 512)
    uint32_t K = (448 - (originalLength + 1)%512 )%512;
    for (int i = 0; i < K; ++i) {
        paddedMessage += '0';
    }

    // Append the original length as a 64-bit big-endian integer
    bitset<64> lengthBits(originalLength);
    string lengthBinary = (lengthBits.to_string());
    paddedMessage += lengthBinary;
    //cout<<endl<<lengthBinary<<endl<<endl;

    return paddedMessage;
}


string hash_Value(string msg){
    uint32_t a,b,c,d,e,f,g,h;
    string block;

    for (int i = 0; i < msg.size(); i += 512) {

        //512 bit block
        block=(msg.substr(i, 512));

        //message schedule array
        uint32_t w[64];
        int j=0;
        for(int i=0;i<16;i++){
            std::bitset<32> bits(block.substr(j, 32));
            w[i] = bits.to_ulong();
            j += 32;
        }

        //w[i] i=16..63        
        for(int i=16;i<64;i++){
            uint32_t s0 = (rotr(w[i-15],7)) ^ (rotr(w[i - 15], 18)) ^ (w[i - 15] >> 3);
            uint32_t s1 = (rotr(w[i - 2],17)) ^ (rotr(w[i - 2],19)) ^ (w[i - 2] >> 10);
            w[i] = modularAddition(w[i - 16], modularAddition(s0, modularAddition(w[i - 7], s1)));
        }

        //initialization of variables
        a=h_initial[0];
        b=h_initial[1];
        c=h_initial[2];
        d=h_initial[3];
        e=h_initial[4];
        f=h_initial[5];
        g=h_initial[6];
        h=h_initial[7];

        
        //compression function main loop
        for (int i = 0; i < 64; ++i) {
            uint32_t S1 = (rotr(e, 6)) ^ (rotr(e,11)) ^ (rotr(e,25));
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = modularAddition(h, modularAddition(S1, modularAddition(ch, modularAddition(k_constants[i], w[i]))));
            uint32_t S0 = (rotr(a,2)) ^ (rotr(a,13)) ^ (rotr(a,22));
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = modularAddition(S0, maj);

            //initialization variables for the next round
            h = g;
            g = f;
            f = e;
            e = modularAddition(d, temp1);
            d = c;
            c = b;
            b = a;
            a = modularAddition(temp1, temp2);
        }

        //initialization variables for the next block
        h_initial[0] = modularAddition(h_initial[0], a);
        h_initial[1] = modularAddition(h_initial[1], b);
        h_initial[2] = modularAddition(h_initial[2], c);
        h_initial[3] = modularAddition(h_initial[3], d);
        h_initial[4] = modularAddition(h_initial[4], e);
        h_initial[5] = modularAddition(h_initial[5], f);
        h_initial[6] = modularAddition(h_initial[6], g);
        h_initial[7] = modularAddition(h_initial[7], h);
    }

    //finally after processing all the blocks for all rounds
    //digest is created
    stringstream digest;
    for(int i=0;i<8;i++){
        digest<<hex<<setfill('0')<<setw(8)<<(h_initial[i]);
    }
    return digest.str();
}

int main() {
    
        
    //reading input 
    stringstream buffer;
    ifstream file("bible.txt");
    buffer<<file.rdbuf();
    file.close();
    string input=buffer.str();

   //convert string to binary format
    string binaryMessage = stringToBinary(input);

    // Apply padding
    string padded_msg=sha256Padding(binaryMessage);

    //create the hash value for the input msg
    cout<<"Hash value for the given input is "<<endl<<hash_Value(padded_msg);
    return 0;   
}

//Hash value for the given input is 
//8d202adf39c3e88510a591cbe1cbd96708e3237140060c0768f2577f9d81d3ad