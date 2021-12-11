# import json untuk memudahkan transfer data dalam format json
import json
from json.encoder import JSONEncoder
# import library mqtt untuk koneksi dengan alat
import paho.mqtt.client as mqtt
# import library mysql untuk koneksi ke database
from MySQLdb import _mysql

# kelas utama dari aplikasi ini
class myApp:
  # fungsi yang dijalankan saat mqtt berhasil terkoneksi dengan server
  def on_connect(self, client, userdata, flags, rc):
    print("connected with result code: ", str(rc))
    client.subscribe("UbayaSocialDistancing/API")

  # fungsi yang dijalankan saat mqtt mendapatkan pesan dari esp
  def on_message(self, c, u, message):
    # lihat pesan yang didapat 
    print('got from client: ', message.payload.decode())
    
    # fungsi try berguna jika suatu saat gagal encode paket jsonnya, aplikasi tidak langsung berhenti secara paksa
    try:
      # message adalah data yang diterima dari esp dalam bentuk byte, dan perlu di encode agar menjadi string
      # lalu di encode menggunakan library json agar gampang untuk pengambilan data
      jObj = json.loads(message.payload.decode())
        
      # jika data mode yang dikirim adalah i / in anggota
      if jObj['mode'] == 'i':
        # cek dulu apakah username dan password nya sesuai dengan yang ada di database
        if self.login(jObj['data']['u'], jObj['data']['p']):
          # jika memang bener username dan password tersedia, cek dulu apakah perangkat sedang digunakan oleh orang lain
          if self.device_check(jObj['mac'], jObj['data']['u']):
            # jika tidak digunakan maka, kirim pesan berhasil ke esp 
            name = self.get_name(jObj['data']['u'])
            message = {"data":"ok","detail":"login berhasil","name":name}
            self.reply(jObj['mac'], message)
          else:
            # jika sedang digunakan maka, kirim pesan gagal ke esp
            message = {"data":"fail","detail":"perangkat sedang digunakan"}
            self.reply(jObj['mac'], message)
        else:
          # jika username dan password tidak cocok, kirim pesan gagal login ke esp
          message = {"data":"fail","detail":"username atau password salah"}
          self.reply(jObj['mac'], message)
      # jika mode nya adalah out atau singkatan dari logout
      elif jObj['mode'] == 'o':
        # cek dulu username dan password memang terdaftar
        if self.login(jObj['data']['u'], jObj['data']['p']):
          # cek apakah user memang logout dari alat yang dulunya dipake buat login
          if self.logout(jObj['mac'], jObj['data']['u']):
            # jika berhasil, kirim pesan berhasil ke esp
            message = {"data":"ok","detail":"logout berhasil"}
            self.reply(jObj['mac'], message)
          else:
            # jika gagal, maka kirim pesan gagal
            message = {"data":"fail","detail":"restricted"}
            self.reply(jObj['mac'], message)
        else:
          # jika username dan password tidak terdaftar, kirim pesan gagal
          message = {"data":"fail","detail":"restricted"}
          self.reply(jObj['mac'], message)
      # jika mode yang dikirim adalah h atau singkatan dari history
      elif jObj['mode'] == 'h':
        # cek dulu username dan password memang terdaftar
        if self.login(jObj['data']['u'], jObj['data']['p']):
          # setelah memang terdaftar, ambil history dari alat yang sedang dipakai oleh user dan kirim kembali ke user
          data = self.get_history(jObj['mac'])
          message = {"data":"ok","detail":data}
          self.reply(jObj['mac'], message)
        else:
          # kirim pesan gagal jika user tidak terdaftar
          message = {"data":"fail","detail":"restricted"}
          self.reply(jObj['mac'], message)
      # jika modenya adalah l atau singkatan dari log
      elif jObj['mode'] == 'l':
        # cek memang user terdaftar di sistem
        if self.login(jObj['data']['u'], jObj['data']['p']):
          # jika memang terdaftar, maka simpan data log ke database dan kirim pesan berhasil ke user
          self.log(jObj['mac'], jObj['data']['u'], jObj['data']['r'], jObj['data']['rm'])
          message = {"data":"ok","detail":"berhasil menyimpan data"}
          self.reply(jObj['mac'], message)
        else:
          # jika tidak terdaftar, kirim pesan gagal ke user
          message = {"data":"fail","detail":"restricted"}
          self.reply(jObj['mac'], message)
      # jika mode nya adalah ia atau singkatan dari in admin
      elif jObj['mode'] == 'ia':
        # cek username dan password
        if self.login_admin(jObj['data']['u'], jObj['data']['p']):
          name = self.get_name(jObj['data']['u'])
          message = {"data":"ok","detail":"login berhasil","name":name}
          self.reply(jObj['mac'], message)
        else:
          message = {"data":"fail","detail":"username atau password salah"}
          self.reply(jObj['mac'], message)
      # jika mode yang dipilih adalah la atau singkatan dari log admin
      elif jObj['mode'] == 'la':
        # pastikan memang datanya dari alat admin, mac yang digunakan memang mac dari esp admin
        if jObj['mac'] == '24:6f:28:aa:87:8e':
          self.log_admin(jObj['data']['r1'], jObj['data']['r2'], jObj['data']['r3'], jObj['data']['r4'])
          print('log admin dapat data')
      # jika mode yang dipilih adalah history admin
      elif jObj['mode'] == 'ha':
        # cek username dan password admin dulu
        if self.login_admin(jObj['data']['u'], jObj['data']['p']):
          # jika memang terdaftar, maka ambil semua data admin dan kirim ke esp
          data = self.get_history_admin()
          message = {"data":"ok","detail":data}
          self.reply(jObj['mac'], message)
        else:
          # jika username dan password nya salah, maka kirim pesan gagal
          message = {"data":"fail","detail":"restricted"}
          self.reply(jObj['mac'], message)
      # jika mode yang dipilih adalah oa atau singkatan dari out admin
      elif jObj['mode'] == 'oa':
        # langsung saja kirim pesan berhasil
        message = {"data":"ok","detail":"logout successfully"}
        self.reply(jObj['mac'], message)
      # jika mode yang dikirim tidak ada di atas sama sekali, kirim pesan perintah tidak diketahui
      else:
        message = {"data":"fail","detail":"unknown command"}
        self.reply(jObj['mac'], message)
    # jika gagal decode json, maka tampilkan ke layar saja, biar aplikasi tidak sampai keluar paksa
    except json.JSONDecodeError as e:
      print("JSONDecodeError: ", e)
    # jika ada error lain selain error json, maka akan masuk ke fungsi dibawah ini, langsung di tampilkan ke console
    except Exception as e:
      print("Exception Mqtt: ",e)
  
  # fungsi reply untuk mengirim balasan ke user
  def reply(self, mac, message):
    # kirim ke topic sesuai dengan topic yang didengarkan oleh masing masing alat, yaitu sesuai dengan mac address alat masing-masing
    # dengan menghilangkan tanda : di mac addressnya
    mac = mac.replace(':', '')
    payload = json.dumps(message)
    print("reply from me: ", payload)

    # pakai fungsi try lagi agar jika terjadi kegagalan pengiriman, aplikasi tidak keluar paksa
    try:
      # karena broker mqtt nya membatasi jumlah paket data sekali kirim adalah 128 byte, maka batasi pengiriman per 128 byte
      # jika pesan balasannya panjang seperti log data, biasanya sampai ribuan byte, maka kirim secara berurutan per 128 byte
      st = 0
      en = 128
      while st < len(payload):
        self.client.publish("UbayaSocialDistancing/{0}".format(mac), payload[st:en])
        st = en
        en += 128
      self.client.publish("UbayaSocialDistancing/{0}".format(mac), "endpacket")
    # jika terjadi kegagalan, maka akan masuk fungsi yang dibawah ini
    except Exception as e:
      print(e)
  
  # fungsi cek login, seperti query yang biasa digunakan ke MySQL
  def login(self, username, password):
    query_str = "select * from tb_user where username='{0}' and password='{1}'".format(
      self.db.escape_string(username).decode(), 
      self.db.escape_string(password).decode()
    )

    self.db.query(query_str)
    result = self.db.store_result()
    if result.num_rows() > 0:
      return True
    else:
      return False
  
  # ambil nama user berdasarkan username
  def get_name(self, username):
    query_str = "select nama from tb_user where username='{0}'".format(self.db.escape_string(username).decode())
    self.db.query(query_str)
    result = self.db.store_result()
    return result.fetch_row(1)[0][0].decode()
    
  # cek login admin
  def login_admin(self, username, password):
    query_str = "select * from tb_user where username='{0}' and password='{1}' and role='A'".format(
      self.db.escape_string(username).decode(),
      self.db.escape_string(password).decode()
    )
    self.db.query(query_str)
    result = self.db.store_result()
    if result.num_rows() > 0:
      return True
    else:
      return False

  # cek apakah alatnya sedang tidak digunakan orang lain
  def device_check(self, mac, username):
    query_str = "select * from tb_device where mac='{0}' and (username='none' or username='{1}')".format(
      self.db.escape_string(mac).decode(), 
      self.db.escape_string(username).decode()
    )

    self.db.query(query_str)
    result = self.db.store_result()
    if result.num_rows() > 0:
      query_str = "update tb_device set username='{0}' where mac='{1}'".format(
        self.db.escape_string(username).decode(), 
        self.db.escape_string(mac).decode()
      )
      self.db.query(query_str)
      return True
    else:
      return False

  # fungsi logout
  def logout(self, mac, username):
    query_str = "update tb_device set username='none' where mac='{0}' and username='{1}'".format(
      self.db.escape_string(mac).decode(), 
      self.db.escape_string(username).decode()
    )
    self.db.query(query_str)
    if self.db.affected_rows() > 0:
      return True
    else:
      return False

  # fungsi ambil history data
  def get_history(self, mac):
    query_str = "select nama, rssi, created_at from tb_log join tb_user on tb_log.username2 = tb_user.username where mac1='{0}' order by tb_log.created_at desc limit 15".format(
      self.db.escape_string(mac).decode()
    )
    self.db.query(query_str)
    result = self.db.store_result()

    outer = []
    for rows in result.fetch_row(0):
      inner = []
      for column in rows:
        inner.append(column.decode())
      outer.append(inner)
    return outer

  # ambil history admin
  def get_history_admin(self):
    query_str = "select rssi1, rssi2, rssi3, rssi4, created_at from tb_master where 1 order by created_at desc limit 50"
    self.db.query(query_str)
    result = self.db.store_result()

    outer = []
    for rows in result.fetch_row(0):
      inner = []
      for column in rows:
        inner.append(column.decode())
      outer.append(inner)
    return outer

  # simpan log user
  def log(self, mac, username, rssi, remote_mac):
    query_str = "insert into tb_log (mac1, mac2, username1, rssi, username2) values ('{0}', '{1}', '{2}', '{3}', (select username from tb_device where mac='{1}'))".format(
      self.db.escape_string(mac).decode(),
      self.db.escape_string(remote_mac).decode(),
      self.db.escape_string(username).decode(),
      self.db.escape_string(rssi).decode()
    )
    self.db.query(query_str)
  
  # simpan log admin
  def log_admin(self, rssi1, rssi2, rssi3, rssi4):
    query_str = "insert into tb_master (rssi1, rssi2, rssi3, rssi4) values ('{0}', '{1}', '{2}', '{3}')".format(
      self.db.escape_string(rssi1).decode(),
      self.db.escape_string(rssi2).decode(),
      self.db.escape_string(rssi3).decode(),
      self.db.escape_string(rssi4).decode()
    )
    self.db.query(query_str)

  # fungsi utama yang dijalankan pertama kali
  def main(self):
    # koneksi dulu ke MySQL
    self.db = _mysql.connect("localhost","root","","ubayasocialdistancing")
    # buat koneksi dengan broker mqtt
    self.client = mqtt.Client()
    # set fungsi saat mqtt sukses terkoneksi dan saat mqtt mendapatkan pesan baru
    self.client.on_connect = self.on_connect
    self.client.on_message = self.on_message
    # konek sekaran ke mqtt
    self.client.connect("broker.hivemq.com", 1883, 60)
    # jalankan mqtt selamanya sampai dihentikan oleh user
    self.client.loop_forever()

# hanya jalankan aplikasi ini jika file ini sebagai file program utama , bukan sebagai library
if __name__ == '__main__':
  try:
    # buat objek dari kelas yang telah kita buat
    App = myApp()
    # jalankan fungsi main dari kelas kita
    App.main()
  # cek jika user menjalankan ctrl + c
  except KeyboardInterrupt:
    print('interrupted by user')