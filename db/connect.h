#include <iostream>
#include<windows.h>
#include<mysql.h>
#include <sstream>
#include<stdio.h>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include<conio.h>

using namespace std;

MYSQL* connect(){

    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "host", "login", "haslo", "baza", 0, NULL, 0);


    if(!conn){

        cout<<"Failed connect to db";
        return 0;

    }
    else
        return conn;

}



