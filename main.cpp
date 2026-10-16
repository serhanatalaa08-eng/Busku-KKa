
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>
#include <ctime>

using namespace std;

// ==================================================
//                  BUSKU TRAVEL
//            SISTEM PEMESANAN TIKET BUS
// ==================================================

const int JUMLAH_KELAS = 3;
const int JUMLAH_RUTE = 2;
const int JUMLAH_KOTA = 4;
const int JUMLAH_KURSI = 50;
const int JUMLAH_BANK = 4;

bool kursiTerpesan[JUMLAH_KELAS]
                  [JUMLAH_RUTE]
                  [JUMLAH_KURSI + 1] = {};

string namaKelas[JUMLAH_KELAS] = {
    "Eksekutif", "Bisnis", "Ekonomi"
};

long long hargaDasar[JUMLAH_KELAS] = {
    600000, 400000, 250000
};

string namaRute[JUMLAH_RUTE] = {
    "Trans Jawa", "Trans Sumatera"
};

string kotaTujuan[JUMLAH_RUTE][JUMLAH_KOTA] = {
    {"Bandung", "Yogyakarta", "Semarang", "Surabaya"},
    {"Lampung", "Palembang", "Pekanbaru", "Medan"}
};

string terminalAsal[JUMLAH_RUTE] = {
    "Terminal Pulo Gebang, Jakarta",
    "Terminal Kampung Rambutan, Jakarta"
};

string namaBank[JUMLAH_BANK] = {
    "BCA", "Bank Mandiri", "BRI", "BNI"
};

// Nomor rekening contoh untuk tampilan aplikasi.
string nomorRekening[JUMLAH_BANK] = {
    "000000000001",
    "111111111112",
    "222222222223",
    "333333333334"
};

string namaPemilikRekening = "BUSKU TRAVEL";
int nomorTransaksi = 1;

// ---------------- STRUKTUR PEMESANAN ----------------

struct Pemesanan {
    string kodeBooking;
    string namaPenumpang;
    string kelas;
    string rute;
    string kotaAsal;
    string tujuan;
    string terminal;
    string jadwal;
    string tanggal;
    string jamBerangkat;
    string metodePembayaran;
    string statusPembayaran;

    vector<int> nomorKursi;

    int jumlahTiket;
    long long hargaSatuan;
    long long subtotal;
    long long diskon;
    long long total;
};

// ---------------- TAMPILAN --------------------------

void garis() {
    cout << "==================================================\n";
}

void garisTipis() {
    cout << "--------------------------------------------------\n";
}

void judul(const string& teks) {
    cout << "\n";
    garis();
    cout << "  " << teks << "\n";
    garis();
}

void tampilkanLogo() {
    cout << R"(
  ____  _   _ ____  _  __ _   _
 | __ )| | | / ___|| |/ /| | | |
 |  _ \| | | \___ \| ' / | | | |
 | |_) | |_| |___) | . \ | |_| |
 |____/ \___/|____/|_|\_\ \___/

             BUSKU TRAVEL
       LAYANAN TIKET BUS ANTARKOTA
)";
    garis();
}

// ---------------- FORMAT RUPIAH ---------------------

string formatRupiah(long long angka) {
    string nilai = to_string(angka);
    string hasil = "";

    for (int i = 0; i < (int)nilai.size(); i++) {
        hasil += nilai[i];

        int sisa = (int)nilai.size() - i - 1;

        if (sisa > 0 && sisa % 3 == 0) {
            hasil += ".";
        }
    }

    return "Rp" + hasil;
}

// ---------------- INPUT ANGKA -----------------------

int inputAngka(
    const string& pesan,
    int minimum,
    int maksimum
) {
    string masukan;
    int angka;

    while (true) {
        cout << pesan;
        getline(cin, masukan);

        stringstream ss(masukan);
        char tambahan;

        if ((ss >> angka) &&
            !(ss >> tambahan) &&
            angka >= minimum &&
            angka <= maksimum) {
            return angka;
        }

        cout << "Input tidak valid. Masukkan angka "
             << minimum << " sampai " << maksimum
             << ".\n";
    }
}

// ---------------- INPUT TEKS ------------------------

string inputTeks(const string& pesan) {
    string teks;

    while (true) {
        cout << pesan;
        getline(cin, teks);

        if (teks.find_first_not_of(" \t\r\n")
            != string::npos) {
            return teks;
        }

        cout << "Input tidak boleh kosong.\n";
    }
}

