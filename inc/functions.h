bool regist(MYSQL* conn){

    MYSQL_ROW row;
    MYSQL_RES* res;

    cout<<"Rozpoczynam proces rejestracji"<<endl;
    Sleep(2500);
    system( "cls" );

    string login;
    string password, password1;
    string account_number,account_number1;
    string pin;
    int unique_code;

    exist_login: // go to if login is busy

    cout<<"Prosze wpisac swoj login"<<endl;
    cin>>login;

    if(check_if_exist(conn, "users", "login", login )){

        cout<<"Ten login jest zajety. Prosze wpisac inny login"<<endl;
        Sleep(2000);
        system("cls");
        goto exist_login;

    }

    password:

    cout<<"Podaj haslo"<<endl;
    cin>>password;
    cout<<"Powtorz haslo"<<endl;
    cin>>password1;

    if(password!=password1){

        cout<<"Hasla sie nie zgadzaja"<<endl;
        cout<<"Podaj ponownie haslo"<<endl;
        goto password;

    }

    srand(time(NULL));
    string acd,b,c,g;

    int a;

    duplicate:

    while(a<16)
    {
        acd = rand ()%10+48;
        b.append(acd);
        a++;
    }
    account_number=b;
    a=0;

    while(a<14)
    {
        acd = rand ()%10+48;
        g.append(acd);
        a++;
    }
    account_number1=g;
    a=0;

    while(a<4)
    {
        acd = rand ()%10+48;
        c.append(acd);
        a++;
    }
    pin=c;

    if(check_if_exist(conn, "account", "number_account", account_number )){

        goto duplicate;

    }

    if(check_if_exist(conn, "account", "number_account", account_number1 )){

        goto duplicate;

    }


    cout<<"Twoje  dane to:"<<endl;
    cout<<"Login: "<<login<<endl;
    cout<<"Numer konta glownego: "<<account_number<<endl;
    cout<<"Numer konta oszczednosciowego: "<<account_number1<<endl;
    cout<<"Pin: "<<pin<<endl;
    string confirm;
    cout<<"Potwierdzasz zalozenie konta? T/N"<<endl;
    cin>>confirm;
    if(confirm=="T" || confirm=="t" ){

        int qstate = 0, quantity;

        stringstream ss;
        ss << "INSERT INTO `users`(`id`, `login`, `password`, `pin`) VALUES (NULL,'" <<login<< "','" <<password<< "','" <<pin<< "')";

        string query = ss.str();
        const char* q = query.c_str();
        qstate = mysql_query(conn, q);

        stringstream ss1;
        ss1 << "SELECT max(id) FROM users";

        string query1 = ss1.str();

        const char* q1 = query1.c_str();
        qstate = mysql_query(conn, q1);

        res = mysql_store_result(conn);
        row= mysql_fetch_row(res);

        stringstream ss2;
        ss2 << "INSERT INTO `account` (`id_owner`, `type`, `number_account`, `amount`, `block`) VALUES ( " <<row[0]<< " , 'primary' ,'" <<account_number<< "' , '0' ,0);";

        string query2 = ss2.str();
        const char* q2 = query2.c_str();
        mysql_query(conn, q2);

        stringstream ss3;
        ss3 << "INSERT INTO `account` (`id_owner`, `type`, `number_account`, `amount`, `block`) VALUES ( " <<row[0]<< " , 'savings' ,'" <<account_number1<< "' , '0' ,0); ";

        string query3 = ss3.str();
        const char* q3 = query3.c_str();
        mysql_query(conn, q3);


        if(qstate == 0){

            cout<<"Prawidlowo zarejestrowano konto"<<endl<<"Za chwile zostaniesz przekierowany do menu wyboru"<<endl;
            Sleep(4000);
            system("cls");

            return true;
        }


    }
    else{

        return false;
        system("PAUSE");

    }


}

