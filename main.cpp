#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include<conio.h>
using namespace std;
int main(){
	int a=0;
	string login,haslo,wyb,nr_konta,b,acd,c,pin;
	srand(time(NULL));
	fstream plik;
	plik.open("dane.txt", ios::in | ios::out); 
	if(plik.good()==true)
	{
		plik.close();
		cout<<"Przyznano dostep do bazy."<<endl<<endl;
		short wyb;
		menu:
		cout<<"Witamy w systemie bankowym"<<endl;
		cout<<"Wybierz co chcesz zrobic"<<endl;
		cout<<"1. Rejestracja"<<endl;
		
		cin>>wyb;
		system("cls");
		switch(wyb){
			case 1: //sprwadü przed wpisaniem do bazy czy podany login nie zosta≥ juø zarejestrowany
				{
				fstream plik;
				plik.open("dane.txt",ios::in | ios::app);
				cout<<"Podaj login: ";cin>>login;
				cout<<"Podaj haslo: ";cin>>haslo;
				while(a<16)
				{
					acd = rand ()%10+48;
					b.append(acd);
					a++;
				}
				nr_konta=b;
					a=0;
					while(a<4)
				{
					acd = rand ()%10+48;
					c.append(acd);
					a++;
				}
				pin=c;
				cout<<"Oto twoj nr. konta: "<<nr_konta<<endl;;
				cout<<"Oto twoj nr. pin: "<<pin<<endl;
				plik<<login+" "<<haslo+" "<<nr_konta+" "<<pin<<endl;
				plik.close();
				cout<<"Rejestracja powiodla sie.";
				getch();
				system("cls");
				goto menu;
				} 
		}
	}
}
