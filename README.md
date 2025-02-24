# DIY Split Keyboard

## Overview

This project is a highly ergonomic and efficient **DIY split keyboard**, designed to enhance comfort and productivity. The keyboard is built using **three Arduinos**, with an **Arduino Leonardo** acting as the master controller, while the **left and right keypads** are each controlled by an **Arduino Pro Micro**. This setup ensures seamless communication between the components, allowing for a fully functional and customizable typing experience.

## Hardware & Architecture

### **Microcontrollers**

- **Arduino Leonardo** – Acts as the **master controller** and utilizes the **HID (Human Interface Device) library** to function as a keyboard.
- **Arduino Pro Micro (x2)** – One for each **left and right keypad**.

### **Communication Protocol**

- The **Leonardo** communicates with the **left and right keypads** using the **I2C protocol**.
- The **master** continuously polls the **slave devices** for input data, ensuring responsive keypress detection.

### **Key Matrix Design**

One of the challenges of this project was handling **more switches than available input pins**. This issue was solved by implementing a **key matrix**, which allows multiple keys to be scanned efficiently with fewer pins.

- **Rows and columns** were created by:
  - Connecting **Pin 1** of each key switch in a row together.
  - Connecting **Pin 2** of each key switch in a column together.
- This matrix design allows the detection of **30 switches** using only **11 input pins**.
- By activating one column at a time and scanning the rows, keypresses are efficiently detected.

## Software & Programming

- The **Arduino firmware** is written in **C++** using **PlatformIO** within **Visual Studio Code**.
- The **I2C protocol** is used for seamless communication between the microcontrollers.
- The **Arduino Leonardo** uses the **HID library** to emulate a keyboard and send keystrokes to a connected computer.
- Custom firmware ensures accurate key mapping and dynamic switching of inputs.

## Special Features

- **Alternative Shift Key** – Allows access to an **additional set of inputs** (e.g., numbers, symbols) by dynamically switching keys between different characters.
- **Customizable Key Mapping** – The firmware can be easily modified to accommodate different layouts or user preferences.
- **Compact & Ergonomic** – The split keyboard design reduces strain and improves typing comfort over long periods.

## Future Improvements

- Adding **RGB backlighting** for visual feedback and customization.
- Implementing **wireless communication** between the keypads and the master controller.
- Creating a **graphical user interface (GUI)** to allow users to configure key mappings easily.

---

This DIY split keyboard project is a powerful and customizable alternative to commercial ergonomic keyboards, offering great flexibility and an open-source approach to keyboard design!

## Gallery

