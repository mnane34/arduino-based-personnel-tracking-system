
#include <SPI.h> // SPI Haberleşmesi Kütüphanesi
#include  <SD.h> // SD Kart Kütüphanesi
#include <DS3231.h>  // RTC Kütüphanesi Dahil Edildi
#include <Wire.h>   // Gerekli Kütüphaneler Tanımlandı

#include <LiquidCrystal_I2C.h> // I2C LCD Kütüphanesi
LiquidCrystal_I2C lcd(0x27,16,2); // LCD Nesnesi Tanımlandı (Adres,Satır,Sutun)

 /* RTC Pin*/
#define SDA 20  
#define SCL 21

DS3231  rtc(SDA,SCL);  // RTC Nesnesi

Time  t; // Gerçek Zaman Değerine Ulaşmak İçin Gerekli bir nesne tanımlaması

/* RTC Değerleri */
int Saat; 
int Dakika;
int Gun;

/* LCD Ekranında Düzgün Saat Bilgisi Basabilmek için gerekli değişkenler */
int SaatOnlar=0; 
int SaatBirler=0;
int DakikaOnlar=0;
int DakikaBirler=0;

/*Haberleşme Değişkenleri */
int GelenData=0;

/*Dosya İşlemleri*/
File myFile; // Dosya Nesnesi
const int chipSelect=53; // SD Kart Pini

/* Çalışma Saati Değişkenleri */

unsigned int ilkSaat=0;
unsigned int sonSaat=0;
unsigned int totalSaat=0;

unsigned int ilkDakika=0;
unsigned int sonDakika=0;
unsigned int totalDakika=0;

unsigned int ilkSaniye=0;
unsigned int sonSaniye=0;
unsigned int totalSaniye=0;

unsigned int ilkGun=0;
unsigned int sonGun=0;
unsigned int totalGun=0;

unsigned int ilkAy=0;
unsigned int sonAy=0;
unsigned int totalAy=0;

unsigned int ilkYil=0;
unsigned int sonYil=0;
unsigned int totalYil=0;


void setup() 
{
  
  Serial.begin(9600); // Seri Port Baud Rate

 /*LCD Başlangıç Ayarları*/
 lcd.begin();
 lcd.home();
 lcd.backlight(); // Arka Plan Işığını Yak.


      /* RTC Ayarları*/
   rtc.begin(); // RTC Başlatıldı     
   //rtc.setDOW(7);               //  BU günü pazartesi olarak ayarladık
  //rtc.setTime(14, 1, 0);       //  saati 12:00:00 şeklinde ayarladık
  //rtc.setDate(21, 04, 2020);   // Tarihi 1/1/2014 olarak ayarladık

  if(!SD.begin(chipSelect))  // SD Kart Yüklenmezse Sonsuz Döngü ile Kilitle
  {
    Serial.println("Yukleme Basarisiz");
    while(1);
  }

}


