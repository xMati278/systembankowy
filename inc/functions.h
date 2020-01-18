bool regist(MYSQL* conn){

    MYSQL_ROW row;
    MYSQL_RES* res;

    cout<<"Rozpoczynam proces rejestracji"<<endl;
    Sleep(2500);
    system( "cls" );

    string login;
    string password, password1;
    string account_number;
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
    string acd,b,c;

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
        while(a<4)
    {
        acd = rand ()%10+48;
        c.append(acd);
        a++;
    }
    pin=c;

    if(check_if_exist(conn, "users", "account_number", account_number )){

        goto duplicate;

    }


    cout<<"Twoje  dane to:"<<endl;
    cout<<"Login: "<<login<<endl;
    cout<<"Numer konta: "<<account_number<<endl;
    cout<<"Pin: "<<pin<<endl;
    string confirm;
    cout<<"Potwierdzasz zalozenie konta? T/N"<<endl;
    cin>>confirm;
    if(confirm=="T" || confirm=="t" ){

        int qstate = 0, quantity;

        stringstream ss;
        ss << "INSERT INTO `users`(`id`, `login`, `password`, `account_number`, `pin`) VALUES (NULL,'" <<login<< "','" <<password<< "','" <<account_number<< "','" <<pin<< "')";

        string query = ss.str();
        const char* q = query.c_str();
        qstate = mysql_query(conn, q);

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

bool user_panel(MYSQL* conn, string login ){

    MYSQL_ROW row;
    MYSQL_RES* res;

    system("cls");

    stringstream ss;

    ss << "SELECT * FROM users where login= '" << login << "' ";

    string query = ss.str();
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    cout<<"Witaj, "<<row[1]<<endl;
    cout<<"Twoj numer konta to: "<<row[3]<<endl;


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

        user_panel(conn,login);

    }
    else{

        cout<<"Dane nieprawidlowe"<<endl;
        Sleep(1500);
        goto login;

    }

}




