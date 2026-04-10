#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <limits>

#ifdef _WIN32
#include <conio.h>
#define CLEAR_CMD "cls"
#else
#define CLEAR_CMD "clear"
// getch() emulyasiyasi Linux/Mac ���n
#include <termios.h>
#include <unistd.h>
int getch() {
    termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

using namespace std;

// ----------------------------------------------
//  Sabitl?r
// ----------------------------------------------
static const char* MAIN_FILE  = "hospital.dat";
static const char* TEMP_FILE  = "hos_temp.dat";

// ----------------------------------------------
//  K�m?k�i funksiyalar
// ----------------------------------------------
void clearScreen() { system(CLEAR_CMD); }

void printLine(char c = '-', int len = 65) {
    for (int i = 0; i < len; ++i) cout << c;
    cout << '\n';
}

void printHeader(const char* title) {
    printLine('=');
    cout << "   " << title << '\n';
    printLine('=');
}

// B�y�k/ki�ik h?rf? h?ssas olmayan m�qayis?
int strIgnoreCase(const char* a, const char* b) {
#ifdef _WIN32
    return _stricmp(a, b);
#else
    return strcasecmp(a, b);
#endif
}

// Integer oxuma � x?tali giris halinda yenid?n sorusur
int readInt(const char* prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return val; }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Xeta: Zehmetolmasa reqem daxil edin.\n";
    }
}

float readFloat(const char* prompt) {
    float val;
    while (true) {
        cout << prompt;
        if (cin >> val && val > 0) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return val; }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Xeta: Musbat reqem daxil edin.\n";
    }
}

// Yalniz g�zl?nil?n d?y?rl?rd?n birini q?bul edir
void readValidated(const char* prompt, char* dest, size_t destSize,
                   const char* choices[], int choiceCount) {
    char buf[64];
    while (true) {
        cout << prompt;
        cin.getline(buf, sizeof(buf));
        for (int i = 0; i < choiceCount; ++i)
            if (strIgnoreCase(buf, choices[i]) == 0) {
                strncpy(dest, buf, destSize - 1);
                dest[destSize - 1] = '\0';
                return;
            }
        cout << "  [!] Xeta: Duzgun secim edin (";
        for (int i = 0; i < choiceCount; ++i) {
            cout << choices[i];
            if (i < choiceCount - 1) cout << "/";
        }
        cout << ").\n";
    }
}

void readString(const char* prompt, char* dest, size_t destSize) {
    cout << prompt;
    cin.getline(dest, destSize);
}

// ----------------------------------------------
//  X?st? sinifi
// ----------------------------------------------
class Hospital {
    int   pid;
    char  pname[60];
    char  fname[60];
    char  mname[60];
    char  hname[60];
    char  ren[60];    // referans ed?nin adi
    char  rer[60];    // referans ed?nin m�nasib?ti
    char  gen[20];    // male / female
    float age;
    char  mob[20];
    char  status[10]; // yes / no (evli?)

public:
    // Konstruktor � b�t�n sah?l?ri sifirla
    Hospital() { clear(); }

    void clear() {
        pid = 0; age = 0.0f;
        memset(pname, 0, sizeof(pname));
        memset(fname, 0, sizeof(fname));
        memset(mname, 0, sizeof(mname));
        memset(hname, 0, sizeof(hname));
        memset(ren,   0, sizeof(ren));
        memset(rer,   0, sizeof(rer));
        memset(gen,   0, sizeof(gen));
        memset(mob,   0, sizeof(mob));
        memset(status,0, sizeof(status));
    }

    // M?lumat daxil etm?
    void input() {
        printHeader("YENI XESTE MELUMATI");

        pid = readInt("  Xeste ID          : ");
        readString  ("  Ad Soyad          : ", pname, sizeof(pname));
        age = readFloat("  Yas               : ");

        const char* genders[] = { "male", "female" };
        readValidated("  Cins (male/female): ", gen, sizeof(gen), genders, 2);

        const char* yesno[] = { "yes", "no" };
        readValidated("  Evlidir? (yes/no) : ", status, sizeof(status), yesno, 2);

        if (strIgnoreCase(gen, "female") == 0) {
            if (strIgnoreCase(status, "yes") == 0) {
                readString("  Erin adi          : ", hname, sizeof(hname));
            } else {
                readString("  Atanin adi        : ", fname, sizeof(fname));
                readString("  Ananin adi        : ", mname, sizeof(mname));
            }
        } else {
            readString("  Atanin adi        : ", fname, sizeof(fname));
            readString("  Referans verenis  : ", ren,   sizeof(ren));
            readString("  Referans munas.   : ", rer,   sizeof(rer));
            readString("  Mobil nomre       : ", mob,   sizeof(mob));
        }
    }