// ---------------- PILIH KELAS -----------------------

int pilihKelas() {
    judul("1. PILIH KELAS BUS");

    for (int i = 0; i < JUMLAH_KELAS; i++) {
        cout << i + 1 << ". "
             << namaKelas[i]
             << " - " << formatRupiah(hargaDasar[i])
             << "\n";
    }

    return inputAngka(
        "\nPilih kelas: ", 1, JUMLAH_KELAS
    ) - 1;
}

// ---------------- PILIH RUTE ------------------------

int pilihRute() {
    judul("2. PILIH RUTE PERJALANAN");

    cout << "1. Trans Jawa\n";
    cout << "   Tujuan: Bandung, Yogyakarta,\n";
    cout << "           Semarang, Surabaya\n\n";

    cout << "2. Trans Sumatera\n";
    cout << "   Tujuan: Lampung, Palembang,\n";
    cout << "           Pekanbaru, Medan\n";

    return inputAngka(
        "\nPilih rute: ", 1, JUMLAH_RUTE
    ) - 1;
}

// ---------------- PILIH TUJUAN -----------------------

int pilihTujuan(int rute) {
    judul("3. PILIH KOTA TUJUAN");

    cout << "Rute: " << namaRute[rute] << "\n\n";

    for (int i = 0; i < JUMLAH_KOTA; i++) {
        cout << i + 1 << ". "
             << kotaTujuan[rute][i]
             << " | Tambahan "
             << formatRupiah(i * 50000LL)
             << "\n";
    }

    return inputAngka(
        "\nPilih tujuan: ", 1, JUMLAH_KOTA
    ) - 1;
}

// ---------------- PILIH JADWAL -----------------------

string pilihJadwal() {
    judul("4. PILIH JADWAL KEBERANGKATAN");

    cout << "1. Pagi  - 07.00 WIB\n";
    cout << "2. Siang - 12.00 WIB\n";
    cout << "3. Sore  - 16.00 WIB\n";
    cout << "4. Malam - 20.00 WIB\n";

    int pilihan = inputAngka(
        "\nPilih jadwal: ", 1, 4
    );

    if (pilihan == 1) return "Pagi";
    if (pilihan == 2) return "Siang";
    if (pilihan == 3) return "Sore";

    return "Malam";
}

string jamJadwal(const string& jadwal) {
    if (jadwal == "Pagi") return "07.00 WIB";
    if (jadwal == "Siang") return "12.00 WIB";
    if (jadwal == "Sore") return "16.00 WIB";

    return "20.00 WIB";
}

// ---------------- PILIH TANGGAL OTOMATIS -------------

string namaHari(int hari) {
    string hariIndonesia[] = {
        "Minggu", "Senin", "Selasa", "Rabu",
        "Kamis", "Jumat", "Sabtu"
    };

    return hariIndonesia[hari];
}

string namaBulan(int bulan) {
    string bulanIndonesia[] = {
        "Januari", "Februari", "Maret", "April",
        "Mei", "Juni", "Juli", "Agustus",
        "September", "Oktober", "November", "Desember"
    };

    return bulanIndonesia[bulan];
}

string formatTanggal(const tm& tanggal) {
    stringstream hasil;

    hasil << namaHari(tanggal.tm_wday)
          << ", "
          << tanggal.tm_mday << " "
          << namaBulan(tanggal.tm_mon) << " "
          << tanggal.tm_year + 1900;

    return hasil.str();
}

string pilihTanggal() {
    judul("5. PILIH TANGGAL KEBERANGKATAN");

    cout << "CATATAN PEMESANAN\n";
    cout << "Tanggal keberangkatan hanya tersedia untuk\n";
    cout << "7 hari ke depan. Silakan pilih tanggal\n";
    cout << "yang tersedia di bawah ini.\n\n";

    time_t sekarang = time(nullptr);
    tm tanggalHariIni = *localtime(&sekarang);

    vector<tm> daftarTanggal;

    for (int i = 1; i <= 7; i++) {
        tm tanggal = tanggalHariIni;

        tanggal.tm_mday += i;
        tanggal.tm_hour = 12;
        tanggal.tm_min = 0;
        tanggal.tm_sec = 0;

        mktime(&tanggal);

        daftarTanggal.push_back(tanggal);

        cout << i << ". "
             << formatTanggal(tanggal)
             << "\n";
    }

    int pilihan = inputAngka(
        "\nPilih tanggal keberangkatan (1-7): ",
        1,
        7
    );

    tm terpilih = daftarTanggal[pilihan - 1];

    stringstream hasil;

    hasil << setfill('0')
          << setw(2) << terpilih.tm_mday << "-"
          << setw(2) << terpilih.tm_mon + 1 << "-"
          << terpilih.tm_year + 1900;

    return hasil.str();
}

