#include <Keypad.h>

// LED Pin variables
const int redLEDPin    = 5;  // out of guesses
const int yellowLEDPin = 4;  // too low
const int blueLEDPin   = 3;  // too high
const int greenLEDPin  = 2;  // correct

// LED Power Constants
// If LED Anode(+) connected to board:
//     Off is LOW impedance
//     On  is HIGH impedance
// Otherwise:
//     Off is HIGH impedance
//     On  is LOW impedance
const int OFF = LOW;
const int ON  = HIGH;


// Keypad Setup
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 13, 12, 11, 10}; //row pins
byte colPins[COLS] = {  9,  8,  7,  6}; //column pins
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


// generate a random number between answerMin and answerMax that may or may not be the middle (based on `allowMiddleAnswer`)
int generateRandomAnswer(int answerMin, int answerMax, bool allowMiddleAnswer) {
    int answerMid = (answerMax - answerMin) / 2;

    int answer;
    do {
        answer = random(answerMin, answerMax);
    } while (!allowMiddleAnswer && answer == answerMid);
    return answer;
}


// track and handle LED light
class LEDBoard {
    public:
        bool redOn;    // out of guesses, game over
        bool yellowOn; // guess is too low  (lower than answer)
        bool blueOn;   // guess is too high (higher than answer)
        bool greenOn;  // guess is correct  (equal to the answer)

        bool prevRedOn;
        bool prevYellowOn;
        bool prevBlueOn;
        bool prevGreenOn;

        LEDBoard(bool redOn, bool yellowOn, bool blueOn, bool greenOn) {
            this->redOn    = redOn;
            this->yellowOn = yellowOn;
            this->blueOn   = blueOn;
            this->greenOn  = greenOn;

            this->prevRedOn    = redOn;
            this->prevYellowOn = yellowOn;
            this->prevBlueOn   = blueOn;
            this->prevGreenOn  = greenOn;
        }

        void updateLEDs() {
            digitalWrite(redLEDPin,    redOn    ? ON : OFF);
            digitalWrite(yellowLEDPin, yellowOn ? ON : OFF);
            digitalWrite(blueLEDPin,   blueOn   ? ON : OFF);
            digitalWrite(greenLEDPin,  greenOn  ? ON : OFF);
        }

        void saveState() {
            this->prevRedOn    = redOn;
            this->prevYellowOn = yellowOn;
            this->prevBlueOn   = blueOn;
            this->prevGreenOn  = greenOn;
        }

        void loadPrevState() {
            this->redOn    = prevRedOn;
            this->yellowOn = prevYellowOn;
            this->blueOn   = prevBlueOn;
            this->greenOn  = prevGreenOn;
        }

        void powerRedSolo() {
            this->redOn    = true;
            this->yellowOn = false;
            this->blueOn   = false;
            this->greenOn  = false;
        }
        
        void powerYellowSolo() {
            this->redOn    = false;
            this->yellowOn = true;
            this->blueOn   = false;
            this->greenOn  = false;
        }
        
        void powerBlueSolo() {
            this->redOn    = false;
            this->yellowOn = false;
            this->blueOn   = true;
            this->greenOn  = false;
        }
        
        void powerGreenSolo() {
            this->redOn    = false;
            this->yellowOn = false;
            this->blueOn   = false;
            this->greenOn  = true;
        }
        
        void setAll(bool newValue) {
            this->redOn    = newValue;
            this->yellowOn = newValue;
            this->blueOn   = newValue;
            this->greenOn  = newValue;
        }

        void blinkRed(int onDurationMS, int offDurationMS = 250) {
            this->saveState();

            this->setAll(false);
            this->updateLEDs();
            delay(offDurationMS);

            this->redOn = true;
            this->updateLEDs();            
            delay(onDurationMS);

            this->redOn = false;
            this->updateLEDs();
            delay(offDurationMS);

            this->loadPrevState();
            this->updateLEDs();
        }