bool change_pin(MYSQL* conn, string login){

    system("cls");
    cout<<"Zmiana pinu"<<endl;

    MYSQL_ROW row;
    MYSQL_RES* res;

    int qstate = 0;
    string old_pin,new_pin,new_pin1,password;

    cout<<"W celu weryfikacji musisz podac swoje aktualne haslo oraz pin"<<endl;
    bad_password:
    cout<<"Podaj haslo"<<endl;
    cin>>password;

    stringstream ss;
    ss << "SELECT * FROM users where login= '" << login << "' AND password='" << password << "'  ";

    string query = ss.str();

    const char* q = query.c_str();
    qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);
    row= mysql_fetch_row(res);

    if(row==0){

        cout<<"Nieprawidlowe haslo"<<endl;
        goto bad_password;

    }

    bad_pin:
    cout<<"Podaj aktualny pin"<<endl;
    cin>>old_pin;

    stringstream ss1;
    ss1 << "SELECT * FROM users where login= '" << login << "' AND pin='" << old_pin << "'  ";

    string query1 = ss1.str();

    const char* q1 = query1.c_str();
    qstate = mysql_query(conn, q1);

    res = mysql_store_result(conn);
    row= mysql_fetch_row(res);

    if(row==0){

        cout<<"Nieprawidlowe pin"<<endl;
        goto bad_pin;

    }
    system("cls");
    cout<<"Dane ktore zostaly wprowadzone sa prawidlowe"<<endl;
    bad_pin1:
    cout<<"Podaj nowy pin"<<endl;
    cin>>new_pin;
    cout<<"Powtorz nowy pin"<<endl;
    cin>>new_pin1;

    if(new_pin1!=new_pin){

        cout<<"Pin sie nie zgadza"<<endl;
        goto bad_pin1;
    }else{

        stringstream ss2;
        ss2 << "UPDATE users SET pin =  '" << new_pin << "' WHERE login = '" << login << "'";
        string query2 = ss2.str();
        const char* q2 = query2.c_str();
        qstate = mysql_query(conn, q2);

        cout<<"Pin zostal zmienieniony"<<endl;
        Sleep(1500);
        return true;

    }

    return false;
}

bool block_cart(MYSQL* conn,string id,string login){

    system("cls");
    cout<<"Blokowanie/Odblokowywanie karty"<<endl;



    int qstate = 0;
    string password,pin,account_number;

    cout<<"W celu weryfikacji musisz podac swoje aktualne haslo, pin oraz numer konta"<<endl;

    bad_data:

    cout<<"Podaj haslo"<<endl;
    cin>>password;
    cout<<"Podaj pin"<<endl;
    cin>>pin;
    cout<<"Podaj numer konta"<<endl;
    cin>>account_number;

    MYSQL_ROW row,row2;
    MYSQL_RES* res,res2;

    stringstream ss1;
    ss1 << "SELECT * FROM users where login= '" << login << "'  AND password= '" << password << "' AND pin= '" << pin << "' ";

    string query = ss1.str();

    const char* q = query.c_str();
    qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);
    row= mysql_fetch_row(res);

    stringstream ss2;
    ss2 << "SELECT * FROM account WHERE id_owner= '" <<id << "' AND number_account= '" <<account_number << "' ";


    string query2 = ss2.str();

    const char* q2 = query2.c_str();
    qstate = mysql_query(conn, q2);

    res = mysql_store_result(conn);
    row2= mysql_fetch_row(res);


    if(row==0 || row2==0){

        cout<<"nieprawidlowe dane"<<endl;
        goto bad_data;

    }else{


        if (strcmp(row2[5], "0") == 0) {


            stringstream ss1;
            ss1 << "UPDATE account SET block = 1 WHERE id_owner = '" << id << "' AND number_account= '" <<account_number << "' ";
            string query1 = ss1.str();
            const char* q1 = query1.c_str();
            qstate = mysql_query(conn, q1);

            cout<<"Zablokowano";
            Sleep(1500);

            return true;

        }
        else{
            cout<<"Odblokowano";

            stringstream ss3;
            ss3 << "UPDATE account SET block = 0 WHERE id_owner = '" << id << "' AND number_account= '" <<account_number << "' ";
            string query3 = ss3.str();
            const char* q3 = query3.c_str();
            qstate = mysql_query(conn, q3);
            Sleep(1500);
            return true;

        }

    }

}


bool user_panel(MYSQL* conn, string login ){

    MYSQL_ROW row,row1;
    MYSQL_RES* res;

    start:

    system("cls");

    stringstream ss;

    ss << "SELECT * FROM users where login= '" << login << "' ";

    string query = ss.str();
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    stringstream ss2;

    ss2 << "SELECT * FROM account where id_owner= '" << row[0] << "' ";

    string query2 = ss2.str();
    const char* q2 = query2.c_str();
    int qstate1 = mysql_query(conn, q2);


    cout<<"Witaj, "<<row[1]<<endl;
    cout<<"Ostatnio zalogowales sie "<<row[4]<<endl;

    string type,status;

    res = mysql_store_result(conn);

    while(row1 = mysql_fetch_row(res)){

        if(strcmp(row1[2], "primary")==0)
             type="glownego";
        else
             type="oszczednosciowego";

        if(strcmp(row1[5], "0")==0)
             status="odblokowane";
        else
             status="zablokowane";

        cout<<"Numer konta "<<type<<": "<<row1[3]<<" Saldo: "<<row1[4]<< " zl  Stan konta: "<<status<<endl;

    }


    time_t czas;
    time( & czas );

    stringstream ss1;
    ss1 << "UPDATE users SET last_login =  '" << ctime( & czas ) << "' WHERE login = '" << login << "'";
    string query1 = ss1.str();
    const char* q1 = query1.c_str();
    qstate = mysql_query(conn, q1);

    int action;
    cout<<endl;
    cout<<"Co chcesz zrobic?"<<endl;
    cout<<"1. Zmien pin"<<endl;
    cout<<"2. Zablokuj/Odblokuj karte"<<endl;
    cout<<"3. Wyloguj sie"<<endl;
    cin>>action;

    switch(action){

    case 1:
        if(change_pin(conn,login))
            goto start;
        break;
    case 2:
        if(block_cart(conn,row[0],login))
            goto start;
        break;
    case 3:
        return false;
        break;

     default:
        cout<<"Nieprawidlowy wybor"<<endl;
        goto start;


    }


}

