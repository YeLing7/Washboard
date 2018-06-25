int red = D0; 
int led0 = D7;
int green = D1;
int blue = D2;

const int tagLen = 16;
const int idLen = 13;
const int kTags = 3;

// Put your known tags here!
char knownTags[kTags][idLen] = {
             "82003BE6C996",    //Colors
             "82003CBA2226",    //White
             "82003CE74019"     //Delicate
};

// Empty array to hold a freshly scanned tag
char newTag[idLen];

char lala[idLen];


void setup() {
    pinMode(red, OUTPUT);
    pinMode(blue, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(led0,OUTPUT);
   // Particle.variable("Tag",tag);
      // Starts the hardware and software serial ports
   //Serial.begin(9600);
   Serial1.begin(9600);
    // LED
   pinMode(13, OUTPUT);
   digitalWrite(13, LOW);
   Particle.variable("lala",lala);

}

void loop() {
    
    digitalWrite(led0, HIGH);
    delay(1000);
    digitalWrite(led0, LOW);
    delay(1000);
    
    
    // Counter for the newTag array
  int i = 0;
  // Variable to hold each byte read from the serial buffer
  int readByte;
  // Flag so we know when a tag is over
  boolean tag = false;

  // This makes sure the whole tag is in the serial buffer before
  // reading, the Arduino can read faster than the ID module can deliver!
  if (Serial1.available() == tagLen) {
    tag = true;
  }

  if (tag == true) {
    while (Serial1.available()) {
      // Take each byte out of the serial buffer, one at a time
      readByte = Serial1.read();

      /* This will skip the first byte (2, STX, start of text) and the last three,
      ASCII 13, CR/carriage return, ASCII 10, LF/linefeed, and ASCII 3, ETX/end of 
      text, leaving only the unique part of the tag string. It puts the byte into
      the first space in the array, then steps ahead one spot */
      if (readByte != 2 && readByte!= 13 && readByte != 10 && readByte != 3) {
        newTag[i] = readByte;
        i++;
      }

      // If we see ASCII 3, ETX, the tag is over
      if (readByte == 3) {
        tag = false;
      }

    }
  }


  // don't do anything if the newTag array is full of zeroes
  if (strlen(newTag)== 0) {
    return;
  }

  else {
    int total = 0;

    for (int ct=0; ct < kTags; ct++){
        total += checkTag(newTag, knownTags[ct]);
    }

    // If newTag matched any of the tags
    // we checked against, total will be 1
    if (total > 0) {

      digitalWrite(red, HIGH);
      delay(1000);
      digitalWrite(red, LOW);
      delay(1000);
      Particle.publish(newTag);
      Particle.publish("Success");

      
        if (!strcmp(newTag,knownTags[0]))
        {
          Particle.publish("Success! 0");
          strcpy(lala, " 82003BE6C996");
        }
        else if (!strcmp(newTag,knownTags[1]))
        {
          Particle.publish("Success!1"); 
          strcpy(lala,"82003CBA2226");   //White
             
        }
        else if(!strcmp(newTag,knownTags[2]))
        {
          Particle.publish("Success!2");
          strcpy(lala,"82003CE74019");
        }
        
     
    }

    else {
        // This prints out unknown cards so you can add them to your knownTags as needed
        Particle.publish("Unknown tag! ");
        Particle.publish(newTag);
    }
  }

  // Once newTag has been checked, fill it with zeroes
  // to get ready for the next tag read
  for (int c=0; c < idLen; c++) {
    newTag[c] = 0;
  }

}



int checkTag(char nTag[], char oTag[]) 
{
    for (int i = 0; i < idLen; i++) {
      if (nTag[i] != oTag[i]) {
        return 0;
      }
    }
  return 1;
}
