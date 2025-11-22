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
    if (argv[i][0] == '-') {
      FILE *file = fopen(argv[i], "r");
      if (file == NULL) {
        if (argv[i][1] != '-') {
          for (char *p = &argv[i][1]; *p; p++) {
            // -f is the only single-dashed option supported by gio trash
            if (*p == 'f') {
              strcat(&cmd[end], "-f ");
              end += 3;
            }
            // ignore everything else
            // this fixes issues with composed options, like -rf
          }
        } else {
          if (strcmp(argv[i], "--force") == 0) {
            strcat(&cmd[end], "-f ");
            end += 3;
          } else if (strcmp(argv[i], "--version") == 0) {
            // if --version, rm shouldn't run so just break
            // not that a script would ever use this option
            cmd = "gio --version";
            break;
          } else if (strcmp(argv[i], "--") == 0) {
            // handle rm's "solution" for filenames starting with a dash
            // forcibly add "./" to the beginning of every filename
            for (int ii = i + 1; ii < argc; ii++) {
              if (argv[ii][0] == '-') {
                strcat(&cmd[end], "./");
                end += 2;
              }

              strcat(strcat(&cmd[end], argv[ii]), " ");
              end += strlen(argv[ii]) + 1;
            }

            break;
          } else if ((strcmp(argv[i], "-i") && strcmp(argv[i], "-I") &&
                      strncmp(argv[i], "--interactive", 13) &&
                      strcmp(argv[i], "--one-file-system") &&
                      strcmp(argv[i], "--no-preserve-root") &&
                      strncmp(argv[i], "--preserve-root", 15) &&
                      strcmp(argv[i], "-r") && strcmp(argv[i], "-R") &&
                      strcmp(argv[i], "--recursive") && strcmp(argv[i], "-d") &&
                      strcmp(argv[i], "--dir") && strcmp(argv[i], "-v") &&
                      strcmp(argv[i], "--verbose")) != 0) {
            // check for bad options (since here we already know they're not
            // files in disguise)
            printf("Bad option: %s", argv[i]);
            return 0;
          }
        }
      } else {
        printf("%s: skipping option, we found a file of the same name in the "
               "current working directory.",
               argv[i]);
      }
      // if arg doesn't start with a dash (likely the path to a file)
    } else {
      char *p = argv[i];
      while (*p && *p != ' ')
        p++;

      // handle filenames containing spaces
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