        void blinkYellow(int onDurationMS, int offDurationMS = 250) {
            this->saveState();

            this->setAll(false);
            this->updateLEDs();
            delay(offDurationMS);

            this->yellowOn = true;
            this->updateLEDs();            
            delay(onDurationMS);

            this->yellowOn = false;
            this->updateLEDs();
            delay(offDurationMS);

            this->loadPrevState();
            this->updateLEDs();
        }

        void blinkBlue(int onDurationMS, int offDurationMS = 250) {
            this->saveState();

            this->setAll(false);
            this->updateLEDs();
            delay(offDurationMS);

            this->blueOn = true;
            this->updateLEDs();            
            delay(onDurationMS);

            this->blueOn = false;
            this->updateLEDs();
            delay(offDurationMS);

            this->loadPrevState();
            this->updateLEDs();
        }

        void blinkGreen(int onDurationMS, int offDurationMS = 250) {
            this->saveState();

            this->setAll(false);
            this->updateLEDs();
            delay(offDurationMS);

            this->greenOn = true;
            this->updateLEDs();            
            delay(onDurationMS);

            this->greenOn = false;
            this->updateLEDs();
            delay(offDurationMS);

            this->loadPrevState();
            this->updateLEDs();
        }

        void blinkAll(int numBlinks, int onDurationMS, int offDurationMS = 250) {
            this->saveState();

            this->setAll(false);
            this->updateLEDs();
            delay(offDurationMS);

            for (int i = 0; i < numBlinks; i++) {
              this->setAll(true);
              this->updateLEDs();
              delay(onDurationMS);

              this->setAll(false);
              this->updateLEDs();
              delay(offDurationMS);
            }

            this->loadPrevState();
            this->updateLEDs();
        }

        void blinkNumber(int number) {
            int msDelay = 250;

            // Get each digit
            int numberDigits = floor(log10(abs(number))) + 1; // number of base 10 digits
            int* digits = new int[numberDigits]; // array for each digit
            int idx = numberDigits - 1; // start at end

            // fill digits array
            do {
                digits[idx] = number % 10;
                number /= 10;
                idx--;
            } while (idx >= 0);

            // Save the LED State and turn them all off
            this->saveState();
            this->setAll(false);
            this->updateLEDs();

            if (numberDigits == 1) 
            {
              this->blinkAll(1, msDelay);
            }
            
            // Do lights
            delay(msDelay);
            for (int i = 0; i < numberDigits; i++) {
                int targetDigit = digits[i];
                this->redOn = targetDigit == 0 || targetDigit == 1  || targetDigit == 3 || targetDigit == 5 || targetDigit == 7 || targetDigit == 9;
                this->yellowOn = targetDigit == 0 || targetDigit == 2  || targetDigit == 3 || targetDigit == 6 || targetDigit == 7;
                this->blueOn = targetDigit == 0 || targetDigit == 4  || targetDigit == 5 || targetDigit == 6 || targetDigit == 7;
                this->greenOn = targetDigit == 0 || targetDigit == 8  || targetDigit == 9;
                this->updateLEDs();
                delay(msDelay);
                this->setAll(false);
                this->updateLEDs();
                delay(msDelay);
            }
            // Restore the previous LED state
            this->loadPrevState();
            this->updateLEDs();
        }

        void cycle(int delaysMS[]) {
            this->saveState();

            setAll(false);
            this->updateLEDs();

            this->powerRedSolo();
            this->updateLEDs();
            delay(delaysMS[0]);

            this->powerYellowSolo();
            this->updateLEDs();
            delay(delaysMS[1]);

            this->powerBlueSolo();
            this->updateLEDs();
            delay(delaysMS[2]);

            this->powerGreenSolo();
            this->updateLEDs();
            delay(delaysMS[3]);

            this->loadPrevState();
            this->updateLEDs();
        }
};


// delays for cycle when (re)starting game
// how long in MS each light should be on in order RED, YELLOW, BLUE, GREEN
const int startLEDDelays[4] = {350, 350, 350, 950};

