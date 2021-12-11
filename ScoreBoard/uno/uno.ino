//#include <SoftwareSerial.h>
#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial_Black_16.h>
#include <fonts/SystemFont5x7.h>

SoftDMD dmd(5,1);
DMD_TextBox team1Box(dmd, 0, 0, 32, 16);
DMD_TextBox team2Box(dmd, 33, 0, 32, 16);
DMD_TextBox infoBox(dmd, 65, 0, 32, 16);
DMD_TextBox point1Box(dmd, 97, 0, 32, 16);
DMD_TextBox point2Box(dmd, 129, 0, 32, 16);

String team1 = "";
String team2 = "";
uint8_t point1 = 0;
uint8_t point2 = 0;
uint8_t score1 = 0;
uint8_t score2 = 0;
uint8_t ronde = 1;
uint8_t lastPoint1 = 0, lastPoint2 = 0, lastScore1 = 0, lastScore2 = 0, lastRonde = 0;

enum Perintah {
  team1name,
  team2name,
  point1up,
  point2up,
  team1win,
  team2win,
  resetgame,
  undoaction
} perintah;

void setup() {
  Serial.begin(9600);
  perintah = undoaction;

  dmd.setBrightness(255);
  dmd.begin();

  Serial.flush();
}

void loop() {
  if(Serial.available() > 0){
    String dataIn = Serial.readStringUntil('\n');
//    Serial.print(F("Data In Over Serial: "));
//    Serial.println(dataIn);

    Serial.println("OK");

    if(dataIn.indexOf("team1:") >= 0){
      team1 = dataIn.substring(dataIn.indexOf(':') + 1);
//      Serial.print(F("Got team 1 name: "));
//      Serial.println(team1);
      perintah = team1name;
    } else 
    if(dataIn.indexOf("team2:") >= 0){
      team2 = dataIn.substring(dataIn.indexOf(':') + 1);
//      Serial.print(F("Got team 2 name: "));
//      Serial.println(team2);
      perintah = team2name;
    } else 
    if(dataIn.indexOf("point1up") >= 0){
//      Serial.println(F("Point 1 up"));
      if(ronde >= 3 || abs(score1-score2) >= 2) return;
      point1++;
      perintah = point1up;
    } else 
    if(dataIn.indexOf("point2up") >= 0){
//      Serial.println(F("Point 2 up"));
      if(ronde >= 3 || abs(score1-score2) >= 2) return;
      point2++;
      perintah = point2up;
    } else 
    if(dataIn.indexOf("team1win") >= 0){
//      Serial.println(F("Team 1 win"));
      if(ronde >= 3 || abs(score1-score2) >= 2) return;
      lastPoint1 = point1;
      lastPoint2 = point2;
      score1++;
      ronde++;
      point1 = 0;
      point2 = 0;
      perintah = team1win;

      for(uint8_t i=0; i<8; i++){
        team1Box.clear();
        delay(500);
        team1Box.println(team1); 
        delay(500);
      }
    } else
    if(dataIn.indexOf("team2win") >= 0){
//      Serial.println(F("Team 2 win"));
      if(ronde >= 3 || abs(score1-score2) >= 2) return;
      lastPoint1 = point1;
      lastPoint2 = point2;
      score2++;
      ronde++;
      point1 = 0;
      point2 = 0;
      perintah = team2win;
      
      for(uint8_t i=0; i<8; i++){
        team2Box.clear();
        delay(500);
        team2Box.println(team2); 
        delay(500);
      }
    } else
    if(dataIn.indexOf("resetgame") >= 0){
//      Serial.println(F("Resetting all game"));
      lastPoint1 = point1;
      lastPoint2 = point2;
      lastScore1 = score1;
      lastScore2 = score2;
      lastRonde = ronde;
      point1 = 0;
      point2 = 0;
      score1 = 0;
      score2 = 0;
      ronde = 1;
      perintah = resetgame;
    } else
    if(dataIn.indexOf("undoaction") >= 0){
      switch(perintah){
        case team1name: team1 = ""; break;
        case team2name: team2 = ""; break;
        case point1up: point1--; break;
        case point2up: point2--; break;
        case team1win: score1--; ronde--; point1 = lastPoint1; point2 = lastPoint2; break;
        case team2win: score2--; ronde--; point1 = lastPoint1; point2 = lastPoint2; break;
        case resetgame: point1 = lastPoint1; point2 = lastPoint2; score1 = lastScore1; score2 = lastScore2; ronde = lastRonde; break;
//        default:
//        Serial.println(F("Do Nothing"));
      }
      perintah = undoaction;
    }

//    Serial.println(F("****************************************************************************"));
//    Serial.print(team1); Serial.print('\t'); 
//    Serial.print(point1); Serial.print('\t'); 
//    Serial.print(score1); Serial.print('\t'); 
//    Serial.print(ronde); Serial.print('\t'); 
//    Serial.print(score2); Serial.print('\t'); 
//    Serial.print(point2); Serial.print('\t'); 
//    Serial.print(team2); Serial.print('\t'); 
//    Serial.println();
//    Serial.println(F("****************************************************************************"));

    updateDisplay();
  }
  delay(200);
}

void updateDisplay(){
  dmd.selectFont(SystemFont5x7);
  team1Box.clear();
  team2Box.clear();
  infoBox.clear();
  point1Box.clear();
  point2Box.clear();
  
  if(abs(score1 - score2) >= 2) ronde = score1 + score2;
  team1Box.println(team1);
  team2Box.println(team2);
  dmd.drawString((32 - dmd.stringWidth(String(ronde))) / 2 + 64, 0, String(ronde));
  dmd.drawString((16 - dmd.stringWidth(String(score1))) / 2 + 64, 8, String(score1));
  dmd.drawString((16 - dmd.stringWidth(String(score2))) / 2 + 80, 8, String(score2));
  dmd.selectFont(Arial_Black_16);
  dmd.drawString((32 - dmd.stringWidth(String(point1))) / 2 + 96, 0, String(point1));
  dmd.drawString((32 - dmd.stringWidth(String(point2))) / 2 + 128, 0, String(point2));
//  point1Box.println(String(point1));
//  point2Box.println(String(point2));
}
