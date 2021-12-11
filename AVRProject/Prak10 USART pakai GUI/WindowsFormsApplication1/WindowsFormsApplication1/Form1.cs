using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        // deklarasi konstanta
        public const char Header1 = '!', Header2 = '@', Header3 = '#', Header4 = '$', HeaderError = '?';
        public const char Tail = '~';
        public const short MaxData = 100;
        public const int spasi = 40, tinggi = 25, top = 45;

        //deklarasi variabel 
        // index data serial yang masuk
        int Index, DigitalOutData, counterSerial = 0, counterSerialTimeout = 0;
        bool Status, StartKomunikasi = false;    // status dari header
        public byte[] dataSerial = new byte[MaxData];   // batasan data sraial dalam 1 paket

        // deklarasi panel untuk group pada masing-masing komponen
        private Panel DigitalOutBox = new Panel();
        private Panel DigitalInBox = new Panel();
        private Panel AnalogOutBox = new Panel();
        private Panel AnalogInBox = new Panel();

        // deklarasi komponen untuk akses data
        public PictureBox[] myPicBox = new PictureBox[8];       // menampilkan data digital input

        public CheckBox[] myCheckBox = new CheckBox[8];         // menuliskan data digital output

        public ProgressBar[] myProgBar = new ProgressBar[8];    // menampilkan data analog input
        public TrackBar[] myTrackBar = new TrackBar[2];         // menuliskan data analog ouput
        public Label[] ProgBarLabel = new Label[8];             // menuliskan data analog input yg terbaca
        public Label[] TrackBarLabel = new Label[2];            // menuliskan data analog ouput yg dikirim 

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            serialPort1.BaudRate = int.Parse(comboBox2.SelectedItem.ToString());
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            while (StartKomunikasi) // menunggu satu paket komunikasi selesai
            {
                Application.DoEvents();
                StartKomunikasi = false;
            }
            if (serialPort1.IsOpen) // tutup kembali port komunikasi serial
            {
                StartKomunikasi = false;
                serialPort1.Close();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen == false)    // jika port belum terbuka
            {
                serialPort1.PortName = comboBox1.Text;  // set port yg dipilih pada comboBox
                serialPort1.Open();                     // coba buka port yg dipilih tadi
                if (serialPort1.IsOpen == true)         // jika port berhasil dibuka
                {
                    button1.Text = "Disconnect";        // aktifkan tombol untuk proses disconnnect
                    label2.Text = "Serial terhubung pada " + serialPort1.PortName + ":" + serialPort1.BaudRate
                    + " bps, " + serialPort1.DataBits + " bit, " + serialPort1.Parity + " parity, " +
                    serialPort1.StopBits + " stop bit";
                    button2.Enabled = true;             // aktifkan tombol untuk memulai proses komunikasi
                }
            }
            else
            {
                serialPort1.Close();        // tutup port komunikasi 
                button1.Text = "Connect";   // aktifkan tombol untuk koneksi ulang
                label2.Text = "Port serial tidak terhubung";    // tuliskan status bahwa port tidak terhubung
                StartKomunikasi = false;    // flag komunikasi start dinonaktifkan
                button2.Enabled = false;    // tombol start komunikasi dinonaktifkan
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (button2.Text == "Start Komunikasi")
            {
                if (serialPort1.IsOpen)
                {
                    serialPort1.Write(Header1.ToString() + Tail);
                    button2.Text = "Stop Komunikasi";
                    StartKomunikasi = true;
                    timer1.Enabled = true;      // start timeout timer
                    label3.Text = "Status : Komunikasi Berlangsung";
                    button2.BackColor = Color.GreenYellow;
                }
            }
            else
            {
                button2.Text = "Start Komunikasi";
                StartKomunikasi = false;
                timer1.Enabled = false;      // start timeout timer
                label3.Text = "Status : Komunikasi Terhenti";
                button2.BackColor = Color.OrangeRed;
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            int dataRate = counterSerial - counterSerialTimeout;    // menghitung jumlah data yang tersamnpling
            counterSerialTimeout = counterSerial;                   // update isi counterSerialTimeout
            if (dataRate == 0)                // jika jumlah sampling=0, berarti tidak ada aktivitas data
            {
                button2_Click(sender, e);   // memanggil fungsi penekanan tombol start komunikasi secara 
                                            // software
            }
            // menampilkan jumlah sampling per detiknya	
            label4.Text = "Sampling : " + dataRate.ToString() + "/s";
        }

        private void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            byte buffer; string x; int i, j, k, nilai;
            j = serialPort1.BytesToRead;        // jumlah karakter yang masuk pada buffer serial
            for (i = 0; i < j; i++)             // looping sebanyak karakter yang masuk
            {
                buffer = (byte)serialPort1.ReadByte();  // ambil data per karakter (per byte)
                if (Status == false)                    // jika status masih false (header belum ditemukan)
                {
                    if (buffer == Header1 || buffer == Header3)
                    {
                        Status = true;      // tanda bahwa header sudah ditemukan          
                        Index = 0;          // siap menyimpan karakter berikutnya
                        dataSerial[Index] = buffer; // simpan nilai header untuk pengecekan saan tail ditemukan
                    }
                    // header 2 hanya berupa acknowledgement bahwa data LED berhasil ditulis
                    else if (buffer == Header2)
                    { // kirim protokol data ke-3 (mengirim Request data Analog In)
                        if (serialPort1.IsOpen && StartKomunikasi) serialPort1.Write(Header3.ToString() + Tail);
                    }
                    // header 2 hanya berupa acknowledgement bahwa data PWM berhasil ditulis
                    else if (buffer == Header4)
                    { //kirim protokol data ke-1 (mengirim Request data Digital In --> Dipswitch)
                        if (serialPort1.IsOpen && StartKomunikasi) serialPort1.Write(Header1.ToString() + Tail);
                        // hitungan ditambah setiap event dataReceived tertrigger(ada aktivitas data)
                        counterSerial++;
                    }
                    // mikrokontroler mengirim tanda error apabila protokol gagal diterjemahkan
                    else if (buffer == HeaderError)
                    {
                        MessageBox.Show("Terjadi Error Protokol", "Komunikasi Serial <<PC-Mikrokontroler>>", MessageBoxButtons.OK);
                        StartKomunikasi = false; // komunikasi dihentikan (request selanjutnya tidak dikirim)
                        this.Invoke((MethodInvoker)delegate ()
                        {
                            button2.Text = "Start Komunikasi";  // menandai untuk dilakukan start lagi secara manual
                            label3.Text = "Status : Error !!!"; // menampilkan status Error
                        });
                    }
                }
                else    // jika header sudah ditemukan
                {
                    if (buffer == Tail) // jika ketemu tail dari packet
                    { // mengubah dari byte array ke sreing dan disimpan ke variabel x
                        x = System.Text.Encoding.Default.GetString(dataSerial);
                        switch ((char)dataSerial[0])        // cek nilai header yang disimpan saat header ditemukan
                        {
                            case Header1:    // isi proses yang terjadi ketika menerima data header 1
                                this.Invoke((MethodInvoker)delegate ()
                                { // memastikan karakter yang diterima sebanyak 3 dan berisi angka 
                                    if (Index == 3 && (int.TryParse(x.Substring(1, 3), out nilai)))
                                    { // menampilkan data dipswitch
                                        for (k = 0; k < 8; k++) // di lakukan looping 8x, karena data = 8 bit         
                                        {
                                            if ((nilai & (1 << k)) != 0)   // apakah bit ke-k = 1
                                            { // jika nilai bit ke-i = 1, maka ditampilkan dengan warna merah
                                                myPicBox[k].BackColor = System.Drawing.Color.Red;
                                            }
                                            else
                                            { // jika tidak ditampilkan dengan  warna putih
                                                myPicBox[k].BackColor = System.Drawing.Color.White;
                                            }
                                        }
                                    }
                                });
                                // kirim protokol data ke-2 (mengirim data Digital Out)
                                if (serialPort1.IsOpen && StartKomunikasi) serialPort1.Write(Header2 +
                                 toFixString3(DigitalOutData) + Tail);
                                break;
                            case Header3:    // isi proses yang terjadi ketika menerima data header 3
                                this.Invoke((MethodInvoker)delegate ()
                                {
                                    if (Index == 24) // menampilkan data ADC jika jumlah data = 3 x 8 kanal
                                    {
                                        for (k = 0; k < 8; k++) // di lakukan looping 8x, karena data = 8 bit         
                                        {
                                            if (int.TryParse(x.Substring(1 + k * 3, 3), out nilai))  // konversi 3 karakter
                                            {
                                                myProgBar[k].Value = nilai;                // menampilkan ke progressbar
                                                ProgBarLabel[k].Text = nilai.ToString();   // menampilkan dalam bentuk angka
                                            }
                                        }
                                    }
                                    // kirim protokol data ke-4 (mengirim data Analog Out --> PWM1 dan PWM2)
                                    if (serialPort1.IsOpen && StartKomunikasi) serialPort1.Write(Header4 +
                                    toFixString3(myTrackBar[0].Value) + toFixString3(myTrackBar[1].Value) + Tail);
                                });
                                break;
                        }
                        Status = false; // setelah ketemu tail, status dikembalikan ke false (harus cari header baru)
                    }
                    else  // selain data header dan tail berarti data payload
                    {
                        if (++Index > MaxData)      // jika payload melebihi jumlah data maksimal yang ditentukan   
                        {
                            Index = 0;              // data overflow
                            Status = false;         // jika ada overflow, maka harus cari header baru
                        }
                        else
                        {
                            dataSerial[Index] = buffer; // data payload disususun pada variabel dataSerial 
                        }
                    }
                }
            }
        }

        public Form1()
        {
            InitializeComponent();
            // seting panel untuk grup data digital input
            this.DigitalInBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.DigitalInBox.Size = new System.Drawing.Size(60, spasi * 8 + 20);
            this.DigitalInBox.Location = new System.Drawing.Point(10, top);
            this.Controls.Add(this.DigitalInBox);

            // seting panel untuk grup data digital output
            this.DigitalOutBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.DigitalOutBox.Size = new System.Drawing.Size(70, spasi * 8 + 20);
            this.DigitalOutBox.Location = new System.Drawing.Point(75, top);
            this.Controls.Add(this.DigitalOutBox);

            // seting panel untuk grup data analog input
            this.AnalogInBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.AnalogInBox.Size = new System.Drawing.Size(340, spasi * 8 + 20);
            this.AnalogInBox.Location = new System.Drawing.Point(150, top);
            this.Controls.Add(this.AnalogInBox);

            // seting panel untuk grup data analog output
            this.AnalogOutBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.AnalogOutBox.Size = new System.Drawing.Size(100, spasi * 8 + 20);
            this.AnalogOutBox.Location = new System.Drawing.Point(500, top);
            this.Controls.Add(this.AnalogOutBox);

            // set lebar form 
            this.Width = this.AnalogOutBox.Left + this.AnalogOutBox.Width + 25;

            short i;
            for (i = 0; i < 8; i++)     // looping 8x untuk data DI, DO, dan AI 
            {
                // picture box (DI)
                this.myPicBox[i] = new PictureBox();
                this.myPicBox[i].BackColor = System.Drawing.Color.White;
                this.myPicBox[i].BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
                this.myPicBox[i].Size = new System.Drawing.Size(tinggi, tinggi);
                this.myPicBox[i].Location = new System.Drawing.Point(15, 15 + (spasi * i));
                this.myPicBox[i].BringToFront();
                this.DigitalInBox.Controls.Add(this.myPicBox[i]);

                //Checkbox (DO)
                this.myCheckBox[i] = new CheckBox();
                this.myCheckBox[i].Location = new System.Drawing.Point(15, 15 + (spasi * i));
                this.myCheckBox[i].Size = new System.Drawing.Size(60, tinggi);
                this.myCheckBox[i].Text = "Bit-" + i.ToString();
                this.DigitalOutBox.Controls.Add(this.myCheckBox[i]);
                this.myCheckBox[i].CheckedChanged += new System.EventHandler(this.myCheckBox_CheckedChanged);

                //progressbar (AI)
                this.myProgBar[i] = new ProgressBar();
                this.myProgBar[i].Location = new System.Drawing.Point(15, 15 + (spasi * i));
                this.myProgBar[i].Size = new System.Drawing.Size(285, tinggi);
                this.myProgBar[i].Minimum = 0;
                this.myProgBar[i].Maximum = 255;
                this.myProgBar[i].Style = System.Windows.Forms.ProgressBarStyle.Continuous;
                this.AnalogInBox.Controls.Add(this.myProgBar[i]);

                // label untuk progressbar (AI)
                this.ProgBarLabel[i] = new Label();
                this.ProgBarLabel[i].AutoSize = true;
                this.ProgBarLabel[i].Location = new System.Drawing.Point(305, this.myProgBar[i].Top + 7);
                this.ProgBarLabel[i].Size = new System.Drawing.Size(30, tinggi);
                this.ProgBarLabel[i].Text = "100";// myProgBar[i].Value.ToString();
                this.AnalogInBox.Controls.Add(this.ProgBarLabel[i]);

            }
            for (i = 0; i < 2; i++) // looping 2 x untuk data AO
            {   //trackbar (AO)
                this.myTrackBar[i] = new TrackBar();
                this.myTrackBar[i].Location = new System.Drawing.Point(15 + (spasi * i), 10);
                this.myTrackBar[i].Minimum = 0;
                this.myTrackBar[i].Maximum = 255;
                this.myTrackBar[i].Size = new System.Drawing.Size(spasi, 300);
                this.myTrackBar[i].Scroll += new System.EventHandler(this.myTrackBar_Scroll);
                this.myTrackBar[i].Orientation = Orientation.Vertical;
                this.AnalogOutBox.Controls.Add(this.myTrackBar[i]);
                // label untuk TrackBar (AO)
                this.TrackBarLabel[i] = new Label();
                this.TrackBarLabel[i].AutoSize = true;
                this.TrackBarLabel[i].TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
                this.TrackBarLabel[i].Location = new System.Drawing.Point(20 + (spasi * i),
                this.myTrackBar[i].Height + this.myTrackBar[i].Top);
                this.TrackBarLabel[i].Size = new System.Drawing.Size(30, tinggi);
                this.TrackBarLabel[i].Text = myTrackBar[i].Value.ToString();
                this.AnalogOutBox.Controls.Add(this.TrackBarLabel[i]);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Enabled = false;     // kondisi awal timer timeout = tidak aktif
            button2.Enabled = false;    // tombol koneksi tidak aktif
            // mencari comm port yang ada pada komputer
            foreach (string s in SerialPort.GetPortNames())
            {
                comboBox1.Items.Add(s); // menampilkan port yang terdeteksi pada comboBox 
            }
            comboBox1.Text = comboBox1.Items[0].ToString(); // set nilai aktif pada data 
                                                            // data pertama
            label4.Text = "Sampling : 0/s";
        }

        private void myTrackBar_Scroll(object sender, EventArgs e)
        {
            // mengambil index trackBar yang aktif
            int index = Array.IndexOf(myTrackBar, sender);
            // menampilkan nilai trackBar yang aktif pada komponen label
            TrackBarLabel[index].Text = myTrackBar[index].Value.ToString();
        }

        private void myCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            // fungsi mengubah dari nilai biner ke nilai desimal
            int i, nilai = 0;                                   // milai  awal = 0
            for (i = 0; i < 8; i++)                             // di cek nilai per bit (8 bit) 
            {
                if (myCheckBox[i].Checked) nilai += +(1 << i);  // jika nilai checkBox ke-i aktif
            }                                                   // hasil +=2^i
            DigitalOutData = nilai;                             // simpan ke variabel yang akan  
        }

        private string toFixString3(int x)
        {
            string s;       // variabel untuk menyimpan hasil konversi
            if (x < 10)                     // jika hanya 1 angka (satuan saja)
            {
                s = "00" + x.ToString();    // tambahkan 2 angka 0 di depannya 
            }
            else if (x < 100)               // jika ada puluhan dan satuan
            {
                s = "0" + x.ToString();     // tambahkan 1 angka 0 di depannya
            }
            else
            {
                s = x.ToString();           // selain itu sudah lengkap 3 angka
            }
            return s;                       // return nilai string hasil konversi
        }

    }
}
