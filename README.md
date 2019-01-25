# sendinput

This module enables you to send INPUTS via `SendInput` (win32 api) function. Therefore it is Windows only.
Currently only keyboard input are supported, you can send VirtualKeys, ScanCodes and Unicode.

API is described by Typescript definition:

```
enum KBDInputType{
    VirtualKey = 0, ScanCode = 1, Unicode = 2
}
interface KBDInput{
    up: boolean;
    val: number;
    type: KBDInputType;
}
function SendInput(inputs: Array<KBDInput>|KBDInput);
```

You should only pass single bytes when sending a scancode input, however you may send two bytes, if the first byte is a 0xe0.
