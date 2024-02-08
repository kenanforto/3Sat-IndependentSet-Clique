#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

class Formula_3_CNF {

private:
  vector<vector<int>> formula;
  vector<vector<int>> tabelaIstine;
  int brojVarijabli = 0;

  int odrediBrojVarijabli() {
    int maxVar = 0;

    for (const auto &klauza : formula) {
      for (int literal : klauza) {
        int var = abs(literal); 
        maxVar = max(maxVar, var);
      }
    }

    return maxVar;
  }

  // Metoda za generiranje tabele istine na osnovu broja varijabli
  void generisiTabeluIstine(int brojVarijabli) {
    int brojRedova = pow(2, brojVarijabli); 

    tabelaIstine.resize(brojRedova, vector<int>(brojVarijabli,0)); // Inicijalizacija tabele istine s nulama

    for (int red = 0; red < brojRedova; red++) {
      for (int var = 0; var < brojVarijabli; var++) {
        // Postavljanje vrijednosti varijabli u redu koristeći bitshift operacije
        tabelaIstine[red][var] = (red >> (brojVarijabli - 1 - var)) & 1; // 5 >> (3 - 1 - 1) <-> 5 >> 1 // 101 - > 10
      }
    }
  }
  // formula je ispunjiva ako postoji kombinacija vrijednosti varijabli koja cini svaku klauzulu istinitom.
  bool provjeriIspunjivost() {
      //int ispunjiva=0;
    for (const auto &klauza : formula) {
      bool klauzaIspunjena = false;

      // Iteracija kroz sve moguće kombinacije vrijednosti varijabli (tabele istine)
      for (const auto &red : tabelaIstine) {
        bool klauzaReda = true;

        // Iteracija kroz literale u trenutnoj klauzi, provjeravmo vrijednosti tabeleisitine
        for (int literal : klauza) {
          int var = abs(literal);
          bool negacija = (literal < 0);
          bool vrijednost = red[var - 1] == 1;

          // Provjera ispunjivosti trenutnog literala u tabeli istine
          klauzaReda = klauzaReda &&
                       ((vrijednost && !negacija) || (!vrijednost && negacija));
          // cout<<"Klauza reda "<<klauzaReda<<endl;
        }

        // Ako je klauza ispunjena u barem jednom retku tabele istine, označi da
        // je klauza ispunjena
        klauzaIspunjena = klauzaIspunjena || klauzaReda;
        // if(klauzaIspunjena)
        // {
        //     ispunjiva++;
        // }
      }

      // Ako klauza nije ispunjena ni u jednom retku tabele istine, formula nije
      // ispunjiva
      if (!klauzaIspunjena) {
        return false;
      }
    }

    // Ako su sve klauze ispunjene barem jednom, formula je ispunjiva
    //if(ispunjiva>=2){
    return true;
    //}
    //else return false;
  }

public:
  void Unos3CNF() {
    cout << "Unesite broj klauzula manji od 8.\n";
    int brKlauzula = 0;

    do {
      cin >> brKlauzula;
      if (brKlauzula > 8)
        cout << "Broj konjunkcija mora biti manji od 8.";
    } while (brKlauzula > 8);

    // Petlja za unos svake klauze
    for (int i = 0; i < brKlauzula; i++) {
      cout << "Unesite " << (i + 1)
           << ". elementarnu disjunkciju kao 3 uzastopna cijela broja "
              "razdvojena zarezom.\n";

      string disjunkcija;
      cin >> disjunkcija;

      // Zamjena zareza s razmakom radi kasnijeg parsiranja
      replace(disjunkcija.begin(), disjunkcija.end(), ',', ' ');

      vector<int> rezultat;

      // Parsiranje stringa u vektor integera
      stringstream ss(disjunkcija);
      int temp;
      while (ss >> temp) {
        rezultat.push_back(temp);
      }

      // Dodavanje rezultata (vektora literala) u formulu
      formula.push_back(rezultat);

      // Ispis unesene formule
      for (int i = 0; i < formula.size(); i++) {
        for (int j = 0; j < formula[i].size(); j++) {
          cout << formula[i][j] << " ";
        }
        cout << endl;
      }
    }

    // Ispis poruke o završetku unosa formule
    cout << "Unos formule je zavrsen.\n";

    // Generiranje tabele istine na temelju broja varijabli
    generisiTabeluIstine(odrediBrojVarijabli());
  }
  
  // vraca 1 ako je ispunjiva
  bool RJESENJE_3_CNF() {
    int brojVarijabli = odrediBrojVarijabli();
    if (brojVarijabli > 24) {
      cout << "Broj varijabli prelazi ogranicenje.";
      return false;
    }
    // generisiTabeluIstine(brojVarijabli); ovo sam prebacil u unos3CNF, jer bih
    // u suprotnom morala svaki put pozvati rjesenje3cnf nakon unos3cnf
    return provjeriIspunjivost();
  }

