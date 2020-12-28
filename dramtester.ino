/*
4164 DRAM Tester
*/

#define RAS               4
#define CAS               5
#define D                 6 // chip data IN
#define Q                 8 // chip data OUT
#define WRITE             9
#define RED_STATUS_LED    7 // use 220ohm resistor
#define GREEN_STATUS_LED  12 // use 220ohm resistor

#define RANDOM_1      0
#define RANDOM_2      1
#define ALL_HIGH      2
#define ALL_LOW       3
#define ALTERNATING_1 4
#define ALTERNATING_2 5
#define NUM_PATTERNS  6

int patterns[2][256] = {
  {1,0,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,0,0,1,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,0,0,1,0,1,1,1,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,0,1,0,0,1,1,1,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,1,0,1,0,0,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,1,0,0,1,1,1,1,0,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,0,1,1,1,1,1,1,0,1,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,1,0,1,0,0,1,0,1,1,1,0,0,1,0,1,0,1,1,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,0,1,1,0,0,0,0,1,0,1,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,1,1,0,1,1,0,0,1,1,0,1,1,0,1,1,1,0,0,1,0,0,0,1,1,0,0,1,1,1,1,0,1,1,0,0,0,1,1,1,1,1,0,0,1,0,1,0,1,1,0,1,0,1,1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,0,1,0,0,1,1,0,0,0,0,1,0,0,1,0,1,1,0,1,1,0,0,0,1,1,1,0,1,1,0,0,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,0,1,1,1,1,1,0,0,1,0,1,1,1,1,1,1,0,0,0,1,1,0,1,0,1,1,1,0,0,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,1,1,1,0,0,0,0,1},
};

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  
  pinMode(A0, OUTPUT);  
  pinMode(A1, OUTPUT);  
  pinMode(A2, OUTPUT);  
  pinMode(A3, OUTPUT);  
  pinMode(A4, OUTPUT);  
  pinMode(A5, OUTPUT);  
  pinMode(10, OUTPUT);  
  pinMode(11, OUTPUT);  

  pinMode(D, OUTPUT); 
  pinMode(Q, INPUT); 

  pinMode(RAS, OUTPUT); 
  pinMode(CAS, OUTPUT); 
  pinMode(WRITE, OUTPUT); 

  pinMode(RED_STATUS_LED, OUTPUT);
  pinMode(GREEN_STATUS_LED, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(RED_STATUS_LED, LOW);
  digitalWrite(GREEN_STATUS_LED, LOW);

  Serial.println("Starting test...");

  for (int t = 0; t < 3; t++) {
    digitalWrite(GREEN_STATUS_LED, HIGH);
    delay(200);
    digitalWrite(GREEN_STATUS_LED, LOW);      
    delay(200);
  }

  // Do a write/read but ignore return.
  writeBits(ALL_LOW, 0);
  readBits(ALL_LOW, 0);

  for(int pattern = 0 ; pattern < NUM_PATTERNS; pattern++) {
    for(int row=0; row<=255; row++) {
      Serial.println("Testing pattern " + String(pattern) + " row: " + String(row));

      writeBits(pattern, row);
      int col = readBits(pattern, row);
    
      if (col>=0) {
        digitalWrite(RED_STATUS_LED, HIGH);
        Serial.println("ERROR: row " + String(row) + " col " + String(col) + " did not match pattern.");
        while(1);
      }

      digitalWrite(RED_STATUS_LED, row % 2 ? HIGH : LOW);
      digitalWrite(GREEN_STATUS_LED, row % 2 ? LOW : HIGH);
    }
    
    digitalWrite(RED_STATUS_LED, LOW);
    digitalWrite(GREEN_STATUS_LED, LOW);

    delay(1000);

    // Flash green with pattern num for success
    for (int t = 0; t < pattern+1; t++) {
      digitalWrite(GREEN_STATUS_LED, HIGH);
      delay(200);
      digitalWrite(GREEN_STATUS_LED, LOW);
      delay(200);
    }
    delay(1000);
  }
  
  Serial.println("Test DONE. All OK!");

  while(1) {
    digitalWrite(GREEN_STATUS_LED, HIGH);
    delay(500);
    digitalWrite(GREEN_STATUS_LED, LOW);      
    delay(500);
  }
}

