#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

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

int main(int argc, char* argv[])
{
	AutoSeededRandomPool rng;

	cout << "Generating private key. This may take some time..." << endl;

	ElGamal::Decryptor decryptor;
	decryptor.AccessKey().GenerateRandomWithKeySize(rng, 2048);
	const ElGamalKeys::PrivateKey& privateKey = decryptor.AccessKey();

	ElGamal::Encryptor encryptor(decryptor);
	const PublicKey& publicKey = encryptor.AccessKey();

	static const int SECRET_SIZE = 16;
	SecByteBlock plaintext( SECRET_SIZE );
	memset( plaintext, 'A', SECRET_SIZE );

	assert( 0 != encryptor.FixedMaxPlaintextLength() );
	assert( plaintext.size() <= encryptor.FixedMaxPlaintextLength() );

	size_t ecl = encryptor.CiphertextLength( plaintext.size() );
	assert( 0 != ecl );
	SecByteBlock ciphertext( ecl );

	encryptor.Encrypt( rng, plaintext, plaintext.size(), ciphertext );

	assert( 0 != decryptor.FixedCiphertextLength() );
	assert( ciphertext.size() <= decryptor.FixedCiphertextLength() );

	size_t dpl = decryptor.MaxPlaintextLength( ciphertext.size() );
	assert( 0 != dpl );
	SecByteBlock recovered( dpl );

	DecodingResult result = decryptor.Decrypt( rng,
	ciphertext, ciphertext.size(), recovered );

	assert( result.isValidCoding );
	assert( result.messageLength <=
	decryptor.MaxPlaintextLength( ciphertext.size() ) );

	recovered.resize( result.messageLength );

	assert( plaintext == recovered );

	if(plaintext == recovered)
	cout << "Recovered plain text" << endl;
	else
	cout << "Failed to recover plain text" << endl;

	return !(plaintext == recovered);
}