  bool VERIFIKACIJA_3_CNF(const vector<int> &literali) {
    int brojVarijabli = odrediBrojVarijabli();

    if (brojVarijabli > 24) {
      cout << "Broj varijabli prelazi ogranicenje.";
      return false;
    }

    if (literali.size() != brojVarijabli) {
      cout << "Pogresan broj literala u pridruživanju.";
      return false;
    }

    // Postavi vrijednosti literala u prvi red tabele istine
    for (int i = 0; i < brojVarijabli; i++) {
      tabelaIstine[0][i] = (literali[i] == 1);
    }

    return provjeriIspunjivost();
  }

  vector<vector<int>> vratiFormulu() { return formula; }
};

// Zadatak 2
template <typename T> class KlikaINDSETGraf {

private:
std::vector<std::vector<T>> matricaSusjedstva = std::vector<std::vector<T>>(15, std::vector<T>(15, T()));
std::vector<std::vector<T>> invmatricaSusjedstva = std::vector<std::vector<T>>(15, std::vector<T>(15, 1));
int brojCvorova = 0;

  // Pomoćna funkcija koja dodaje čvor u skup ako već nije tamo.
  void dodajAkoNijeUSet(vector<int> &skup, int cvor) {
    if (std::find(skup.begin(), skup.end(), cvor) == skup.end()) {
      skup.push_back(cvor);
    }
  }

  // Pomoćna funkcija koja vraća susjedne čvorove za zadani čvor u matrici
  // susjedstva.
  vector<int> susjedi(const vector<vector<int>> &matricaSusjedstva, int cvor) {
    vector<int> susjedniCvorovi;
    for (int i : matricaSusjedstva[cvor]) {
    if (matricaSusjedstva[cvor][i] == 1 || matricaSusjedstva[i][cvor] == 1) {
        susjedniCvorovi.push_back(i);
    }
    }
    return susjedniCvorovi;
  }

  // Pomoćna funkcija koja provjerava postojanje k-klike ili k-nezavisnog skupa.
  bool postojiKSet(const vector<vector<int>> &matricaSusjedstva, int k,
                   bool jeKlika) {
    int brojCvorova = matricaSusjedstva.size();

    // Inicijalizacija skupa čvorova koji tvore k-kliku ili k-nezavisni skup.
    vector<int> kSet;

    // Implementacija algoritma za pronalaženje k-klike ili k-nezavisnog skupa.
    for (int i = 0; i < brojCvorova; ++i) {
      // Ako trenutni čvor već pripada k-klici ili k-nezavisnom skupu, preskoči
      // ga.
      if (std::find(kSet.begin(), kSet.end(), i) != kSet.end()) {
        continue;
      }

      // Dodaj trenutni čvor u k-kliku ili k-nezavisnom skupu.
      dodajAkoNijeUSet(kSet, i);

      // Povećavaj veličinu k-klike ili k-nezavisnog skupa sve dok postoji
      // susjedni čvor.
      while (true) {
        bool susjedDodan = false;
        for (int j : kSet) {
          for (int susjed : susjedi(matricaSusjedstva, j)) {
            // Ako čvor susjed nije već u k-klici ili k-nezavisnom skupu.
            if (std::find(kSet.begin(), kSet.end(), susjed) == kSet.end()) {
              dodajAkoNijeUSet(kSet, susjed);
              susjedDodan = true;
            }
          }
        }

        // Ako nismo dodali nijedan susjed, završavamo s dodavanjem čvorova u
        // k-kliku ili k-nezavisni skup.
        if (!susjedDodan) {
          break;
        }
      }

      // Ako smo dosegli k-kliku ili k-nezavisni skup veličine barem k, vraćamo
      // true.
      if (kSet.size() >= k) {
        return true;
      }

      // Inače, čvorovi u k-klici ili k-nezavisnom skupu nisu dovoljni, pa ih
      // uklanjamo i pokušavamo s drugim čvorom.
      kSet.clear();
    }

    // Ako smo došli do ovdje, ne postoji k-klika ili k-nezavisni skup.
    return false;
  }

  void popraviInvMat() {
    for (int i = 0; i < invmatricaSusjedstva.size(); i++) {
      for (int j = 0; j < invmatricaSusjedstva[0].size(); j++)
        if (j == i)
          invmatricaSusjedstva[i][j] = 0;
    }
  }

public:
  KlikaINDSETGraf() = default;

  void unosGrafa() {
    std::cout << "Unesite broj čvorova (do 15): ";
    std::cin >> brojCvorova;

    if (brojCvorova > 15) {
      std::cout << "Broj čvorova premašuje maksimalni dopušteni broj."
                << std::endl;
      return;
    }
    invmatricaSusjedstva.resize(brojCvorova, std::vector<T>(brojCvorova, 1));
    std::cout << "Unesite grane grafa (završetak unosa unosom -1):"
              << std::endl;
    int u, v;
    while (true) {
      std::cin >> u;
      if (u == -1)
        break;
      std::cin >> v;
      if (v == -1)
        break;

      if (u < 1 || u > brojCvorova || v < 1 || v > brojCvorova) {
        std::cout << "Neispravan unos čvorova." << std::endl;
        continue;
      }

      matricaSusjedstva[u - 1][v - 1] = 1;
      matricaSusjedstva[v - 1][u - 1] = 1; // Graf je neusmjeren
      invmatricaSusjedstva[u - 1][v - 1] = 0;
      invmatricaSusjedstva[v - 1][u - 1] = 0; // pravili pomocnu matricu koja ce nam sluziti za k nezavisni skup varijabli
    }

    popraviInvMat();

    cout << "Graf predstavljen preko matrice susjedstva:\n";
    for (int i = 0; i < brojCvorova; i++) {
      for (int j = 0; j != brojCvorova; j++)
        cout << matricaSusjedstva[i][j] << " ";
      cout << "\n";
    }
  }

bool RJESENJE_K_INDSET(int k) {

    return postojiKSet(invmatricaSusjedstva, k, false);
  }

bool RJESENJE_K_CLIQUE(int k) {
    return postojiKSet(matricaSusjedstva, k, true);
  }

bool VERIFIKACIJA_INDSET(const vector<int> &certifikat) {
    for (int i = 0; i < certifikat.size(); ++i) {
      for (int j = 0; j < certifikat.size(); ++j) {
        if (i == j)
          continue;
        if (matricaSusjedstva[certifikat[i]][certifikat[j]] ||
            matricaSusjedstva[certifikat[j]][certifikat[i]])
          return false;
      }
    }
    return true;
  }

  bool VERIFIKACIJA_K_CLIQUE(const vector<int> &certifikat) {
    for (int i = 0; i < certifikat.size(); ++i) {
      for (int j = 0; j < certifikat.size(); ++j) {
        if (i == j)
          continue;
        if (!matricaSusjedstva[certifikat[i]][certifikat[j]] &&
            !matricaSusjedstva[certifikat[j]][certifikat[i]])
          return false;
      }
    }
    return true;
  }

  vector<vector<int>> satToIndset(Formula_3_CNF F) {
    vector<vector<int>> formula = F.vratiFormulu();
    int k = formula.size() * 3;
    vector<vector<int>> result(k, vector<int>(k, 0));
    for (int i = 0; i < k; i += 3) {
      result[i][i + 1] = 1;
      result[i][i + 2] = 1;
      result[i + 1][i] = 1;
      result[i + 1][i + 2] = 1;
      result[i + 2][i] = 1;
      result[i + 2][i + 1] = 1;
    }
    for (int i = 0; i < formula.size(); ++i) {
      for (int j = 0; j < formula[i].size();
           ++j) // idemo i petljom kroz formulu a j petljom kroz svaku zasebnu
                // klauzu koja je u nasem slucaju 3 elementa
      {
        int literal = formula[i][j]; // treutni clan koji cemo porediti
        for (k = i + 1; k < formula.size();
             ++k) // idemo u sljedece klauze da vidimo ima li isti ko nac clan
        {
          for (int l = 0; l < formula[k].size(); ++l) {
            int literalNext = formula[k][l]; // element sa kojim poredimo
            if (literal != literalNext && abs(literal) == abs(literalNext)) {
              result[i * 3 + j][k * 3 + l] = 1;
              result[k * 3 + l][i * 3 + j] = 1;
            }
          }
        }
      }
    }
    return result;
  }

  vector<vector<int>> satToClique(Formula_3_CNF F) { 
    vector<vector<int>> formula = F.vratiFormulu();
    int k = formula.size() * 3;
    vector<vector<int>> result(k, vector<int>(k, 1));
    for (int i = 0; i < k; i += 3) {
      result[i][i] = 0;
      result[i][i + 1] = 0;
      result[i][i + 2] = 0;
      result[i + 1][i] = 0;
      result[i + 1][i + 1] = 0;
      result[i + 1][i + 2] = 0;
      result[i + 2][i] = 0;
      result[i + 2][i + 1] = 0;
      result[i + 2][i + 2] = 0;
    }
    for (int i = 0; i < formula.size(); i++) {
      for (int j = 0; j < formula[i].size(); j++) {
        int literal = formula[i][j];
        for (int k = i + 1; k < formula.size(); k++) {
          for (int l = 0; l < formula[k].size(); l++) {
            int literalNext = formula[k][l];
            if (literal != literalNext && abs(literal) == abs(literalNext)) {
              result[i * 3 + j][k * 3 + l] = 0;
              result[k * 3 + l][i * 3 + j] = 0;
            }
          }
        }
      }
    }

    return result;
  }
};



