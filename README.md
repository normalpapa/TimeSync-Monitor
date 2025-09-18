# TimeSync Monitor

**TimeSync Monitor** is a C application designed to query public NTP servers and measure two key metrics:

- **Clock offset** – the difference between the local system clock and the server’s reference time
- **Network delay** – the round-trip latency of the time request

The application can operate in two modes:

- **Command-line utility** – executes a single query and outputs results in human-readable or JSON format
- **Background service (daemon)** – runs at defined intervals, logs measurements into an SQLite database, and provides access to recent results through a lightweight HTTP endpoint

---

### Project Purpose

This project was developed to demonstrate practical expertise in **C programming**, **networking (UDP/TCP/IP)**, **database integration**, and **system-level programming** on Unix-like platforms. It is inspired by production-grade time synchronization solutions such as _NTPD_ and _Chrony_, but implemented with a focus on simplicity and educational value.

---

### Technical Highlights

- Implemented in **C11**
- Communicates with NTP servers via **UDP sockets**
- Persists measurement data in **SQLite**
- Provides a **minimal HTTP interface** exposing JSON output
- Built and packaged with **CMake**
- Includes support for **testing and static analysis** with tools such as _Valgrind_ and _clang-tidy_

---

### Example

```bash
$ ./timesync pool.ntp.org
server=pool.ntp.org offset_ms=-12.456 delay_ms=23.109
```
