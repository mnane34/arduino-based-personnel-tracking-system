#include <SPI.h>  // SPI Kütüphanesi tanımlandı
#include <RFID.h> // RFID Kütüphanesi tanımlandı

RFID Kart(10,9);  // RFID Nesnesi tanımlandı

byte BizimKart[5]={150,62,202,37,71}; // Bizim Kartımızın ID'si

/* RGB Pinleri*/
#define RGB_Kirmizi 7
#define RGB_Yesil 6
#define RGB_Mavi 5

boolean durum=true; // Kart ID Karşılaştırma Değişkeni

/* Kullanıcı Giriş - Çıkış Değişkenleri*/
int Sayac=10;

void setup() 
{
  
  Serial.begin(9600); // 9600 bps

  SPI.begin(); // SPI Başlatıldı
  Kart.init(); // RFID Başlatıldı
  
  /* RGB Pinleri Çıkış Olarak Atandı */
  pinMode(RGB_Kirmizi,OUTPUT);
  pinMode(RGB_Yesil,OUTPUT);
  pinMode(RGB_Mavi,OUTPUT);

  /* RGB Başlangıçta Sönük */
  digitalWrite(RGB_Kirmizi,HIGH);
  digitalWrite(RGB_Yesil,HIGH);
  digitalWrite(RGB_Mavi,HIGH);

}

/* RGB Kırmızı*/
void kirmizi_isik(void)
{
  digitalWrite(RGB_Kirmizi,LOW);
  digitalWrite(RGB_Yesil,HIGH);
  digitalWrite(RGB_Mavi,HIGH);
}

/* RGB Yeşil*/
void yesil_isik(void)
{
  digitalWrite(RGB_Kirmizi,HIGH);
  digitalWrite(RGB_Yesil,LOW);
  digitalWrite(RGB_Mavi,HIGH);
}

/* RGB Mavi*/
void mavi_isik(void)
{
  digitalWrite(RGB_Kirmizi,HIGH);
  digitalWrite(RGB_Yesil,HIGH);
  digitalWrite(RGB_Mavi,LOW);
}



void loop() 
{
   durum=true; // Kart Sorgulama Değişkeni Normalde True
   
   mavi_isik(); // Normalde Hep Mavi Işık Yansın

    while(Kart.isCard()) // Kart Okuduğu Sürece 
{
 
    Kart.readCardSerial();
    
   for(int i=0; i<=5; i++) // Bizim Kartımız ile Okunan Kart ID'si Karşılaştırılır
   {
    if(Kart.serNum[i] != BizimKart[i]) 
    {
      durum = false; // Kartlar Uyuşmuyor.
    } 
   }

/* Hatalı Kart Girişi*/
  if(durum==false)
  {
    kirmizi_isik();
    Serial.write(1); // Hatalı Giriş.
    delay(500);
  }
/* Kart Okutuldu , Giriş*/
  else if(durum==true & Sayac==10)
  {
    yesil_isik();
    Serial.write(2);  // Doğru Giriş. Kullanıcı Giriş Yaptı
    delay(500); 
  }

/* Kart Okutuldu , Çıkış*/
  else if(durum==true & Sayac==11)
  {
    yesil_isik();
    Serial.write(3); // Doğru Giriş. Kullanıcı Çıkış Yaptı
    delay(500); 
  }
/* Kart Okutuldu , 2'den Fazla Giriş*/
  else if(durum==true & Sayac!=10 & Sayac!=11)
  {
    kirmizi_isik();
    Serial.write(4); // Doğru Giriş . Kullanıcı Günde Okutacağı Sınırı Geçti
    delay(500);
  }
Sayac++;  
Kart.halt();
}

}
