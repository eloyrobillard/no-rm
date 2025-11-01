#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int len = 10, end = 10;
  for (int i = 1; i < argc; i++)
    len += strlen(argv[i]) + 1;

  char *cmd = (char *)malloc(2 * len);

  if (cmd == NULL) {
    printf("Failed to allocate memory");
    return 1;
  }

  strcat(cmd, "gio trash ");

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--force") == 0) {
      strcat(&cmd[end], "-f ");
      end += 3;
    } else if (strcmp(argv[i], "-f") == 0) {
      strcat(&cmd[end], "-f ");
      end += 3;
    } else if (argv[i][0] != '-') {
      strcat(strcat(&cmd[end], argv[i]), " ");
      end += strlen(argv[i]) + 1;
    } else if (strcmp(argv[i], "--") == 0) {
      for (int ii = i + 1; ii < argc; ii++) {
        if (argv[ii][0] == '-') {
          strcat(&cmd[end], "./");
          end += 2;
        }

        strcat(strcat(&cmd[end], argv[ii]), " ");
        end += strlen(argv[ii]) + 1;
      }

      break;
    }
  }

  // printf("%d %s\n", argc, cmd);
  system(cmd);

  return 0;
}
