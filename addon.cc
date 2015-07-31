#include <v8.h>
#include <nan.h>
#include <node.h>
#include <pthread.h>
#include <unistd.h>

using namespace v8;
NanCallback *callback;

void* threadFun(void*);

NAN_METHOD(Run) 
{
    NanScope();
    callback = new NanCallback(args[0].As<Function>());

    pthread_t thread1;
    int iret1;

    iret1 = pthread_create(&thread1, NULL, threadFun, NULL);
    if(iret1)
    {
        NanThrowError("Can't start thread");
    }

    NanReturnUndefined();
}

void nop(uv_work_t* req)
{
    //Do nothing
}

void uv_exit(uv_work_t* req, int status)
{
    delete req;

    NanScope();
    Local<Value> argv[] = {
        NanNull(),
        NanNew<Number>(-1)
    };

    callback->Call(2, argv);

    delete callback;
}

void uv_req(uv_work_t* req, int status)
{
    int x = *(int*)(req->data);
    delete req;

    NanScope();
    Local<Value> argv[] = {
        NanNull(),
        NanNew<Number>(x)
    };

    callback->Call(2, argv);
}

void* threadFun(void*) {
    uv_work_t* uv;

    for(int i = 0; ; i++)
    {
        uv = new uv_work_t;
        uv->data = (void *)new int;
        *(int*)uv->data = i;

        int r = uv_queue_work(uv_default_loop(), uv, nop, uv_req);

        if(r != 0)
        {
            delete uv;
        }

        sleep(1);
    }

    uv = new uv_work_t;
    uv->data = (void*)new int;
    *(int*)uv->data = 0;

    int r = uv_queue_work(uv_default_loop(), uv, nop, uv_exit);

    if(r != 0)
    {
        delete uv;
    }

    sleep(1);
}

void Init(Handle<Object> exports)
{
    exports->Set(NanNew<String>("run"), NanNew<FunctionTemplate>(Run)->GetFunction());
}


NODE_MODULE(addon, Init);
