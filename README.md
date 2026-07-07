# Embedded CAN Bus Development

## Circuit Photos

<p align="center">
  <img src="img/smallCircuit1.jpeg" alt="Small circuit 1" width="420" />
  <img src="img/SmallCircuit2.jpeg" alt="Small circuit 2" width="420" />
</p>

## Project Overview

From what this project shows, it is best presented as a progression of engineering work rather than three unrelated Arduino sketches.

The timeline was:

1. **CAN Bus Data Logger**

- Built a CAN logger using an Arduino and MCP2515.
- Learned CAN initialization, frame reception, serial monitoring, SD card logging, and basic hardware interfacing.
- The logger records CAN frames to CSV with a timestamp, CAN ID, DLC, and data bytes.

2. **CAN Bus Filter**

- Reused and extended the logger hardware/software.
- Implemented CAN acceptance filtering and message handling so only selected CAN IDs were processed.
- Demonstrated understanding of MCP2515 configuration beyond simple frame capture.

3. **Arduino as ISP Programmer**

- Used an Arduino as an ISP programmer to program standalone ATmega328P chips.
- Built a standalone CAN logger on the ATmega328P instead of relying on an Arduino Uno board.
- Showed the move from development-board prototyping to a more complete embedded hardware setup.

## Workflow

The workflow is best explained as an engineering evolution:

CAN Logger  
↓  
CAN Filtering  
↓  
Arduino as ISP Programming  
↓  
Standalone ATmega328P CAN Logger

In practice, each revision built on the previous one:

- **Revision 1:** verify CAN communication and serial logging.
- **Revision 2:** add filtering so the logger processes only relevant CAN traffic.
- **Revision 3:** remove dependence on the Arduino Uno by programming a standalone ATmega328P.

This is a stronger story for a CV because it shows increasing complexity, iteration, and embedded systems growth.

## Suggested CV Summary

**Embedded CAN Bus Development**

- Developed an Arduino-based CAN bus data logger using the MCP2515 CAN controller.
- Implemented CAN hardware acceptance filters and masks to selectively process CAN frames.
- Designed and programmed a standalone ATmega328P CAN logger using Arduino as ISP, eliminating dependence on a development board.
- Configured SPI communication, validated CAN data flow, and used serial debugging for hardware bring-up.

## Repository Structure

```text
CAN-Bus-Development/
├── CanFilter/
├── logger/
├── img/
└── README.md
```

## Notes

- Proteus and KiCad schematic files are not included yet.
- The images in `img/` show the smaller circuit revisions and can be expanded later with schematic or PCB documentation.