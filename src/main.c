#include "gitm.h"
#include "command.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    ERROR("Usage: gitm <command>\n");
    return 1;
  }
  if (!strcmp(argv[1], "version")) {
    printf("%s",
#include "../icon.txt"
    );
    return 0;
  }
  if (!strcmp(argv[1], "init")){
    init();
  }
  if (!strcmp(argv[1], "commit")){
    assert(argc <= 3);
    if (argc == 3){
      commit(argv[2]);
    }
    else{
      commit("\"No messsage\"");
    }
  }
  if (!strcmp(argv[1], "log")){
    print_log();
  }
  if (!strcmp(argv[1], "checkout")){
    assert(argc == 3);
    checkout(argv[2]);
  }
  if (!strcmp(argv[1], "test")){
    test(argv[2]);
  }
  if (!strcmp(argv[1], "clear")){
    clear();
  }
}
