# wallpaper-addon

A Node.js native addon for Windows that allows you to set windows behind desktop icons, effectively creating live wallpapers or desktop overlays.

## Features

- Set any window behind desktop icons (as wallpaper)
- Get the WorkerW window handle
- Restore windows from behind desktop
- Validate window handles
- Uses the same proven logic as your original C++ implementation

## Prerequisites

- Windows operating system
- Node.js 12.0.0 or higher
- Visual Studio Build Tools or Visual Studio with C++ development tools
- Python (for node-gyp)

## Installation

```bash
npm install
```

This will automatically build the native addon using node-gyp.

## Usage

```javascript
const wallpaperAddon = require('wallpaper-addon');

// Get the WorkerW window handle
const workerHandle = wallpaperAddon.getWorkerWindow();
console.log('WorkerW handle:', workerHandle);

// Set a window behind desktop icons (windowHandle should be a string)
const windowHandle = "123456789"; // Replace with actual window handle
const success = wallpaperAddon.setWindowBehindDesktop(windowHandle);
console.log('Set behind desktop:', success);

// Restore window from behind desktop
const restored = wallpaperAddon.restoreWindow(windowHandle);
console.log('Restored:', restored);

// Check if window handle is valid
const isValid = wallpaperAddon.isValidWindow(windowHandle);
console.log('Is valid window:', isValid);
```

## API Reference

### `setWindowBehindDesktop(windowHandle)`

Sets a window behind desktop icons, making it appear as wallpaper.

- **Parameters:**
  - `windowHandle` (string): The window handle as a string
- **Returns:** boolean - `true` if successful, `false` otherwise

### `getWorkerWindow()`

Gets the WorkerW window handle that sits behind desktop icons.

- **Returns:** string|null - The WorkerW window handle as string, or `null` if not found

### `restoreWindow(windowHandle)`

Restores a window from behind desktop icons back to normal window behavior.

- **Parameters:**
  - `windowHandle` (string): The window handle as a string
- **Returns:** boolean - `true` if successful, `false` otherwise

### `isValidWindow(windowHandle)`

Checks if a window handle is valid and the window still exists.

- **Parameters:**
  - `windowHandle` (string): The window handle as a string
- **Returns:** boolean - `true` if valid, `false` otherwise

## How It Works

This addon uses the same logic as your original C++ code:

1. **Finds the Program Manager window** (`Progman`)
2. **Sends a special message** (0x052C) to spawn the WorkerW window behind desktop icons
3. **Locates the WorkerW window** using enumeration callbacks
4. **Sets the target window as a child** of WorkerW using `SetParent()`
5. **Positions and sizes the window** to fill the desktop area

The WorkerW window is a special Windows desktop window that sits between the desktop wallpaper and desktop icons, making it perfect for live wallpapers.

## Getting Window Handles

To get window handles for use with this addon, you can:

1. **Use Windows Spy++ tool** (included with Visual Studio)
2. **Use PowerShell:**
   ```powershell
   Get-Process | Where-Object {$_.MainWindowTitle -ne ""} | Select-Object ProcessName, Id, MainWindowHandle
   ```
3. **Use another Node.js addon** like `win32-api` or `ffi-napi` to enumerate windows
4. **From your own applications** - most GUI frameworks provide ways to get window handles

## Example with Electron

```javascript
// In your Electron main process
const { BrowserWindow } = require('electron');
const wallpaperAddon = require('wallpaper-addon');

// Create window
const win = new BrowserWindow({
  width: 800,
  height: 600,
  webPreferences: {
    nodeIntegration: true
  }
});

// Get window handle and set as wallpaper
const handle = win.getNativeWindowHandle().toString();
wallpaperAddon.setWindowBehindDesktop(handle);
```

## Building from Source

```bash
# Install dependencies
npm install

# Build the addon
npm run build

# Or rebuild
node-gyp rebuild
```

## Testing

```bash
npm test
```

## Troubleshooting

1. **Build errors:** Make sure you have Visual Studio Build Tools installed
2. **WorkerW not found:** This can happen on some Windows versions - try running as administrator
3. **Window not appearing:** Ensure the window handle is valid and the window exists
4. **Permission issues:** Some system windows cannot be manipulated - try with regular application windows

## Platform Support

- **Windows 7** and later
- **x64 and x86** architectures supported
- **Node.js 12.0.0+** required

## License

MIT License

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request