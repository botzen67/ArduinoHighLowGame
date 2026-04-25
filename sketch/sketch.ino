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
int generateRandomHighLowAnswer(int answerMin, int answerMax, bool allowMiddleAnswer) {
    int answerMid = (answerMax - answerMin) / 2;

    int answer;
    do {
        answer = random(answerMin, answerMax);
    } while (!allowMiddleAnswer && answer == answerMid);
    return answer;
}

char* generateRandomMastermindAnswer(int answerDigits) {
    char* answer = new char[answerDigits + 1];
    for (int i = 0; i < answerDigits; i++) {
        answer[i] = '0' + random(0, 10);
    }
    return answer;
}


// track and handle LED light
class LEDBoard {
    public:
        bool redOn, yellowOn, blueOn, greenOn;
        bool prevRedOn, prevYellowOn, prevBlueOn, prevGreenOn;

        bool redBlink, yellowBlink, blueBlink, greenBlink;
        bool prevRedBlink, prevYellowBlink, prevBlueBlink, prevGreenBlink;

        unsigned long lastBlinkToggle = 0;

        bool blinkPhase = false;

        LEDBoard(bool redOn, bool yellowOn, bool blueOn, bool greenOn) {
            this->redOn    = redOn;
            this->yellowOn = yellowOn;
            this->blueOn   = blueOn;
            this->greenOn  = greenOn;

            this->prevRedOn    = redOn;
            this->prevYellowOn = yellowOn;
            this->prevBlueOn   = blueOn;
            this->prevGreenOn  = greenOn;

            this->redBlink    = false;
            this->yellowBlink = false;
            this->blueBlink   = false;
            this->greenBlink  = false;

            this->prevRedBlink    = false;
            this->prevYellowBlink = false;
            this->prevBlueBlink   = false;
            this->prevGreenBlink  = false;
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

            this->prevRedBlink    = redBlink;
            this->prevYellowBlink = yellowBlink;
            this->prevBlueBlink   = blueBlink;
            this->prevGreenBlink  = greenBlink;
        }

