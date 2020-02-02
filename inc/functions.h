bool regist(MYSQL* conn){

    MYSQL_ROW row;
    MYSQL_RES* res;

    cout<<"Rozpoczynam proces rejestracji"<<endl;
    getch();
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
        getch();
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
        getch();
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

bool fast_transfer(MYSQL* conn,string id_own,string login){

    system("cls");
    cout<<"Szybki przelew"<<endl;

    int qstate = 0;
    MYSQL_ROW row;
    MYSQL_RES* res;

    stringstream ss1;
    ss1 << "SELECT block,amount FROM account where id_owner= '" << id_own << "'  AND type= 'primary' ";

    string query = ss1.str();

    const char* q = query.c_str();
    qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);
    row= mysql_fetch_row(res);

     if(strcmp(row[0], "1")==0){

        cout<<"Konto jest zablokowane"<<endl;
        Sleep(2000);
        return true;
    }

    string id_cin;
    int value;

    id:

    cout<<"Podaj unikalny numer osoby do ktorej chcesz przelac pieniadze"<<endl;
    cin>>id_cin;

    if(id_cin!=id_own){

        if(check_if_exist(conn, "users", "id", id_cin )){

            val:

            cout<<"Ile PLN chcesz przeslac?"<<endl;
            cin>>value;

            int val = atoi(row[1]);

            if(val<value){

                cout<<"Brak srodkow"<<endl;
                goto val;

            }

            stringstream ss2,ss3,ss4;
            ss2 << "UPDATE account SET amount = amount- " << value << " WHERE id_owner = " << id_own << " AND type='primary';";
            ss3 << "UPDATE account SET amount = amount+ " << value << " WHERE id_owner = " << id_cin << " AND type='primary';";
            ss4 << "INSERT INTO `transactions`(`id_sending`, `id_receiving`, `value`) VALUES (" << id_own << "," << id_cin << "," << value << ")";
            string query2 = ss2.str();
            const char* q2 = query2.c_str();

            string query3 = ss3.str();
            const char* q3 = query3.c_str();

            string query4 = ss4.str();
            const char* q4 = query4.c_str();

            mysql_query(conn, q2 );
            mysql_query(conn, q3 );
            mysql_query(conn, q4 );

            cout<<"Pomyslnie przelano pieniadze"<<endl;
            Sleep(2000);
            return true;

        }
        else{

            cout<<"Podales nieprawidlowe id"<<endl;
            goto id;
            Sleep(1000);

        }

    }
    else{

        cout<<"Podales swoje id!"<<endl;
        Sleep(2000);
    }


}

bool traditional_transfer(MYSQL* conn,string id_own,string login,string primary_account){

    system("cls");
    cout<<"Tradycyjny przelew"<<endl;

    string number_account;
    int value;

    int qstate = 0;
    MYSQL_ROW row;
    MYSQL_RES* res;

    stringstream ss1;
    ss1 << "SELECT block,amount FROM account where number_account= '" << primary_account << "' ";

    string query = ss1.str();

    const char* q = query.c_str();
    qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);
    row= mysql_fetch_row(res);

     if(strcmp(row[0], "1")==0){

        cout<<"Konto jest zablokowane"<<endl;
        Sleep(2000);
        return true;
    }

    start:

    cout<<"Podaj numer konta na ktory chcesz przelac pieniadze"<<endl;
    cin>>number_account;

    if(number_account!=primary_account){

        if(check_if_exist(conn, "account", "number_account", number_account )){

            val:

            cout<<"Ile PLN chcesz przeslac?"<<endl;
            cin>>value;

            int val = atoi(row[1]);

            if(val<value){

                cout<<"Brak srodkow"<<endl;
                goto val;

            }

            stringstream ss2,ss3,ss4;
            ss2 << "UPDATE account SET amount = amount- " << value << " WHERE number_account = " << primary_account << ";";
            ss3 << "UPDATE account SET amount = amount+ " << value << " WHERE number_account = " << number_account << ";";
            ss4 << "INSERT INTO `transactions`(`account_sending`, `account_receiving`, `value`) VALUES (" << primary_account << "," << number_account << "," << value << ")";
            string query2 = ss2.str();
            const char* q2 = query2.c_str();

            string query3 = ss3.str();
            const char* q3 = query3.c_str();

            string query4 = ss4.str();
            const char* q4 = query4.c_str();

            mysql_query(conn, q2 );
            mysql_query(conn, q3 );
            mysql_query(conn, q4 );

            cout<<"Pomyslnie przelano pieniadze"<<endl;
            Sleep(2000);
            return true;

        } else{

            cout<<"Numer konta nieprawidlowy"<<endl;
            Sleep(1000);
            goto start;
        }
    }else
    {
        cout<<"Wybrales swoje glowne konto numer konta"<<endl;
        Sleep(1000);
        goto start;

    }
}

