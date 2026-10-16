## Welcome to our project for Engineering Innovation Design

## Early Stage
<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/9a3e59fd-5633-4293-8993-cb396f539b02" />

## Final Stage
<img width="960" height="1280" alt="image" src="https://github.com/user-attachments/assets/31977847-79d4-4405-a147-76a4b0744f85" />



## Problem Statement

Retrieving items from storage is still largely manual in many settings. In banks, staff retrieve deposit boxes by hand, which is slow and raises security concerns because more people need physical access to the storage area. In warehouses, manually locating and fetching items is time-consuming and prone to human error. 

## Solution

We designed and built a working prototype **Automated Storage and Retrieval System (AS/RS)** that retrieves items from designated storage slots on demand.

**How it works:**
1. The user selects a storage box (e.g., Box 1) on the **Nextion HMI touchscreen**.
2. The HMI sends the command to an **Arduino** controller over UART serial.
3. The Arduino drives **DC encoder motors** to move the system to the selected slot, using encoder feedback for precise positioning.
4. **Linear actuators** engage to grab and retrieve the item.
5. The system brings the item back to the user and returns to its home position, ready for the next request.

## Hardware

| Component | Purpose |
|---|---|
| Arduino | Main controller |
| DC encoder motors | Position |
| Linear actuators | Movement in XYZ plane |
| Nextion HMI | Touchscreen user interface |
| Custom machined parts | Structural frame and mounts |# EID Project


## Folder guide

| Folder / file | What it is |
|---|---|
| `working_final_no_revisi/` | Final working Arduino code (Nextion display + servo control) |
| `nextiondisp/` | Nextion Editor project (`ScreenUI.HMI`) and fonts |
| `Shelves/`, `Linear Slider/`, `Linear Actuator/` | SolidWorks parts and assemblies |
| `Assembly.SLDASM` | Full assembly |
| `*.STL` | 3D-print files |

## Requirements

- Arduino IDE
- Nextion Arduino library (ITEADLIB_Arduino_Nextion)
- Servo library
- Nextion Editor (to open the `.HMI` file)
- SolidWorks (to open the CAD files)

## Contributors
1. Justin <a href = "https://github.com/sntin">(@sntin)</a>
2. Jason Jonathan <a href = "https://github.com/jejeez">(@jejeez)</a>
3. Justin Wong
4. Sheng
5. Michelle
6. Fritz Luhur
7. Ivan Effendi
8. Azka
9. Timothy
