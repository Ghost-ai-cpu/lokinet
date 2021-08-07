#include <napi.h>
#include <llarp.hpp>

struct Worktipsnet : public Napi::ObjectWrap< Worktipsnet >
{
  llarp::Context ctx;

  static Napi::Object
  Init(Napi::Env env, Napi::Object exports)
  {
    Napi::HandleScope scope(env);

    Napi::Function func =
        DefineClass(env, "Worktipsnet",
                    {InstanceMethod("configure", &Worktipsnet::Configure),
                     InstanceMethod("run", &Worktipsent::Run),
                     InstanceMethod("kill", &Worktipsnet::Kill)});

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Worktipsnet", func);
    return exports;
  };

  Napi::Value
  Configure(const Napi::CallbackInfo& info)
  {
    if(info.Length() != 1 || !info[0].IsString())
    {
      Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }
    bool result = ctx.LoadConfig(info[0].As< std::string >());
    if(result)
    {
      result &= ctx.Setup() == 0;
    }
    return Napi::Value(info.Env(), result);
  }

  Napi::Value
  Run(const Napi::CallbackInfo& info)
  {
    bool result = ctx.Run() == 0;
    return Napi::Value(info.Env(), result);
  }

  Napi::Value
  Kill(const Napi::CallbackInfo& info)
  {
    bool result = ctx.Stop();
    return Napi::Value(info.Env(), result);
  }
};

Napi::Object
InitAll(Napi::Env env, Napi::Object exports)
{
  return Worktipsnet::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)