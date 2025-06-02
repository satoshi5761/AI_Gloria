import math
import json

def muat_database(nama_file="database_dosen.json"):
    try:
        with open(nama_file, 'r', encoding='utf-8') as f:
            db = json.load(f)
        return db
    except FileNotFoundError:
        print(f"Error: File database '{nama_file}' tidak ditemukan. Pastikan file ada di direktori yang sama.")
        return None
    except json.JSONDecodeError:
        print(f"Error: Format JSON dalam file '{nama_file}' tidak valid.")
        return None

def forward_chaining(kandidat, jawaban_user_untuk_sifat_ini):
    hasil = {}
    sifat_ditanya, nilai_jawaban = next(iter(jawaban_user_untuk_sifat_ini.items()))

    for nama, sifat_dosen in kandidat.items():
        if sifat_dosen.get(sifat_ditanya) == nilai_jawaban:
            hasil[nama] = sifat_dosen
    return hasil

def entropy(prob):
    if prob == 0 or prob == 1:
        return 0
    return -prob * math.log2(prob) - (1 - prob) * math.log2(1 - prob)

def hitung_information_gain(kandidat, pertanyaan_tersisa):
    total_kandidat = len(kandidat)
    if total_kandidat == 0:
        return {p: 0 for p in pertanyaan_tersisa}

    gain_pertanyaan = {}
    for pertanyaan in pertanyaan_tersisa:
        count_1 = sum(1 for sifat_dosen in kandidat.values() if sifat_dosen.get(pertanyaan) == 1)
        prop_1 = count_1 / total_kandidat
        
        if prop_1 == 0 or prop_1 == 1:
            info_gain = 0
        else:
            entropy_attribute = - (prop_1 * math.log2(prop_1) if prop_1 > 0 else 0) \
                                - ((1 - prop_1) * math.log2(1 - prop_1) if (1 - prop_1) > 0 else 0)
            info_gain = 1 - entropy_attribute
        gain_pertanyaan[pertanyaan] = round(info_gain, 4)
    return gain_pertanyaan

def main():
    dosen_db = muat_database()
    if not dosen_db:
        return

    kandidat = dosen_db.copy()
    jawaban_user_kumulatif = {}

    if not kandidat:
        print("Database dosen kosong.")
        return
        
    try:
        semua_pertanyaan_potensial = list(next(iter(kandidat.values())).keys())
    except StopIteration:
        print("Database dosen tampaknya tidak memiliki entri data dosen yang valid.")
        return
        
    pertanyaan_sisa = semua_pertanyaan_potensial.copy()

    print("Selamat datang di Penebak Dosen!")
    print("Jawablah pertanyaan berikut dengan 'y' (ya), 'n' (tidak), atau 'r' (ragu-ragu).")

    while len(kandidat) > 1 and pertanyaan_sisa:
        gain_pertanyaan = hitung_information_gain(kandidat, pertanyaan_sisa)

        if not gain_pertanyaan or all(g == 0 for g in gain_pertanyaan.values()):
            if not pertanyaan_sisa:
                 print("\nTidak ada pertanyaan lagi yang bisa ditanyakan.")
            else:
                 print("\nTidak ada pertanyaan yang cukup membedakan untuk kandidat saat ini.")
            break
        
        pertanyaan_terbaik = max(gain_pertanyaan, key=gain_pertanyaan.get)

        print(f"\nSisa kandidat: {len(kandidat)}")
        if len(kandidat) < 10:
             print(f"Kandidat saat ini: {list(kandidat.keys())}")
        print(f"Pertanyaan: Apakah dosennya memiliki sifat '{pertanyaan_terbaik}'? (y/n/r)")
        jawab = input("> ").strip().lower()

        if jawab == 'y':
            nilai_jawaban = 1
            jawaban_user_kumulatif[pertanyaan_terbaik] = nilai_jawaban
            kandidat = forward_chaining(kandidat, {pertanyaan_terbaik: nilai_jawaban})
        elif jawab == 'n':
            nilai_jawaban = 0
            jawaban_user_kumulatif[pertanyaan_terbaik] = nilai_jawaban
            kandidat = forward_chaining(kandidat, {pertanyaan_terbaik: nilai_jawaban})
        elif jawab == 'r':
            print("Baik, saya akan mengabaikan sifat ini untuk sementara.")
        else:
            print("Jawaban tidak valid. Harap ketik 'y', 'n', atau 'r'.")
            continue

        if pertanyaan_terbaik in pertanyaan_sisa:
            pertanyaan_sisa.remove(pertanyaan_terbaik)
        
        if not kandidat:
            break

    print("-" * 30)
    if len(kandidat) == 1:
        dosen_tertebak = list(kandidat.keys())[0]
        print(f"Dosen yang Anda maksud kemungkinan adalah: {dosen_tertebak}")
    elif len(kandidat) > 1:
        print("Saya belum bisa menentukan dengan pasti, tapi mungkin salah satu dari berikut:")
        for nama_dosen in kandidat.keys():
            print(f"- {nama_dosen}")
        if not pertanyaan_sisa:
            print("Saya sudah kehabisan pertanyaan untuk membedakan mereka lebih lanjut.")
    else:
        print("Maaf, tidak ada dosen di database saya yang cocok dengan semua jawaban yang Anda berikan.")

if __name__ == "__main__":
    main()