void loop() 
{
 
  t = rtc.getTime(); // Saat Bilgisi Alabilmek için Nesne Tanımlaması gibi bişi

  Saat=(int)t.hour; // Saat Bilgisini Değişkene ata
  Dakika=(int)t.min; // Dakika Bilgisini Değişkene Ata
 


  /* LCD Ekrana Düzgün Basabilmek İçin Gerekli Matematiksel İşlemler */
  SaatOnlar = Saat / 10; 
  SaatBirler = Saat % 10;
  DakikaOnlar = Dakika / 10;
  DakikaBirler = Dakika%10;

  /* LCD Ekranda Sürekli Basılı Olmasını İstediklerim */
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("=====");
  lcd.setCursor(5,0);
  lcd.print(SaatOnlar);
  lcd.setCursor(6,0);
  lcd.print(SaatBirler);
  lcd.setCursor(7,0);
  lcd.print(":");
  lcd.setCursor(8,0);
  lcd.print(DakikaOnlar);
  lcd.setCursor(9,0);
  lcd.print(DakikaBirler);
  lcd.setCursor(10,0);
  lcd.print("======");
  lcd.setCursor(0,1);
  lcd.print("=");
  lcd.setCursor(1,1);
  lcd.print(rtc.getDateStr());
  lcd.setCursor(12,1);
  lcd.print(rtc.getDOWStr(FORMAT_SHORT));
  lcd.setCursor(15,1);
  lcd.print("=");
  delay(300);

/* UART Haberleşmesi Üzerinden Veri Alınan Kısım*/
    while(Serial.available()>0)
  {
    GelenData = Serial.read();
    Serial.println (GelenData);
  }

  /* Hatalı Kart Okuması*/
  while(GelenData==1)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Hatali Giris");
    lcd.setCursor(0,1);
    lcd.print("Yeniden Dene !");
    delay(300);

    delay(2500);
    GelenData=0;
  }


   /* Başarılı Kart Okuması , Giriş*/
  while(GelenData==2)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Giris Yapildi");
    lcd.setCursor(0,1);
    lcd.print("Hosgeldiniz ...");
    delay(300);

  ilkSaat=(int)t.hour; // Saat Bilgisini Değişkene ata
  ilkDakika=(int)t.min; // Dakika Bilgisini Değişkene Ata
  ilkSaniye =(int)t.sec; // Saniye Bilgisini Değişkene Atamak

  ilkGun=(int)t.date;
  ilkAy=(int)t.mon;
  ilkYil=(int)t.year;

  delay(2500);
  GelenData=0;

}

      /* Başarılı Kart Okuması , Çıkış*/
    while(GelenData==3)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cikis Yapildi");
    lcd.setCursor(0,1);
    lcd.print("iyi Aksamlar ...");
    delay(300);

  sonSaat=(int)t.hour; // Saat Bilgisini Değişkene ata
  sonDakika=(int)t.min; // Dakika Bilgisini Değişkene Ata
  sonSaniye =(int)t.sec; // Saniye Bilgisini Değişkene Atamak

  sonGun=(int)t.date;
  sonAy=(int)t.mon;
  sonYil=(int)t.year;

  totalGun=abs(sonGun-ilkGun); 
  totalAy=abs(sonAy-ilkAy);
  totalYil=abs(sonYil-ilkYil);


  if((totalGun==0) & (totalAy==0) & (totalYil==0 ))
{
  
 myFile = SD.open("deneme.txt",FILE_WRITE);

  if(myFile) 
  {
myFile.println("----------------------------------------------------------------------------------------------------------");
myFile.println("");
myFile.println("Ad = Mertcan");
myFile.println("Soyad = NANE");
myFile.println("Cinsiyet = Erkek");
myFile.println("TC Kimlik = 1234567890");
myFile.println("Telefon = 1234567890");
myFile.println("Meslek = Elektronik ve Haberlesme Mühendisi ");
myFile.print("Personel Cikis Saati = ");
myFile.print(sonSaat);
myFile.print(":");
myFile.print(sonDakika);
myFile.print(":");
myFile.println(sonSaniye);
myFile.print("Personel Giris Saati = ");
myFile.print(ilkSaat);
myFile.print(":");
myFile.print(ilkDakika);
myFile.print(":");
myFile.println(ilkSaniye);
myFile.print("Personel Calisma Saati = ");

totalSaat=abs(sonSaat-ilkSaat);
totalDakika=abs(sonDakika-ilkDakika);
totalSaniye=abs(sonSaniye-ilkSaniye);

myFile.print(totalSaat);
myFile.print(":");
myFile.print(totalDakika);
myFile.print(":");
myFile.println(totalSaniye);
myFile.println("");
myFile.print("                                                                              ");
myFile.print(rtc.getDateStr());
myFile.print("     ");
myFile.println(rtc.getDOWStr());
myFile.println("");
myFile.println("----------------------------------------------------------------------------------------------------------");

myFile.close();
}
}

delay(2500);
GelenData=0;
 
  }
/* Başarılı Giriş Ancak 2'den Fazla Okuma Yapılmıştır*/
    while(GelenData==4)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  Yeteri Okuma");
    lcd.setCursor(0,1);
    lcd.print("    Yapildi");
    delay(300);

    delay(2500);
    GelenData=0;
  }
  
}