    // M?lumat g�st?rm?
    void display() const {
        printLine('-');
        cout << "  ID              : " << pid   << '\n';
        cout << "  Ad Soyad        : " << pname << '\n';
        cout << "  Yas             : " << age   << '\n';
        cout << "  Cins            : " << gen   << '\n';
        cout << "  Evlilik         : " << (strIgnoreCase(status,"yes")==0 ? "Evli" : "Subay") << '\n';

        if (strIgnoreCase(gen, "female") == 0) {
            if (strIgnoreCase(status, "yes") == 0)
                cout << "  Erin adi        : " << hname << '\n';
            else {
                cout << "  Atanin adi      : " << fname << '\n';
                cout << "  Ananin adi      : " << mname << '\n';
            }
        } else {
            cout << "  Atanin adi      : " << fname << '\n';
            cout << "  Referans (ad)   : " << ren   << '\n';
            cout << "  Referans (mun.) : " << rer   << '\n';
            cout << "  Mobil           : " << mob   << '\n';
        }
        printLine('-');
    }

    int  getId()  const { return pid; }
    float getAge() const { return age; }

    // M?lumat yenil?m?
    void update() {
        printLine();
        cout << "  Ne deyisdirmek isteyirsiniz?\n"
             << "  [1] Ad/Soyad   [2] Ata adi   [3] Er adi\n"
             << "  [4] Ana adi    [5] Mobil      [6] Evlilik statusu\n"
             << "  [0] Vazgec\n";
        printLine();
        int ch = readInt("  Secim: ");

        char buf[64];
        switch (ch) {
            case 1: readString("  Yeni ad/soyad : ", buf, sizeof(buf)); strncpy(pname, buf, sizeof(pname)-1); break;
            case 2: readString("  Yeni ata adi  : ", buf, sizeof(buf)); strncpy(fname, buf, sizeof(fname)-1); break;
            case 3: readString("  Yeni er adi   : ", buf, sizeof(buf)); strncpy(hname, buf, sizeof(hname)-1); break;
            case 4: readString("  Yeni ana adi  : ", buf, sizeof(buf)); strncpy(mname, buf, sizeof(mname)-1); break;
            case 5: readString("  Yeni mobil    : ", buf, sizeof(mob)-1); strncpy(mob,  buf, sizeof(mob)-1);  break;
            case 6: {
                const char* yn[] = {"yes","no"};
                readValidated("  Evlidir? (yes/no): ", status, sizeof(status), yn, 2);
                break;
            }
            case 0: cout << "  Deyisiklik l?gv edildi.\n"; break;
            default: cout << "  Etibarsiz secim.\n"; break;
        }
    }
};

// ----------------------------------------------
//  ID m�vcuddurmu? yoxlama
// ----------------------------------------------
bool idExists(int id) {
    ifstream file(MAIN_FILE, ios::binary);
    if (!file) return false;
    Hospital h;
    while (file.read(reinterpret_cast<char*>(&h), sizeof(h)))
        if (h.getId() == id) return true;
    return false;
}

// ----------------------------------------------
//  �mumi x?st? sayi
// ----------------------------------------------
int totalPatients() {
    ifstream file(MAIN_FILE, ios::binary);
    if (!file) return 0;
    file.seekg(0, ios::end);
    long size = (long)file.tellg();
    return (size > 0) ? (int)(size / sizeof(Hospital)) : 0;
}

// ----------------------------------------------
//  1. X?st? ?lav? et
// ----------------------------------------------
void addPatient() {
    cout << "\n  [!] Ad/soyad ve diger sahelerde BOSLUG OLMAMALIDIR.\n\n";
    Hospital h;
    h.input();

    if (idExists(h.getId())) {
        cout << "  [!] Bu ID artiq m�vcuddur. Elave edilmedi.\n";
        return;
    }

    fstream file(MAIN_FILE, ios::binary | ios::out | ios::app);
    if (!file) { cout << "  [!] Fayl acilamadi.\n"; return; }
    file.write(reinterpret_cast<const char*>(&h), sizeof(h));
    file.close();
    cout << "\n  [?] Xeste ugurla elave edildi. Umumi: " << totalPatients() << " xeste.\n";
}

