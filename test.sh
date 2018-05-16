cd ./unpatched
sudo make install
cd ../
make clean
make
./cryptopp-keygen
./cryptopp-challenger
python attack_cryptopp.py 
cd ./patched
sudo make install
cd ../
make clean
make
./cryptopp-keygen
./cryptopp-challenger
python attack_cryptopp.py
