# Embedded CAN Bus Development

This repository documents an embedded CAN workflow built around the MCP2515 CAN controller and Arduino-compatible hardware. It starts with logging, moves into filtering, and finishes with programming a standalone ATmega328P using Arduino as ISP.

## Circuit Gallery

All circuit photos included in the repository are shown below.

<table>
  <tr>
    <td align="center"><img src="img/BigCircuit1.jpeg" alt="Big circuit 1" width="380" /><br />BigCircuit1</td>
    <td align="center"><img src="img/BigCircuit2.jpeg" alt="Big circuit 2" width="380" /><br />BigCircuit2</td>
  </tr>
  <tr>
    <td align="center"><img src="img/smallCircuit1.jpeg" alt="Small circuit 1" width="380" /><br />smallCircuit1</td>
    <td align="center"><img src="img/SmallCircuit2.jpeg" alt="Small circuit 2" width="380" /><br />SmallCircuit2</td>
  </tr>
</table>

## Project Flow

The work is best presented as a progression of engineering tasks:

1. **CAN Bus Data Logger**

- Built a CAN logger using an Arduino and MCP2515.
- Learned CAN initialization, frame reception, serial output, SD card logging, and hardware interfacing.
- The logger records CAN frames to CSV with a timestamp, CAN ID, DLC, and data bytes.

2. **CAN Bus Filter**

- Reused the same core hardware and extended the software.
- Implemented CAN acceptance filters and masks so only selected CAN IDs were processed.
- Demonstrated controller configuration beyond basic message capture.

3. **Arduino as ISP Programmer**

- Used an Arduino as an ISP programmer to program standalone ATmega328P chips.
- Built a CAN logger on the standalone ATmega328P instead of relying on an Arduino Uno board.
- Showed the move from development-board prototyping to a more complete embedded hardware setup.

## Workflow Summary

CAN Logger  
↓  
CAN Filtering  
↓  
Arduino as ISP Programming  
↓  
Standalone ATmega328P CAN Logger

Each revision builds on the previous one:

- **Revision 1:** verify CAN communication and serial logging.
- **Revision 2:** add filtering so only relevant CAN traffic is processed.
- **Revision 3:** remove dependence on the Arduino Uno by programming a standalone ATmega328P.

## Suggested CV Summary

**Embedded CAN Bus Development**

- Developed an Arduino-based CAN bus data logger using the MCP2515 CAN controller.
- Implemented CAN hardware acceptance filters and masks to selectively process CAN frames.
- Designed and programmed a standalone ATmega328P CAN logger using Arduino as ISP, eliminating dependence on a development board.
- Configured SPI communication, validated CAN data flow, and used serial debugging for hardware bring-up.

## Repository Structure

```text
CAN-logger/
├── CanFilter/
├── logger/
├── img/
└── README.md
```

## Notes

- Proteus and KiCad schematic files are not included yet.
- The circuit gallery at the top contains every image currently in `img/`.