        void loadPrevState() {
            this->redOn    = prevRedOn;
            this->yellowOn = prevYellowOn;
            this->blueOn   = prevBlueOn;
            this->greenOn  = prevGreenOn;

            this->redBlink    = prevRedBlink;
            this->yellowBlink = prevYellowBlink;
            this->blueBlink   = prevBlueBlink;
            this->greenBlink  = prevGreenBlink;
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
            int msDelay = 300;

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

        void blinkNumbers(char* numbers, int digits, int msDelay = 300) {
            // Save the LED State and turn them all off
            this->saveState();
            this->setAll(false);
            this->updateLEDs();

            delay(msDelay);
            for (int i = 0; i < digits; i++) {
                int targetDigit = numbers[i] - '0';
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

        void setLEDByIndex(int idx, int state) {
            switch (idx) {
                case 0: redOn    = state; break;
                case 1: yellowOn = state; break;
                case 2: blueOn   = state; break;
                case 3: greenOn  = state; break;
            }
        }

        void setBlinkByIndex(int idx, bool shouldBlink) {
            switch (idx) {
                case 0: redBlink    = shouldBlink; break;
                case 1: yellowBlink = shouldBlink; break;
                case 2: blueBlink   = shouldBlink; break;
                case 3: greenBlink  = shouldBlink; break;
            }
        }

        void clearBlinks() {
            redBlink = yellowBlink = blueBlink = greenBlink = false;
        }

        void updateBlinking(unsigned long intervalMS = 300) {
            unsigned long now = millis();
            if (now - lastBlinkToggle >= intervalMS) {
                lastBlinkToggle = now;
                blinkPhase = !blinkPhase;

                digitalWrite(redLEDPin,    redBlink    ? (blinkPhase ? ON : OFF) : (redOn    ? ON : OFF));
                digitalWrite(yellowLEDPin, yellowBlink ? (blinkPhase ? ON : OFF) : (yellowOn ? ON : OFF));
                digitalWrite(blueLEDPin,   blueBlink   ? (blinkPhase ? ON : OFF) : (blueOn   ? ON : OFF));
                digitalWrite(greenLEDPin,  greenBlink  ? (blinkPhase ? ON : OFF) : (greenOn  ? ON : OFF));
            }
        }
};

class KeypadGame {
    public:
        LEDBoard* board = nullptr;

        bool won;

        char* inputBuffer;
        int inputBufferSize;          // max size of input buffer excluding terminating null byte
        int inputBufferIdx;           // current index in buffer for next input character to go
        int max_guesses;              // max guesses each game, inclusive
        int current_guesses;          // how many guesses have been made
        int additional_guesses;       // added to max_guesses

        KeypadGame(int inputBufferSize, int max_guesses) {
            this->max_guesses = max_guesses;

            // initialize board
            this->board = new LEDBoard(false, false, false, false);
            this->board->updateLEDs();

            this->inputBufferSize = inputBufferSize;
            this->inputBuffer = new char[inputBufferSize + 1];        // +1 for terminating null byte
        }

        virtual ~KeypadGame() {
            delete[] inputBuffer;
        }

        virtual bool isGameOver() {
            return this->won || this->isOutOfGuesses();
        }

        virtual bool isOutOfGuesses() {
            return this->current_guesses >= this->max_guesses + this->additional_guesses;
        }

        virtual void handleInput(char key) = 0;

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

        void guessInvalid() {
            this->board->blinkRed(1000);
        }

        void outOfGuesses() {
            this->board->redOn = true;
            this->board->updateLEDs();
            Serial.println("Game Over! You lost!");
            Serial.print("Guesses Made: ");
            Serial.println(this->current_guesses);
        }

};


// delays for cycle when (re)starting game
// how long in MS each light should be on in order RED, YELLOW, BLUE, GREEN
const int startLEDDelays[4] = { 350, 350, 350, 950 };

class HighLowGame: public KeypadGame {
    private:
        int answerMin;          // inclusive
        int answerMax;          // inclusive
        bool allowMiddleAnswer; // whether random answer can be (answerMax - answerMin) / 2

        int answer;             // random integer in range [answerMin, answerMax] (*see allowMiddleAnswer)

        static int calculateInputBufferSize(int answerMax) {
            return floor(log10(abs(answerMax))) + 1;
        }


    public:
        HighLowGame(int answerMin, int answerMax, int max_guesses, bool allowMiddleAnswer = false) : KeypadGame(calculateInputBufferSize(answerMax), max_guesses) {
            // setup game constants
            this->answerMin = answerMin;
            this->answerMax = answerMax + 1; // random function uses exclusive max
            this->allowMiddleAnswer = allowMiddleAnswer;

            // setup game variables
            this->start();
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
            this->answer = generateRandomHighLowAnswer(this->answerMin, this->answerMax, this->allowMiddleAnswer);
            Serial.println(this->answer);
            this->current_guesses = 0;    // init guess counter
            this->additional_guesses = 0; // start game with 0 additional guesses

            this->board->cycle(startLEDDelays);
            Serial.println("(re)starting done...");
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
                        Serial.print("Answer: ");
                        Serial.println(this->answer);
                        Serial.println();
                    }
                } else {
                    this->guessInvalid();
                }

                this->clearInputBuffer();
            }
        }