bool account_history(MYSQL* conn,string id_own,string primary_account,string savings_account){

    system("cls");
    cout<<"Historia konta"<<endl<<"Twoje ID: "<<id_own<<endl;;

    MYSQL_ROW row;
    MYSQL_RES* res;

    stringstream ss1;

    ss1 << "SELECT * FROM `transactions` WHERE `id_sending`= " << id_own << " OR `id_receiving`=" << id_own << "  OR `account_sending`= " << primary_account << " OR `account_receiving`= " << primary_account << "  OR `account_sending`= " << savings_account << " OR `account_receiving`= " << savings_account << " ";

    string query1 = ss1.str();
    const char* q1 = query1.c_str();
    mysql_query(conn, q1);

    res = mysql_store_result(conn);

    while(row = mysql_fetch_row(res)){

        cout<<"Id wysylajacego: "<<row[1]<<" | "<<"Id odbierajacego: "<<row[2]<<" | "<<"Numer konta wysylajacego: "<<row[3]<<" | "<<"Numer konta odbierajacego: "<<row[4]<<" | "<<"PLN: "<<row[5]<<" | "<<"Data: "<<row[6]<<endl;

    }

    getch();
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
    string primary_account,savings_account;

    res = mysql_store_result(conn);

    while(row1 = mysql_fetch_row(res)){

        if(strcmp(row1[2], "primary")==0){
             type="glownego";
             primary_account=row1[3];
        }
        else{
             type="oszczednosciowego";
             savings_account=row1[3];


        }

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
    cout<<"3. Szybki przelew"<<endl;
    cout<<"4. Tradycyjny przelew"<<endl;
    cout<<"5. Historia konta"<<endl;
    cout<<"6. Wyloguj sie"<<endl;
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
        if(fast_transfer(conn,row[0],login))
            goto start;
        break;
    case 4:
        if(traditional_transfer(conn,row[0],login,primary_account))
            goto start;
        break;
    case 5:
        if(account_history(conn,row[0],primary_account,savings_account))
            goto start;
        break;
    case 6:
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
bool bankomat(MYSQL* conn){
    bankomat:
    int bankomat_wyb, bankomat_nr,bankomat_kasa,n,p;
    string bankomat_pin;
    cout<<"Co chcesz zrobic?"<<endl;
    cout<<"1. Wplata  gotowki"<<endl;
    cout<<"2. Wyplata gotowki"<<endl;
    cout<<"3. Wyjscie"<<endl;
    cin>>bankomat_wyb;
    if(bankomat_wyb==1 || bankomat_wyb==2)
    {
        cout<<"Podaj nr.konta: ";
        cin>>bankomat_nr;
        cout<<"Podaj pin: ";
        cin>>bankomat_pin;

        MYSQL_ROW row,row1;
        MYSQL_RES* res;

        int qstate = 0;
        stringstream ss1;
        ss1 << "SELECT id_owner,amount,type,block FROM account where number_account= '" << bankomat_nr << "'";

        string query = ss1.str();

        const char* q = query.c_str();
        qstate = mysql_query(conn, q);

        res = mysql_store_result(conn);
        row= mysql_fetch_row(res);

        char* owner_id=row[0];


        stringstream ss2;
        ss2 << "SELECT pin FROM users where id= '" << owner_id << "'";

        string query1 = ss2.str();

        const char* q1 = query1.c_str();
        qstate = mysql_query(conn, q1);

        res = mysql_store_result(conn);
        row1= mysql_fetch_row(res);

        char* pin=row1[0];

        const char* bank_pin = bankomat_pin.c_str();

        if(strcmp(pin, bank_pin)==0){

            if(strcmp(row[2], "savings")==0){

                cout<<"Nie mozesz wplacac/wyplacac pieniedzy z konta oszczednosciowego w bankomacie"<<endl;
                goto bankomat;

            }

            if(strcmp(row[3], "1")==0){

                cout<<"Konto jest zablokowane"<<endl;
                goto bankomat;

            }


            if(bankomat_wyb==1){

                cout<<"Podaj ile chcesz wplacic: ";
                cin>>bankomat_kasa;

                stringstream ss2;
                ss2 << "UPDATE account SET amount = amount+ " << bankomat_kasa << " WHERE number_account = '" << bankomat_nr << "'";
                string query2 = ss2.str();
                const char* q2 = query2.c_str();
                qstate = mysql_query(conn, q2);

                cout<<"Pieniadze zostaly wplacone"<<endl;

                qstate = mysql_query(conn, q);

                res = mysql_store_result(conn);
                row= mysql_fetch_row(res);

                cout<<"Aktualny stan konta: "<<row[1]<<" PLN"<<endl;

            }

              if(bankomat_wyb==2)
                {
                    cout<<"Podaj ile chcesz wyplacic: ";
                    cin>>bankomat_kasa;


                    qstate = mysql_query(conn, q);

                    res = mysql_store_result(conn);
                    row= mysql_fetch_row(res);

                    int val = atoi(row[1]);

                    if(val<bankomat_kasa){

                        cout<<"Brak srodkow"<<endl;

                    }else{

                        stringstream ss2;
                        ss2 << "UPDATE account SET amount = amount- " << bankomat_kasa << " WHERE number_account = '" << bankomat_nr << "'";
                        string query2 = ss2.str();
                        const char* q2 = query2.c_str();
                        qstate = mysql_query(conn, q2);

                        cout<<"Pieniadze zostaly wyplacone"<<endl;

                        qstate = mysql_query(conn, q);

                        res = mysql_store_result(conn);
                        row= mysql_fetch_row(res);

                        cout<<"Aktualny stan konta: "<<row[1]<<" PLN"<<endl;

                    }

                }
        }
        else
        cout<<"Dane nieprawidlowe"<<endl;



     /*   n=bankomat_nr.length();
        p=bankomat_pin.length();

        if(n==16 && p==4)
        {
            if(bankomat_wyb==1)
                {
            cout<<"Podaj ile chcesz wplacic: ";
            cin>>bankomat_kasa;
                }
            if(bankomat_wyb==2)
                {
            cout<<"Podaj ile chcesz wyplacic: ";
            cin>>bankomat_kasa;
                }
        }
        else
        {
            cout<<"Podaj poprawny nr. konta oraz pin"<<endl;
            getch();
            system("cls");
            goto bankomat;
        }
    }

    */
    }

    if(bankomat_wyb==3)
            {
        return(0);
            }

}




