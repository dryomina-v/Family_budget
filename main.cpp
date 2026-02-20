#include "database/Database.h"
#include "services/TransactionService.h"
#include "handlers/TransactionHandler.h"
#include <iostream>

int main() {
    Database db("budget.db");
    db.init();
    db.clearTransactions();
    
    TransactionService service(db);
    TransactionHandler handler(service);

    auto result = handler.getTransactions(1);

    std::cout << result.dump(4) << std::endl;

    return 0;
}
