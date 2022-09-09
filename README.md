List of Button Assignment Ids

- SELECT      = 1
- START       = 8
- UP          = 16
- RIGHT       = 32
- DOWN        = 64
- LEFT        = 128
- L2          = 256      # Only Supported on DS4/DS3 controller
- R2          = 512      # Only Supported on DS4/DS3 controller
- L1          = 1024
- R1          = 2048
- TRIANGLE    = 4096
- CIRCLE      = 8192
- CROSS       = 16384
- SQUARE      = 32768
- PSBUTTON    = 65536

Create a file called "ur0:tai/qblauncher.ini" or "ux0:tai/qblauncher.ini".
If both files exists, the file in "ux0:tai/" will be used instead.

In the file, create a key combination separated by commas followed by "=" and then the title_id of App or Game.

NOTE: For now only 1 key combination is supported.

Example: You want the button combination of L1,L2,CIRCLE to trigger VitaShell.

1024,2048,8192=VITASHELL