class HighLowGame {
    private:
        int answerMin;          // inclusive
        int answerMax;          // inclusive
        int max_guesses;        // max guesses each game, inclusive
        bool allowMiddleAnswer; // whether random answer can be (answerMax - answerMin) / 2

        int answer;                   // random integer in range [answerMin, answerMax] (*see allowMiddleAnswer)


    public:
        LEDBoard* board = nullptr;    // track power status of LED lights

        bool won;

        char* inputBuffer;            // buffer that tracks user input
        int inputBufferSize;          // max size of input buffer excluding terminating null byte
        int inputBufferIdx;           // current index in buffer for next input character to go
        int current_guesses;          // how many guesses have been made
        int additional_guesses;       // added to max_guesses

        HighLowGame(int answerMin, int answerMax, int max_guesses, bool allowMiddleAnswer = false) {
            // setup game constants
            this->answerMin = answerMin;
            this->answerMax = answerMax + 1; // random function uses exclusive max
            this->max_guesses = max_guesses;
            this->allowMiddleAnswer = allowMiddleAnswer;

            // initialize board
            this->board = new LEDBoard(false, false, false, false);
            this->board->updateLEDs();

            // setup input buffer
            this->inputBufferSize = floor(log10(abs(answerMax))) + 1; // get length of integer in base 10
            this->inputBuffer = new char[inputBufferSize + 1];        // +1 for terminating null byte

            // setup game variables
            this->start();
        }

        ~HighLowGame() {
            delete[] inputBuffer;
        }

        bool isOutOfGuesses() {
            return this->current_guesses >= this->max_guesses + this->additional_guesses;
        }

        bool isGameOver() {
            return this->won || this->isOutOfGuesses();
        }

        void addToInputBuffer(char newChar) {
            if (this->inputBufferIdx < this->inputBufferSize) {
                this->inputBuffer[this->inputBufferIdx] = newChar;
                Serial.print(newChar);

                this->inputBufferIdx++;
            }
        }

        void clearInputBuffer() {
            for (int i = 0; i < this->inputBufferSize; i++) {
              this->inputBuffer[i] = 0;
            }
            this->inputBufferIdx = 0;
        }

        void oneMoreGuess() {
            this->additional_guesses += 1;
        }

        void blinkGuesses(int onDurationMS, int offDurationMS = 250) {
            this->board->blinkAll(this->current_guesses, onDurationMS, offDurationMS);
        }

        void blinkSecretNumber() {
            this->board->blinkNumber(this->answer);
        }

        void start() {
            this->board->setAll(false);
            this->board->updateLEDs();

            Serial.println("(re)starting...");
            this->won = false;            // ...
            this->clearInputBuffer();     // init values and 0 index
                                          // generate and set answer to rand value
            this->answer = generateRandomAnswer(this->answerMin, this->answerMax, this->allowMiddleAnswer);
            Serial.println(this->answer);
            this->current_guesses = 0;    // init guess counter
            this->additional_guesses = 0; // start game with 0 additional guesses

            this->board->cycle(startLEDDelays);
            Serial.println("(re)starting done...");
        }

        void outOfGuesses() {
            this->board->redOn = true;
            this->board->updateLEDs();
            Serial.println("Game Over! You lost!");
            Serial.print("Guesses Made: ");
            Serial.println(this->current_guesses);
            Serial.print("Answer: ");
            Serial.println(this->answer);
            Serial.println();
        }

        void guessInvalid() {
            this->board->blinkRed(1000);
        }

        void guessLow() {
            this->board->powerYellowSolo();
            this->board->updateLEDs();
            Serial.println("Too Low!");
            Serial.println();
        }

        void guessHigh() {
            this->board->powerBlueSolo();
            this->board->updateLEDs();
            Serial.println("Too High!");
            Serial.println();
        }

        void guessCorrect() {
            this->won = true;

            this->board->powerGreenSolo();
            this->board->updateLEDs();
            Serial.println("Correct!");
            Serial.println();
            Serial.println("Game Over! You won!");
            Serial.print("Guesses Made: ");
            Serial.println(this->current_guesses);
            Serial.print("Answer: ");
            Serial.println(this->answer);
        }

