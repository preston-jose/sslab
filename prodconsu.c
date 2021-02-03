#include <stdio.h>

int mutex=1, full=0, empty=3, item, x=0, y=0;

int buffer[3];

void insertToBuffer(int item) {
    buffer[x++] = item;
    if(x==3) x=0;
}

int getFromBuffer() {
    int val = buffer[y++];
    if(y==3) y=0;
    return val;
}

int wait(int s)
{
  return (--s);
}
 
int signal(int s)
{
  return(++s);
}

void produce() {
    empty = wait(empty);
    mutex = wait(mutex);
    printf("Item to produce : ");
    scanf("%d", &item);
    insertToBuffer(item);
    mutex = signal(mutex);
    full = signal(full);
}

void consume() {
    full = wait(full);
    mutex = wait(mutex);
    item = getFromBuffer();
    printf("Consumed item %d\n", item);
    mutex = signal(mutex);
    empty = signal(empty);
}


int main() {
    int choice;
    while(1) {
        printf("Choice : ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                if((mutex==1)&&(empty>0)) produce();
                else printf("Buffer Full\n");
                break;
            case 2:
                if((mutex==1)&&(full>0)) consume();
                else printf("Buffer Empty\n");
                break;
            case 3:
                return 0;
        }
    }
    return 0;
}
