#include <EspNow.h>
Peer baseDirection;
Peer ballPick;
Peer lagoriPick1;
Peer lagoriPick2;
Peer peerServo;
Peer crisis;
JSONVar switchCheck;
JSONVar lagoriData1;
JSONVar lagoriData2;
JSONVar baseData;
JSONVar ballData;
JSONVar servoData;
JSONVar autoCompute;
JSONVar emergencyStop;
char datachar;
String datastring;
const char *remoteStringData;
int basePwm = 80, posX = 100, negX = -100, posY = 100, negY = -100, posZ = 0, posR = 0;
int baseSingleData = 0;
int checkData = 0;
bool basex = false, basey = false, baser = false, digit = false, init_ = false;
bool pressed = false;
bool ball = false, boolServo = false, lagori1 = false, lagori2 = false, autoMode = false;
bool smartBase = false, switchmode = false, boolCircle = false, opt = false, optmode = false;
bool autoBase = false, autoDown = false, autoRight = false;
bool disaster = false;
uint8_t broadcastAddress[] = {0x30, 0xC6, 0xF7, 0x20, 0x32, 0x58};
uint8_t castAddress[] = {0x0C, 0xB8, 0x15, 0xC3, 0x46, 0x90};
String checkList[] = {"rel", "ps", "touch", "cross", "triangle", "square", "circle", "sq", "cr", "tri", "cir", "u", "d", "l", "i", "lpush", "bpush"};

void setup()
{
  Serial.begin(115200);
  Serial2.begin(921600, SERIAL_8N1, 16, 17);

  setId("TenZZ");
  baseDirection.init("WardL");
  //  setId("TenZZ");
  //  baseDirection.init("WardL");

  //  lagoriPick1.init("ShahZ");
  //  lagoriPick2.init("DapRZ");
  //  peerServo.init("SickZ");
  //  ballPick.init("ZombS");

  //  crisis.init("WeEeD");
  Serial2.setTimeout(1);
  baseDirection.setOnRecieve(defaultHandler);
  //  addData();
  autoAddData();
}

