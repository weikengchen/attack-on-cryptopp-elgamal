#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::ofstream;
using std::ios;

#include <cryptopp/osrng.h>
using CryptoPP::AutoSeededRandomPool;

#include <cryptopp/secblock.h>
using CryptoPP::SecByteBlock;

#include <cryptopp/integer.h>
using CryptoPP::Integer;

#include <cryptopp/elgamal.h>
using CryptoPP::ElGamal;
using CryptoPP::ElGamalKeys;
using CryptoPP::PublicKey;

#include <cryptopp/filters.h>
using CryptoPP::StringSource;
using CryptoPP::StringSink;

#include <cryptopp/files.h>
using CryptoPP::FileSource;
using CryptoPP::FileSink;

int main(int argc, char* argv[])
{
	AutoSeededRandomPool prng;

	cout << "Generating private key. This may take some time..." << endl;

	ElGamalKeys::PrivateKey privateKey;
	privateKey.GenerateRandomWithKeySize(prng, 64);
	privateKey.Save(FileSink("elgamal.der", true /*binary*/).Ref());

	ElGamal::Decryptor decryptor(privateKey);
	ElGamal::Encryptor encryptor(decryptor);
	ElGamalKeys::PublicKey& publicKey = encryptor.AccessKey();

	int ciphertext_size = encryptor.FixedCiphertextLength();
	int plaintext_size = encryptor.FixedMaxPlaintextLength();

	ofstream pt_size_file;
	pt_size_file.open("ptsize", ios::out);
	pt_size_file << plaintext_size;
	pt_size_file.close();

	ofstream pk_file;
	pk_file.open("pk", ios::out);
	pk_file << IntToString(publicKey.GetPublicElement());
	pk_file.close();

	ofstream p_file;
	p_file.open("p", ios::out);
	p_file << IntToString(encryptor.GetGroupParameters().GetModulus());
	p_file.close();

	ofstream g_file;
	g_file.open("g", ios::out);
	g_file << IntToString(encryptor.GetGroupParameters().GetGenerator());
	g_file.close();

	return 0;
}

