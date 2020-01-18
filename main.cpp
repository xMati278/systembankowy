#include "db/connect.h"
#include "inc/query.h"
#include "inc/functions.h"


MYSQL* conn=connect(); //connect to db


int main()
{
    menu:

    cout<<"Witaj, co chcesz zrobic?"<<endl;

    int choose;

    start:

    cout<<"1. Zaloguj sie"<<endl;
    cout<<"2. Zarejestruj sie"<<endl;
    cin>>choose;

    switch(choose){

    case 1:
        login(conn);
        break;

    case 2:
        if(regist(conn))
            goto menu;
        break;

    default:
        cout<<"Nieprawidlowy wybor"<<endl;
        goto start;



    }






}
