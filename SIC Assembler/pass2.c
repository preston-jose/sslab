#include <stdio.h> include <string.h> include <stdlib.h>

FILE * optab, * symtab, * imf;
const char * getOpcode(char opcode[8]) {
    rewind(optab);
    static char op[8], code[8];
    while (feof(optab) == 0) {
        fscanf(optab, "%s %s", op, code);
        if (strcmp(op, opcode) == 0) {
            return code;
        }
    }
    return NULL;
}
const char * getAddr(char label[8]) {
    rewind(symtab);
    static char lab[8], addr[8];
    while (feof(symtab) == 0) {
        fscanf(symtab, "%s %s", lab, addr);
        if (strcmp(lab, label) == 0) {
            return addr;
        }
    }
    return NULL;
}
int main(void) {
    int p = 0, address, startaddr;
    static char obj[6][100], finalop[6], progname[6];
    static char
    label[8], opcode[8], operand[8];
    optab = fopen("optab", "r");
    symtab = fopen("symtab", "r");
    imf =
        fopen("imf", "r");
    while (feof(imf) == 0) {
        fscanf(imf, "%X %s %s %s", & address, label, opcode, operand);
        if (strcmp(opcode, "START") == 0) {
            strcpy(progname, label);
            startaddr = address;
            continue;
        }
        if (strcmp(opcode, "RESB") == 0) {
            continue;
        }
        if (strcmp(opcode, "RESW") == 0) {
            continue;
        }
        if (strcmp(opcode, "END") == 0) {
            break;
        }
        if (strcmp(opcode, "WORD") == 0) {
            sprintf(obj[p++], "%06X", atoi(operand));
            continue;
        }
        if (strcmp(opcode, "BYTE") == 0) {
            sprintf(obj[p++], "%s", operand);
            continue;
        }
        sprintf(obj[p++], "%s%s", getOpcode(opcode), getAddr(operand));
    }

    printf("H^%-6s^%06X^%06X\n", progname, startaddr, address + 3 - startaddr);

    for (int i = 0; i < p; i++) {
        printf("%s\n", obj[i]);
    }

    return 0;
}
