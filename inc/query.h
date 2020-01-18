
bool check_if_exist(MYSQL* conn, string db, string target, string value ){

    MYSQL_ROW row;
    MYSQL_RES* res;

    int qstate = 0;
    stringstream ss1;
    ss1 << "SELECT * FROM "<<db<<" where "<<target<<"= '" << value << "'";

    string query = ss1.str();


    const char* q = query.c_str();
    qstate = mysql_query(conn, q);

    res = mysql_store_result(conn);
    row= mysql_fetch_row(res);

    if(row!=0)
        return true;
    else
        return false;


}
