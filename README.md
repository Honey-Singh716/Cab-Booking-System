# Cab Booking System

This project has two parts:

- A C++ app you run in the terminal.
- A simple web page made with HTML, CSS, and JS.

You can use it to learn basic cab booking and simple route logic.

## Features

- Basic cab and booking management utilities
- Map/route logic in C++ (see `real_map_logic.*`)
- Browser-based UI with vanilla HTML/CSS/JS
- Prebuilt Windows binary (`cab_app.exe`) for quick run of the CLI app

## Project Structure

- `main.cpp` — Entry point for the C++ CLI program
- `cab_management.h/.cpp` — Cab and booking management utilities
- `real_map_logic.h/.cpp` — Core map/graph and routing logic
- `index.html` — Web UI markup
- `style.css` — Styles for the web UI
- `script.js` — Client-side logic for the web UI
- `cab_app.exe` — Prebuilt Windows executable of the CLI app (may be out of date; rebuild if needed)

## Getting Started

You can either:

- Run the browser UI by opening `index.html`, or
- Build and run the C++ command-line application.

### 1) Run the Browser UI

- Double-click `index.html` to open it in your browser.
- You can also use a small local server (sometimes browsers work better this way):
  - Python 3: `python -m http.server 8000` then open `http://localhost:8000/`
  - Node: `npx http-server` then open the shown link

No external dependencies are required for the static UI.

### 2) Build and Run the C++ App

What you need (pick one):

- GCC/MinGW (Windows) or GCC/Clang (Linux/macOS)
- Microsoft Visual C++ (MSVC) on Windows

Build (run this in the project folder):

Using g++ (GCC/MinGW/Clang):

```bash
g++ -std=c++17 -O2 -o cab_app.exe main.cpp cab_management.cpp real_map_logic.cpp
```

Using MSVC (Developer Command Prompt):

```cmd
cl /EHsc /std:c++17 /Fe:cab_app.exe main.cpp cab_management.cpp real_map_logic.cpp
```

Run:

```cmd
cab_app.exe
```

The app will show a simple menu (add cabs, create bookings, run routes, etc.).

## Notes

- If `cab_app.exe` does not run or seems old, rebuild it with the commands above.
- The web UI is static. It does not need a backend server.

## Development

- Change C++ files (`*.cpp`, `*.h`), then rebuild.
- For the web page, edit `index.html`, `style.css`, and `script.js`, then refresh the browser.

## Troubleshooting

- Build errors: Use a C++17 compiler and include all listed `.cpp` files in the build command.
- Crashes or wrong results: Add simple `print` lines in `main.cpp` or `script.js` to see what is happening.

## Team Acknowledgment

- Abhishek — Worked on core logic in C++ (`real_map_logic.cpp`). Managed cab state (available/booked), location updates, and helped with the technical write-up.
- Vedant — Built route display in the web app (`script.js`) using Leaflet.js and smooth animations. Handled map interactions and code walkthrough.
- Honey — Built nearest cab selection in the web app (`script.js`). Used OSRM table service for travel times, did linear search over cabs, and handled “no cabs” cases. Led the solution overview.
- Tejas — Set up the main app (`main.cpp`). Helped define the problem and kept the app running flow simple.

## License

No license is included yet. If you plan to share this project, add a license file (for example, MIT) in the root folder.

## Acknowledgments

- Built with C++ and plain HTML/CSS/JS.
