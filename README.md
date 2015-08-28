Ardo Arduino
============

Libraries:
- [aRest]
- [SPI]
- [Ethernet]
 
**Routes**

**Route /play**<br>
usage: /play?params={your song}<br>
Response: `{"return_value": 1, "id": "001", "name": "Ardo", "connected": true}`<br>
<br>
Available notes:<br>
`'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'`<br>
Example: `/play?params=cdec cdec efg efg gagfec gagfec`
<br>
<br>
**Route /temp**<br>
usage: /temp<br>
Response: `{"return_value": 22, "id": "001", "name": "Ardo", "connected": true}`<br>

**Route /temp**<br>
usage: /temp<br>
Response: `{"return_value": 25, "id": "001", "name": "Ardo", "connected": true}`<br>
<br>
How lower the return_value how darker it is.<br>

[aRest]: https://github.com/marcoschwartz/aREST
[SPI]: https://www.arduino.cc/en/Reference/SPI
[Ethernet]: https://www.arduino.cc/en/Reference/Ethernet
