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
    _delay_ms(500);
    lcd.HapusKanan(0);
    lcd.HapusKiri(1);
    //            0123456789012345
    lcd.Tulis(1, "MKi=    MKa=    ");

    // pengaturan motor DC dapat diakses melalui obyek roda yang dideklarasikan pada file "myMotor.h"
    // pengaturan kecepatan dengan PID diakses dengan fungsi SpeedPID(x,y)
    // dimana x = data kecepatan motor kiri dan y = data kecepatan motor kanan
    // masing-masing nilai x dan y mempunyai range antara -20 sampai dengan 20
    // tanda positif menghasilkan putaran maju, sedangkan nilai negatif menghasilkan putaran mundur
    // sedangkan nilai 0 (nol) membuat motor berhenti

    // fungsi SpeedPID baru aktif ketika opsi UsePIDmotor =1 (lihat pengaktifannya pada file "myModeule.h")

    while(1)
    {   if(Tombol1)                             // jika push button 1 ditekan
        {   roda.SpeedPID(5,5);                 // kecepatan motor 1/4 dari kecepatan maksimal ke arah depan
            lcd.Tulis(0, "Robot Maju   25%");
        }
        else if(Tombol2)                        // jika push button 2 ditekan
        {   roda.SpeedPID(20,20);               // kecepatan motor  maksimal ke arah depan
            lcd.Tulis(0, "Robot Maju  100%");
        }
        else if(Tombol3)                        // jika push button 3 ditekan
        {   roda.SpeedPID(-5,-5);               // kecepatan motor 1/4 dari kecepatan maksimal ke arah belakang
            lcd.Tulis(0, "Robot Mundur 25%");
        }
        else if(Tombol4)                        // jika push button 4 ditekan
        {   roda.SpeedPWM(-20,-20);             // kecepatan motor  maksimal ke arah belakang
            lcd.Tulis(0, "Robot Mundur100%");
        }
        else                                    // jika tidak ada push button yang ditekan
        {   roda.SpeedPWM(0,0);                 // motor berhenti berputar dan dalam kondisi ngerem (brake)
            lcd.Tulis(1, "   Robot Stop.  ");
        }
        lcd.GotoXY(5,1);    lcd.uByte(roda.dSpeedKi);
        lcd.GotoXY(12,1);   lcd.uByte(roda.dSpeedKa);
    }
}