void writeBits(int pattern, int row) {

  // Pull RAS and CAS HIGH
  digitalWrite(RAS, HIGH);
  digitalWrite(CAS, HIGH);

  // Loop though all the columns
  for (int col=0; col<=255; col++) {

    // Set row address
    digitalWrite(A0, bitRead(row, 0));
    digitalWrite(A1, bitRead(row, 1));
    digitalWrite(A2, bitRead(row, 2));
    digitalWrite(A3, bitRead(row, 3));
    digitalWrite(A4, bitRead(row, 4));
    digitalWrite(A5, bitRead(row, 5));
    digitalWrite(10, bitRead(row, 6));
    digitalWrite(11, bitRead(row, 7));

    // Pull RAS LOW
    digitalWrite(RAS, LOW);

    // Pull Write LOW (Enables write)
    digitalWrite(WRITE, LOW);   

    // Set Data
    switch (pattern) {
    case RANDOM_1:
    case RANDOM_2:
       digitalWrite(D, patterns[pattern][col] ? HIGH : LOW);
       break;
    case ALL_HIGH:
       digitalWrite(D, HIGH);
       break;
    case ALL_LOW:
       digitalWrite(D, LOW);
       break;
    case ALTERNATING_1:
       digitalWrite(D, col % 2 ? HIGH : LOW);
       break;
    case ALTERNATING_2:
       digitalWrite(D, col % 2 ? LOW : HIGH);
       break;
    }

    // Set column address
    digitalWrite(A0, bitRead(col, 0));
    digitalWrite(A1, bitRead(col, 1));
    digitalWrite(A2, bitRead(col, 2));
    digitalWrite(A3, bitRead(col, 3));
    digitalWrite(A4, bitRead(col, 4));
    digitalWrite(A5, bitRead(col, 5));
    digitalWrite(10, bitRead(col, 6));
    digitalWrite(11, bitRead(col, 7));

    // Pull CAS LOW
    digitalWrite(CAS, LOW);

    digitalWrite(RAS, HIGH);
    digitalWrite(CAS, HIGH);
  }
}

int readBits(int pattern, int row) {

  // Bit counter
  int numberOfBits = 0;

  // Pull RAS, CAS and Write HIGH
  digitalWrite(RAS, HIGH);
  digitalWrite(CAS, HIGH);
  digitalWrite(WRITE, HIGH);

  // Loop though all the columns
  for (int col=0; col<=255; col++) {

    // Set row address
    digitalWrite(A0, bitRead(row, 0));
    digitalWrite(A1, bitRead(row, 1));
    digitalWrite(A2, bitRead(row, 2));
    digitalWrite(A3, bitRead(row, 3));
    digitalWrite(A4, bitRead(row, 4));
    digitalWrite(A5, bitRead(row, 5));
    digitalWrite(10, bitRead(row, 6));
    digitalWrite(11, bitRead(row, 7));

    // Pull RAS LOW
    digitalWrite(RAS, LOW);

    // Set column address
    digitalWrite(A0, bitRead(col, 0));
    digitalWrite(A1, bitRead(col, 1));
    digitalWrite(A2, bitRead(col, 2));
    digitalWrite(A3, bitRead(col, 3));
    digitalWrite(A4, bitRead(col, 4));
    digitalWrite(A5, bitRead(col, 5));
    digitalWrite(10, bitRead(col, 6));
    digitalWrite(11, bitRead(col, 7));

    // Pull CAS LOW
    digitalWrite(CAS, LOW);

    // Read the stored bit and compare

    int expect;
    switch (pattern) {
    case RANDOM_1:
    case RANDOM_2:
       expect = patterns[pattern][col] ? HIGH : LOW;
       break;
    case ALL_HIGH:
       expect = HIGH;
       break;
    case ALL_LOW:
       expect = LOW;
       break;
    case ALTERNATING_1:
       expect = col % 2 ? HIGH : LOW;
       break;
    case ALTERNATING_2:
       expect = col % 2 ? LOW : HIGH;
       break;
    }
    
    if (digitalRead(Q) != expect) {
       return col;
    }

    // Pull RAS and CAS HIGH
    digitalWrite(RAS, HIGH);
    digitalWrite(CAS, HIGH);
  }

  return -1; // OKAY!
}