// ----------------------------------------------
//  2. B�t�n x?st?l?ri g�st?r
// ----------------------------------------------
void showAll() {
    printHeader("BUTUN XESTE SIYAHISI");
    ifstream file(MAIN_FILE, ios::binary);
    if (!file) { cout << "  Hec bir xeste tapilmadi.\n"; return; }

    Hospital h;
    int count = 0;
    while (file.read(reinterpret_cast<char*>(&h), sizeof(h))) {
        h.display();
        ++count;
    }
    file.close();
    if (count == 0) cout << "  Hec bir xeste tapilmadi.\n";
    else            cout << "\n  Umumi: " << count << " xeste.\n";
}

// ----------------------------------------------
//  3. ID il? axtaris
// ----------------------------------------------
void searchById() {
    printHeader("ID ILE AXTARIS");
    ifstream file(MAIN_FILE, ios::binary);
    if (!file) { cout << "  Xeste qeydi tapilmadi.\n"; return; }

    int id = readInt("  Xeste ID: ");
    Hospital h;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&h), sizeof(h))) {
        if (h.getId() == id) { h.display(); found = true; }
    }
    file.close();
    if (!found) cout << "  Bu ID-e sahib xeste tapilmadi.\n";
}

// ----------------------------------------------
//  4. Yasa g�r? axtaris (YENI)
// ----------------------------------------------
void searchByAge() {
    printHeader("YAS ARALIGINA GORE AXTARIS");
    float minAge = readFloat("  Minimum yas: ");
    float maxAge = readFloat("  Maximum yas: ");

    ifstream file(MAIN_FILE, ios::binary);
    if (!file) { cout << "  Xeste qeydi tapilmadi.\n"; return; }

    Hospital h;
    int count = 0;
    while (file.read(reinterpret_cast<char*>(&h), sizeof(h))) {
        if (h.getAge() >= minAge && h.getAge() <= maxAge) {
            h.display();
            ++count;
        }
    }
    file.close();
    if (count == 0) cout << "  Bu yas araliginda xeste tapilmadi.\n";
    else            cout << "\n  Tapilan: " << count << " xeste.\n";
}

// ----------------------------------------------
//  5. ID siyahisini g�st?r (YENI)
// ----------------------------------------------
void listIds() {
    printHeader("MOVCUD XESTE ID SIYAHISI");
    ifstream file(MAIN_FILE, ios::binary);
    if (!file) { cout << "  Xeste qeydi tapilmadi.\n"; return; }

    Hospital h;
    int count = 0;
    while (file.read(reinterpret_cast<char*>(&h), sizeof(h))) {
        cout << "  ID: " << h.getId() << "  |  Ad: ";
        // sad? display ���n yenid?n oxuyuruq
        h.display();
        ++count;
    }
    file.close();
    cout << "\n  Umumi: " << count << " qeyd.\n";
}

// ----------------------------------------------
//  6. ID araligina g�r? �ixar (k�hn?: transpose)
// ----------------------------------------------
void exportRange() {
    printHeader("ID ARALIGINA GORE IXRAC");
    ifstream file(MAIN_FILE, ios::binary);
    if (!file) { cout << "  Xeste qeydi tapilmadi.\n"; return; }

    int id1 = readInt("  Baslanggic ID: ");
    int id2 = readInt("  Bitis      ID: ");
    if (id1 > id2) swap(id1, id2);

    ofstream ofile(TEMP_FILE, ios::binary | ios::trunc);
    Hospital h;
    int count = 0;
    while (file.read(reinterpret_cast<char*>(&h), sizeof(h))) {
        if (h.getId() >= id1 && h.getId() <= id2) {
            ofile.write(reinterpret_cast<const char*>(&h), sizeof(h));
            ++count;
        }
    }
    file.close();
    ofile.close();
    if (count == 0) cout << "  Bu araligda xeste tapilmadi.\n";
    else            cout << "  [?] " << count << " xeste " << TEMP_FILE << " faylina ixrac edildi.\n";
}

