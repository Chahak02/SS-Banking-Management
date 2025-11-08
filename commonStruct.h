#ifndef COMMON_STRUCT_H
#define COMMON_STRUCT_H

#include <stdbool.h>

/**
 * @file commonStruct.h
 * @brief Common data structures used throughout the banking system
 *
 * This file contains all the core data structures that represent
 * different entities in the banking system such as customers,
 * employees, managers, admins, and various banking operations.
 */

/**
 * @struct customer
 * @brief Represents a bank customer account
 *
 * Contains all personal and financial information of a customer
 * Note: customer_id and account_number must be unique across the system
 */
struct customer
{
	char name[128];		// Customer's full name (also used as username)
	int customer_id;	// Unique identifier for the customer
	int account_number; // Unique bank account number
	int balance;		// Current account balance
	bool need_loan;		// Indicates if customer has requested a loan
	int loan_amount;	// Amount of loan requested/granted
	bool loan_approved; // Loan approval status
	char password[128]; // Hashed password for authentication
	bool is_online;		// Current login status
	bool is_active;		// Account status (active/deactivated)
	int contact;		// Contact number
	char address[1024]; // Customer's address
};

/**
 * @struct employee
 * @brief Represents a bank employee
 *
 * Contains employee information and account status
 * Used for regular bank staff members
 */
struct employee
{
	int emp_id;			// Unique employee identifier
	char username[128]; // Employee username
	char password[128]; // Hashed password for authentication
	bool is_online;		// Current login status
	bool is_active;		// Employment status (active/inactive)
						// Used during role change (e.g., promotion to manager)
};

/**
 * @struct loan
 * @brief Represents a loan application/record
 *
 * Tracks loan applications and their status
 * Links customers with the employees handling their loan
 */
struct loan
{
	int emp_id;		 // ID of employee handling the loan
	int customer_id; // ID of customer requesting the loan
	int loan_amount; // Requested/approved loan amount
	char status[64]; // Current loan status (applied/approved)
};

/**
 * @struct manager
 * @brief Represents a bank manager
 *
 * Contains manager information and account status
 * Managers have additional privileges compared to regular employees
 */
struct manager
{
	int emp_id;			// Unique manager identifier
	char username[128]; // Manager username
	char password[128]; // Hashed password for authentication
	bool is_online;		// Current login status
};

/**
 * @struct admin
 * @brief Represents a system administrator
 *
 * Contains admin information and account status
 * Admins have the highest level of system access
 */
struct admin
{
	int emp_id;			// Unique admin identifier
	char username[128]; // Admin username
	char password[128]; // Hashed password for authentication
	bool is_online;		// Current login status
};

/**
 * @struct transact
 * @brief Represents a transaction record
 *
 * Stores details of financial transactions
 * Used for transaction history and audit trails
 */
struct transact
{
	int user_id;		   // ID of user performing the transaction
	char transaction[256]; // Transaction details
	char date_time[128];   // Timestamp of the transaction
	int current_balance;   // Balance after transaction
};

/**
 * @struct feedback
 * @brief Represents a customer feedback or complaint
 *
 * Stores customer feedback and its resolution status
 */
struct feedback
{
	int user_id;		  // ID of user providing feedback
	bool resolved;		  // Whether the feedback has been addressed
	char feed_back[1024]; // Actual feedback content
};

#endif // COMMON_STRUCT_H