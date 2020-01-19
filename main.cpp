#include "db/connect.h"
#include "inc/query.h"
#include "inc/functions.h"


MYSQL* conn=connect(); //connect to db


int main()
{
    menu:

    system("cls");

    cout<<"Witaj, co chcesz zrobic?"<<endl;

    int choose;

    start:

    cout<<"1. Zaloguj sie"<<endl;
    cout<<"2. Zarejestruj sie"<<endl;
    cout<<"3. Kalkulator kapitalu"<<endl;
    cout<<"4. Kalkulator walut"<<endl;
    cin>>choose;

    switch(choose){

    case 1:
        if(!login(conn))
            goto menu;
        break;

    case 2:
        if(regist(conn))
            goto menu;
        break;
    case 3:
        kalkulator_kapital();
        goto menu;
        break;

    case 4:
        kalkulator_walut();
            goto menu;
        break;

    default:
        cout<<"Nieprawidlowy wybor"<<endl;
        goto start;



    }






}
