#include <stdio.h>

#include <string.h>

#define SE - 1
#define MKDIR 1
#define CD 2
#define TOUCH 3
#define RM 4
#define RMDIR 5
#define LS 6
#define FIND 7
#define HELP 8
#define EXIT 9

#define NKEYS(sizeof(lookuptable) / sizeof(cmd_lookup))

int cDir = 0;
int nDir = 0;
int nFil = 0;

struct Dir {
    char dname[10];
    int addr;
}
dir[100];

struct File {
    char fname[10];
    int addr;
}
file[100];

typedef struct {
    char * key;
    int val;
}
cmd_lookup;

static cmd_lookup lookuptable[] = {
    {
        "mkdir",
        MKDIR
    },
    {
        "cd",
        CD
    },
    {
        "touch",
        TOUCH
    },
    {
        "rm",
        RM
    },
    {
        "rmdir",
        RMDIR
    },
    {
        "ls",
        LS
    },
    {
        "find",
        FIND
    },
    {
        "help",
        HELP
    },
    {
        "exit",
        EXIT
    }
};

int parsecmd(char * key) {
    int i;
    for (i = 0; i < NKEYS; i++) {
        cmd_lookup sym = lookuptable[i];
        if (strcmp(sym.key, key) == 0) return sym.val;
    }
    return SE;
}

int nrev(int num) {
    int rev_num = 0;
    while (num > 0) {
        rev_num = rev_num * 100 + num % 100;
        num = num / 100;
    }
    rev_num = rev_num * 100;
    return rev_num;
}

void getdir(int n) {
    int x = 0;
    int i;
    n = nrev(n);
    printf("\x1B[36m");
    printf("/root/");
    while (n) {
        x = ((x * 100) + (n % 100));
        for (i = 0; i <= nDir; i++) {
            if (dir[i].addr == (x)) {
                if (strcmp(dir[i].dname, "root") == 0) continue;
                if (strcmp(dir[i].dname, "NULL") == 0) continue;
                printf("%s/", dir[i].dname);
            }
        }
        n /= 100;
    }
    printf("\x1B[0m");
}

int checkSuffix(int a, int b) {
    char s1[32], s2[32];
    sprintf(s1, "%d", a);
    sprintf(s2, "%d", b);
    int n1 = strlen(s1);
    int n2 = strlen(s2);
    if (n2 % 2 == 0) {
        if (n1 > n2 + 2) return 0;
    } else {
        if (n1 > n2 + 3) return 0;
    }
    if (n1 < n2) {
        return 0;
    }
    for (int i = 0; i < n2; i++) {
        if (s1[n1 - i - 1] != s2[n2 - i - 1]) {
            return 0;
        }
    }
    if (n1 == n2) return 0;
    return 1;
}

void rmdir(int addr) {
    int i;
    for (i = 0; i <= nFil; i++) {
        if (checkSuffix(nrev(file[i].addr), nrev(addr))) {
            if (strcmp(file[i].fname, "NULL") == 0) continue;
            strcpy(file[i].fname, "NULL");
            file[i].addr = 0;
        }
    }
    for (i = 0; i <= nDir; i++) {
        if (dir[i].addr == addr || dir[i].addr == 0) continue;
        if (checkSuffix(nrev(dir[i].addr), nrev(addr))) {
            rmdir(dir[i].addr);
        }
    }
    for (i = 0; i <= nDir; i++) {
        if (dir[i].addr == addr) {
            strcpy(dir[i].dname, "NULL");
            dir[i].addr = 0;
        }
    }
}

