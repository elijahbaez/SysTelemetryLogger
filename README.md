\# C++ System Telemetry Logger



A lightweight background utility built in C++ utilizing the Windows API to capture CPU/GPU sensor data and thermal metrics. 



\## Features

\* \*\*Low Overhead:\*\* Maintains under 2% CPU overhead during benchmarking.

\* \*\*Rolling Averages:\*\* Utilizes a custom circular buffer to compute rolling temperature averages.

\* \*\*Persistent Storage:\*\* Backed by a local SQLite storage layer polling at 1-second intervals.

