from pathlib import Path
import ctypes
folder=Path(__file__).resolve().parents[1]/'build/loader-test'
k=ctypes.WinDLL('kernel32',use_last_error=True)
k.GetModuleHandleW.argtypes=[ctypes.c_wchar_p];k.GetModuleHandleW.restype=ctypes.c_void_p
k.FreeLibrary.argtypes=[ctypes.c_void_p];k.FreeLibrary.restype=ctypes.c_int
assert not k.GetModuleHandleW('ScriptHookV.dll'),'Test must start without a loaded hook'
asi=ctypes.WinDLL(str(folder/'SoraMod.asi'),winmode=0x1100)
assert k.GetModuleHandleW('ScriptHookV.dll'),'ASI must load its dependency automatically'
hook=ctypes.WinDLL(str(folder/'ScriptHookV.dll'))
assert hook.RegistrationCount()==1,'Script entry must be registered'
assert k.FreeLibrary(asi._handle)
assert hook.RegistrationCount()==0,'Script entry must be unregistered on unload'
print('PASS: Windows automatically loads ScriptHookV, registers the trainer and unregisters on unload (mock runtime; no game injection)')
