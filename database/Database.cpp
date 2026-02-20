#include "Database.h"
#include <iostream>

void Database::init() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER,
            type TEXT,
            amount REAL
        );
    )";

    char* errMsg = nullptr;

    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Failed to create table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}


Database::Database(const std::string& db_name) {
    if (sqlite3_open(db_name.c_str(), &db)) {
        std::cerr << "Can't open database\n";
    }
}

Database::~Database() {
    sqlite3_close(db);
}


void Database::addTransaction(const Transaction& t) {
    std::string sql = "INSERT INTO transactions (user_id, type, amount) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, t.user_id);
        sqlite3_bind_text(stmt, 2, t.type.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, t.amount);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to insert transaction\n";
        }
    }

    sqlite3_finalize(stmt);
}


std::vector<Transaction> Database::getTransactionsByUser(int user_id) {
    std::vector<Transaction> result;

    std::string sql = "SELECT id, user_id, type, amount FROM transactions WHERE user_id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Transaction t;
            t.id = sqlite3_column_int(stmt, 0);
            t.user_id = sqlite3_column_int(stmt, 1);
            t.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            t.amount = sqlite3_column_double(stmt, 3);

            result.push_back(t);
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

void Database::clearTransactions() {
    const char* sql = "DELETE FROM transactions;";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Failed to clear table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}