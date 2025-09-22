#include "ntp.h"
#include <sqlite3.h>
#include <stdio.h>

int ntp_log_sqlite(const char *dbpath, const ntp_result_t *r)
{
    sqlite3 *db;
    char *err = 0;

    if (sqlite3_open(dbpath, &db))
    {
        fprintf(stderr, "Can't open DB: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    const char *sql =
        "CREATE TABLE IF NOT EXISTS measurements ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "ts DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "offset_ms REAL, "
        "delay_ms REAL);";
    sqlite3_exec(db, sql, 0, 0, &err);

    char query[256];
    snprintf(query, sizeof(query),
             "INSERT INTO measurements (offset_ms, delay_ms) VALUES (%.3f, %.3f);",
             r->offset_s * 1000, r->delay_s * 1000);

    if (sqlite3_exec(db, query, 0, 0, &err) != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err);
        sqlite3_free(err);
    }

    sqlite3_close(db);
    return 0;
}
