#include "db/connect.h"
#include "inc/query.h"
#include "inc/functions.h"

MYSQL* conn=connect(); //connect to db

int main()
{
    menu:

    system("cls");

    cout<<"Witaj, co chcesz zrobic?"<<endl;

    string choose;

    start:

    cout<<"1. Zaloguj sie"<<endl;
    cout<<"2. Zarejestruj sie"<<endl;
    cout<<"3. Kalkulator kapitalu"<<endl;
    cout<<"4. Kalkulator walut"<<endl;
    cout<<"5. Wplata / Wyplata srodkow z konta"<<endl;
    cin>>choose;


    if(choose=="1"){
        if(!login(conn))
            goto menu;
    }

if(choose=="2"){
                if(regist(conn))
            goto menu;
        }
if(choose=="3"){
            kalkulator_kapital();
        goto menu;
}

if(choose=="4"){
            kalkulator_walut();
            goto menu;
}
if(choose=="5"){
            bankomat(conn);
            goto menu;
        }

    else{
        cout<<"Nieprawidlowy wybor"<<endl;
        system("cls");
        goto start;
    }

    }


