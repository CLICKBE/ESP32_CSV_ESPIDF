# ShakeUp 2022 - ESP32 CSV with ESP-IDF

Quick demonstrator of how to read, parse and store in an array the content of a csv file stored directly on the esp.
One element from each parsed line is then used to build a json string meant to be sent throught BLE.

This code has been developped during CLICK workshop's "Shake Up 2022".

The copy of the csv file on the esp has been done following this tutorial : https://esp32tutorials.com/esp32-spiffs-esp-idf/
    - Display MenuConfig
    - Serial Flasher config : check Flas size
       - Partition table : 
        - Partition table : Custom partition table, 
          - Enter name of csv partition file
        - Hit save

## License
 © 2022 – CLICK - Université de Mons

ESP32 CSV with ESP-IDF– CLICK - UMONS (Loïc Reboursière) is free software: you can redistribute it and/or modify it under the terms of the Apache License Version 2.0. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the Apache License Version 2.0 for more details.
You should have received a copy of the Apache License Version 2.0 along with this program.  If not, see http://www.apache.org/licenses/
Each use of this software must be attributed to University of MONS – CLICK (Loïc Reboursière).
Any other additional authorizations may be asked to avre@umons.ac.be.

## Legal Notices
This work was produced as part of the FEDER Digistorm project, co-financed by the European Union and the Wallonia Region.

![Logo FEDER-FSE](https://www.enmieux.be/sites/default/files/assets/media-files/signatures/vignette_FEDER%2Bwallonie.png)