// ----------------------------------------------
//  7. Ixrac edilmis fayli g�st?r
// ----------------------------------------------
void showExported() {
    printHeader("IXRAC EDILMIS XESTELER");
    ifstream file(TEMP_FILE, ios::binary);
    if (!file) { cout << "  Ixrac edilmis qeyd tapilmadi. Evvelce ixrac edin.\n"; return; }

    Hospital h;
    int count = 0;
    while (file.read(reinterpret_cast<char*>(&h), sizeof(h))) {
        h.display();
        ++count;
    }
    file.close();
    if (count == 0) cout << "  Ixrac edilmis qeyd yoxdur.\n";
    else            cout << "\n  Umumi: " << count << " qeyd.\n";
}

// ----------------------------------------------
//  8. X?st?ni sil
// ----------------------------------------------
void deletePatient() {
    printHeader("XESTE SIL");
    ifstream file(MAIN_FILE, ios::binary);
    if (!file) { cout << "  Xeste qeydi tapilmadi.\n"; return; }

    int id = readInt("  Silmek istediginiz xeste ID: ");

    ofstream ofile(TEMP_FILE, ios::binary | ios::trunc);
    Hospital h;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&h), sizeof(h))) {
        if (h.getId() == id) {
            cout << "  Silinecek xeste:\n";
            h.display();
            found = true;
            // bu qeydi yazmiriq ? silinir
        } else {
            ofile.write(reinterpret_cast<const char*>(&h), sizeof(h));
        }
    }
    file.close();
    ofile.close();

    if (!found) {
        cout << "  Bu ID-e sahib xeste tapilmadi.\n";
        remove(TEMP_FILE);
        return;
    }
    char conf[10];
    cout << "  Tesdiq edin (yes/no): ";
    cin.getline(conf, sizeof(conf));
    if (strIgnoreCase(conf, "yes") == 0) {
        remove(MAIN_FILE);
        rename(TEMP_FILE, MAIN_FILE);
        cout << "  [?] Xeste silindi. Qalan: " << totalPatients() << " xeste.\n";
    } else {
        remove(TEMP_FILE);
        cout << "  Silme legv edildi.\n";
    }
}

// ----------------------------------------------
//  9. B�t�n qeydl?ri sil
// ----------------------------------------------
void eraseAll() {
    printHeader("BUTUN QEYDLERI SIL");
    cout << "  [!] Bu emeliyyat GERI ALINMAZ!\n";
    cout << "  Teyin edin (yes/no): ";
    char choice[10];
    cin.getline(choice, sizeof(choice));
    if (strIgnoreCase(choice, "yes") == 0) {
        ofstream file(MAIN_FILE, ios::binary | ios::trunc);
        file.close();
        cout << "  [?] Butun qeydler silindi.\n";
    } else {
        cout << "  Emeliyyat legv edildi.\n";
    }
}

// ----------------------------------------------
//  10. X?st? m?lumatini d?yisdir
// ----------------------------------------------
void modifyPatient() {
    printHeader("XESTE MELUMATINI DEYIS");
    fstream file(MAIN_FILE, ios::binary | ios::in | ios::out);
    if (!file) { cout << "  Xeste qeydi tapilmadi.\n"; return; }

    int id = readInt("  Deyisdirmek istediginiz xeste ID: ");
    Hospital h;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&h), sizeof(h))) {
        if (h.getId() == id) {
            found = true;
            cout << "  Movcud melumat:\n";
            h.display();
            // yazma pozisyonunu geri �?kirik
            streamoff pos = file.tellg();
            pos -= static_cast<streamoff>(sizeof(h));
            h.update();
            file.seekp(pos);
            file.write(reinterpret_cast<const char*>(&h), sizeof(h));
            cout << "  [?] Melumat yenilendi.\n";
        }
    }
    file.close();
    if (!found) cout << "  Bu ID-e sahib xeste tapilmadi.\n";
}

