#include <windows.h>

#include <napi.h>


/*
typedef struct tagINPUT {
    DWORD   type;

    union
    {
        MOUSEINPUT      mi;
        KEYBDINPUT      ki;
        HARDWAREINPUT   hi;
    } DUMMYUNIONNAME;
} INPUT, *PINPUT, FAR* LPINPUT;*/
/*
	typedef struct tagKEYBDINPUT {
    WORD    wVk;
    WORD    wScan;
    DWORD   dwFlags;
    DWORD   time;
    ULONG_PTR dwExtraInfo;
} KEYBDINPUT, *PKEYBDINPUT, FAR* LPKEYBDINPUT;
*/

class INPUTDummyClass{
	public:
		INPUT inp;
};
typedef Napi::External<INPUTDummyClass> InputExternal;

InputExternal CreateKBDInpVKey(const Napi::CallbackInfo &info){
	auto env = info.Env();
	if(info.Length() != 2)
		Napi::TypeError::New(env, "Expected 2 parameters!").ThrowAsJavaScriptException();
	if(!info[0].IsNumber())
		Napi::TypeError::New(env, "Expected param 1 to be a uint!").ThrowAsJavaScriptException();
	uint32_t val = info[0].As<Napi::Number>().Uint32Value();
	if(!info[1].IsBoolean())
		Napi::TypeError::New(env, "Expected param 2 to be a boolean!").ThrowAsJavaScriptException();
	bool up = info[1].As<Napi::Boolean>().Value();
	INPUTDummyClass* idc = new INPUTDummyClass;
	idc->inp.type = INPUT_KEYBOARD;
	idc->inp.ki.wVk = val & 0xFFFF;
	idc->inp.ki.wScan = 0;
	idc->inp.ki.time = 0;
	idc->inp.ki.dwFlags = (up ? KEYEVENTF_KEYUP : 0);
	return Napi::External<INPUTDummyClass>::New(env, idc, [](Napi::Env env, INPUTDummyClass* data) {
		delete data;
	});
}

InputExternal CreateKBDInpScanCode(const Napi::CallbackInfo &info){
	auto env = info.Env();
	if(info.Length() < 2 || info.Length() > 3)
		Napi::TypeError::New(env, "Expected 2 or 3 parameters!").ThrowAsJavaScriptException();
	if(!info[0].IsNumber())
		Napi::TypeError::New(env, "Expected param 1 to be a uint!").ThrowAsJavaScriptException();
	uint32_t val = info[0].As<Napi::Number>().Uint32Value();
	if(!info[1].IsBoolean())
		Napi::TypeError::New(env, "Expected param 2 to be a boolean!").ThrowAsJavaScriptException();
	bool up = info[1].As<Napi::Boolean>().Value();
	bool extended = false;
	if(info.Length() > 2){
		if(!info[2].IsBoolean())
			Napi::TypeError::New(env, "Expected param 3 to be a boolean!").ThrowAsJavaScriptException();
		extended = info[2].As<Napi::Boolean>().Value();
	}
	INPUTDummyClass* idc = new INPUTDummyClass;
	idc->inp.type = INPUT_KEYBOARD;
	idc->inp.ki.wVk = 0;
	idc->inp.ki.wScan = val & 0xFFFF;
	idc->inp.ki.time = 0;
	idc->inp.ki.dwFlags = KEYEVENTF_SCANCODE | ( extended ? KEYEVENTF_EXTENDEDKEY : 0 ) | ( up ? KEYEVENTF_KEYUP : 0 );
	return Napi::External<INPUTDummyClass>::New(env, idc, [](Napi::Env env, INPUTDummyClass* data) {
		delete data;
	});
}

InputExternal CreateKBDInpUnicode(const Napi::CallbackInfo &info){
	auto env = info.Env();
	if(info.Length() != 2)
		Napi::TypeError::New(env, "Expected 2 parameters!").ThrowAsJavaScriptException();
	if(!info[0].IsNumber())
		Napi::TypeError::New(env, "Expected param 1 to be a uint!").ThrowAsJavaScriptException();
	uint32_t val = info[0].As<Napi::Number>().Uint32Value();
	if(!info[1].IsBoolean())
		Napi::TypeError::New(env, "Expected param 2 to be a boolean!").ThrowAsJavaScriptException();
	bool up = info[1].As<Napi::Boolean>().Value();
	INPUTDummyClass* idc = new INPUTDummyClass;
	idc->inp.type = INPUT_KEYBOARD;
	idc->inp.ki.wVk = 0;
	idc->inp.ki.wScan = val & 0xFFFF;
	idc->inp.ki.time = 0;
	idc->inp.ki.dwFlags = KEYEVENTF_UNICODE | ( up ? KEYEVENTF_KEYUP : 0 );
	return Napi::External<INPUTDummyClass>::New(env, idc, [](Napi::Env env, INPUTDummyClass* data) {
		delete data;
	});
}

Napi::Number SendInputWrapper(const Napi::CallbackInfo &info){
	Napi::Env env = info.Env();
	if (info.Length() != 1 || !info[0].IsArray())
		Napi::TypeError::New(env, "Expected an array as only parameter to SendInput!").ThrowAsJavaScriptException();
	auto arr = info[0].As<Napi::Array>();
	int ret = 0;
	auto len = arr.Length();
	if (len > 0){
		for (uint32_t i = 0; i < len; i++){
			auto v = arr.Get(i);
			if(!v.IsExternal() || dynamic_cast<INPUTDummyClass*>(v.As<InputExternal>().Data()) == nullptr)
				Napi::TypeError::New(env, "Expected an array with only INPUT elements!").ThrowAsJavaScriptException();
		}
		INPUT* inputs = new INPUT[len];
		for (uint32_t i = 0; i < len; i++){
			auto v = arr.Get(i);
			inputs[i] = v.As<InputExternal>().Data()->inp;
		}
		ret = SendInput(len, inputs, sizeof(INPUT));
	}
	return Napi::Number::New(env, ret);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
	exports.Set("SendInput", Napi::Function::New(env, SendInputWrapper));
	exports.Set("CreateKBDInpVKey", Napi::Function::New(env, CreateKBDInpVKey));
	exports.Set("CreateKBDInpScanCode", Napi::Function::New(env, CreateKBDInpScanCode));
	exports.Set("CreateKBDInpUnicode", Napi::Function::New(env, CreateKBDInpUnicode));
	return exports;
}

NODE_API_MODULE(win32_sendinput, InitAll)