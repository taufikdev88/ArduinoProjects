/***************************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C
Code    : Pengaturan kecepatan motor DC (motor roda) dengan PWM
*****************************************************************/

/*
Jika menggunakan Code::Blocks sebagai editor:
untuk optimasi supaya file hex hasil kompilasi menjadi kecil (fungsi yang tidak dipanggil tidak dimasukkan ke dalam
file hex), maka dilakukan pengaturan proses compile dan linker program sbb:
1. copy option berikut pada compiler setting    (Menu Settings,Compiler, Compiler settings, Other options:)
   -ffunction-sections
2. copy option berikut pada linker setting      (Menu Settings,Compiler, Linker settings, Other linker options:)
   -Wl,-gc-sections
*/

#include "myModule.h"                   // edit module yang akan digunakan pada file ini
#include "../AdRoiT_Files/mySystem.h"

int main(void)
{
    SystemInit();       // inisialisasi sistem (wajib bagi semua project)
                        // detail dari  modul yang diaktifkan pada setiap project bisa dilihat pada file "myModule.h"
    lcd.Hapus();

    lcd.TulisKanan (0, "Push Button : x ");
    lcd.TulisKiri  (1, "Atur Kecp. Motor");
    buzzer.Nada1();

    // pengaturan motor DC dapat diakses melalui obyek roda yang dideklarasikan pada file "myMotor.h"
    // pengaturan kecepatan dengan PWM diakses dengan fungsi SpeedPWM(x,y)
    // dimana x = data kecepatan motor kiri dan y = data kecepatan motor kanan
    // masing-masing nilai x dan y mempunyai range antara -400 sampai dengan 400
    // tanda positif menghasilkan putaran maju, sedangkan nilai negatif menghasilkan putaran mundur
    // sedangkan nilai 0 (nol) membuat motor berhenti

    while(1)
    {   if(Tombol1)                             // jika push button 1 ditekan
        {   roda.SpeedPWM(100,100);             // kecepatan motor 1/4 dari kecepatan maksimal ke arah depan
            lcd.GotoXY(14,0);   lcd.Data('1');
            lcd.Tulis(1, "Robot Maju   25%");
        }
        else if(Tombol2)                        // jika push button 2 ditekan
        {   roda.SpeedPWM(400,400);             // kecepatan motor  maksimal ke arah depan
            lcd.GotoXY(14,0);   lcd.Data('2');
            lcd.Tulis(1, "Robot Maju  100%");
        }
        else if(Tombol3)                        // jika push button 3 ditekan
        {   roda.SpeedPWM(-100,-100);           // kecepatan motor 1/4 dari kecepatan maksimal ke arah belakang
            lcd.GotoXY(14,0);   lcd.Data('3');
            lcd.Tulis(1, "Robot Mundur 25%");
        }
        else if(Tombol4)                        // jika push button 4 ditekan
        {   roda.SpeedPWM(-400,-400);           // kecepatan motor  maksimal ke arah belakang
            lcd.GotoXY(14,0);   lcd.Data('4');
            lcd.Tulis(1, "Robot Mundur100%");
        }
        else                                    // jika tidak ada push button yang ditekan
        {   roda.SpeedPWM(0,0);                 // motor berhenti berputar dan dalam kondisi ngerem (brake)
            lcd.GotoXY(15,0);   lcd.Data('x');
            lcd.Tulis(1, "Robot Stop      ");
        }
    }
}
