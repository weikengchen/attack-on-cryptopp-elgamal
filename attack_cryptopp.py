from termcolor import colored
import sys
import random
import math

def isQR(x, p):
    q = (p - 1) / 2
    return pow(x, q, p)

p = int(open("./p").read(), 10)
g = int(open("./g").read(), 10)
pk = int(open("./pk").read(), 10)
ptsize = int(open("./ptsize").read(), 10)

question_txt = open("./question").read()
answer_txt = open("./answer").read()

questions = question_txt.split("\n")
answers = answer_txt.split()

wrong = 0
giveup = 0
go_guess = 0
go_guess_correct = 0
runs = 1000

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

    plaintext1 = plaintext1 << 8;
    plaintext2 = plaintext2 << 8;
    plaintext1 = plaintext1 + ptsize;
    plaintext2 = plaintext2 + ptsize;

    preference = 0
    plaintext1_QR = 0
    plaintext2_QR = 0

    base = pow(2, (ptsize + 1) * 8)
    for j in xrange(0, 255, 1):
        if isQR(plaintext1 + base * j, p) == 1:
            plaintext1_QR = plaintext1_QR + 1
        if isQR(plaintext2 + base * j, p) == 1:
            plaintext2_QR = plaintext2_QR + 1

    if plaintext1_QR > plaintext2_QR:
        preference = 1
    else:
        if plaintext1_QR < plaintext2_QR:
            preference = -1 

    if isQR(ciphertext1_2, p) == 1 and isQR(ciphertext2_2, p) == 1:
        sys.stdout.write(colored("0.500 ", "green"))
    else:
        if (isQR(ciphertext1_2, p) != 1) and (isQR(ciphertext2_2, p) != 1):
            sys.stdout.write(colored("0.500 ", "green"))
        else:
            bottom = plaintext1_QR / 256.0 * (1 - plaintext2_QR / 256.0) + plaintext2_QR / 256.0 * (1 - plaintext1_QR / 256.0)
            if preference == 1:
                buf = plaintext1_QR / 256.0 * (1 - plaintext2_QR / 256.0) / bottom
                buf = "%.3f " % buf
                sys.stdout.write(colored(buf, "red"))
            else:
                buf = plaintext2_QR / 256.0 * (1 - plaintext1_QR / 256.0) / bottom
                buf = "%.3f " % buf
                sys.stdout.write(colored(buf, "red"))

    if (i + 1) % 10 == 0:
        sys.stdout.write("\n")

    if preference == 0:
        giveup = giveup + 1
        guess = random.randint(0, 1)
        if guess != answer:
            wrong = wrong + 1
        if (i + 1) % 100 == 0:
            print "Total ", (i + 1), " (correct: ", (i + 1  - wrong), "), giving up by random toss: ", giveup, ", guessing: ", go_guess, " (correct: ", go_guess_correct, ")" 
        continue

    if isQR(ciphertext1_2, p) == 1 and isQR(ciphertext2_2, p) == 1:
        guess = random.randint(0, 1)
        if guess != answer:
            wrong = wrong + 1
        giveup = giveup + 1
    else:
        if (isQR(ciphertext1_2, p) != 1) and (isQR(ciphertext2_2, p) != 1):
            guess = random.randint(0, 1)
            if guess != answer:
                wrong = wrong + 1
            giveup = giveup + 1
        else:
            go_guess = go_guess + 1

            if isQR(ciphertext1_2, p) == 1:
                if preference == 1:
                    if answer != 1:
                        wrong = wrong + 1
                    else:
                        go_guess_correct = go_guess_correct + 1
                else:
                    if answer != 0:
                        wrong = wrong + 1
                    else:
                        go_guess_correct = go_guess_correct + 1
            else:
                if preference == 1:
                    if answer != 0:
                        wrong = wrong + 1
                    else:
                        go_guess_correct = go_guess_correct + 1
                else:
                    if answer != 1:
                        wrong = wrong + 1
                    else:
                        go_guess_correct = go_guess_correct + 1
    if (i + 1) % 100 == 0:
        print "Total ", (i + 1), " (correct: ", (i + 1  - wrong), "), giving up by random toss: ", giveup, ", guessing: ", go_guess, " (correct: ", go_guess_correct, ")"

