#include "gitm.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    ERROR("Usage: gitm <command>\n");
    return 1;
  }
  if (!strcmp(argv[1], "version")) {
    printf("%s",
#include "icon.txt"
    );
  }
  return 0;
}