        void handleInput(char key) {
            if (key) {
                if (this->isGameOver()) {
                    // if out of guesses or guessed correctly
                    switch(key) {
                        case '*':
                            Serial.println();
                            Serial.println();

                            this->start();

                            Serial.print("Guesses Made: ");
                            Serial.println(this->current_guesses);
                            Serial.print("Current Buffer: ");
                            break;
                        case 'C':
                            this->blinkGuesses(550);
                            break;
                        case 'B':
                            if (!this->won) {
                                this->board->redOn = false;
                                this->board->updateLEDs();

                                this->oneMoreGuess();
                                Serial.print("Guesses Made: ");
                                Serial.println(this->current_guesses);
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

                            this->start();

                            Serial.print("Guesses Made: ");
                            Serial.println(this->current_guesses);
                            Serial.print("Current Buffer: ");
                            break;
                        case 'D':
                            this->blinkSecretNumber();
                            break;
                        case 'C':
                            this->blinkGuesses(550);
                            break;
                        case 'B':
                            this->oneMoreGuess();
                            break;
                        case 'A':
                            Serial.println();

                            this->clearInputBuffer();

                            Serial.print("Guesses Made: ");
                            Serial.println(this->current_guesses);
                            Serial.print("Current Buffer: ");
                            break;
                        case '#':
                            Serial.println();

                            this->submitGuess();

                            if (!this->isGameOver()) {
                                Serial.print("Guesses Made: ");
                                Serial.println(this->current_guesses);
                                Serial.print("Current Buffer: ");
                            }
                            break;
                        default:
                            this->board->setAll(false);
                            this->board->updateLEDs();

                            this->addToInputBuffer(key);
                            break;
                    }
                }
            }
        }   
};


class MastermindGame: public KeypadGame {
    private:
        char* answer;            // random integer in range [answerMin, answerMax] (*see allowMiddleAnswer)

    public:
        MastermindGame(int answerDigits, int max_guesses) : KeypadGame(answerDigits, max_guesses) {
            // setup game variables
            this->start();
        }

        void start() {
            this->board->setAll(false);
            this->board->updateLEDs();

            Serial.println("(re)starting...");
            this->won = false;            // ...
            this->clearInputBuffer();     // init values and 0 index
                                          // generate and set answer to rand value
            this->answer = generateRandomMastermindAnswer(this->inputBufferSize);
            Serial.println(this->answer);
            this->current_guesses = 0;    // init guess counter
            this->additional_guesses = 0; // start game with 0 additional guesses

            this->board->cycle(startLEDDelays);
            Serial.println("(re)starting done...");
        }

        void guessCorrect() {
            this->won = true;

            Serial.println("Correct!");
            Serial.println();
            Serial.println("Game Over! You won!");
            Serial.print("Guesses Made: ");
            Serial.println(this->current_guesses);
            Serial.print("Answer: ");
            Serial.println(this->answer);
        }

        void processInputValue(char value, int idx) {
            if (this->answer[idx] == value) {
                this->board->setLEDByIndex(idx, ON);
                return;
            }

            int valueAnswerCount = 0;   // how many of value character are in answer
            int valueGuessedCount = 0;  // how many of value character have been processed already
            for (int i = 0; i < this->inputBufferSize; i++) {
                char checkValueAnswer = this->answer[i];
                char checkValueInput = this->inputBuffer[i];
                if (value == checkValueAnswer) {
                    valueAnswerCount++;
                }
                if (value == checkValueInput && i < idx) {
                    valueGuessedCount++;
                }
            }

            if (valueAnswerCount > valueGuessedCount) {
                this->board->setLEDByIndex(idx, ON);
                this->board->setBlinkByIndex(idx, true);
            } else {
                this->board->setLEDByIndex(idx, OFF);
                this->board->setBlinkByIndex(idx, false);
            }
        }

        void blinkAnswer() {
            this->board->blinkNumbers(this->answer, this->inputBufferSize);
        }

        void submitGuess() {
            if (!this->isGameOver()) {
                char* guess = this->inputBuffer;
                char* answer = this->answer;

                if (strlen(guess) < this->inputBufferSize) {
                    this->guessInvalid();
                } else {
                    this->board->clearBlinks();

                    this->current_guesses += 1;

                    bool allCorrect = true;
                    for (int i = 0; i < this->inputBufferSize; i++) {
                        this->processInputValue(this->inputBuffer[i], i);
                        if (this->inputBuffer[i] != this->answer[i]) {
                            allCorrect = false;
                        }
                    }

                    if (allCorrect) {
                        this->guessCorrect();
                    }

                    if (this->isOutOfGuesses() && !this->won) {
                        this->outOfGuesses();
                        Serial.print("Answer: ");
                        Serial.println(this->answer);
                        Serial.println();
                    }
                }
                this->board->updateLEDs();
                this->clearInputBuffer();
            }
        }

