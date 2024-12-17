#ifndef GITM_INCLUDE_H_
#define GITM_INCLUDE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>

#include <openssl/sha.h>

#if defined(__unix__)
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <limits.h>
#endif

/* you can include what you want here, although headers files above are generally enough for this project */

/* below are utilities prepared for you */

/**
 * @brief Prints an message to standard error stream (aka stderr). The usage is the same as printf.
 * @example ERROR("Hello, world!\n");
 */
#define ERROR(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)

/**
 * @brief Callback function type for walk_path.
 * @note How to understand this type? Recall what you've learned in class!
 */
typedef void *(*callback_t)(char const *path, void *arg);

/**
 * @brief Checks if a file or directory exists.
 * @param path The path to the file or directory.
 * @return true if the file or directory exists, false otherwise.
 */
bool exists(char const *path);

/**
 * @brief Copies a file from src_path to dst_path.
 * @param src_path The path to the source file.
 * @param dst_path The path to the destination file.
 * @return 0 if the file is copied successfully, -1 otherwise.
 * @note The TA didn't use this function in the reference solution.
 */
int copy_file(char const *src_path, char const *dst_path);

/**
 * @brief Moves a file from old_path to new_path. If intermediate directories don't exist, they will be created.
 * @param old_path The path to the old file.
 * @param new_path The path to the new file.
 * @return 0 if the file is moved successfully, -1 otherwise.
 * @note The TA didn't use this function in the reference solution, but it's worthwhile to learn how the function creates intermediate directories briefly.
 */
int rename_file(char const *old_path, char const *new_path);

/**
 * @brief Removes a file or directory.
 * @param path The path to the file or directory.
 * @return 0 if the file or directory is removed successfully, -1 otherwise.
 */
int remove_file(char const *path);

/**
 * @brief Creates a directory.
 * @param path The path to the directory.
 * @return 0 if the directory is created successfully, -1 otherwise.
 */
int make_directory(char const *path);

/**
 * @brief Removes a directory which must be empty.
 * @param path The path to the directory.
 * @return 0 if the directory is removed successfully, -1 otherwise.
 * @note The TA didn't use this function in the reference solution.
 */
int remove_directory(char const *path);

/**
 * @brief Walks through a directory, touching all files under it or its subdirectories and applying a callback function with an argument to them.
 * @param path The path to the directory.
 * @param callback The callback function to apply to each file.
 * @param arg The argument to pass to the callback function.
 * @return 0 if the whole walking is successful, -1 otherwise.
 * @attention This function might be VERY useful.
 */
int walk_path(char const *path, callback_t callback, void *arg);

#define SHA_SUM_LEN ((SHA_DIGEST_LENGTH) * 2)

/**
 * @brief Computes the SHA-1 hash of a data block and generates a hexadecimal string representation of it.
 * @param sum The buffer to store the hash, which must be at least (SHA_SUM_LEN + 1) bytes long (including the null terminator).
 * @param data The data block to hash.
 * @param size The size of the data block.
 * @return 0 if the hash is computed successfully, -1 otherwise.
 */
int sha1sum(char *sum, void const *data, size_t size);

#define TIMESTAMP_LEN 25

/**
 * @brief Generates a timestamp string which looks like "Tue Dec 31 23:59:59 2024".
 * @param buf The buffer to store the timestamp, which must be at least TIMESTAMP_LEN bytes long (including the null terminator).
 * @return 0 if the timestamp is generated successfully, -1 otherwise.
 */
int timestamp(char *buf);

#endif  // GITM_INCLUDE_H_
