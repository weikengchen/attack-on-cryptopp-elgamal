cd ./unpatched
make
sudo make install
cd ../
make clean
make
./cryptopp-keygen
./cryptopp-challenger
python attack_cryptopp.py 
cd ./patched
make
sudo make install
cd ../
make clean
make
./cryptopp-keygen
./cryptopp-challenger
python attack_cryptopp.py
