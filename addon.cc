#include <v8.h>
#include <nan.h>
#include <node.h>

using namespace v8;

NAN_METHOD(Run) 
{
    NanScope();

    NanReturnUndefined();
}

void Init(Handle<Object> exports)
{
    exports->Set(NanNew<String>("run"), NanNew<FunctionTemplate>(Run)->GetFunction());
}


NODE_MODULE(addon, Init);
