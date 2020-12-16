#include <iostream>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <vector>
#include <pthread.h>

#include "UserDefinition.h"
#include "TrashManager.h"

using namespace std;

typedef struct thread_testargs {
    TrashManager* context;
    UserDefinition* udf;
    int ctr;
} ThreadArgs;

void* do_work(void* args) {
    ThreadArgs* local_var = (ThreadArgs*)args;
    local_var->context->move_to_trash(local_var->udf, local_var->ctr);
}

int main(int argc, char** argv) {
    UserDefinition usr_de;
    TrashManager trash_manager;
    ThreadArgs thread_argument[WORKER_PROCESS];
    pthread_t tid[WORKER_PROCESS];

    if (trash_manager.setargs(argc, argv, usr_de) == -1) {
        cerr << "Exiting program due to previous error." << endl;
        return -1;
    }

    for (int i = 0; i < WORKER_PROCESS; i++) {
        thread_argument[i].ctr = i;
        thread_argument[i].context = &trash_manager;
        thread_argument[i].udf = &usr_de;
        pthread_create(&tid[i], NULL, do_work, &thread_argument[i]);
    }

    for (int i = 0; i < WORKER_PROCESS; i++) {
        pthread_join(tid[i], NULL);
    }
    return 0;
}