int main() {

  Formula_3_CNF formula;
  KlikaINDSETGraf<int> clique;
  int izbor = 0, k = 0;
  vector<int> certifikat;
  vector<vector<int>> rezultat;

  do {
    cout << "Izaberite opciju:" << endl;
    cout << "---------------------------------------------" << endl;
    cout << " 1)  Unos formule" << endl;
    cout << " 2)  Unos grafa" << endl;
    cout << " 3)  Je li formula ispunjiva" << endl;
    cout << " 4)  Postoji li k-nezavisnih cvorova" << endl;
    cout << " 5)  Postoji li k-klika" << endl;
    cout << " 6)  Verifikacija formule" << endl;
    cout << " 7)  Verifikacija skupa nezavisnih cvorova" << endl;
    cout << " 8)  Verifikacija k-klike" << endl;
    cout << " 9)  Redukcija 3-SAT-TO-INDSET" << endl;
    cout << " 10) Redukcija 3-SAT-TO-CLIQUE" << endl;
    cout << " 11) Izlaz" << endl;
    cout << "---------------------------------------------" << endl;
    cin >> izbor;

    switch (izbor) {
    case 1: {
      formula.Unos3CNF();
      break;
    }
    case 2: {
      clique.unosGrafa();
      break;
    }
    case 3: {
      if (formula.RJESENJE_3_CNF())
        cout << "Formula je ispunjiva.";
      else
        cout << "Formula nije ispunjiva.";
      break;
    }
    case 4: {
      cout << "Provjeriti jel postoji nezavisnih cvorova stepena: " << endl;
      cin >> k;
      string postojiIndset = clique.RJESENJE_K_INDSET(k) ? "TRUE" : "FALSE";
      cout << "Graf -> Postojanje " << k
           << " nezavisnih covora: " << postojiIndset << endl;
      break;
    }
    case 5: {
      cout << "\nUnesite velicinu klike: ";
      cin >> k;
      string postojiKlika = clique.RJESENJE_K_CLIQUE(k) ? "TRUE" : "FALSE";
      cout << "Graf -> Postojanje k-klike stepena " << k << ": " << postojiKlika
           << endl;
      break;
    }
    case 6: {
      cout << "Unesite vrijednosti literala, za zavrsetak unijeti -1:  "
           << endl;
      do {
        cin >> k;
        if (k != -1)
          certifikat.push_back(k);
      } while (k != -1);

      string verifikacija = formula.VERIFIKACIJA_3_CNF(certifikat) ? "TRUE" : "FALSE";
      cout << "Verifikacija unesene vrijednosti: " << verifikacija << endl;
      break;
    }
    case 7: {
      certifikat.clear();
      cout << "Unesite nezavisne cvorove numerisano od 0, za zavrsetak unijeti "
              "-1:  "
           << endl;
      do {
        cin >> k;
        if (k != -1)
          certifikat.push_back(k);
      } while (k != -1);

      string verIndset =
          clique.VERIFIKACIJA_INDSET(certifikat) ? "TRUE" : "FALSE";
      cout << "Verifikacija skupa nezavisnih cvorova: " << verIndset << endl;
      break;
    }
    case 8: {
      certifikat.clear();
      cout << "Unesite k kliku numerisano od 0, za zavrsetak unijeti -1:  "
           << endl;

      do {
        cin >> k;
        if (k != -1)
          certifikat.push_back(k);
      } while (k != -1);

      string verKlike =
          clique.VERIFIKACIJA_K_CLIQUE(certifikat) ? "TRUE" : "FALSE";
      cout << "Verifikacija k klike: " << verKlike << endl;
      break;
    }
    case 9: {
      rezultat = clique.satToIndset(formula);
      if (rezultat.empty())
        break;

      cout << "Trazena matrica nakon 3SAT-TO-INDSET redukcije: " << std::endl;
      for (auto &i : rezultat) {
        for (int j : i)
          cout << j << " ";
        cout << endl;
      }
      cout << endl;
      break;
    }
    case 10: {
      rezultat = clique.satToClique(formula);
      if (rezultat.empty())
        break;

      cout << "Trazena matrica nakon 3SAT-TO-CLIQUE redukcije: " << std::endl;
      for (auto &i : rezultat) {
        for (int j : i)
          cout << j << " ";
        cout << endl;
      }
      cout << endl;
      break;
    }
    case 11: {
      cout << "Izlaz iz programa";
      return 0;
    }
    }
  } while (izbor != 11);
  return 0;
}