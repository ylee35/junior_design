#include <LiquidCrystal_I2C.h>

//define I2C address....
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();

}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bad Cats In Da");
  lcd.setCursor(0, 1);
  lcd.print("HOUSEEEE ^-^");
  delay(1000);

  lcd.clear();
  lcd.setCursor(2 , 0);
  lcd.print("Yay");
  delay(1000);


}