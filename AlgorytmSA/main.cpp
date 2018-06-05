#include<iostream>
#include<Windows.h>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <random>
#include "Metal.h"
#include "Zadanie.h"
#include "Priorytet.h"
#include "Neh.h"

using namespace std;
static random_device rd;  // Random device engine, usually based on /dev/random on UNIX-like systems
static mt19937_64 rng(rd()); // Initialize Mersennes' twister using rd to generate the seed

// Generate random doubles in the interval [initial, last)
double random_real(double initial, double last) {
	uniform_real_distribution<double> distribution(initial, last);
	return distribution(rng);  // Use rng as a generator
}
int LiczCmax(Zadanie *DaneNEH, int LiczbaZadan, int LiczbaMaszyn) {
	int Cmax = 0;
	int* TymczasoweCzasyNaMaszynach;
	TymczasoweCzasyNaMaszynach = new int[LiczbaMaszyn];
	for (int i = 1; i <= LiczbaMaszyn; i++) {
		TymczasoweCzasyNaMaszynach[i] = 0;
	}
	for (int kk = 0; kk < LiczbaZadan; kk++) {
		//cout << "Tymczasowe Czasy na maszynie: " << l << "   " << TymczasoweCzasyNaMaszynach[l] << endl;
		TymczasoweCzasyNaMaszynach[1] = TymczasoweCzasyNaMaszynach[1] + DaneNEH[kk].CzasTrwania[1];
		for (int i = 2; i <= LiczbaMaszyn; i++) {
			TymczasoweCzasyNaMaszynach[i] = max(TymczasoweCzasyNaMaszynach[i - 1] + DaneNEH[kk].CzasTrwania[i], TymczasoweCzasyNaMaszynach[i] + DaneNEH[kk].CzasTrwania[i]);
			Cmax = TymczasoweCzasyNaMaszynach[i];
		}
	}
	return Cmax;
}
void swapDanePriorytety(Priorytet* PrtiorytetyW, int i, int j)
{
	swap(PrtiorytetyW[i].NumerZadania, PrtiorytetyW[j].NumerZadania);
	swap(PrtiorytetyW[i].SumaPracyNaWszystkichMaszynach, PrtiorytetyW[j].SumaPracyNaWszystkichMaszynach);
}
void swapDaneNEH(Zadanie *DaneNEH, int i, int j)
{
	swap(DaneNEH[i].CzasTrwania, DaneNEH[j].CzasTrwania);
	swap(DaneNEH[i].liczbaMaszyn, DaneNEH[j].liczbaMaszyn);
	swap(DaneNEH[i].NumerZadania, DaneNEH[j].NumerZadania);
	swap(DaneNEH[i].SumaCzasuTrwania, DaneNEH[j].SumaCzasuTrwania);
}
void SortowanieMalejaceSzybkie2(Priorytet* mojeWspolczynnikiW, int lewy, int prawy) {
	int i = lewy;
	int j = prawy;
	int x = mojeWspolczynnikiW[((lewy + prawy) / 2)].SumaPracyNaWszystkichMaszynach;
	do {
		while (mojeWspolczynnikiW[i].SumaPracyNaWszystkichMaszynach > x)
			i++;
		while (mojeWspolczynnikiW[j].SumaPracyNaWszystkichMaszynach < x)
			j--;
		if (i <= j) {
			swapDanePriorytety(mojeWspolczynnikiW, i, j);
			i++;
			j--;
		}
	} while (i <= j);
	if (lewy < j) { SortowanieMalejaceSzybkie2(mojeWspolczynnikiW, lewy, j); }
	if (prawy > i) { SortowanieMalejaceSzybkie2(mojeWspolczynnikiW, i, prawy); }
}
void SortowanieMalejaceSzybkiePoSumieCzasuTrwania(Zadanie* DaneNEH, int lewy, int prawy) {
	int i = lewy;
	int j = prawy;
	int x = DaneNEH[((lewy + prawy) / 2)].SumaCzasuTrwania;
	do {
		while (DaneNEH[i].SumaCzasuTrwania > x)
			i++;
		while (DaneNEH[j].SumaCzasuTrwania < x)
			j--;
		if (i <= j) {
			swapDaneNEH(DaneNEH, i, j);
			i++;
			j--;
		}
	} while (i <= j);
	if (lewy < j) { SortowanieMalejaceSzybkiePoSumieCzasuTrwania(DaneNEH, lewy, j); }
	if (prawy > i) { SortowanieMalejaceSzybkiePoSumieCzasuTrwania(DaneNEH, i, prawy); }
}
void AlgorytmNEH(Zadanie *DaneNEH, int LiczbaZadan, int LiczbaMaszyn) {

	Priorytet *mojeWspolczynnikiW;
	mojeWspolczynnikiW = new Priorytet[LiczbaZadan];
	for (int i = 0; i < LiczbaZadan; i++) {
		mojeWspolczynnikiW[i].NumerZadania = 0;
		mojeWspolczynnikiW[i].SumaPracyNaWszystkichMaszynach = 0;
	}
	for (int i = 0; i < LiczbaZadan; i++) {
		for (int j = 1; j <= LiczbaMaszyn; j++) {
			DaneNEH[i].SumaCzasuTrwania = DaneNEH[i].SumaCzasuTrwania + DaneNEH[i].CzasTrwania[j];
			mojeWspolczynnikiW[i].NumerZadania = i;
			mojeWspolczynnikiW[i].SumaPracyNaWszystkichMaszynach = mojeWspolczynnikiW[i].SumaPracyNaWszystkichMaszynach + DaneNEH[i].CzasTrwania[j];
		}
		//cout << "Zadanie: " << i << "/" << mojeWspolczynnikiW[i].NumerZadania << " Priorytet W: " << mojeWspolczynnikiW[i].SumaPracyNaWszystkichMaszynach << endl;
	}
	cout << endl;
	//Sortowanie Priorytetów nierosnaco
	SortowanieMalejaceSzybkie2(mojeWspolczynnikiW, 0, LiczbaZadan - 1);

	Kolejnosci_Cmaxy *mojNeh;
	mojNeh = new Kolejnosci_Cmaxy[LiczbaZadan];
	mojNeh->LiczbaZadan = LiczbaZadan;
	for (int k = 0; k < LiczbaZadan; k++) {
		for (int i = 0; i < LiczbaZadan; i++) {
			mojNeh[k].Cmax = 900000000;
			mojNeh[k].Kolejnosc[i] = 0;
		}
	}
	int *NajlepszaKombinacja = new int[LiczbaZadan];
	for (int i = 0; i < LiczbaZadan; i++) {
		NajlepszaKombinacja[i] = 0;
	}
	int najmniejszyElement = 999990;

	Zadanie *DaneNEH2; DaneNEH2 = new Zadanie[LiczbaZadan];
	DaneNEH2->liczbaMaszyn = LiczbaMaszyn;
	for (int i = 0; i < LiczbaZadan; i++) {
		for (int j = 1; j <= LiczbaMaszyn; j++) {
			DaneNEH2[i].CzasTrwania[j] = DaneNEH[i].CzasTrwania[j];
			DaneNEH2[i].NumerZadania = DaneNEH[i].NumerZadania;
			//cout << "Numer Zadania: " << DaneNEH2[i].NumerZadania << " Numer Maszyny: " << j << " Czas Trwania: " << DaneNEH2[i].CzasTrwania[j] << std::endl;
		}//cout << endl;
	}

	for (int k = 0; k < LiczbaZadan; k++) {
		for (int i = 0; i < LiczbaZadan; i++) {

			swapDaneNEH(DaneNEH2, i, mojeWspolczynnikiW[k].NumerZadania);
			mojNeh[k].Cmax = LiczCmax(DaneNEH2, LiczbaZadan, LiczbaMaszyn);
			//cout << "!!!!!!!!!!!!!!!!!!!!!!C__max: " << mojNeh[k].Cmax << endl;
			//cout << "Dla kombinacji: " << endl;
			for (int m = 0; m < LiczbaZadan; m++) {
				mojNeh[k].Kolejnosc[m] = DaneNEH2[m].NumerZadania;
				//cout << mojNeh[k].Kolejnosc[m] << endl;
			}
			if (najmniejszyElement > mojNeh[k].Cmax) {
				cout << "C__max: " << mojNeh[k].Cmax << endl;
				//cout << "Dla kombinacji: " << endl;
				for (int m = 0; m < LiczbaZadan; m++) {
					NajlepszaKombinacja[m] = mojNeh[k].Kolejnosc[m];
					//cout << mojNeh[k].Kolejnosc[m] << endl;
				}
				najmniejszyElement = mojNeh[k].Cmax;
			}
			swapDaneNEH(DaneNEH2, mojeWspolczynnikiW[k].NumerZadania, i);
		}
		for (int i = 0; i < LiczbaZadan; i++) {
			//cout << "Numer Zadania: " << endl;
			for (int j = 1; j <= LiczbaMaszyn; j++) {
				DaneNEH2[i].CzasTrwania[j] = DaneNEH[NajlepszaKombinacja[i]].CzasTrwania[j];
				DaneNEH2[i].NumerZadania = DaneNEH[NajlepszaKombinacja[i]].NumerZadania;
				//cout<< DaneNEH2[i].NumerZadania << " Numer Maszyny: " << j << " Czas Trwania: " << DaneNEH2[i].CzasTrwania[j] << std::endl;
			}
		}

	}
	//for (int i = 0; i < LiczbaZadan; i++) {
	//cout << "Numer Zadania: " << endl;
	//	for (int j = 1; j <= LiczbaMaszyn; j++) {
	//cout<< DaneNEH2[i].NumerZadania << " Numer Maszyny: " << j << " Czas Trwania: " << DaneNEH2[i].CzasTrwania[j] << std::endl;
	//	}
	//}
	delete[] mojNeh; //uwolnienie pamieci
	mojNeh = NULL;
	delete[] mojeWspolczynnikiW; //uwolnienie pamieci
	mojeWspolczynnikiW = NULL;
	delete[] NajlepszaKombinacja; //uwolnienie pamieci
	NajlepszaKombinacja = NULL;
	//for (int i(0); i < LiczbaZadan; ++i)
	//	delete[] WspolczynnikPriorytetowW[i]; //uwolnienie pamieci
	//delete[] WspolczynnikPriorytetowW; //uwolnienie pamieci
	//WspolczynnikPriorytetowW = NULL;
}
void AlgorytmSA(Zadanie *DaneSA,int LiczbaZadan,int LiczbaMaszyn) {

	double TemperaturaPoczatkowa = 1000;
	double WspolczynnikWychladzania = 0.80;

	float rnd = 0;
	float P = 0;

	Kolejnosci_Cmaxy *mojSA;
	mojSA = new Kolejnosci_Cmaxy[LiczbaZadan];
	mojSA->LiczbaZadan = LiczbaZadan;
	for (int k = 0; k < LiczbaZadan; k++) {
		for (int i = 0; i < LiczbaZadan; i++) {
			mojSA[k].Cmax = 900000000;
			mojSA[k].Kolejnosc[i] = 0;
		}

	}

	mojSA[0].Cmax = LiczCmax(DaneSA, LiczbaZadan, LiczbaMaszyn); 
	for (int m = 0; m < LiczbaZadan; m++) {
		mojSA[0].Kolejnosc[m] = DaneSA[m].NumerZadania;
		//cout << mojSA[0].Kolejnosc[m] << endl;
	}
	int LosowyRuch=0,LosowyRuch2=0;
	while (TemperaturaPoczatkowa > 10) {
		for (int k = 1; k < LiczbaZadan; k++) {

			LosowyRuch = random_real(0, LiczbaZadan);
			LosowyRuch2 = random_real(0, LiczbaZadan);

			cout << "LosowyRuch1: " << LosowyRuch << endl;
			cout << "LosowyRuch2: " << LosowyRuch2 << endl;

			//SWAP ALBO INSERT -> LOSOWY RUCH
			swapDaneNEH(DaneSA, LosowyRuch, LosowyRuch2);
			// PRZELICZ C_MAX
			mojSA[k].Cmax = LiczCmax(DaneSA, LiczbaZadan, LiczbaMaszyn);
			for (int m = 0; m < LiczbaZadan; m++) {
				mojSA[k].Kolejnosc[m] = DaneSA[m].NumerZadania;
			}

			if (mojSA[k].Cmax < mojSA[k - 1].Cmax) {
				TemperaturaPoczatkowa = TemperaturaPoczatkowa * WspolczynnikWychladzania;
				cout << "TemperaturaObecna: " << TemperaturaPoczatkowa << endl;
			}else {
				rnd = random_real(0, 1);
				cout <<"rnd: "<< rnd << endl;
				if ((mojSA[k].Cmax - mojSA[k - 1].Cmax) == 0) {
					P = 0;
				}else {
					if (TemperaturaPoczatkowa == 0) {
						break;
					}
					P = exp((mojSA[k].Cmax - mojSA[k - 1].Cmax) / TemperaturaPoczatkowa);
				}
				cout <<"P: "<< P << endl;
				if (rnd < P) {
					TemperaturaPoczatkowa = TemperaturaPoczatkowa * WspolczynnikWychladzania;
					cout << "TemperaturaObecna: " << TemperaturaPoczatkowa << endl;
				}else {
					//cofnij ruch
					swapDaneNEH(DaneSA, LosowyRuch, LosowyRuch2);
				}
			}
			cout << "!!!!!!!!!!!!!!!!!!!!!!C__max: " << mojSA[k].Cmax << endl;
			cout << "Dla kombinacji: " << endl;
			for (int m = 0; m < LiczbaZadan; m++) {
				mojSA[k].Kolejnosc[m] = DaneSA[m].NumerZadania;
				cout << mojSA[k].Kolejnosc[m] << endl;
			}
		}
	}


	delete[] mojSA; //uwolnienie pamieci
	mojSA = NULL;

}
int main() {

	fstream Plik_z_Danymi;
	int liczbaZadan;
	int liczbaMaszyn;
	int TempCzasWykonywania_p = 0, TempKtoraMaszyna = 0;
	int Cmax = 0;

	Zadanie *DaneNEH;
	Plik_z_Danymi.open("ta001.txt", ios::in | ios::out);
	if (Plik_z_Danymi.good() == true) {

		cout << "Uzyskano dostep do pliku!" << endl;
		Plik_z_Danymi >> liczbaZadan >> liczbaMaszyn;
		DaneNEH = new Zadanie[liczbaZadan];
		DaneNEH->liczbaMaszyn = liczbaMaszyn;

		for (int i = 0; i < liczbaZadan; i++) {
			for (int j = 1; j <= liczbaMaszyn; j++) {
				Plik_z_Danymi >> TempKtoraMaszyna >> TempCzasWykonywania_p;
				DaneNEH[i].CzasTrwania[TempKtoraMaszyna] = TempCzasWykonywania_p;
				DaneNEH[i].NumerZadania = i;
				DaneNEH[i].SumaCzasuTrwania = 0;
				//cout << "Numer Zadania: " << DaneNEH[i].NumerZadania<< " Numer Maszyny: " << j << " Czas Trwania: " << DaneNEH[i].CzasTrwania[TempKtoraMaszyna] << std::endl;
			}cout << endl;
		}
		Cmax = LiczCmax(DaneNEH, liczbaZadan, liczbaMaszyn);
		cout << "Nasz C_MAX przed NEH: " << Cmax << endl;

		AlgorytmSA(DaneNEH, liczbaZadan, liczbaMaszyn);

		//AlgorytmNEH(DaneNEH, liczbaZadan, liczbaMaszyn);

		delete[] DaneNEH; //uwolnienie pamieci
		DaneNEH = NULL;



	}
	else {
		cout << "Brak dostepu do pliku" << endl;
	}
	if (Plik_z_Danymi.good() == true) {
		Plik_z_Danymi.close();
	}
	system("pause");
	return 0;

}



















