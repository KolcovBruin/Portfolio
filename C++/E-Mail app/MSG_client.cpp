#include <stdio.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>

struct mymsgbuf
{
    long mtype;
    long pid;
  //  key_t qid;
    int text;
};

long MSG = 1; //тип отправленнго сообщения
long ANSWER = 2; // тип ответа сервера

int TRUE = 1;
int FALSE = 0;

int open_queue(key_t keyval)
{
    int qid;

    if ((qid = msgget(keyval, IPC_CREAT | 0660)) == -1)
    {
        return (-1);
    }
    return (qid);
}

int send_message(int qid, struct mymsgbuf *qbuf)
{
    int result, length;

    length = sizeof(struct mymsgbuf) - sizeof(long);

    if ((result = msgsnd(qid, qbuf, length, IPC_NOWAIT)) == -1)
    {
        return (-1);
    }

    return (result);
}

int read_message(int qid, long type, struct mymsgbuf *qbuf)
{
    int result, length;

    length = sizeof(struct mymsgbuf) - sizeof(long);

    if ((result = msgrcv(qid, qbuf, length, type, IPC_NOWAIT)) == -1)
    {
        return (-1);
    }

    return (result);
}

int peek_message(int qid, long type)
{

    if ((msgrcv(qid, NULL, 0, type, IPC_NOWAIT)) == -1)
    {
        if (errno == E2BIG)
            return (TRUE);
    }
    return (FALSE);
}

int remove_queue(int qid)
{
    if (msgctl(qid, IPC_RMID, 0) == -1)
    {
        return (-1);
    }

    return (0);
}

int main()
{
    int qid_server;
   // int qid_client;
    char str[120];
    pid_t pid = getpid();
    sprintf(str, "%d", pid);
    key_t msgkey_server;
  //  key_t msgkey_client;

    fopen(str, "w");

    msgkey_server = ftok("/home/", 'm');
  //  msgkey_client = ftok(str, 'm');

    if (msgkey_server != -1)
    {
        printf("%s", "Key for queue was build\n");
    }

    if ((qid_server = open_queue(msgkey_server)) == -1)
    {
        perror("open_queue");
        return -1;
    }

//    if ((qid_client = open_queue(msgkey_client)) == -1)
//    {
//        perror("open_queue");
//        return -1;
//    }

    struct mymsgbuf msg;

    while (1)
    {
        if (peek_message(qid_server, pid))
        {
            read_message(qid_server, pid, &msg);
            printf("[CLIENT] ANSWER from server was received \"%d\"\n", msg.text);
        }
        else
        {
            msg.mtype = MSG;
            msg.pid = pid;
            msg.text = pid;
            //msg.qid = qid_client;
            send_message(qid_server, &msg);
            printf("[CLIENT] Sent MSG to SERVER \"%d\" \n", msg.text);
            sleep(5);
        }
    }
}
