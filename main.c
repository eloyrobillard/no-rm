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
    if ((strcmp(argv[i], "-i") && strcmp(argv[i], "-I") &&
         strncmp(argv[i], "--interactive", 13) &&
         strcmp(argv[i], "--one-file-system") &&
         strcmp(argv[i], "--no-preserve-root") &&
         strncmp(argv[i], "--preserve-root", 15) && strcmp(argv[i], "-r") &&
         strcmp(argv[i], "-R") && strcmp(argv[i], "--recursive") &&
         strcmp(argv[i], "-d") && strcmp(argv[i], "--dir") &&
         strcmp(argv[i], "-v") && strcmp(argv[i], "--verbose")) == 0) {
      // skip options that rm supports but gio trash doesn't
      continue;
    } else if (strcmp(argv[i], "--version") == 0) {
      cmd = "gio --version";
      break;
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
    } else {
      char *p = argv[i];
      while (*p && *p != ' ')
        p++;

      if (*p == ' ') {
        cmd[end++] = '"';
        strcat(&cmd[end], argv[i]);
        end += strlen(argv[i]);
        cmd[end++] = '"';
        cmd[end++] = ' ';
      } else {
        strcat(strcat(&cmd[end], argv[i]), " ");
        end += strlen(argv[i]) + 1;
      }
    }
  }

  system(cmd);

  return 0;
}