// ----------------------------------------------
//  11. Statistika (YENI)
// ----------------------------------------------
void statistics() {
    printHeader("STATISTIKA");
    ifstream file(MAIN_FILE, ios::binary);
    if (!file) { cout << "  Xeste qeydi tapilmadi.\n"; return; }

    int total = 0, males = 0, females = 0, married = 0;
    float totalAge = 0;
    Hospital h;
    while (file.read(reinterpret_cast<char*>(&h), sizeof(h))) {
        ++total;
        totalAge += h.getAge();
        if (strIgnoreCase(h.getId() >= 0 ? "ok" : "no", "ok") == 0) {} // sad? check
        // cins sayi ���n display() �agirmadan birbasa isl? � getId() public, dig?rl?ri yox
        // getAge() var, gender ���n ayrica getter ?lav? etm?yin lazimi yoxdur:
        // Bu blok statistics() ���n kifay?t edir.
        ++males; // placeholder: real sayim ���n asagida h?ll
    }
    file.close();

    // Daha d�zg�n hesablama ���n fayli yenid?n oxu
    ifstream file2(MAIN_FILE, ios::binary);
    total = males = females = married = 0; totalAge = 0;

    // Hospital sinifini genisl?ndirilmis getter-larla istifad? etm?k ���n
    // struct-a �eviririk (fayl formati d?yismir):
    struct RawRec {
        int   pid;
        char  pname[60], fname[60], mname[60], hname[60];
        char  ren[60], rer[60], gen[20];
        float age;
        char  mob[20], status[10];
    };
    RawRec r;
    while (file2.read(reinterpret_cast<char*>(&r), sizeof(r))) {
        ++total;
        totalAge += r.age;
        if (strIgnoreCase(r.gen, "male") == 0)   ++males;
        else                                       ++females;
        if (strIgnoreCase(r.status, "yes") == 0)  ++married;
    }
    file2.close();

    if (total == 0) { cout << "  Melumat yoxdur.\n"; return; }
    cout << "  Umumi xeste sayi : " << total   << '\n';
    cout << "  Kisi             : " << males   << '\n';
    cout << "  Qadin            : " << females << '\n';
    cout << "  Evli             : " << married << '\n';
    cout << "  Orta yas         : " << (totalAge / total) << '\n';
    printLine();
}

// ----------------------------------------------
//  Xos g?lmisiniz ekrani
// ----------------------------------------------
void welcome() {
    clearScreen();
    printLine('*');
    cout << '\n';
    cout << "        XESTE IDAR?ETM? SISTEMI\n";
    cout << "        Hospital Management System\n\n";
    cout << "        Versiya 2.0  |  C++17\n\n";
    printLine('*');
    cout << "\n  Davam etmek ucun ist?nil?n duse basin...\n";
    getch();
    clearScreen();
}

// ----------------------------------------------
//  ?sas menyu
// ----------------------------------------------
void showMenu() {
    printLine('=');
    cout << "   XESTE IDAR?ETM? SISTEMI  �  ANA MENYU\n";
    printLine('=');
    cout << "   [1]  Xeste elave et\n";
    cout << "   [2]  Butun xesteteri goster\n";
    cout << "   [3]  ID ile axtar\n";
    cout << "   [4]  Yas araligina gore axtar\n";
    cout << "   [5]  ID araligina gore ixrac et\n";
    cout << "   [6]  Ixrac edilmis qeydleri goster\n";
    cout << "   [7]  Xeste sil\n";
    cout << "   [8]  Butun qeydleri sil\n";
    cout << "   [9]  Xeste melumatini deyis\n";
    cout << "   [10] Statistika\n";
    cout << "   [0]  Cixis\n";
    printLine('=');
    cout << "   Secim: ";
}

// ----------------------------------------------
//  main()
// ----------------------------------------------
int main() {
    welcome();

    int choice;
    while (true) {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Xeta: reqem daxil edin.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        clearScreen();
        switch (choice) {
            case  1: addPatient();    break;
            case  2: showAll();       break;
            case  3: searchById();    break;
            case  4: searchByAge();   break;
            case  5: exportRange();   break;
            case  6: showExported();  break;
            case  7: deletePatient(); break;
            case  8: eraseAll();      break;
            case  9: modifyPatient(); break;
            case 10: statistics();    break;
            case  0: {
                cout << "  Cixmaq isteyirsiniz? (yes/no): ";
                char c[10];
                cin.getline(c, sizeof(c));
                if (strIgnoreCase(c, "yes") == 0) {
                    cout << "\n  Gorushenedek!\n\n";
                    return 0;
                }
                break;
            }
            default: cout << "  [!] Etibarsiz secim.\n"; break;
        }

        cout << "\n  Davam etmek ucun ist?nil?n duse basin...\n";
        getch();
        clearScreen();
    }
}
