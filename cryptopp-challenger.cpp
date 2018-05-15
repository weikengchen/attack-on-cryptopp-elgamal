#include <iostream>
#include <fstream>
using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::ifstream;
using std::ofstream;

#include <cryptopp/osrng.h>
using CryptoPP::AutoSeededRandomPool;

#include <cryptopp/secblock.h>
using CryptoPP::SecByteBlock;

#include <cryptopp/elgamal.h>
using CryptoPP::ElGamal;
using CryptoPP::ElGamalKeys;

#include <cryptopp/cryptlib.h>
using CryptoPP::DecodingResult;
using CryptoPP::PublicKey;
using CryptoPP::PrivateKey;

#include <cryptopp/files.h>
using CryptoPP::FileSource;
using CryptoPP::FileSink;

#include <cryptopp/pubkey.h>
using CryptoPP::DL_GroupParameters;
using CryptoPP::DL_SimpleKeyAgreementDomainBase;

#include <cryptopp/integer.h>
using CryptoPP::Integer;

int main(int argc, char* argv[])
{
	AutoSeededRandomPool rng;	

	ElGamalKeys::PrivateKey privateKey;
	privateKey.Load(FileSource("elgamal.der", true /*pump*/).Ref());
	privateKey.Validate(rng, 3);

	ElGamal::Decryptor decryptor(privateKey);
	ElGamal::Encryptor encryptor(decryptor);
	const PublicKey& publicKey = encryptor.AccessKey();

	ofstream file_question;
	ofstream file_answer;

        file_question.open("question", ios::out);
	file_answer.open("answer", ios::out);

	int ptsize;
	ifstream file_ptsize;
        file_ptsize.open("ptsize", ios::in);
       	file_ptsize >> ptsize;
        file_ptsize.close();

	for(int i = 0; i < 10000; i++){
		SecByteBlock plaintext1(ptsize);
		SecByteBlock plaintext2(ptsize);

		rng.GenerateBlock(plaintext1, ptsize);
		rng.GenerateBlock(plaintext2, ptsize);

		size_t ecl_1 = encryptor.CiphertextLength(plaintext1.size());
		assert(0 != ecl_1);
		SecByteBlock ciphertext1(ecl_1);

		size_t ecl_2 = encryptor.CiphertextLength(plaintext2.size());
		assert(0 != ecl_2);
		SecByteBlock ciphertext2(ecl_2);

		encryptor.Encrypt(rng, plaintext1, plaintext1.size(), ciphertext1);
		encryptor.Encrypt(rng, plaintext2, plaintext2.size(), ciphertext2);

		SecByteBlock ciphertext1_d(ecl_1 / 2);
		SecByteBlock ciphertext2_d(ecl_2 / 2);
		SecByteBlock ciphertext1_c(ecl_1 / 2);
                SecByteBlock ciphertext2_c(ecl_2 / 2);

		memcpy(ciphertext1_c, ciphertext1, ecl_1 / 2);
		memcpy(ciphertext2_c, ciphertext2, ecl_2 / 2);
		memcpy(ciphertext1_d, &ciphertext1[ecl_1 / 2], ecl_1 / 2);
		memcpy(ciphertext2_d, &ciphertext2[ecl_2 / 2], ecl_2 / 2);

		unsigned char rand;
		rng.GenerateBlock(&rand, 1);

		file_question << IntToString(Integer(plaintext1, ptsize));
		file_question << " ";
		file_question << IntToString(Integer(plaintext2, ptsize));
                file_question << " ";

		if(rand & 1){ 
			file_question << IntToString(Integer(ciphertext1_c, ecl_1 / 2));
			file_question << " ";
			file_question << IntToString(Integer(ciphertext1_d, ecl_1 / 2));
			file_question << " ";
			file_question << IntToString(Integer(ciphertext2_c, ecl_2 / 2));
			file_question << " ";
			file_question << IntToString(Integer(ciphertext2_d, ecl_2 / 2));
			file_question << "\n";
		}else{
			file_question << IntToString(Integer(ciphertext2_c, ecl_2 / 2));
                        file_question << " ";
			file_question << IntToString(Integer(ciphertext2_d, ecl_2 / 2));
                        file_question << " ";
			file_question << IntToString(Integer(ciphertext1_c, ecl_1 / 2));
                        file_question << " ";
			file_question << IntToString(Integer(ciphertext1_d, ecl_1 / 2));
			file_question << "\n";
		}

		file_answer << (rand & 1);
		file_answer << " ";
	}

	file_question.close();
	file_answer.close();

	return 0;
}

