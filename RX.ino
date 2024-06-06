
#include <SPI.h>      //Βιβλιοθήκη SPI για επικοινωνία με το nRF24L01+
#include "RF24.h"     //Η κύρια βιβλιοθήκη του nRF24L01+

//Dορίστε τις ακίδες ενεργοποίησης των κινητήρων
const int enbA = 3;
const int enbB = 6;

// Καθορίστε τις ακίδες ελέγχου των κινητήρων
//Εάν οι κινητήρες περιστρέφονται προς την αντίθετη κατεύθυνση, μπορείτε να αλλάξετε τις θέσεις των παρακάτω αριθμών ακίδων
const int IN1 = 2;    //Δεξί μοτέρ (-)
const int IN2 = 4;    //Δεξί μοτέρ (+)
const int IN3 = 5;    //Αριστερό μοτέρ (+)
const int IN4 = 7;    //Αριστερό μοτέρ (-)

//Ορίστε μεταβλητή για τις στροφές των κινητήρων
//Έχω ορίσει μια μεταβλητή για κάθε έναν από τους δύο κινητήρες
//Με αυτόν τον τρόπο μπορείτε να συγχρονίσετε τη διαφορά ταχύτητας περιστροφής μεταξύ των δύο κινητήρων
int RightSpd = 200;
int LeftSpd = 200;

//Με αυτόν τον τρόπο μπορείτε να συγχρονίσετε την ταχύτητα περιστροφής Καθορίστε το πακέτο για την κατεύθυνση (άξονας X και άξονας Y)
int data[2];

//Ορισμός αντικειμένου από τη βιβλιοθήκη RF24 - 8 και 9 είναι αριθμοί ψηφιακής ακίδας στους οποίους συνδέονται τα σήματα CE και CSN
RF24 radio(8,9);

//Δημιουργήστε διευθύνσεις σωλήνα για την επικοινωνία
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(){
  //Ορίστε τους ακροδέκτες του κινητήρα ως OUTPUT
  pinMode(enbA, OUTPUT);
  pinMode(enbB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

   //digitalWrite(enbA, HIGH);
   //digitalWrite(enbB, HIGH);
  
  Serial.begin(9600);
  radio.begin();                    //Ξεκινήστε την επικοινωνία nRF24       
  radio.openReadingPipe(1, pipe);   //Ορίζει τη διεύθυνση του πομπού στον οποίο θα λαμβάνει δεδομένα το πρόγραμμα.
  radio.startListening();             
  }

void loop(){
  if (radio.available()){
    radio.read(data, sizeof(data));

    if(data[0] < 340){
      //προς τα εμπρός            
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      Serial.println("forward");
    }
    
    if(data[0] > 360){
      //προσ τα πίσω              
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      Serial.println("backward");
    }
     
    if(data[1] > 160){
       //δεξία
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      Serial.println("right");
    }

    if(data[1] < 140){
     //αριστερά 
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      Serial.println("left");
    }

    if(data[0] > 340 && data[0] < 360 && data[1] > 140 && data[1] < 160){
      //0 κίνηση
      analogWrite(enbA, 0);
      analogWrite(enbB, 0);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      Serial.println("stop");
    }
  }
}
