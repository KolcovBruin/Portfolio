#include <stdio.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>

struct mymsgbuf {
    long mtype; //тип сообщения
    long pid; //id процесса
   // key_t qid; //id очереди
    int text; //текст сообщения
};

long MSG = 1;
long ANSWER = 2;

int TRUE = 1;
int FALSE = 0;

int open_queue(key_t keyval) {
    int qid;

    if ((qid = msgget(keyval, IPC_CREAT | 0660)) == -1) { //создание очереди с созданным ключём
        return (-1);
    }
    return (qid);
}

int send_message(int qid, struct mymsgbuf *qbuf) {
    int result, length;

    length = sizeof(struct mymsgbuf) - sizeof(long);

    if ((result = msgsnd(qid, qbuf, length, IPC_NOWAIT)) == -1) { //добавление сообщения в очереждь. qid - id очереди полученный от msgget, qbuf - указатель на сообщение, lenght длина сообщения, IPC_NOWAIT - немедленный возврат, если в очереди нет сообщений необходимого типа. При этом системный вызов возвращает ошибку, присваивая errno значение ENOMSG. Если в очереди нет сообщения необходимого типа и в msgflg не указан IPC_NOWAIT, то вызывающий процесс будет заблокирован до тех пор, пока не произойдёт одно из следующих событий:-  в очередь не будет помещено сообщение необходимого типа;-  очередь сообщений удалена из системы. В этом случае системный вызов возвращает ошибку и присваивает errno значение EIDRM; - вызывающий процесс не получит сигнал, который должен обработать. В этом случае системный вызов возвращает ошибку и присваивает переменной errno значение EINTR. Вызов msgrcv() никогда не перезапускается автоматически после прерывания обработчиком сигнала, независимо от установки флага SA_RESTART при настройке обработчика сигнала.

        return (-1);
    }

    return (result);
}

int read_message(int qid, long type, struct mymsgbuf *qbuf) {
    int result, length;

    length = sizeof(struct mymsgbuf) - sizeof(long);

    if ((result = msgrcv(qid, qbuf, length, type, IPC_NOWAIT)) == -1) { //всё тоже самое только достаем из очереди
        return (-1);
    }

    return (result);
}

int peek_message(int qid, long type) {
    if ((msgrcv(qid, NULL, 0, type, IPC_NOWAIT)) == -1) {
        if (errno == E2BIG) //Но проверили возвращение E2BIG, которое должно показать, существует ли сообщение затребованного типа.
            return (TRUE);
    }
    return (FALSE);
}

int remove_queue(int qid) {
    if (msgctl(qid, IPC_RMID, 0) == -1) {
        return (-1);
    }
    return (0);
}

int main() {
    int qid_server;
    //int qid_client;
    key_t msgkey;

    msgkey = ftok("/home/", 'm');

    if (msgkey != -1) {
        printf("%s", "Key for queue was build\n");
    }

    if ((qid_server = open_queue(msgkey)) == -1) {
        perror("open_queue");
        return -1;
    }

    struct mymsgbuf msg;

    while (1) {
        if (peek_message(qid_server, MSG)) { //проверяем есть в очереди сообщения нужного типа
            read_message(qid_server, MSG, &msg);
            printf("[SERVER] MSG from CLIENT (%ld): \"%d\"\n", msg.pid, msg.text);
            msg.mtype = msg.pid;
            msg.text = msg.text - 1;
            send_message(qid_server, &msg);
        } else {
            printf("[SERVER] No MSG in queue \n");
            sleep(5);
        }
    }
    remove_queue(qid_server);
}