// ---------------- HITUNG HARGA -----------------------

long long hitungHarga(
    int kelas,
    int rute,
    int tujuan
) {
    return hargaDasar[kelas]
         + rute * 100000LL
         + tujuan * 50000LL;
}

void hitungTotal(Pemesanan& data) {
    data.subtotal =
        data.hargaSatuan * data.jumlahTiket;

    if (data.jumlahTiket > 2) {
        data.diskon = data.subtotal * 10 / 100;
    } else {
        data.diskon = 0;
    }

    data.total = data.subtotal - data.diskon;
}

void tampilkanRincianHarga(const Pemesanan& data) {
    judul("RINCIAN BIAYA");

    cout << left << setw(25) << "Harga per tiket"
         << ": " << formatRupiah(data.hargaSatuan) << "\n";

    cout << left << setw(25) << "Jumlah tiket"
         << ": " << data.jumlahTiket << "\n";

    cout << left << setw(25) << "Subtotal"
         << ": " << formatRupiah(data.subtotal) << "\n";

    cout << left << setw(25) << "Diskon"
         << ": " << formatRupiah(data.diskon) << "\n";

    garisTipis();

    cout << left << setw(25) << "TOTAL PEMBAYARAN"
         << ": " << formatRupiah(data.total) << "\n";

    if (data.jumlahTiket > 2) {
        cout << "\nPROMO AKTIF: Diskon 10% berhasil diterapkan.\n";
    } else {
        cout << "\nPromo 10% berlaku untuk pembelian lebih dari 2 tiket.\n";
    }
}

// ---------------- DENAH KURSI -------------------------

bool sudahDipilih(
    const vector<int>& daftar,
    int nomor
) {
    return find(
        daftar.begin(),
        daftar.end(),
        nomor
    ) != daftar.end();
}

void tampilkanKeteranganKursi() {
    cout << "\nKETERANGAN KURSI\n";
    cout << "[01] Kursi tersedia\n";
    cout << "[XX] Kursi telah dipesan\n";
    cout << "[**] Kursi pilihan Anda\n\n";
}

void tampilkanPetaKursi(
    int kelas,
    int rute,
    const vector<int>& pilihan
) {
    garis();
    cout << "               DENAH KURSI BUS\n";
    garis();

    cout << "                  BAGIAN DEPAN\n";
    cout << "                     [SOPIR]\n\n";

    tampilkanKeteranganKursi();

    cout << "       SISI KIRI        SISI KANAN\n";
    cout << "       A      B         C      D\n";
    garisTipis();

    for (int baris = 0; baris < 13; baris++) {
        for (int posisi = 0; posisi < 4; posisi++) {
            int nomor = baris * 4 + posisi + 1;

            if (nomor > JUMLAH_KURSI) {
                continue;
            }

            bool terpesan =
                kursiTerpesan[kelas][rute][nomor];

            bool dipilih =
                sudahDipilih(pilihan, nomor);

            if (terpesan) {
                cout << "[XX]";
            } else if (dipilih) {
                cout << "[**]";
            } else {
                cout << "["
                     << setw(2) << setfill('0')
                     << nomor << setfill(' ')
                     << "]";
            }

            if (posisi == 0) {
                cout << " ";
            } else if (posisi == 1) {
                cout << "       ";
            } else if (posisi == 2) {
                cout << " ";
            }
        }

        cout << "\n";
    }

    int tersedia = 0;

    for (int i = 1; i <= JUMLAH_KURSI; i++) {
        if (!kursiTerpesan[kelas][rute][i]) {
            tersedia++;
        }
    }

    cout << "\nKursi tersedia: "
         << tersedia << " dari "
         << JUMLAH_KURSI << " kursi.\n";

    garis();
}

