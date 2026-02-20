#pragma once
#include <vector>
#include "../models/Transaction.h"
#include <sqlite3.h>

class Database {
public:
    Database(const std::string& db_name);
    ~Database();

    void init();

    void addTransaction(const Transaction& t);
    std::vector<Transaction> getTransactionsByUser(int user_id);

    void clearTransactions();

private:
    sqlite3* db;
};