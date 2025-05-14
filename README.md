# Network Data Processor

## Project Overview

**Network Data Processor** is a school project designed to demonstrate network communication in C language. The program connects to a server, sends and receives data, and performs several operations on the received data, including XOR encryption, remainder calculation, and extracting characters at prime indices.

---

## Features

- **Network Server Connection:** Connects to a specified server and port.
- **Data Transmission:** Sends and receives messages in 17 iterations.
- **XOR Encryption:** Applies XOR encryption to the received data using the constant `XOR_KEY`.
- **Prime Index Extraction:** Outputs characters at prime indices of the received buffer.
- **Student ID Calculation:** Calculates the remainder from the sum of a student ID array.

---

## How It Works

1. The program initializes network communication using `WSAStartup`.
2. It creates a TCP socket and attempts to connect to the server (IP and port are specified by the user).
3. A loop of 17 iterations is executed, where:
    - A message is sent to the server.
    - A response is received from the server.
    - Based on the iteration, one of three special operations is performed (ID calculation, XOR encryption, prime index extraction).
4. The results of the operations are logged into the file `communication_log.txt`.