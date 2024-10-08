<h1 align="center" style="font-weight: bold;"> Control a Suspension Bridge and Ensure Safe Passage for Ships! - 🌉</h1>

[![Maintenance](https://img.shields.io/badge/Maintained%3F-no-red.svg)](https://bitbucket.org/lbesson/ansi-colors) &ensp; &nbsp;[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)&ensp; &nbsp;[![Ask Me Anything !](https://img.shields.io/badge/Ask%20me-anything-1abc9c.svg)](http://www.linkedin.com/in/erivelto-silva-39a61a275) &ensp; &nbsp;![Arduino](https://img.shields.io/badge/-Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)


`Menu:`
<ol>
 <li><a href="#electricalDiagram">Electrical Diagram</a></li> 
 <li><a href="#started">Getting Started</a> </li> 
 <li><a href="#features">Features</a>       </li> 
 <li><a href="#benefits">Benefits</a>       </li> 
 <li><a href="#usedComponents">Used Components</a> </li>
 <li><a href="#simulation">Simulation</a></li>
 <li><a href="#collaborators">Collaborators</a></li> 
 <li><a href="#license">License</a></li> 
 <li><a href="#keyWords">Key words</a> </li> 
</ol>



<p align="left">
  <strong>The goal of this project is to develop an automated suspension bridge control system using sensors and automation technologies. The system aims to ensure the safe passage of ships by automatically managing the bridge's operation, including detecting approaching vessels, signaling with traffic lights, and controlling the bridge's opening and closing. Additionally, it provides a manual mode for professional intervention during maintenance or emergency situations.</strong>
</p>

<h2 id="electricalDiagram"></h2>
<img src="./Docs/diagram.png"  alt="Electrical Diagram" />


<h2 id="features">⚙️ Project Features</h2>

- <strong>Automatic Detection:</strong>
  - Ultrasonic sensors continuously monitor the proximity of approaching ships.
  - The system activates yellow traffic lights when a ship is detected within a 7-14 cm range, indicating its approach.

- <strong>Bridge Operation:</strong>
  - When a ship comes within less than 3 cm of the bridge, red traffic lights are activated.
  - The bridge automatically opens to allow the ship to pass safely.
  - After the ship passes, the bridge waits for 5 seconds before automatically closing.

- <strong>Manual Mode:</strong>
  - The system includes a manual mode for professional use during maintenance, malfunctions, or other necessary situations.
  - In manual mode, users can override automatic controls to ensure safety and proper operation during exceptional circumstances.

<h2 id="benefits">🗒️ Benefits</h2>

- <strong>Enhanced Safety:</strong> The system ensures safe passage for ships by automatically managing the bridge’s operation and preventing accidents.

- <strong>Efficient Traffic Management:</strong> Automatic detection and control reduce the need for manual intervention, streamlining the process and minimizing delays.

- <strong>Resource Optimization:</strong> The system’s automatic operation saves time and reduces the need for constant human supervision, allowing for more efficient resource use.

- <strong>Flexibility in Operation:</strong> The manual mode provides flexibility for maintenance and emergency situations, ensuring the bridge can be safely managed at all times.

- <strong>Convenient Monitoring:</strong> Real-time detection and operation make it easier to manage the bridge, improving overall traffic flow and safety.


<h2 id="usedComponents">🗒️ Used Components</h2>

- Arduino Uno (main micro-controller)
- Ultrasonic modules (HC-SR04)
- Module Traffic Light
- Limit Switches
- Motors
- Buttons
- LCD display with I2C module
- Relay
- Active Buzzer

<h2 id="started">🚀 Getting started</h2>

1. Clone this repository to your local directory.
2. Open the project in `arduino/arduino.ino`  in the Arduino IDE.
3. Configure the arduino uno board and the correct serial port.
4. Upload the code to the arduino uno.
5. Connect the sensors, buttons, buzzer, lcd, relays  etc to the arduino uno. 
6. The system will be ready to use!


<h2 id="simulation">Simulation</h2>

[See it simulation working on my youtube channel](#) 


<h2 id="collaborators">🤝 Collaborators</h2>

Special thank you for all people that contributed for this project.

<table>
  <tr>
    <td align="center">
      <a href="#">
        <img src="https://avatars.githubusercontent.com/u/125351173?s=400&u=d6b335b5e253bb45f3ed82ec322684ab203b5243&v=4" width="100px;" alt="Erivelto Silva Profile Picture"/><br>
        <sub>
          <b>Erivelto Silva</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="#">
        <img src="" width="100px;" alt="Osvaldo Manual Picture"/><br>
        <sub>
          <b>Osvaldo Napoleão Manual</b>
        </sub>
      </a>
    </td>
  </tr>
</table>

<h2 id="license"> LICENSE</h2>

This project is licensed under the [MIT License](LICENSE.txt) © Erivelto Silva.


<h2 id="keyWords">Key Words</h2>

Keys: Arduino Uno, suspension bridge, automation, control system, ultrasonic sensors, traffic lights, manual mode, ship detection, safety, bridge operation.
