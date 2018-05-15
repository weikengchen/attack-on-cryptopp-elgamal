from subprocess import call
import random
import math

# Check if a value is quadratic residue (QR) for safe primes
def isQR(x, p):
    q = (p - 1) / 2
    return pow(x, q, p)

# Key generation. We use 512-bit only for better performance
#call("make")
#call("./cryptopp-keygen")
#call("./cryptopp-challenger")

p = int(open("./p").read(), 10)
g = int(open("./g").read(), 10)
pk = int(open("./pk").read(), 10)
ptsize = int(open("./ptsize").read(), 10)

question_txt = open("./question").read()
answer_txt = open("./answer").read()

questions = question_txt.split("\n")
answers = answer_txt.split()

wrong = 0
runs = 10000

print "Running the experiment..."

for i in xrange(runs):
    question = questions.pop(0)
    answer = int(answers.pop(0), 10)

    elems_in_question = question.split()

    plaintext1 = int(elems_in_question[0], 10)
    plaintext2 = int(elems_in_question[1], 10)
    ciphertext1_1 = int(elems_in_question[2], 10)
    ciphertext1_2 = int(elems_in_question[3], 10)
    ciphertext2_1 = int(elems_in_question[4], 10)
    ciphertext2_2 = int(elems_in_question[5], 10)

    plaintext1_QR = 0
    plaintext2_QR = 0

    base = pow(2, ptsize * 8)
    for j in xrange(0, 255, 1):
        if isQR(plaintext1 + base * j, p) == 1:
            plaintext1_QR = plaintext1_QR + 1
        if isQR(plaintext2 + base * j, p) == 1:
            plaintext2_QR = plaintext2_QR + 1

    preference = 0;
    if plaintext1_QR > plaintext2_QR:
        preference = 1
    else:
        if plaintext1_QR < plaintext2_QR:
            preference = -1

    if preference == 0:
        continue

    if isQR(ciphertext1_1, p) == 1 and isQR(ciphertext2_1, p) == 1:
        guess = random.randint(0, 1)
        if guess != answer:
            wrong = wrong + 1
    else:
        if isQR(ciphertext1_1, p) != 1 and isQR(ciphertext2_1, p) != 1:
            guess = random.randint(0, 1)
            if guess != answer:
                wrong = wrong + 1
        else:
            if isQR(ciphertext1_1, p) == 1:
                if preference == 1:
                    if answer != 1:
                        wrong = wrong + 1
                else:
                    if answer != 0:
                        wrong = wrong + 1
            else:
                if preference == 1:
                    if answer != 0:
                        wrong = wrong + 1
                else:
                    if answer != 1:
                        wrong = wrong + 1

print "Number of times the guess was wrong (should be 50% if ElGamal is secure):", wrong, "/", runs

