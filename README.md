# blackout-detect-o-matic
Arduino project to detect and log electricity blackouts.
Useful if one goes on a holiday and wants to know if
there was a long enough blackout during the holiday that
would ruin all the food in the freezer.

Also it should be 'quick' enough to detect momentary blackouts.


## Configure WiFi and other settings
1. Compile and upload image
2. Connect to serial console (for example: `pio run -t monitor`)
3. Use `config get` to view available config options
4. Set WiFi credentials:
   ```text
   config set wifiSSID=<your wifi ssid>
   config set wifiPassword=<your wifi password>
   ``` 
5. Save configuration with `config save` command
6. Reboot the device with `reboot` command