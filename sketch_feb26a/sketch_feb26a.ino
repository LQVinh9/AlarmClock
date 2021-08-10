// Ban quyen code thuoc ve team: Vinh, Huynh, Long Nhan
// Big Project - IOT - Thay Loi 

#include<LiquidCrystal.h>   // Thu vien LCD
#include <TimeLib.h>        // Thu vien thoi gian
#include <DS1307RTC.h>      // Thu vien DS1307
//#include "RTClib.h"       // Thu vien Cai thoi gian ban dau

//RTC_DS1307 rtc;        // Khai bao DS1307

bool BaoThuc=false;      // Khong co Bao thuc

int switchPin=7;         // Chan Button
bool checkSwitch=false;  // Kiem tra xem Button co duoc nhan khong

int value;               // Gia tri Chiet Ap
const int tempV=A1;      // Chan Chiet Ap chinh bao thuc

int temValue;            // Gia tri do C
const int tempPin=A2;    // Chan Output cua LM35
int temChan=10;          // Chan nguon vao cho LM35

int dem=0;               // Bien de check cai bao thuc

int HClock;              // Gio bao thuc
int MClock;              // Phut bao thuc

LiquidCrystal lcd(12,11,5,4,3,2);  // Khoi tao cac chan LCD

tmElements_t tm;  // Khai bao Thoi gian

void setup()
{
  lcd.begin(16, 2);       // Thong bao day la LCD1602
  Serial.begin(9600);     // Mo port o muc 9600
  pinMode(9,OUTPUT);      // Dau ra Chan Loa
  pinMode(8,OUTPUT);      // Dau ra Chan Chiet Ap
  digitalWrite(8, LOW);   // Tat dau ra Chan Chiet Ap
  
  pinMode(switchPin,INPUT);     //Dau vao Chan Button
  pinMode(temChan,OUTPUT);      //Dau ra LM35
  digitalWrite(temChan, HIGH);  //Bat dau ra LM35

  while (!Serial) ; // Doi khoi dong cong Serial
  delay(200);       // Doi 0.2s
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  //Khoi tao thoi gian ban dau
}

void loop()
{
  Time();                       // Hien thi Thoi gian
  if (BaoThuc==true) Alarm();   // Neu Bao thuc BAT thi hien thi thoi gian Bao thuc    
  else {
    lcd.setCursor(11,0);        // Toi vi tri (11,0) LCD
    lcd.print("     ");         // Neu Bao thuc TAT thi hien thi thoi gian Bao thuc 
  }
  if ((dem==0)&&(tm.Second%2==0)) NhietDo();  // Neu Button ko an, thi Nhiet Do se tu reset lien tuc sau 2s
  CheckButton();                // Kiem tra Button co an hay khong
}

void CheckButton()
{
  int var= digitalRead(switchPin);  // Doc gia tri Button
  if (var==LOW)                     // Neu Button duoc an
  {
    checkSwitch=true;               // Xac nhan da an Button
    delay(500);                     // Delay 0,5s de tranh sai xot
  }
  if (var==HIGH)                    // Neu Button khong duoc an
  {
    checkSwitch=false;              // Xac nhan Button Khong duoc an
  }

  if(checkSwitch==true)             // Neu da an Button
  {
      dem++;                        // So lan an Button hien tai la 1
      checkSwitch==false;           // Xac nhan Button Khong duoc an
  }
  if (dem!=0) CheckDem();           // Lam cong viec tuong duong so lan an Button
}

void print2digits(int number)       // Ham in them chu so "0"
{
  if (number >= 0 && number < 10)   // So tron khoang tu 0-10
  {
    lcd.print('0');                 // In them so 0 phia truoc
  }
  lcd.print(number);                // Sau do In so
}

void NhietDo()      // Ham in nhiet do            
{
  digitalWrite(temChan, HIGH);    // Cap nguon cho LM35
  temValue=analogRead(tempPin);   // Doc gia tri LM35
  delay(1);                       // Doi 1s de tranh sai so
  float tempCelsius=(temValue*5.0/1024.0)*100.0;    // Nhiet do C
  float TempFahrenheit=(tempCelsius*9)/5.0+32;      // Nhiet do F
  lcd.setCursor(12,1);            // Toi vi tri (12,1) LCD
  lcd.print(tempCelsius,0);       // In ra do C da duoc lam tron
  lcd.setCursor(14,1);            // Toi vi tri (14,1) LCD
  lcd.write(223);                 // In ra do trong mang ma Ascii
  lcd.print("C");                 // In ra chu C
}

void ClearNhiet()   // Ham xoa Nhiet Do
{
   if (dem==1) digitalWrite(temChan, LOW);  // Neu nhan Button lan mot thi ko cap nguon cho LM35
   lcd.setCursor(10,1);      // Toi vi tri (10,1) LCD 
   lcd.print("      ");      // Xoa Nhiet Do
   lcd.setCursor(10,0);      // Toi vi tri (10,0) LCD
   lcd.print(" ");           // Xoa hien thi bat tat Bao thuc
}