        void handleInput(char key) { 
            if (key) {
                if (this->isGameOver()) {
                    // if out of guesses or guessed correctly
                    switch(key) {
                        case '*':
                            Serial.println();
                            Serial.println();

                            this->start();

                            Serial.print("Guesses Made: ");
                            Serial.println(this->current_guesses);
                            Serial.print("Current Buffer: ");
                            break;
                        case 'C':
                            this->blinkGuesses(550);
                            break;
                        case 'B':
                            if (!this->won) {
                                this->board->redOn = false;
                                this->board->updateLEDs();

                                this->oneMoreGuess();
                                Serial.print("Guesses Made: ");
                                Serial.println(this->current_guesses);
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

                            this->start();

                            Serial.print("Guesses Made: ");
                            Serial.println(this->current_guesses);
                            Serial.print("Current Buffer: ");
                            break;
                        case 'D':
                            this->blinkAnswer();
                            break;
                        case 'C':
                            this->blinkGuesses(550);
                            break;
                        case 'B':
                            this->oneMoreGuess();
                            break;
                        case 'A':
                            Serial.println();

                            this->clearInputBuffer();

                            Serial.print("Guesses Made: ");
                            Serial.println(this->current_guesses);
                            Serial.print("Current Buffer: ");
                            break;
                        case '#':
                            Serial.println();

                            this->submitGuess();

                            if (!this->isGameOver()) {
                                Serial.print("Guesses Made: ");
                                Serial.println(this->current_guesses);
                                Serial.print("Current Buffer: ");
                            }
                            break;
                        default:
                            this->addToInputBuffer(key);
                            break;
                    }
                }
            }
        }
};


// game instance
bool setupDone = false;
KeypadGame* game = nullptr;

void setupAndPlayHighLow() {    
    // initialize Game Parameter Values
    int answerMin          =     1; // inclusive
    int answerMax          =   100; // inclusive
    int max_guesses        =     8; // max guesses each game, inclusive
    bool allowMiddleAnswer = false; // whether random answer can be (answerMax - answerMin) / 2

    // create game instance
    game = new HighLowGame(answerMin, answerMax, max_guesses, allowMiddleAnswer);

    // debugging output
    Serial.println("Setup done...\n");

    Serial.print("Guesses Made: ");
    Serial.println(game->current_guesses);
    Serial.print("Current Buffer: ");
}

void setupAndPlayMastermind() {    
    // initialize Game Parameter Values
    int answerDigits       =     4; // inclusive
    int max_guesses        =     8; // max guesses each game, inclusive

    // create game instance
    game = new MastermindGame(answerDigits, max_guesses);

    // debugging output
    Serial.println("Setup done...\n");

    Serial.print("Guesses Made: ");
    Serial.println(game->current_guesses);
    Serial.print("Current Buffer: ");
}

// special function. runs on start.
void setup() {
    // initialize Serial Connection
    Serial.begin(9600);
    randomSeed(analogRead(0));

    Serial.println("");
    Serial.println("starting setup...");

    // setup LED Pins as output
    pinMode(redLEDPin,    OUTPUT);
    pinMode(yellowLEDPin, OUTPUT);
    pinMode(blueLEDPin,   OUTPUT);
    pinMode(greenLEDPin,  OUTPUT);


    Serial.println("Waiting for game selection...");
    Serial.println("    [A] High-Low");
    Serial.println("    [B] Mastermind");
    Serial.println("");

    digitalWrite(greenLEDPin, ON);

    while (game == nullptr) {
        char key = keypad.getKey();
        if (key) {
            switch (key) {
                case 'A':
                    Serial.println("Starting High-Low Game...");
                    setupAndPlayHighLow();
                    break;
                case 'B':
                    Serial.println("Starting Mastermind Game...");
                    setupAndPlayMastermind();
                    break;
            }
        }
    }
    setupDone = true;
}


// main loop
void loop() {
    if (!setupDone) {
        return;
    }

    char key = keypad.getKey();
    game->handleInput(key);

    game->board->updateBlinking();
}