int main() {
    int addr, x;
    int i, flag;
    char cmd[16];
    char name[16];
    char path[16];
    strcpy(dir[0].dname, "root");
    dir[0].addr = 0;
    while (1) {
        getdir(cDir);
        printf("\e[1;91m"
            " > "
            "\x1B[0m");
        scanf("%s", cmd);
        switch (parsecmd(cmd)) {
        case MKDIR:
            scanf("%s", name);
            if (cDir != 0) {
                printf("\e[0;31m Invalid \e[0m[ Working path should be \x1B[36m/root/ \e[0m]\n");
                break;
            }
            nDir++;
            strcpy(dir[nDir].dname, name);
            dir[nDir].addr = nDir;
            break;
        case TOUCH:
            scanf("%s", name);
            nFil++;
            strcpy(file[nFil].fname, name);
            file[nFil].addr = (cDir * 100) + nFil;
            break;
        case LS:
            x = nrev(cDir);
            for (i = 0; i <= nDir; i++) {
                if (checkSuffix(nrev(dir[i].addr), x)) {
                    printf("\e[0;92m"
                        " %s [dir] \n"
                        "\e[0m", dir[i].dname);
                }
            }
            for (i = 0; i <= nFil; i++) {
                if (checkSuffix(nrev(file[i].addr), x)) {
                    if (strcmp(file[i].fname, "NULL") == 0) continue;
                    printf("\e[0;94m"
                        " %s [file] \n"
                        "\e[0m", file[i].fname);
                }
            }
            break;
        case CD:
            flag = 1;
            scanf("%s", path);
            if (strcmp(path, "..") == 0) {
                cDir /= 100;
            } else {
                for (i = 0; i <= nDir; i++) {
                    if (strcmp(dir[i].dname, path) == 0) {
                        cDir = dir[i].addr;
                        flag = 0;
                    }
                }
                if (flag) printf("\e[0;31m Invalid Directory\n\e[0m");
            }
            break;
        case FIND:
            flag = 1;
            scanf("%s", name);
            for (i = 0; i <= nDir; i++) {
                if (strcmp(dir[i].dname, name) == 0) {
                    printf("\e[0;33m %s (dir) found at path \e[0m", name);
                    getdir(dir[i].addr / 100);
                    printf("\n");
                    flag = 0;
                }
            }
            for (i = 0; i <= nFil; i++) {
                if (strcmp(file[i].fname, name) == 0) {
                    printf("\e[0;33m %s (file) found at path \e[0m", name);
                    getdir(file[i].addr / 100);
                    printf("\n");
                    flag = 0;
                }
            }
            if (flag) printf("\e[0;31m Directory/File not found!\n\e[0m");
            break;
        case RMDIR:
            flag = 1;
            scanf("%s", name);
            for (i = 0; i <= nDir; i++) {
                if (strcmp(dir[i].dname, name) == 0) {
                    rmdir(dir[i].addr);
                    flag = 0;
                }
            }
            if (flag) printf("\e[0;31m Invalid Directory\n\e[0m");
            break;
        case RM:
            scanf("%s", name);
            for (i = 0; i <= nFil; i++) {
                if (checkSuffix(nrev(file[i].addr), x)) {
                    if (strcmp(file[i].fname, name) == 0) {
                        strcpy(file[i].fname, "NULL");
                    }
                }
            }
            break;
        case HELP:
            printf("\e[0;95m");
            printf("\n  ls                | Lists contents of the current directory\n");
            printf("  mkdir {dir_name}  | Creates a directory\n");
            printf("  touch {file_name} | Creates a file\n");
            printf("  cd {dir_name}     | Changes to another directory\n");
            printf("  cd ..             | Changes to parent directory\n");
            printf("  find {dir_name}   | Universal search for a directory\n");
            printf("  find {file_name}  | Universal search for a file\n");
            printf("  rm {file_name}    | Deletes a file\n");
            printf("  rmdir {dir_name}  | Deletes a directory along with its contents\n");
            printf("  exit              | Terminate the program\n\n");
            printf("\e[0m");
            break;
        case SE:
            printf("\e[0;31m"
                " Invalid Command\n\e[0m Type 'help' to get available commands\n");
            break;
        case EXIT:
            return 0;
            break;
        }
    }
    return 0;
}