void CheckDem()   // Thuc hien cong viec khi nhan Button
{
  if (dem==1)     // Khi nhan Button lan 1
  {
    ClearNhiet();         // Xoa nhiet do
    OpenPoint(10,1);      // Toa do diem chinh bao thuc
    if (ChietAp(0,2)==1)  // Neu gia tri cua Chiet Ap bang 1
    {
       lcd.setCursor(10,0);   // Toi vi tri (10,0) LCD 
       lcd.print('B');        // Hien thi B co nghia la BAT, da bat bao thuc 
       BaoThuc=true;          // Bao thuc da bat
    }
    else if (ChietAp(0,2)==0) // Neu gia tri cua Chiet Ap bang 0
    {
      lcd.setCursor(10,0);  // Toi vi tri (10,0) LCD 
      lcd.print('T');       // Hien thi T co nghia la TAT, da tat bao thuc 
      BaoThuc=false;        // Bao thuc da tat
    }
  }
  if (dem==2&&BaoThuc==false) dem=4; // Khi nhan Button lan 2 va ko dat Bao thuc thi nhu da nhan Button lan 4
  if (dem==2)   // Khi nhan Button lan 2
  {
    ClearNhiet();      // Xoa nhiet do
    OpenPoint(11,1);   // Toa do diem chinh bao thuc
    OpenPoint(12,1);   // Toa do diem chinh bao thuc
    HClock=ChietAp(0,24); // Chinh gio bao thuc tu 0-23
  }
  if (dem==3)   // Khi nhan Button lan 3
  {
    ClearNhiet();     // Xoa nhiet do
    OpenPoint(14,1);  // Toa do diem chinh bao thuc
    OpenPoint(15,1);  // Toa do diem chinh bao thuc
    MClock=ChietAp(0,60);  // Chinh phut bao thuc tu 0-59
  }
  if (dem==4)  // Khi nhan Button lan 4
  {
    digitalWrite(8, LOW); // Tat Chiet Ap
    ClearNhiet();         // Xoa nhiet do va nhung thu linh tinh con lai
    dem=0;                // Khoi tao lai so lan nhan Button
    NhietDo();            // Hien thi lai Nhiet do
  }
}

int ChietAp(int Start, int Stop)  // Nhan gia tri cua Chiet Ap trong khoang tuy chon
{
    digitalWrite(8, HIGH);        // Bat Chiet Ap
    value = analogRead(tempV);    // Doc gia tri cua Chiet Ap
    delay(1);                     // Doi de tranh sai xot
    int voltage;                  // Bien de luu gia tri chuyen doi
    voltage = map(value,0,1023,Start,Stop);  // Chuyen doi gia tri cua Chiet Ap sang gia tri khac
    return voltage;               // Tra gia tri vua chuyen doi cho Ham
}

void OpenPoint(int x,int y)   // Dung de in con tro Bao thuc
{
   lcd.setCursor(x,y);        // Toi vi tri (x,y) LCD 
   lcd.print('*');            // In dau * chi vi tri dang duoc thay doi
}

void Time()      // Hien thi Thoi gian
{
    if (RTC.read(tm)) {       // Doc gia tri tu DS1307
    lcd.setCursor(0,1);       // Toi vi tri (0,1) LCD 
    print2digits(tm.Hour);    // In Gio
    lcd.print(':');           // In dau 2 cham
    print2digits(tm.Minute);  // In Phut
    lcd.print(':');           // In dau 2 cham
    print2digits(tm.Second);  // In giay

    lcd.setCursor(0,0);       // Toi vi tri (0,0) LCD
    print2digits(tm.Day);     // In Ngay
    lcd.print('/');           // In dau /
    print2digits(tm.Month);   // In Thang
    lcd.print('/');           // In dau /
    lcd.print(tmYearToCalendar(tm.Year));   //In Nam
  } 
}

void Alarm()  // In thoi gian Bao thuc
{
    lcd.setCursor(11,0);    // Toi vi tri (11,0) LCD
    print2digits(HClock);   // In Gio Bao thuc
    lcd.print(":");         // In dau 2 cham
    print2digits(MClock);   // In Phut Bao thuc
    if ((HClock==tm.Hour)&&(MClock==tm.Minute)&&(BaoThuc==true)&&(dem==0))  // Cac dieu kien de Bao thuc reng
    {
        digitalWrite(9, HIGH);    // Cap nguon cho Loa
        tone(9,2093,250);         // Tan so am thanh phat ra
        delay(250);               // Ngat nhip cho Bao thuc
    }
    else digitalWrite(9, LOW);    // Tat Bao thuc
}
