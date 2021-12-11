struct PacketRaspi {
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t w = 0;
  uint16_t h = 0;
  String data = "";
};
PacketRaspi readQrRaspi();

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
}

void loop() {
  if(Serial.available()){
    Serial.readStringUntil('\n');
    Serial.println("try reading");
    
    PacketRaspi p = readQrRaspi();
    Serial.println();
    Serial.println((String) "x:" + p.x + " y:" + p.y + " w:" + p.w + " h:" + p.h + " qrcode:" + p.data);
  }
}

PacketRaspi readQrRaspi()
{
  uint8_t pidx = 0;
  uint8_t pcnt = 0;
  String pstr = "";
  Serial2.write('R');
  bool rto = false;
  PacketRaspi p;
//  p.x = 0;
//  p.y = 0;
//  p.w = 0;
//  p.h = 0;
//  p.data = "";
  unsigned long timeStart = millis();
  while(!Serial2.available())
  {
    if ((unsigned long) millis()-timeStart > 1100)
    {
      Serial.println("RTO");
      rto = true;
      break;
    }
  }
  if (rto) return p;
  while (Serial2.available())
  {
    char c = (char) Serial2.read();
    Serial.write(c);
    if (pcnt == 0)
    {
      if (c != 'H')
      {
        pcnt = 0;
        Serial.println(F(" :data head salah"));
        continue; 
      }
      pstr = "";
    }
    else
    {
      if (c == ',' || c == 'T')
      {
        if (pidx == 0)
        {
          p.x = pstr.toInt();
        }
        else if (pidx == 1)
        {
          p.y = pstr.toInt();
        }
        else if (pidx == 2)
        {
          p.w = pstr.toInt();
        }
        else if (pidx == 3)
        {
          p.h = pstr.toInt();
        }
        else if (pidx == 4)
        {
          p.data = pstr;
          pcnt = 0;
          continue;
        }
        pidx++;
        pstr = "";
      }
      else
      {
        if (pidx < 4 && !isDigit(c))
        {
          p.x = 0;
          p.y = 0;
          p.w = 0;
          p.h = 0;
          p.data = "";
          pcnt = 0;
          rto = true;
          continue;
        }
        pstr += c;
      }
    }
    pcnt++;
    delay(1);
  }
  return p;
}
