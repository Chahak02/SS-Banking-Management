#ifndef TRANSACTION_HISTORY_H
#define TRANSACTION_HISTORY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include "commonStruct.h"

/**
 * @brief Get the current system date and time as a formatted string
 */
void get_current_time(char *buffer, size_t buffer_size)
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

/**
 * @brief Internal helper — silently get current balance of a user
 * (Unlike view_balance, it does NOT send anything to the client)
 */
int get_balance(int customer_id)
{
    int fd = open("customer_db.txt", O_RDONLY);
    if (fd == -1)
        return -1;

    struct customer temp;
    int balance = -1;

    while (read(fd, &temp, sizeof(struct customer)) > 0)
    {
        if (temp.customer_id == customer_id)
        {
            balance = temp.balance;
            break;
        }
    }
    close(fd);
    return balance;
}

/**
 * @brief Log a new transaction silently (no messages sent to client)
 */
void log_transaction(int client_socket, int customer_id, int amount, char *transaction_type)
{
    struct transact new_transaction;
    new_transaction.user_id = customer_id;

    // Use silent balance fetch
    int current_balance = get_balance(customer_id);
    new_transaction.current_balance = current_balance;

    // Prepare formatted transaction string
    char temp_buffer[128];
    snprintf(temp_buffer, sizeof(temp_buffer), "%s: %d", transaction_type, amount);
    strcpy(new_transaction.transaction, temp_buffer);

    // Timestamp
    get_current_time(new_transaction.date_time, sizeof(new_transaction.date_time));

    // Open transaction log
    int fd_transact = open("transaction_db.txt", O_RDWR);
    if (fd_transact == -1)
    {
        perror("open system call error");
        return;
    }

    struct flock lk_tr;
    lk_tr.l_type = F_WRLCK;
    lk_tr.l_whence = SEEK_END;
    lk_tr.l_start = 0;
    lk_tr.l_len = sizeof(struct transact);

    if (fcntl(fd_transact, F_SETLKW, &lk_tr) == -1)
    {
        perror("error fcntl");
        close(fd_transact);
        return;
    }

    lseek(fd_transact, 0, SEEK_END);
    write(fd_transact, &new_transaction, sizeof(struct transact));

    lk_tr.l_type = F_UNLCK;
    fcntl(fd_transact, F_SETLK, &lk_tr);

    close(fd_transact);
}

/**
 * @brief Display transaction history for a specific customer
 */
void show_transaction_history(int cust_id)
{
    int fd_transact = open("transaction_db.txt", O_RDONLY);
    if (fd_transact == -1)
    {
        perror("error open");
        return;
    }

    struct transact temp_trans;
    while (read(fd_transact, &temp_trans, sizeof(struct transact)) > 0)
    {
        if (temp_trans.user_id == cust_id)
        {
            printf("| user id:|%d|  status:|%s|  date: |%s| current Balance: |%d| |\n",
                   temp_trans.user_id,
                   temp_trans.transaction,
                   temp_trans.date_time,
                   temp_trans.current_balance);
        }
    }

    close(fd_transact);
}

#endif
