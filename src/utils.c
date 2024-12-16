#include "gitm.h"

static char const GITM_DIR[] = ".gitm"; // make sure this is the name of your gitm directory

bool exists(char const *path) {
  struct stat s;
  return stat(path, &s) == 0;
}

__attribute_maybe_unused__
int copy_file(char const *src_path, char const *dest_path) {
  FILE *src = fopen(src_path, "rb");
  if (src == NULL) {
    ERROR("failed to open file %s\n", src_path);
    return -1;
  }
  char path[PATH_MAX];
  for (char const *slash = strchr(dest_path, '/'); *slash != '\0'; slash = strchr(slash + 1, '/')) {
    strncpy(path, dest_path, slash - dest_path);
    path[slash - dest_path] = '\0';
    if (!exists(path) && make_directory(path) != 0) {
      ERROR("failed to create directory %s\n", path);
      fclose(src);
      return -1;
    }
  }
  FILE *dest = fopen(dest_path, "wb");
  if (dest == NULL) {
    ERROR("failed to open file %s\n", dest_path);
    fclose(src);
    return -1;
  }
  char buf[BUFSIZ];
  size_t n;
  while ((n = fread(buf, 1, sizeof(buf), src)) > 0) {
    if (fwrite(buf, 1, n, dest) != n) {
      ERROR("failed to write to file %s\n", dest_path);
      fclose(src);
      fclose(dest);
      return -1;
    }
  }
  fclose(src);
  fclose(dest);
  return 0;
}

__attribute_maybe_unused__
int rename_file(char const *old_path, char const *new_path) {
  return rename(old_path, new_path);
}

__attribute_maybe_unused__
int remove_file(char const *path) {
  return remove(path);
}

int make_directory(char const *path) {
#if defined(__unix__)
  return mkdir(path, 0755);
#elif defined(_WIN32) || defined(_WIN64)
  return mkdir(path);
#endif
}

__attribute_maybe_unused__
int remove_directory(char const *path) {
  return rmdir(path);
}

int walk_path(char const *path, callback_t callback, void *arg) {
  DIR *dir = opendir(path);
  if (dir == NULL) {
    ERROR("failed to open directory %s\n", path);
    return -1;
  }
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    struct stat s;
    char new_path[PATH_MAX];
    snprintf(new_path, PATH_MAX, "%s/%s", path, entry->d_name);
    if (stat(new_path, &s) != 0) {
      ERROR("failed to get the status of file %s\n", entry->d_name);
      return -1;
    }
    if (s.st_mode & S_IFDIR) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, GITM_DIR) == 0) {
        continue;
      }
      walk_path(new_path, callback, arg);
    } else if (s.st_mode & S_IFREG) {
      callback(new_path, arg);
    } else {
      ERROR("unknown file type %s\n", entry->d_name);
      return -1;
    }
  }
  closedir(dir);
  return 0;
}

__attribute_maybe_unused__
int sha1sum(char *sum, void const *data, size_t size) {
  SHA_CTX const *ctx = (SHA_CTX *)SHA1(data, size, NULL);
  if (ctx == NULL) {
    ERROR("failed to calculate sha1sum\n");
    return -1;
  }
  unsigned char const *buf = (unsigned char const *)ctx;
  for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
    sprintf(sum + i * 2, "%02x", buf[i]);
  }
  return 0;
}

__attribute_maybe_unused__
int timestamp(char *buf) {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  strftime(buf, TIMESTAMP_LEN, "%a %b %d %H:%M:%S %Y", tm);
  return 0;
}
