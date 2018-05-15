cd ./unpatched
{
	make
	sudo make install
} &> /dev/null
cd ../
{
	make clean
	make
	./cryptopp-keygen
	./cryptopp-challenger
} &> /dev/null
python attack_cryptopp.py 
cd ./patched
{
	make
	sudo make install
} &> /dev/null
cd ../
{
	make clean
	make
	./cryptopp-keygen
	./cryptopp-challenger
} &> /dev/null
python attack_cryptopp.py
