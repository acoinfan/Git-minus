#ifndef COMMAND_INCLUDE_H_
#define COMMAND_INCLUDE_H_

#include "gitm.h"
#include "macros.h"
#include "committree.h"
#include "filetree.h"
#include "filemanage.h"
#include "structures.h"
#include "pair.h"

/**
 * @brief Initialize a repository
 * @return 0 if the repository is created successfully, -1 otherwise.
 */
int init(void);

/**
 * @brief Commit files and directories
 * @return 0 if the commit succeed, -1 otherwise.
 */
int commit(char *message);

/**
 * @brief Checkout files and directories
 * @return 0 if the checkout succeed, -1 otherwise.
 */
int checkout(char *id);

void print_log(void);

void test(void);

void clear(void);
#endif