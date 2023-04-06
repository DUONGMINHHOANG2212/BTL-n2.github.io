#include <iostream>
#include <cstdlib>
using namespace std;
int generateRandomNumber();
int getPlayerGuess();
void printAnswer(int guess, int secretNumber);
int main()
{
int secretNumber = generateRandomNumber();
int guess;
do {
guess = getPlayerGuess();
printAnswer(guess, secretNumber);
} while (guess != secretNumber);
return 0;
}
