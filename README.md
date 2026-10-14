# Smart Drone Hangar  hangar

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Java](https://img.shields.io/badge/Java-007396?style=for-the-badge&logo=java&logoColor=white)

This project implements an intelligent drone hangar system that automates the management of drones, including takeoff, landing, and environmental monitoring. It comprises two main components: an embedded system for controlling the hangar hardware and a remote unit for user interaction and monitoring.

## Table of Contents 

* [About the Project](#about-the-project)
* [Features](#features) 
* [Tech Stack](#tech-stack) 
* [Project Structure](#project-structure) 
* [License](#license) 

## About the Project 

The Smart Drone Hangar is an automated system designed to manage drone operations within a dedicated hangar. It handles the entire lifecycle of a drone's stay, from taking off and landing to monitoring the hangar's environmental conditions and ensuring the safety of operations. The system is composed of an embedded C++ application running on a microcontroller (likely an Arduino-based platform) that controls the physical components of the hangar, and a Java-based remote unit that provides a user interface for monitoring and control.

## Features 

*   **Automated Drone Operations:** Manages drone takeoff and landing sequences.
*   **Hangar Door Control:** Automatically opens and closes the hangar door for drone entry and exit.
*   **Environmental Monitoring:** Tracks temperature within the hangar, triggering alerts for high-temperature conditions.
*   **Drone Detection:** Utilizes sensors (PIR and Sonar) to detect the presence and distance of drones.
*   **Safety Alerts:** Implements pre-alarm and alarm states based on temperature thresholds.
*   **Remote Monitoring Interface:** A Java Swing application provides a GUI for viewing drone status, environmental data, and controlling operations.
*   **Serial Communication:** Enables communication between the embedded hardware and the remote monitoring unit.
*   **Task Scheduling:** Employs a scheduler for managing concurrent tasks on the embedded system.
*   **Hardware Abstraction:** Provides a clear interface for interacting with various hardware components like LEDs, motors, and sensors.

## Tech Stack 

*   **Embedded:**
    *   **Language:** C++
    *   **Framework/Platform:** Arduino
    *   **Libraries:** TimerOne, jssc (for serial communication in remote unit)
*   **Remote Unit:**
    *   **Language:** Java
    *   **GUI Framework:** Swing
    *   **Libraries:** jssc (for serial communication)

## Project Structure 

```
Smart_Drone_Hangar/
├── drone-hangar/
│   ├── src/
│   │   ├── config.h
│   │   ├── devices/
│   │   │   ├── Button.h
│   │   │   ├── ButtonImpl.cpp
│   │   │   ├── ButtonImpl.h
│   │   │   ├── Led.cpp
│   │   │   ├── Led.h
│   │   │   ├── LedExt.cpp
│   │   │   ├── LedExt.h
│   │   │   ├── Light.h
│   │   │   ├── LightExt.h
│   │   │   ├── LightSensor.h
│   │   │   ├── LightSensorImpl.cpp
│   │   │   ├── LightSensorImpl.h
│   │   │   ├── Pir.cpp
│   │   │   ├── Pir.h
│   │   │   ├── PresenceSensor.h
│   │   │   ├── ProximitySensor.h
│   │   │   ├── ServoMotor.h
│   │   │   ├── ServoMotorImpl.cpp
│   │   │   ├── ServoMotorImpl.h
│   │   │   ├── ServoTimer2.cpp
│   │   │   ├── ServoTimer2.h
│   │   │   ├── Sonar.cpp
│   │   │   ├── Sonar.h
│   │   │   ├── TempSensor.h
│   │   │   ├── TempSensorTMP36.cpp
│   │   │   ├── TempSensorTMP36.h
│   │   │   └── ... (other device headers)
│   │   ├── kernel/
│   │   │   ├── Logger.cpp
│   │   │   ├── Logger.h
│   │   │   ├── MsgService.cpp
│   │   │   ├── MsgService.h
│   │   │   ├── Scheduler.cpp
│   │   │   ├── Scheduler.h
│   │   │   └── Task.h
│   │   ├── model/
│   │   │   ├── Dashboard.cpp
│   │   │   ├── Dashboard.h
│   │   │   ├── DroneHangar.cpp
│   │   │   ├── DroneHangar.h
│   │   │   ├── HWPlatform.cpp
│   │   │   ├── HWPlatform.h
│   │   │   ├── UserPanel.cpp
│   │   │   └── UserPanel.h
│   │   ├── task/
│   │   │   ├── HangarHealthCheckTask.cpp
│   │   │   ├── HangarHealthCheckTask.h
│   │   │   ├── LandingTask.cpp
│   │   │   ├── LandingTask.h
│   │   │   ├── TakeOffTask.cpp
│   │   │   ├── TakeOffTask.h
│   │   │   ├── TestHWTask.cpp
│   │   │   └── TestHWTask.h
│   │   └── main.cpp
│   └── platformio.ini
├── doc/
│   ├── linkVideo.txt
│   └── report.docx
└── drone-remote-unit/
    ├── java/
    │   ├── src/
    │   │   └── main/
    │   │       ├── CommChannel.java
│   │       ├── DashboardController.java
│   │       ├── DashboardLauncher.java
│   │       ├── DashboardView.java
│   │       ├── LogView.java
│   │       ├── MonitoringAgent.java
│   │       └── SerialCommChannel.java
│   ├── .classpath
│   └── lib/
│       ├── jssc-2.9.4.jar
│       └── slf4j-simple-1.7.36.jar

```
## License 

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