vector<int> pilihKursi(
    int kelas,
    int rute,
    int jumlah
) {
    vector<int> pilihan;

    while ((int)pilihan.size() < jumlah) {
        tampilkanPetaKursi(kelas, rute, pilihan);

        cout << "Kursi dipilih: "
             << pilihan.size() << " dari "
             << jumlah << "\n";

        int nomor = inputAngka(
            "Pilih nomor kursi (1-50): ",
            1,
            JUMLAH_KURSI
        );

        if (kursiTerpesan[kelas][rute][nomor]) {
            cout << "Kursi sudah dipesan.\n";
        } else if (sudahDipilih(pilihan, nomor)) {
            cout << "Kursi sudah Anda pilih.\n";
        } else {
            pilihan.push_back(nomor);

            cout << "Kursi " << nomor
                 << " berhasil dipilih.\n";
        }
    }

    cout << "\nKursi pilihan: ";

    for (int nomor : pilihan) {
        cout << nomor << " ";
    }

    cout << "\n";

    return pilihan;
}

// ---------------- METODE PEMBAYARAN ------------------

void tampilkanMetodePembayaran() {
    judul("6. METODE PEMBAYARAN");

    cout << "1. Transfer BCA\n";
    cout << "2. Transfer Mandiri\n";
    cout << "3. Transfer BRI\n";
    cout << "4. Transfer BNI\n";
    cout << "5. QRIS\n";
}

void tampilkanQRIS() {
    cout << "\n";
    garis();
    cout << "                 QRIS BUSKU\n";
    garis();

    cout << "       +---------------------+\n";
    cout << "       | ## # ### ## # ###   |\n";
    cout << "       | # ### # ## ## # #   |\n";
    cout << "       | ### # ## # ### ##   |\n";
    cout << "       | ## ## ### # # ## #   |\n";
    cout << "       | # # ## ### ## # ##   |\n";
    cout << "       +---------------------+\n";

    cout << "\nSilakan lakukan pembayaran sesuai total tagihan.\n";

    garis();
}

void tampilkanInformasiBank(int pilihan) {
    int indeks = pilihan - 1;

    garis();
    cout << "          INFORMASI TRANSFER BANK\n";
    garis();

    cout << "Bank           : "
         << namaBank[indeks] << "\n";

    cout << "Nomor rekening : "
         << nomorRekening[indeks] << "\n";

    cout << "Nama pemilik   : "
         << namaPemilikRekening << "\n";

    garis();
}

void prosesPembayaran(Pemesanan& data) {
    tampilkanMetodePembayaran();

    int metode = inputAngka(
        "\nPilih metode pembayaran: ", 1, 5
    );

    if (metode <= JUMLAH_BANK) {
        tampilkanInformasiBank(metode);

        data.metodePembayaran =
            "Transfer " + namaBank[metode - 1];
    } else {
        tampilkanQRIS();

        data.metodePembayaran = "QRIS";
    }

    cout << "\nTotal pembayaran: "
         << formatRupiah(data.total) << "\n";

    int konfirmasi;

    do {
        cout << "\n1. Konfirmasi pembayaran\n";
        cout << "2. Tampilkan kembali informasi pembayaran\n";

        konfirmasi = inputAngka("Pilihan: ", 1, 2);

        if (konfirmasi == 2) {
            if (metode <= JUMLAH_BANK) {
                tampilkanInformasiBank(metode);
            } else {
                tampilkanQRIS();
            }

            cout << "Total pembayaran: "
                 << formatRupiah(data.total) << "\n";
        }

    } while (konfirmasi == 2);

    data.statusPembayaran = "LUNAS";

    cout << "\nPembayaran berhasil dikonfirmasi.\n";
}

// ---------------- DATA PENUMPANG ----------------------

void isiDataPenumpang(Pemesanan& data) {
    judul("7. DATA PENUMPANG");

    data.namaPenumpang =
        inputTeks("Nama lengkap penumpang: ");
}

// ---------------- KODE BOOKING ------------------------

string buatKodeBooking() {
    stringstream kode;

    kode << "BSK-"
         << setfill('0')
         << setw(6)
         << nomorTransaksi++;

    return kode.str();
}

// ---------------- SIMPAN KURSI ------------------------

void simpanKursiTerpesan(
    int kelas,
    int rute,
    const vector<int>& pilihan
) {
    for (int nomor : pilihan) {
        kursiTerpesan[kelas][rute][nomor] = true;
    }
}

// ---------------- CETAK E-TICKET ----------------------

