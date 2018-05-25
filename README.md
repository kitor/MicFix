# MicFix
Bypass Windows 10 1803 disabled Microphone GPO policy

Windows 10 1803 changes policies a bit. Until 1709 privacy settings from Modern UI control panel affected only Modern apps. Now they affects desktop apps too.

However changing corresponding registry values enables microphone for as long as GPO is not updated. This piece of code watches registry key with relevant policies and re-enables microphone if it was disabled. It should be easily expandable for any other privacy options.

App works simply by monitoring registry key and updating it if needed. Windows unlocks microphone immediately after key is updated.
NOTE: If any app requests microphone access when it was disabled, it needs to request it again (or be restarted).

This app needs admin rights as it writes to registry. It should be polished a bit (ask for rights, check if key exists at all, etc) but it was 30 minute hack and last time I used WinAPI was about 10 years ago. PRs are welcome :)