void loop()
{
  if (Serial2.available())
  {
    datachar = Serial2.read();
    if (datachar == '\n')
    {
      if (!digit)
      {
        if (!switchmode)
        {
          ballDataConvert(datastring);
        }
        checkData = int(switchCheck[datastring]);
        switch (checkData)
        {
          case 1:
            Serial.println("released");
            pressed = false;
            baseData["fx"] = posZ;
            baseData["fy"] = posZ;
            baseData["type"] = "drive";
            digit = true;
            break;

          case 2:
            //Serial.println("ps");
            Serial.println("2");
            //Serial.println(switchmode);
            switchmode = !switchmode ? true : false;
            //Serial.println("changed: " + String(switchmode));
            break;

          case 3:
            Serial.println("3");
            disaster = true;
            emergencyStop["type"] = "panic";
            break;

          case 4:
            //Serial.println("cross");
            Serial.println("4");
            lagoriData1["type"] = "cross";
            lagori1 = true;
            break;

          case 5:
            //            Serial.println("triangle");
            Serial.println("5");
            lagoriData1["type"] = "triangle";
            lagori1 = true;
            break;

          case 6:
            Serial.println("6");
            if (switchmode)
            {
              //              Serial.println("square");
              lagoriData2["type"] = "square";
              lagori2 = true;
            }
            break;

          case 7:
            Serial.println("7");
            //            Serial.println("circle");
            lagoriData2["type"] = "circle";
            lagori2 = true;
            break;

          case 8:
            //Serial.println("ballSquare");
            Serial.println("8");
            break;

          case 9:
            //Serial.println("ballCross");
            Serial.println("9");
            ballData["type"] = "down";
            ball = true;
            break;

          case 10:
            //            Serial.println("ballTriangle");
            Serial.println("10");
            ballData["type"] = "up";
            ball = true;
            break;

          case 11:
            //            Serial.println("ballCircle");
            Serial.println("11");
            ballData["type"] = !boolCircle ? "close" : "open";
            boolCircle = !boolCircle ? true : false;
            ball = true;
            break;

          case 12:
            //            Serial.println("up");
            //            baseData["fy"] = posY;
            //            baseData["fx"] = posZ;
            Serial.println("12");
            baseData["fr"] = posR;
            baseData["fx"] = int(-sqrt(3) * 0.5 * basePwm);
            baseData["fy"] =  0.5 * basePwm;
            baseData["type"] = "drive";
            pressed = true;
            digit = true;
            break;


          case 13:
            //Serial.println("down");
            Serial.println("13");
            autoDown = true;
            autoRight = false;
            //            baseData["fy"] = negY;
            //            baseData["fx"] = posZ;
            baseData["fr"] = posR;
            baseData["fx"] = int(sqrt(3) * 0.5 * basePwm);
            baseData["fy"] =  -0.5 * basePwm;
            baseData["type"] = "drive";
            pressed = true;
            digit = true;
            break;

          case 14:
            Serial.println("14");
            //            Serial.println("left");
            //            baseData["fx"] = negX;
            //            baseData["fy"] = posZ;
            baseData["fr"] = posR;
            baseData["fx"] = 1 * 0.5 * basePwm;
            baseData["fy"] = 1 * int(sqrt(3) * 0.5 * basePwm);
            baseData["type"] = "drive";
            pressed = true;
            digit = true;
            break;

          case 15:
            //            Serial.println("right");
            Serial.println("15");
            autoRight = true;
            autoDown = false;
            //            baseData["fx"] = posX;
            //            baseData["fy"] = posZ;
            baseData["fr"] = posR;
            baseData["fx"] = -1 * 0.5 * basePwm;
            baseData["fy"] = -1 * int(sqrt(3) * 0.5 * basePwm);
            baseData["type"] = "drive";
            pressed = true;
            digit = true;
            break;

          case 16:
            Serial.println("16");
            if (switchmode)
            {
              lagoriData1["type"] = "push";
              lagori1 = true;
              //Serial.println("lpush");
            }
            break;

          case 17:
            Serial.println("17");
            if (switchmode)
            {
              lagoriData1["type"] = "bPush";
              lagori1 = true;
              //Serial.println("bpush");
            }
            break;
        }
      }

      if (digit && !smartBase && !autoBase)
      {
        baseData["type"] = "drive";
        baseDirection.send(baseData);
//        Serial.println("BaseDirection !!SmatBase is : " + JSON.stringify(baseData));
      }

      if (disaster)
      {
        crisis.send(emergencyStop);
        Serial.println("Pickle");
      }

      if (lagori1)
      {
        //        Serial.println(JSON.stringify(lagoriData1));
        lagoriPick1.send(lagoriData1);
      }

      if (lagori2)
      {
        //        Serial.println(JSON.stringify(lagoriData2));
        lagoriPick2.send(lagoriData2);
      }

      if (ball)
      {
        //        Serial.println(JSON.stringify(ballData));
        ballPick.send(ballData);
      }

      basex = false;
      basey = false;
      baser = false;
      ball = false;
      lagori2 = false;
      lagori1 = false;
      digit = false;
      init_ = false;
      smartBase = false;
      autoBase = false;
      disaster = false;
      datastring = "";
    }
    else
    {
      if ((isDigit(datachar) == true || datachar == ',' || datachar == '-') && init_ == false)
      {
        //Serial.println(datachar);
        digit = true;
        if (datachar == ',')
        {
          baseSingleData = datastring.toInt();
          if (!basex)
          {
            if (!pressed)
            {
              baseData["fx"] = baseSingleData;
            }
            //Serial.println("fx: "+String(baseData["fx"]));
            basex = true;
            datastring = "";
          }

          else if (!basey)
          {
            if (!pressed)
            {
              baseData["fy"] = baseSingleData;
            }
            baseData["type"] = "values";
            basey = true;
            datastring = "";
          }
          else if (!baser)
          {
            baseData["fr"] = baseSingleData / 4;
            posR = baseData["fr"];
            //Serial.println("posR: " + String(posR));
            baseData["type"] = "base";
            baser = true;
            digit = true; //redundant
            datastring = "";
          }
        }
        else
        {
          datastring += datachar;
        }
      }
      else
      {
        if (!digit)
        {
          init_ = true;
          Serial.println(datachar);
          datastring += datachar;
        }
      }
    }
  }
}

void ballDataConvert(String ballDataString)
{
  datastring = datastring == "cross" ? "cr" : (datastring == "triangle" ? "tri" : (datastring == "circle" ? "cir" : datastring));
}

void autoAddData()
{
  for (int i = 0; i < sizeof(checkList) / sizeof(checkList[0]); i++)
  {
    switchCheck[checkList[i]] = i + 1;
  }
}

void defaultHandler(JSONVar msg)
{
  Serial.println("Default: " + JSON.stringify(msg));
}

void addData()
{
  //  switchCheck["cross"] = remoteValue::cross;
  switchCheck["rel"] = 1;
  switchCheck["ps"] = 2;
  switchCheck["touch"] = 3;
  switchCheck["cross"] = 4;
  switchCheck["triangle"] = 5;
  switchCheck["square"] = 6;
  switchCheck["circle"] = 7;
  switchCheck["sq"] = 8;
  switchCheck["cr"] = 9;
  switchCheck["tri"] = 10;
  switchCheck["cir"] = 11;
  switchCheck["u"] = 12;
  switchCheck["d"] = 13;
  switchCheck["l"] = 14;
  switchCheck["i"] = 15;
  switchCheck["lpush"] = 16;
  switchCheck["bpush"] = 17;
}
