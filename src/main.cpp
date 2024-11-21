//SQL
#undef SQLITE_ESP32VFS_BUFFERSZ
#define SQLITE_ESP32VFS_BUFFERSZ 1024

//BLUETOOTH
#undef RX_QUEUE_SIZE
#define RX_QUEUE_SIZE 1080

#include <Arduino.h>
#include <RTClib.h>
#include <sqlite3.h>
#include <BluetoothSerial.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <SD.h>
#include <FPM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Update.h>
#include <TinyGPSPlus.h>
//manifest
//"https://drive.google.com/uc?export=download&id=1iWa3G4wrV-FPYcnqddI0h3QE3EEdb3eH"
//log
//#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#undef RX_QUEUE_SIZE
#define RX_QUEUE_SIZE 1080
//Identity
#define SN 21298001
#define PATH_OTA  "/firmware.bin"
#define PRODUCT_NAME "FP307S-BW-GPS"
//#define PRODUCT_NAME "TESTING.... UPDATE"
#define SENSOR_TYPE "R307S"
#define FIRMWARE "1.3.1"
#define HARDWARE_VER "1.0.0"
#define MAX_FINGER "1000"

//Debug ON-OF
#define DEBUG_ON 0
#if DEBUG_ON == 1
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTF(x,y) Serial.printf(x,y)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(x,y)
#endif

//MACRO SQL
#undef SQLITE_ESP32VFS_BUFFERSZ
#define SQLITE_ESP32VFS_BUFFERSZ 2048

//MACRO BLUETOOTH
#undef RX_QUEUE_SIZE
#define RX_QUEUE_SIZE 1080

//MACRO FINGERPRINT
#define BUFF_SZ     1024
#define BUKA_SENSOR -1
#define TUTUP_SENSOR 1
#define KEDIP 500
#define MAX_ROW 100000
#define GANTI_MODE "/wifi_mode"
#define GPS_MODE "/gps_mode"
#define MULTIPLE_SCAN "/multi_scan"
//MACRO OUTPUT
#define CONNECTION 14 //CONNECTION
#define ERROR 25 //ERR
#define BUZZ 26 // BUZZER
#define HIJAU 32
#define CONTROL 12 //5V control
#define OUTPUT_PIN {14,25,4,32,12}
#define OUTPUT_PIN_NUMBER 5
#define LED_PIN {14,25,4,32}
#define LED_PIN_NUMBER 4
//MACRO INPUT
#define BUTTON1 36
#define BUTTON2 2
#define BATSENSE 33 //BATTERY SENSE VOLTAGE
#define JARI 15
#define INPUT_PIN {2,34,35,36,39,15}
#define INPUT_PIN_NUMBER 6
//define timeout register 1 JAM
#define TIMEOUT 1800000 
//Macro Tombol
#define OPEN 0
#define CLOSE_CEK 1
#define CLOSE_SHORT 2
#define CLOSE_LONG 3
#define CLOSE_LONG2 4

//Buzzer passive setting
#define FREQ 10000
#define CHANNEL 0
#define RESOLUTION 8

//Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
//Bitmap
#define bat_width 20
#define bat_height 12
// 'bat4', 20x12px
const unsigned char bat4 [] PROGMEM = {
  0xff, 0xff, 0xe0, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xf0, 0xff, 
  0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xe0, 0xff, 0xff, 
  0xe0, 0xff, 0xff, 0xe0
};
// 'bat3', 20x12px
const unsigned char bat3 [] PROGMEM = {
  0xff, 0xff, 0xe0, 0xff, 0xfc, 0x20, 0xff, 0xfc, 0x20, 0xff, 0xfc, 0x20, 0xff, 0xfc, 0x30, 0xff, 
  0xfc, 0x30, 0xff, 0xfc, 0x30, 0xff, 0xfc, 0x30, 0xff, 0xfc, 0x20, 0xff, 0xfc, 0x20, 0xff, 0xfc, 
  0x20, 0xff, 0xff, 0xe0
};
// 'bat2', 20x12px
const unsigned char bat2 [] PROGMEM = {
  0xff, 0xff, 0xe0, 0xff, 0xc0, 0x20, 0xff, 0xc0, 0x20, 0xff, 0xc0, 0x20, 0xff, 0xc0, 0x30, 0xff, 
  0xc0, 0x30, 0xff, 0xc0, 0x30, 0xff, 0xc0, 0x30, 0xff, 0xc0, 0x20, 0xff, 0xc0, 0x20, 0xff, 0xc0, 
  0x20, 0xff, 0xff, 0xe0
};
// 'bat1', 20x12px
const unsigned char bat1 [] PROGMEM = {
  0xff, 0xff, 0xe0, 0xfc, 0x00, 0x20, 0xfc, 0x00, 0x20, 0xfc, 0x00, 0x20, 0xfc, 0x00, 0x30, 0xfc, 
  0x00, 0x30, 0xfc, 0x00, 0x30, 0xfc, 0x00, 0x30, 0xfc, 0x00, 0x20, 0xfc, 0x00, 0x20, 0xfc, 0x00, 
  0x20, 0xff, 0xff, 0xe0
};
// 'bat0', 20x12px
const unsigned char bat0 [] PROGMEM = {
  0xff, 0xff, 0xe0, 0x80, 0x00, 0x20, 0x80, 0x00, 0x20, 0x80, 0x00, 0x20, 0x80, 0x00, 0x30, 0x80, 
  0x00, 0x30, 0x80, 0x00, 0x30, 0x80, 0x00, 0x30, 0x80, 0x00, 0x20, 0x80, 0x00, 0x20, 0x80, 0x00, 
  0x20, 0xff, 0xff, 0xe0
};
// 'wificon', 16x16px
const unsigned char wificon [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x1c, 0x38, 0x71, 0x8e, 0x0f, 0xf0, 0x18, 0x18, 
  0x03, 0xc0, 0x07, 0xe0, 0x00, 0x00, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'wifierr', 16x16px
const unsigned char wifierr [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x3f, 0xf0, 0x1c, 0x38, 0x6f, 0x8e, 0x07, 0xf0, 0x1b, 0x18, 
  0x01, 0xc0, 0x06, 0xf0, 0x00, 0x60, 0x01, 0xb0, 0x01, 0x98, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00
};

#define bt_width 16
#define bt_height 16
// 'bterr (2)', 16x16px
const unsigned char bterr [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x11, 0xc0, 0x19, 0xe0, 0x0c, 0xb0, 0x06, 0x60, 0x03, 0x00, 
  0x01, 0x80, 0x03, 0xc0, 0x07, 0xe0, 0x0d, 0xf0, 0x09, 0xd8, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00
};
// 'btcon (2)', 16x16px
const unsigned char btcon [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x80, 0x33, 0xc0, 0x1a, 0xcc, 0x0f, 0x8c, 0x07, 0x24, 
  0x07, 0x24, 0x0f, 0x84, 0x1a, 0xcc, 0x32, 0xc0, 0x03, 0x80, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00
};

#define gps_width 16
#define gps_height 16

#define gps_width 16
#define gps_height 16
// 'gps', 16x16px
const unsigned char gpson [] PROGMEM = {
	0x00, 0x00, 0x03, 0xe0, 0x0f, 0xf0, 0x1e, 0x78, 0x1c, 0x18, 0x18, 0x1c, 0x38, 0x1c, 0x1c, 0x1c, 
	0x1e, 0x38, 0x1f, 0xf8, 0x0f, 0xf0, 0x0f, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00
};

// 'gps_err', 16x16px
const unsigned char gpserr [] PROGMEM = {
	0xc0, 0x00, 0xe3, 0xe0, 0x7f, 0xf0, 0x3e, 0x78, 0x1c, 0x18, 0x1e, 0x1c, 0x3f, 0x1c, 0x1f, 0xdc, 
	0x1f, 0xf8, 0x1f, 0xf8, 0x0f, 0xf8, 0x0f, 0xf8, 0x07, 0xfc, 0x03, 0xce, 0x01, 0x87, 0x00, 0x03
};
// 'gps', 16x16px
#define owl_width 32
#define owl_height 32
const unsigned char owl [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 
	0x00, 0x00, 0x03, 0x00, 0x00, 0x01, 0x83, 0x90, 0x00, 0x00, 0xc1, 0x98, 0x00, 0x00, 0x61, 0xc8, 
	0x00, 0x00, 0x21, 0xcc, 0x00, 0x00, 0x31, 0xcc, 0x00, 0x00, 0x31, 0xcc, 0x00, 0x00, 0x11, 0xcc, 
	0x00, 0x00, 0x11, 0xcc, 0x07, 0x00, 0x31, 0xcc, 0x0f, 0x80, 0x21, 0xcc, 0x1f, 0xc0, 0x21, 0xcc, 
	0x1f, 0xc0, 0x43, 0x9c, 0x1f, 0xc0, 0x43, 0x9c, 0x1f, 0xc0, 0x07, 0x38, 0x2f, 0x80, 0x0e, 0x38, 
	0x37, 0x00, 0x1c, 0x78, 0x38, 0x00, 0xf0, 0xf8, 0x1c, 0x03, 0x81, 0xf0, 0x1f, 0x00, 0x07, 0xf0, 
	0x0f, 0xe0, 0x3f, 0xe0, 0x07, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x00, 
	0x00, 0x7f, 0xfc, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char cloud [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x01, 0xff, 0xf0, 0x00, 
	0x01, 0xe7, 0xff, 0x00, 0x03, 0x9f, 0xff, 0xc0, 0x07, 0xbf, 0xff, 0xe0, 0x07, 0x3f, 0xff, 0xe0, 
	0x07, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xfc, 
	0x7f, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xfe, 
	0x7f, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char fp_icon [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x7e, 0x70, 0x00, 
	0x00, 0xe0, 0x01, 0x00, 0x01, 0x9e, 0x79, 0x80, 0x03, 0x38, 0x1c, 0xc0, 0x00, 0xe3, 0xc7, 0x60, 
	0x0d, 0xdf, 0xfb, 0xb0, 0x19, 0xb8, 0x1d, 0x98, 0x1b, 0x66, 0xe6, 0xd8, 0x16, 0x4e, 0x70, 0x68, 
	0x36, 0x18, 0x18, 0x6c, 0x34, 0xb3, 0xcd, 0x0c, 0x35, 0xb6, 0x6d, 0xac, 0x25, 0xa5, 0xa5, 0xa4, 
	0x21, 0xa5, 0xa5, 0xa4, 0x35, 0xb1, 0xa5, 0xac, 0x35, 0xb1, 0xa5, 0xac, 0x05, 0xa1, 0x85, 0xac, 
	0x0d, 0xad, 0xad, 0xa0, 0x0d, 0x8d, 0xad, 0xa0, 0x09, 0x0d, 0x2d, 0xa0, 0x03, 0x0d, 0x61, 0x20, 
	0x02, 0x4b, 0x61, 0x40, 0x00, 0xdb, 0x61, 0x00, 0x00, 0x92, 0x58, 0x00, 0x00, 0x36, 0xda, 0x00, 
	0x00, 0x0c, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//class init
RTC_DS3231 rtc;
HardwareSerial fserial(1);
FPM finger(&fserial);
FPM_System_Params params;
BluetoothSerial SerialBT;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
TinyGPSPlus gps;

//All status
struct cek_status {
  bool wifi_mode,jari,status_upload,tambah_admin,upload_data,con,cekrow,upload,statup,ledstatus,ledoff,loopscan,validasi_awal,validasi_gps,gps_status,cek_battery,validate,validate_register,submenu;
  int8_t button1=0,button2=0;
  uint8_t inot=1;
  uint8_t max_inout;
  uint8_t multiple_scan=1;
  int error_time;
  };
cek_status status_cek;

struct waktu {
  uint8_t detik;
  uint32_t unix,sec_del,last_time_led_con,last_time_led_gps,last_time_upload,register_timeout,button1,button2;
};
waktu cek_waktu;

//Variable
const char* data = "Callback function called";
char *zErrMsg = 0;
int serial_number;
sqlite3 *db1;
sqlite3_stmt *res;
const char *tail;
int8_t openFinger = 1;
int16_t idF,confidence,batcap=100;
int32_t maxrow,rowid=0,rowid2=0,intervals,full,low,error_code;
char server [80];
char namalog[18],niklog[12],privilege[22],lat[12],lng[13];;
RTC_DATA_ATTR int depleted = 0;

uint16_t getID16(){
          uint32_t low     = ESP.getEfuseMac() & 0xFFFFFFFF; 
          uint32_t high    = ( ESP.getEfuseMac() >> 32 ) % 0xFFFFFFFF;
          //uint32_t ids=ESP.getEfuseMac();
          uint16_t id=low+high;
          return id;
}

uint32_t getID32(){
          uint32_t low     = ESP.getEfuseMac() & 0xFFFFFFFF; 
          uint32_t high    = ( ESP.getEfuseMac() >> 32 ) % 0xFFFFFFFF;
          uint32_t id32=low+high;
          return id32;
}
//RTC_DATA_ATTR bool deleting = false;
void tampilkan (const char * perintah, uint8_t x,uint8_t y,uint8_t size,bool clear, bool tampil, int perintah2=0){
  //Serial.println(perintah);
  //DEBUG_PRINTLN(*perintah);
  //DEBUG_PRINTLN(perintah2);
  if(clear==true)
  display.clearDisplay();
  display.setTextSize(size);             // Normal 2:2 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(x,y);             // Start at top-left corner
  perintah2==0?display.print(perintah):display.print(perintah2);
        //Serial.println(perintah);
  if(tampil==true)
    display.display();
      //delay(10);
}

void tampilkan_bitmap(const unsigned char * perintah,uint8_t x,uint8_t y, uint8_t width, uint8_t height, bool clear, bool tampil){
  if(clear==true)
    display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.drawBitmap( x,y, perintah, width, height, WHITE); // logo
  if(tampil==true)
    display.display(); 
}

void performUpdate(Stream &updateSource, size_t updateSize) {
   if (Update.begin(updateSize)) {      
      size_t written = Update.writeStream(updateSource);
      if (written == updateSize) {
         Serial.println("Written : " + String(written) + " successfully");
      }
      else {
         Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
      }
      if (Update.end()) {
         Serial.println("OTA done!");
         if (Update.isFinished()) {
            Serial.println("Update successfully completed. Rebooting.");
            tampilkan("UPDATE SELESAI...",0,21,1,true,true);         
         }
         else {
            Serial.println("Update not finished? Something went wrong!");
         }
      }
      else {
         Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }

   }
   else
   {
      Serial.println("Not enough space to begin OTA");
   }
}
// check given FS for valid update.bin and perform update if available
void updateFromFS(fs::FS &fs) {
   File updateBin = fs.open(PATH_OTA);
   if (updateBin) {
      if(updateBin.isDirectory()){
         Serial.println("Error, update.bin is not a file");
         updateBin.close();
         return;
      }

      size_t updateSize = updateBin.size();

      if (updateSize > 0) {
         Serial.println("Try to start update");
         tampilkan("UPDATE FIRMWARE...",0,21,1,true,true);         
         performUpdate(updateBin, updateSize);
      }

      else {
         Serial.println("Error, file is empty");
      }
      
      updateBin.close();
      // whe finished remove the binary from sd card to indicate end of the process
      fs.remove(PATH_OTA);      
      delay(1000);
      ESP.restart();
   }
   else {
      Serial.println("Could not load update.bin from sd root");
   }
}

static int uploadcallback(void *data, int argc, char **argv, char **azColName){        
  DEBUG_PRINTLN ("masuk db upload");
  //Serial.printf("free heap = %d\n",esp_get_free_heap_size());
  if (status_cek.con) {//rowid,id,nik,jam,status,status_upload,latitude,longitude
    status_cek.upload_data=true;
    if(argv[0]>0)
      rowid=atoi(argv[0]);
    else rowid=0;
    //strcpy(rowid,argv[0]);
    HTTPClient http;
    http.begin(server);
    http.setTimeout(15000);
    http.setConnectTimeout(15000);
    http.addHeader(F("Content-Type"),F("application/x-www-form-urlencoded"));
    char httpreq [300]="method=insertdata&nik=";
    char serial_number_char [15];
    //rowid,id_sensor,nik,jam,status,status_upload,lat,lng
    //   0 ,    1    , 2 , 3 ,  4   ,      5      , 6 , 7
    //rowid,id_sensor,nik,jam,status,status_upload,lat,lng
    //   0 ,    1    , 2 , 3 ,  4   ,      5      , 6 , 7
    strcat(httpreq,argv[2]);
    strcat(httpreq,"&idfinger=");
    strcat(httpreq,argv[1]);
    strcat(httpreq,"&sn=");
    sprintf(serial_number_char,"%u",serial_number);//convert SN int to SN char array;
    strcat(httpreq,serial_number_char);
    strcat(httpreq,"&datetime=");
    strcat(httpreq,argv[3]);
    strcat(httpreq,"&inoutmode=");
    strcat(httpreq,argv[4]);
    strcat(httpreq,"&latitude=");
    strcat(httpreq,argv[6]);
    strcat(httpreq,"&longitude=");
    strcat(httpreq,argv[7]);
    //Serial.printf("request post, free heap = %d\n",esp_get_free_heap_size());
    int httpResponseCode = http.POST(httpreq);
    //Serial.printf("request post, free heap = %d\n",esp_get_free_heap_size());
    if (httpResponseCode>0&&httpResponseCode==200) {
      status_cek.statup=true;
      status_cek.upload=false; 
      status_cek.status_upload=true;
      DEBUG_PRINTF("HTTP RESPONSE CODE: %d",httpResponseCode);
      DEBUG_PRINTLN(http.getString());
    }
    else {
      status_cek.statup=false;
      status_cek.upload=false;
      status_cek.status_upload=false;
      DEBUG_PRINTF("HTTP RESPONSE CODE: %d",httpResponseCode);
      DEBUG_PRINTLN(http.getString());
      tampilkan("Error 4101",0,20,1,true,false);
      tampilkan("Response :",0,30,1,false,false);
      tampilkan("o",0,40,1,false,true,httpResponseCode);
      delay(700);
    }
    http.end();
  }
  return 0;  
}

void buzz (bool matikan, bool beep){
  if(matikan){
    ledcDetachPin(BUZZ);
    return;
  }

  if (!matikan){
    ledcAttachPin(BUZZ, CHANNEL);
    ledcWrite(CHANNEL,255);
    ledcWriteNote(CHANNEL,NOTE_Bb,5);
  }
  if (beep){
    delay (50);
    ledcDetachPin(BUZZ);
  }

}

void led_gagal(){
  for (int i=0;i<4;i++){
    delay (100);
    digitalWrite(ERROR,HIGH);
    buzz(false,false);
    delay (100);
    digitalWrite(ERROR,LOW);
    buzz(true,false);
    }
}

void led_sukses(){
  uint8_t led_pin[LED_PIN_NUMBER]=LED_PIN;
  for(int i=0;i<LED_PIN_NUMBER;i++){
    digitalWrite(led_pin[i],HIGH);
    buzz(false,false);
  }
  delay(1000);
  buzz(true,false);
  //delay(4000);
  for(int i=0;i<LED_PIN_NUMBER;i++){
  digitalWrite(led_pin[i],LOW);
  }
}

int openDb(const char *filename, sqlite3 **db) {
   int rc = sqlite3_open(filename, db);
   if (rc) {
       DEBUG_PRINTF("Can't open database: %s\n", sqlite3_errmsg(*db));
       return rc;
   } else {
       DEBUG_PRINT("Opened database successfully\n");
   }
   return rc;
}

int db_upload(sqlite3 *db, const char *sql) {
   int rc = sqlite3_exec(db, sql, uploadcallback, (void*)data, &zErrMsg);
   if (rc != SQLITE_OK) {
    DEBUG_PRINTF("SQL error: %s\n", zErrMsg);
    tampilkan(zErrMsg,0,0,1,true,true);
    sqlite3_free(zErrMsg);
    delay(500);
    led_gagal();
    sqlite3_close(db1);
   }
   else {
     DEBUG_PRINTLN("DB_UPLOAD SUKSES");
   }
   return rc;}

#define PATH_TIMEZONE "/tz"
#define PATH_NEWTIME  "/newtime"
//when new setting time > create > newtime, delete timezone
uint32_t get_timeZ(uint32_t *local, uint32_t *utp){ 
    if(SD.exists(PATH_TIMEZONE)){
        File file=SD.open(PATH_TIMEZONE,FILE_READ);
        vTaskDelay(10/portTICK_PERIOD_MS);
        if(file.available()>0){
            String TimeZone=file.readStringUntil('\n');
            file.close();
            return TimeZone.toInt();
        }
        file.close();
        return 0;
    }

    else if(!SD.exists(PATH_TIMEZONE)){
        int32_t diff=*local-*utp;
        int x=diff/3600;
        int c=diff%3600;
        int z=0;
        if(abs(c)>30*60)
            z=x+c/(abs(c));
        else
            z=x;

        File file=SD.open(PATH_TIMEZONE,FILE_WRITE);
            if(file){
                file.println(z*60*60);
            }
        file.close();
        SD.remove(PATH_NEWTIME);
        return z*60*60;
    }
}

#define SINC_FROM_NTP   1
#define SINC_FROM_GPS   2
//1203 not yet setting
//1204 setting but not yet get Tz
//1205 setting, tz, but faild sinc

int sinc_time(int error=1204, int mode=1, uint32_t *unixgps=0 ){
    DateTime now = rtc.now();
    if(error==1204){
        switch (mode)
        {
            case SINC_FROM_NTP:{
                configTime(0,0,"0.pool.ntp.org","1.pool.ntp.org");
                struct tm timeinfo;
                time_t timeObject;
                if(!getLocalTime(&timeinfo)){
                    error=1204;
                    return 1204;
                }
                time(&timeObject);
                uint32_t unix=timeObject;
                uint32_t local=now.unixtime();  
                get_timeZ(&local,&unix);          
            }
                break;
            case SINC_FROM_GPS:{
                uint32_t local=now.unixtime();  
                get_timeZ(&local,unixgps);
            }
        }
    }

    if(SD.exists(PATH_TIMEZONE)){
        switch (mode)
        {
            case SINC_FROM_NTP:{
                configTime(0,0,"0.pool.ntp.org","1.pool.ntp.org");
                struct tm timeinfo;
                time_t timeObject;
                if(!getLocalTime(&timeinfo))
                    return 1205;
                time(&timeObject);
                uint32_t unix=timeObject;
                rtc.adjust(DateTime(unix+get_timeZ(NULL,NULL))); 
            }
                break;
            case SINC_FROM_GPS:{
                rtc.adjust(DateTime(*unixgps+get_timeZ(NULL,NULL)));
            }
        }
    }
  return 0;
}
void check_update(int bat, float temp,uint32_t sn){//TODO: KONFIRM
  if(WiFi.status()==WL_CONNECTED){
    HTTPClient http;    
    http.begin("http://103.150.191.136/owl_inventory/produksi/firmware.php");
    http.setTimeout(15000);
    http.setConnectTimeout(15000);
    http.addHeader(F("Content-Type"), F("application/x-www-form-urlencoded"));
    char httpreq[170];
    IPAddress ipa = WiFi.localIP();
    uint8_t IP_array[4]= {ipa[0],ipa[1],ipa[2],ipa[3]};
    sprintf(httpreq,"sn=%d&produk=%s&firmware=%s&hardware=%s&bat=%d&temperature=%.2f&ip_address=%d.%d.%d.%d",sn,PRODUCT_NAME,FIRMWARE,HARDWARE_VER,bat,temp,ipa[0],ipa[1],ipa[2],ipa[3]);
    DEBUG_PRINTLN(httpreq);
    int httpCode = http.POST(httpreq);
                if(httpCode > 0) {
                DEBUG_PRINTF("[HTTP] GET... code: %d\n", httpCode);
                if(httpCode == HTTP_CODE_OK) {
                  tampilkan("UPDATE ?",0,25,2,true,false);//
                  tampilkan("YA",105,0,1,false,false);//1
                  tampilkan("TDK",105,54,1,false,true);//2                 
                  int choice=0;
                  while (choice==0)
                  {
                    delay(50);
                    if(digitalRead(BUTTON1)==HIGH){
                      choice=2;
                      delay(500);
                      return;
                      } //no                  //button 1 bawah, button 2 atas
                    if(digitalRead(BUTTON2)==HIGH){
                      choice=1; //yes (atas)
                    }
                  }
                    tampilkan("DOWNLOAD FP301T....",10,10,1,true,true);
                    File file=SD.open(PATH_OTA,FILE_WRITE);
                    int len = http.getSize();
                    int max_len=len;
                    int persen;
                    int percentage;
                    uint8_t buff[128] = { 0 };
                    WiFiClient * stream = http.getStreamPtr();
                    while(http.connected() && (len > 0 || len == -1)) {
                        size_t size = stream->available();
                        if(size) {
                            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
                            file.write(buff, c);
                            if(len > 0) {
                                len -= c;
                            }
                        }
                        persen=map(len,0,max_len,100,0);
                        if(percentage!=persen){
                          percentage=persen;
                          char tampil [10];
                          sprintf(tampil,"%d",percentage);
                          strcat(tampil,"%");
                          tampilkan(tampil,0,10,2,true,false);
                          tampilkan("download FP307",0,30,1,false,true);
                          }
                        vTaskDelay(1/portTICK_PERIOD_MS);
                    }
                    DEBUG_PRINTLN("[HTTP] connection closed or file end.\n");
                    if(percentage!=100){
                      tampilkan("DOWNLOAD GAGAL...",10,10,1,true,true);
                      file.close();
                      SD.remove(PATH_OTA);
                      led_gagal();
                      return;
                    }
                file.close();//sukses;
                http.end();
                ESP.restart();
                }
            }
            else {
                http.end();
                tampilkan("TIDAK TERKONEKSI...",10,10,1,true,true);
                led_gagal();
                DEBUG_PRINTLN("[HTTP] GET... failed, error: %s\n");
                DEBUG_PRINTLN(http.errorToString(httpCode).c_str());
            }           
}
};
/*
void check_update(){//TODO: KONFIRM
  if(WiFi.status()==WL_CONNECTED){
//DOWNLOAD ---- MANIFEST --------------
  char newfirmware [10]="";
  if(true){
            HTTPClient http;
            tampilkan("CHECKING UPDATE..",10,10,1,true,true);
            DEBUG_PRINTLN("[HTTP] begin...\n");
            http.begin("https://drive.google.com/uc?export=download&id=1iWa3G4wrV-FPYcnqddI0h3QE3EEdb3eH");
            http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
            DEBUG_PRINTLN("[HTTP] GET...\n");
            int httpCode = http.GET();
            if(httpCode > 0 && httpCode == HTTP_CODE_OK) {
                DEBUG_PRINTF("[HTTP] GET... code: %d\n", httpCode);
                int len = http.getSize();
                WiFiClient * stream = http.getStreamPtr();
                int length=stream->readBytesUntil('\n',newfirmware,len);
                newfirmware[length]='\0';
                }
            else {
                tampilkan("TIDAK TERKONEKSI...",10,10,1,true,true);
                led_gagal();
                DEBUG_PRINTLN("[HTTP] GET... failed, error: %s\n");
                DEBUG_PRINTLN(http.errorToString(httpCode).c_str());
            }
            http.end();
  }
  if(strlen(newfirmware)>=3 && strcmp(newfirmware,FIRMWARE)!=0){//THERE IS NEW VERSION/DIFFERENT VERSION
    tampilkan("UPDATE ?",0,25,2,true,false);//
    tampilkan("YA",105,0,1,false,false);//1
    tampilkan("TDK",105,54,1,false,true);//2                  
    int choice=0;
    while (choice==0)
    {
      delay(50);
      if(digitalRead(BUTTON1)==HIGH){
        choice=2;
        delay(500);
        return;
        } //no                  //button 1 bawah, button 2 atas
      if(digitalRead(BUTTON2)==HIGH){
        choice=1; //yes (atas)
      }
    }
    HTTPClient http;
    tampilkan("DOWNLOAD FP3017....",0,30,1,true,true);
    http.begin("https://drive.google.com/uc?export=download&id=1NpKB2tOHmW04Uev9N9jB9kjFmzLAMy9K");
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    DEBUG_PRINTLN("[HTTP] GET...\n");
    int httpCode = http.GET();
                if(httpCode > 0) {
                DEBUG_PRINTF("[HTTP] GET... code: %d\n", httpCode);
                if(httpCode == HTTP_CODE_OK) {
                    File file=SD.open(PATH_OTA,FILE_WRITE);
                    int len = http.getSize();
                    int max_len=len;
                    int persen;
                    int percentage;
                    uint8_t buff[128] = { 0 };
                    WiFiClient * stream = http.getStreamPtr();
                    while(http.connected() && (len > 0 || len == -1)) {
                        size_t size = stream->available();
                        if(size) {
                            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
                            file.write(buff, c);
                            if(len > 0) {
                                len -= c;
                            }
                        }
                        persen=map(len,0,max_len,100,0);
                        if(percentage!=persen){
                          percentage=persen;
                          char tampil [10];
                          sprintf(tampil,"%d",percentage);
                          strcat(tampil,"%");
                          tampilkan(tampil,0,10,2,true,false);
                          tampilkan("download FP307",0,30,1,false,true);
                          }
                        vTaskDelay(1/portTICK_PERIOD_MS);
                    }
                    DEBUG_PRINTLN("[HTTP] connection closed or file end.\n");
                    if(percentage!=100){
                      tampilkan("DOWNLOAD GAGAL...",10,10,1,true,true);
                      file.close();
                      SD.remove(PATH_OTA);
                      led_gagal();
                      return;
                    }
                file.close();//sukses;
                http.end();
                ESP.restart();
                }
            }
            else {
                http.end();
                tampilkan("TIDAK TERKONEKSI...",10,10,1,true,true);
                led_gagal();
                DEBUG_PRINTLN("[HTTP] GET... failed, error: %s\n");
                DEBUG_PRINTLN(http.errorToString(httpCode).c_str());
            }           
  }
}
};
*/

void select_server(){
  char ssid [30]="na";
  char pass [30]="na";
  File file = SD.open("/ssid",FILE_READ);
  if (file && file.available()>0){
    int cek=file.readBytesUntil('\n',ssid,29);
    ssid[cek]='\0';
  }
  file.close();

  file = SD.open("/pass",FILE_READ);
  if (file && file.available()>0){
    int cek=file.readBytesUntil('\n',pass,29);
    pass[cek]='\0';
  }
  file.close();
      
  file = SD.open("/ip",FILE_READ);
  if (file && file.available()>0){
    int cek=file.readBytesUntil('\n',server,99);
    server[cek]='\0';
  }
  file.close();

  if((strlen(pass)<=2)&&(strlen(ssid)<=2))
    WiFi.begin("Devices","87654321");
  else if(strlen(pass)<=2&&(strlen(ssid)>2))
    WiFi.begin(ssid);
  else WiFi.begin(ssid,pass);     //Connect to your WiFi router
  DEBUG_PRINT(".");
  DEBUG_PRINT("Connecting");       // Wait for connection
  int i=0;
  tampilkan ("Connecting to..",0,20,1,true,false);
  tampilkan (ssid,0,31,1,false,true);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(CONNECTION,HIGH);
    delay(500);
    digitalWrite(CONNECTION,LOW);
    delay(500);
    DEBUG_PRINT(".");
    i++;
    if(i>=10){
      status_cek.con=false;
      return;
    }
  }
  status_cek.con=true;
  check_update(batcap=map(analogRead(BATSENSE),low,full,0,100),rtc.getTemperature(),serial_number);
  if(status_cek.error_time==1202||status_cek.error_time==1204)
    error_code=status_cek.error_time=sinc_time(status_cek.error_time,SINC_FROM_NTP);

}

uint16_t read_template(uint16_t fid, uint16_t buff_sz)
{
    uint8_t template_buffer [buff_sz];
    uint8_t * buffer=template_buffer;
    int16_t p = finger.loadModel(fid);
    
    switch (p) {
        case FPM_OK:
            DEBUG_PRINT("Template "); DEBUG_PRINT(fid); DEBUG_PRINTLN(" loaded");
            break;
        default:
            DEBUG_PRINT("Unknown error: "); DEBUG_PRINTLN(p);
            return 0;
    }

    // OK success!

    p = finger.downloadModel();
    switch (p) {
        case FPM_OK:
            break;
        default:
            DEBUG_PRINT("Unknown error: "); DEBUG_PRINTLN(p);
            return 0; 
    }

    bool read_finished;
    int16_t count = 0;
    uint16_t readlen = buff_sz;
    uint16_t pos = 0;

    while (true) {
        bool ret = finger.readRaw(FPM_OUTPUT_TO_BUFFER, buffer + pos, &read_finished, &readlen);
        if (ret) {
            count++;
            pos += readlen;
            readlen = buff_sz - pos;
            if (read_finished)
                break;
        }
        else {
            DEBUG_PRINT("Error receiving packet ");
            DEBUG_PRINTLN(count);
            return 0;
        }
        yield();
    }
    
    uint16_t total_bytes = count * FPM::packet_lengths[params.packet_len];
     for(int i=0;i<BUFF_SZ;i++){
                         if(template_buffer[i]<16){
                          SerialBT.print(0);
                         }
                         SerialBT.print(template_buffer[i],HEX);
                         }
                        SerialBT.print("\"}");
                        SerialBT.print('\n');
                        
    return total_bytes;
}

void delete_template(uint16_t fid) {
    int16_t p = finger.deleteModel(fid);
    switch (p) {
        case FPM_OK:
            DEBUG_PRINT("Template "); DEBUG_PRINT(fid); DEBUG_PRINTLN(" deleted");
            break;
        default:
            DEBUG_PRINT("Unknown error: "); 
            DEBUG_PRINTLN(p);
            break;
    }
    return;
}

static int callback2(void *data, int argc, char **argv, char **azColName){
  int i;
   //DEBUG_PRINTLN("%s: ", (const char*)data);
   /*
    if (idF==-12){
    if(argv[0]>0)modenow=atoi(argv[0]);
    if(argv[1]>0)intervals=atoi(argv[1]);
    if(argv[2]>0)full=atoi(argv[2]);
    if(argv[3]>0)low=atoi(argv[3]);
   }
    */
  if (idF==-2){ //tambah admin ambil ID dlu
    idF=atoi(argv[0]);
    return 0;
  }
   
  else if (idF==-1){ // read data_base apapun isinya
    SerialBT.print("{");
    for (i = 0; i<argc; i++){
      if(i==argc-1)
       SerialBT.printf("\"%s\":\"%s\"", azColName[i], argv[i] ? argv[i] : "NULL");
      else
       SerialBT.printf("\"%s\":\"%s\",", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   SerialBT.printf("}\n");
  } 
   
  else if (idF==-5){//delete datakaryawan di sensor by nik
   idF=atoi(argv[0]);
   delete_template(idF);
   idF=-5;
  }
  
  else if (idF==-6 or idF==-7){// download template sn,sensor,idf,nik,template {"nik":"value","template":"value"}
    idF=atoi(argv[0]);
    SerialBT.print("{\"sn\":\"");
    SerialBT.print(serial_number);
    SerialBT.print("\",\"sensor\":\"");
    SerialBT.print(SENSOR_TYPE);
    SerialBT.print("\",\"idF\":\"");
    SerialBT.print(argv[0]);
    SerialBT.print("\",\"nik\":\"");
    SerialBT.print(argv[1]);
    SerialBT.print("\",\"nama\":\"");
    SerialBT.print(argv[2]);
      /*
    if(argv[3]>0){
      SerialBT.print("\",\"jari\":\"");
      SerialBT.print(argv[3]);
    } */
    SerialBT.print("\",\"template\":\"");
    uint16_t total_read = read_template(idF, BUFF_SZ);
    if (!total_read){
      SerialBT.print("gagal\"}");
      SerialBT.print('\n');
    }  
    idF=-6;
    delay(200);
  }
   
  else if(!status_cek.tambah_admin && idF>=0 && openFinger!=BUKA_SENSOR && !status_cek.cekrow){ //jika idf>0 case untuk cek max row, ambil nik validasi/validate finger, ambil nik untuk tambah admin
    if(status_cek.validasi_awal){ //hasil scan jari > ambil nik untuk di cek privilege nya
      if(argv[1]>0) 
        strcpy(niklog,argv[1]);
    }
    
    else{ //hasil search nik >> masuk ke callback2 artinya ada privilege
      DEBUG_PRINTLN("validasi finger");
      SerialBT.println("{\"validatefinger\":\"correct\"}");
      if(argv[1]>0) strcpy(privilege,argv[1]);
        status_cek.validate=true;
    }
  }

  else if(idF>=0 && openFinger==BUKA_SENSOR && !status_cek.cekrow){ //buka absen ambil nik & nama;
    DEBUG_PRINTLN(argv[0]);
    DEBUG_PRINTLN(argv[1]);
    DEBUG_PRINTLN(argv[2]);
    if (argv[2]>0)
      memcpy(namalog,argv[2],18);
    namalog[17]='\0'; 
    strcpy(niklog,argv[1]);
    DEBUG_PRINTLN(niklog);
    DEBUG_PRINTLN(namalog);
    SerialBT.println(niklog);
  }
  
  else if(idF==-3 || status_cek.cekrow){ //cek maxrow
    if(argv[0]>0)
    maxrow=atoi(argv[0]);
    else maxrow=0;
  }
  
  return 0;
}

bool db_exec2(sqlite3 *db, const char *sql) {
  DEBUG_PRINTLN(sql);
  int rc = sqlite3_exec(db, sql, callback2, (void*)data, &zErrMsg);
  if (rc != SQLITE_OK) {
    DEBUG_PRINTF("SQL error: %s\n", zErrMsg);
    SerialBT.printf("SQL error: %s\n", zErrMsg);
    tampilkan(zErrMsg,0,0,1,true,true);
    sqlite3_free(zErrMsg);
    delay(500);
    sqlite3_close(db1);
    return false;
    //idF=-1;
  } 
  
  else {
    DEBUG_PRINTLN("Operation done successfully\n");
    //SerialBT.println("Operation done successfully\n");
    delay(50);
    sqlite3_close(db1);
    return true;
  }
}
void standby();
void setup() {
  //LOG
  //params.capacity = 1700;
  // put your setup code here, to run once:
  
  
  if(esp_sleep_get_wakeup_cause()==ESP_SLEEP_WAKEUP_EXT1){
    pinMode(BUTTON1,INPUT);
    pinMode(BUTTON2,INPUT);
    for (int i = 0; i < 30; i++)
    {
      delay(100);
      if(digitalRead(BUTTON1)==LOW&&digitalRead(BUTTON2)==LOW)
        standby();
    }
    pinMode(BATSENSE,INPUT);
  if(analogRead(BATSENSE)<=depleted){
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    esp_deep_sleep_start();
  }
    
    //if lolos, cek voltage, jiga lowbatt, shutdown then charge
  }
  Serial.begin(9600); //START GPS && OUTPUT DEBUG
  ledcSetup(CHANNEL, FREQ, RESOLUTION);
  uint8_t output_pin[OUTPUT_PIN_NUMBER]=OUTPUT_PIN;
  uint8_t input_pin[INPUT_PIN_NUMBER]=INPUT_PIN;
  for(int i=0; i<OUTPUT_PIN_NUMBER;i++){
    pinMode(output_pin[i],OUTPUT);
  }
  digitalWrite(CONTROL,HIGH);
  delay(500);
  for(int i=0; i<INPUT_PIN_NUMBER;i++){
    pinMode(input_pin[i],INPUT);
  }
  uint8_t led_pin[LED_PIN_NUMBER]=LED_PIN;
  for(int i=0;i<LED_PIN_NUMBER;i++){
    digitalWrite(led_pin[i],HIGH);
    if(i==LED_PIN_NUMBER-1)
      buzz(false,true);
    //delay(150);
  }
  DEBUG_PRINTLN("FP307 START");

  Wire.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    error_code=1401;

  tampilkan_bitmap(owl,0,15,owl_width,owl_height,true,false);
  tampilkan(PRODUCT_NAME,35,27,1,false,false);
  tampilkan(FIRMWARE,35,37,1,false,true);

  if(!rtc.begin()){
    error_code=1201;
    DEBUG_PRINTLN("ERROR 1201 RTC GAGAL");
    tampilkan("o",0,20,1,true,false,error_code);
    tampilkan("JAM GAGAL",0,31,1,false,true);
    led_gagal();
  }

  if(rtc.lostPower()){
    error_code=1202;
    status_cek.error_time=1202;
    DEBUG_PRINTLN("ERROR 1202 WATKU TIDAK AKURAT");
    tampilkan("o",0,20,1,true,false,error_code);
    tampilkan("JAM TIDAK AKURAT",0,31,1,false,true);
    led_gagal();
  }

  SPI.begin();
  if(!SD.begin()){
    error_code=1301;
    DEBUG_PRINTLN("ERROR 1301 SD CARD GAGAL");
    tampilkan("o",0,20,1,true,false,error_code);
    tampilkan("SD CARD GAGAL",0,31,1,false,true);
    led_gagal();
  }

  if(SD.exists(PATH_OTA))
    updateFromFS(SD);
  SD.remove(PATH_OTA); // memastikan tidak ada bootloop

  fserial.begin(57600,SERIAL_8N1,16,17);
  if (finger.begin()){
    finger.readParams(&params);
    DEBUG_PRINTLN("Found fingerprint sensor");
    DEBUG_PRINT("Capacity: "); DEBUG_PRINTLN(params.capacity);
    DEBUG_PRINT("Packet length: "); DEBUG_PRINTLN(FPM::packet_lengths[params.packet_len]);
  }
  else {
    error_code = 1101;
    DEBUG_PRINTLN("ERROR 1101 FINGERPRINT GAGAL");
    tampilkan("o",0,20,1,true,false,error_code);
    tampilkan("SENSOR FP GAGAL",0,31,1,false,true);
    led_gagal();
  }

  sqlite3_initialize();
  bool cek;
  openDb("/sd/privilege.db", &db1); //privilege
  cek=db_exec2(db1,"CREATE TABLE IF NOT EXISTS privilege (id_sensor	INTEGER PRIMARY KEY,privilege	text,nik text)");
  if (!cek){
    error_code =3202;
    DEBUG_PRINTLN("DB PRIVILEGE ERROR");
    tampilkan("o",0,20,1,true,false,error_code);
    tampilkan("DATA PRIVILAGE GAGAL",0,31,1,false,true);
  }
  openDb("/sd/datakar.db", &db1); //privilege
  cek=db_exec2(db1,"CREATE TABLE IF NOT EXISTS finger_template (id_sensor	INTEGER,nik	text,nama	text)");
  if (!cek){
    error_code =3203;
    DEBUG_PRINTLN("DB KARYAWAN ERROR");
    tampilkan("o",0,20,1,true,false,error_code);
    tampilkan("DATA KARYAWAN GAGAL",0,31,1,false,true);
  }
  openDb("/sd/absensi.db", &db1); //privilege
  cek=db_exec2(db1,"CREATE TABLE IF NOT EXISTS absensi_log (rowid	INTEGER PRIMARY KEY,id_sensor INTEGER,nik text,jam text,status INTEGER,status_upload INTEGER DEFAULT 0,lat text,lng text)");
  if (!cek){
    error_code =3204;
    DEBUG_PRINTLN("DB ABSENSI ERROR");
    tampilkan("o",0,20,1,true,false,error_code);
    tampilkan("DATA ABSENSI GAGAL",0,31,1,false,true);
   }
  //setup parameter (wifi_mode, invervals, full, low, ledoff, loopscan, sec_del,last_del)
  //gps_mode
  status_cek.validasi_gps=SD.exists(GPS_MODE);
  //wifi_mode
  status_cek.wifi_mode=SD.exists(GANTI_MODE);
  //ledoff
  //status_cek.ledoff=true; //untuk versi finger 301T tidak ada led_control
  //loopscan
  status_cek.loopscan=SD.exists("/loopscan"); //untuk versi finger compact & super compact gk ada trigger scan
  //sec_del
  File file = SD.open("/sec_del","r");
  if (file){
    char test [25];
    int cek=0;
    if(file.available()>0)
      cek=file.readBytesUntil('\n',test,24);
    test[cek]='\0';
    cek_waktu.sec_del=atoi(test);
  }
  file.close();
  if(cek_waktu.sec_del==0)
    cek_waktu.sec_del=(30*24*60*60);

  //last dellete unix
  file = SD.open("/lastdelete", FILE_READ);
      if(file){
        char test [25];
        int cek=0;
        if(file.available()>0)
        cek=file.readBytesUntil('\n',test,24);
        test[cek]='\0';
        cek_waktu.unix=atoi(test);
      }
  file.close();
  cek_waktu.unix=cek_waktu.unix+cek_waktu.sec_del;

  //intervals
  file = SD.open("/intervals", FILE_READ);
      if(file){
        char test [25];
        int cek=0;
        if(file.available()>0)
          cek=file.readBytesUntil('\n',test,24);
        test[cek]='\0';
        intervals=atoi(test);
      }
  file.close();
  if (intervals==0)
    intervals=60; //60 menit

  //low&full
  file = SD.open("/batsetting", FILE_READ);
      if(file){
        char test [6];
        int cek=0;
        if(file.available()>0){
          cek=file.readBytesUntil('\n',test,5);
          test[cek]='\0';
          low=atoi(test);
          cek=file.readBytesUntil('\n',test,5);
          test[cek]='\0';
          full=atoi(test);
        }
      }
      else if(low==0||full==0){
        low=1843;
        full=2346;
      }
  file.close();
  depleted=(low*3)/3.3;
  file = SD.open("/max_flag", FILE_READ);
    if(file){
      char test [6];
      int cek=0;
        if(file.available()>0){
          cek=file.readBytesUntil('\n',test,5);
          test[cek]='\0';
          status_cek.max_inout=atoi(test);
          if (status_cek.max_inout<2)
             status_cek.max_inout=2;
        }
      }
    else
      status_cek.max_inout=2;
  file.close();

  //serial_number
  file = SD.open("/sn_inject", FILE_READ);
      if(file){
        char test [25];
        int cek=0;
        if(file.available()>0)
        cek=file.readBytesUntil('\n',test,12);
        test[cek]='\0';
        serial_number=atoi(test);
      }
  file.close();
      if(serial_number<=0)
        serial_number=getID32();
  
  if (!SD.exists("/pin.txt")){
    file = SD.open("/pin.txt",FILE_WRITE);
    if(file){
     file.print("1234");
     file.write('\n');
    }
    file.close();
  }

  if(SD.exists(MULTIPLE_SCAN))
    status_cek.multiple_scan=2;

  //CHECK WAKTU ERROR
  if(!SD.exists(PATH_NEWTIME)&&!SD.exists(PATH_TIMEZONE)){
    error_code=1203;
    status_cek.error_time=1203;
  }

  if(SD.exists(PATH_NEWTIME)&&!SD.exists(PATH_TIMEZONE)){
    error_code=1204;
    status_cek.error_time=1204;
  }

  if(status_cek.wifi_mode){
    WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
    WiFi.mode(WIFI_STA);  //This line hides the viewing of ESP as wifi hotspot
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname("OWL_FINGER");
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    select_server();
  }

  else if(!status_cek.wifi_mode){
    SerialBT.begin(F("OWL_FINGER"));
  }

  for(int i=LED_PIN_NUMBER-1;i>=0;i--){
    digitalWrite(led_pin[i],LOW);
    //delay(150);
  }
  finger.led_control(0, 1, 1, 1);
  
  status_cek.cekrow=true;
  if (openDb("/sd/datakar.db", &db1))
        return;
  db_exec2(db1,"SELECT count(rowid) from finger_template");
  uint16_t capacity;
  finger.getTemplateCount(&capacity);    
  if(!(maxrow==capacity)){
    error_code=1103;
  }
  status_cek.cekrow=false;
  status_cek.validate=false;
  status_cek.validate_register=false;
  while (Serial.available()>0)
  {
    uint8_t byte=Serial.read();
    if (byte==10)
      break;
  }
}

bool regis(int id, const char * nik, const char * nama){
  if (openDb("/sd/datakar.db", &db1))
    return false;
  char *sql = "INSERT INTO finger_template VALUES (?, ?, ?)";
  int rc = sqlite3_prepare_v2(db1, sql, strlen(sql), &res, &tail);
  
  if (rc != SQLITE_OK) {
    DEBUG_PRINTF("ERROR preparing sql: %s\n", sqlite3_errmsg(db1));
    delay(200);
    sqlite3_close(db1);
    return false;
  }

  sqlite3_bind_int(res, 1,id); //id_sensor.toInt()
  sqlite3_bind_text(res, 2,nik, strlen(nik), SQLITE_STATIC); //nik.c_str()
  sqlite3_bind_text(res, 3, nama, strlen(nama), SQLITE_STATIC); //nama.c_str()
  
  if (sqlite3_step(res) != SQLITE_DONE) {
    DEBUG_PRINTF("ERROR executing stmt: %s\n", sqlite3_errmsg(db1));
    delay(500);
    sqlite3_close(db1);
    return false;
  }

  sqlite3_finalize(res);
  delay(200);
  sqlite3_close(db1);
  return true;
}

int16_t getFingerprintID() { //fid database search
  int16_t p = finger.getImage();
  //DEBUG_PRINTLN("Waiting for valid finger");
  //while (digitalRead(JARI)==LOW&&p!=FPM_OK&&!status_cek.loopscan){
  /*
  while (p!=FPM_OK&&!status_cek.loopscan){
    p = finger.getImage();
    delay(1);
  }
  */
  if(!status_cek.ledoff){
    finger.led_control(1, 1, 1, 1);
    status_cek.ledstatus=true;
  }
  if(p==FPM_OK)
    buzz(false,true);
  switch (p) {
    case FPM_NOFINGER:
      return -2;
    case FPM_OK:
      DEBUG_PRINTLN(F("Image taken"));
      break;
    default:
      DEBUG_PRINTLN(F("Unknown error"));
    return -1;
  }

  p = finger.image2Tz();
  switch (p) {
    case FPM_OK:
      DEBUG_PRINTLN(F("Image converted"));
      break;
    default:
      DEBUG_PRINTLN(F("Unknown error"));
      return -1;
  }

  uint16_t fid, score;
  p = finger.searchDatabase(&fid, &score);
  ////finger.led_off();
  //delay(50);
  if (p == FPM_OK) {
    DEBUG_PRINTLN(F("Found a print match!"));
  } 
  else {
    DEBUG_PRINTLN(F("Unknown error"));
    return -99;
  }
  DEBUG_PRINT(F("Found ID #")); DEBUG_PRINT(fid);
  DEBUG_PRINT(F(" with confidence of ")); DEBUG_PRINTLN(score);
  confidence=score;
  return fid;
}

void kedip(uint32_t * mills, uint8_t pin){
  digitalWrite(pin,HIGH);
  if ((millis() - *mills) > KEDIP){
    digitalWrite(pin,LOW); 
  }
  if ((millis() - *mills) > 1000){
    digitalWrite(pin,HIGH);
    *mills=millis(); 
  }
}

void ceklow(uint32_t * mills, uint8_t pin, int8_t * button_state){
    if(*button_state==OPEN){
      *button_state+=1;
      *mills=millis();
      DEBUG_PRINT("CLOSE BUTTON STATE = ");
      DEBUG_PRINT(*button_state);
      DEBUG_PRINT("PIN = ");
      DEBUG_PRINTLN(pin);
    }

    if((millis()-*mills> 20) && (*button_state<CLOSE_SHORT)){
      *button_state+=1;
      buzz(false,true);
      DEBUG_PRINT("CLOSE_SHORT BUTTON STATE = ");
      DEBUG_PRINT(*button_state);
      DEBUG_PRINT("PIN = ");
      DEBUG_PRINTLN(pin);
      //tampilkan("Ganti Mode",10,27,1,true,true,true);
      //submenu=true;
    }

    if((millis()-*mills > 3000) && (*button_state<CLOSE_LONG)){
      *button_state+=1;
      buzz(false,true);
      DEBUG_PRINT("CLOSE_LONG BUTTON STATE = ");
      DEBUG_PRINT(*button_state);
      DEBUG_PRINT("PIN = ");
      DEBUG_PRINTLN(pin);
      //tampilkan("Ganti Mode",10,27,1,true,true,true);
      //submenu=true;
    }

    if((millis()-*mills > 5000) && (*button_state<CLOSE_LONG2)){
      *button_state+=1;
      buzz(false,true);
      DEBUG_PRINT("CLOSE_LONG2 BUTTON STATE = ");
      DEBUG_PRINT(*button_state);
      DEBUG_PRINT("PIN = ");
      DEBUG_PRINTLN(pin);
      //submenu=false;
    }
}

void ganti_mode(){
  DEBUG_PRINTLN("ganti mode");
        if (status_cek.wifi_mode){
          SD.remove(GANTI_MODE);
          ESP.restart();
        }
        
        else {
          File file = SD.open(GANTI_MODE,"w");
          if (file){
            file.close();
            ESP.restart();
          }
          else
            led_gagal();
        }
}

void standby(){
  //led_gagal();
  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_36,1);
  esp_sleep_enable_ext1_wakeup(0x1000000004,ESP_EXT1_WAKEUP_ANY_HIGH); //gpio 36 & 2
  //esp_sleep_enable_ext1_wakeup(0x1400000000,ESP_EXT1_WAKEUP_ALL_LOW);
      ////finger.led_off();
      //finger.led_control(0, 1, 1, 1);
  esp_deep_sleep_start();
}


void cektombol(){
  
  if(digitalRead(BUTTON1)==HIGH){ //button 1 ditekan
    ceklow(&cek_waktu.button1,BUTTON1,&status_cek.button1);
  if(status_cek.button1>=CLOSE_SHORT)
      status_cek.submenu=true;
    switch (status_cek.button1)
    {
    case CLOSE_SHORT:
      status_cek.wifi_mode?tampilkan("GANTI MODE: BLUETOOTH",0,27,1,true,true):tampilkan("GANTI MODE: WIFI",0,27,1,true,true);
    break;
 
    case CLOSE_LONG:{
      tampilkan("UPLOAD",0,22,1,true,false);
      char ip [30];
      if(WiFi.status()==WL_CONNECTED){
        display.setCursor(0,32);             // Start at top-left corner
        display.print("IP: ");
        display.print(WiFi.localIP());
        display.display();
      }
      else{
        sprintf(ip,"WIFI NOT CONNECTED",WiFi.localIP());
        tampilkan(ip,0,32,1,false,true);
      }
    }
    break;

    case CLOSE_LONG2:{
      tampilkan("SLEEP",0,27,1,true,true);
    }
    break;
    
    default:
      break;
    }}
    
  if(digitalRead(BUTTON2)==HIGH){ //button 2 ditekan
    ceklow(&cek_waktu.button2,BUTTON2,&status_cek.button2);
    if(status_cek.button2>=CLOSE_SHORT)
      status_cek.submenu=true;
    switch (status_cek.button2)
    {
    case CLOSE_SHORT:{
      uint8_t inot=status_cek.inot+1;
      if (inot>status_cek.max_inout)
        inot=1;
      switch (inot)
        {
        case 1 :
          tampilkan("ABSEN MASUK",0,27,1,true,true);
          break;
        case 2 :
          tampilkan("ABSEN KELUAR",0,27,1,true,true);
          break;
        case 3 :
          tampilkan("ISTIRAHAT KELUAR",0,27,1,true,true);
          break;
        case 4 :
          tampilkan("ISTIRAHAT MASUK",0,27,1,true,true);
          break;
        case 5 :
          tampilkan("LEMBUR MASUK",0,27,1,true,true);
          break;
        case 6 :
          tampilkan("LEMBUR KELUAR",0,27,1,true,true);
          break;
        }
    }
    break;
 
    case CLOSE_LONG:
      tampilkan("VALIDATE GPS",0,27,1,true,true);
    break;

    case CLOSE_LONG2:{
      tampilkan("SLEEP",0,27,1,true,true);
    }
    break;
    
    default:
      break;
    }
  }
                                                                                        
  if(digitalRead(BUTTON1)==LOW && status_cek.button1>OPEN){
    DEBUG_PRINTLN("BUTTON1");
    status_cek.submenu=false;
    switch (status_cek.button1)
    {
      case CLOSE_LONG2:
        standby();
        break;
      
      case CLOSE_LONG:
        status_cek.upload=true;
        break;

      case CLOSE_SHORT:
        ganti_mode();
        break;

      default:
        break;
    }
    status_cek.button1=OPEN;
  }

  if(digitalRead(BUTTON2)==LOW && status_cek.button2>OPEN){
    DEBUG_PRINTLN("BUTTON2");
    status_cek.submenu=false;
    switch (status_cek.button2)
    {
      case CLOSE_LONG2:
        standby();
        break;

      case CLOSE_LONG:
        status_cek.validasi_gps=!status_cek.validasi_gps;
        break;

      case CLOSE_SHORT:
        status_cek.inot>=status_cek.max_inout?status_cek.inot=1:status_cek.inot++;
        break;
      
      default:
        break;
    }
    status_cek.button2=OPEN;
  }
}

bool backup(const char* path){
  File file = SD.open(path,FILE_READ);
  delay(100);
  if(file.available()>0){
    char newpath [100]="";
    strcat(newpath,path);
    strcat(newpath,"bc");//newpath will be /absensi.dbbc >> test.jpg >> test.jpgbc
    File file2 = SD.open(newpath,FILE_WRITE);
    if(file2){
      while(file.available()>0)
        file2.write(file.read());
    }
    file.close();
    file2.close();
    return true;
  }
  return false;
}


void display_control(){
  DateTime now = rtc.now();  
  if(now.second()!=cek_waktu.detik&&!status_cek.submenu){
    batcap=map(analogRead(BATSENSE),low,full,0,100);
    if(now.unixtime()>=cek_waktu.unix && (error_code!=1301)){
      tampilkan("delete......",0,30,1,true,true);
      tampilkan("mohon tunggu",0,40,1,true,true);
      bool perintah=false;
      perintah=backup("/absensi.db");
      if(perintah){
        openDb("/sd/absensi.db", &db1);
        perintah=db_exec2(db1,"Delete FROM absensi_log WHERE status_upload = 1");
      }
      if(perintah){
        File file = SD.open("/lastdelete", FILE_WRITE);
        if(file){
          file.print(now.unixtime());
          file.write('\n');
          file.close();
        }
      }
      else if(!perintah){
        tampilkan("delete gagal",0,30,1,true,false);
        tampilkan("manual delete",0,40,1,false,true);
        File file = SD.open("/lastdelete", FILE_WRITE);
        if(file){
          file.print(now.unixtime());
          file.write('\n');
          file.close();
        }
        //deleting=true;
        ESP.restart();
      }
      cek_waktu.unix=now.unixtime()+cek_waktu.sec_del;
    }
    //---------------------
    char currentDate [15];
    sprintf (currentDate, "%02d-%02d-%d", now.day(), now.month(), now.year()); //add leading zeros to the day and month
    tampilkan(currentDate,30,16,1,true,false);
    sprintf(currentDate,"%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    tampilkan(currentDate,15,27,2,false,false);
    if(!status_cek.wifi_mode){
      if(SerialBT.hasClient())
        tampilkan_bitmap(btcon,0,0,bt_width,bt_height,false,false);
      else if (!SerialBT.hasClient())
      tampilkan_bitmap(bterr,0,0,bt_width,bt_height,false,false);
    }
    else if(status_cek.wifi_mode){
      if( WiFi.status()== WL_CONNECTED){
      status_cek.con=true;
      tampilkan_bitmap(wificon,0,0,bt_width,bt_height,false,false);
      }
      else if (WiFi.status()!= WL_CONNECTED){
      tampilkan_bitmap(wifierr,0,0,bt_width,bt_height,false,false);
      status_cek.con=false;
      WiFi.reconnect();
      }
    }

    if(status_cek.validasi_gps){
      if(status_cek.gps_status)
        tampilkan_bitmap(gpson,18,0,bt_width,bt_height,false,false);
      else
        tampilkan_bitmap(gpserr,18,0,bt_width,bt_height,false,false);
    }

    if(error_code>0){
      tampilkan("ERR:",42,4,1,false,false);
      tampilkan("0",71,4,1,false,false,error_code);
    }

    if(batcap>=100)
    {
        tampilkan_bitmap(bat4,108,0,bat_width,bat_height,false,false);
      //tampilkan_bitmap(plugin_image,108,0,bat_width,bat_height,false,false);
      //batcap=100;
      }
    else if(batcap>=80)
      tampilkan_bitmap(bat4,108,0,bat_width,bat_height,false,false);
    else if(batcap>=60)
      tampilkan_bitmap(bat3,108,0,bat_width,bat_height,false,false);
    else if(batcap>=40)
      tampilkan_bitmap(bat2,108,0,bat_width,bat_height,false,false);
    else if (batcap>=20)
      tampilkan_bitmap(bat1,108,0,bat_width,bat_height,false,false);
    else if (batcap>=10)
      tampilkan_bitmap(bat0,108,0,bat_width,bat_height,false,false);
    //---------------------
    if (batcap<=5){
      DEBUG_PRINTLN("lowbatt");
      DEBUG_PRINTLN(batcap);
      DEBUG_PRINTLN(analogRead(BATSENSE));
      DEBUG_PRINTLN(low); 
      DEBUG_PRINTLN(full); 
      //led_gagal();
      tampilkan("Low battery....",0,30,1,true,true);
      led_gagal();
      delay(2000);
      standby();
      //esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,0);
      //esp_sleep_enable_ext1_wakeup(0x1400000000,ESP_EXT1_WAKEUP_ALL_LOW);
      ////finger.led_off();
      //finger.led_control(0, 1, 1, 1);
      //esp_deep_sleep_start();
    }

    if (rtc.getTemperature()>=60){
      tampilkan("Over Temperature....",0,30,1,true,true);
      led_gagal();
      delay(2000);
      standby();
      //esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,0);
      //esp_sleep_enable_ext1_wakeup(0x1400000000,ESP_EXT1_WAKEUP_ALL_LOW);
      ////finger.led_off();
      //finger.led_control(0, 1, 1, 1);
      //esp_deep_sleep_start();
    }
    switch (status_cek.inot)
        {
        case 1 :
          tampilkan("ABSEN MASUK",32,47,1,false,true);
          break;
        case 2 :
          tampilkan("ABSEN KELUAR",30,47,1,false,true);
          break;
        case 3 :
          tampilkan("ISTIRAHAT KELUAR",18,47,1,false,true);
          break;
        case 4 :
          tampilkan("ISTIRAHAT MASUK",20,47,1,false,true);
          break;
        case 5 :
          tampilkan("LEMBUR MASUK",31,47,1,false,true);
          break;
        case 6 :
          tampilkan("LEMBUR KELUAR",29,47,1,false,true);
          break;
        }
   cek_waktu.detik=now.second();
  }

  //---------------------------------------------------GPS CODE--------------------------------------------------
      if(status_cek.validasi_gps&&gps.charsProcessed()<10){
      if(status_cek.validasi_gps)
        error_code=1501;
    }

    while (Serial.available() > 0){
      if (gps.encode(Serial.read())){
        if (gps.location.isValid()&&gps.location.age()<=100000){
         if(error_code==1501)
            error_code=0;
         sprintf(lat,"%.6f",gps.location.lat());
         sprintf(lng,"%.6f",gps.location.lng());
         status_cek.gps_status=true;
       }
        else{  
          status_cek.gps_status=false;
          sprintf(lat,"%d",0);
          sprintf(lng,"%d",0);
        }
        if (gps.time.isValid()&&gps.date.isValid()&&gps.time.age()<=20000&&gps.date.age()<=20000) {
          if(status_cek.error_time==1202||status_cek.error_time==1204){
            DateTime gpsDate(gps.date.year(),gps.date.month(),gps.date.day(),gps.time.hour(),gps.time.minute(),gps.time.second());
            uint32_t unixgps=gpsDate.unixtime();
            error_code=status_cek.error_time=sinc_time(status_cek.error_time,SINC_FROM_GPS,&unixgps);
          }
        }
      }
   }

  if(status_cek.validasi_gps&&!status_cek.gps_status)
      openFinger=TUTUP_SENSOR;
  //---------------------------------------------------END GPS CODE---------------------------------------------

  //------------------LED Control/GPS VALIDATE/OPEN FINGER--------------------------------
    if (error_code>0)
      digitalWrite(ERROR,HIGH);
    else 
      digitalWrite(ERROR,LOW);
    if (!status_cek.con)
      kedip(&cek_waktu.last_time_led_con,CONNECTION);
    else
      digitalWrite(CONNECTION,HIGH);

    /*if (!status_cek.gps_status&&status_cek.validasi_gps)
    kedip(&cek_waktu.last_time_led_gps,GPS);
    else if(status_cek.gps_status&&status_cek.validasi_gps)
      digitalWrite(GPS,HIGH);
    else if(!status_cek.validasi_gps)
      digitalWrite(GPS,LOW);
    if (status_cek.inot==2)
      digitalWrite(OUT,HIGH);
    else
      digitalWrite(OUT,LOW);

    digitalWrite(WIFI,status_cek.wifi_mode);
    */
}

bool cekpin(){
  //File file = SD.open("/test.txt");
  File file = SD.open("/pin.txt");
  char pin [13];  
  int jmlcek=file.readBytesUntil('\n',pin,12);
  pin[jmlcek]='\0';
  file.close();
  file = SD.open("/test.txt");
  char cek [150];
  
  while(file.available()>0){
    jmlcek=file.readBytesUntil('\n',cek,149);
    cek[jmlcek]='\0';
    DEBUG_PRINTLN("loop ke");
    if(cek[0]=='?'){
      jmlcek=file.readBytesUntil('\n',cek,149);
      cek[jmlcek]='\0';
      DEBUG_PRINTLN("ini pinnya");
      DEBUG_PRINTLN(cek);
      if(strcmp(cek,pin)==0){
        file.close();
        return true;}
    }
  }

  DEBUG_PRINTLN("gk ada pin");
  SerialBT.println("{\"validate\":\"incorrect\"}");
  file.close();
  return false;
}

void empty(){
  //SD.remove("/ledoff");
  //SD.remove("/ssid");
  //SD.remove("/pass");
  //SD.remove("/ip");
  SD.remove("/max_flag");
  if(!SD.remove("/datakar.db"))
    SerialBT.println("{\"perintah\":\"hapus datakar gagal\"}");
  if(!SD.remove("/absensi.db"))
    SerialBT.println("{\"perintah\":\"hapus absensi gagal\"}");
  if(!SD.remove("/privilege.db"))
    SerialBT.println("{\"perintah\":\"hapus privilege gagal\"}");

  File file2 = SD.open("/pin.txt",FILE_WRITE);
  if(file2){
    file2.print("1234");
    file2.write('\n');
    file2.close();
  } 
  led_sukses();
  SerialBT.println("{\"perintah\":\"Reset selesai, device restart..\"}");
}

bool registrasi_finger(uint16_t id, const char * nama2) {
    DEBUG_PRINTLN(F("masuk di daftar finger"));
  int p = -1;
  while (p != FPM_OK) {
    p = finger.getImage();
    switch (p) {
      case FPM_OK:
          buzz(false,true);
      break;
      default:
      DEBUG_PRINTLN(F("Unknown error"));
      break;
    }
    delay(5);
  }

  p = finger.image2Tz(1);
  switch (p) {
    case FPM_OK:
    break;
    default:
      DEBUG_PRINTLN(F("Unknown error"));
      return false;
  }
  
  uint16_t fid, score;
  p = finger.searchDatabase(&fid, &score,1);
  if (p == FPM_OK){
    //led_gagal();
    char tampil [20];
    sprintf(tampil,"%d, %d %",fid,score);
    tampilkan("GAGAL!!",0,15,1,true,false);
    tampilkan("JARI SUDAH TERDAFTAR",0,25,1,false,false);
    tampilkan(tampil,0,40,1,false,true);
    delay(1500);
    status_cek.jari=true;
    return false;
  }

  //tampilkan_bitmap(fp_icon,0,0,owl_width,owl_height,true,false);
  tampilkan("ANGKAT JARI!!",0,20,1,true,false);
  tampilkan("TEMPEL ULANG JARI",0,31,1,false,true);
  p = -1;
  
  while (p !=FPM_NOFINGER) {
  p = finger.getImage();
  delay(1000);
  }
  
  tampilkan_bitmap(fp_icon,0,15,owl_width,owl_height,true,false);
  tampilkan("REGISTRASI",35,27,1,false,false);
  tampilkan(nama2,35,37,1,false,true);      
        
  p = -1;
  while (p != FPM_OK) {
    p = finger.getImage();
    switch (p) {
    case FPM_OK:
      buzz(false,true);
      break;
    default:
      DEBUG_PRINTLN(F("Unknown error"));
      break;
    }
  delay(5);
  }
 
  p = finger.image2Tz(2);
  switch (p) {
    case FPM_OK:
      break;
    default:
      DEBUG_PRINTLN(F("Unknown error"));
      return false;
  }
  
  p = finger.searchDatabase(&fid, &score,2);
  if (p == FPM_OK){
    //led_gagal();
    char tampil [20];
    sprintf(tampil,"%d, %d %",fid,score);
    tampilkan("GAGAL!!",0,15,1,true,false);
    tampilkan("JARI SUDAH TERDAFTAR",0,25,1,false,false);
    tampilkan(tampil,0,40,1,false,true);
    delay(1500);
    status_cek.jari=true;
    return false;
  }

  //tampilkan_bitmap(fp_icon,0,0,owl_width,owl_height,true,false);
  //tampilkan("ANGKAT JARI!!",0,20,1,true,false);
  //tampilkan("TEMPEL ULANG JARI",0,31,1,false,true);
  p = -1;

  // OK converted!
  DEBUG_PRINT(F("Creating model for #"));  DEBUG_PRINTLN(id);
  p = finger.createModel();
  if (p != FPM_OK) {
  SerialBT.println("{\"perintah\":\"combine template gagal\"}");
  DEBUG_PRINTLN(F("Unknown error"));
  return false;
  }

  DEBUG_PRINT(F("ID ")); 
  DEBUG_PRINTLN(id);
  p = finger.storeModel(id);
  if (p == FPM_OK) {
    DEBUG_PRINTLN(F("Stored!"));
  }
  else {
    DEBUG_PRINTLN(F("Unknown error"));
    return false;
  }   
return true;
  
/*DEBUG_PRINTLN(F("masuk di daftar finger"));
  int p = -1;
  while (p != FPM_OK) {
    p = finger.getImage();
    switch (p) {
      case FPM_OK:
          buzz(false,true);
      break;
      default:
      DEBUG_PRINTLN(F("Unknown error"));
      break;
    }
    delay(5);
  }

  p = finger.image2Tz(1);
  switch (p) {
    case FPM_OK:
    break;
    default:
      DEBUG_PRINTLN(F("Unknown error"));
      return false;
  }
  
  uint16_t fid, score;
  p = finger.searchDatabase(&fid, &score);
  if (p == FPM_OK){
    //led_gagal();
    tampilkan("GAGAL!!",0,20,1,true,false);
    tampilkan("JARI SUDAH TERDAFTAR",0,31,1,false,true);
    delay(1500);
    status_cek.jari=true;
    return false;
  }

  //tampilkan_bitmap(fp_icon,0,0,owl_width,owl_height,true,false);
  tampilkan("ANGKAT JARI!!",0,20,1,true,false);
  tampilkan("TEMPEL ULANG JARI",0,31,1,false,true);
  p = -1;
  
  while (p !=FPM_NOFINGER) {
  p = finger.getImage();
  delay(1000);
  }
  
  tampilkan_bitmap(fp_icon,0,15,owl_width,owl_height,true,false);
  tampilkan("REGISTRASI",35,27,1,false,false);
  tampilkan(nama2,35,37,1,false,true);      
  
  p = -1;
  while (p != FPM_OK) {
    p = finger.getImage();
    switch (p) {
    case FPM_OK:
      buzz(false,true);
      break;
    default:
      DEBUG_PRINTLN(F("Unknown error"));
      break;
    }
  delay(5);
  }
 
  p = finger.image2Tz(2);
  switch (p) {
    case FPM_OK:
      break;
    default:
      DEBUG_PRINTLN(F("Unknown error"));
      return false;
  }
  
  // OK converted!
  DEBUG_PRINT(F("Creating model for #"));  DEBUG_PRINTLN(id);
  p = finger.createModel();
  if (p != FPM_OK) {
  DEBUG_PRINTLN(F("Unknown error"));
  return false;
  }

  DEBUG_PRINT(F("ID ")); 
  DEBUG_PRINTLN(id);
  p = finger.storeModel(id);
  if (p == FPM_OK) {
    DEBUG_PRINTLN(F("Stored!"));
  }
  else {
    DEBUG_PRINTLN(F("Unknown error"));
    return false;
  }   
return true;
*/
}

bool get_free_id(int16_t * fid) {
  int16_t p = -1;
  for (int page = 0; page < (params.capacity / FPM_TEMPLATES_PER_PAGE) + 1; page++) {
    p = finger.getFreeIndex(page, fid);
    switch (p) {
      case FPM_OK:
        if (*fid != FPM_NOFREEINDEX) {
          DEBUG_PRINT("Free slot at ID ");
          DEBUG_PRINTLN(*fid);
          return true;
        }
      break;
      default:
        DEBUG_PRINTLN("Unknown error!");
        return false;
    }
    yield();
  }
  DEBUG_PRINTLN("No free slots!");
  return false;
}

bool move_template(uint16_t fid, uint8_t * buffer, uint16_t to_write) {
  int16_t p = finger.uploadModel();
  switch (p) {
    case FPM_OK:
      DEBUG_PRINTLN("Starting template upload");
    break;
        
    default:{
      DEBUG_PRINT("Unknown error upload: "); 
      DEBUG_PRINTLN(p);
      return false;
    }
  }
  yield();
  
  finger.writeRaw(buffer, to_write);
  uint16_t fid2, score2;
  p = finger.searchDatabase(&fid2, &score2);
  if(p==FPM_NOTFOUND){ 
    p = finger.storeModel(fid);
    switch (p) {
      case FPM_OK:{
        DEBUG_PRINT("Template moved to ID "); DEBUG_PRINTLN(fid);
        return true;
      }
      break;
      
      default:
        DEBUG_PRINT("Unknown error store: "); 
        DEBUG_PRINTLN(p);
        return false;
      break;
    }
  }
    
  if (p == FPM_OK) {
    DEBUG_PRINTLN(F("Jari sudah terdaftar!"));
    tampilkan("GAGAL!!",0,20,1,true,false);
    tampilkan("JARI SUDAH TERDAFTAR",0,31,1,false,true);
    delay(2000);
  }
  
  DEBUG_PRINT("P ");
  DEBUG_PRINTLN(p);
  return false;        //finger.deleteModel(fid);
}

void download(){
  DEBUG_PRINTLN(SerialBT.available());
  char nama [41];
  int wait_data =0;
  
  while(SerialBT.available()<53){
    delayMicroseconds(1);
    wait_data++;
    if(wait_data>100000)break;
  }
  
  int bite=SerialBT.readBytesUntil('\n',niklog,11);niklog[bite]='\0';
  bite=SerialBT.readBytesUntil('\n',nama,40);nama[bite]='\0';
  char nama2 [14];
  memcpy(nama2,nama,14);
  nama2[13]='\0';
  tampilkan_bitmap(fp_icon,0,15,owl_width,owl_height,true,false);
  tampilkan("TERIMA TEMPLATE",35,27,1,false,false);  
  tampilkan(nama2,35,37,1,false,true);
  char test[3];
  uint16_t i=0;
  uint16_t j=0;
  //int8_t result=0;
  uint8_t template_buffer [BUFF_SZ];
  wait_data=0;
  DEBUG_PRINTLN(SerialBT.available());
  
  while(i!=BUFF_SZ){
    
    if (SerialBT.available()>=2){
      j=SerialBT.readBytesUntil('\n',test,2);
      test[j]='\0';
      if(j>0 && test[0]!=' '&& test[1]!=' '){
        byte puluhan=test[0]<='9'?test[0]-48:test[0]-55;
        byte satuan=test[1]<='9'?test[1]-48:test[1]-55;
        template_buffer[i]=(puluhan*16)+satuan;                //masuk ke buffer  
        i++;
      }
    }
    
    delayMicroseconds(1);
    wait_data++;
    if(wait_data>500000)
      break;
  }

  DEBUG_PRINT("i ");
  DEBUG_PRINTLN(i);
  DEBUG_PRINT("j ");
  DEBUG_PRINTLN(j);
  char buang;
  while(SerialBT.available()>0){
    DEBUG_PRINTLN("ngabisin buffer gk jelas");
    buang=SerialBT.read();
    DEBUG_PRINTLN(buang);
    delay(1);
  }
  
  if(i==BUFF_SZ){
    int16_t idregis;
    if (!get_free_id(&idregis)){
      SerialBT.println("{\"result\":\"missing byte\"}");
      error_code=1102;
      return;
    }
    bool stat=move_template(idregis,template_buffer,BUFF_SZ);
    if(stat&&regis(idregis,niklog,nama)){
      SerialBT.println("{\"result\":\"oke\"}");
      led_sukses();
    }
    else {
      SerialBT.println("{\"result\":\"missing byte\"}");
      led_gagal();
    }
  }

  else if(i!=BUFF_SZ){
    SerialBT.println("{\"result\":\"missing byte\"}");
    DEBUG_PRINTLN("{\"result\":\"missing byte\"}");
    led_gagal();
  }
}

void bluetooth(){
  char ts[3];
  SerialBT.readBytesUntil('\n',ts,3); 
  switch (ts[0]) {
    case 'm':{//absen masuk
      openFinger = BUKA_SENSOR;
      status_cek.inot=1;
    }
    break;
    case 'k':{//absen keluar
      openFinger = BUKA_SENSOR;
      status_cek.inot=2;
    }
    break;
    case 'b':{//absen break
      openFinger = BUKA_SENSOR;
      status_cek.inot=3;
    }
    break;
    case 'B':{//absen break keluar
      openFinger = BUKA_SENSOR;
      status_cek.inot=4;
    }
    break;    
    case 'l':{//absen lembur
      openFinger = BUKA_SENSOR;
      status_cek.inot=5;
    }
    break;
    case 'L':{//absen lembur keluar
      openFinger = BUKA_SENSOR;
      status_cek.inot=6;
    }
    break;
    case 's'://stop absen
      openFinger = TUTUP_SENSOR;
    break;
    case '9':{//download data wifi 
      char ssid [30]={"nan"};
      char pass [30]={"nan"};
      char ip   [100]={"nan"};
      
      File file = SD.open("/ssid",FILE_READ);
        if (file && file.available()>0){
          int cek=file.readBytesUntil('\n',ssid,29);
          ssid[cek]='\0';
        }
      file.close();

      file = SD.open("/pass",FILE_READ);
        if (file && file.available()>0){
          int cek=file.readBytesUntil('\n',pass,29);
          pass[cek]='\0';
        }
      file.close();

      file = SD.open("/ip",FILE_READ);
        if (file && file.available()>0){
          int cek=file.readBytesUntil('\n',ip,99);
          ip[cek]='\0';
        }
      file.close();
      SerialBT.print("{\"ssid\":\"");
      SerialBT.print(ssid);
      SerialBT.print("\",\"pass\":\"");
      SerialBT.print(pass);
      SerialBT.print("\",\"ip\":\"");
      SerialBT.print(ip);
      SerialBT.print("\",\"interval\":\"");
      SerialBT.print(intervals);
      SerialBT.println("\"}");
    }
    break;
    
    case '%':{//akses database backdoor 
      idF=-1;
      char namadb [60];
      int ceeek=SerialBT.readBytesUntil('\n',namadb,59);
      namadb [ceeek]='\0';
      char sql [400];
      ceeek=SerialBT.readBytesUntil('\n',sql,399);
      sql [ceeek]='\0';
      if(strlen(namadb)>12&&strlen(sql)>15){
        openDb(namadb, &db1);
        db_exec2(db1,sql);
      }
    }
    break;
    
        case 'C':{//kalibrasi battery saat full
      //saat full> C <enter> okem <enter> SN
      //low voltage 3.3 volt > full 4.2 volt
      //0.8056 mv per value
      //low ke full 900mv

      char pass[10];
      int value=0;
      value=SerialBT.readBytesUntil('\n',pass,9);
      pass[value]='\0';
      analogRead(BATSENSE);
      if (strcmp("okem",pass)==0){
      value=0;
      for(int i=0;i<20;i++){
        value+=analogRead(BATSENSE);  
        delay(100);
      }
      value=value/20;
      full=value;
      low=(value*3.3)/4.2;
      SerialBT.println(low);
      SerialBT.println(full);
      File file=SD.open("/batsetting",FILE_WRITE);
        if (file){
          file.print(low);
          file.write('\n');
          file.print(full);
          file.write('\n');
        }
      file.close();
      }
      else
        SerialBT.println("password salah");
    }
    break;

    case 'c':{//cek gps 
      char cek[15];
      int value=0;
      value=SerialBT.readBytesUntil('\n',cek,(sizeof(cek)-1));
      cek[value]='\0';
      if (strcmp("gpsraw",cek)==0){
        delay(700);
        SerialBT.println("tunggu");
        while (Serial.available()>0)
        {
          SerialBT.write(Serial.read());
        }
      }
      if (strcmp("gps",cek)==0){
        SerialBT.print("{\"latitude\":\"");
        SerialBT.print(lat);
        SerialBT.print("\",\"longitude\":\"");
        SerialBT.print(lng);
        SerialBT.println("\"}");
      }
      if (strcmp("sn_inject",cek)==0){
        File file=SD.open("/sn_inject",FILE_WRITE);
        if (file && SerialBT.available()>0){
          char serial_number_inject [11];
          int cek;
          cek=SerialBT.readBytesUntil('\n',serial_number_inject,sizeof(serial_number_inject)-1);
          serial_number_inject[cek]='\0';
          file.print(serial_number_inject);
          file.write('\n');
        }
      file.close();
      }

      if(strcmp("otas",cek)==0&&ts[1]=='s'){
        File file=SD.open(PATH_OTA,FILE_WRITE);
        value=SerialBT.readBytesUntil('\n',cek,(sizeof(cek)-1));
        cek[value]='\0';
        int maxPacket=atoi(cek);
        //SerialBT.println(maxPacket);
        value=SerialBT.readBytesUntil('\n',cek,(sizeof(cek)-1));
        cek[value]='\0';
        int lenPacket=atoi(cek);
        //SerialBT.println(lenPacket);
        SerialBT.print("{\"ota\":\"ready\"}");
        SerialBT.print('\n');
        //Serial.println("starting to receive packet");
        tampilkan("DOWNLOAD FIRMWARE...",0,11,1,true,true);
        int packetCount=0;
        int lenCount=0;
        int ceksum=0;
        char ceksumIn [12];
        int ceksumInInt;          
        uint8_t test[2];
        uint8_t data[lenPacket];
        int timeOUT=0;
        SerialBT.setTimeout(2000);
        while(maxPacket>=100 && packetCount<maxPacket){
          while(!(SerialBT.available()>0)){
            delay(1);
            timeOUT++;
            if (timeOUT>=5000){
              DEBUG_PRINTLN("TIME OUT NO RESPONSE DARI RESPONDER");
              break;
            }
          }
                  
          if (timeOUT>=5000){
            DEBUG_PRINTLN("TIME OUT NO RESPONSE DARI RESPONDER");
            break;
          }     
          timeOUT=0;
          int j;
          j=SerialBT.readBytesUntil('\n',ceksumIn,sizeof(ceksumIn)-1);
          ceksumIn[j]='\0';
          ceksumInInt=atoi(ceksumIn);   
          ceksum=SerialBT.readBytes(data,sizeof(data));
          Serial.printf("Ceksum input %d ceksum output %d",ceksumInInt,ceksum);
          if (ceksumInInt==ceksum){
            packetCount++;
            file.write(data,sizeof(data));
            SerialBT.print("{\"ota\":\"next\"}");
            SerialBT.print('\n');
            Serial.print("{\"ota\":\"next\"}");
            Serial.print('\n');  
          }

          else{
            SerialBT.print("{\"ota\":\"retry\"}");
            SerialBT.print('\n');
            Serial.print("{\"ota\":\"retry\"}");
            Serial.print('\n');
            break;
          }

        }
        file.close();
        ESP.restart();
      }
    }
    break;
    
    case '$':{//cek analog read battery 
      for(int i=0;i<20;i++){
        SerialBT.println(analogRead(BATSENSE));
        delay(100);
      }
    }
    break;

    case 'y':{//kirim info 
      uint16_t capacity;
      finger.getTemplateCount(&capacity);
      status_cek.cekrow=true;
      idF=-3;
      if (openDb("/sd/absensi.db", &db1))
        return;
      db_exec2(db1,"SELECT MAX(rowid) From absensi_log");
      status_cek.cekrow=false;
      SerialBT.print("{\"sn\":\"");
      SerialBT.print(serial_number);
      SerialBT.print("\",\"name\":\"");
      SerialBT.print(PRODUCT_NAME);
      SerialBT.print("\",\"type\":\"");
      SerialBT.print(SENSOR_TYPE);
      SerialBT.print("\",\"log\":\"");
      SerialBT.print(maxrow);
      SerialBT.print("\",\"logmax\":\"");
      SerialBT.print("100000");
      SerialBT.print("\",\"capacity\":\"");
      SerialBT.print(capacity);
      SerialBT.print("\",\"capacitymax\":\"");
      SerialBT.print(params.capacity);
      SerialBT.print("\",\"temp\":\"");
      SerialBT.print(rtc.getTemperature());
      SerialBT.print("\",\"bat\":\"");
      SerialBT.print(batcap);
      SerialBT.print("\",\"firmware\":\"");
      SerialBT.print(FIRMWARE);
      SerialBT.print("\",\"hardware\":\"");
      SerialBT.print(HARDWARE_VER);
      SerialBT.print("\",\"mac\":\"");
      SerialBT.print(WiFi.macAddress());
      SerialBT.print("\",\"status_error\":\"");
      SerialBT.print(error_code);
      SerialBT.print("\",\"free_ram\":\"");
      SerialBT.print(esp_get_free_heap_size());
      SerialBT.print("\",\"min_ram\":\"");
      SerialBT.print(esp_get_minimum_free_heap_size());
      SerialBT.print("\",\"validasi_gps\":\"");
      SerialBT.print(status_cek.validasi_gps);
      SerialBT.println("\"}");
    }
    break;
    
    case '8':{//simpan template 
      if(status_cek.validate && privilege[8]!='1'){
        SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
      char buang;  
      while(SerialBT.available()>0){
          buang=SerialBT.read();
          DEBUG_PRINTLN(buang);
      }
        goto lewat;
      }
      download(); 
    }
    break;
                                  
    case '0':{//validate jari admin
      openFinger=TUTUP_SENSOR;
      idF=-1;
      tampilkan_bitmap(fp_icon,0,15,owl_width,owl_height,true,false);
      tampilkan("SCAN ADMIN",35,27,1,false,true);
      while(idF!=-99 && idF<0){
        ////finger.led_on();
        idF=getFingerprintID();
        delay(5);
      }
      status_cek.ledstatus=true;
                          ////finger.led_off();
      status_cek.validate=false;
      for(int i=0; i<sizeof(niklog);i++){
        niklog[i]='\0';
      }
      
      if (openDb("/sd/datakar.db", &db1)){//buang buffer than return jika gagal open db
        while (SerialBT.available()>0){
          char p=SerialBT.read();
        }
        return;
      }                   
      status_cek.validasi_awal=true;
      char select_nama [70]="Select * from finger_template where id_sensor = ";
      char id[5];
      sprintf(id,"%d",idF);
      strcat (select_nama,id);
      strcat (select_nama," LIMIT 1");
      db_exec2(db1,select_nama); 
      if(strlen(niklog)>0){
        if (openDb("/sd/privilege.db", &db1)) //buang buffer if gagal open db then return
          while (SerialBT.available()>0){
           char p=SerialBT.read();
          }
        status_cek.validasi_awal=false;
        status_cek.validate=false;
        char select_nama2 [70]="Select * from privilege where nik = ";
        strcat (select_nama2,"'");
        strcat (select_nama2,niklog);
        strcat (select_nama2,"'");
        strcat (select_nama2," LIMIT 1");
        db_exec2(db1,select_nama2);
      }

      for(int i=0; i<sizeof(niklog);i++){ //reset niklog history agar saat absen tidak ngaco
        niklog[i]='\0';
      }
      
      if (!status_cek.validate){
        SerialBT.println("{\"validatefinger\":\"incorrect\"}");
        tampilkan("Coba Lagi",0,27,1,true,true);
        led_gagal();
      }
    }
    break;
    lewat:;             
  }

}

void bluetooth_sd(){
  File file = SD.open("/test.txt");
  char ts[3];
  while(file.available()>0){           
    file.readBytesUntil('\n',ts,3); 

    switch (ts[0]) {
      
      case '?':{ //buang pin
        char buang;
        while(file.available()>0){
          buang=file.read();
        }
      }
      break;
      
      case 'v':{ //tambah admin
        DEBUG_PRINTLN ("masuk siniiiii");
        idF=-2;
        status_cek.tambah_admin=true;
        char nik [12];
        char privilege2[22];
        int8_t bite=file.readBytesUntil('\n',nik,12);
        nik[bite]='\0';
        DEBUG_PRINTLN(nik);
        bite=file.readBytesUntil('\n',privilege2,19);
        privilege2[bite]='\0';
        char buang;
        
        while(file.available()>0){
          buang=file.read();
        }
        
        if ((status_cek.validate || status_cek.validate_register) && privilege[0]!='1'){
          SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
          status_cek.tambah_admin=false;
          goto ulang;}
        
        char niksql [60]="Select * FROM finger_template where nik='";
        strcat(niksql,nik);
        strcat(niksql,"'");
        openDb("/sd/datakar.db", &db1);
        bool perintah=db_exec2(db1,niksql);
        delay(200);
        bool perintah2;
        
        if(idF>=0){
          openDb("/sd/privilege.db", &db1);
          char id[5]; sprintf(id,"%d",idF);
          char niksql2 [160]="Insert OR Replace INTO privilege (id_sensor,privilege,nik) VALUES (";
          strcat(niksql2,id);strcat(niksql2,",");
          strcat(niksql2,"'");strcat(niksql2,privilege2);strcat(niksql2,"'");strcat(niksql2,",");
          strcat(niksql2,"'");strcat(niksql2,nik);strcat(niksql2,"'");strcat(niksql2,")");     
          perintah2=db_exec2(db1,niksql2);
        }
        
        if(perintah&&perintah2){
          SerialBT.println("{\"perintah\":\"tambah admin berhasil\"}");
          status_cek.tambah_admin=false;
          led_sukses();
        }
    
        else{
          SerialBT.println("{\"perintah\":\"tambah admin gagal\"}");
          led_gagal();
        }
      }
      break;
                    
      case '1':{ //sec_delete / interval delete otomatis
        char detiik[25];
        int8_t bite=file.readBytesUntil('\n',detiik,24);detiik[bite]='\0';
        
        if ((status_cek.validate || status_cek.validate_register) && privilege[1]!='1'){
          SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
          goto ulang;
        }
                        
        File file2 = SD.open("/sec_del", FILE_WRITE);
        if(file){
          file2.println(detiik);
          file2.close();
          SerialBT.println("{\"perintah\":\"setting delete log berhasil\"}");
          led_sukses();
        }
        cek_waktu.sec_del=atoi(detiik);
      }
      break;
      
      case '2':{ //setting extension door....
        char extension[50];
        int8_t bite=file.readBytesUntil('\n',extension,49);extension[bite]='\0';
        
        if ((status_cek.validate || status_cek.validate_register) && privilege[2]!='1'){
          SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
          goto ulang;
          }
                            
        File file2 = SD.open("/extension", FILE_WRITE);
          if(file){
            file2.println(extension);
            file2.close();
            SerialBT.println("{\"perintah\":\"setting extension berhasil\"}");
            led_sukses();
          }
      }
      break;

      case '3':{ //setting default fingerprint > a,b,c,d >> a=ledoff? 1 b=loopscan?, c=validategps 1=on/0=off, d=maxflag(2-4)
        char cek[5];
        file.readBytesUntil('\n',cek,5);
        
        if ((status_cek.validate || status_cek.validate_register) && privilege[3]!='1'){
          SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
          goto ulang;
        }
      
        finger.setParam(5,cek[4]-48);//set security level

        if (cek[5]=='1'){//set multi scan
          File file2 = SD.open(MULTIPLE_SCAN,FILE_WRITE);
          file2.close();
        }
        else
        SD.remove(MULTIPLE_SCAN);

        if (cek[1]=='0'){
          File file2 =SD.open("/loopscan",FILE_WRITE);
          if (file2)file2.println(1);
          file2.close();
          }
        else SD.remove("/loopscan");

        if (cek[2]=='1'){
          File file2 =SD.open(GPS_MODE,FILE_WRITE);
          if (file2)file2.println(1);
          file2.close();
          }
        else SD.remove(GPS_MODE);
        int cek2=cek[3]-48;
        if ((cek2>=2) && (cek2<=6)){
          File file2 =SD.open("/max_flag",FILE_WRITE);
          if (file2){
            file2.print(cek2);
            file2.write('\n');
          }
          file2.close();
          status_cek.max_inout=cek2;
        }
        DEBUG_PRINTLN(cek2);
        DEBUG_PRINTLN(cek);
        DEBUG_PRINTLN(status_cek.max_inout);
        //status_cek.ledoff=SD.exists("/ledoff");
        status_cek.loopscan=SD.exists("/loopscan");
        status_cek.validasi_gps=SD.exists(GPS_MODE);
        SerialBT.println("{\"perintah\":\"setting berhasil\"}");
        led_sukses();
      }
      break;                 
      
      
      case '4':{ //delete log absen all
        
        if ((status_cek.validate || status_cek.validate_register) && privilege[4]!='1'){
          SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
          goto ulang;
        }
        tampilkan("delete......",0,20,1,true,false);
        tampilkan("mohon tunggu",0,30,1,false,true);
        bool perintah=SD.remove("/absensi.db");
        //openDb("/sd/absensi.db", &db1);
        //bool perintah=db_exec2(db1,"Delete FROM absensi_log");
        if(perintah){
          SerialBT.println("{\"perintah\":\"delete absensi log berhasil\"}");
          led_sukses();
        }
        else{
          SerialBT.println("{\"perintah\":\"delete absensi log gagal\"}");
          led_gagal();
        }
        ESP.restart();
      }
      break;                 
      
      case '5':{ //Delete data karyawan by NIK
        DEBUG_PRINTLN("nyasar ke 5");
        idF=-5;
        char nik [12];
        int8_t bite=file.readBytesUntil('\n',nik,11);nik[bite]='\0';
        
        if ((status_cek.validate || status_cek.validate_register) && privilege[5]!='1'){
        SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
        goto ulang;
        }

        bool perintah=false;
        bool perintah2=false;
        bool isIdSensor=false;
        char niksql [60]="";
        if(strlen(nik)>4){
          strcat(niksql,"select*from finger_template where nik='"); //delete by nik, semua id sensor di sensor finger
          strcat(niksql,nik);
          strcat(niksql,"'");
          //SerialBT.println(nik);
          //SerialBT.println(niksql);
        }
        
        else{
          isIdSensor=true;
          strcat(niksql,"select*from finger_template where id_sensor="); //delete id_sensor
          strcat(niksql,nik);
          //SerialBT.println(nik);
          //SerialBT.println(niksql);
        }

        openDb("/sd/datakar.db", &db1);
        perintah=db_exec2(db1,niksql);//execute select

        char del [8]="delete ";
        for(int i=0;i<7;i++){  //replace select to delete, untuk hemat memory
            niksql[i]=del[i];
        }

        openDb("/sd/datakar.db", &db1);
        //SerialBT.println(niksql);
        perintah2=db_exec2(db1,niksql);//execute delete
        
        if(perintah&&perintah2){
          isIdSensor?SerialBT.println("{\"perintah\":\"delete id sensor berhasil\"}"):SerialBT.println("{\"perintah\":\"delete datakar berhasil\"}");
          led_sukses();
        }
        
        else{
          isIdSensor?SerialBT.println("{\"perintah\":\"delete id sensor gagal\"}"):SerialBT.println("{\"perintah\":\"delete datakar gagal\"}");
          led_gagal();
        }
      }
      break;
      
      case '6':{ //Kirim template finger by NIK
        DEBUG_PRINTLN("nyasar ke 6");
        idF=-6;
        char nik [12];
        int8_t bite=file.readBytesUntil('\n',nik,11);nik[bite]='\0';

        if ((status_cek.validate || status_cek.validate_register) && privilege[6]!='1'){
          SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
          goto ulang;
        }
        tampilkan_bitmap(fp_icon,0,15,owl_width,owl_height,true,false);
        tampilkan("KIRIM TEMPLATE",35,27,1,false,true);
        openDb("/sd/datakar.db", &db1);
        char niksql [60]="Select * FROM finger_template WHERE nik='";
        strcat(niksql,nik);
        strcat(niksql,"'");
        bool perintah=db_exec2(db1,niksql);
        if(perintah){
          led_sukses();
        }
        else{
          SerialBT.println("{\"perintah\":\"kirim template gagal\"}");
          led_gagal();
        }
      }
      break;
                        
      case '7':{//kirim template all 
        if ((status_cek.validate || status_cek.validate_register) && privilege[7]!='1'){
          SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
          goto ulang;
        }
        tampilkan_bitmap(fp_icon,0,15,owl_width,owl_height,true,false);
        tampilkan("KIRIM TEMPLATE",35,27,1,false,true);
        idF=-7;
        openDb("/sd/datakar.db", &db1);
        bool perintah=db_exec2(db1,"Select * FROM finger_template");
        if(perintah){
          led_sukses();
        }
        else{
          SerialBT.println("{\"perintah\":\"kirim template gagal\"}");
          led_gagal();
        }
      }
      break;
                            
      case '8':{//terima template //>>> KOSONG GK kepake
        
        if ((status_cek.validate || status_cek.validate_register) && privilege[8]!='1'){
          SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
          goto ulang;
        }
      }
      break;

      case 't':{//setting jam YYYY:MM:DD hh:mm:ss
        String anything = file.readStringUntil('\n');
        
        if ((status_cek.validate || status_cek.validate_register) && privilege[9]!='1'){
          SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
          goto ulang;
        }    
        rtc.adjust(DateTime(anything.substring(0,4).toInt(),anything.substring(5,7).toInt(),anything.substring(8,10).toInt(),anything.substring(11,13).toInt(),anything.substring(14,16).toInt(),anything.substring(17,19).toInt()));  
        SerialBT.println("{\"perintah\":\"jam berhasil diganti\"}");
        led_sukses();
        SD.remove(PATH_TIMEZONE);
        File file2=SD.open(PATH_NEWTIME,FILE_WRITE);
        file2.close();
        status_cek.error_time=1204;
      }
      break;
                         
      case 'x':{//ganti PIN
        char pinnew [12];
        int cek=file.readBytesUntil('\n',pinnew,11);
        pinnew[cek]='\0';
        File file2 = SD.open("/pin.txt",FILE_WRITE);
        if (file2){
          file2.print(pinnew);
          file2.write('\n');
          led_sukses();
          SerialBT.println("{\"perintah\":\"ganti pin berhasil\"}");
        }
        else {
          led_gagal();
          SerialBT.println("{\"perintah\":\"ganti pin gagal\"}");
        }
        file2.close();
      }
      break;
                                       
      case 'u':{//setting interval upload 
        char waktu [10];
        int i=file.readBytesUntil('\n',waktu,9);
        waktu [i]='\0';
        
        if ((status_cek.validate || status_cek.validate_register) && privilege[10]!='1'){
          SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
          goto ulang;
        }

        File file2 = SD.open("/intervals",FILE_WRITE);
        if (file2){
          file2.println(waktu);
          led_sukses();
          SerialBT.println("{\"perintah\":\"interval upload berhasil diganti\"}");
        }
        else {
          led_gagal();
          SerialBT.println("{\"perintah\":\"interval upload gagal diganti\"}");
        }
        file2.close();
      }
      break;
                     
      case 'r':{//register
        char nik [12]; 
        char nama[41];
        char nama2[14];
        int8_t bite=file.readBytesUntil('\n',nik,11);nik[bite]='\0';
        bite=file.readBytesUntil('\n',nama,40);nama[bite]='\0';
        
        memcpy(nama2,nama,14);
        nama2[13]='\0';

        if (bite<3){
          file.close();
          return;
        }
        
        if ((status_cek.validate || status_cek.validate_register) && privilege[11]!='1'){
          SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
          goto ulang;
        }
        tampilkan_bitmap(fp_icon,0,15,owl_width,owl_height,true,false);
        tampilkan("REGISTRASI",35,27,1,false,false);
        tampilkan(nama2,35,37,1,false,true);

        cek_waktu.register_timeout=millis();
        status_cek.validate_register=true;
        int16_t idregis;
        file.close();
        if (!get_free_id(&idregis)){
          error_code=1102;
          tampilkan("0",0,0,1,true,false,error_code);
          tampilkan("FP Penuh",0,11,1,false,true);
          delay(500);
          SerialBT.println("{\"perintah\":\"registrasi gagal\"}");
          return;
        }
        
        DEBUG_PRINT(idregis);
        DEBUG_PRINT("ID sensor : ");
        DEBUG_PRINTLN(idregis);
        DEBUG_PRINT("Nama Karyawan : ");
        DEBUG_PRINTLN(nama);           
        DEBUG_PRINT(F("nik krywn"));
        DEBUG_PRINTLN(nik);

        if(registrasi_finger(idregis,nama2)==true){
          if(regis(idregis, nik, nama)){
            SerialBT.println("{\"perintah\":\"registrasi berhasil\"}");
            led_sukses();
          }
          else{//delete ke sensor jika kasus berhasil simpan sensor gagal simpan jari
            delete_template(idregis);
            SerialBT.println("{\"perintah\":\"registrasi db gagal\"}");
            led_gagal();
          }
        }
        else{
          //status_cek.jari?SerialBT.println("{\"perintah\":\"gagal jari sudah terdaftar\"}"):SerialBT.println("{\"perintah\":\"registrasi gagal\"}");
          SerialBT.println("{\"perintah\":\"registrasi gagal\"}");
          status_cek.jari=false;
          tampilkan("Coba Lagi",0,27,1,true,true);
          led_gagal();
        }
      }
      break;
      
      case 'n':{//FACTORY RESET
        
        if ((status_cek.validate || status_cek.validate_register) && privilege[12]!='1'){
          SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
          goto ulang;
        }
        tampilkan("RESET DEVICE...",0,11,1,true,true);
        finger.emptyDatabase();
        SerialBT.println("{\"perintah\":\"hapus sidik jari berhasil\"}");
        empty();
        delay(100);
        file.close();
        ESP.restart();
      }
      break;
      
      case 'w':
      case 'p':
      case 'i':{//setting wifi/pass/server
        char wifi [120];
        delay(100);
        int8_t bite=file.readBytesUntil('\n',wifi,119);
        wifi[bite]='\0';
        bool perintah=false;
        if(ts[0]=='w'){
          if ((status_cek.validate || status_cek.validate_register) && privilege[13]!='1'){
            SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
            goto ulang;
          }
          File file2 = SD.open("/ssid",FILE_WRITE);
          if (file2){
            file2.print(wifi);
            file2.write('\n');
            perintah=true;
          }
          file2.close();
        }
        
        else if(ts[0]=='p'){
          if ((status_cek.validate || status_cek.validate_register) && privilege[13]!='1'){
            SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
            goto ulang;
          }
          File file2 = SD.open("/pass",FILE_WRITE);
          if (file2){
            file2.print(wifi);
            file2.write('\n');
            perintah=true;
          }
          file2.close();        
        }
        
        else if(ts[0]=='i'){
          if ((status_cek.validate || status_cek.validate_register) && privilege[14]!='1'){
            SerialBT.println("{\"perintah\":\"admin tidak ada hak\"}");led_gagal();
            goto ulang;
          }
          File file2 = SD.open("/ip",FILE_WRITE);
          if (file2){
            file2.print(wifi);
            file2.write('\n');
            perintah=true;
          }
          file2.close();        
        }  

        if (!perintah) {
          if(ts[0]=='p')SerialBT.println("{\"perintah\":\"setting password gagal\"}");
          else if (ts[0]=='w')SerialBT.println("{\"perintah\":\"setting ssid gagal\"}");
          else if (ts[0]=='i')SerialBT.println("{\"perintah\":\"setting server gagal\"}");
          led_gagal();
        }
        
        else if(perintah){
          led_sukses();
          if (ts[0]=='w')SerialBT.println("{\"perintah\":\"wifi berhasil diganti\"}");
          if (ts[0]=='i')SerialBT.println("{\"perintah\":\"server berhasil diganti\"}");
        }
      }
      break;

      ulang:;
    }
  }
  file.close();
}

void simpansd(){
  DEBUG_PRINTLN("Writing file");
  File file = SD.open("/test.txt", FILE_WRITE);
  
  if(!file){
    DEBUG_PRINTLN("Failed to open file for writing");
    return;
  }
  
  while(SerialBT.available()>0){
    file.write(SerialBT.read());
    DEBUG_PRINTLN("File written");
    delay(1);
  }

  file.close();
  
  if(status_cek.validate){
    bluetooth_sd();
    status_cek.validate=false;
    for(int i=0;i<sizeof(privilege);i++){
      privilege[i]='\0';
    }
    return;
  }

  if(status_cek.validate_register&&(millis()-cek_waktu.register_timeout<TIMEOUT)){
    for(int i=0;i<sizeof(privilege);i++){
      privilege[i]='\0';
    }
    privilege[11]='1'; //PRIVILEGE REGISTER
    bluetooth_sd();
    return;
  }
  
  if(millis()-cek_waktu.register_timeout>TIMEOUT)
    status_cek.validate_register=false;
    
  if(!status_cek.validate && cekpin()){
    DEBUG_PRINTLN("Reading file karna sudah verified by PIN");
    SerialBT.println("{\"validate\":\"correct\"}");
    DEBUG_PRINTLN("Read from file: "); 
    bluetooth_sd();
  }
}

void absensilog(){
  DEBUG_PRINTLN(F("Sebelum Open log"));
  DateTime now = rtc.now();
  status_cek.cekrow=true;
  if (openDb("/sd/absensi.db", &db1))
   return;
  db_exec2(db1,"SELECT MAX(rowid) From absensi_log");
  status_cek.cekrow=false;
  if(maxrow>=100000){
    error_code=3201;
    tampilkan("0",0,20,1,true,false,error_code);
    tampilkan("log penuh",0,31,1,false,true);
    led_gagal();
    delay(1500);
    return;
  }
  
  if (openDb("/sd/absensi.db", &db1))
   return;

  DEBUG_PRINTLN(F("Setelah Open log")); 
  //displayFreeHeap();
  
  char *sql = "INSERT INTO absensi_log (id_sensor,nik,jam,status,lat,lng) VALUES (?, ?, ?, ?, ?, ?)"; 
  int rc = sqlite3_prepare_v2(db1, sql, strlen(sql), &res, &tail);
  
  if (rc != SQLITE_OK) {
     DEBUG_PRINTF("ERROR preparing sql: %s\n", sqlite3_errmsg(db1));
     sqlite3_close(db1);
     return;
  }
   
  sqlite3_bind_int(res, 1,idF); //id_sensor.toInt()
  
  sqlite3_bind_text(res, 2,niklog , strlen(niklog), SQLITE_STATIC); //nik.c_str()
  
  char fullstring2 [25];
  sprintf (fullstring2, "%d-%02d-%02d %02d:%02d:%02d", now.year(),now.month(),now.day(),now.hour(),now.minute(),now.second());
  sqlite3_bind_text(res, 3, fullstring2, strlen(fullstring2), SQLITE_STATIC); //nama.c_str()

  sqlite3_bind_int(res, 4,status_cek.inot); //nik.c_str()
      
  sqlite3_bind_text(res, 5, lat, strlen(lat), SQLITE_STATIC); //nama.c_str()
      
  sqlite3_bind_text(res, 6, lng, strlen(lng), SQLITE_STATIC); //nama.c_str()
      
  if (sqlite3_step(res) != SQLITE_DONE) {
    DEBUG_PRINTF("ERROR executing stmt: %s\n", sqlite3_errmsg(db1));
    sqlite3_close(db1);
    return;
  }
  
  sqlite3_finalize(res);
  tampilkan(namalog,0,20,1,true,false);
  tampilkan("0",0,30,1,false,false,idF);
  tampilkan("0",0,40,1,false,true,confidence);
  led_sukses();
  delay(700);
  sqlite3_close(db1);
  return;
}



void select_nama(){
  DEBUG_PRINTLN(idF);
  if (openDb("/sd/datakar.db", &db1))
    return ;
  char select_nama [70]="Select * from finger_template where id_sensor = ";
  char id[5];
  sprintf(id,"%d",idF);
  strcat (select_nama,id);
  strcat (select_nama," LIMIT 1");
  db_exec2(db1,select_nama);
}

void multiAbsent(){
  vTaskDelay(50/portTICK_PERIOD_MS);
  idF = getFingerprintID();
  if(idF==-2||idF==-1)
    return;
  if(idF >= 0){
    DEBUG_PRINTLN(idF);
    select_nama();
    absensilog();
    cek_waktu.last_time_upload=millis();
  /*  discountinue
    if(status_cek.wifi_mode&&status_cek.con){//buka pintu
      char extension [50];
      File file = SD.open("/extension", FILE_READ);
      if(file){
        int cek=0;
        if(file.available()>0)
        cek=file.readBytesUntil('\n',extension,49);
        extension[cek]='\0';
        strcat(extension,"/sesame");
        file.close();
      }
      if(strlen(extension)>=9){
        HTTPClient http;
        http.begin(extension);
        int httpResponseCode = http.GET();
      //display extension error
        DEBUG_PRINTLN("responsecode");
        DEBUG_PRINTLN(httpResponseCode);
        http.end();
      }
    }
  */

  }
  if(idF==-99){
    tampilkan("Coba Lagi",0,27,1,true,true);
    led_gagal();
    delay(500);
  }
}

bool uploadlog(){
  if (openDb("/sd/absensi.db", &db1)){
    error_code=4102;
    return false; 
  }
  int rc = db_upload(db1,"SELECT * FROM absensi_log WHERE status_upload = 0 LIMIT 1");
  
  if (rc != SQLITE_OK) {
    sqlite3_close(db1);
    error_code=4103;
    //Serial.printf("Gagal buka absen, free heap = %d\n",esp_get_free_heap_size());
    return false;
  }
  
  sqlite3_close(db1);

  if(status_cek.statup==true&& (rowid!=rowid2)){
  if (openDb("/sd/absensi.db", &db1)){
    error_code=4104;
    return false;
    }
  //Serial.printf("berhasil upload absen, free heap = %d\n",esp_get_free_heap_size());
  DEBUG_PRINTLN(rowid);
  //DEBUG_PRINTLN(rowid2);
  status_cek.statup=false;
  //strcpy(rowid2,rowid);
  char sql[70]="UPDATE absensi_log SET status_upload = 1 WHERE rowid = ";
  char rowidchar[13];
  sprintf(rowidchar,"%d",rowid);
  strcat(sql,rowidchar);
  int rc = db_upload(db1,sql);
  //Serial.printf("update status, free heap = %d\n",esp_get_free_heap_size());
    if (rc != SQLITE_OK) {
      error_code=4105;
      sqlite3_close(db1);
      return false;
    }
  sqlite3_close(db1);
  rowid2=rowid;
  error_code=0;
  return true;
  }

  else if(status_cek.statup==false){
    status_cek.upload=false;
    return false;
  }
  return false;
}

void cek_tombol(){
  pinMode(HIJAU,INPUT_PULLDOWN);
  
  if(digitalRead(HIJAU)==HIGH){
    pinMode(HIJAU,OUTPUT);
    digitalWrite(HIJAU,HIGH);
    delay(5000);
  }

  pinMode(HIJAU,OUTPUT);
  digitalWrite(HIJAU,LOW);
}

void loop() {
  cek_tombol();
  if (!status_cek.wifi_mode){
    //digitalWrite(WIFI,LOW);
    SerialBT.hasClient()?status_cek.con=true:status_cek.con=false;
    if(!status_cek.con)
      openFinger=TUTUP_SENSOR;
    if (SerialBT.available()>0){
      char firstchar = SerialBT.peek();//DEBUG_PRINT("karakter pertama = ");
      if (firstchar!='r') //time out register
        status_cek.validate_register=false;
      if(firstchar!='L' && firstchar!='l' && firstchar!='c' && firstchar!='C' && firstchar!='B' && firstchar!='b' && firstchar!='m' && firstchar!='k' && firstchar!='s' && firstchar!='9'&& firstchar!='%'&& firstchar!='$'&& firstchar!='0'&& firstchar!='8'&& firstchar!='y') //yg bisa di akses gk pake password cuma ini?? 
        simpansd();
      else
        bluetooth();
    }
  }

  if (status_cek.wifi_mode){
    WiFi.status()==WL_CONNECTED?status_cek.con=true:status_cek.con=false;
    /*
    if(!status_cek.con)
      kedip(&cek_waktu.last_time_led_con,CONNECTION);
    */
    if ((status_cek.upload==true || (millis() - cek_waktu.last_time_upload) > intervals*10*1000)){
      //digitalWrite(UPLOAD,HIGH);
      status_cek.status_upload=false;
      status_cek.upload=false;
      if(WiFi.status() != WL_CONNECTED){
          WiFi.reconnect();
          int i=0;
          while (WiFi.status() != WL_CONNECTED)
          {    
            delay(100);
            i++;
            if(i==30)
              break;
          }
        }

      if (WiFi.status() == WL_CONNECTED){
        tampilkan_bitmap(cloud,0,15,owl_height,owl_width,true,false);
        tampilkan("UPLOAD DATA",35,27,1,false,true);
        openFinger=TUTUP_SENSOR;
        check_update(batcap=map(analogRead(BATSENSE),low,full,0,100),rtc.getTemperature(),serial_number);
        //finger.led_control(0, 1, 1, 1);
        //finger.led_off();
        DEBUG_PRINTLN("UPLOAD DATA KE SERVER");
        while(uploadlog());
      }
      cek_waktu.last_time_upload=millis();
      if(status_cek.upload_data==false && WiFi.status() == WL_CONNECTED)
        tampilkan("TIDAK ADA DATA",0,31,1,true,true);
      if(WiFi.status() != WL_CONNECTED)
        tampilkan("WIFI TIDAK TERKONEKSI",0,31,1,true,true);
      status_cek.upload_data=false;
      status_cek.status_upload?led_sukses():led_gagal();
      //digitalWrite(UPLOAD,LOW);
    }
    openFinger=BUKA_SENSOR;        
  }

  if(openFinger == BUKA_SENSOR&& error_code!=1101){//on lampu
    if(!status_cek.ledstatus&&!status_cek.ledoff){
      finger.led_control(1, 1, 1, 1);
      status_cek.ledstatus=true;
    }
  }
  
  else if(openFinger == TUTUP_SENSOR&& error_code!=1101){//off lampu
    if(status_cek.ledstatus){
      finger.led_control(0, 1, 1, 1);
      status_cek.ledstatus=false;
    }
  }
    //sync gps serial buffer
  display_control();
  cektombol();
  if(openFinger == BUKA_SENSOR && (digitalRead(JARI)==LOW||status_cek.loopscan) && error_code!=1101 && error_code!=1103) //scan flexibel (loopscan / trigger scan)
    multiAbsent();
}