void cetakTiket(const Pemesanan& data) {
    judul("E-TICKET BUSKU TRAVEL");

    cout << "                 BUSKU TRAVEL\n";
    cout << "              TIKET ANTARKOTA\n";

    garis();

    cout << left << setw(22) << "Kode booking"
         << ": " << data.kodeBooking << "\n";

    cout << left << setw(22) << "Nama penumpang"
         << ": " << data.namaPenumpang << "\n";

    cout << left << setw(22) << "Kelas bus"
         << ": " << data.kelas << "\n";

    cout << left << setw(22) << "Rute"
         << ": " << data.rute << "\n";

    cout << left << setw(22) << "Kota asal"
         << ": " << data.kotaAsal << "\n";

    cout << left << setw(22) << "Kota tujuan"
         << ": " << data.tujuan << "\n";

    cout << left << setw(22) << "Terminal"
         << ": " << data.terminal << "\n";

    cout << left << setw(22) << "Tanggal"
         << ": " << data.tanggal << "\n";

    cout << left << setw(22) << "Jadwal"
         << ": " << data.jadwal << "\n";

    cout << left << setw(22) << "Jam berangkat"
         << ": " << data.jamBerangkat << "\n";

    cout << left << setw(22) << "Jumlah tiket"
         << ": " << data.jumlahTiket << "\n";

    cout << left << setw(22) << "Nomor kursi"
         << ": ";

    for (int nomor : data.nomorKursi) {
        cout << nomor << " ";
    }

    cout << "\n";

    garisTipis();

    cout << "RINCIAN PEMBAYARAN\n";

    cout << left << setw(22) << "Harga satuan"
         << ": " << formatRupiah(data.hargaSatuan)
         << "\n";

    cout << left << setw(22) << "Subtotal"
         << ": " << formatRupiah(data.subtotal)
         << "\n";

    cout << left << setw(22) << "Diskon"
         << ": " << formatRupiah(data.diskon)
         << "\n";

    cout << left << setw(22) << "Total dibayar"
         << ": " << formatRupiah(data.total)
         << "\n";

    cout << left << setw(22) << "Pembayaran"
         << ": " << data.metodePembayaran << "\n";

    cout << left << setw(22) << "Status"
         << ": " << data.statusPembayaran << "\n";

    garis();

    cout << "INFORMASI PERJALANAN\n";
    cout << "- Datang lebih awal sebelum keberangkatan.\n";
    cout << "- Simpan kode booking sebagai bukti pemesanan.\n";
    cout << "- Pastikan tanggal dan tujuan sudah benar.\n";

    garis();

    cout << "       TERIMA KASIH TELAH MEMILIH BUSKU\n";
    cout << "          Selamat menikmati perjalanan!\n";

    garis();
}

// ---------------- RINGKASAN PEMESANAN ----------------

void tampilkanRingkasan(const Pemesanan& data) {
    judul("RINGKASAN PEMESANAN");

    cout << "Kelas        : " << data.kelas << "\n";
    cout << "Rute         : " << data.rute << "\n";
    cout << "Tujuan       : " << data.tujuan << "\n";
    cout << "Tanggal      : " << data.tanggal << "\n";
    cout << "Jadwal       : " << data.jadwal << "\n";
    cout << "Jumlah tiket : " << data.jumlahTiket << "\n";

    cout << "Nomor kursi  : ";

    for (int nomor : data.nomorKursi) {
        cout << nomor << " ";
    }

    cout << "\n";

    garisTipis();

    cout << "Total: " << formatRupiah(data.total) << "\n";

    garis();
}

// ---------------- ALUR PEMESANAN ---------------------