bool login(MYSQL* conn){

    string login,password;

    login:
    system("cls");

    cout<<"Podaj login"<<endl;
    cin>>login;
    cout<<"Podaj haslo"<<endl;
    cin>>password;

    MYSQL_ROW row;
    MYSQL_RES* res;

    int qstate = 0;
    stringstream ss1;
    ss1 << "SELECT * FROM users where login= '" << login << "'  AND password= '" << password << "'";

    string query = ss1.str();


    const char* q = query.c_str();
    qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);
    row= mysql_fetch_row(res);

    if(row!=0){

        if(!user_panel(conn,login))
            return false;


    }
    else{

        cout<<"Dane nieprawidlowe"<<endl;
        Sleep(1500);
        goto login;

    }

}
bool kalkulator_kapital(){
    system("cls");
    float KP,KO,CT,SP,WYNKK;
    cout<<"Kapital poczatkowy: ";
    cin>>KP;
    cout<<"Liczba kapitalizacji odsetek w roku: ";
    cin>>KO;
    cout<<"Czas trwania inwestycji(w latach): ";
    cin>>CT;
    cout<<"Stopa procentowa(w postaci liczby dziesietnej z znakiem . ): ";
    cin>>SP;
    WYNKK=KP*pow(1+(SP/KO), KO*CT);
    WYNKK=WYNKK*100;
    WYNKK=round(WYNKK);
    WYNKK=WYNKK/100;
    cout<<"Koncowy kapital: "<<WYNKK<<endl;
    getch();
    system("cls");
}
bool kalkulator_walut(){
    float WP,wWP;
    int kal_wal_wyb;
    cout<<"Wprowadz kwote do przeliczenia na inna walute: ";
    cin>>WP;
    kalkulator_wyb:
    cout<<"Dostepne waluty: "<<endl;
    cout<<"1. Dolar Amerykanski (USD)"<<endl;
    cout<<"2. Euro (EUR)"<<endl;
    cout<<"3. Jen Japonski (JPY)"<<endl;
    cout<<"4. Funt Szterling (GBP)"<<endl;
    cout<<"5. Frank Szwajcarski (CHF)"<<endl<<endl;
    cout<<"Na jaka walute chcesz przeliczyc kwote? ";
    cin>>kal_wal_wyb;
    switch(kal_wal_wyb)
    case 1:{
        {
        wWP=WP/3.8;
        wWP=wWP*100;
        wWP=round(wWP);
        wWP=wWP/100;
        cout<<"Podana kwota w przeliczeniu na USD to: "<<wWP<<endl;
        getch();
        break;
        }
    case 2:
        {
        wWP=WP/4.24;
        wWP=wWP*100;
        wWP=round(wWP);
        wWP=wWP/100;
        cout<<"Podana kwota w przeliczeniu na EUR to: "<<wWP<<endl;
        getch();
        break;
        }
    case 3:
        {
        wWP=WP/0.035;
        wWP=wWP*100;
        wWP=round(wWP);
        wWP=wWP/100;
        cout<<"Podana kwota w przeliczeniu na JPY to: "<<wWP<<endl;
        getch();
        break;
        }
    case 4:
        {
        wWP=WP/4.98;
        wWP=wWP*100;
        wWP=round(wWP);
        wWP=wWP/100;
        cout<<"Podana kwota w przeliczeniu na GBP to: "<<wWP<<endl;
        getch();
        break;
        }
    case 5:
        {
        wWP=WP/3.95;
        wWP=wWP*100;
        wWP=round(wWP);
        wWP=wWP/100;
        cout<<"Podana kwota w przeliczeniu na CHF to: "<<wWP<<endl;
        getch();
        break;
        }
    default:
        {
        cout<<"Podaj poprawny wybor waluty."<<endl;
        getch();
        goto kalkulator_wyb;
        }
}
}




