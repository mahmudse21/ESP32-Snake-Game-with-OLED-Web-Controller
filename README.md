# 🐍 ESP32 Snake Game with OLED & Web Controller

Bu loyiha ESP32 mikrokontrolleri va 128x64 OLED displey yordamida yasalgan klassik "Snake" o‘yini hisoblanadi. O‘yinni telefon yoki kompyuterdan Wi-Fi orqali boshqarish mumkin.

## ⚙️ Texnik Talablar

- **ESP32** (WiFi bilan)
- **OLED Display** (128x64, I2C interfeysli, SSD1306)
- **Arduino IDE** (ESP32 board manager o‘rnatilgan)

## 📷 Loyiha Ko‘rinishi

![OLED Snake Game](https://your-image-url-if-any.com)

## 🔧 O‘rnatish

1. **ESP32 board manager** ni Arduino IDE ga qo‘shing:  
   File > Preferences > "Additional Board URLs"ga quyidagini qo‘shing:  

https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json


2. **Kutubxonalarni o‘rnating**:
- `WiFi.h` (standart ESP32 bilan keladi)
- `WebServer.h`
- `U8g2lib.h` (OLED uchun)

3. **Kodni ESP32 ga yuklang.**

4. Qurilma o‘zining Wi-Fi tarmog‘ini yaratadi:  
- **SSID**: `ESP32_Snake_Game`  
- **Password**: `snakegame123`

5. Tarmoqqa ulanib, brauzerdan quyidagiga o‘ting:  

http://192.168.4.1


## 🎮 Boshqaruv

- Web interfeys orqali UP, DOWN, LEFT, RIGHT tugmalari bilan harakatlantirasiz.
- Kompyuter klaviaturasidan ham foydalanish mumkin:
- ⬆️ `ArrowUp`
- ⬇️ `ArrowDown`
- ⬅️ `ArrowLeft`
- ➡️ `ArrowRight`
- `Spacebar` — o‘yinni reset qilish

## 🕹️ O‘yin Qoidalari

- Oziqni yeb ilonni o‘stiring
- Devorga yoki o‘zingizga urilmaslikka harakat qiling
- Har safar oziq yeyilganda tezlik ortadi

## 📁 Fayllar

- `main.ino`: ESP32 mikrokontroller kodi
- OLED displeyda o‘yinni chiqarish va WebServer orqali masofadan boshqaruv mavjud

## 📜 Litsenziya

Ushbu loyiha ochiq manbali va ta'limiy maqsadlarda foydalanish uchun mo‘ljallangan. Istalgan maqsadda foydalanishingiz mumkin, ammo mualliflikni ko‘rsatish tavsiya etiladi.

---

🎉 **Yaratildi: ESP32 va OLED asosida o‘yin loyihasi!**  
🚀 **Muallif:** [Sizning ismingiz yoki GitHub profilingiz]