void pesanTiket() {
    Pemesanan data;

    int kelas = pilihKelas();
    data.kelas = namaKelas[kelas];

    int rute = pilihRute();
    data.rute = namaRute[rute];

    data.kotaAsal = "Jakarta";
    data.terminal = terminalAsal[rute];

    int tujuan = pilihTujuan(rute);
    data.tujuan = kotaTujuan[rute][tujuan];

    data.jadwal = pilihJadwal();
    data.jamBerangkat = jamJadwal(data.jadwal);

    // Tanggal dipilih dari 7 hari ke depan.
    data.tanggal = pilihTanggal();

    judul("6. JUMLAH TIKET");

    cout << "Promo: diskon 10% untuk pembelian lebih dari 2 tiket.\n";

    int jumlahTiket = inputAngka(
        "Jumlah tiket (1-50): ", 1, JUMLAH_KURSI
    );

    int tersedia = 0;

    for (int i = 1; i <= JUMLAH_KURSI; i++) {
        if (!kursiTerpesan[kelas][rute][i]) {
            tersedia++;
        }
    }

    if (jumlahTiket > tersedia) {
        cout << "\nKursi tersedia hanya "
             << tersedia << ".\n";

        cout << "Pemesanan tidak dapat dilanjutkan.\n";
        return;
    }

    data.jumlahTiket = jumlahTiket;
    data.hargaSatuan = hitungHarga(kelas, rute, tujuan);

    hitungTotal(data);
    tampilkanRincianHarga(data);

    judul("8. PILIH KURSI");

    data.nomorKursi =
        pilihKursi(kelas, rute, jumlahTiket);

    tampilkanRingkasan(data);

    cout << "\n1. Lanjut ke pembayaran\n";
    cout << "2. Batalkan pemesanan\n";

    int lanjut = inputAngka("Pilihan: ", 1, 2);

    if (lanjut == 2) {
        cout << "\nPemesanan dibatalkan.\n";
        return;
    }

    prosesPembayaran(data);
    isiDataPenumpang(data);

    data.kodeBooking = buatKodeBooking();

    simpanKursiTerpesan(
        kelas, rute, data.nomorKursi
    );

    cetakTiket(data);
}

// ---------------- MENU UTAMA -------------------------

void tampilkanMenuUtama() {
    cout << "\n";
    garis();
    cout << "                 MENU UTAMA\n";
    garis();

    cout << "1. Pesan tiket bus\n";
    cout << "2. Lihat ketersediaan kursi\n";
    cout << "3. Informasi layanan\n";
    cout << "4. Keluar dari aplikasi\n";

    garis();
}

void lihatKetersediaan() {
    judul("KETERSEDIAAN KURSI");

    int kelas = pilihKelas();
    int rute = pilihRute();

    vector<int> kosong;

    tampilkanPetaKursi(kelas, rute, kosong);
}

void informasiLayanan() {
    judul("INFORMASI LAYANAN BUSKU");

    cout << "Nama layanan : BUSKU TRAVEL\n";
    cout << "Jenis layanan: Bus antarkota\n";
    cout << "Rute         : Trans Jawa dan Trans Sumatera\n";
    cout << "Kelas        : Eksekutif, Bisnis, Ekonomi\n";
    cout << "Kapasitas    : 50 kursi per kelas dan rute\n";
    cout << "Pembayaran   : Transfer bank atau QRIS\n";
    cout << "Promo        : Diskon 10% untuk lebih dari 2 tiket\n";

    cout << "Tanggal pemesanan tersedia untuk 7 hari ke depan.\n";

    garis();
}

// ---------------- PROGRAM UTAMA ---------------------

int main() {
    int pilihan;
    bool berjalan = true;

    tampilkanLogo();

    cout << "Selamat datang di BUSKU TRAVEL.\n";

    while (berjalan) {
        tampilkanMenuUtama();

        pilihan = inputAngka(
            "Pilih menu: ", 1, 4
        );

        switch (pilihan) {
            case 1:
                pesanTiket();
                break;

            case 2:
                lihatKetersediaan();
                break;

            case 3:
                informasiLayanan();
                break;

            case 4: {
                cout << "\nApakah Anda yakin ingin keluar?\n";
                cout << "1. Kembali ke menu utama\n";
                cout << "2. Keluar dari aplikasi\n";

                int keluar = inputAngka(
                    "Pilihan: ", 1, 2
                );

                if (keluar == 2) {
                    berjalan = false;
                }

                break;
            }
        }

        if (berjalan) {
            cout << "\n";
            cout << "1. Kembali ke menu utama\n";
            cout << "2. Keluar dari aplikasi\n";

            int kembali = inputAngka(
                "Pilihan: ", 1, 2
            );

            if (kembali == 2) {
                cout << "\nApakah Anda yakin ingin keluar?\n";
                cout << "1. Kembali ke menu utama\n";
                cout << "2. Ya, keluar\n";

                int konfirmasi = inputAngka(
                    "Pilihan: ", 1, 2
                );

                if (konfirmasi == 2) {
                    berjalan = false;
                }
            }
        }
    }

    cout << "\n";
    garis();
    cout << "Terima kasih telah menggunakan BUSKU TRAVEL.\n";
    cout << "Semoga perjalanan Anda menyenangkan!\n";
    garis();

    return 0;
}
