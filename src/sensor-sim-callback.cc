// src/sensor-sim-callback.cc
#include "sensor-sim-callback.hh"

void WatchSensor(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  if (args.Length() < 1 || !args[0]->IsFunction())
    return;
  v8::Isolate *isolate = args.GetIsolate();
  v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(args[0]);

  AsyncData *data = new AsyncData();
  data->async.data = data;
  data->callback.Reset(isolate, callback);
  data->context.Reset(isolate, isolate->GetCurrentContext());
  data->isolate_ptr = isolate;
  auto handler = [](uv_async_t *handle)
  {
    AsyncData *data = static_cast<AsyncData *>(handle->data);

    v8::Isolate *isolate = data->isolate_ptr;
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);
    v8::Local<v8::Context> context = data->context.Get(isolate);
    v8::Context::Scope contextScope(context);
    v8::Local<v8::Function> localCallback = data->callback.Get(isolate);

    v8::Local<v8::Value> argv[1] = {v8::Integer::New(isolate, data->value)};

    std::cout << "Handler called with value: " << data->value << std::endl;

    v8::TryCatch tryCatch(isolate);
    localCallback->Call(context, v8::Undefined(isolate), 1, argv);

    // cleanup
    data->callback.Reset();
    data->context.Reset();
    uv_close(reinterpret_cast<uv_handle_t *>(&data->async), [](uv_handle_t *handle)
             { delete static_cast<AsyncData *>(handle->data); });
  };

  uv_async_init(uv_default_loop(), &data->async, handler);

  auto cb2 = [](int value, void *userData)
  {
    std::cout << "Callback received value: " << value << std::endl;
    AsyncData *data = static_cast<AsyncData *>(userData);
    data->value = value;
    uv_async_send(&data->async);
  };

  sensorWatch(cb2, data);
}
