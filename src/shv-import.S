# Bridge GCC's C identifier to the stable x64 Script Hook V export.
# The import table makes Windows load ScriptHookV.dll before SoraMod's DllMain.
.text
.globl shvScriptWait
shvScriptWait:
    jmp *"__imp_?scriptWait@@YAXK@Z"(%rip)