        void submitGuess() {
            if (!this->isGameOver()) {
                int guess = atoi(this->inputBuffer);

                if (guess >= this->answerMin && guess <= this->answerMax) {
                    this->current_guesses += 1;
                    if (guess == this->answer) {
                        this->guessCorrect();
                    } else if (guess < this->answer) {
                        this->guessLow();
                    } else if (guess > this->answer) {
                        this->guessHigh();
                    }

                    if (this->isOutOfGuesses() && !this->won) {
                        this->outOfGuesses();
                    }
                } else {
                    this->guessInvalid();
                }

                this->clearInputBuffer();
            }
        }
};


// game instance
HighLowGame* game = nullptr;


// special function. runs on start.
void setup() {
    // initialize Serial Connection
    Serial.begin(9600);
    randomSeed(analogRead(0));
    Serial.println("starting setup...");

    // setup LED Pins as output
    pinMode(redLEDPin,    OUTPUT);
    pinMode(yellowLEDPin, OUTPUT);
    pinMode(blueLEDPin,   OUTPUT);
    pinMode(greenLEDPin,  OUTPUT);
    
    // initialize Game Parameter Values
    int answerMin          =     1; // inclusive
    int answerMax          =   100; // inclusive
    int max_guesses        =     8; // max guesses each game, inclusive
    bool allowMiddleAnswer = false; // whether random answer can be (answerMax - answerMin) / 2

    // create game instance
    game = new HighLowGame(answerMin, answerMax, max_guesses, allowMiddleAnswer);

    // debugging output
    Serial.println("setup done...\n");

    Serial.print("Guesses Made: ");
    Serial.println(game->current_guesses);
    Serial.print("Current Buffer: ");
}


// main loop
void loop() {
    char key = keypad.getKey();
    if (key) {
        if (game->isGameOver()) {
            // if out of guesses or guessed correctly
            switch(key) {
                case '*':
                    Serial.println();
                    Serial.println();

                    game->start();

                    Serial.print("Guesses Made: ");
                    Serial.println(game->current_guesses);
                    Serial.print("Current Buffer: ");
                    break;
                case 'C':
                    game->blinkGuesses(550);
                    break;
                case 'B':
                    if (!game->won) {
                        game->board->redOn = false;
                        game->board->updateLEDs();

                        game->oneMoreGuess();
                        Serial.print("Guesses Made: ");
                        Serial.println(game->current_guesses);
                        Serial.print("Current Buffer: ");
                    }
                    break;
                default:
                    break;
            }
        } else {
            // if game is in progress
            // "*" key restarts the game
            // "C" key blinks number of guesses made in current game including additional
            // "B" key gives another guess even if game is over
            // "A" key clears current input buffer
            // "#" key submits current input buffer as guess
            // Every other key is added to buffer and sets lights to off
            // "D" blinks answer in base 2
            switch(key) {
                case '*':
                    Serial.println();
                    Serial.println();

                    game->start();

                    Serial.print("Guesses Made: ");
                    Serial.println(game->current_guesses);
                    Serial.print("Current Buffer: ");
                    break;
                case 'D':
                    game->blinkSecretNumber();
                    break;
                case 'C':
                    game->blinkGuesses(550);
                    break;
                case 'B':
                    game->oneMoreGuess();
                    break;
                case 'A':
                    Serial.println();

                    game->clearInputBuffer();

                    Serial.print("Guesses Made: ");
                    Serial.println(game->current_guesses);
                    Serial.print("Current Buffer: ");
                    break;
                case '#':
                    Serial.println();

                    game->submitGuess();

                    if (!game->isGameOver()) {
                        Serial.print("Guesses Made: ");
                        Serial.println(game->current_guesses);
                        Serial.print("Current Buffer: ");
                    }
                    break;
                default:
                    game->board->setAll(false);
                    game->board->updateLEDs();

                    game->addToInputBuffer(key);
                    break;
            }
        }
    }
}
