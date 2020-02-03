#include <SD.h>                // need to include the SD library
#define SD_ChipSelectPin 10     //using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>


TMRpcm tmrpcm;   // create an object for use in this sketch
File root;

void setup(){

  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
   Serial.println("initialized");  


  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc

  root = SD.open("/");      // open SD card main root
  printDirectory(root, 0);  // print all files names and sizes

  tmrpcm.quality(1);
  tmrpcm.setVolume(7);
}



void loop(){  

  tmrpcm.play("BASS.WAV");
  delay(1000);
}



void printDirectory(File dir, int numTabs) {
  while (true) {
 
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
